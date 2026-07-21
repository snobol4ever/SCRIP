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
  YYSYMBOL_scalar_methcall = 131,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 132,                 /* sub_body  */
  YYSYMBOL_method_body = 133,              /* method_body  */
  YYSYMBOL_class_decl = 134,               /* class_decl  */
  YYSYMBOL_role_decl = 135,                /* role_decl  */
  YYSYMBOL_is_clauses = 136,               /* is_clauses  */
  YYSYMBOL_class_body_list = 137,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 138,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 139,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 140,           /* named_arg_list  */
  YYSYMBOL_pair_list = 141,                /* pair_list  */
  YYSYMBOL_param_list = 142,               /* param_list  */
  YYSYMBOL_block = 143,                    /* block  */
  YYSYMBOL_closure = 144,                  /* closure  */
  YYSYMBOL_expr = 145,                     /* expr  */
  YYSYMBOL_tern_expr = 146,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 147,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 148,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 149,                 /* dor_expr  */
  YYSYMBOL_range_expr = 150,               /* range_expr  */
  YYSYMBOL_add_expr = 151,                 /* add_expr  */
  YYSYMBOL_repl_expr = 152,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 153,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 154,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 155,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 156,             /* postfix_expr  */
  YYSYMBOL_call_expr = 157,                /* call_expr  */
  YYSYMBOL_arg_list = 158,                 /* arg_list  */
  YYSYMBOL_atom = 159                      /* atom  */
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
#define YYLAST   3126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  117
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  324
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  819

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
     358,   360,   362,   364,   369,   373,   377,   382,   386,   387,
     389,   391,   393,   395,   398,   401,   404,   407,   410,   413,
     415,   417,   419,   421,   424,   427,   430,   434,   437,   440,
     443,   446,   449,   452,   455,   458,   461,   464,   467,   470,
     471,   472,   473,   474,   475,   476,   478,   480,   482,   484,
     485,   486,   487,   488,   489,   490,   493,   495,   497,   499,
     501,   503,   505,   507,   511,   513,   515,   517,   519,   521,
     525,   527,   531,   533,   535,   537,   541,   543,   547,   551,
     557,   563,   568,   574,   579,   584,   592,   603,   604,   610,
     618,   624,   630,   638,   644,   650,   659,   666,   675,   684,
     693,   700,   707,   708,   711,   714,   716,   718,   722,   723,
     724,   727,   730,   732,   734,   738,   754,   769,   770,   782,
     796,   797,   800,   803,   806,   809,   812,   815,   818,   821,
     824,   827,   830,   833,   840,   847,   854,   861,   865,   869,
     873,   877,   884,   891,   898,   905,   915,   923,   931,   940,
     947,   954,   964,   972,   982,   997,   998,  1003,  1008,  1015,
    1019,  1025,  1027,  1029,  1031,  1035,  1036,  1037,  1038,  1039,
    1040,  1041,  1042,  1045,  1046,  1047,  1049,  1051,  1053,  1057,
    1060,  1063,  1066,  1069,  1072,  1074,  1076,  1080,  1083,  1084,
    1089,  1092,  1094,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,  1105,  1106,  1107,  1113,  1119,  1125,  1128,  1129,  1130,
    1133,  1135,  1138,  1139,  1140,  1143,  1144,  1147,  1148,  1151,
    1152,  1153,  1156,  1157,  1158,  1159,  1160,  1163,  1164,  1165,
    1167,  1169,  1174,  1175,  1180,  1182,  1188,  1194,  1198,  1202,
    1207,  1214,  1219,  1225,  1231,  1238,  1245,  1250,  1255,  1262,
    1267,  1272,  1274,  1276,  1278,  1280,  1282,  1285,  1286,  1289,
    1290,  1291,  1292,  1300,  1301,  1302,  1303,  1304,  1308,  1312,
    1315,  1317,  1319,  1321,  1323,  1325,  1326,  1330,  1334,  1338,
    1339,  1340,  1342,  1345,  1346
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
  "for_stmt", "given_stmt", "when_list", "sub_decl", "scalar_methcall",
  "sub_body", "method_body", "class_decl", "role_decl", "is_clauses",
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

#define YYPACT_NINF (-618)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-324)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -618,    65,  1268,  -618,  -618,  -618,  -618,  -618,   339,   -41,
     -38,     5,   129,  -618,  -618,  -618,  -618,  -618,   111,  1329,
     383,  2291,  2351,  2411,  2471,  2531,    -6,     7,  2531,  1390,
     278,  2531,   148,   157,  2591,  2651,     7,    66,    66,  2711,
       7,     7,  2531,   176,   195,   123,  -618,   218,   235,   247,
    3011,  3011,  -618,  1451,   206,  -618,  -618,  -618,  -618,  -618,
    -618,  -618,  -618,  -618,    41,  -618,  -618,  -618,  1143,   324,
    -618,   396,   380,   192,  -618,   143,   205,   256,   268,  -618,
    -618,   199,   259,   376,   289,   323,  2531,  2531,  2531,  2531,
    2531,  2531,   431,  1511,  2531,   452,  2531,  2531,   135,  1571,
     393,   140,   363,   101,  -618,     7,  -618,  1631,  -618,  -618,
     229,   -59,   -33,   233,   498,   513,   390,  1451,   300,  1451,
      19,  1451,     7,  1451,     7,    -7,   243,   -72,  -618,  -618,
     412,  -618,   436,   464,   469,   461,   108,   124,  1451,     7,
    1451,     7,  -618,  2531,  2531,  2531,  1691,  2531,  -618,   546,
    -618,  -618,  -618,   487,   474,    54,   496,   515,   527,   520,
    -618,  -618,  -618,   307,   535,   797,  2531,  2531,  2531,  2531,
    2531,  -618,  2531,  2531,  2531,  2531,  2531,  -618,  3011,  3011,
    3011,  3011,  3011,  3011,  3011,  3011,  3011,   504,  3011,  3011,
    3011,  3011,  3011,  3011,  3011,  3011,  3011,  3011,  3011,  3011,
    3011,  3011,  3011,   627,   367,   109,  -618,  -618,   553,   564,
     567,   569,   570,   571,   202,  -618,   336,   568,   583,   572,
     574,   573,   671,   575,  -618,   348,  -618,  2531,  2531,   673,
    2531,   580,   307,   408,  -618,  2531,  2531,  -618,  2771,  -618,
    2831,  -618,   242,   266,   269,   320,   597,   598,   413,  2531,
    2531,  2531,  2531,  -618,  2531,   414,  2531,  2531,  -618,  2531,
     421,   455,   422,  -618,   684,  2531,  -618,  3011,  3011,     4,
    -618,  -618,  -618,  -618,  2531,  2531,  -618,   682,  2531,   685,
    2531,   438,   674,   441,  -618,   586,  -618,  -618,  -618,     7,
      44,  -618,    14,   593,   595,  -618,  -618,  -618,  1751,  -618,
    -618,   411,   599,   144,   612,  2291,  2351,  2531,   594,  -618,
     201,   602,   604,   605,   606,   607,   611,   613,   614,   615,
     616,   405,   405,   628,   405,   405,   405,   405,   405,   405,
    -618,  -618,  -618,   405,   405,  -618,  -618,  -618,   620,   620,
     205,   256,   268,   268,  -618,  -618,  -618,  -618,   617,   618,
     712,  -618,  1811,  -618,  -618,  -618,  -618,  -618,  -618,  2531,
    1871,  -618,   632,   635,   636,  -618,  1931,  -618,  1991,  -618,
    -618,   619,   637,   621,  -618,   625,  -618,   629,  1451,   246,
     669,   -54,  2051,   329,   630,  2531,  -618,  2891,  -618,  2531,
    -618,    20,  -618,  2531,  2531,  1751,   634,   639,   640,   641,
     458,  1751,   643,   644,   545,     7,   -11,  2951,  -618,     7,
       7,   154,    29,   114,  -618,    36,   626,   442,   918,   645,
     646,   -26,   647,   624,   649,   648,     7,     7,     7,  -618,
    -618,   728,   730,  -618,    23,   626,   445,    21,   430,    15,
    -618,   446,   742,  2531,  2531,  2531,    94,   151,    18,  -618,
      73,  -618,  -618,  -618,  -618,  -618,  -618,  -618,  -618,  -618,
    -618,  -618,  3011,  2111,  2171,  -618,   650,   450,   651,  -618,
     451,  2531,  2531,  2531,  -618,   462,     2,  -618,   463,   467,
    -618,  -618,  -618,  -618,  -618,   470,  2531,  -618,  2531,  2531,
    2531,   471,  -618,    59,  -618,   652,  1451,   547,   653,   626,
     479,   654,   656,   480,  -618,  -618,  -618,  -618,  -618,   483,
    -618,  -618,  -618,   631,  -618,  -618,  1451,     7,  -618,  -618,
     747,  -618,   748,   756,  -618,   757,   758,  -618,   447,   626,
    2291,  2351,  1093,  -618,   292,  -618,  -618,  2531,     7,  -618,
    -618,  -618,   667,   668,   751,  -618,  -618,  -618,  -618,   272,
      38,   476,   729,  -618,  -618,   626,   626,   491,   506,   765,
     767,   768,  -618,  -618,   220,   670,   672,   180,  -618,  -618,
    -618,  -618,  -618,  -618,   507,  -618,   510,   511,  -618,   676,
    -618,  -618,   678,   680,   683,  -618,  2531,   769,  -618,  -618,
    1751,   686,   551,  -618,  -618,   687,   724,   726,  -618,   517,
    -618,  2531,  -618,  -618,   626,  -618,  -618,   688,   689,   -11,
    -618,   518,   638,     7,     7,     7,  -618,  -618,  -618,    46,
    -618,   165,   236,  -618,   270,  -618,     7,   690,  -618,  -618,
       7,  -618,   357,   370,     3,   161,   449,   695,   696,   780,
    -618,  -618,   626,   626,   792,   807,   817,  2531,  2231,   753,
     754,  -618,  -618,  -618,  -618,  -618,  -618,  -618,  -618,  -618,
     783,   522,  -618,  -618,  -618,  2531,  2531,  1751,   558,  -618,
    -618,  -618,  -618,  -618,     7,     7,  -618,  -618,  -618,  -618,
    -618,   815,   843,  -618,  -618,  -618,  -618,  -618,  -618,    28,
     793,  -618,    34,  -618,   840,   844,   750,  2531,  -618,   847,
     848,   755,  2531,  -618,   171,   200,   759,   760,  -618,  -618,
     392,  -618,  -618,  -618,  -618,  -618,   273,   741,   525,  2531,
    2531,  2531,   761,  -618,  -618,   532,  -618,   642,  -618,  -618,
    -618,   752,   536,   749,  1032,   752,   537,   762,   763,  -618,
     764,   766,   770,  -618,   771,   849,   852,   774,  2531,  -618,
     860,   862,   775,  2531,  -618,  -618,  -618,    39,  -618,  -618,
    -618,   772,   287,   304,  -618,  -618,   776,     7,  -618,  -618,
     752,  -618,  2291,  2351,  1154,  -618,   306,  -618,   752,  -618,
    -618,  -618,  -618,  -618,  -618,   777,   778,  -618,   781,   782,
     786,  -618,   787,   752,   540,  -618,  -618,  -618,  -618,  -618,
    -618,   261,   253,  -618,   321,  -618,  -618,  -618,  -618,  -618,
    -618,  -618,  -618,  -618,   752,  -618,  -618,  -618,  -618
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   299,   300,   301,   303,   304,   305,
     306,   316,   315,   317,   318,   307,   308,   309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   302,     0,     0,     0,
       0,     0,    80,     0,     3,     4,    81,    82,    89,    90,
      91,    83,    84,    92,     0,    93,    94,    95,    88,     0,
     230,   232,   246,   249,   251,   254,   256,   258,   261,   266,
     269,   270,   296,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,   305,   306,   316,     0,    29,     0,   323,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   254,     0,   324,   229,
       0,    41,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,     0,     0,     0,     3,     0,   294,    85,
      87,   291,   157,     0,     0,     0,     0,     0,     0,   304,
     268,   267,   319,     0,     0,     0,     0,     0,     0,     0,
       0,    48,     0,     0,     0,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,     0,     0,
       0,     0,     0,   228,     0,   274,     0,     0,     0,     0,
       0,   279,     0,   278,   272,     0,    24,     0,     0,     0,
       0,   319,     0,     0,    32,     0,     0,     6,     0,     7,
       0,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,     0,     0,     0,     0,    36,     0,
       0,    99,     0,   111,     0,     0,   124,     0,     0,     0,
       3,   131,    39,    40,     0,     0,   127,     0,     0,     0,
       0,     0,   114,     0,   117,     0,   292,   293,   295,     0,
       0,   160,     0,     0,     0,   195,    77,    78,     0,   320,
     214,   304,   305,   306,   316,     0,     0,     0,     0,   213,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   233,   234,     0,   235,   236,   239,   240,   241,   242,
     243,   244,   245,   237,   238,   247,   248,   250,   252,   253,
     255,   257,   259,   260,   265,   262,   263,   264,   290,   287,
       0,    45,     0,    70,    71,    72,    73,    74,    42,     0,
       0,   273,   310,   311,   312,    47,     0,   282,     0,   271,
     228,     0,     0,     0,    31,     0,   298,     0,     0,     0,
     301,   315,     0,     0,     0,     0,    21,     0,    22,     0,
      23,     0,   134,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,     0,   102,   320,
       0,     0,   252,   253,   205,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   320,     0,   320,   227,
      86,     0,     0,   160,     0,     0,     0,     0,     0,     0,
     321,     0,     0,     0,     0,     0,     0,     0,     0,   226,
       0,   215,    49,    51,    53,    50,    52,    59,    61,    63,
      60,    62,     0,     0,     0,   283,     0,     0,     0,   141,
       0,     0,     0,     0,   281,     0,   315,   277,     0,     0,
     310,   311,   312,    30,     5,     0,     0,     9,     0,     0,
       0,     0,    14,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    67,    66,    65,    34,     0,
      68,    69,    37,    96,   101,   100,     0,     0,   110,   121,
       0,   123,     0,     0,   206,     0,     0,   130,     0,     0,
       0,     0,     0,   142,     0,    26,    25,     0,     0,   125,
     313,   314,     0,     0,   112,   115,   116,   158,   159,     0,
       0,     0,     0,   156,   139,     0,     0,     0,     0,     0,
       0,     0,   194,   322,     0,     0,     0,     0,   216,   217,
     224,   225,   231,   289,     0,   286,     0,     0,    44,     0,
      46,   140,     0,     0,     0,   280,     0,     0,   275,   276,
       0,     0,     0,   202,   201,     0,     0,     0,    16,     0,
      17,     0,    20,   133,     0,    28,    27,   322,   322,     0,
     103,     0,   104,     0,     0,     0,   207,   208,   209,     0,
     129,     0,     0,   145,     0,   143,     0,     0,    57,    58,
       0,   155,     0,     0,     0,     0,     0,     0,     0,     0,
     138,   136,     0,     0,     0,     0,     0,     0,     0,   310,
     312,   221,   288,   285,   284,    43,    54,    55,    56,   199,
       0,     0,    11,    10,    15,     0,     0,     0,     0,   132,
      35,    38,    98,    97,   320,     0,   108,   122,   119,   120,
     210,     0,     0,   146,   147,   144,   128,   126,   113,     0,
       3,   187,     0,   190,     0,     0,     0,     0,   166,     0,
       0,     0,     0,   161,     0,     0,     0,     0,   162,   163,
       0,   135,   137,   196,   197,   198,     0,   141,     0,     0,
       0,     0,   322,   204,   203,     0,    18,   105,   106,   211,
     212,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,   178,     0,     0,     0,     0,     0,   168,
       0,     0,     0,     0,   167,   164,   165,     0,   193,   220,
     219,   140,     0,     0,   200,    12,   322,     0,   109,   186,
       0,   149,     0,     0,     0,   148,     0,   189,     0,   175,
     176,   170,   173,   174,   169,     0,     0,   179,     0,     0,
       0,   180,     0,     0,     0,   218,   222,   223,    19,   107,
     185,     0,     0,   152,     0,   150,   188,   183,   184,   172,
     181,   182,   171,   192,     0,   153,   154,   151,   191
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -618,  -618,     1,  -618,  -401,  -510,  -618,  -618,  -618,  -618,
    -618,  -618,  -618,  -618,  -618,  -236,  -617,  -618,  -618,  -618,
     444,  -618,  -618,   425,   516,  -290,   160,   633,    -2,  -176,
    -618,   358,  -618,   325,   -15,   697,   701,   477,   -44,  -618,
    -618,   -52,  -618
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   165,    55,    56,   408,    57,    58,    59,    60,
      61,    62,   421,    63,    64,   271,   691,    65,    66,   290,
     434,    67,   439,   478,   383,   417,   108,   144,   109,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,   110,    82
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      69,     2,   436,   610,   323,   514,   160,   161,   127,   392,
     126,   537,   538,   490,   414,    23,   693,   694,   415,   118,
     120,   122,   124,   125,   414,   695,   130,   132,   415,   135,
     414,   414,   139,   141,   415,   415,   236,   148,   414,   269,
     151,   216,   415,   270,   414,   256,   524,   225,   415,   414,
     237,   163,   632,   415,    98,   233,   680,    99,   431,    95,
     257,   696,   238,   264,   596,     3,   432,   166,   293,   586,
     167,   168,    94,   597,   550,   551,   239,    96,   552,   559,
     560,   561,   169,   170,   208,   209,   210,   211,   212,   213,
     539,   633,   217,   758,   219,   220,   525,   526,   697,   522,
      97,   500,   676,   265,    54,   232,   681,   682,    54,    54,
      98,   294,   698,    99,   769,   248,   416,   255,   777,   260,
     249,   262,    54,   250,   251,   100,   435,   272,   258,   259,
     195,   562,   499,   556,   570,   252,   281,   154,   283,   553,
     731,   285,   286,   287,   285,   288,   735,   557,   558,   221,
     171,   793,   222,   800,   155,   344,   345,   346,   347,   433,
     136,   806,    68,   310,   311,   312,   313,   314,   315,   137,
     316,   317,   318,   319,   320,   699,   813,   256,   338,   339,
     527,   143,   273,   700,   523,   745,   128,   129,   223,   571,
     152,   249,   257,   746,   250,   251,   142,   818,   229,   554,
     149,   150,   400,   253,   254,   277,   252,   404,   672,   153,
     568,   193,   194,   411,   750,   195,   230,   768,   351,   701,
     352,   279,   751,   278,   520,   370,   371,   172,   373,   747,
     173,   174,   156,   376,   377,   227,   379,    98,   384,   280,
      99,    95,   175,   176,   195,   157,   441,   396,   397,   398,
     399,    93,   412,   413,   402,   403,   702,   158,   752,   444,
     258,   259,   256,   603,   235,   128,   748,   569,   164,    54,
     703,   418,   419,   420,   253,   254,   423,   257,   425,   256,
     749,   683,   261,   192,   263,   266,   572,   249,   133,   365,
     250,   251,   134,   620,   257,   753,   651,   359,   196,   282,
     467,   284,   252,   446,   447,   448,   450,   203,   470,   754,
     177,   267,   268,   360,   475,   647,   479,   451,   172,   640,
     641,   173,   174,   550,   551,    68,   249,   552,   240,   250,
     251,   648,   172,   175,   176,   173,   174,   385,   234,   235,
     486,   252,   241,   503,   195,   258,   259,   175,   176,   509,
     172,   386,   684,   173,   174,   487,   488,   468,   197,   198,
     199,   387,   258,   259,   389,   175,   176,   204,   669,   816,
     253,   254,   200,   201,   202,   388,   485,   815,   390,   273,
     163,   349,   580,   495,   350,   497,   685,   498,   631,   759,
     205,   501,   502,   111,   112,   113,   656,   114,   206,   732,
     246,   177,   736,   796,   247,   517,   711,   712,   625,   253,
     254,   574,   577,   658,   115,   177,   534,   298,   116,   299,
     797,    83,   805,    84,    85,    86,    87,    88,    89,    90,
     273,   391,   207,   177,    91,   270,   592,   817,   492,   493,
     414,   565,   566,   567,   415,   214,   235,    92,   361,   430,
      93,   181,   182,   183,   184,   185,   186,   618,   235,   704,
     369,   619,   705,   187,   706,   707,   218,   794,   689,   582,
     583,   584,   690,   178,   179,   180,   228,   188,   189,   190,
     191,   692,   406,   407,   591,   690,   634,   593,   594,   635,
     636,   637,   638,    83,   599,    84,    85,    86,    87,    88,
      89,    90,   226,   757,   190,   191,    91,   690,   242,   243,
     244,   330,   331,   332,   611,   335,   336,   337,   235,   442,
     375,   272,    93,   395,   401,   299,   299,   245,   621,   622,
     624,   298,   298,   405,   409,   626,   321,   322,   661,   324,
     325,   326,   327,   328,   329,   273,   333,   334,   298,   668,
     426,   298,   528,   428,   529,   528,   235,   555,   563,   274,
     235,   235,   579,   581,   275,   513,   515,   508,   235,   518,
     519,   521,   235,   587,   585,   588,   276,   235,    68,   589,
     590,   493,   299,   595,   659,   292,   544,   545,   546,   528,
     235,   604,   607,   235,   289,   608,   718,     4,     5,     6,
       7,   528,   291,   642,   101,   102,   103,   104,    12,    13,
      14,   295,    15,    16,    17,   725,   528,   235,   643,   652,
     587,   235,   653,   654,   296,   105,    27,   667,   298,   299,
     674,    93,   235,    32,   722,   235,   297,   761,    37,    38,
      39,   348,   235,    42,   766,   716,   528,   528,   770,   778,
     528,   300,   814,    46,   512,   235,   600,   601,   609,   407,
     663,   235,   353,   723,   724,   675,   407,   726,   235,   767,
     407,   145,   147,   354,   342,   343,   355,   612,   356,   357,
     358,   363,   362,   365,   366,   367,   368,   372,   364,   374,
      50,   734,   393,   394,   410,   740,   422,    51,   627,   424,
     744,   427,   429,   131,   437,    53,   438,   445,   462,    54,
     449,   452,   443,   453,   454,   455,   456,   762,   763,   764,
     457,   195,   458,   459,   460,   461,   465,   471,   463,   464,
     472,   473,   776,   480,   483,   481,   489,   482,   484,   494,
     541,   270,   547,   504,   548,   540,   788,   542,   505,   506,
     507,   792,   510,   511,   535,   536,   564,   613,   614,   578,
     580,   598,   602,   605,   543,   606,   615,   616,   617,   673,
     801,   802,   804,   677,   678,   679,   628,   629,   630,   644,
     639,   645,   646,   660,   649,   655,   686,   656,   650,   657,
     688,   665,   658,   666,   710,   662,   664,   670,   671,   713,
       4,     5,     6,     7,   708,   709,   687,   301,   302,   303,
     304,    12,    13,    14,   714,    15,    16,    17,    18,    19,
      20,   305,   306,    23,   715,   729,    24,    25,    26,    27,
     307,   308,    30,    31,   727,   728,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,   719,   720,
     721,    44,    45,   730,   737,   733,    46,   760,   738,   739,
      47,   741,   742,   785,   743,   771,   786,   690,   755,   756,
     765,   779,   780,   781,   789,   782,   790,   549,     0,   783,
     784,    48,    49,   787,   791,   798,   807,   808,   795,   576,
     809,   810,   340,    50,    68,   811,   812,   341,   491,     0,
      51,     0,     0,     0,     0,     0,    52,     0,    53,     0,
       0,     0,    54,   309,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,   799,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,    17,    18,
      19,    20,   530,   531,    23,     0,     0,    24,    25,    26,
      27,    28,   532,    30,    31,     0,     0,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     0,
       0,     0,    44,    45,     0,     0,     0,    46,     0,     0,
       0,    47,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,    52,     0,    53,
       0,     0,     0,    54,   533,     4,     5,     6,     7,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,     0,
      15,    16,    17,    18,    19,    20,   772,   773,    23,     0,
       0,    24,    25,    26,    27,    28,   774,    30,    31,     0,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,     0,     0,     0,    44,    45,     0,     0,
       0,    46,     0,     0,     0,    47,     4,     5,     6,     7,
       0,     0,     0,   101,   102,   103,   104,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    48,    49,     0,     0,
       0,     0,     0,     0,   105,    27,     0,     0,    50,     0,
       0,     0,    32,     0,     0,    51,     0,    37,    38,    39,
       0,    52,    42,    53,     0,     0,     0,    54,   775,     0,
       0,     0,    46,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   101,   102,   103,   104,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,    27,     0,     0,    50,
       0,     0,     0,    32,     0,     0,    51,     0,    37,    38,
      39,     0,   131,    42,    53,     0,     0,     0,    54,   623,
       0,  -323,  -323,    46,  -323,  -323,  -323,  -323,  -323,  -323,
    -323,  -323,  -323,     0,     0,     0,  -323,     0,     0,     0,
       0,     0,     0,     0,  -323,  -323,  -323,     0,     0,     0,
    -323,  -323,  -323,  -323,  -323,  -323,     0,  -323,  -323,  -323,
      50,  -323,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,   131,     0,    53,     0,     0,     0,    54,
     803,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,     0,    15,    16,    17,    18,
      19,    20,    21,    22,    23,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,     0,     0,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,     0,
       0,     0,    44,    45,     0,     0,     0,    46,     0,     0,
       0,    47,     4,     5,     6,     7,     0,     0,     0,   101,
     102,   103,   104,    12,    13,    14,     0,    15,    16,    17,
       0,     0,    48,    49,     0,     0,     0,     0,     0,     0,
     105,    27,     0,     0,    50,     0,     0,     0,    32,     0,
       0,    51,     0,    37,    38,    39,     0,    52,    42,    53,
       0,     0,     0,    54,     0,     0,     0,     0,    46,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     101,   102,   103,   104,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,    27,     0,     0,    50,     0,     0,     0,    32,
       0,     0,    51,     0,    37,    38,    39,     0,   106,    42,
     107,     0,     0,     0,    54,     0,     0,     0,     0,    46,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,    50,     0,     0,     0,
      32,     0,     0,    51,     0,    37,    38,    39,     0,   131,
      42,    53,     0,     0,     0,    54,     0,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   162,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   215,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   224,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   231,     0,     0,    54,     0,     0,     0,
      46,     0,     0,   164,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   440,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   466,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   469,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,   476,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   474,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,   380,     7,     0,     0,
       0,   101,   102,   103,   104,   381,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   477,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   162,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,   476,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   573,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   575,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,   717,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   117,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   119,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   121,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   123,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   138,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   140,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,    53,     0,     0,     0,   146,     0,     0,     0,
      46,     0,     0,     0,     4,     5,   380,     7,     0,     0,
       0,   101,   102,   103,   104,   381,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   378,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   382,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   101,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,    27,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   496,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   159,   102,   103,   104,    12,    13,    14,     0,    15,
      16,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,     0,     0,     0,     0,    50,     0,     0,
      32,     0,     0,     0,    51,    37,    38,    39,     0,     0,
      42,     0,   516,     0,     0,     0,    54,     0,     0,     0,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,    53,     0,     0,     0,    54
};

static const yytype_int16 yycheck[] =
{
       2,     0,   292,   513,   180,   406,    50,    51,    14,   245,
      25,    37,    38,    67,    10,    26,   633,    14,    14,    21,
      22,    23,    24,    25,    10,    22,    28,    29,    14,    31,
      10,    10,    34,    35,    14,    14,    95,    39,    10,   111,
      42,    93,    14,   115,    10,    26,    10,    99,    14,    10,
     109,    53,    14,    14,   108,   107,    10,   111,    14,    97,
      41,    58,    95,    70,     5,     0,    22,    26,    14,    67,
      29,    30,   113,    14,    51,    52,   109,   115,    55,    64,
      65,    66,    41,    42,    86,    87,    88,    89,    90,    91,
     116,    53,    94,   710,    96,    97,    60,    61,    95,    70,
      95,   391,   612,   110,   115,   107,    60,    61,   115,   115,
     108,    57,   109,   111,   731,   117,   112,   119,   735,   121,
      26,   123,   115,    29,    30,    14,   112,   109,   109,   110,
     101,   116,   112,   112,   116,    41,   138,    14,   140,   116,
     112,   143,   144,   145,   146,   147,   112,   437,   438,    14,
     109,   112,    17,   770,    31,   199,   200,   201,   202,   115,
      12,   778,     2,   165,   166,   167,   168,   169,   170,    12,
     172,   173,   174,   175,   176,    14,   793,    26,   193,   194,
     416,   115,   109,    22,    70,    14,    26,    27,    53,   116,
      14,    26,    41,    22,    29,    30,    36,   814,    97,   435,
      40,    41,   254,   109,   110,    97,    41,   259,   609,    14,
     116,    68,    69,   265,    14,   101,   115,   727,   109,    58,
     111,    97,    22,   115,    70,   227,   228,    26,   230,    58,
      29,    30,    14,   235,   236,    95,   238,   108,   240,   115,
     111,    97,    41,    42,   101,    10,   298,   249,   250,   251,
     252,   111,   267,   268,   256,   257,    95,    10,    58,   115,
     109,   110,    26,   499,   110,   105,    95,   116,    62,   115,
     109,   270,   274,   275,   109,   110,   278,    41,   280,    26,
     109,   116,   122,    91,   124,   125,   462,    26,    10,   109,
      29,    30,    14,   529,    41,    95,   116,    95,    93,   139,
     352,   141,    41,   305,   306,   307,   308,   108,   360,   109,
     109,    68,    69,   111,   366,    95,   368,   116,    26,   555,
     556,    29,    30,    51,    52,   165,    26,    55,    95,    29,
      30,   111,    26,    41,    42,    29,    30,    95,   109,   110,
      94,    41,   109,   395,   101,   109,   110,    41,    42,   401,
      26,   109,   116,    29,    30,   109,   110,   359,   102,   103,
      92,    95,   109,   110,    95,    41,    42,   108,   604,   116,
     109,   110,   104,   105,   106,   109,   378,   116,   109,   109,
     382,    14,   109,   385,    17,   387,   116,   389,   116,   116,
      14,   393,   394,    10,    11,    12,   109,    14,   109,   689,
      10,   109,   692,   116,    14,   407,   642,   643,   116,   109,
     110,   463,   464,   109,    31,   109,   418,   110,    35,   112,
     116,    82,   116,    84,    85,    86,    87,    88,    89,    90,
     109,   111,   109,   109,    95,   115,   488,   116,   109,   110,
      10,   443,   444,   445,    14,    14,   110,   108,   112,   289,
     111,    71,    72,    73,    74,    75,    76,    10,   110,    10,
     112,    14,    13,    83,    15,    16,    14,   757,   111,   471,
     472,   473,   115,    77,    78,    79,   113,    97,    98,    99,
     100,   111,    27,    28,   486,   115,    10,   489,   490,    13,
      14,    15,    16,    82,   496,    84,    85,    86,    87,    88,
      89,    90,   109,   111,    99,   100,    95,   115,    10,    11,
      12,     7,     8,     9,   516,   190,   191,   192,   110,   108,
     112,   109,   111,   110,   110,   112,   112,    14,   530,   531,
     532,   110,   110,   112,   112,   537,   178,   179,   590,   181,
     182,   183,   184,   185,   186,   109,   188,   189,   110,   601,
     112,   110,   110,   112,   112,   110,   110,   112,   112,    95,
     110,   110,   112,   112,    95,   405,   406,   109,   110,   409,
     410,   411,   110,   110,   112,   112,   115,   110,   418,   112,
     110,   110,   112,   112,   586,   111,   426,   427,   428,   110,
     110,   112,   112,   110,    48,   112,   648,     3,     4,     5,
       6,   110,   115,   112,    10,    11,    12,    13,    14,    15,
      16,   115,    18,    19,    20,   667,   110,   110,   112,   112,
     110,   110,   112,   112,   109,    31,    32,   110,   110,   112,
     112,   111,   110,    39,   112,   110,   109,   112,    44,    45,
      46,    14,   110,    49,   112,   647,   110,   110,   112,   112,
     110,   116,   112,    59,   109,   110,   109,   110,    27,    28,
     109,   110,   109,   665,   666,    27,    28,   109,   110,    27,
      28,    38,    39,   109,   197,   198,   109,   517,   109,   109,
     109,    98,   114,   109,   111,    14,   111,    14,   116,   109,
      96,   690,    95,    95,    10,   697,    14,   103,   538,    14,
     702,    27,   116,   109,   111,   111,   111,    95,    80,   115,
     116,   109,   113,   109,   109,   109,   109,   719,   720,   721,
     109,   101,   109,   109,   109,   109,    14,    95,   111,   111,
      95,    95,   734,   114,   109,    98,    67,   116,   109,   109,
     116,   115,    14,   109,    14,    98,   748,    98,   109,   109,
     109,   753,   109,   109,   109,   109,    14,    10,    10,   109,
     109,   109,   109,   109,   116,   109,    10,    10,    10,   609,
     772,   773,   774,   613,   614,   615,   109,   109,    27,    14,
      51,    14,    14,    14,   114,   109,   626,   109,   116,   109,
     630,    67,   109,    67,    14,   109,   109,   109,   109,     7,
       3,     4,     5,     6,   109,   109,   116,    10,    11,    12,
      13,    14,    15,    16,     7,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     7,    10,    29,    30,    31,    32,
      33,    34,    35,    36,   674,   675,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    95,    95,
      67,    54,    55,    10,    14,    62,    59,   116,    14,   109,
      63,    14,    14,    14,   109,   116,    14,   115,   109,   109,
     109,   109,   109,   109,    14,   109,    14,   433,    -1,   109,
     109,    84,    85,   109,   109,   109,   109,   109,   116,   464,
     109,   109,   195,    96,   734,   109,   109,   196,   382,    -1,
     103,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,    -1,
      -1,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,   767,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    -1,   109,    -1,   111,
      -1,    -1,    -1,   115,   116,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    54,    55,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    63,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    -1,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    96,    -1,
      -1,    -1,    39,    -1,    -1,   103,    -1,    44,    45,    46,
      -1,   109,    49,   111,    -1,    -1,    -1,   115,   116,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    96,
      -1,    -1,    -1,    39,    -1,    -1,   103,    -1,    44,    45,
      46,    -1,   109,    49,   111,    -1,    -1,    -1,   115,   116,
      -1,    68,    69,    59,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    91,    92,    93,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,   106,
      96,   108,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    -1,   109,    -1,   111,    -1,    -1,    -1,   115,
     116,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    -1,
      -1,    -1,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    63,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    -1,    18,    19,    20,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    96,    -1,    -1,    -1,    39,    -1,
      -1,   103,    -1,    44,    45,    46,    -1,   109,    49,   111,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    59,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    96,    -1,    -1,    -1,    39,
      -1,    -1,   103,    -1,    44,    45,    46,    -1,   109,    49,
     111,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    96,    -1,    -1,    -1,
      39,    -1,    -1,   103,    -1,    44,    45,    46,    -1,   109,
      49,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    62,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      39,    -1,    -1,    -1,   103,    44,    45,    46,    -1,    -1,
      49,    -1,   111,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    -1,   115
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
     126,   127,   128,   130,   131,   134,   135,   138,   143,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   159,    82,    84,    85,    86,    87,    88,    89,
      90,    95,   108,   111,   113,    97,   115,    95,   108,   111,
      14,    10,    11,    12,    13,    31,   109,   111,   143,   145,
     158,    10,    11,    12,    14,    31,    35,   111,   145,   111,
     145,   111,   145,   111,   145,   145,   151,    14,   143,   143,
     145,   109,   145,    10,    14,   145,    12,    12,   111,   145,
     111,   145,   143,   115,   144,   144,   115,   144,   145,   143,
     143,   145,    14,    14,    14,    31,    14,    10,    10,    10,
     155,   155,   112,   145,    62,   119,    26,    29,    30,    41,
      42,   109,    26,    29,    30,    41,    42,   109,    77,    78,
      79,    71,    72,    73,    74,    75,    76,    83,    97,    98,
      99,   100,    91,    68,    69,   101,    93,   102,   103,    92,
     104,   105,   106,   108,   108,    14,   109,   109,   145,   145,
     145,   145,   145,   145,    14,   112,   158,   145,    14,   145,
     145,    14,    17,    53,   112,   158,   109,    95,   113,    97,
     115,   112,   145,   158,   109,   110,    95,   109,    95,   109,
      95,   109,    10,    11,    12,    14,    10,    14,   145,    26,
      29,    30,    41,   109,   110,   145,    26,    41,   109,   110,
     145,   143,   145,   143,    70,   110,   143,    68,    69,   111,
     115,   132,   109,   109,    95,    95,   115,    97,   115,    97,
     115,   145,   143,   145,   143,   145,   145,   145,   145,    48,
     136,   115,   111,    14,    57,   115,   109,   109,   110,   112,
     116,    10,    11,    12,    13,    24,    25,    33,    34,   116,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   148,   148,   146,   148,   148,   148,   148,   148,   148,
       7,     8,     9,   148,   148,   150,   150,   150,   151,   151,
     152,   153,   154,   154,   155,   155,   155,   155,    14,    14,
      17,   109,   111,   109,   109,   109,   109,   109,   109,    95,
     111,   112,   114,    98,   116,   109,   111,    14,   111,   112,
     145,   145,    14,   145,   109,   112,   145,   145,   111,   145,
       5,    14,   111,   141,   145,    95,   109,    95,   109,    95,
     109,   111,   132,    95,    95,   110,   145,   145,   145,   145,
     158,   110,   145,   145,   158,   112,    27,    28,   122,   112,
      10,   158,   151,   151,    10,    14,   112,   142,   119,   145,
     145,   129,    14,   145,    14,   145,   112,    27,   112,   116,
     143,    14,    22,   115,   137,   112,   142,   111,   111,   139,
     112,   158,   108,   113,   115,    95,   145,   145,   145,   116,
     145,   116,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,    80,   111,   111,    14,   112,   158,   145,   112,
     158,    95,    95,    95,   112,   158,    14,   112,   140,   158,
     114,    98,   116,   109,   109,   145,    94,   109,   110,    67,
      67,   141,   109,   110,   109,   145,   111,   145,   145,   112,
     142,   145,   145,   158,   109,   109,   109,   109,   109,   158,
     109,   109,   109,   143,   121,   143,   111,   145,   143,   143,
      70,   143,    70,    70,    10,    60,    61,   132,   110,   112,
      24,    25,    34,   116,   145,   109,   109,    37,    38,   116,
      98,   116,    98,   116,   143,   143,   143,    14,    14,   137,
      51,    52,    55,   116,   132,   112,   112,   142,   142,    64,
      65,    66,   116,   112,    14,   145,   145,   145,   116,   116,
     116,   116,   146,   112,   158,   112,   140,   158,   109,   112,
     109,   112,   145,   145,   145,   112,    67,   110,   112,   112,
     110,   145,   158,   145,   145,   112,     5,    14,   109,   145,
     109,   110,   109,   132,   112,   109,   109,   112,   112,    27,
     122,   145,   143,    10,    10,    10,    10,    10,    10,    14,
     132,   145,   145,   116,   145,   116,   145,   143,   109,   109,
      27,   116,    14,    53,    10,    13,    14,    15,    16,    51,
     132,   132,   112,   112,    14,    14,    14,    95,   111,   114,
     116,   116,   112,   112,   112,   109,   109,   109,   109,   145,
      14,   158,   109,   109,   109,    67,    67,   110,   158,   132,
     109,   109,   121,   143,   112,    27,   122,   143,   143,   143,
      10,    60,    61,   116,   116,   116,   143,   116,   143,   111,
     115,   133,   111,   133,    14,    22,    58,    95,   109,    14,
      22,    58,    95,   109,    10,    13,    15,    16,   109,   109,
      14,   132,   132,     7,     7,     7,   145,   112,   158,    95,
      95,    67,   112,   145,   145,   158,   109,   143,   143,    10,
      10,   112,   142,    62,   119,   112,   142,    14,    14,   109,
     145,    14,    14,   109,   145,    14,    22,    58,    95,   109,
      14,    22,    58,    95,   109,   109,   109,   111,   133,   116,
     116,   112,   145,   145,   145,   109,   112,    27,   122,   133,
     112,   116,    24,    25,    34,   116,   145,   133,   112,   109,
     109,   109,   109,   109,   109,    14,    14,   109,   145,    14,
      14,   109,   145,   112,   142,   116,   116,   116,   109,   143,
     133,   145,   145,   116,   145,   116,   133,   109,   109,   109,
     109,   109,   109,   133,   112,   116,   116,   116,   133
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
     120,   120,   120,   120,   120,   120,   121,   121,   121,   121,
     121,   121,   121,   121,   122,   122,   122,   122,   122,   122,
     123,   123,   124,   124,   124,   124,   125,   125,   126,   127,
     127,   127,   127,   127,   127,   128,   128,   129,   129,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     131,   131,   132,   132,   132,   132,   132,   132,   133,   133,
     133,   133,   133,   133,   133,   134,   135,   136,   136,   136,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   139,   139,   139,   139,   140,
     140,   141,   141,   141,   141,   142,   142,   142,   142,   142,
     142,   142,   142,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   144,   145,   145,
     145,   146,   146,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   148,   148,   148,
     149,   149,   150,   150,   150,   151,   151,   152,   152,   153,
     153,   153,   154,   154,   154,   154,   154,   155,   155,   155,
     156,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     3,     3,     5,
       7,     7,     9,     5,     5,     7,     6,     6,     8,    10,
       6,     4,     4,     4,     3,     5,     5,     6,     6,     2,
       5,     4,     3,     3,     5,     7,     3,     5,     7,     3,
       3,     2,     4,     7,     6,     4,     6,     4,     2,     4,
       4,     4,     4,     4,     7,     7,     7,     6,     6,     4,
       4,     4,     4,     4,     5,     5,     5,     5,     5,     5,
       4,     4,     4,     4,     4,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     2,     4,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     7,     7,     3,
       5,     5,     4,     6,     3,     5,     5,     7,     4,     6,
       5,     3,     5,     7,     3,     5,     5,     3,     2,     7,
       7,     5,     7,     5,     3,     5,     7,     0,     4,     6,
       5,     3,     7,     6,     4,     7,     6,     7,     6,     5,
       6,     5,     3,     4,     5,     4,     5,     5,     3,     3,
       4,     5,     4,     5,     5,     6,     5,     0,     3,     3,
       0,     4,     4,     4,     5,     5,     4,     5,     5,     6,
       6,     7,     7,     6,     6,     6,     6,     5,     5,     6,
       6,     7,     7,     7,     7,     7,     6,     4,     7,     6,
       4,     8,     7,     5,     5,     0,     4,     4,     4,     3,
       5,     3,     3,     5,     5,     1,     2,     3,     3,     3,
       4,     5,     5,     3,     3,     4,     5,     5,     9,     8,
       8,     6,     9,     9,     5,     5,     4,     3,     3,     2,
       1,     5,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     2,     2,     1,
       1,     4,     3,     4,     3,     6,     6,     5,     3,     3,
       6,     5,     4,     4,     6,     6,     5,     3,     6,     5,
       3,     2,     3,     3,     2,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     5,     5,     1,     1,     1,     1,     2,
       3,     4,     5,     1,     2
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
#line 2400 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 271 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2406 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 272 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2412 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 276 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2418 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 278 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2424 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 280 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2430 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 282 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2436 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 284 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2442 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 286 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2450 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 290 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2457 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 293 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2465 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 297 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2471 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 299 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2477 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 301 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2483 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 303 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2489 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 305 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2495 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 307 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2503 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 311 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2511 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 315 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2517 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 317 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2523 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 319 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2529 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 321 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2535 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 323 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2541 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 325 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2547 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 327 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2553 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 329 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2559 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 331 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2565 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 333 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2571 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 335 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2577 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 337 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2583 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 339 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2589 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 341 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2595 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 343 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2602 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 346 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2609 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 349 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2615 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 351 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2622 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 354 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2629 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 357 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2635 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 359 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2641 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 361 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2647 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 363 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2653 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 365 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2662 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 370 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2670 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 374 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2678 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 378 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2687 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 383 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2695 "raku.tab.c"
    break;

  case 48: /* stmt: scalar_methcall ';'  */
#line 386 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 2701 "raku.tab.c"
    break;

  case 49: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 388 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2707 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 390 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2713 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 392 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2719 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 394 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2725 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 396 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2732 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2739 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2746 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2753 "raku.tab.c"
    break;

  case 57: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 408 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2760 "raku.tab.c"
    break;

  case 58: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 411 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2767 "raku.tab.c"
    break;

  case 59: /* stmt: expr KW_IF expr ';'  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2773 "raku.tab.c"
    break;

  case 60: /* stmt: expr KW_UNLESS expr ';'  */
#line 416 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2779 "raku.tab.c"
    break;

  case 61: /* stmt: expr KW_WHILE expr ';'  */
#line 418 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2785 "raku.tab.c"
    break;

  case 62: /* stmt: expr KW_UNTIL expr ';'  */
#line 420 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2791 "raku.tab.c"
    break;

  case 63: /* stmt: expr KW_FOR expr ';'  */
#line 422 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2798 "raku.tab.c"
    break;

  case 64: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 425 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 2805 "raku.tab.c"
    break;

  case 65: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 428 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 2812 "raku.tab.c"
    break;

  case 66: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 431 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 2820 "raku.tab.c"
    break;

  case 67: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 435 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 2827 "raku.tab.c"
    break;

  case 68: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 438 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 2834 "raku.tab.c"
    break;

  case 69: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 441 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 2841 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 444 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 2848 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 447 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 2855 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 450 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 2862 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 453 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 2869 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 456 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 2876 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 459 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 2883 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 462 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 2890 "raku.tab.c"
    break;

  case 77: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 465 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 2897 "raku.tab.c"
    break;

  case 78: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 468 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 2904 "raku.tab.c"
    break;

  case 79: /* stmt: expr ';'  */
#line 470 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2910 "raku.tab.c"
    break;

  case 80: /* stmt: ';'  */
#line 471 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 2916 "raku.tab.c"
    break;

  case 81: /* stmt: if_stmt  */
#line 472 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2922 "raku.tab.c"
    break;

  case 82: /* stmt: while_stmt  */
#line 473 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2928 "raku.tab.c"
    break;

  case 83: /* stmt: for_stmt  */
#line 474 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2934 "raku.tab.c"
    break;

  case 84: /* stmt: given_stmt  */
#line 475 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2940 "raku.tab.c"
    break;

  case 85: /* stmt: KW_TRY block  */
#line 477 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2946 "raku.tab.c"
    break;

  case 86: /* stmt: KW_TRY block KW_CATCH block  */
#line 479 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2952 "raku.tab.c"
    break;

  case 87: /* stmt: KW_CATCH block  */
#line 481 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2958 "raku.tab.c"
    break;

  case 88: /* stmt: block  */
#line 483 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2964 "raku.tab.c"
    break;

  case 89: /* stmt: unless_stmt  */
#line 484 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2970 "raku.tab.c"
    break;

  case 90: /* stmt: until_stmt  */
#line 485 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2976 "raku.tab.c"
    break;

  case 91: /* stmt: repeat_stmt  */
#line 486 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2982 "raku.tab.c"
    break;

  case 92: /* stmt: sub_decl  */
#line 487 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2988 "raku.tab.c"
    break;

  case 93: /* stmt: class_decl  */
#line 488 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2994 "raku.tab.c"
    break;

  case 94: /* stmt: role_decl  */
#line 489 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3000 "raku.tab.c"
    break;

  case 95: /* stmt: grammar_decl  */
#line 490 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3006 "raku.tab.c"
    break;

  case 96: /* if_stmt: KW_IF '(' expr ')' block  */
#line 494 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3012 "raku.tab.c"
    break;

  case 97: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 496 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3018 "raku.tab.c"
    break;

  case 98: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 498 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3024 "raku.tab.c"
    break;

  case 99: /* if_stmt: KW_IF expr block  */
#line 500 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3030 "raku.tab.c"
    break;

  case 100: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 502 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3036 "raku.tab.c"
    break;

  case 101: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3042 "raku.tab.c"
    break;

  case 102: /* if_stmt: KW_IF expr block elsif_tail  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3048 "raku.tab.c"
    break;

  case 103: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 508 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3054 "raku.tab.c"
    break;

  case 104: /* elsif_tail: KW_ELSIF expr block  */
#line 512 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3060 "raku.tab.c"
    break;

  case 105: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 514 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3066 "raku.tab.c"
    break;

  case 106: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 516 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3072 "raku.tab.c"
    break;

  case 107: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 518 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3078 "raku.tab.c"
    break;

  case 108: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 520 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3084 "raku.tab.c"
    break;

  case 109: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 522 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3090 "raku.tab.c"
    break;

  case 110: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 526 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3096 "raku.tab.c"
    break;

  case 111: /* while_stmt: KW_WHILE expr block  */
#line 528 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3102 "raku.tab.c"
    break;

  case 112: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 532 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3108 "raku.tab.c"
    break;

  case 113: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 534 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3114 "raku.tab.c"
    break;

  case 114: /* unless_stmt: KW_UNLESS expr block  */
#line 536 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3120 "raku.tab.c"
    break;

  case 115: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 538 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3126 "raku.tab.c"
    break;

  case 116: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 542 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3132 "raku.tab.c"
    break;

  case 117: /* until_stmt: KW_UNTIL expr block  */
#line 544 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3138 "raku.tab.c"
    break;

  case 118: /* repeat_stmt: KW_REPEAT block  */
#line 548 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3144 "raku.tab.c"
    break;

  case 119: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 552 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3154 "raku.tab.c"
    break;

  case 120: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 558 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 3164 "raku.tab.c"
    break;

  case 121: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 564 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3173 "raku.tab.c"
    break;

  case 122: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 569 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3183 "raku.tab.c"
    break;

  case 123: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 575 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3192 "raku.tab.c"
    break;

  case 124: /* for_stmt: KW_FOR expr block  */
#line 580 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3199 "raku.tab.c"
    break;

  case 125: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 585 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3211 "raku.tab.c"
    break;

  case 126: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 593 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3224 "raku.tab.c"
    break;

  case 127: /* when_list: %empty  */
#line 603 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3230 "raku.tab.c"
    break;

  case 128: /* when_list: when_list KW_WHEN expr block  */
#line 605 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3238 "raku.tab.c"
    break;

  case 129: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 611 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3250 "raku.tab.c"
    break;

  case 130: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 619 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3260 "raku.tab.c"
    break;

  case 131: /* sub_decl: KW_SUB IDENT sub_body  */
#line 625 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3270 "raku.tab.c"
    break;

  case 132: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 631 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3282 "raku.tab.c"
    break;

  case 133: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 639 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3292 "raku.tab.c"
    break;

  case 134: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 645 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3302 "raku.tab.c"
    break;

  case 135: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 651 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3315 "raku.tab.c"
    break;

  case 136: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 660 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3326 "raku.tab.c"
    break;

  case 137: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 667 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3339 "raku.tab.c"
    break;

  case 138: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 676 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3352 "raku.tab.c"
    break;

  case 139: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 685 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3363 "raku.tab.c"
    break;

  case 140: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 694 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3374 "raku.tab.c"
    break;

  case 141: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 701 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3383 "raku.tab.c"
    break;

  case 142: /* sub_body: '{' stmt_list '}'  */
#line 707 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3389 "raku.tab.c"
    break;

  case 143: /* sub_body: '{' stmt_list expr '}'  */
#line 709 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3396 "raku.tab.c"
    break;

  case 144: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 712 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3403 "raku.tab.c"
    break;

  case 145: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 715 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3409 "raku.tab.c"
    break;

  case 146: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 717 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3415 "raku.tab.c"
    break;

  case 147: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 719 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3421 "raku.tab.c"
    break;

  case 148: /* method_body: '{' stmt_list '}'  */
#line 722 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3427 "raku.tab.c"
    break;

  case 149: /* method_body: '{' YADA '}'  */
#line 723 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3433 "raku.tab.c"
    break;

  case 150: /* method_body: '{' stmt_list expr '}'  */
#line 725 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3440 "raku.tab.c"
    break;

  case 151: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 728 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3447 "raku.tab.c"
    break;

  case 152: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 731 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3453 "raku.tab.c"
    break;

  case 153: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 733 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3459 "raku.tab.c"
    break;

  case 154: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 735 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3465 "raku.tab.c"
    break;

  case 155: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 739 "raku.y"
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
#line 3483 "raku.tab.c"
    break;

  case 156: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 755 "raku.y"
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
#line 3500 "raku.tab.c"
    break;

  case 157: /* is_clauses: %empty  */
#line 769 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3506 "raku.tab.c"
    break;

  case 158: /* is_clauses: is_clauses IDENT IDENT  */
#line 771 "raku.y"
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
#line 3522 "raku.tab.c"
    break;

  case 159: /* is_clauses: is_clauses TESTOP IDENT  */
#line 783 "raku.y"
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
#line 3538 "raku.tab.c"
    break;

  case 160: /* class_body_list: %empty  */
#line 796 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3544 "raku.tab.c"
    break;

  case 161: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 798 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3551 "raku.tab.c"
    break;

  case 162: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 801 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3558 "raku.tab.c"
    break;

  case 163: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 804 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3565 "raku.tab.c"
    break;

  case 164: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 807 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3572 "raku.tab.c"
    break;

  case 165: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 810 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3579 "raku.tab.c"
    break;

  case 166: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 813 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3586 "raku.tab.c"
    break;

  case 167: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 816 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3593 "raku.tab.c"
    break;

  case 168: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 819 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3600 "raku.tab.c"
    break;

  case 169: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 822 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3607 "raku.tab.c"
    break;

  case 170: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 825 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3614 "raku.tab.c"
    break;

  case 171: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 828 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3621 "raku.tab.c"
    break;

  case 172: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 831 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3628 "raku.tab.c"
    break;

  case 173: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 834 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3639 "raku.tab.c"
    break;

  case 174: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 841 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3650 "raku.tab.c"
    break;

  case 175: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 848 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3661 "raku.tab.c"
    break;

  case 176: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 855 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3672 "raku.tab.c"
    break;

  case 177: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 862 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3680 "raku.tab.c"
    break;

  case 178: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 866 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3688 "raku.tab.c"
    break;

  case 179: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 870 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3696 "raku.tab.c"
    break;

  case 180: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 874 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3704 "raku.tab.c"
    break;

  case 181: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 878 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3715 "raku.tab.c"
    break;

  case 182: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 885 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3726 "raku.tab.c"
    break;

  case 183: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 892 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3737 "raku.tab.c"
    break;

  case 184: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 899 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3748 "raku.tab.c"
    break;

  case 185: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 906 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3762 "raku.tab.c"
    break;

  case 186: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 916 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3774 "raku.tab.c"
    break;

  case 187: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 924 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3786 "raku.tab.c"
    break;

  case 188: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 932 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3799 "raku.tab.c"
    break;

  case 189: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 941 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3810 "raku.tab.c"
    break;

  case 190: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 948 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3821 "raku.tab.c"
    break;

  case 191: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 955 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3835 "raku.tab.c"
    break;

  case 192: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 965 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3847 "raku.tab.c"
    break;

  case 193: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 973 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 3859 "raku.tab.c"
    break;

  case 194: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 983 "raku.y"
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
#line 3876 "raku.tab.c"
    break;

  case 195: /* grammar_body_list: %empty  */
#line 997 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3882 "raku.tab.c"
    break;

  case 196: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 999 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3891 "raku.tab.c"
    break;

  case 197: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1004 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3900 "raku.tab.c"
    break;

  case 198: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1009 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3909 "raku.tab.c"
    break;

  case 199: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1016 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3917 "raku.tab.c"
    break;

  case 200: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1020 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3925 "raku.tab.c"
    break;

  case 201: /* pair_list: IDENT OP_FATARROW expr  */
#line 1026 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3931 "raku.tab.c"
    break;

  case 202: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1028 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3937 "raku.tab.c"
    break;

  case 203: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1030 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3943 "raku.tab.c"
    break;

  case 204: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1032 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3949 "raku.tab.c"
    break;

  case 205: /* param_list: VAR_SCALAR  */
#line 1035 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3955 "raku.tab.c"
    break;

  case 206: /* param_list: IDENT VAR_SCALAR  */
#line 1036 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3961 "raku.tab.c"
    break;

  case 207: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1037 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3967 "raku.tab.c"
    break;

  case 208: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1038 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3973 "raku.tab.c"
    break;

  case 209: /* param_list: param_list ',' VAR_SCALAR  */
#line 1039 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3979 "raku.tab.c"
    break;

  case 210: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1040 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3985 "raku.tab.c"
    break;

  case 211: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1041 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3991 "raku.tab.c"
    break;

  case 212: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1042 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3997 "raku.tab.c"
    break;

  case 213: /* block: '{' stmt_list '}'  */
#line 1045 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4003 "raku.tab.c"
    break;

  case 214: /* block: '{' YADA '}'  */
#line 1046 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4009 "raku.tab.c"
    break;

  case 215: /* block: '{' stmt_list expr '}'  */
#line 1048 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4015 "raku.tab.c"
    break;

  case 216: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1050 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4021 "raku.tab.c"
    break;

  case 217: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1052 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4027 "raku.tab.c"
    break;

  case 218: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1054 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4035 "raku.tab.c"
    break;

  case 219: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1058 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4042 "raku.tab.c"
    break;

  case 220: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1061 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4049 "raku.tab.c"
    break;

  case 221: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1064 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4056 "raku.tab.c"
    break;

  case 222: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1067 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4063 "raku.tab.c"
    break;

  case 223: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1070 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4070 "raku.tab.c"
    break;

  case 224: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1073 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4076 "raku.tab.c"
    break;

  case 225: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1075 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4082 "raku.tab.c"
    break;

  case 226: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1077 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4088 "raku.tab.c"
    break;

  case 227: /* closure: '{' expr '}'  */
#line 1080 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4094 "raku.tab.c"
    break;

  case 228: /* expr: VAR_SCALAR '=' expr  */
#line 1083 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4100 "raku.tab.c"
    break;

  case 229: /* expr: KW_GATHER block  */
#line 1084 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4110 "raku.tab.c"
    break;

  case 230: /* expr: tern_expr  */
#line 1089 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4116 "raku.tab.c"
    break;

  case 231: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1093 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4122 "raku.tab.c"
    break;

  case 232: /* tern_expr: cmp_expr  */
#line 1094 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4128 "raku.tab.c"
    break;

  case 233: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1097 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4134 "raku.tab.c"
    break;

  case 234: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1098 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4140 "raku.tab.c"
    break;

  case 235: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 1099 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4146 "raku.tab.c"
    break;

  case 236: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 1100 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4152 "raku.tab.c"
    break;

  case 237: /* cmp_expr: jct_expr '<' jct_expr  */
#line 1101 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4158 "raku.tab.c"
    break;

  case 238: /* cmp_expr: jct_expr '>' jct_expr  */
#line 1102 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4164 "raku.tab.c"
    break;

  case 239: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 1103 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4170 "raku.tab.c"
    break;

  case 240: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 1104 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4176 "raku.tab.c"
    break;

  case 241: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1105 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4182 "raku.tab.c"
    break;

  case 242: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1106 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4188 "raku.tab.c"
    break;

  case 243: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1108 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4198 "raku.tab.c"
    break;

  case 244: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1114 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4208 "raku.tab.c"
    break;

  case 245: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1120 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4218 "raku.tab.c"
    break;

  case 246: /* cmp_expr: jct_expr  */
#line 1125 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4224 "raku.tab.c"
    break;

  case 247: /* jct_expr: jct_expr '|' range_expr  */
#line 1128 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4230 "raku.tab.c"
    break;

  case 248: /* jct_expr: jct_expr '&' range_expr  */
#line 1129 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4236 "raku.tab.c"
    break;

  case 249: /* jct_expr: dor_expr  */
#line 1130 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4242 "raku.tab.c"
    break;

  case 250: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1134 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4248 "raku.tab.c"
    break;

  case 251: /* dor_expr: range_expr  */
#line 1135 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4254 "raku.tab.c"
    break;

  case 252: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1138 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4260 "raku.tab.c"
    break;

  case 253: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1139 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4266 "raku.tab.c"
    break;

  case 254: /* range_expr: add_expr  */
#line 1140 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4272 "raku.tab.c"
    break;

  case 255: /* add_expr: add_expr '~' repl_expr  */
#line 1143 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4278 "raku.tab.c"
    break;

  case 256: /* add_expr: repl_expr  */
#line 1144 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4284 "raku.tab.c"
    break;

  case 257: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1147 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4290 "raku.tab.c"
    break;

  case 258: /* repl_expr: addsub_expr  */
#line 1148 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4296 "raku.tab.c"
    break;

  case 259: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1151 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4302 "raku.tab.c"
    break;

  case 260: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1152 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4308 "raku.tab.c"
    break;

  case 261: /* addsub_expr: mul_expr  */
#line 1153 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4314 "raku.tab.c"
    break;

  case 262: /* mul_expr: mul_expr '*' unary_expr  */
#line 1156 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4320 "raku.tab.c"
    break;

  case 263: /* mul_expr: mul_expr '/' unary_expr  */
#line 1157 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4326 "raku.tab.c"
    break;

  case 264: /* mul_expr: mul_expr '%' unary_expr  */
#line 1158 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4332 "raku.tab.c"
    break;

  case 265: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1159 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4338 "raku.tab.c"
    break;

  case 266: /* mul_expr: unary_expr  */
#line 1160 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4344 "raku.tab.c"
    break;

  case 267: /* unary_expr: '-' unary_expr  */
#line 1163 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4350 "raku.tab.c"
    break;

  case 268: /* unary_expr: '!' unary_expr  */
#line 1164 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4356 "raku.tab.c"
    break;

  case 269: /* unary_expr: postfix_expr  */
#line 1165 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4362 "raku.tab.c"
    break;

  case 270: /* postfix_expr: call_expr  */
#line 1167 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4368 "raku.tab.c"
    break;

  case 271: /* call_expr: IDENT '(' arg_list ')'  */
#line 1170 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4377 "raku.tab.c"
    break;

  case 272: /* call_expr: IDENT '(' ')'  */
#line 1174 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4383 "raku.tab.c"
    break;

  case 273: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1176 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4392 "raku.tab.c"
    break;

  case 274: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1181 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4398 "raku.tab.c"
    break;

  case 275: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1183 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4408 "raku.tab.c"
    break;

  case 276: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1189 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4418 "raku.tab.c"
    break;

  case 277: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1195 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4426 "raku.tab.c"
    break;

  case 278: /* call_expr: IDENT '.' KW_NEW  */
#line 1199 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4434 "raku.tab.c"
    break;

  case 279: /* call_expr: IDENT '.' IDENT  */
#line 1203 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4443 "raku.tab.c"
    break;

  case 280: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1208 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4454 "raku.tab.c"
    break;

  case 281: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1215 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4463 "raku.tab.c"
    break;

  case 282: /* call_expr: IDENT '.' CARET IDENT  */
#line 1220 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4473 "raku.tab.c"
    break;

  case 283: /* call_expr: atom '.' CARET IDENT  */
#line 1226 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4483 "raku.tab.c"
    break;

  case 284: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1232 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4494 "raku.tab.c"
    break;

  case 285: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1239 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4505 "raku.tab.c"
    break;

  case 286: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1246 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4514 "raku.tab.c"
    break;

  case 287: /* call_expr: atom '.' IDENT  */
#line 1251 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4523 "raku.tab.c"
    break;

  case 288: /* call_expr: call_expr '.' IDENT '(' arg_list ')'  */
#line 1256 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4534 "raku.tab.c"
    break;

  case 289: /* call_expr: call_expr '.' IDENT '(' ')'  */
#line 1263 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4543 "raku.tab.c"
    break;

  case 290: /* call_expr: call_expr '.' IDENT  */
#line 1268 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4552 "raku.tab.c"
    break;

  case 291: /* call_expr: KW_DIE expr  */
#line 1273 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 4558 "raku.tab.c"
    break;

  case 292: /* call_expr: KW_MAP closure expr  */
#line 1275 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4564 "raku.tab.c"
    break;

  case 293: /* call_expr: KW_GREP closure expr  */
#line 1277 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4570 "raku.tab.c"
    break;

  case 294: /* call_expr: KW_SORT expr  */
#line 1279 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4576 "raku.tab.c"
    break;

  case 295: /* call_expr: KW_SORT closure expr  */
#line 1281 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4582 "raku.tab.c"
    break;

  case 296: /* call_expr: atom  */
#line 1282 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4588 "raku.tab.c"
    break;

  case 297: /* arg_list: expr  */
#line 1285 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4594 "raku.tab.c"
    break;

  case 298: /* arg_list: arg_list ',' expr  */
#line 1286 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4600 "raku.tab.c"
    break;

  case 299: /* atom: LIT_INT  */
#line 1289 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4606 "raku.tab.c"
    break;

  case 300: /* atom: LIT_FLOAT  */
#line 1290 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4612 "raku.tab.c"
    break;

  case 301: /* atom: LIT_STR  */
#line 1291 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4618 "raku.tab.c"
    break;

  case 302: /* atom: WORDLIST  */
#line 1293 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4630 "raku.tab.c"
    break;

  case 303: /* atom: LIT_INTERP_STR  */
#line 1300 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4636 "raku.tab.c"
    break;

  case 304: /* atom: VAR_SCALAR  */
#line 1301 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4642 "raku.tab.c"
    break;

  case 305: /* atom: VAR_ARRAY  */
#line 1302 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4648 "raku.tab.c"
    break;

  case 306: /* atom: VAR_HASH  */
#line 1303 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4654 "raku.tab.c"
    break;

  case 307: /* atom: VAR_CAPTURE  */
#line 1305 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4662 "raku.tab.c"
    break;

  case 308: /* atom: VAR_FH  */
#line 1309 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4670 "raku.tab.c"
    break;

  case 309: /* atom: VAR_NAMED_CAPTURE  */
#line 1313 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4677 "raku.tab.c"
    break;

  case 310: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1316 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4683 "raku.tab.c"
    break;

  case 311: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1318 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4689 "raku.tab.c"
    break;

  case 312: /* atom: VAR_HASH '{' expr '}'  */
#line 1320 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4695 "raku.tab.c"
    break;

  case 313: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1322 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4701 "raku.tab.c"
    break;

  case 314: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1324 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4707 "raku.tab.c"
    break;

  case 315: /* atom: IDENT  */
#line 1325 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4713 "raku.tab.c"
    break;

  case 316: /* atom: VAR_TWIGIL  */
#line 1327 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4721 "raku.tab.c"
    break;

  case 317: /* atom: VAR_ARRAY_TWIGIL  */
#line 1331 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4729 "raku.tab.c"
    break;

  case 318: /* atom: VAR_HASH_TWIGIL  */
#line 1335 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4737 "raku.tab.c"
    break;

  case 319: /* atom: '(' ')'  */
#line 1338 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 4743 "raku.tab.c"
    break;

  case 320: /* atom: '(' expr ')'  */
#line 1339 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 4749 "raku.tab.c"
    break;

  case 321: /* atom: '(' expr ',' ')'  */
#line 1341 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 4755 "raku.tab.c"
    break;

  case 322: /* atom: '(' expr ',' arg_list ')'  */
#line 1343 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 4762 "raku.tab.c"
    break;

  case 323: /* atom: block  */
#line 1345 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4768 "raku.tab.c"
    break;

  case 324: /* atom: KW_SUB block  */
#line 1346 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4774 "raku.tab.c"
    break;


#line 4778 "raku.tab.c"

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

#line 1348 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
