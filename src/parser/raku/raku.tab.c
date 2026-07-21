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
  YYSYMBOL_elsif_tail = 114,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 115,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 116,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 117,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 118,              /* repeat_stmt  */
  YYSYMBOL_for_stmt = 119,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 120,               /* given_stmt  */
  YYSYMBOL_when_list = 121,                /* when_list  */
  YYSYMBOL_sub_decl = 122,                 /* sub_decl  */
  YYSYMBOL_sub_body = 123,                 /* sub_body  */
  YYSYMBOL_method_body = 124,              /* method_body  */
  YYSYMBOL_class_decl = 125,               /* class_decl  */
  YYSYMBOL_role_decl = 126,                /* role_decl  */
  YYSYMBOL_is_clauses = 127,               /* is_clauses  */
  YYSYMBOL_class_body_list = 128,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 129,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 130,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 131,           /* named_arg_list  */
  YYSYMBOL_pair_list = 132,                /* pair_list  */
  YYSYMBOL_param_list = 133,               /* param_list  */
  YYSYMBOL_block = 134,                    /* block  */
  YYSYMBOL_closure = 135,                  /* closure  */
  YYSYMBOL_expr = 136,                     /* expr  */
  YYSYMBOL_tern_expr = 137,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 138,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 139,                 /* jct_expr  */
  YYSYMBOL_range_expr = 140,               /* range_expr  */
  YYSYMBOL_add_expr = 141,                 /* add_expr  */
  YYSYMBOL_repl_expr = 142,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 143,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 144,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 145,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 146,             /* postfix_expr  */
  YYSYMBOL_call_expr = 147,                /* call_expr  */
  YYSYMBOL_arg_list = 148,                 /* arg_list  */
  YYSYMBOL_atom = 149                      /* atom  */
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
#define YYLAST   2645

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  298
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  758

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
       0,   257,   257,   268,   269,   272,   274,   276,   278,   280,
     282,   286,   289,   293,   295,   297,   299,   301,   303,   307,
     311,   313,   315,   317,   319,   321,   323,   325,   327,   329,
     331,   333,   335,   337,   339,   342,   345,   347,   350,   353,
     355,   357,   359,   361,   366,   370,   374,   379,   383,   390,
     395,   398,   401,   404,   407,   410,   412,   414,   416,   418,
     421,   424,   427,   431,   434,   437,   440,   441,   442,   443,
     444,   445,   446,   448,   450,   452,   454,   455,   456,   457,
     458,   459,   460,   463,   465,   467,   469,   471,   473,   475,
     477,   481,   483,   485,   487,   489,   491,   495,   497,   501,
     503,   505,   507,   511,   513,   517,   521,   527,   533,   538,
     544,   549,   554,   562,   573,   574,   580,   588,   594,   600,
     608,   614,   620,   629,   636,   645,   654,   663,   664,   667,
     669,   673,   674,   675,   678,   680,   684,   700,   715,   716,
     728,   742,   743,   746,   749,   752,   755,   758,   761,   764,
     767,   770,   773,   776,   779,   786,   793,   800,   807,   811,
     815,   819,   823,   830,   837,   844,   851,   861,   869,   877,
     886,   893,   900,   910,   918,   928,   943,   944,   949,   954,
     961,   965,   971,   973,   975,   977,   981,   982,   983,   984,
     985,   986,   987,   988,   991,   992,   993,   995,   997,   999,
    1003,  1006,  1009,  1012,  1015,  1018,  1022,  1025,  1026,  1031,
    1034,  1036,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1055,  1061,  1067,  1070,  1071,  1072,  1075,
    1076,  1077,  1080,  1081,  1084,  1085,  1088,  1089,  1090,  1093,
    1094,  1095,  1096,  1097,  1100,  1101,  1102,  1104,  1106,  1111,
    1112,  1117,  1119,  1125,  1131,  1135,  1139,  1144,  1151,  1156,
    1162,  1168,  1175,  1182,  1187,  1192,  1194,  1196,  1198,  1200,
    1202,  1205,  1206,  1209,  1210,  1211,  1212,  1220,  1221,  1222,
    1223,  1224,  1228,  1232,  1235,  1237,  1239,  1241,  1243,  1245,
    1246,  1250,  1254,  1258,  1259,  1260,  1262,  1265,  1266
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
  "program", "stmt_list", "stmt", "if_stmt", "elsif_tail", "while_stmt",
  "unless_stmt", "until_stmt", "repeat_stmt", "for_stmt", "given_stmt",
  "when_list", "sub_decl", "sub_body", "method_body", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "pair_list", "param_list",
  "block", "closure", "expr", "tern_expr", "cmp_expr", "jct_expr",
  "range_expr", "add_expr", "repl_expr", "addsub_expr", "mul_expr",
  "unary_expr", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-532)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-298)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -532,   121,  1033,  -532,  -532,  -532,  -532,  -532,   247,    51,
     -16,    92,   322,  -532,  -532,  -532,  -532,  -532,   171,   455,
     375,  1866,  1918,  1978,  2030,  2090,    -9,    94,  2090,   600,
       2,  2090,   204,   237,  2142,  2202,    94,   152,   152,  2254,
      94,    94,  2090,   279,   305,    71,  -532,   316,  2538,  2538,
    -532,  1098,   334,  -532,  -532,  -532,  -532,  -532,  -532,  -532,
    -532,  -532,  -532,  -532,  -532,   448,    45,  -532,   428,   340,
    -532,    98,   182,   -38,   295,  -532,  -532,  -532,   346,   343,
    2090,   420,  1150,  2090,   442,  2090,  2090,    63,  1216,   384,
       6,   371,     7,  -532,    94,  -532,  1268,  -532,  -532,   123,
     183,   259,   266,   498,   501,   391,  1098,   325,  1098,    85,
    1098,    94,  1098,    94,   -55,   102,    -8,  -532,  -532,   392,
    -532,   395,   431,   473,   381,    76,    87,  1098,    94,  1098,
      94,  -532,  2090,  2090,  2090,  1334,  2090,  -532,   536,  -532,
    -532,  -532,   485,   506,    22,   510,   544,  -532,  -532,  -532,
     360,   444,   705,  2090,  2090,  2090,  2090,  2090,  -532,  2538,
    2538,  2538,  2538,  2538,  2538,  2538,  2538,  2538,   504,  2538,
    2538,  2538,  2538,  2538,  2538,  2538,  2538,  2538,  2538,  2538,
    2538,  2538,  2538,   467,   379,   540,   155,  -532,   387,   584,
     586,   583,   591,   590,   682,   595,  -532,   432,  -532,  2090,
    2090,   685,  2090,   599,   360,   445,  -532,  2090,  2090,  -532,
    2314,  -532,  2366,  -532,   277,   301,   308,    -6,   615,   617,
     449,  2090,  2090,  2090,  2090,  -532,  2090,   453,  2090,  2090,
    -532,  2090,   457,   253,   461,  -532,   695,  2090,  -532,  2538,
    2538,     3,  -532,  -532,  -532,  -532,  2090,  2090,  -532,   692,
    2090,   698,  2090,   462,   706,   465,  -532,   614,  -532,  -532,
    -532,    94,    -4,  -532,    14,   629,   639,  -532,  1394,  -532,
    -532,   294,   638,   107,   669,  1866,  1918,  2090,  -532,   131,
     656,   657,   660,   661,   662,   278,   278,   687,   278,   278,
     278,   278,   278,   278,  -532,  -532,  -532,   278,   278,  -532,
    -532,   672,   672,   182,   -38,   295,   295,  -532,  -532,  -532,
    -532,   666,   759,  -532,  1446,  -532,  2090,  1512,  -532,   688,
     689,   690,  -532,  1564,  -532,  1630,  -532,  -532,   668,   693,
     670,  -532,   678,  -532,   679,  1098,   208,   715,    81,  1682,
     401,   683,  2090,  -532,  2426,  -532,  2090,  -532,    20,  -532,
    2090,  2090,  1394,   686,   691,   694,   696,   427,  1394,   697,
     700,   470,    94,   -15,  2478,  -532,    94,    94,   -35,   -28,
     -17,  -532,    28,   681,   466,   821,   701,   702,    21,   699,
     703,   704,   707,    94,    94,    94,  -532,  -532,   772,   776,
    -532,     9,   681,   478,    31,   393,     4,  -532,   479,   777,
    2090,  2090,  2090,   205,   177,    67,  -532,  -532,  -532,  -532,
    -532,  -532,  2538,  1748,  -532,   708,   483,   709,   713,   486,
    2090,  2090,  2090,  -532,   489,   169,  -532,   495,   496,  -532,
    -532,  -532,  -532,  -532,   519,  2090,  -532,  2090,  2090,  2090,
     520,  -532,   312,  -532,   716,  1098,   474,   717,   681,   523,
     718,   719,   524,  -532,  -532,  -532,  -532,  -532,   531,  -532,
    -532,  -532,   574,  -532,  -532,  1098,    94,  -532,  -532,   786,
    -532,   789,   794,  -532,   795,   797,  -532,   394,   681,  1866,
    1918,  -532,   235,  -532,  -532,  2090,    94,  -532,  -532,  -532,
     720,   721,   796,  -532,  -532,  -532,  -532,    11,    29,   425,
     765,  -532,  -532,   681,   681,   538,   546,   814,   824,   834,
    -532,  -532,   215,   743,   750,   154,  -532,  -532,  -532,  -532,
    -532,   549,   550,  -532,   758,  -532,  -532,   771,   773,   778,
     780,  -532,  2090,   859,  -532,  -532,  1394,   781,   528,  -532,
    -532,   782,   810,   811,  -532,   553,  -532,  2090,  -532,  -532,
     681,  -532,  -532,   784,   785,   -15,  -532,   554,   609,    94,
      94,    94,  -532,  -532,  -532,   180,  -532,   257,   189,  -532,
      94,   779,  -532,  -532,    94,  -532,   314,   321,    91,   101,
     482,   788,   790,   876,  -532,  -532,   681,   681,   885,   886,
     887,  2090,  1800,   808,   809,  -532,  -532,  -532,  -532,  -532,
    -532,  -532,  -532,  -532,   830,   561,  -532,  -532,  -532,  2090,
    2090,  1394,   577,  -532,  -532,  -532,  -532,  -532,    94,    94,
    -532,  -532,  -532,  -532,  -532,   888,   889,  -532,  -532,  -532,
    -532,  -532,    34,   838,  -532,    40,  -532,   890,   891,   800,
    2090,  -532,   892,   893,   801,  2090,  -532,   106,   195,   802,
     807,  -532,  -532,   350,  -532,  -532,  -532,  -532,  -532,   191,
     219,   562,  2090,  2090,  2090,   812,  -532,  -532,   565,  -532,
     653,  -532,  -532,  -532,   803,   566,   804,   927,   803,   569,
     813,   816,  -532,   817,   818,   819,  -532,   822,   897,   901,
     825,  2090,  -532,   911,   913,   833,  2090,  -532,  -532,  -532,
      41,  -532,  -532,  -532,   220,   244,   260,  -532,  -532,   835,
      94,  -532,  -532,   803,  -532,  1866,  1918,  -532,   274,  -532,
     803,  -532,  -532,  -532,  -532,  -532,  -532,   843,   853,  -532,
     854,   863,   864,  -532,   877,   803,   570,  -532,  -532,  -532,
    -532,  -532,  -532,   282,   230,  -532,  -532,  -532,  -532,  -532,
    -532,  -532,  -532,  -532,   803,  -532,  -532,  -532
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   273,   274,   275,   277,   278,   279,
     280,   290,   289,   291,   292,   281,   282,   283,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   276,     0,     0,     0,
      67,     0,     3,     4,    68,    69,    76,    77,    78,    70,
      71,    79,    80,    81,    82,    75,     0,   209,   211,   225,
     228,   231,   233,   235,   238,   243,   246,   247,   270,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     278,   279,   280,   290,     0,    29,     0,   297,   271,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,     0,   298,   208,     0,
      41,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   105,     0,     0,     0,     3,     0,   268,    72,    74,
     265,   138,     0,     0,     0,     0,   278,   245,   244,   293,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   207,     0,   251,     0,     0,
       0,     0,     0,   256,     0,   255,   249,     0,    24,     0,
       0,     0,     0,   293,     0,     0,    32,     0,     0,     6,
       0,     7,     0,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
      36,     0,     0,    86,     0,    98,     0,     0,   111,     0,
       0,     0,     3,   118,    39,    40,     0,     0,   114,     0,
       0,     0,     0,     0,   101,     0,   104,     0,   266,   267,
     269,     0,     0,   141,     0,     0,     0,   176,     0,   294,
     195,   278,   279,   280,   290,     0,     0,     0,   194,     0,
       0,     0,     0,     0,     0,   212,   213,     0,   214,   215,
     218,   219,   220,   221,   222,   223,   224,   216,   217,   226,
     227,   229,   230,   232,   234,   236,   237,   242,   239,   240,
     241,   264,     0,    45,     0,    42,     0,     0,   250,   284,
     285,   286,    47,     0,   259,     0,   248,   207,     0,     0,
       0,    31,     0,   272,     0,     0,     0,   275,   289,     0,
       0,     0,     0,    21,     0,    22,     0,    23,     0,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   294,     0,     0,    89,   294,     0,     0,   229,
     230,   186,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   294,     0,   294,   206,    73,     0,     0,
     141,     0,     0,     0,     0,     0,     0,   295,     0,     0,
       0,     0,     0,     0,     0,     0,   196,    55,    57,    59,
      56,    58,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,   258,     0,   289,   254,     0,     0,   284,
     285,   286,    30,     5,     0,     0,     9,     0,     0,     0,
       0,    14,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    63,    62,    61,    34,     0,    64,
      65,    37,    83,    88,    87,     0,     0,    97,   108,     0,
     110,     0,     0,   187,     0,     0,   117,     0,     0,     0,
       0,   127,     0,    26,    25,     0,     0,   112,   287,   288,
       0,     0,    99,   102,   103,   139,   140,     0,     0,     0,
       0,   137,   126,     0,     0,     0,     0,     0,     0,     0,
     175,   296,     0,     0,     0,     0,   197,   198,   205,   210,
     263,     0,     0,    44,     0,    46,    49,     0,     0,     0,
       0,   257,     0,     0,   252,   253,     0,     0,     0,   183,
     182,     0,     0,     0,    16,     0,    17,     0,    20,   120,
       0,    28,    27,   296,   296,     0,    90,     0,    91,     0,
       0,     0,   188,   189,   190,     0,   116,     0,     0,   128,
       0,     0,    53,    54,     0,   136,     0,     0,     0,     0,
       0,     0,     0,     0,   125,   123,     0,     0,     0,     0,
       0,     0,     0,   284,   286,   202,   262,   261,    43,    48,
      50,    51,    52,   180,     0,     0,    11,    10,    15,     0,
       0,     0,     0,   119,    35,    38,    85,    84,   294,     0,
      95,   109,   106,   107,   191,     0,     0,   129,   130,   115,
     113,   100,     0,     3,   168,     0,   171,     0,     0,     0,
       0,   147,     0,     0,     0,     0,   142,     0,     0,     0,
       0,   143,   144,     0,   122,   124,   177,   178,   179,     0,
       0,     0,     0,     0,     0,   296,   185,   184,     0,    18,
      92,    93,   192,   193,     0,     0,     0,     0,     0,     0,
       0,     0,   158,     0,     0,     0,   159,     0,     0,     0,
       0,     0,   149,     0,     0,     0,     0,   148,   145,   146,
       0,   174,   201,   200,     0,     0,     0,   181,    12,   296,
       0,    96,   167,     0,   132,     0,     0,   131,     0,   170,
       0,   156,   157,   151,   154,   155,   150,     0,     0,   160,
       0,     0,     0,   161,     0,     0,     0,   199,   203,   204,
      19,    94,   166,     0,     0,   133,   169,   164,   165,   153,
     162,   163,   152,   173,     0,   134,   135,   172
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -532,  -532,     1,  -532,  -356,  -458,  -532,  -532,  -532,  -532,
    -532,  -532,  -532,  -532,  -215,  -531,  -532,  -532,  -532,   545,
    -532,  -532,   567,   640,  -258,   211,   644,    -2,  -158,  -532,
     285,   513,   -11,   820,   815,   509,   -40,  -532,  -532,   -57,
    -532
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   152,    53,    54,   365,    55,    56,    57,    58,
      59,    60,   378,    61,   243,   634,    62,    63,   262,   391,
      64,   396,   427,   340,   374,    97,   133,    98,    67,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    99,
      78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      66,     2,   349,   287,   556,   116,   393,   463,   147,   148,
     388,    23,   122,   371,   115,   236,   123,   372,   389,   107,
     109,   111,   113,   114,   371,   188,   119,   121,   372,   124,
     371,   197,   128,   130,   372,   469,   265,   137,   473,   205,
     140,   371,   471,   576,   371,   372,   636,   237,   372,   150,
     371,   371,    52,   472,   372,   372,   177,   178,   485,   486,
     498,   499,   498,   499,   500,   175,   500,   207,   507,   508,
     509,   153,    52,    84,   154,   155,   175,   193,   185,   266,
     194,   189,   577,   191,   192,   143,   156,   157,   474,   475,
     449,    85,    52,   199,   204,   241,   201,   348,    52,   242,
     620,   242,   144,   390,   220,   637,   227,   373,   232,    82,
     234,   228,   510,   638,   202,   642,   195,   501,   392,   575,
     688,     3,   701,   643,   448,   253,   229,   255,   689,   487,
     257,   258,   259,   257,   260,   504,   505,   506,   674,   307,
     308,   309,   310,   712,   678,   735,   158,   719,   439,   639,
     279,   280,   281,   282,   283,   284,    83,   153,   476,   644,
     154,   155,   301,   302,   690,   249,   173,   174,   244,   357,
     239,   240,   156,   157,   361,   518,   251,   502,   640,    86,
     368,    87,   742,   250,    88,    89,   230,   231,   645,   746,
     624,   175,   641,   691,   252,   175,    84,   327,   328,   616,
     330,    52,   646,   228,   753,   333,   334,   692,   336,   693,
     341,   398,   711,    65,   401,   228,   125,   694,   229,   353,
     354,   355,   356,   757,   206,   207,   359,   360,   369,   370,
     229,   221,   158,   549,   222,   223,   532,   117,   118,   406,
     625,   626,   316,   375,   376,   377,   224,   131,   380,   126,
     382,   138,   139,   695,   519,   322,   228,   416,   317,   132,
     419,   153,   595,   566,   154,   155,   424,   176,   428,    87,
     208,   229,    88,   403,   404,   405,   156,   157,   230,   231,
     363,   364,   696,   221,   209,   517,   222,   223,   584,   585,
     230,   231,   525,   141,   435,   452,   697,   628,   224,   702,
     153,   458,   591,   154,   155,   117,   225,   226,   221,   436,
     437,   222,   223,   516,   417,   156,   157,   542,   592,   142,
     526,   599,   233,   224,   235,   238,   543,   703,   737,    79,
     145,   230,   231,   434,    80,   613,   158,   150,   756,   254,
     444,   256,   446,   569,   447,   600,   210,    81,   450,   451,
      82,   221,   738,   212,   222,   223,   522,   184,   225,   226,
     211,   602,   466,    65,   342,   627,   224,   213,   739,   171,
     172,   654,   655,   482,   675,   158,    79,   679,   343,   179,
     538,    80,   745,   225,   226,   100,   101,   102,   344,   103,
     755,   180,   181,   182,   399,   346,   151,    82,   513,   514,
     515,   218,   345,   371,   564,   219,   104,   372,   565,   347,
     105,   162,   163,   164,   165,   166,   167,   632,   528,   529,
     530,   633,    87,   168,   635,    88,   225,   226,   633,   169,
     170,   171,   172,   537,   186,   578,   539,   540,   579,   580,
     581,   582,   736,   545,   285,   286,   183,   288,   289,   290,
     291,   292,   293,   700,   297,   298,   190,   633,     4,     5,
       6,     7,   268,   557,   269,    90,    91,    92,    93,    12,
      13,    14,   387,    15,    16,    17,   200,   567,   568,   605,
     313,   311,   314,   570,   312,   198,    94,    27,   248,   207,
     612,   318,   647,   244,    32,   648,   245,   649,   650,    37,
      38,    39,   441,   442,    42,   159,   160,   161,   214,   215,
     216,   294,   295,   296,    46,   217,  -297,  -297,   246,  -297,
    -297,  -297,  -297,  -297,  -297,  -297,  -297,  -297,   457,   207,
     603,  -297,  -297,  -297,   207,   661,   326,  -297,  -297,  -297,
    -297,  -297,  -297,    48,  -297,  -297,  -297,   207,  -297,   332,
      49,   352,   270,   269,   668,   358,    95,   269,    96,   268,
     247,   362,    52,   268,   268,   366,   383,   268,   477,   385,
     478,   461,   207,   462,   464,   546,   547,   467,   468,   470,
     477,   207,   503,   511,   261,   207,    65,   524,   207,   659,
     527,   207,   263,   531,   492,   493,   494,   533,   207,   534,
     535,   555,   364,     4,     5,     6,     7,   666,   667,   264,
      90,    91,    92,    93,    12,    13,    14,   267,    15,    16,
      17,   536,   442,   269,   541,   477,   207,   550,   553,   607,
     207,    94,    27,   207,   677,   554,   619,   364,   683,    32,
     477,   315,   586,   687,    37,    38,    39,    82,   477,    42,
     587,   533,   207,   596,   597,   611,   268,   269,   618,    46,
     705,   706,   707,   207,   207,   665,   704,   207,   477,   709,
     713,   477,   477,   720,   754,   718,   320,   558,   669,   207,
     710,   364,   134,   136,   299,   300,   305,   306,    48,   730,
     319,   321,   322,   323,   734,    49,   324,   571,   325,   329,
     331,   120,   350,    51,   351,   367,   379,    52,     4,     5,
       6,     7,   381,   743,   744,   271,   272,   273,   274,    12,
      13,    14,   386,    15,    16,    17,    18,    19,    20,   275,
     276,    23,   394,   384,    24,    25,    26,    27,   277,    29,
      30,    31,   395,   400,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,   402,   407,   408,    44,
      45,   409,   410,   411,    46,   175,   617,   412,    47,   413,
     621,   622,   623,   414,   429,   420,   421,   422,   431,   432,
     433,   629,   438,   430,   443,   631,   495,   453,   242,   488,
     496,   512,   454,    48,   490,   455,   559,   456,   459,   560,
      49,   460,   483,   484,   561,   562,    50,   563,    51,   523,
     525,   489,    52,   278,   526,   491,   583,   544,   548,   551,
     552,   572,   573,   574,     4,     5,     6,     7,   588,   670,
     671,     8,     9,    10,    11,    12,    13,    14,   589,    15,
      16,    17,    18,    19,    20,   479,   480,    23,   590,   593,
      24,    25,    26,    27,    28,    29,    30,    31,   594,   598,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,   599,   604,   600,    44,    45,   609,   610,   601,
      46,   602,   606,   608,    47,   614,   615,   630,    65,   651,
     653,   652,   656,   657,   658,   662,   663,   664,   672,   673,
     676,   682,   686,   698,   680,   681,   684,   685,   699,    48,
     633,   727,   714,   708,   721,   728,    49,   722,   723,   724,
     725,   741,    50,   726,    51,   731,   729,   732,    52,   481,
       4,     5,     6,     7,   733,   497,   740,     8,     9,    10,
      11,    12,    13,    14,   747,    15,    16,    17,    18,    19,
      20,   715,   716,    23,   748,   749,    24,    25,    26,    27,
      28,    29,    30,    31,   750,   751,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,   752,   440,
     521,    44,    45,     0,     0,     0,    46,     0,     0,     0,
      47,   304,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,     0,     0,
       0,     0,    49,     0,     0,     0,     0,     0,    50,     0,
      51,     0,     0,     0,    52,   717,     4,     5,     6,     7,
       0,     0,     0,     8,     9,    10,    11,    12,    13,    14,
       0,    15,    16,    17,    18,    19,    20,    21,    22,    23,
       0,     0,    24,    25,    26,    27,    28,    29,    30,    31,
       0,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,     0,     0,     0,    44,    45,     0,
       0,     0,    46,     0,     0,     0,    47,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    90,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,    48,     0,     0,     0,     0,     0,     0,    49,    94,
      27,     0,     0,     0,    50,     0,    51,    32,     0,     0,
      52,     0,    37,    38,    39,     0,     0,    42,     0,     0,
       0,     0,     0,     4,     5,     6,     7,    46,     0,     0,
      90,    91,    92,    93,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    27,     0,     0,     0,    48,     0,     0,    32,
       0,     0,     0,    49,    37,    38,    39,     0,     0,    42,
       0,    51,   149,     0,     0,    52,     0,     0,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    90,    91,    92,    93,
      12,    13,    14,     0,    15,    16,    17,     0,    48,     0,
       0,     0,     0,     0,     0,    49,     0,    94,    27,     0,
       0,     0,     0,    51,   187,    32,     0,    52,     0,     0,
      37,    38,    39,     0,     0,    42,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    46,     0,     0,    90,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      27,     0,     0,     0,    48,     0,     0,    32,     0,     0,
       0,    49,    37,    38,    39,     0,     0,    42,     0,    51,
     196,     0,     0,    52,     0,     0,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    90,    91,    92,    93,    12,    13,
      14,     0,    15,    16,    17,     0,    48,     0,     0,     0,
       0,     0,     0,    49,     0,    94,    27,     0,     0,     0,
       0,    51,   203,    32,     0,    52,     0,     0,    37,    38,
      39,     0,     0,    42,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,     0,     0,   151,     4,     5,     6,
       7,     0,     0,     0,    90,    91,    92,    93,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    94,    27,     0,     0,    49,
       0,     0,     0,    32,     0,     0,     0,    51,    37,    38,
      39,    52,     0,    42,     0,     0,     0,     0,     0,     4,
       5,     6,     7,    46,     0,     0,    90,    91,    92,    93,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    27,     0,
       0,     0,    48,     0,     0,    32,     0,     0,     0,    49,
      37,    38,    39,     0,     0,    42,     0,    51,   397,     0,
       0,    52,     0,     0,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    90,    91,    92,    93,    12,    13,    14,     0,
      15,    16,    17,     0,    48,     0,     0,     0,     0,     0,
       0,    49,     0,    94,    27,     0,     0,     0,     0,    51,
     415,    32,     0,    52,     0,     0,    37,    38,    39,     0,
       0,    42,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    46,     0,     0,    90,    91,    92,    93,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    27,     0,     0,     0,
      48,     0,     0,    32,     0,     0,     0,    49,    37,    38,
      39,     0,     0,    42,     0,    51,   418,     0,     0,    52,
       0,     0,     0,    46,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      90,    91,    92,    93,   425,    13,    14,     0,    15,    16,
      17,     0,    48,     0,     0,     0,     0,     0,     0,    49,
       0,    94,    27,     0,     0,     0,     0,    51,   423,    32,
       0,    52,     0,     0,    37,    38,    39,     0,     0,    42,
       0,     0,     0,     0,     0,     4,     5,   337,     7,    46,
       0,     0,    90,    91,    92,    93,   338,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    27,     0,     0,     0,    48,     0,
       0,    32,     0,     0,     0,    49,    37,    38,    39,     0,
       0,    42,     0,    51,   426,     0,     0,    52,     0,     0,
       0,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    90,    91,
      92,    93,   425,    13,    14,     0,    15,    16,    17,     0,
      48,     0,     0,     0,     0,     0,     0,    49,     0,    94,
      27,     0,     0,     0,     0,    51,   149,    32,     0,    52,
       0,     0,    37,    38,    39,     0,     0,    42,     0,     0,
       0,     0,     0,     4,     5,     6,     7,    46,     0,     0,
      90,    91,    92,    93,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    27,     0,     0,     0,    48,     0,     0,    32,
       0,     0,     0,    49,    37,    38,    39,     0,     0,    42,
       0,    51,   520,     0,     0,    52,     0,     0,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    90,    91,    92,    93,
      12,    13,    14,     0,    15,    16,    17,     0,    48,     0,
       0,     0,     0,     0,     0,    49,     0,    94,    27,     0,
       0,     0,     0,    51,   660,    32,     0,    52,     0,     0,
      37,    38,    39,     0,     0,    42,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    46,     0,     0,    90,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      27,     0,     0,     0,    48,     0,     0,    32,     0,     0,
       0,    49,    37,    38,    39,     0,     0,    42,     0,   106,
       0,     0,     0,    52,     0,     0,     0,    46,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,    90,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,    94,
      27,     0,     0,    49,     0,     0,     0,    32,     0,     0,
       0,   108,    37,    38,    39,    52,     0,    42,     0,     0,
       0,     0,     0,     4,     5,     6,     7,    46,     0,     0,
      90,    91,    92,    93,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    27,     0,     0,     0,    48,     0,     0,    32,
       0,     0,     0,    49,    37,    38,    39,     0,     0,    42,
       0,   110,     0,     0,     0,    52,     0,     0,     0,    46,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
      90,    91,    92,    93,    12,    13,    14,     0,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,    94,    27,     0,     0,    49,     0,     0,     0,    32,
       0,     0,     0,   112,    37,    38,    39,    52,     0,    42,
       0,     0,     0,     0,     0,     4,     5,     6,     7,    46,
       0,     0,    90,    91,    92,    93,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    27,     0,     0,     0,    48,     0,
       0,    32,     0,     0,     0,    49,    37,    38,    39,     0,
       0,    42,     0,    51,     0,     0,     0,    52,     0,     0,
       0,    46,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,    90,    91,    92,    93,    12,    13,    14,     0,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
      48,     0,     0,    94,    27,     0,     0,    49,     0,     0,
       0,    32,     0,     0,     0,   127,    37,    38,    39,    52,
       0,    42,     0,     0,     0,     0,     0,     4,     5,     6,
       7,    46,     0,     0,    90,    91,    92,    93,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    27,     0,     0,     0,
      48,     0,     0,    32,     0,     0,     0,    49,    37,    38,
      39,     0,     0,    42,     0,   129,     0,     0,     0,    52,
       0,     0,     0,    46,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,    90,    91,    92,    93,    12,    13,
      14,     0,    15,    16,    17,     0,     0,     0,     0,     0,
       0,     0,    48,     0,     0,    94,    27,     0,     0,    49,
       0,     0,     0,    32,     0,     0,     0,    51,    37,    38,
      39,   135,     0,    42,     0,     0,     0,     0,     0,     4,
       5,   337,     7,    46,     0,     0,    90,    91,    92,    93,
     338,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    27,     0,
       0,     0,    48,     0,     0,    32,     0,     0,     0,    49,
      37,    38,    39,     0,     0,    42,     0,   335,     0,     0,
       0,    52,     0,     0,     0,    46,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,    90,    91,    92,    93,
      12,    13,    14,     0,    15,    16,    17,     0,     0,     0,
       0,     0,     0,     0,    48,     0,     0,    94,    27,     0,
       0,    49,     0,     0,     0,    32,     0,     0,     0,   339,
      37,    38,    39,    52,     0,    42,     0,     0,     0,     0,
       0,     4,     5,     6,     7,    46,     0,     0,    90,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      27,     0,     0,     0,    48,     0,     0,    32,     0,     0,
       0,    49,    37,    38,    39,     0,     0,    42,     0,   445,
       0,     0,     0,    52,     0,     0,     0,    46,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   146,    91,
      92,    93,    12,    13,    14,     0,    15,    16,    17,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,    94,
       0,     0,     0,    49,     0,     0,     0,    32,     0,     0,
       0,   465,    37,    38,    39,    52,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,     0,     0,     0,
       0,     0,     0,    49,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,    52
};

static const yytype_int16 yycheck[] =
{
       2,     0,   217,   161,   462,    14,   264,   363,    48,    49,
      14,    26,    10,    10,    25,    70,    14,    14,    22,    21,
      22,    23,    24,    25,    10,    82,    28,    29,    14,    31,
      10,    88,    34,    35,    14,    70,    14,    39,    10,    96,
      42,    10,    70,    14,    10,    14,   577,   102,    14,    51,
      10,    10,   107,    70,    14,    14,    94,    95,    37,    38,
      51,    52,    51,    52,    55,    93,    55,   102,    64,    65,
      66,    26,   107,    89,    29,    30,    93,    14,    80,    57,
      17,    83,    53,    85,    86,    14,    41,    42,    60,    61,
     348,   107,   107,    87,    96,   103,    89,   103,   107,   107,
     558,   107,    31,   107,   106,    14,   108,   104,   110,   103,
     112,    26,   108,    22,   107,    14,    53,   108,   104,   108,
      14,     0,   653,    22,   104,   127,    41,   129,    22,   108,
     132,   133,   134,   135,   136,   104,   394,   395,   104,   179,
     180,   181,   182,   674,   104,   104,   101,   678,    67,    58,
     152,   153,   154,   155,   156,   157,   105,    26,   373,    58,
      29,    30,   173,   174,    58,    89,    68,    69,   101,   226,
      68,    69,    41,    42,   231,   108,    89,   392,    87,    87,
     237,   100,   713,   107,   103,    14,   101,   102,    87,   720,
      10,    93,   101,    87,   107,    93,    89,   199,   200,   555,
     202,   107,   101,    26,   735,   207,   208,   101,   210,    14,
     212,   268,   670,     2,   107,    26,    12,    22,    41,   221,
     222,   223,   224,   754,   101,   102,   228,   229,   239,   240,
      41,    26,   101,   448,    29,    30,    67,    26,    27,   108,
      60,    61,    87,   242,   246,   247,    41,    36,   250,    12,
     252,    40,    41,    58,   412,   101,    26,   314,   103,   107,
     317,    26,   108,   478,    29,    30,   323,    85,   325,   100,
      87,    41,   103,   275,   276,   277,    41,    42,   101,   102,
      27,    28,    87,    26,   101,   108,    29,    30,   503,   504,
     101,   102,   101,    14,    86,   352,   101,   108,    41,   108,
      26,   358,    87,    29,    30,    94,   101,   102,    26,   101,
     102,    29,    30,   108,   316,    41,    42,     5,   103,    14,
     101,   101,   111,    41,   113,   114,    14,   108,   108,    82,
      14,   101,   102,   335,    87,   550,   101,   339,   108,   128,
     342,   130,   344,   108,   346,   101,    87,   100,   350,   351,
     103,    26,   108,    87,    29,    30,   413,    14,   101,   102,
     101,   101,   364,   152,    87,   108,    41,   101,   108,    91,
      92,   586,   587,   375,   632,   101,    82,   635,   101,    84,
     437,    87,   108,   101,   102,    10,    11,    12,    87,    14,
     108,    96,    97,    98,   100,    87,    62,   103,   400,   401,
     402,    10,   101,    10,    10,    14,    31,    14,    14,   101,
      35,    71,    72,    73,    74,    75,    76,   103,   420,   421,
     422,   107,   100,    83,   103,   103,   101,   102,   107,    89,
      90,    91,    92,   435,    14,    10,   438,   439,    13,    14,
      15,    16,   700,   445,   159,   160,   100,   162,   163,   164,
     165,   166,   167,   103,   169,   170,    14,   107,     3,     4,
       5,     6,   102,   465,   104,    10,    11,    12,    13,    14,
      15,    16,   261,    18,    19,    20,   105,   479,   480,   536,
     101,    14,   103,   485,    17,   101,    31,    32,   107,   102,
     547,   104,    10,   101,    39,    13,   101,    15,    16,    44,
      45,    46,   101,   102,    49,    77,    78,    79,    10,    11,
      12,     7,     8,     9,    59,    14,    68,    69,    87,    71,
      72,    73,    74,    75,    76,    77,    78,    79,   101,   102,
     532,    83,    84,    85,   102,   592,   104,    89,    90,    91,
      92,    93,    94,    88,    96,    97,    98,   102,   100,   104,
      95,   102,   108,   104,   611,   102,   101,   104,   103,   102,
      87,   104,   107,   102,   102,   104,   104,   102,   102,   104,
     104,   101,   102,   362,   363,   101,   102,   366,   367,   368,
     102,   102,   104,   104,    48,   102,   375,   104,   102,   591,
     104,   102,   107,   104,   383,   384,   385,   102,   102,   104,
     104,    27,    28,     3,     4,     5,     6,   609,   610,   103,
      10,    11,    12,    13,    14,    15,    16,   107,    18,    19,
      20,   102,   102,   104,   104,   102,   102,   104,   104,   101,
     102,    31,    32,   102,   633,   104,    27,    28,   640,    39,
     102,   101,   104,   645,    44,    45,    46,   103,   102,    49,
     104,   102,   102,   104,   104,   102,   102,   104,   104,    59,
     662,   663,   664,   102,   102,   104,   104,   102,   102,   104,
     104,   102,   102,   104,   104,   677,    90,   466,   101,   102,
      27,    28,    38,    39,   171,   172,   177,   178,    88,   691,
     106,   108,   101,   103,   696,    95,    14,   486,   103,    14,
     101,   101,    87,   103,    87,    10,    14,   107,     3,     4,
       5,     6,    14,   715,   716,    10,    11,    12,    13,    14,
      15,    16,   108,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   103,    27,    29,    30,    31,    32,    33,    34,
      35,    36,   103,   105,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    87,   101,   101,    54,
      55,   101,   101,   101,    59,    93,   555,    80,    63,   103,
     559,   560,   561,    14,   106,    87,    87,    87,   108,   101,
     101,   570,    67,    90,   101,   574,    14,   101,   107,    90,
      14,    14,   101,    88,    90,   101,    10,   101,   101,    10,
      95,   101,   101,   101,    10,    10,   101,    10,   103,   101,
     101,   108,   107,   108,   101,   108,    51,   101,   101,   101,
     101,   101,   101,    27,     3,     4,     5,     6,    14,   618,
     619,    10,    11,    12,    13,    14,    15,    16,    14,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    14,   106,
      29,    30,    31,    32,    33,    34,    35,    36,   108,   101,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,   101,    14,   101,    54,    55,    67,    67,   101,
      59,   101,   101,   101,    63,   101,   101,   108,   677,   101,
      14,   101,     7,     7,     7,    87,    87,    67,    10,    10,
      62,   101,   101,   101,    14,    14,    14,    14,   101,    88,
     107,    14,   108,   101,   101,    14,    95,   101,   101,   101,
     101,   710,   101,   101,   103,    14,   101,    14,   107,   108,
       3,     4,     5,     6,   101,   390,   101,    10,    11,    12,
      13,    14,    15,    16,   101,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   101,   101,    29,    30,    31,    32,
      33,    34,    35,    36,   101,   101,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,   101,   339,
     413,    54,    55,    -1,    -1,    -1,    59,    -1,    -1,    -1,
      63,   176,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,   101,    -1,
     103,    -1,    -1,    -1,   107,   108,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    -1,    -1,    -1,    54,    55,    -1,
      -1,    -1,    59,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    31,
      32,    -1,    -1,    -1,   101,    -1,   103,    39,    -1,    -1,
     107,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,
      -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,
      -1,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    31,    32,    -1,
      -1,    -1,    -1,   103,   104,    39,    -1,   107,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    59,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,
      -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,   103,
     104,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    31,    32,    -1,    -1,    -1,
      -1,   103,   104,    39,    -1,   107,    -1,    -1,    44,    45,
      46,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    59,    -1,    -1,    62,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    31,    32,    -1,    -1,    95,
      -1,    -1,    -1,    39,    -1,    -1,    -1,   103,    44,    45,
      46,   107,    -1,    49,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    59,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,    95,
      44,    45,    46,    -1,    -1,    49,    -1,   103,   104,    -1,
      -1,   107,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    -1,    31,    32,    -1,    -1,    -1,    -1,   103,
     104,    39,    -1,   107,    -1,    -1,    44,    45,    46,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    59,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      88,    -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,
      46,    -1,    -1,    49,    -1,   103,   104,    -1,    -1,   107,
      -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      -1,    31,    32,    -1,    -1,    -1,    -1,   103,   104,    39,
      -1,   107,    -1,    -1,    44,    45,    46,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    59,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,    -1,
      -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,    -1,
      -1,    49,    -1,   103,   104,    -1,    -1,   107,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    95,    -1,    31,
      32,    -1,    -1,    -1,    -1,   103,   104,    39,    -1,   107,
      -1,    -1,    44,    45,    46,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,
      -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,
      -1,   103,   104,    -1,    -1,   107,    -1,    -1,    -1,    59,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    -1,    31,    32,    -1,
      -1,    -1,    -1,   103,   104,    39,    -1,   107,    -1,    -1,
      44,    45,    46,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    59,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,
      -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    31,
      32,    -1,    -1,    95,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   103,    44,    45,    46,   107,    -1,    49,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    59,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    88,    -1,    -1,    39,
      -1,    -1,    -1,    95,    44,    45,    46,    -1,    -1,    49,
      -1,   103,    -1,    -1,    -1,   107,    -1,    -1,    -1,    59,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    31,    32,    -1,    -1,    95,    -1,    -1,    -1,    39,
      -1,    -1,    -1,   103,    44,    45,    46,   107,    -1,    49,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    59,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    88,    -1,
      -1,    39,    -1,    -1,    -1,    95,    44,    45,    46,    -1,
      -1,    49,    -1,   103,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    59,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    31,    32,    -1,    -1,    95,    -1,    -1,
      -1,    39,    -1,    -1,    -1,   103,    44,    45,    46,   107,
      -1,    49,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    59,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      88,    -1,    -1,    39,    -1,    -1,    -1,    95,    44,    45,
      46,    -1,    -1,    49,    -1,   103,    -1,    -1,    -1,   107,
      -1,    -1,    -1,    59,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    31,    32,    -1,    -1,    95,
      -1,    -1,    -1,    39,    -1,    -1,    -1,   103,    44,    45,
      46,   107,    -1,    49,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    59,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    88,    -1,    -1,    39,    -1,    -1,    -1,    95,
      44,    45,    46,    -1,    -1,    49,    -1,   103,    -1,    -1,
      -1,   107,    -1,    -1,    -1,    59,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    31,    32,    -1,
      -1,    95,    -1,    -1,    -1,    39,    -1,    -1,    -1,   103,
      44,    45,    46,   107,    -1,    49,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    59,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    88,    -1,    -1,    39,    -1,    -1,
      -1,    95,    44,    45,    46,    -1,    -1,    49,    -1,   103,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    59,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    31,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    39,    -1,    -1,
      -1,   103,    44,    45,    46,   107,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    59,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,   107
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
     101,   103,   107,   112,   113,   115,   116,   117,   118,   119,
     120,   122,   125,   126,   129,   134,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   149,    82,
      87,   100,   103,   105,    89,   107,    87,   100,   103,    14,
      10,    11,    12,    13,    31,   101,   103,   134,   136,   148,
      10,    11,    12,    14,    31,    35,   103,   136,   103,   136,
     103,   136,   103,   136,   136,   141,    14,   134,   134,   136,
     101,   136,    10,    14,   136,    12,    12,   103,   136,   103,
     136,   134,   107,   135,   135,   107,   135,   136,   134,   134,
     136,    14,    14,    14,    31,    14,    10,   145,   145,   104,
     136,    62,   111,    26,    29,    30,    41,    42,   101,    77,
      78,    79,    71,    72,    73,    74,    75,    76,    83,    89,
      90,    91,    92,    68,    69,    93,    85,    94,    95,    84,
      96,    97,    98,   100,    14,   136,    14,   104,   148,   136,
      14,   136,   136,    14,    17,    53,   104,   148,   101,    87,
     105,    89,   107,   104,   136,   148,   101,   102,    87,   101,
      87,   101,    87,   101,    10,    11,    12,    14,    10,    14,
     136,    26,    29,    30,    41,   101,   102,   136,    26,    41,
     101,   102,   136,   134,   136,   134,    70,   102,   134,    68,
      69,   103,   107,   123,   101,   101,    87,    87,   107,    89,
     107,    89,   107,   136,   134,   136,   134,   136,   136,   136,
     136,    48,   127,   107,   103,    14,    57,   107,   102,   104,
     108,    10,    11,    12,    13,    24,    25,    33,   108,   136,
     136,   136,   136,   136,   136,   139,   139,   137,   139,   139,
     139,   139,   139,   139,     7,     8,     9,   139,   139,   140,
     140,   141,   141,   142,   143,   144,   144,   145,   145,   145,
     145,    14,    17,   101,   103,   101,    87,   103,   104,   106,
      90,   108,   101,   103,    14,   103,   104,   136,   136,    14,
     136,   101,   104,   136,   136,   103,   136,     5,    14,   103,
     132,   136,    87,   101,    87,   101,    87,   101,   103,   123,
      87,    87,   102,   136,   136,   136,   136,   148,   102,   136,
     136,   148,   104,    27,    28,   114,   104,    10,   148,   141,
     141,    10,    14,   104,   133,   111,   136,   136,   121,    14,
     136,    14,   136,   104,    27,   104,   108,   134,    14,    22,
     107,   128,   104,   133,   103,   103,   130,   104,   148,   100,
     105,   107,    87,   136,   136,   136,   108,   101,   101,   101,
     101,   101,    80,   103,    14,   104,   148,   136,   104,   148,
      87,    87,    87,   104,   148,    14,   104,   131,   148,   106,
      90,   108,   101,   101,   136,    86,   101,   102,    67,    67,
     132,   101,   102,   101,   136,   103,   136,   136,   104,   133,
     136,   136,   148,   101,   101,   101,   101,   101,   148,   101,
     101,   101,   134,   113,   134,   103,   136,   134,   134,    70,
     134,    70,    70,    10,    60,    61,   123,   102,   104,    24,
      25,   108,   136,   101,   101,    37,    38,   108,    90,   108,
      90,   108,   134,   134,   134,    14,    14,   128,    51,    52,
      55,   108,   123,   104,   104,   133,   133,    64,    65,    66,
     108,   104,    14,   136,   136,   136,   108,   108,   108,   137,
     104,   131,   148,   101,   104,   101,   101,   104,   136,   136,
     136,   104,    67,   102,   104,   104,   102,   136,   148,   136,
     136,   104,     5,    14,   101,   136,   101,   102,   101,   123,
     104,   101,   101,   104,   104,    27,   114,   136,   134,    10,
      10,    10,    10,    10,    10,    14,   123,   136,   136,   108,
     136,   134,   101,   101,    27,   108,    14,    53,    10,    13,
      14,    15,    16,    51,   123,   123,   104,   104,    14,    14,
      14,    87,   103,   106,   108,   108,   104,   104,   101,   101,
     101,   101,   101,   136,    14,   148,   101,   101,   101,    67,
      67,   102,   148,   123,   101,   101,   113,   134,   104,    27,
     114,   134,   134,   134,    10,    60,    61,   108,   108,   134,
     108,   134,   103,   107,   124,   103,   124,    14,    22,    58,
      87,   101,    14,    22,    58,    87,   101,    10,    13,    15,
      16,   101,   101,    14,   123,   123,     7,     7,     7,   136,
     104,   148,    87,    87,    67,   104,   136,   136,   148,   101,
     134,   134,    10,    10,   104,   133,    62,   111,   104,   133,
      14,    14,   101,   136,    14,    14,   101,   136,    14,    22,
      58,    87,   101,    14,    22,    58,    87,   101,   101,   101,
     103,   124,   108,   108,   104,   136,   136,   136,   101,   104,
      27,   114,   124,   104,   108,    24,    25,   108,   136,   124,
     104,   101,   101,   101,   101,   101,   101,    14,    14,   101,
     136,    14,    14,   101,   136,   104,   133,   108,   108,   108,
     101,   134,   124,   136,   136,   108,   124,   101,   101,   101,
     101,   101,   101,   124,   104,   108,   108,   124
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
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   114,   114,   114,   114,   114,   114,   115,   115,   116,
     116,   116,   116,   117,   117,   118,   119,   119,   119,   119,
     119,   119,   120,   120,   121,   121,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   124,   124,   124,   124,   124,   125,   126,   127,   127,
     127,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   129,   130,   130,   130,   130,
     131,   131,   132,   132,   132,   132,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   135,   136,   136,   136,
     137,   137,   138,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   138,   138,   138,   139,   139,   139,   140,
     140,   140,   141,   141,   142,   142,   143,   143,   143,   144,
     144,   144,   144,   144,   145,   145,   145,   146,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   148,   148,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149
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
       5,     5,     5,     5,     5,     5,     2,     1,     1,     1,
       1,     1,     2,     4,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     7,     7,     3,     5,     5,     4,
       6,     3,     5,     5,     7,     4,     6,     5,     3,     5,
       7,     3,     5,     5,     3,     2,     7,     7,     5,     7,
       5,     3,     5,     7,     0,     4,     6,     5,     3,     7,
       6,     4,     7,     6,     7,     6,     5,     3,     4,     5,
       5,     3,     3,     4,     5,     5,     6,     5,     0,     3,
       3,     0,     4,     4,     4,     5,     5,     4,     5,     5,
       6,     6,     7,     7,     6,     6,     6,     6,     5,     5,
       6,     6,     7,     7,     7,     7,     7,     6,     4,     7,
       6,     4,     8,     7,     5,     5,     0,     4,     4,     4,
       3,     5,     3,     3,     5,     5,     1,     2,     3,     3,
       3,     4,     5,     5,     3,     3,     4,     5,     5,     9,
       8,     8,     6,     9,     9,     5,     3,     3,     2,     1,
       5,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     2,     2,     1,     1,     4,     3,
       4,     3,     6,     6,     5,     3,     3,     6,     5,     4,
       4,     6,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     4,     5,     5,     1,
       1,     1,     1,     2,     3,     4,     5,     1,     2
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
#line 258 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2264 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 268 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2270 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 269 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2276 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 273 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2282 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 275 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2288 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 277 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2294 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_HASH ';'  */
#line 279 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2300 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 281 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2306 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 283 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2314 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 287 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2321 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 290 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2329 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 294 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2335 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 296 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2341 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 298 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2347 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 300 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2353 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 302 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2359 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 304 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2367 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 308 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2375 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 312 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2381 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 314 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2387 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 316 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2393 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 318 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2399 "raku.tab.c"
    break;

  case 24: /* stmt: KW_USE IDENT ';'  */
#line 320 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2405 "raku.tab.c"
    break;

  case 25: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 322 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2411 "raku.tab.c"
    break;

  case 26: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 324 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2417 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 326 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2423 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 328 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2429 "raku.tab.c"
    break;

  case 29: /* stmt: TESTOP ';'  */
#line 330 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2435 "raku.tab.c"
    break;

  case 30: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 332 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2441 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP '(' ')' ';'  */
#line 334 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2447 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP arg_list ';'  */
#line 336 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2453 "raku.tab.c"
    break;

  case 33: /* stmt: KW_SAY expr ';'  */
#line 338 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2459 "raku.tab.c"
    break;

  case 34: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 340 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2466 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 343 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2473 "raku.tab.c"
    break;

  case 36: /* stmt: KW_PRINT expr ';'  */
#line 346 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2479 "raku.tab.c"
    break;

  case 37: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 348 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2486 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 351 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2493 "raku.tab.c"
    break;

  case 39: /* stmt: KW_TAKE expr ';'  */
#line 354 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2499 "raku.tab.c"
    break;

  case 40: /* stmt: KW_RETURN expr ';'  */
#line 356 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2505 "raku.tab.c"
    break;

  case 41: /* stmt: KW_RETURN ';'  */
#line 358 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2511 "raku.tab.c"
    break;

  case 42: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 360 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2517 "raku.tab.c"
    break;

  case 43: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 362 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2526 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 367 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2534 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 371 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2542 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 375 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2551 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 380 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 2559 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '(' arg_list ')' ';'  */
#line 384 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-6].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args = (yyvsp[-2].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 2570 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR '.' IDENT '(' ')' ';'  */
#line 391 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node) = c; }
#line 2579 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 396 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2586 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 399 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2593 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 402 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2600 "raku.tab.c"
    break;

  case 53: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 405 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 2607 "raku.tab.c"
    break;

  case 54: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 408 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 2614 "raku.tab.c"
    break;

  case 55: /* stmt: expr KW_IF expr ';'  */
#line 411 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2620 "raku.tab.c"
    break;

  case 56: /* stmt: expr KW_UNLESS expr ';'  */
#line 413 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2626 "raku.tab.c"
    break;

  case 57: /* stmt: expr KW_WHILE expr ';'  */
#line 415 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 2632 "raku.tab.c"
    break;

  case 58: /* stmt: expr KW_UNTIL expr ';'  */
#line 417 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 2638 "raku.tab.c"
    break;

  case 59: /* stmt: expr KW_FOR expr ';'  */
#line 419 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 2645 "raku.tab.c"
    break;

  case 60: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 422 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 2652 "raku.tab.c"
    break;

  case 61: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 425 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 2659 "raku.tab.c"
    break;

  case 62: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 428 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 2667 "raku.tab.c"
    break;

  case 63: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 432 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 2674 "raku.tab.c"
    break;

  case 64: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 435 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 2681 "raku.tab.c"
    break;

  case 65: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 438 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 2688 "raku.tab.c"
    break;

  case 66: /* stmt: expr ';'  */
#line 440 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 2694 "raku.tab.c"
    break;

  case 67: /* stmt: ';'  */
#line 441 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 2700 "raku.tab.c"
    break;

  case 68: /* stmt: if_stmt  */
#line 442 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2706 "raku.tab.c"
    break;

  case 69: /* stmt: while_stmt  */
#line 443 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2712 "raku.tab.c"
    break;

  case 70: /* stmt: for_stmt  */
#line 444 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2718 "raku.tab.c"
    break;

  case 71: /* stmt: given_stmt  */
#line 445 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2724 "raku.tab.c"
    break;

  case 72: /* stmt: KW_TRY block  */
#line 447 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2730 "raku.tab.c"
    break;

  case 73: /* stmt: KW_TRY block KW_CATCH block  */
#line 449 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2736 "raku.tab.c"
    break;

  case 74: /* stmt: KW_CATCH block  */
#line 451 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2742 "raku.tab.c"
    break;

  case 75: /* stmt: block  */
#line 453 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 2748 "raku.tab.c"
    break;

  case 76: /* stmt: unless_stmt  */
#line 454 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2754 "raku.tab.c"
    break;

  case 77: /* stmt: until_stmt  */
#line 455 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2760 "raku.tab.c"
    break;

  case 78: /* stmt: repeat_stmt  */
#line 456 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2766 "raku.tab.c"
    break;

  case 79: /* stmt: sub_decl  */
#line 457 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2772 "raku.tab.c"
    break;

  case 80: /* stmt: class_decl  */
#line 458 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2778 "raku.tab.c"
    break;

  case 81: /* stmt: role_decl  */
#line 459 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2784 "raku.tab.c"
    break;

  case 82: /* stmt: grammar_decl  */
#line 460 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 2790 "raku.tab.c"
    break;

  case 83: /* if_stmt: KW_IF '(' expr ')' block  */
#line 464 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2796 "raku.tab.c"
    break;

  case 84: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 466 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2802 "raku.tab.c"
    break;

  case 85: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 468 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2808 "raku.tab.c"
    break;

  case 86: /* if_stmt: KW_IF expr block  */
#line 470 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2814 "raku.tab.c"
    break;

  case 87: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 472 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2820 "raku.tab.c"
    break;

  case 88: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2826 "raku.tab.c"
    break;

  case 89: /* if_stmt: KW_IF expr block elsif_tail  */
#line 476 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2832 "raku.tab.c"
    break;

  case 90: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 478 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2838 "raku.tab.c"
    break;

  case 91: /* elsif_tail: KW_ELSIF expr block  */
#line 482 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2844 "raku.tab.c"
    break;

  case 92: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 484 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2850 "raku.tab.c"
    break;

  case 93: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 486 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2856 "raku.tab.c"
    break;

  case 94: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 488 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2862 "raku.tab.c"
    break;

  case 95: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 490 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2868 "raku.tab.c"
    break;

  case 96: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 492 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2874 "raku.tab.c"
    break;

  case 97: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 496 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 2880 "raku.tab.c"
    break;

  case 98: /* while_stmt: KW_WHILE expr block  */
#line 498 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 2886 "raku.tab.c"
    break;

  case 99: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 502 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2892 "raku.tab.c"
    break;

  case 100: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2898 "raku.tab.c"
    break;

  case 101: /* unless_stmt: KW_UNLESS expr block  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2904 "raku.tab.c"
    break;

  case 102: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 508 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2910 "raku.tab.c"
    break;

  case 103: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 512 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2916 "raku.tab.c"
    break;

  case 104: /* until_stmt: KW_UNTIL expr block  */
#line 514 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2922 "raku.tab.c"
    break;

  case 105: /* repeat_stmt: KW_REPEAT block  */
#line 518 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 2928 "raku.tab.c"
    break;

  case 106: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 522 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 2938 "raku.tab.c"
    break;

  case 107: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 528 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 1; ast_push(r, ex);
          (yyval.node) = r; }
#line 2948 "raku.tab.c"
    break;

  case 108: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 534 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2957 "raku.tab.c"
    break;

  case 109: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 539 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2967 "raku.tab.c"
    break;

  case 110: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 545 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2976 "raku.tab.c"
    break;

  case 111: /* for_stmt: KW_FOR expr block  */
#line 550 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 2983 "raku.tab.c"
    break;

  case 112: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 555 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 2995 "raku.tab.c"
    break;

  case 113: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 563 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3008 "raku.tab.c"
    break;

  case 114: /* when_list: %empty  */
#line 573 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3014 "raku.tab.c"
    break;

  case 115: /* when_list: when_list KW_WHEN expr block  */
#line 575 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3022 "raku.tab.c"
    break;

  case 116: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 581 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3034 "raku.tab.c"
    break;

  case 117: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 589 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3044 "raku.tab.c"
    break;

  case 118: /* sub_decl: KW_SUB IDENT sub_body  */
#line 595 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3054 "raku.tab.c"
    break;

  case 119: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 601 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3066 "raku.tab.c"
    break;

  case 120: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 609 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3076 "raku.tab.c"
    break;

  case 121: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 615 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3086 "raku.tab.c"
    break;

  case 122: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 621 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3099 "raku.tab.c"
    break;

  case 123: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 630 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3110 "raku.tab.c"
    break;

  case 124: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 637 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3123 "raku.tab.c"
    break;

  case 125: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 646 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3136 "raku.tab.c"
    break;

  case 126: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 655 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3147 "raku.tab.c"
    break;

  case 127: /* sub_body: '{' stmt_list '}'  */
#line 663 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3153 "raku.tab.c"
    break;

  case 128: /* sub_body: '{' stmt_list expr '}'  */
#line 665 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3160 "raku.tab.c"
    break;

  case 129: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 668 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3166 "raku.tab.c"
    break;

  case 130: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 670 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3172 "raku.tab.c"
    break;

  case 131: /* method_body: '{' stmt_list '}'  */
#line 673 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3178 "raku.tab.c"
    break;

  case 132: /* method_body: '{' YADA '}'  */
#line 674 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3184 "raku.tab.c"
    break;

  case 133: /* method_body: '{' stmt_list expr '}'  */
#line 676 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3191 "raku.tab.c"
    break;

  case 134: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 679 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3197 "raku.tab.c"
    break;

  case 135: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 681 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3203 "raku.tab.c"
    break;

  case 136: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 685 "raku.y"
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
#line 3221 "raku.tab.c"
    break;

  case 137: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 701 "raku.y"
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
#line 3238 "raku.tab.c"
    break;

  case 138: /* is_clauses: %empty  */
#line 715 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 3244 "raku.tab.c"
    break;

  case 139: /* is_clauses: is_clauses IDENT IDENT  */
#line 717 "raku.y"
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
#line 3260 "raku.tab.c"
    break;

  case 140: /* is_clauses: is_clauses TESTOP IDENT  */
#line 729 "raku.y"
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
#line 3276 "raku.tab.c"
    break;

  case 141: /* class_body_list: %empty  */
#line 742 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3282 "raku.tab.c"
    break;

  case 142: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 744 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3289 "raku.tab.c"
    break;

  case 143: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 747 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3296 "raku.tab.c"
    break;

  case 144: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 750 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3303 "raku.tab.c"
    break;

  case 145: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 753 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3310 "raku.tab.c"
    break;

  case 146: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 756 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3317 "raku.tab.c"
    break;

  case 147: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 759 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 3324 "raku.tab.c"
    break;

  case 148: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 762 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3331 "raku.tab.c"
    break;

  case 149: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 765 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3338 "raku.tab.c"
    break;

  case 150: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 768 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3345 "raku.tab.c"
    break;

  case 151: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 771 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3352 "raku.tab.c"
    break;

  case 152: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 774 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3359 "raku.tab.c"
    break;

  case 153: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 777 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3366 "raku.tab.c"
    break;

  case 154: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 780 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3377 "raku.tab.c"
    break;

  case 155: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 787 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3388 "raku.tab.c"
    break;

  case 156: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 794 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3399 "raku.tab.c"
    break;

  case 157: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 801 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3410 "raku.tab.c"
    break;

  case 158: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 808 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3418 "raku.tab.c"
    break;

  case 159: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 812 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 3426 "raku.tab.c"
    break;

  case 160: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 816 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3434 "raku.tab.c"
    break;

  case 161: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 820 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 3442 "raku.tab.c"
    break;

  case 162: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 824 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3453 "raku.tab.c"
    break;

  case 163: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 831 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3464 "raku.tab.c"
    break;

  case 164: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 838 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3475 "raku.tab.c"
    break;

  case 165: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 845 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 3486 "raku.tab.c"
    break;

  case 166: /* class_body_list: class_body_list KW_METHOD IDENT '(' param_list ')' method_body  */
#line 852 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3500 "raku.tab.c"
    break;

  case 167: /* class_body_list: class_body_list KW_METHOD IDENT '(' ')' method_body  */
#line 862 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3512 "raku.tab.c"
    break;

  case 168: /* class_body_list: class_body_list KW_METHOD IDENT method_body  */
#line 870 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3524 "raku.tab.c"
    break;

  case 169: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 878 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3537 "raku.tab.c"
    break;

  case 170: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 887 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 3548 "raku.tab.c"
    break;

  case 171: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 894 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 3559 "raku.tab.c"
    break;

  case 172: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' param_list ')' method_body  */
#line 901 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 3573 "raku.tab.c"
    break;

  case 173: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT '(' ')' method_body  */
#line 911 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 3585 "raku.tab.c"
    break;

  case 174: /* class_body_list: class_body_list KW_MULTI KW_METHOD IDENT method_body  */
#line 919 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 3597 "raku.tab.c"
    break;

  case 175: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 929 "raku.y"
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
#line 3614 "raku.tab.c"
    break;

  case 176: /* grammar_body_list: %empty  */
#line 943 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 3620 "raku.tab.c"
    break;

  case 177: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 945 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3629 "raku.tab.c"
    break;

  case 178: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 950 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3638 "raku.tab.c"
    break;

  case 179: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 955 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 3647 "raku.tab.c"
    break;

  case 180: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 962 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 3655 "raku.tab.c"
    break;

  case 181: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 966 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 3663 "raku.tab.c"
    break;

  case 182: /* pair_list: IDENT OP_FATARROW expr  */
#line 972 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3669 "raku.tab.c"
    break;

  case 183: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 974 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 3675 "raku.tab.c"
    break;

  case 184: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 976 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3681 "raku.tab.c"
    break;

  case 185: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 978 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 3687 "raku.tab.c"
    break;

  case 186: /* param_list: VAR_SCALAR  */
#line 981 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 3693 "raku.tab.c"
    break;

  case 187: /* param_list: IDENT VAR_SCALAR  */
#line 982 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3699 "raku.tab.c"
    break;

  case 188: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 983 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3705 "raku.tab.c"
    break;

  case 189: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 984 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3711 "raku.tab.c"
    break;

  case 190: /* param_list: param_list ',' VAR_SCALAR  */
#line 985 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 3717 "raku.tab.c"
    break;

  case 191: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 986 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 3723 "raku.tab.c"
    break;

  case 192: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 987 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3729 "raku.tab.c"
    break;

  case 193: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 988 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 3735 "raku.tab.c"
    break;

  case 194: /* block: '{' stmt_list '}'  */
#line 991 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3741 "raku.tab.c"
    break;

  case 195: /* block: '{' YADA '}'  */
#line 992 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3747 "raku.tab.c"
    break;

  case 196: /* block: '{' stmt_list expr '}'  */
#line 994 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 3753 "raku.tab.c"
    break;

  case 197: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 996 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3759 "raku.tab.c"
    break;

  case 198: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 998 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3765 "raku.tab.c"
    break;

  case 199: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1000 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3773 "raku.tab.c"
    break;

  case 200: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1004 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3780 "raku.tab.c"
    break;

  case 201: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1007 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 3787 "raku.tab.c"
    break;

  case 202: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1010 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 3794 "raku.tab.c"
    break;

  case 203: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1013 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3801 "raku.tab.c"
    break;

  case 204: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1016 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 3808 "raku.tab.c"
    break;

  case 205: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1019 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 3814 "raku.tab.c"
    break;

  case 206: /* closure: '{' expr '}'  */
#line 1022 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 3820 "raku.tab.c"
    break;

  case 207: /* expr: VAR_SCALAR '=' expr  */
#line 1025 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 3826 "raku.tab.c"
    break;

  case 208: /* expr: KW_GATHER block  */
#line 1026 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 3836 "raku.tab.c"
    break;

  case 209: /* expr: tern_expr  */
#line 1031 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3842 "raku.tab.c"
    break;

  case 210: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1035 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 3848 "raku.tab.c"
    break;

  case 211: /* tern_expr: cmp_expr  */
#line 1036 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 3854 "raku.tab.c"
    break;

  case 212: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1039 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3860 "raku.tab.c"
    break;

  case 213: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1040 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3866 "raku.tab.c"
    break;

  case 214: /* cmp_expr: jct_expr OP_EQ jct_expr  */
#line 1041 "raku.y"
                                { (yyval.node)=expr_binary(TT_EQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3872 "raku.tab.c"
    break;

  case 215: /* cmp_expr: jct_expr OP_NE jct_expr  */
#line 1042 "raku.y"
                                { (yyval.node)=expr_binary(TT_NE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3878 "raku.tab.c"
    break;

  case 216: /* cmp_expr: jct_expr '<' jct_expr  */
#line 1043 "raku.y"
                                { (yyval.node)=expr_binary(TT_LT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3884 "raku.tab.c"
    break;

  case 217: /* cmp_expr: jct_expr '>' jct_expr  */
#line 1044 "raku.y"
                                { (yyval.node)=expr_binary(TT_GT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3890 "raku.tab.c"
    break;

  case 218: /* cmp_expr: jct_expr OP_LE jct_expr  */
#line 1045 "raku.y"
                                { (yyval.node)=expr_binary(TT_LE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3896 "raku.tab.c"
    break;

  case 219: /* cmp_expr: jct_expr OP_GE jct_expr  */
#line 1046 "raku.y"
                                { (yyval.node)=expr_binary(TT_GE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3902 "raku.tab.c"
    break;

  case 220: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1047 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3908 "raku.tab.c"
    break;

  case 221: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1048 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3914 "raku.tab.c"
    break;

  case 222: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1050 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 3924 "raku.tab.c"
    break;

  case 223: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1056 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 3934 "raku.tab.c"
    break;

  case 224: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1062 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 3944 "raku.tab.c"
    break;

  case 225: /* cmp_expr: jct_expr  */
#line 1067 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3950 "raku.tab.c"
    break;

  case 226: /* jct_expr: jct_expr '|' range_expr  */
#line 1070 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3956 "raku.tab.c"
    break;

  case 227: /* jct_expr: jct_expr '&' range_expr  */
#line 1071 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3962 "raku.tab.c"
    break;

  case 228: /* jct_expr: range_expr  */
#line 1072 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 3968 "raku.tab.c"
    break;

  case 229: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1075 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3974 "raku.tab.c"
    break;

  case 230: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1076 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3980 "raku.tab.c"
    break;

  case 231: /* range_expr: add_expr  */
#line 1077 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 3986 "raku.tab.c"
    break;

  case 232: /* add_expr: add_expr '~' repl_expr  */
#line 1080 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3992 "raku.tab.c"
    break;

  case 233: /* add_expr: repl_expr  */
#line 1081 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 3998 "raku.tab.c"
    break;

  case 234: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1084 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4004 "raku.tab.c"
    break;

  case 235: /* repl_expr: addsub_expr  */
#line 1085 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 4010 "raku.tab.c"
    break;

  case 236: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1088 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4016 "raku.tab.c"
    break;

  case 237: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1089 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4022 "raku.tab.c"
    break;

  case 238: /* addsub_expr: mul_expr  */
#line 1090 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 4028 "raku.tab.c"
    break;

  case 239: /* mul_expr: mul_expr '*' unary_expr  */
#line 1093 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4034 "raku.tab.c"
    break;

  case 240: /* mul_expr: mul_expr '/' unary_expr  */
#line 1094 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4040 "raku.tab.c"
    break;

  case 241: /* mul_expr: mul_expr '%' unary_expr  */
#line 1095 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4046 "raku.tab.c"
    break;

  case 242: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1096 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4052 "raku.tab.c"
    break;

  case 243: /* mul_expr: unary_expr  */
#line 1097 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 4058 "raku.tab.c"
    break;

  case 244: /* unary_expr: '-' unary_expr  */
#line 1100 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 4064 "raku.tab.c"
    break;

  case 245: /* unary_expr: '!' unary_expr  */
#line 1101 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 4070 "raku.tab.c"
    break;

  case 246: /* unary_expr: postfix_expr  */
#line 1102 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 4076 "raku.tab.c"
    break;

  case 247: /* postfix_expr: call_expr  */
#line 1104 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 4082 "raku.tab.c"
    break;

  case 248: /* call_expr: IDENT '(' arg_list ')'  */
#line 1107 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4091 "raku.tab.c"
    break;

  case 249: /* call_expr: IDENT '(' ')'  */
#line 1111 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 4097 "raku.tab.c"
    break;

  case 250: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1113 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 4106 "raku.tab.c"
    break;

  case 251: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1118 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 4112 "raku.tab.c"
    break;

  case 252: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1120 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4122 "raku.tab.c"
    break;

  case 253: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1126 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4132 "raku.tab.c"
    break;

  case 254: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1132 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 4140 "raku.tab.c"
    break;

  case 255: /* call_expr: IDENT '.' KW_NEW  */
#line 1136 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4148 "raku.tab.c"
    break;

  case 256: /* call_expr: IDENT '.' IDENT  */
#line 1140 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4157 "raku.tab.c"
    break;

  case 257: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1145 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4168 "raku.tab.c"
    break;

  case 258: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1152 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4177 "raku.tab.c"
    break;

  case 259: /* call_expr: IDENT '.' CARET IDENT  */
#line 1157 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4187 "raku.tab.c"
    break;

  case 260: /* call_expr: atom '.' CARET IDENT  */
#line 1163 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4197 "raku.tab.c"
    break;

  case 261: /* call_expr: atom '.' IDENT '(' arg_list ')'  */
#line 1169 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4208 "raku.tab.c"
    break;

  case 262: /* call_expr: atom '.' IDENT '(' named_arg_list ')'  */
#line 1176 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 4219 "raku.tab.c"
    break;

  case 263: /* call_expr: atom '.' IDENT '(' ')'  */
#line 1183 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4228 "raku.tab.c"
    break;

  case 264: /* call_expr: atom '.' IDENT  */
#line 1188 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 4237 "raku.tab.c"
    break;

  case 265: /* call_expr: KW_DIE expr  */
#line 1193 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 4243 "raku.tab.c"
    break;

  case 266: /* call_expr: KW_MAP closure expr  */
#line 1195 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4249 "raku.tab.c"
    break;

  case 267: /* call_expr: KW_GREP closure expr  */
#line 1197 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4255 "raku.tab.c"
    break;

  case 268: /* call_expr: KW_SORT expr  */
#line 1199 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4261 "raku.tab.c"
    break;

  case 269: /* call_expr: KW_SORT closure expr  */
#line 1201 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4267 "raku.tab.c"
    break;

  case 270: /* call_expr: atom  */
#line 1202 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 4273 "raku.tab.c"
    break;

  case 271: /* arg_list: expr  */
#line 1205 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 4279 "raku.tab.c"
    break;

  case 272: /* arg_list: arg_list ',' expr  */
#line 1206 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 4285 "raku.tab.c"
    break;

  case 273: /* atom: LIT_INT  */
#line 1209 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 4291 "raku.tab.c"
    break;

  case 274: /* atom: LIT_FLOAT  */
#line 1210 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 4297 "raku.tab.c"
    break;

  case 275: /* atom: LIT_STR  */
#line 1211 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 4303 "raku.tab.c"
    break;

  case 276: /* atom: WORDLIST  */
#line 1213 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 4315 "raku.tab.c"
    break;

  case 277: /* atom: LIT_INTERP_STR  */
#line 1220 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 4321 "raku.tab.c"
    break;

  case 278: /* atom: VAR_SCALAR  */
#line 1221 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4327 "raku.tab.c"
    break;

  case 279: /* atom: VAR_ARRAY  */
#line 1222 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4333 "raku.tab.c"
    break;

  case 280: /* atom: VAR_HASH  */
#line 1223 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4339 "raku.tab.c"
    break;

  case 281: /* atom: VAR_CAPTURE  */
#line 1225 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4347 "raku.tab.c"
    break;

  case 282: /* atom: VAR_FH  */
#line 1229 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 4355 "raku.tab.c"
    break;

  case 283: /* atom: VAR_NAMED_CAPTURE  */
#line 1233 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 4362 "raku.tab.c"
    break;

  case 284: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1236 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4368 "raku.tab.c"
    break;

  case 285: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1238 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4374 "raku.tab.c"
    break;

  case 286: /* atom: VAR_HASH '{' expr '}'  */
#line 1240 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4380 "raku.tab.c"
    break;

  case 287: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1242 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 4386 "raku.tab.c"
    break;

  case 288: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1244 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4392 "raku.tab.c"
    break;

  case 289: /* atom: IDENT  */
#line 1245 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 4398 "raku.tab.c"
    break;

  case 290: /* atom: VAR_TWIGIL  */
#line 1247 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4406 "raku.tab.c"
    break;

  case 291: /* atom: VAR_ARRAY_TWIGIL  */
#line 1251 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4414 "raku.tab.c"
    break;

  case 292: /* atom: VAR_HASH_TWIGIL  */
#line 1255 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 4422 "raku.tab.c"
    break;

  case 293: /* atom: '(' ')'  */
#line 1258 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 4428 "raku.tab.c"
    break;

  case 294: /* atom: '(' expr ')'  */
#line 1259 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 4434 "raku.tab.c"
    break;

  case 295: /* atom: '(' expr ',' ')'  */
#line 1261 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 4440 "raku.tab.c"
    break;

  case 296: /* atom: '(' expr ',' arg_list ')'  */
#line 1263 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 4447 "raku.tab.c"
    break;

  case 297: /* atom: block  */
#line 1265 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4453 "raku.tab.c"
    break;

  case 298: /* atom: KW_SUB block  */
#line 1266 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 4459 "raku.tab.c"
    break;


#line 4463 "raku.tab.c"

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

#line 1268 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
