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
static tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body) {
    tree_t *n = ast_node_new(TT_CLOOP);
    expr_add_child(n, init); expr_add_child(n, cond); expr_add_child(n, incr); expr_add_child(n, body);
    return n;
}
static tree_t *rk_incdec(const char *var, int add) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_ASSIGN, var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one));
}
static tree_t *rk_destructure(ExprList *targets, tree_t *rhs_arr) {
    static int __destr_uid = 0;
    char tmp[32]; snprintf(tmp, sizeof tmp, "__destr_%d", __destr_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    tree_t *bind = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rhs_arr); expr_add_child(seq, bind);
    int n = targets ? targets->count : 0;
    for (int i = 0; i < n; i++) {
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, tmp));
        tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = i; expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, targets->items[i], get));
    }
    if (targets) exprlist_free(targets);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_range_ex(tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return expr_binary(TT_TO, lo, d); }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static int rk_is_chain_cmp(tree_e k) {
    return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return expr_binary(TT_SEQ, left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
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

#line 328 "raku.tab.c"

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
  YYSYMBOL_KW_WITH = 39,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 40,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 41,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 42,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 43,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 44,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 45,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 46,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 47,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 48,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 49,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 50,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 51,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 52,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 53,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 54,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 55,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 56,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 57,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 58,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 59,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 60,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 61,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 62,                   /* OP_NAME  */
  YYSYMBOL_KW_HANDLES = 63,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 64,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 65,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 66,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 67,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 68,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 69,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 70,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 71,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 72,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 73,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 74,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 75,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 76,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 77,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 78,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 79,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 80,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 81,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 82,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 83,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 84,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 85,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 86,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 87,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 88,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 89,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 90,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 91,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 92,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 93,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 94,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 95,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 96,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 97,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 98,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 99,                 /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 100,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 101,                   /* OP_DIV  */
  YYSYMBOL_OP_REP_X = 102,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 103,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 104,                   /* OP_POW  */
  YYSYMBOL_105_ = 105,                     /* '='  */
  YYSYMBOL_106_ = 106,                     /* '!'  */
  YYSYMBOL_107_ = 107,                     /* '<'  */
  YYSYMBOL_108_ = 108,                     /* '>'  */
  YYSYMBOL_109_ = 109,                     /* '|'  */
  YYSYMBOL_110_ = 110,                     /* '&'  */
  YYSYMBOL_111_ = 111,                     /* '~'  */
  YYSYMBOL_112_ = 112,                     /* '+'  */
  YYSYMBOL_113_ = 113,                     /* '-'  */
  YYSYMBOL_114_ = 114,                     /* '*'  */
  YYSYMBOL_115_ = 115,                     /* '/'  */
  YYSYMBOL_116_ = 116,                     /* '%'  */
  YYSYMBOL_UMINUS = 117,                   /* UMINUS  */
  YYSYMBOL_118_ = 118,                     /* '.'  */
  YYSYMBOL_119_ = 119,                     /* ';'  */
  YYSYMBOL_120_ = 120,                     /* '('  */
  YYSYMBOL_121_ = 121,                     /* ')'  */
  YYSYMBOL_122_ = 122,                     /* ','  */
  YYSYMBOL_123_ = 123,                     /* '['  */
  YYSYMBOL_124_ = 124,                     /* ']'  */
  YYSYMBOL_125_ = 125,                     /* '{'  */
  YYSYMBOL_126_ = 126,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 127,                 /* $accept  */
  YYSYMBOL_program = 128,                  /* program  */
  YYSYMBOL_stmt_list = 129,                /* stmt_list  */
  YYSYMBOL_stmt = 130,                     /* stmt  */
  YYSYMBOL_if_stmt = 131,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 132,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 133,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 134,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 135,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 136,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 137,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 138,                /* loop_incr  */
  YYSYMBOL_for_stmt = 139,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 140,               /* given_stmt  */
  YYSYMBOL_when_list = 141,                /* when_list  */
  YYSYMBOL_sub_decl = 142,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 143,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 144,                 /* sub_body  */
  YYSYMBOL_method_body = 145,              /* method_body  */
  YYSYMBOL_class_decl = 146,               /* class_decl  */
  YYSYMBOL_role_decl = 147,                /* role_decl  */
  YYSYMBOL_is_clauses = 148,               /* is_clauses  */
  YYSYMBOL_class_body_list = 149,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 150,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 151,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 152,           /* named_arg_list  */
  YYSYMBOL_pair_list = 153,                /* pair_list  */
  YYSYMBOL_param_list = 154,               /* param_list  */
  YYSYMBOL_block = 155,                    /* block  */
  YYSYMBOL_closure = 156,                  /* closure  */
  YYSYMBOL_expr = 157,                     /* expr  */
  YYSYMBOL_tern_expr = 158,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 159,                 /* cmp_expr  */
  YYSYMBOL_jct_expr = 160,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 161,                 /* dor_expr  */
  YYSYMBOL_range_expr = 162,               /* range_expr  */
  YYSYMBOL_add_expr = 163,                 /* add_expr  */
  YYSYMBOL_repl_expr = 164,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 165,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 166,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 167,               /* unary_expr  */
  YYSYMBOL_pow_expr = 168,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 169,              /* scalar_list  */
  YYSYMBOL_meth_name = 170,                /* meth_name  */
  YYSYMBOL_postfix_expr = 171,             /* postfix_expr  */
  YYSYMBOL_call_expr = 172,                /* call_expr  */
  YYSYMBOL_arg_list = 173,                 /* arg_list  */
  YYSYMBOL_atom = 174                      /* atom  */
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
#define YYLAST   3682

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  414
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1057

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   360


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
       2,     2,     2,   106,     2,     2,     2,   116,   110,     2,
     120,   121,   114,   112,   122,   113,   118,   115,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   119,
     107,   105,   108,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   123,     2,   124,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   125,   109,   126,   111,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     117
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   338,   338,   349,   350,   353,   355,   357,   359,   363,
     365,   367,   369,   373,   376,   380,   382,   384,   386,   388,
     390,   394,   398,   400,   402,   404,   406,   408,   410,   412,
     414,   416,   418,   420,   422,   424,   426,   429,   432,   434,
     437,   440,   442,   444,   446,   448,   453,   457,   461,   466,
     470,   471,   473,   475,   477,   479,   482,   484,   486,   488,
     491,   494,   497,   500,   503,   505,   507,   509,   511,   514,
     516,   518,   520,   523,   526,   530,   533,   535,   537,   539,
     542,   545,   548,   551,   554,   557,   560,   563,   566,   569,
     572,   573,   574,   575,   576,   577,   578,   580,   582,   584,
     586,   587,   588,   589,   590,   591,   592,   594,   596,   598,
     600,   601,   602,   603,   606,   608,   610,   612,   614,   616,
     618,   620,   624,   626,   628,   630,   632,   634,   638,   640,
     644,   646,   648,   650,   654,   656,   660,   662,   664,   668,
     670,   672,   676,   677,   678,   679,   680,   683,   689,   695,
     700,   706,   711,   716,   724,   735,   736,   742,   750,   756,
     762,   770,   776,   782,   791,   798,   807,   816,   825,   832,
     839,   840,   843,   846,   848,   850,   852,   854,   856,   858,
     860,   863,   865,   867,   869,   872,   875,   881,   882,   883,
     886,   889,   891,   893,   895,   897,   899,   901,   903,   906,
     908,   910,   912,   915,   918,   924,   940,   955,   956,   968,
     982,   983,   986,   989,   992,   995,   998,  1001,  1004,  1007,
    1010,  1013,  1016,  1019,  1026,  1033,  1040,  1047,  1051,  1055,
    1059,  1063,  1070,  1077,  1084,  1091,  1101,  1109,  1117,  1126,
    1133,  1140,  1150,  1158,  1168,  1183,  1184,  1189,  1194,  1201,
    1205,  1211,  1213,  1215,  1217,  1221,  1222,  1223,  1224,  1225,
    1226,  1227,  1228,  1231,  1232,  1233,  1235,  1237,  1239,  1241,
    1243,  1246,  1248,  1250,  1252,  1254,  1257,  1260,  1264,  1267,
    1270,  1273,  1275,  1279,  1282,  1285,  1288,  1291,  1294,  1296,
    1298,  1300,  1302,  1304,  1306,  1308,  1310,  1314,  1317,  1318,
    1323,  1326,  1328,  1331,  1332,  1333,  1334,  1335,  1336,  1337,
    1338,  1339,  1340,  1341,  1342,  1343,  1344,  1345,  1351,  1357,
    1363,  1366,  1367,  1368,  1371,  1373,  1376,  1377,  1378,  1381,
    1382,  1385,  1386,  1389,  1390,  1391,  1394,  1395,  1396,  1397,
    1398,  1401,  1402,  1403,  1404,  1407,  1408,  1411,  1412,  1415,
    1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,
    1427,  1429,  1434,  1435,  1440,  1442,  1448,  1454,  1458,  1462,
    1467,  1474,  1479,  1485,  1491,  1498,  1505,  1510,  1515,  1522,
    1527,  1532,  1534,  1536,  1538,  1540,  1542,  1545,  1546,  1549,
    1550,  1551,  1552,  1560,  1561,  1562,  1563,  1564,  1568,  1572,
    1575,  1577,  1579,  1581,  1583,  1585,  1586,  1590,  1594,  1598,
    1599,  1600,  1602,  1605,  1606
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
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_TRY", "KW_CATCH",
  "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS", "KW_NEW", "KW_ROLE",
  "KW_MULTI", "KW_PROTO", "OP_NAME", "KW_HANDLES", "WORDLIST",
  "OP_COLON_D", "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE",
  "KW_REGEX", "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW",
  "OP_EQ", "OP_NE", "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT",
  "OP_SLE", "OP_SGT", "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1",
  "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC",
  "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ",
  "OP_DOR", "OP_DIV", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt",
  "loop_incr", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "scalar_methcall", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "jct_expr", "dor_expr", "range_expr",
  "add_expr", "repl_expr", "addsub_expr", "mul_expr", "unary_expr",
  "pow_expr", "scalar_list", "meth_name", "postfix_expr", "call_expr",
  "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-760)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-414)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -760,    26,  1534,  -760,  -760,  -760,  -760,  -760,  1036,   -86,
      41,   -19,   129,  -760,  -760,   359,  -760,  -760,  -760,   125,
    1668,    47,  2820,  2887,  2954,  3021,  3088,    -3,    80,  3088,
    1735,    73,  3088,   227,   273,  3155,  3222,    80,    53,    91,
      92,   166,   166,  3289,    80,    80,  3088,   287,   345,    65,
    -760,   347,   377,   393,   359,   359,  -760,  1802,   355,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,   583,
    -760,  -760,  -760,   876,   614,  -760,   307,   856,   369,  -760,
      -4,   310,   399,   233,  -760,  -760,   338,   365,   370,   492,
     430,   434,  3088,  3088,  3088,  3088,  3088,  3088,   511,  1870,
    3088,   548,  3088,  3088,    63,  1938,   469,   488,   100,  -760,
      80,  -760,  -760,   501,   191,  -760,  2006,  -760,   150,   -66,
       8,   190,   564,   615,   160,   605,  1802,   578,  1802,   180,
    1802,    80,  1802,    80,   137,    30,   148,  -760,  -760,   514,
    -760,   528,   546,   561,   580,   119,   139,  1802,    80,  1802,
      80,    87,  2074,  -760,  3088,  3088,  -760,  3088,  3088,  -760,
    3088,  3088,  3088,  2141,  3088,  -760,   617,  -760,  -760,  -760,
     607,   618,     2,   660,   649,   668,  -760,  -760,  -760,   -11,
     636,  1058,  3088,  3088,  3088,  3088,  3088,  3088,  3088,  3088,
    -760,  3088,  3088,  3088,  3088,  3088,  3088,  3088,  3088,  -760,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   728,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     859,  1006,   421,  -760,  -760,   670,   674,   683,   701,   716,
     729,   216,  -760,   589,   702,   742,   736,   735,   762,   852,
     768,  -760,   619,  3088,   877,  3088,  -760,  3088,   777,   -11,
     621,  -760,  3088,  3088,  -760,  3356,  -760,  3423,  -760,   236,
     261,   263,   282,   802,   806,  -760,   623,   625,  3088,  3088,
    3088,  3088,  3088,  3088,  3088,  -760,  3088,   635,  3088,  3088,
    -760,  3088,   637,   733,   643,  -760,   888,  3088,  -760,   359,
     359,     4,  -760,  -760,  -760,  -760,  3088,  3088,  -760,   937,
    3088,   957,  3088,   645,   890,   650,  -760,  3088,  3088,   965,
     860,   862,   863,   870,   874,   869,  -760,  -760,  -760,    80,
     -10,  -760,    15,   878,   879,  -760,  -760,  -760,  -760,  2208,
    -760,  1094,   873,   163,   892,  2820,  2887,  3088,   675,   114,
     188,  -760,    45,   881,   882,   883,   884,   885,   886,   895,
     896,   897,   898,   899,   900,   905,   913,   917,   918,   496,
     496,   496,   496,   496,   496,   932,   496,   496,   496,   496,
     496,   496,   496,   496,  -760,  -760,  -760,  -760,  -760,  -760,
     911,   911,   310,   399,   233,   233,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,   921,  1024,   922,  -760,  2276,  -760,  -760,  -760,
    -760,  -760,  -760,  3088,  2344,  -760,   938,   939,   940,  -760,
    2412,  -760,  2480,  -760,   925,   942,   920,  -760,  -760,   933,
    -760,   934,  1802,   126,   979,   -53,  2548,   279,   941,  3088,
    -760,  3490,  -760,  3088,  -760,    21,  -760,  3088,  3088,   954,
    1055,  2208,   947,   948,   966,   967,   976,   977,   978,   506,
    2208,   995,   996,   577,    80,   -13,  3557,  -760,    80,    80,
     168,   -35,    58,  -760,     7,   973,   655,  1231,   997,  1000,
      11,  1012,  1002,  1013,  1010,    80,    80,    80,  1004,  1018,
    1033,  3088,  -760,  -760,  -760,  -760,  -760,  -760,  1125,  1126,
    -760,    -6,   973,   657,    22,   406,   106,  -760,   662,  1128,
    3088,  3088,  3088,   198,   209,   -20,  -760,   193,  3088,  3088,
    -760,  3088,  3088,  -760,  3088,  3088,  3088,  3088,  3088,  3088,
    3088,  3088,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,   359,
    2616,  -760,  2684,  1025,   693,  1026,  -760,   697,  3088,  3088,
    3088,  -760,   700,   136,  -760,   707,   709,  -760,  -760,  -760,
    -760,  -760,   711,  3088,  -760,  3088,  3088,  3088,   718,  -760,
     324,  -760,  1027,  1802,   599,  1028,   973,   724,  1029,  1030,
    3088,  -760,   734,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,   737,  -760,  -760,  -760,   837,  -760,  -760,  1802,    80,
    -760,  -760,  1133,  -760,  1140,  1143,  -760,  1145,  1147,  -760,
     973,   456,  2820,  2887,   793,  -760,   451,  -760,  -760,  3088,
      80,  -760,  -760,  -760,  1039,  1040,  1134,  -760,  -760,  -760,
    -760,  3088,  1041,  -760,  -760,     6,   827,   699,  1106,  -760,
    -760,   973,   973,   749,   753,  1149,  1151,  1152,  -760,  -760,
     247,  1043,  1042,   199,  3088,  3088,  3088,  3088,  3088,  3088,
    -760,  -760,  -760,  -760,   211,   262,   270,   278,   280,   292,
     298,   306,   312,   315,   320,   331,  -760,  -760,   758,  -760,
     765,   773,  -760,  1050,  -760,  -760,  1051,  1053,  1054,  -760,
    3088,  -760,  1160,  -760,  2208,  1056,   600,  -760,  -760,  1057,
    1107,  1108,  -760,   781,  -760,  3088,  -760,  -760,   973,  -760,
    -760,   601,  1062,  1063,   -13,  -760,   783,   893,    80,    80,
      80,  -760,  -760,  -760,  -760,   134,   474,   231,  -760,   333,
    3088,  3088,  3088,  3088,  3088,  3088,  3088,  3088,  -760,    80,
    1060,  -760,  -760,    80,  1075,  1601,  -760,   342,   372,    46,
      83,   688,  1076,  1077,   859,  -760,  -760,   973,   973,  1190,
    1191,  1193,  3088,  2752,  1096,  1097,  -760,   334,   335,   349,
     352,   363,   367,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  1131,   801,  -760,  -760,  -760,  3088,  3088,
    2208,   608,  -760,  -760,  3088,  -760,  -760,  -760,  -760,    80,
      80,  -760,  -760,  -760,  -760,  -760,  1194,  1195,  3088,  3088,
    3088,  -760,  -760,  -760,   379,   383,   396,   397,   402,   405,
     424,   432,  -760,  -760,  -760,  3088,   343,  1196,  1197,  1087,
    -760,    28,  1142,  -760,    33,  -760,  1199,  1201,  1091,  3088,
    -760,  1202,  1203,  1092,  3088,  -760,   187,   208,  1099,  1101,
    -760,  -760,   376,  -760,  -760,  -760,  -760,  -760,   435,  1093,
     803,  3088,  3088,  -760,  -760,  -760,  -760,  -760,  -760,  3088,
    1103,  -760,  -760,   805,  -760,   612,   902,  -760,  -760,  -760,
     438,   452,   461,  -760,  -760,  -760,  -760,  -760,  -760,  -760,
    -760,  1104,  -760,  -760,  -760,  -760,    80,  1102,   810,  1098,
    1349,  1102,   812,  1109,  1110,  -760,  1111,  1112,  1113,  -760,
    1114,  1212,  1224,  1120,  3088,  -760,  1226,  1244,  1150,  3088,
    -760,  -760,  -760,    42,  -760,  -760,  -760,  1144,   471,   473,
    -760,  -760,  1168,  -760,    80,  -760,  -760,  -760,  -760,  1601,
    -760,  -760,  1102,  -760,  2820,  2887,  1416,  -760,   516,  -760,
    1102,  -760,  -760,  -760,  -760,  -760,  -760,  1169,  1170,  -760,
    1173,  1174,  1175,  -760,  1177,  1102,   822,  -760,  -760,  -760,
    -760,  -760,  1176,  -760,   543,   232,  -760,   475,  3088,  3088,
    3088,  3088,  3088,  3088,  3088,  3088,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  1102,    80,  3088,  3088,  3088,
    -760,  -760,  -760,   476,   484,   515,   519,   520,   529,   533,
     537,  -760,  -760,   541,   542,   545,  -760,  -760,  -760,  -760,
    -760,  -760,  -760,  -760,  -760,  -760,  -760
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   389,   390,   391,   393,   394,   395,
     396,   406,   405,   407,   408,     0,   397,   398,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,    91,     0,     3,     4,
      92,    93,   100,   101,   102,   103,    94,    95,   110,     0,
     111,   112,   113,    99,     0,   300,   302,   320,   323,   325,
     328,   330,   332,   335,   340,   344,   346,   360,   386,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   394,   395,   396,   406,
       0,   413,   343,     0,   394,    31,     0,   387,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   328,     0,   414,   299,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   136,     0,   139,     0,     0,   104,     0,     0,   105,
       0,     0,     0,     3,     0,   384,    96,    98,   381,   207,
       0,     0,     0,     0,     0,     0,   342,   341,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    86,    87,     0,     0,     0,     0,     0,
     298,     0,   364,     0,     0,     0,     0,     0,   369,     0,
     368,   362,     0,     0,     0,     0,    26,     0,   409,     0,
       0,    34,     0,     0,     6,     0,     9,     0,    10,     0,
       0,     0,     0,     0,     0,   347,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      38,     0,     0,   117,     0,   129,     0,     0,   152,     0,
       0,     0,     3,   159,    41,    42,     0,     0,   155,     0,
       0,     0,     0,     0,   132,     0,   135,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   382,   383,   385,     0,
       0,   210,     0,     0,     0,   245,    88,    89,   410,     0,
     264,   394,   395,   396,   406,     0,     0,     0,     0,     0,
       0,   263,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   305,
     306,   309,   310,   303,   304,     0,   307,   308,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   321,   322,   324,
     326,   327,   329,   331,   333,   334,   339,   336,   337,   338,
     345,   349,   359,   353,   354,   355,   356,   357,   358,   351,
     352,   350,   380,     0,   377,    47,     0,    81,    82,    83,
      84,    85,    44,     0,     0,   363,   400,   401,   402,    49,
       0,   372,     0,   361,     0,     0,     0,   298,    33,     0,
     388,     0,     0,     0,   391,   405,     0,     0,     0,     0,
      23,     0,    24,     0,    25,     0,   162,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   410,     0,     0,   120,   410,     0,
       0,   326,   327,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   410,     0,   410,     0,     0,
       0,     0,   106,   107,   108,   109,   297,    97,     0,     0,
     210,     0,     0,     0,     0,     0,     0,   411,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     291,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,     0,   265,    51,    53,    55,    58,    56,    57,    52,
      54,    64,    66,    68,    71,    69,    70,    65,    67,     0,
       0,   373,     0,     0,     0,     0,   169,     0,     0,     0,
       0,   371,     0,   405,   367,     0,     0,   400,   401,   402,
      32,     5,     0,     0,    11,     0,     0,     0,     0,    16,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   348,     0,    72,    75,    74,    78,    76,    77,    73,
      36,     0,    79,    80,    39,   114,   119,   118,     0,     0,
     128,   149,     0,   151,     0,     0,   256,     0,     0,   158,
       0,     0,     0,     0,     0,   170,     0,    28,    27,     0,
       0,   153,   403,   404,     0,     0,   130,   133,   134,   137,
     138,     0,     0,   208,   209,     0,     0,     0,     0,   206,
     167,     0,     0,     0,     0,     0,     0,     0,   244,   412,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     274,   281,   288,   289,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   301,   379,     0,   376,
       0,     0,    46,     0,    48,   168,     0,     0,     0,   370,
       0,   365,     0,   366,     0,     0,     0,   252,   251,     0,
       0,     0,    18,     0,    19,     0,    22,   161,     0,    30,
      29,     0,   412,   412,     0,   121,     0,   122,     0,     0,
       0,   257,   258,   157,   259,     0,     0,     0,   173,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,    62,    63,     0,     0,     0,   205,     0,     0,     0,
       0,     0,     0,     0,     0,   166,   164,     0,     0,     0,
       0,     0,     0,     0,   400,   402,   285,     0,     0,     0,
       0,     0,     0,   293,   294,   295,   296,   266,   268,   270,
     273,   271,   272,   267,   269,   378,   375,   374,    45,    59,
      60,    61,   249,     0,     0,    13,    12,    17,     0,     0,
       0,     0,   160,     7,     0,    37,    40,   116,   115,   410,
       0,   126,   150,   147,   148,   260,     0,     0,     0,     0,
       0,   174,   175,   172,     0,     0,     0,     0,     0,     0,
       0,     0,   156,   154,   131,     0,   394,     0,     0,     0,
     142,     0,     3,   240,     0,   237,     0,     0,     0,     0,
     216,     0,     0,     0,     0,   211,     0,     0,     0,     0,
     212,   213,     0,   163,   165,   246,   247,   248,     0,   169,
       0,     0,     0,   275,   277,   280,   278,   279,   276,     0,
     412,   254,   253,     0,    20,     0,   123,   124,   261,   262,
       0,     0,     0,   176,   178,   180,   183,   181,   182,   177,
     179,     0,   143,   144,   145,   146,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   227,     0,     0,     0,   228,
       0,     0,     0,     0,     0,   218,     0,     0,     0,     0,
     217,   214,   215,     0,   243,   284,   283,   168,     0,     0,
     250,    14,   412,     8,     0,   127,   184,   186,   185,     0,
     141,   239,     0,   188,     0,     0,     0,   187,     0,   236,
       0,   225,   226,   220,   223,   224,   219,     0,     0,   229,
       0,     0,     0,   230,     0,     0,     0,   282,   286,   287,
      21,   125,     0,   238,     0,     0,   191,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   189,   235,   233,   234,
     222,   231,   232,   221,   242,     0,     0,     0,     0,     0,
     192,   193,   190,     0,     0,     0,     0,     0,     0,     0,
       0,   241,   140,     0,     0,     0,   194,   196,   198,   201,
     199,   200,   195,   197,   202,   204,   203
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -760,  -760,     1,  -760,  -467,  -608,  -760,  -760,  -760,  -760,
    -760,   299,  -760,  -760,  -760,  -760,  -760,  -270,  -759,  -760,
    -760,  -760,   761,  -760,  -760,   744,   871,  -317,   295,   903,
      -2,  -203,  -760,  1100,  -760,   466,   -16,  1095,  1105,   746,
      -9,  -760,  -760,  -226,  -760,  -760,     3,  -760
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   181,    59,    60,   477,    61,    62,    63,    64,
      65,   859,    66,    67,   490,    68,    69,   303,   863,    70,
      71,   330,   511,    72,   516,   575,   447,   486,   111,   161,
     117,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   276,   412,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   456,   375,   508,   414,   112,   735,   616,   865,
     135,   136,   509,    24,   483,   513,   333,   626,   484,   587,
     127,   129,   131,   133,   134,   483,     3,   139,   141,   484,
     144,   483,   483,   148,   150,   484,   484,   100,   483,   263,
     624,   165,   484,   483,   168,   176,   177,   484,   639,   640,
     656,   657,   483,   264,   658,   179,   484,   119,   120,   121,
     866,   122,   656,   657,   334,   104,   658,   105,   867,   219,
     220,   534,   627,   628,   535,   536,   221,   248,   123,   171,
     249,   537,   124,   142,   538,   539,   103,   143,   540,   541,
     235,   236,   237,   238,   239,   240,   172,   871,   244,   304,
     246,   247,   243,   299,   300,   872,   682,   221,   252,   868,
     338,   339,    58,   265,   259,   510,   317,   154,   157,   260,
     659,   250,    58,   954,   277,   485,   287,   266,   292,   831,
     294,   318,   766,   625,   155,   158,   512,   641,   597,   113,
     528,   221,   596,   662,   835,   313,   873,   315,   101,   927,
     320,   869,   321,   322,   931,   323,   324,   529,   325,   326,
     327,   325,   328,   995,   199,   870,   102,   125,   971,   221,
     273,   542,   979,   152,   274,   665,   666,   667,    58,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   874,   361,
     362,   363,   364,   365,   366,   367,   368,   663,   664,   836,
     837,   941,   875,   390,   391,    58,   288,   254,   710,   942,
     156,   159,   296,  1003,   531,   629,   396,   397,   398,   399,
     400,  1017,   946,   289,   674,   255,   309,   279,   675,   583,
     947,   532,   668,   156,   676,   288,  1024,   677,   678,   145,
     530,   679,   660,   622,   310,   584,   311,   104,   585,   105,
     943,   434,   289,   436,   104,   437,   105,   288,   288,   297,
     440,   441,    58,   443,   312,   448,  1041,   827,   301,   261,
     101,   948,   262,   302,   289,   289,   462,   463,   464,   465,
     466,   467,   468,   481,   482,   146,   471,   472,   521,   469,
     262,   160,   944,    58,   473,   267,   257,    73,   965,   290,
     480,   169,   291,   487,   488,   489,   945,   159,   492,   268,
     494,    99,   305,   949,   533,   498,   499,   285,   429,   683,
     286,   423,   137,   138,   680,   786,   727,   950,   290,   720,
     502,   291,   151,   153,   225,   681,   424,   793,   721,   166,
     167,   449,   518,   523,   524,   525,   527,   226,   227,   228,
     290,   290,   782,   291,   291,   450,   696,   842,  1031,   170,
     743,   173,     4,     5,     6,     7,   451,   783,   453,   106,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
     452,   503,   454,   200,   201,   202,   203,   174,   794,   504,
     110,   775,   776,   204,   205,   206,   795,   505,   589,   551,
      33,   590,   455,   175,   796,   137,   797,   302,    41,    42,
      43,   552,   222,    46,   207,   208,   483,   553,   798,   564,
     484,   565,   180,    50,   799,   554,   293,   567,   295,   298,
     768,   555,   800,   572,   556,   576,   922,   923,   801,   557,
     582,   802,   229,   314,   179,   316,   803,   592,   257,   594,
     558,   595,   305,   603,   605,   598,   599,   804,   822,   843,
     893,   894,   861,    99,   602,    54,   744,   862,   606,   218,
     745,   607,    55,   611,   619,   895,    73,   750,   896,    57,
     751,   752,   608,   230,    58,   636,   609,   753,   231,   897,
     754,   755,   864,   898,   756,   757,   953,   862,   551,   652,
     838,   862,   552,   279,   839,   913,   232,   883,   884,   914,
     281,   223,   224,   282,   283,   553,   554,   840,   671,   672,
     673,   555,   915,   916,   556,   241,   684,   685,   917,   686,
     687,   918,   688,   689,   690,   691,   692,   693,   694,   695,
     415,   416,  1008,   557,   928,  1009,  1010,   932,   882,   233,
     919,   558,  1011,   234,   704,  1012,  1013,   603,   920,  1014,
    1015,   955,   245,   698,   966,   701,   706,   707,   708,  1027,
     199,   605,   279,  1028,   269,   270,   271,   758,   967,   281,
     609,   715,   282,   283,   717,   718,  1029,   968,   716,    99,
     809,   723,   811,   285,   305,   551,   286,   998,   731,   999,
     841,  1032,  1046,   552,   278,   216,   217,   279,   280,   182,
    1047,   253,   183,   184,   281,   275,   736,   282,   283,   185,
     256,   284,   186,   187,   507,   610,   188,   189,   262,   272,
     746,   747,   749,   304,   553,   199,   996,   759,   554,   555,
     191,  1048,  1016,   192,   193,  1049,  1050,   305,   556,   764,
     194,   306,   557,   195,   196,  1051,   558,   197,   198,  1052,
     603,   605,   285,  1053,   609,   286,   307,  1054,  1055,  1030,
     329,  1056,   787,   788,   789,   790,   791,   792,     4,     5,
       6,     7,   387,   388,   389,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,   614,   285,   876,   262,
     286,   877,   190,   878,   879,   308,   110,    28,   812,   769,
     425,   262,   770,   771,   772,   773,    33,   814,   724,   816,
     823,   725,   262,   824,    41,    42,    43,   904,   821,    46,
     262,   963,   331,   199,   262,   384,   385,   386,   332,    50,
     433,   262,   439,   262,   459,   460,   338,   461,   844,   845,
     846,   847,   848,   849,   850,   851,   338,   470,   474,   339,
     475,   476,   340,   860,   478,   339,   495,   339,   336,   615,
     617,   497,   339,   620,   621,   623,   630,   631,   661,   631,
     888,    54,    73,   669,   262,   335,   890,   337,    55,   417,
     646,   647,   648,   418,   140,    57,     4,     5,     6,     7,
      58,   526,   419,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,   703,   262,   901,   902,   705,   262,
     420,   709,   262,   903,   110,    28,   426,   905,   711,   712,
     713,   262,   338,   714,    33,   421,   910,   911,   912,   719,
     590,   401,    41,    42,    43,   728,   631,    46,   422,   402,
     427,   403,   404,   921,   429,   732,   262,    50,   733,   262,
     405,   406,   428,   930,   734,   476,   431,   936,   407,   408,
     777,   631,   940,   401,   778,   631,   409,   410,   411,   805,
     262,   402,   430,   403,   404,   767,   806,   712,   432,   958,
     959,   435,   405,   406,   807,   262,   438,   960,   479,    54,
     407,   408,   338,   820,   829,   339,    55,   457,   409,   410,
     411,   458,   140,    57,   737,  -413,  -413,   496,    58,   748,
     830,   476,   900,   262,   957,   262,   962,   262,   978,   964,
     476,   972,   631,   980,   631,   760,   209,   210,   211,   212,
     213,   214,   990,  1025,   631,   162,   164,   994,   215,  -413,
    -413,   491,  -413,  -413,  -413,  -413,  -413,  -413,  -413,  -413,
    -413,  -413,  -413,  -413,  -413,   216,   217,   860,  -413,   394,
     395,   493,  1004,  1005,  1007,   500,  -413,  -413,  -413,   501,
    -413,   502,   503,  -413,  -413,  -413,  -413,  -413,  -413,   504,
    -413,  -413,  -413,   505,  -413,   506,   520,   522,   514,   515,
     543,   544,   545,   546,   547,   548,  1033,  1034,  1035,  1036,
    1037,  1038,  1039,  1040,   549,   550,   551,   552,   553,   554,
     401,   559,   221,   413,   555,  1043,  1044,  1045,   402,   828,
     403,   404,   556,   832,   833,   834,   557,   558,   561,   405,
     406,   560,   562,   568,   569,   570,   579,   407,   408,   577,
     578,   586,   580,   581,   852,   409,   410,   411,   854,   600,
     591,     4,     5,     6,     7,   601,   603,   604,   341,   342,
     343,   344,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   345,   346,    24,   605,   606,    25,    26,    27,
      28,   347,   348,    31,    32,   607,   608,   609,   302,    33,
      34,    35,    36,    37,    38,   349,   350,    41,    42,    43,
      44,    45,    46,    47,   612,   613,   637,    48,    49,   638,
     642,   644,    50,   649,   906,   907,    51,    89,   643,    90,
      91,    92,    93,    94,    95,    96,   645,   650,   651,   653,
     654,    97,   670,   738,   702,   704,   722,   726,   729,   730,
     739,    52,    53,   740,    98,   741,    99,   742,   761,   762,
     765,   763,   774,   779,    54,   780,   781,   784,   785,   808,
     809,    55,   810,   811,   813,   815,   817,    56,    57,   818,
     819,   825,   826,    58,   351,    89,   853,    90,    91,    92,
      93,    94,    95,    96,   855,   880,   881,   885,   886,    97,
     887,   891,   892,   899,   908,   909,   924,   925,   926,   929,
     935,   939,   519,   933,    99,   934,   937,   938,   951,   956,
     952,   970,   961,   969,   973,    73,   987,   862,   981,   982,
     983,   984,   985,   986,     4,     5,     6,     7,   988,   989,
     991,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   632,   633,    24,   992,  1001,
      25,    26,    27,    28,    29,   634,    31,    32,  1002,   993,
     997,   655,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,  1000,  1018,  1019,
      48,    49,  1020,  1021,  1022,    50,  1023,  1026,     0,    51,
     369,   370,   371,   372,   373,   374,   700,   376,   377,   378,
     379,   380,   381,   382,   383,     0,   392,   588,     0,     0,
       0,  1042,     0,     0,    52,    53,     0,   393,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
      56,    57,     4,     5,     6,     7,    58,   635,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   974,   975,    24,     0,     0,    25,    26,
      27,    28,    29,   976,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,     0,    50,     0,     0,     0,    51,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,    56,    57,
      46,     0,     0,     0,    58,   977,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,   140,    57,     4,     5,     6,
       7,    58,  1006,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,     0,     0,    25,    26,    27,    28,    29,    30,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     0,     4,     5,     6,     7,     0,     0,
       0,   856,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,    56,    57,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,   857,   858,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
       0,    57,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,   115,   116,    46,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,    54,     0,    33,     0,     0,     0,     0,    55,     0,
       0,    41,    42,    43,   140,    57,    46,     0,     0,     0,
      58,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   178,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   242,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   251,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,   319,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   258,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,     0,    57,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,   180,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
       0,    57,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   517,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   563,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   566,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,   573,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   571,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,   444,     7,     0,     0,     0,   114,   107,
     108,   109,   445,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   574,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   178,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,   573,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   697,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   699,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   889,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,     0,
     126,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,     0,   128,    46,     0,
       0,     0,    58,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,     0,   130,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
       0,   132,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,     0,    57,    46,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,    54,     0,    33,     0,     0,     0,     0,    55,     0,
       0,    41,    42,    43,     0,   147,    46,     0,     0,     0,
      58,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,    54,     0,
      33,     0,     0,     0,     0,    55,     0,     0,    41,    42,
      43,     0,   149,    46,     0,     0,     0,    58,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,     0,    57,
      46,     0,     0,     0,   163,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     4,     5,   444,     7,
       0,     0,     0,   114,   107,   108,   109,   445,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,     0,   442,    46,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,    54,
       0,    33,     0,     0,     0,     0,    55,     0,     0,    41,
      42,    43,     0,   446,    46,     0,     0,     0,    58,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,     0,
     593,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
      55,     0,     0,     0,     0,     0,     0,   618,     0,     0,
       0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   272,   206,    14,   231,    15,   615,   475,   768,
      26,    14,    22,    26,    10,   332,    14,    10,    14,    72,
      22,    23,    24,    25,    26,    10,     0,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,   123,    10,   105,
      75,    43,    14,    10,    46,    54,    55,    14,    37,    38,
      56,    57,    10,   119,    60,    57,    14,    10,    11,    12,
      14,    14,    56,    57,    62,   118,    60,   120,    22,    73,
      74,    26,    65,    66,    29,    30,   111,    14,    31,    14,
      17,    36,    35,    10,    39,    40,   105,    14,    43,    44,
      92,    93,    94,    95,    96,    97,    31,    14,   100,   119,
     102,   103,    99,    73,    74,    22,   126,   111,   105,    63,
     121,   122,   125,   105,   116,   125,    29,    26,    26,   116,
     126,    58,   125,   882,   126,   121,   128,   119,   130,   737,
     132,    44,   126,    75,    43,    43,   121,   126,   455,    14,
      26,   111,   121,   121,    10,   147,    63,   149,   107,   121,
     152,   105,   154,   155,   121,   157,   158,    43,   160,   161,
     162,   163,   164,   121,   119,   119,   125,   120,   927,   111,
      10,   126,   931,   120,    14,    69,    70,    71,   125,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   105,   191,
     192,   193,   194,   195,   196,   197,   198,   514,   515,    65,
      66,    14,   119,   219,   220,   125,    26,   107,    72,    22,
     119,   119,    75,   972,    26,   485,   225,   226,   227,   228,
     229,   980,    14,    43,    26,   125,   107,    29,    30,   103,
      22,    43,   126,   119,    36,    26,   995,    39,    40,    12,
     126,    43,   512,    75,   125,   119,   107,   118,   122,   120,
      63,   253,    43,   255,   118,   257,   120,    26,    26,   122,
     262,   263,   125,   265,   125,   267,  1025,   734,   120,   119,
     107,    63,   122,   125,    43,    43,   278,   279,   280,   281,
     282,   283,   284,   299,   300,    12,   288,   289,   125,   286,
     122,   125,   105,   125,   291,   105,   105,     2,   906,   119,
     297,    14,   122,   302,   306,   307,   119,   119,   310,   119,
     312,   120,   119,   105,   126,   317,   318,   119,   119,   126,
     122,   105,    27,    28,   126,   126,   596,   119,   119,     5,
     119,   122,    37,    38,   101,   126,   120,   126,    14,    44,
      45,   105,   339,   345,   346,   347,   348,   114,   115,   116,
     119,   119,   105,   122,   122,   119,   559,   126,   126,    14,
     630,    14,     3,     4,     5,     6,   105,   120,   105,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     119,   119,   119,    76,    77,    78,    79,    10,   126,   119,
      31,   661,   662,    86,    87,    88,   126,   119,   119,   119,
      41,   122,   120,    10,   126,   110,   126,   125,    49,    50,
      51,   119,   102,    54,   107,   108,    10,   119,   126,   416,
      14,   423,    67,    64,   126,   119,   131,   424,   133,   134,
     656,   119,   126,   430,   119,   432,    93,    94,   126,   119,
     442,   126,   104,   148,   446,   150,   126,   449,   105,   451,
     119,   453,   119,   119,   119,   457,   458,   126,   728,   126,
     126,   126,   120,   120,   461,   106,    10,   125,   119,   100,
      14,   119,   113,   470,   476,   126,   181,    26,   126,   120,
      29,    30,   119,   118,   125,   487,   119,    36,   118,   126,
      39,    40,   120,   126,    43,    44,   120,   125,   119,   501,
      26,   125,   119,    29,    30,   126,    14,   777,   778,   126,
      36,   112,   113,    39,    40,   119,   119,    43,   520,   521,
     522,   119,   126,   126,   119,    14,   528,   529,   126,   531,
     532,   126,   534,   535,   536,   537,   538,   539,   540,   541,
     119,   120,    26,   119,   861,    29,    30,   864,   774,   119,
     126,   119,    36,   119,   119,    39,    40,   119,   126,    43,
      44,   126,    14,   560,   126,   562,   568,   569,   570,    26,
     119,   119,    29,    30,    10,    11,    12,   126,   126,    36,
     119,   583,    39,    40,   586,   587,    43,   126,   585,   120,
     119,   593,   119,   119,   119,   119,   122,   126,   600,   126,
     126,   126,   126,   119,    26,   109,   110,    29,    30,    26,
     126,   123,    29,    30,    36,    10,   618,    39,    40,    36,
     119,    43,    39,    40,   329,   119,    43,    44,   122,    14,
     632,   633,   634,   119,   119,   119,   953,   639,   119,   119,
      26,   126,   126,    29,    30,   126,   126,   119,   119,   651,
      36,   105,   119,    39,    40,   126,   119,    43,    44,   126,
     119,   119,   119,   126,   119,   122,   105,   126,   126,   126,
      53,   126,   674,   675,   676,   677,   678,   679,     3,     4,
       5,     6,   216,   217,   218,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   119,   119,    10,   122,
     122,    13,   119,    15,    16,   125,    31,    32,   710,    10,
     121,   122,    13,    14,    15,    16,    41,   714,   119,   119,
     119,   122,   122,   122,    49,    50,    51,   119,   725,    54,
     122,   119,   125,   119,   122,     7,     8,     9,   120,    64,
     121,   122,   121,   122,   121,   122,   121,   122,   750,   751,
     752,   753,   754,   755,   756,   757,   121,   122,   121,   122,
      27,    28,   126,   765,   121,   122,   121,   122,   119,   474,
     475,   121,   122,   478,   479,   480,   121,   122,   121,   122,
     782,   106,   487,   121,   122,   125,   783,   119,   113,   119,
     495,   496,   497,   119,   119,   120,     3,     4,     5,     6,
     125,   126,   119,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,   121,   122,   818,   819,   121,   122,
     119,   121,   122,   820,    31,    32,   124,   824,   121,   122,
     121,   122,   121,   122,    41,   119,   838,   839,   840,   121,
     122,    14,    49,    50,    51,   121,   122,    54,   119,    22,
     108,    24,    25,   855,   119,   121,   122,    64,   121,   122,
      33,    34,   126,   862,    27,    28,    14,   869,    41,    42,
     121,   122,   874,    14,   121,   122,    49,    50,    51,   121,
     122,    22,   120,    24,    25,    58,   121,   122,   120,   891,
     892,    14,    33,    34,   121,   122,   119,   899,    10,   106,
      41,    42,   121,   122,   121,   122,   113,   105,    49,    50,
      51,   105,   119,   120,   619,    39,    40,    27,   125,   126,
      27,    28,   121,   122,   121,   122,   121,   122,   930,    27,
      28,   121,   122,   121,   122,   640,    80,    81,    82,    83,
      84,    85,   944,   121,   122,    42,    43,   949,    92,    73,
      74,    14,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,   109,   110,   969,    92,   223,
     224,    14,   974,   975,   976,    10,   100,   101,   102,   119,
     104,   119,   119,   107,   108,   109,   110,   111,   112,   119,
     114,   115,   116,   119,   118,   126,   123,   105,   120,   120,
     119,   119,   119,   119,   119,   119,  1008,  1009,  1010,  1011,
    1012,  1013,  1014,  1015,   119,   119,   119,   119,   119,   119,
      14,    89,   111,    17,   119,  1027,  1028,  1029,    22,   734,
      24,    25,   119,   738,   739,   740,   119,   119,    14,    33,
      34,   120,   120,   105,   105,   105,   126,    41,    42,   124,
     108,    72,   119,   119,   759,    49,    50,    51,   763,   105,
     119,     3,     4,     5,     6,    10,   119,   119,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   119,   119,    29,    30,    31,
      32,    33,    34,    35,    36,   119,   119,   119,   125,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,   119,   119,   119,    59,    60,   119,
     108,   108,    64,   119,   829,   830,    68,    91,   126,    93,
      94,    95,    96,    97,    98,    99,   126,   119,   105,    14,
      14,   105,    14,    10,   119,   119,   119,   119,   119,   119,
      10,    93,    94,    10,   118,    10,   120,    10,   119,   119,
     119,    27,    56,    14,   106,    14,    14,   124,   126,   119,
     119,   113,   119,   119,    14,   119,   119,   119,   120,    72,
      72,   119,   119,   125,   126,    91,   126,    93,    94,    95,
      96,    97,    98,    99,   119,   119,   119,     7,     7,   105,
       7,   105,   105,    72,    10,    10,    10,    10,   121,    67,
     119,   119,   118,    14,   120,    14,    14,    14,   119,   126,
     119,   926,   119,   119,   126,   930,    14,   125,   119,   119,
     119,   119,   119,   119,     3,     4,     5,     6,    14,   119,
      14,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    14,   964,
      29,    30,    31,    32,    33,    34,    35,    36,   969,   119,
     126,   510,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,   119,   119,   119,
      59,    60,   119,   119,   119,    64,   119,   121,    -1,    68,
     200,   201,   202,   203,   204,   205,   562,   207,   208,   209,
     210,   211,   212,   213,   214,    -1,   221,   446,    -1,    -1,
      -1,  1026,    -1,    -1,    93,    94,    -1,   222,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,
     119,   120,     3,     4,     5,     6,   125,   126,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    49,    50,    51,   119,   120,
      54,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,   119,   120,     3,     4,     5,
       6,   125,   126,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    68,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,   119,   120,    54,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,
      -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,   119,   120,    54,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    49,    50,    51,   119,   120,    54,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,
      -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,
      -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,
      -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,    54,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,
      51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,
      54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,
      50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,    -1,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   128,   129,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      64,    68,    93,    94,   106,   113,   119,   120,   125,   130,
     131,   133,   134,   135,   136,   137,   139,   140,   142,   143,
     146,   147,   150,   155,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   171,   172,   174,    91,
      93,    94,    95,    96,    97,    98,    99,   105,   118,   120,
     123,   107,   125,   105,   118,   120,    10,    11,    12,    13,
      31,   155,   167,    14,    10,   119,   120,   157,   173,    10,
      11,    12,    14,    31,    35,   120,   120,   157,   120,   157,
     120,   157,   120,   157,   157,   163,    14,   155,   155,   157,
     119,   157,    10,    14,   157,    12,    12,   120,   157,   120,
     157,   155,   120,   155,    26,    43,   119,    26,    43,   119,
     125,   156,   156,   125,   156,   157,   155,   155,   157,    14,
      14,    14,    31,    14,    10,    10,   167,   167,   121,   157,
      67,   129,    26,    29,    30,    36,    39,    40,    43,    44,
     119,    26,    29,    30,    36,    39,    40,    43,    44,   119,
      76,    77,    78,    79,    86,    87,    88,   107,   108,    80,
      81,    82,    83,    84,    85,    92,   109,   110,   100,    73,
      74,   111,   102,   112,   113,   101,   114,   115,   116,   104,
     118,   118,    14,   119,   119,   157,   157,   157,   157,   157,
     157,    14,   121,   173,   157,    14,   157,   157,    14,    17,
      58,   121,   173,   123,   107,   125,   119,   105,   121,   157,
     173,   119,   122,   105,   119,   105,   119,   105,   119,    10,
      11,    12,    14,    10,    14,    10,   169,   157,    26,    29,
      30,    36,    39,    40,    43,   119,   122,   157,    26,    43,
     119,   122,   157,   155,   157,   155,    75,   122,   155,    73,
      74,   120,   125,   144,   119,   119,   105,   105,   125,   107,
     125,   107,   125,   157,   155,   157,   155,    29,    44,    23,
     157,   157,   157,   157,   157,   157,   157,   157,   157,    53,
     148,   125,   120,    14,    62,   125,   119,   119,   121,   122,
     126,    10,    11,    12,    13,    24,    25,    33,    34,    47,
      48,   126,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   160,
     160,   160,   160,   160,   160,   158,   160,   160,   160,   160,
     160,   160,   160,   160,     7,     8,     9,   162,   162,   162,
     163,   163,   164,   165,   166,   166,   167,   167,   167,   167,
     167,    14,    22,    24,    25,    33,    34,    41,    42,    49,
      50,    51,   170,    17,   170,   119,   120,   119,   119,   119,
     119,   119,   119,   105,   120,   121,   124,   108,   126,   119,
     120,    14,   120,   121,   157,    14,   157,   157,   119,   121,
     157,   157,   120,   157,     5,    14,   120,   153,   157,   105,
     119,   105,   119,   105,   119,   120,   144,   105,   105,   121,
     122,   122,   157,   157,   157,   157,   157,   157,   157,   173,
     122,   157,   157,   173,   121,    27,    28,   132,   121,    10,
     173,   163,   163,    10,    14,   121,   154,   129,   157,   157,
     141,    14,   157,    14,   157,   121,    27,   121,   157,   157,
      10,   119,   119,   119,   119,   119,   126,   155,    14,    22,
     125,   149,   121,   154,   120,   120,   151,   121,   173,   118,
     123,   125,   105,   157,   157,   157,   126,   157,    26,    43,
     126,    26,    43,   126,    26,    29,    30,    36,    39,    40,
      43,    44,   126,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,    89,
     120,    14,   120,   121,   173,   157,   121,   173,   105,   105,
     105,   121,   173,    14,   121,   152,   173,   124,   108,   126,
     119,   119,   157,   103,   119,   122,    72,    72,   153,   119,
     122,   119,   157,   120,   157,   157,   121,   154,   157,   157,
     105,    10,   173,   119,   119,   119,   119,   119,   119,   119,
     119,   173,   119,   119,   119,   155,   131,   155,   120,   157,
     155,   155,    75,   155,    75,    75,    10,    65,    66,   144,
     121,   122,    24,    25,    34,   126,   157,   119,   119,    37,
      38,   126,   108,   126,   108,   126,   155,   155,   155,   119,
     119,   105,   157,    14,    14,   149,    56,    57,    60,   126,
     144,   121,   121,   154,   154,    69,    70,    71,   126,   121,
      14,   157,   157,   157,    26,    30,    36,    39,    40,    43,
     126,   126,   126,   126,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   158,   121,   173,   121,
     152,   173,   119,   121,   119,   121,   157,   157,   157,   121,
      72,   121,   122,   121,   122,   157,   173,   157,   157,   121,
       5,    14,   119,   157,   119,   122,   119,   144,   121,   119,
     119,   157,   121,   121,    27,   132,   157,   155,    10,    10,
      10,    10,    10,   144,    10,    14,   157,   157,   126,   157,
      26,    29,    30,    36,    39,    40,    43,    44,   126,   157,
     155,   119,   119,    27,   157,   119,   126,    58,   170,    10,
      13,    14,    15,    16,    56,   144,   144,   121,   121,    14,
      14,    14,   105,   120,   124,   126,   126,   157,   157,   157,
     157,   157,   157,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   121,   121,   121,   119,   119,
     119,   119,   157,    14,   173,   119,   119,   119,    72,    72,
     122,   173,   144,   119,   122,   119,   119,   131,   155,   121,
      27,   132,   155,   155,   155,    10,    65,    66,    26,    30,
      43,   126,   126,   126,   157,   157,   157,   157,   157,   157,
     157,   157,   155,   126,   155,   119,    10,    93,    94,   138,
     157,   120,   125,   145,   120,   145,    14,    22,    63,   105,
     119,    14,    22,    63,   105,   119,    10,    13,    15,    16,
     119,   119,   170,   144,   144,     7,     7,     7,   157,   121,
     173,   105,   105,   126,   126,   126,   126,   126,   126,    72,
     121,   157,   157,   173,   119,   173,   155,   155,    10,    10,
     157,   157,   157,   126,   126,   126,   126,   126,   126,   126,
     126,   157,    93,    94,    10,    10,   121,   121,   154,    67,
     129,   121,   154,    14,    14,   119,   157,    14,    14,   119,
     157,    14,    22,    63,   105,   119,    14,    22,    63,   105,
     119,   119,   119,   120,   145,   126,   126,   121,   157,   157,
     157,   119,   121,   119,    27,   132,   126,   126,   126,   119,
     155,   145,   121,   126,    24,    25,    34,   126,   157,   145,
     121,   119,   119,   119,   119,   119,   119,    14,    14,   119,
     157,    14,    14,   119,   157,   121,   154,   126,   126,   126,
     119,   155,   138,   145,   157,   157,   126,   157,    26,    29,
      30,    36,    39,    40,    43,    44,   126,   145,   119,   119,
     119,   119,   119,   119,   145,   121,   121,    26,    30,    43,
     126,   126,   126,   157,   157,   157,   157,   157,   157,   157,
     157,   145,   155,   157,   157,   157,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   126
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   127,   128,   129,   129,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   130,   130,   131,   131,   131,   131,   131,   131,
     131,   131,   132,   132,   132,   132,   132,   132,   133,   133,
     134,   134,   134,   134,   135,   135,   136,   136,   136,   137,
     137,   137,   138,   138,   138,   138,   138,   139,   139,   139,
     139,   139,   139,   140,   140,   141,   141,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   143,   143,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   146,   147,   148,   148,   148,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   150,   151,   151,   151,   151,   152,
     152,   153,   153,   153,   153,   154,   154,   154,   154,   154,
     154,   154,   154,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   156,   157,   157,
     157,   158,   158,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   160,   160,   160,   161,   161,   162,   162,   162,   163,
     163,   164,   164,   165,   165,   165,   166,   166,   166,   166,
     166,   167,   167,   167,   167,   168,   168,   169,   169,   170,
     170,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     171,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   173,   173,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     5,     5,     7,     6,     6,
       8,    10,     6,     4,     4,     4,     3,     5,     5,     6,
       6,     2,     5,     4,     3,     3,     5,     7,     3,     5,
       7,     3,     3,     2,     4,     7,     6,     4,     6,     4,
       2,     4,     4,     4,     4,     4,     4,     4,     4,     7,
       7,     7,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     4,     4,     4,     4,     4,     3,     3,     3,     3,
       2,     1,     1,     1,     1,     1,     2,     4,     2,     1,
       1,     1,     1,     1,     2,     2,     4,     4,     4,     4,
       1,     1,     1,     1,     5,     7,     7,     3,     5,     5,
       4,     6,     3,     5,     5,     7,     4,     6,     5,     3,
       5,     7,     3,     5,     5,     3,     2,     5,     5,     2,
      12,     9,     1,     2,     2,     2,     2,     7,     7,     5,
       7,     5,     3,     5,     7,     0,     4,     6,     5,     3,
       7,     6,     4,     7,     6,     7,     6,     5,     6,     5,
       3,     4,     5,     4,     5,     5,     6,     6,     6,     6,
       6,     6,     6,     6,     7,     7,     7,     3,     3,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     6,     5,     0,     3,     3,
       0,     4,     4,     4,     5,     5,     4,     5,     5,     6,
       6,     7,     7,     6,     6,     6,     6,     5,     5,     6,
       6,     7,     7,     7,     7,     7,     6,     4,     7,     6,
       4,     8,     7,     5,     5,     0,     4,     4,     4,     3,
       5,     3,     3,     5,     5,     1,     2,     3,     3,     3,
       4,     5,     5,     3,     3,     4,     6,     6,     6,     6,
       6,     6,     6,     6,     5,     7,     7,     7,     7,     7,
       7,     5,     9,     8,     8,     6,     9,     9,     5,     5,
       4,     4,     4,     6,     6,     6,     6,     3,     3,     2,
       1,     5,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     2,     2,     2,     1,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 339 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2704 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 349 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2710 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 350 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2716 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 354 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2722 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 356 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2728 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 358 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2734 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 360 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2742 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 364 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2748 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 366 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2754 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 368 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2760 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 370 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2768 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 374 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2775 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 377 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2783 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 381 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2789 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 383 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2795 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 385 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2801 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 387 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2807 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 389 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2813 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 391 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2821 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 395 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2829 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 399 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2835 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 401 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2841 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 403 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2847 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 405 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2853 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 407 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2859 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 409 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2865 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2871 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2877 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 415 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2883 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 417 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2889 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 419 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2895 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 421 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2901 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 423 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2907 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 425 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2913 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 427 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2920 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 430 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2927 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 433 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2933 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 435 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2940 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 438 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2947 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 441 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2953 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 443 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2959 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 445 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2965 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 447 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2971 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 449 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2980 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 454 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2988 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 458 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2996 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 462 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3005 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 467 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3013 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 470 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3019 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 472 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3025 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3031 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 476 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3037 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 478 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3043 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 480 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3050 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 483 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3056 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 485 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3062 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3068 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 489 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3075 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 492 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3082 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 495 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3089 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 498 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3096 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 501 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3103 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3109 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3115 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 508 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3121 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 510 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3127 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 512 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3134 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 515 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3140 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 517 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3146 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 519 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3152 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 521 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3159 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3166 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 527 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3174 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 531 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3181 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 534 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3187 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 536 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3193 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 538 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3199 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 540 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3206 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 543 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3213 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 546 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3220 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 549 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3227 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 552 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3234 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 555 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3241 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 558 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3248 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_INC ';'  */
#line 561 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,var_node((yyvsp[-2].sval)),one)); }
#line 3255 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_DEC ';'  */
#line 564 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,var_node((yyvsp[-2].sval)),one)); }
#line 3262 "raku.tab.c"
    break;

  case 88: /* stmt: OP_INC VAR_SCALAR ';'  */
#line 567 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_ADD,var_node((yyvsp[-1].sval)),one)); }
#line 3269 "raku.tab.c"
    break;

  case 89: /* stmt: OP_DEC VAR_SCALAR ';'  */
#line 570 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1;
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-1].sval)),expr_binary(TT_SUB,var_node((yyvsp[-1].sval)),one)); }
#line 3276 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 572 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3282 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 573 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3288 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 574 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3294 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 575 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3300 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 576 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3306 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 577 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3312 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3318 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3324 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3330 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 585 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3336 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 586 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3342 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 587 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3348 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 588 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3354 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 589 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3360 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 590 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3366 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 591 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3372 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 593 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3378 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 595 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3384 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 597 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3390 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3396 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 600 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3402 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 601 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3408 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 602 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3414 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 603 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3420 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 607 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3426 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 609 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3432 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 611 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3438 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 613 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3450 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 617 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3456 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 619 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3462 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 621 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3468 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3474 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3480 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 629 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3486 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 631 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3492 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 633 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3498 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 635 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3504 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 639 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3510 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 641 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3516 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 645 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3522 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3528 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 649 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3534 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3540 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 655 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3546 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3552 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3558 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3564 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3570 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 669 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3576 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 671 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3582 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 673 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3588 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 676 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3594 "raku.tab.c"
    break;

  case 143: /* loop_incr: VAR_SCALAR OP_INC  */
#line 677 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),1); }
#line 3600 "raku.tab.c"
    break;

  case 144: /* loop_incr: VAR_SCALAR OP_DEC  */
#line 678 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[-1].sval),0); }
#line 3606 "raku.tab.c"
    break;

  case 145: /* loop_incr: OP_INC VAR_SCALAR  */
#line 679 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 3612 "raku.tab.c"
    break;

  case 146: /* loop_incr: OP_DEC VAR_SCALAR  */
#line 680 "raku.y"
                          { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 3618 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 684 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3628 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 690 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3638 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 696 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3647 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 701 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3657 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 707 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3666 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr block  */
#line 712 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3673 "raku.tab.c"
    break;

  case 153: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 717 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3685 "raku.tab.c"
    break;

  case 154: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 725 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3698 "raku.tab.c"
    break;

  case 155: /* when_list: %empty  */
#line 735 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3704 "raku.tab.c"
    break;

  case 156: /* when_list: when_list KW_WHEN expr block  */
#line 737 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3712 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 743 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3724 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 751 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3734 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_SUB IDENT sub_body  */
#line 757 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3744 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 763 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3756 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 771 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3766 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 777 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3776 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 783 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3789 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 792 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3800 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 799 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3813 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 808 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3826 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 817 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3837 "raku.tab.c"
    break;

  case 168: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 826 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3848 "raku.tab.c"
    break;

  case 169: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 833 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3857 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list '}'  */
#line 839 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3863 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr '}'  */
#line 841 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3870 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 844 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3877 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 847 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3883 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 849 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3889 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 851 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3895 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 853 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3901 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 855 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3907 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 857 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3913 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 859 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3919 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 861 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3926 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 864 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3932 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 866 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3938 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 868 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3944 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 870 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3951 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 873 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3958 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 876 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3966 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list '}'  */
#line 881 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3972 "raku.tab.c"
    break;

  case 188: /* method_body: '{' YADA '}'  */
#line 882 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3978 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr '}'  */
#line 884 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3985 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 887 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3992 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 890 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3998 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 892 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4004 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 894 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4010 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 896 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4016 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 898 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4022 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 900 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4028 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 902 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4034 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 904 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4041 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 907 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4047 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 909 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4053 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 911 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4059 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 913 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4066 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 916 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4073 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 919 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4081 "raku.tab.c"
    break;

  case 205: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 925 "raku.y"
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
#line 4099 "raku.tab.c"
    break;

  case 206: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 941 "raku.y"
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
#line 4116 "raku.tab.c"
    break;

  case 207: /* is_clauses: %empty  */
#line 955 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4122 "raku.tab.c"
    break;

  case 208: /* is_clauses: is_clauses IDENT IDENT  */
#line 957 "raku.y"
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
#line 4138 "raku.tab.c"
    break;

  case 209: /* is_clauses: is_clauses TESTOP IDENT  */
#line 969 "raku.y"
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
#line 4154 "raku.tab.c"
    break;

  case 210: /* class_body_list: %empty  */
#line 982 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4160 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 984 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4167 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 987 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4174 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 990 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4181 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 993 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4188 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 996 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4195 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 999 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4202 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1002 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4209 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1005 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4216 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1008 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4223 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1011 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4230 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1014 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4237 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1017 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4244 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1020 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4255 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1027 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4266 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1034 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4277 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1041 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4288 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1048 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4296 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1052 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4304 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1056 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4312 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1060 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4320 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1064 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4331 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1071 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4342 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1078 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4353 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1085 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4364 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1092 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4378 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1102 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4390 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1110 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4402 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1118 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4415 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1127 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4426 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1134 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4437 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1141 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4451 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1151 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4463 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1159 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4475 "raku.tab.c"
    break;

  case 244: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1169 "raku.y"
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
#line 4492 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: %empty  */
#line 1183 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4498 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1185 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4507 "raku.tab.c"
    break;

  case 247: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1190 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4516 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1195 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4525 "raku.tab.c"
    break;

  case 249: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1202 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4533 "raku.tab.c"
    break;

  case 250: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1206 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4541 "raku.tab.c"
    break;

  case 251: /* pair_list: IDENT OP_FATARROW expr  */
#line 1212 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4547 "raku.tab.c"
    break;

  case 252: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1214 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4553 "raku.tab.c"
    break;

  case 253: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1216 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4559 "raku.tab.c"
    break;

  case 254: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1218 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4565 "raku.tab.c"
    break;

  case 255: /* param_list: VAR_SCALAR  */
#line 1221 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4571 "raku.tab.c"
    break;

  case 256: /* param_list: IDENT VAR_SCALAR  */
#line 1222 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4577 "raku.tab.c"
    break;

  case 257: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1223 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4583 "raku.tab.c"
    break;

  case 258: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1224 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4589 "raku.tab.c"
    break;

  case 259: /* param_list: param_list ',' VAR_SCALAR  */
#line 1225 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4595 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1226 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4601 "raku.tab.c"
    break;

  case 261: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1227 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4607 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1228 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4613 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list '}'  */
#line 1231 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4619 "raku.tab.c"
    break;

  case 264: /* block: '{' YADA '}'  */
#line 1232 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4625 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr '}'  */
#line 1234 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4631 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1236 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4637 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1238 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4643 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1240 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4649 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1242 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4655 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1244 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4662 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1247 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4668 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1249 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4674 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1251 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4680 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1253 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4686 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1255 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4693 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1258 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4700 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1261 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4708 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1265 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4715 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1268 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4722 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1271 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4729 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1274 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4735 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1276 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4743 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1280 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4750 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1283 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4757 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1286 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4764 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1289 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4771 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1292 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4778 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1295 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4784 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1297 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4790 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1299 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4796 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_LAST '}'  */
#line 1301 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4802 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1303 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4808 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1305 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4814 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1307 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4820 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1309 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4826 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1311 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4832 "raku.tab.c"
    break;

  case 297: /* closure: '{' expr '}'  */
#line 1314 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4838 "raku.tab.c"
    break;

  case 298: /* expr: VAR_SCALAR '=' expr  */
#line 1317 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4844 "raku.tab.c"
    break;

  case 299: /* expr: KW_GATHER block  */
#line 1318 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4854 "raku.tab.c"
    break;

  case 300: /* expr: tern_expr  */
#line 1323 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4860 "raku.tab.c"
    break;

  case 301: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1327 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4866 "raku.tab.c"
    break;

  case 302: /* tern_expr: cmp_expr  */
#line 1328 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4872 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1331 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4878 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1332 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4884 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1333 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4890 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1334 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4896 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1335 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4902 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1336 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4908 "raku.tab.c"
    break;

  case 309: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1337 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4914 "raku.tab.c"
    break;

  case 310: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1338 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4920 "raku.tab.c"
    break;

  case 311: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1339 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4926 "raku.tab.c"
    break;

  case 312: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1340 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4932 "raku.tab.c"
    break;

  case 313: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1341 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4938 "raku.tab.c"
    break;

  case 314: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1342 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4944 "raku.tab.c"
    break;

  case 315: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1343 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4950 "raku.tab.c"
    break;

  case 316: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1344 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4956 "raku.tab.c"
    break;

  case 317: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1346 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4966 "raku.tab.c"
    break;

  case 318: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1352 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4976 "raku.tab.c"
    break;

  case 319: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1358 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4986 "raku.tab.c"
    break;

  case 320: /* cmp_expr: jct_expr  */
#line 1363 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4992 "raku.tab.c"
    break;

  case 321: /* jct_expr: jct_expr '|' range_expr  */
#line 1366 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4998 "raku.tab.c"
    break;

  case 322: /* jct_expr: jct_expr '&' range_expr  */
#line 1367 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5004 "raku.tab.c"
    break;

  case 323: /* jct_expr: dor_expr  */
#line 1368 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5010 "raku.tab.c"
    break;

  case 324: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1372 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5016 "raku.tab.c"
    break;

  case 325: /* dor_expr: range_expr  */
#line 1373 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5022 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1376 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5028 "raku.tab.c"
    break;

  case 327: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1377 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5034 "raku.tab.c"
    break;

  case 328: /* range_expr: add_expr  */
#line 1378 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5040 "raku.tab.c"
    break;

  case 329: /* add_expr: add_expr '~' repl_expr  */
#line 1381 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5046 "raku.tab.c"
    break;

  case 330: /* add_expr: repl_expr  */
#line 1382 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5052 "raku.tab.c"
    break;

  case 331: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1385 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5058 "raku.tab.c"
    break;

  case 332: /* repl_expr: addsub_expr  */
#line 1386 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5064 "raku.tab.c"
    break;

  case 333: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1389 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5070 "raku.tab.c"
    break;

  case 334: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1390 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5076 "raku.tab.c"
    break;

  case 335: /* addsub_expr: mul_expr  */
#line 1391 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5082 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '*' unary_expr  */
#line 1394 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5088 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr '/' unary_expr  */
#line 1395 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5094 "raku.tab.c"
    break;

  case 338: /* mul_expr: mul_expr '%' unary_expr  */
#line 1396 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5100 "raku.tab.c"
    break;

  case 339: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1397 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5106 "raku.tab.c"
    break;

  case 340: /* mul_expr: unary_expr  */
#line 1398 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5112 "raku.tab.c"
    break;

  case 341: /* unary_expr: '-' unary_expr  */
#line 1401 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5118 "raku.tab.c"
    break;

  case 342: /* unary_expr: '!' unary_expr  */
#line 1402 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5124 "raku.tab.c"
    break;

  case 343: /* unary_expr: CARET unary_expr  */
#line 1403 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5130 "raku.tab.c"
    break;

  case 344: /* unary_expr: pow_expr  */
#line 1404 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5136 "raku.tab.c"
    break;

  case 345: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1407 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5142 "raku.tab.c"
    break;

  case 346: /* pow_expr: postfix_expr  */
#line 1408 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5148 "raku.tab.c"
    break;

  case 347: /* scalar_list: VAR_SCALAR  */
#line 1411 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5154 "raku.tab.c"
    break;

  case 348: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1412 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5160 "raku.tab.c"
    break;

  case 349: /* meth_name: IDENT  */
#line 1415 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5166 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_SORT  */
#line 1416 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5172 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_MAP  */
#line 1417 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5178 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_GREP  */
#line 1418 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5184 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_SAY  */
#line 1419 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5190 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_PRINT  */
#line 1420 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5196 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_TAKE  */
#line 1421 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5202 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_RETURN  */
#line 1422 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5208 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_EXISTS  */
#line 1423 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5214 "raku.tab.c"
    break;

  case 358: /* meth_name: KW_DELETE  */
#line 1424 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5220 "raku.tab.c"
    break;

  case 359: /* meth_name: TESTOP  */
#line 1425 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5226 "raku.tab.c"
    break;

  case 360: /* postfix_expr: call_expr  */
#line 1427 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5232 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '(' arg_list ')'  */
#line 1430 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5241 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '(' ')'  */
#line 1434 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5247 "raku.tab.c"
    break;

  case 363: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1436 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5256 "raku.tab.c"
    break;

  case 364: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1441 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5262 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1443 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5272 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1449 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5282 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1455 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5290 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' KW_NEW  */
#line 1459 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5298 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' IDENT  */
#line 1463 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5307 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1468 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5318 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1475 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5327 "raku.tab.c"
    break;

  case 372: /* call_expr: IDENT '.' CARET IDENT  */
#line 1480 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5337 "raku.tab.c"
    break;

  case 373: /* call_expr: atom '.' CARET IDENT  */
#line 1486 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5347 "raku.tab.c"
    break;

  case 374: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1492 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5358 "raku.tab.c"
    break;

  case 375: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5369 "raku.tab.c"
    break;

  case 376: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1506 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5378 "raku.tab.c"
    break;

  case 377: /* call_expr: atom '.' meth_name  */
#line 1511 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5387 "raku.tab.c"
    break;

  case 378: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1516 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5398 "raku.tab.c"
    break;

  case 379: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5407 "raku.tab.c"
    break;

  case 380: /* call_expr: call_expr '.' meth_name  */
#line 1528 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5416 "raku.tab.c"
    break;

  case 381: /* call_expr: KW_DIE expr  */
#line 1533 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5422 "raku.tab.c"
    break;

  case 382: /* call_expr: KW_MAP closure expr  */
#line 1535 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5428 "raku.tab.c"
    break;

  case 383: /* call_expr: KW_GREP closure expr  */
#line 1537 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5434 "raku.tab.c"
    break;

  case 384: /* call_expr: KW_SORT expr  */
#line 1539 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5440 "raku.tab.c"
    break;

  case 385: /* call_expr: KW_SORT closure expr  */
#line 1541 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5446 "raku.tab.c"
    break;

  case 386: /* call_expr: atom  */
#line 1542 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5452 "raku.tab.c"
    break;

  case 387: /* arg_list: expr  */
#line 1545 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5458 "raku.tab.c"
    break;

  case 388: /* arg_list: arg_list ',' expr  */
#line 1546 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5464 "raku.tab.c"
    break;

  case 389: /* atom: LIT_INT  */
#line 1549 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5470 "raku.tab.c"
    break;

  case 390: /* atom: LIT_FLOAT  */
#line 1550 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5476 "raku.tab.c"
    break;

  case 391: /* atom: LIT_STR  */
#line 1551 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5482 "raku.tab.c"
    break;

  case 392: /* atom: WORDLIST  */
#line 1553 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5494 "raku.tab.c"
    break;

  case 393: /* atom: LIT_INTERP_STR  */
#line 1560 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5500 "raku.tab.c"
    break;

  case 394: /* atom: VAR_SCALAR  */
#line 1561 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5506 "raku.tab.c"
    break;

  case 395: /* atom: VAR_ARRAY  */
#line 1562 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5512 "raku.tab.c"
    break;

  case 396: /* atom: VAR_HASH  */
#line 1563 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5518 "raku.tab.c"
    break;

  case 397: /* atom: VAR_CAPTURE  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5526 "raku.tab.c"
    break;

  case 398: /* atom: VAR_FH  */
#line 1569 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5534 "raku.tab.c"
    break;

  case 399: /* atom: VAR_NAMED_CAPTURE  */
#line 1573 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5541 "raku.tab.c"
    break;

  case 400: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1576 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5547 "raku.tab.c"
    break;

  case 401: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1578 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5553 "raku.tab.c"
    break;

  case 402: /* atom: VAR_HASH '{' expr '}'  */
#line 1580 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5559 "raku.tab.c"
    break;

  case 403: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1582 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5565 "raku.tab.c"
    break;

  case 404: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1584 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5571 "raku.tab.c"
    break;

  case 405: /* atom: IDENT  */
#line 1585 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5577 "raku.tab.c"
    break;

  case 406: /* atom: VAR_TWIGIL  */
#line 1587 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5585 "raku.tab.c"
    break;

  case 407: /* atom: VAR_ARRAY_TWIGIL  */
#line 1591 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5593 "raku.tab.c"
    break;

  case 408: /* atom: VAR_HASH_TWIGIL  */
#line 1595 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5601 "raku.tab.c"
    break;

  case 409: /* atom: '(' ')'  */
#line 1598 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5607 "raku.tab.c"
    break;

  case 410: /* atom: '(' expr ')'  */
#line 1599 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5613 "raku.tab.c"
    break;

  case 411: /* atom: '(' expr ',' ')'  */
#line 1601 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5619 "raku.tab.c"
    break;

  case 412: /* atom: '(' expr ',' arg_list ')'  */
#line 1603 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5626 "raku.tab.c"
    break;

  case 413: /* atom: block  */
#line 1605 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5632 "raku.tab.c"
    break;

  case 414: /* atom: KW_SUB block  */
#line 1606 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5638 "raku.tab.c"
    break;


#line 5642 "raku.tab.c"

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

#line 1608 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
