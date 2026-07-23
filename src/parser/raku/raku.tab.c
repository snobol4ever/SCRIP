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
}/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_post_incdec(const char *var, int add) {
    static int __post_uid = 0; char tmp[32]; snprintf(tmp, sizeof tmp, "__post_%d", __post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), var_node(var)));
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    expr_add_child(seq, expr_binary(TT_ASSIGN, var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one)));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
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

#line 337 "raku.tab.c"

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
  YYSYMBOL_OP_DIVIS = 102,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 103,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 104,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 105,                   /* OP_POW  */
  YYSYMBOL_106_ = 106,                     /* '='  */
  YYSYMBOL_107_ = 107,                     /* '!'  */
  YYSYMBOL_108_ = 108,                     /* '<'  */
  YYSYMBOL_109_ = 109,                     /* '>'  */
  YYSYMBOL_110_ = 110,                     /* '|'  */
  YYSYMBOL_111_ = 111,                     /* '&'  */
  YYSYMBOL_112_ = 112,                     /* '~'  */
  YYSYMBOL_113_ = 113,                     /* '+'  */
  YYSYMBOL_114_ = 114,                     /* '-'  */
  YYSYMBOL_115_ = 115,                     /* '*'  */
  YYSYMBOL_116_ = 116,                     /* '/'  */
  YYSYMBOL_117_ = 117,                     /* '%'  */
  YYSYMBOL_UMINUS = 118,                   /* UMINUS  */
  YYSYMBOL_119_ = 119,                     /* '.'  */
  YYSYMBOL_120_ = 120,                     /* ';'  */
  YYSYMBOL_121_ = 121,                     /* '('  */
  YYSYMBOL_122_ = 122,                     /* ')'  */
  YYSYMBOL_123_ = 123,                     /* ','  */
  YYSYMBOL_124_ = 124,                     /* '['  */
  YYSYMBOL_125_ = 125,                     /* ']'  */
  YYSYMBOL_126_ = 126,                     /* '{'  */
  YYSYMBOL_127_ = 127,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 128,                 /* $accept  */
  YYSYMBOL_program = 129,                  /* program  */
  YYSYMBOL_stmt_list = 130,                /* stmt_list  */
  YYSYMBOL_stmt = 131,                     /* stmt  */
  YYSYMBOL_if_stmt = 132,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 133,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 134,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 135,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 136,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 137,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 138,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 139,                /* loop_incr  */
  YYSYMBOL_for_stmt = 140,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 141,               /* given_stmt  */
  YYSYMBOL_when_list = 142,                /* when_list  */
  YYSYMBOL_sub_decl = 143,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 144,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 145,                 /* sub_body  */
  YYSYMBOL_method_body = 146,              /* method_body  */
  YYSYMBOL_class_decl = 147,               /* class_decl  */
  YYSYMBOL_role_decl = 148,                /* role_decl  */
  YYSYMBOL_is_clauses = 149,               /* is_clauses  */
  YYSYMBOL_class_body_list = 150,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 151,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 152,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 153,           /* named_arg_list  */
  YYSYMBOL_pair_list = 154,                /* pair_list  */
  YYSYMBOL_param_list = 155,               /* param_list  */
  YYSYMBOL_block = 156,                    /* block  */
  YYSYMBOL_closure = 157,                  /* closure  */
  YYSYMBOL_expr = 158,                     /* expr  */
  YYSYMBOL_tern_expr = 159,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 160,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 161,               /* divis_expr  */
  YYSYMBOL_jct_expr = 162,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 163,                 /* dor_expr  */
  YYSYMBOL_range_expr = 164,               /* range_expr  */
  YYSYMBOL_add_expr = 165,                 /* add_expr  */
  YYSYMBOL_repl_expr = 166,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 167,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 168,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 169,               /* unary_expr  */
  YYSYMBOL_pow_expr = 170,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 171,              /* scalar_list  */
  YYSYMBOL_meth_name = 172,                /* meth_name  */
  YYSYMBOL_postfix_expr = 173,             /* postfix_expr  */
  YYSYMBOL_call_expr = 174,                /* call_expr  */
  YYSYMBOL_arg_list = 175,                 /* arg_list  */
  YYSYMBOL_atom = 176                      /* atom  */
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
#define YYLAST   3635

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  128
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  412
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1049

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   361


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
       2,     2,     2,   107,     2,     2,     2,   117,   111,     2,
     121,   122,   115,   113,   123,   114,   119,   116,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   120,
     108,   106,   109,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   124,     2,   125,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   126,   110,   127,   112,     2,     2,     2,
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
     105,   118
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   349,   349,   360,   361,   364,   366,   368,   370,   374,
     376,   378,   380,   384,   387,   391,   393,   395,   397,   399,
     401,   405,   409,   411,   413,   415,   417,   419,   421,   423,
     425,   427,   429,   431,   433,   435,   437,   440,   443,   445,
     448,   451,   453,   455,   457,   459,   464,   468,   472,   477,
     481,   482,   484,   486,   488,   490,   493,   495,   497,   499,
     502,   505,   508,   511,   514,   516,   518,   520,   522,   525,
     527,   529,   531,   534,   537,   541,   544,   546,   548,   550,
     553,   556,   559,   562,   565,   568,   571,   572,   573,   574,
     575,   576,   577,   579,   581,   583,   585,   586,   587,   588,
     589,   590,   591,   593,   595,   597,   599,   600,   601,   602,
     605,   607,   609,   611,   613,   615,   617,   619,   623,   625,
     627,   629,   631,   633,   637,   639,   643,   645,   647,   649,
     653,   655,   659,   661,   663,   667,   669,   671,   675,   678,
     684,   690,   695,   701,   706,   711,   719,   730,   731,   737,
     745,   751,   757,   765,   771,   777,   786,   793,   802,   811,
     820,   827,   834,   835,   838,   841,   843,   845,   847,   849,
     851,   853,   855,   858,   860,   862,   864,   867,   870,   876,
     877,   878,   881,   884,   886,   888,   890,   892,   894,   896,
     898,   901,   903,   905,   907,   910,   913,   919,   935,   950,
     951,   963,   977,   978,   981,   984,   987,   990,   993,   996,
     999,  1002,  1005,  1008,  1011,  1014,  1021,  1028,  1035,  1042,
    1046,  1050,  1054,  1058,  1065,  1072,  1079,  1086,  1096,  1104,
    1112,  1121,  1128,  1135,  1145,  1153,  1163,  1178,  1179,  1184,
    1189,  1196,  1200,  1206,  1208,  1210,  1212,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1223,  1226,  1227,  1228,  1230,  1232,
    1234,  1236,  1238,  1241,  1243,  1245,  1247,  1249,  1252,  1255,
    1259,  1262,  1265,  1268,  1270,  1274,  1277,  1280,  1283,  1286,
    1289,  1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,  1309,
    1312,  1313,  1318,  1321,  1323,  1326,  1327,  1328,  1329,  1330,
    1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,
    1346,  1352,  1358,  1361,  1362,  1365,  1366,  1367,  1370,  1372,
    1375,  1376,  1377,  1380,  1381,  1384,  1385,  1388,  1389,  1390,
    1393,  1394,  1395,  1396,  1397,  1400,  1401,  1402,  1403,  1406,
    1407,  1410,  1411,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1426,  1428,  1433,  1434,  1439,  1441,
    1447,  1453,  1457,  1461,  1466,  1473,  1478,  1484,  1490,  1497,
    1504,  1509,  1514,  1521,  1526,  1531,  1533,  1535,  1537,  1539,
    1541,  1544,  1545,  1548,  1549,  1550,  1551,  1559,  1560,  1561,
    1562,  1563,  1564,  1565,  1566,  1567,  1571,  1575,  1578,  1580,
    1582,  1584,  1586,  1588,  1589,  1593,  1597,  1601,  1602,  1603,
    1605,  1608,  1609
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
  "OP_DOR", "OP_DIV", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='",
  "'!'", "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'",
  "'%'", "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'",
  "'}'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "when_list",
  "sub_decl", "scalar_methcall", "sub_body", "method_body", "class_decl",
  "role_decl", "is_clauses", "class_body_list", "grammar_decl",
  "grammar_body_list", "named_arg_list", "pair_list", "param_list",
  "block", "closure", "expr", "tern_expr", "cmp_expr", "divis_expr",
  "jct_expr", "dor_expr", "range_expr", "add_expr", "repl_expr",
  "addsub_expr", "mul_expr", "unary_expr", "pow_expr", "scalar_list",
  "meth_name", "postfix_expr", "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-756)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-412)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -756,    26,  1680,  -756,  -756,  -756,  -756,  -756,   726,   -86,
     -56,   -58,   332,  -756,  -756,   861,  -756,  -756,  -756,    46,
    1748,    93,  2834,  2889,  2958,  3013,  3082,    -6,   -12,  3082,
    1816,   363,  3082,   126,   132,  3137,  3206,   -12,    81,    54,
      57,    24,    24,  3261,   -12,   -12,  3082,   153,   161,    53,
    -756,   198,   224,   237,   861,   861,  -756,  1884,   221,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,   520,
    -756,  -756,  -756,   611,   532,  -756,   534,   522,   188,   234,
    -756,   146,   246,   226,   321,  -756,  -756,   219,   271,   312,
     415,  -756,  -756,  3082,  3082,  3082,  3082,  3082,  3082,   427,
    1953,  3082,   452,  3082,  3082,   120,  2008,   175,   359,   -36,
    -756,   -12,  -756,  -756,   390,   196,  -756,  2077,  -756,   241,
      98,   144,   165,   694,   525,   379,   543,  1884,   473,  1884,
      19,  1884,   -12,  1884,   -12,   -57,   162,   133,  -756,  -756,
     435,  -756,   437,   463,   517,   448,     9,   102,  1884,   -12,
    1884,   -12,    69,  2132,  -756,  3082,  3082,  -756,  3082,  3082,
    -756,  3082,  3082,  3082,  2201,  3082,  -756,   529,  -756,  -756,
    -756,   502,   512,    15,   531,  -756,  -756,  -756,  -756,  -756,
     240,   540,  1072,  3082,  3082,  3082,  3082,  3082,  3082,  3082,
    3082,  -756,  3082,  3082,  3082,  3082,  3082,  3082,  3082,  3082,
    -756,   861,   861,   861,   861,   861,   861,   861,   861,   861,
     861,   861,   861,   861,   861,   861,   793,   861,   861,   861,
     861,   861,   861,   861,   861,   861,   861,   861,   861,   861,
     861,   861,   613,   744,   254,   549,   562,   580,   595,   605,
     609,    92,  -756,   383,   612,   667,   619,   683,   693,   832,
     697,  -756,   476,  3082,   837,  3082,  -756,  3082,   728,   240,
     648,  -756,  3082,  3082,  -756,  3330,  -756,  3385,  -756,   187,
     191,   213,   199,   756,   778,  -756,   686,   704,  3082,  3082,
    3082,  3082,  3082,  3082,  3082,  -756,  3082,   707,  3082,  3082,
    -756,  3082,   719,   617,   721,  -756,   881,  3082,  -756,   861,
     861,    -1,  -756,  -756,  -756,  -756,  3082,  3082,  -756,   886,
    3082,   916,  3082,   727,   892,   731,  -756,  3082,  3082,   930,
     838,   858,   859,   860,   863,   854,  -756,  -756,  -756,   -12,
      -8,  -756,    21,   846,   864,  -756,  -756,  2269,  -756,   810,
     862,   141,   878,  2834,  2889,  3082,  1425,    25,   104,  -756,
     292,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   887,   888,   889,   890,   891,   893,   904,   904,   904,
     904,   904,   904,   923,   904,   904,   904,   904,   904,   904,
     904,   904,  -756,  -756,  -756,   188,  -756,  -756,  -756,   902,
     902,   246,   226,   321,   321,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,   894,  1002,   899,  -756,  2338,  -756,  -756,  -756,  -756,
    -756,  -756,  3082,  2393,  -756,   915,   917,   918,  -756,  2462,
    -756,  2517,  -756,   897,   919,   898,  -756,  -756,   906,  -756,
     907,  1884,   152,   957,   -32,  2586,   351,   910,  3082,  -756,
    3454,  -756,  3082,  -756,    33,  -756,  3082,  3082,   925,  1022,
    2269,   913,   914,   920,   921,   922,   924,   926,   368,  2269,
     932,   934,   404,   -12,   -15,  3509,  -756,   -12,   -12,   -38,
     -26,    -4,  -756,   160,   909,   733,  1238,   935,   936,   -21,
     927,   911,   928,   912,   -12,   -12,   -12,   937,   938,   939,
    3082,  -756,  -756,  -756,  -756,  -756,  -756,  1036,  1037,  -756,
      22,   909,   735,    36,   397,     5,  -756,   738,  1039,  3082,
    3082,  3082,   340,   -11,   103,  -756,   174,  3082,  3082,  -756,
    3082,  3082,  -756,  3082,  3082,  3082,  3082,  3082,  3082,  3082,
    3082,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,   861,  2641,
    -756,  2710,   940,   747,   941,  -756,   760,  3082,  3082,  3082,
    -756,   765,   127,  -756,   767,   772,  -756,  -756,  -756,  -756,
    -756,   774,  3082,  -756,  3082,  3082,  3082,   776,  -756,   105,
    -756,   942,  1884,   420,   943,   909,   791,   944,   945,  3082,
    -756,   795,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
     799,  -756,  -756,  -756,   896,  -756,  -756,  1884,   -12,  -756,
    -756,  1049,  -756,  1056,  1057,  -756,  1059,  1060,  -756,   909,
     486,  2834,  2889,  1493,  -756,   342,  -756,  -756,  3082,   -12,
    -756,  -756,  -756,   951,   953,  1047,  -756,  -756,  -756,  -756,
    3082,   959,  -756,  -756,   116,   567,   797,  1024,  -756,  -756,
     909,   909,   804,   813,  1067,  1085,  1086,  -756,  -756,   185,
     984,   983,   203,  3082,  3082,  3082,  3082,  3082,  3082,  -756,
    -756,  -756,  -756,   225,   230,   275,   279,   288,   289,   305,
     307,   308,   313,   329,   337,  -756,  -756,   815,  -756,   819,
     821,  -756,   991,  -756,  -756,   992,  1008,  1009,  -756,  3082,
    -756,  1116,  -756,  2269,  1013,   431,  -756,  -756,  1014,  1063,
    1065,  -756,   823,  -756,  3082,  -756,  -756,   909,  -756,  -756,
     533,  1021,  1023,   -15,  -756,   825,   905,   -12,   -12,   -12,
    -756,  -756,  -756,  -756,   179,   358,    13,  -756,   345,  3082,
    3082,  3082,  3082,  3082,  3082,  3082,  3082,  -756,   -12,  1015,
    -756,  -756,   -12,  1025,  3082,  -756,   232,   326,    59,   189,
     818,  1026,  1027,   613,  -756,  -756,   909,   909,  1137,  1141,
    1142,  3082,  2765,  1044,  1045,  -756,   348,   350,   384,   394,
     395,   403,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,
    -756,  -756,  1080,   828,  -756,  -756,  -756,  3082,  3082,  2269,
     558,  -756,  -756,  3082,  -756,  -756,  -756,  -756,   -12,   -12,
    -756,  -756,  -756,  -756,  -756,  1143,  1144,  3082,  3082,  3082,
    -756,  -756,  -756,   421,   425,   458,   459,   467,   468,   499,
     514,  -756,  -756,  -756,  3082,  1033,  -756,    44,  1089,  -756,
      47,  -756,  1145,  1146,  1038,  3082,  -756,  1147,  1148,  1043,
    3082,  -756,   194,   207,  1048,  1050,  -756,  -756,   361,  -756,
    -756,  -756,  -756,  -756,   538,  1030,   830,  3082,  3082,  -756,
    -756,  -756,  -756,  -756,  -756,  3082,  1051,  -756,  -756,   834,
    -756,   560,   933,  -756,  -756,  -756,   539,   541,   550,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  1052,   -12,  1041,
     840,  1042,  1357,  1041,   847,  1053,  1054,  -756,  1055,  1058,
    1061,  -756,  1062,  1150,  1162,  1064,  3082,  -756,  1163,  1166,
    1068,  3082,  -756,  -756,  -756,    49,  -756,  -756,  -756,  1069,
     559,   581,  -756,  -756,  1070,  -756,   -12,  -756,  -756,  -756,
    -756,  3082,  -756,  -756,  1041,  -756,  2834,  2889,  1561,  -756,
     374,  -756,  1041,  -756,  -756,  -756,  -756,  -756,  -756,  1071,
    1074,  -756,  1075,  1077,  1081,  -756,  1082,  1041,   849,  -756,
    -756,  -756,  -756,  -756,  1078,  -756,   450,   303,  -756,   582,
    3082,  3082,  3082,  3082,  3082,  3082,  3082,  3082,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  1041,   -12,  3082,
    3082,  3082,  -756,  -756,  -756,   590,   636,   637,   640,   645,
     653,   654,   655,  -756,  -756,   670,   671,   672,  -756,  -756,
    -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756,  -756
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   383,   384,   385,   387,   388,   393,
     394,   404,   403,   405,   406,     0,   395,   396,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     386,     0,     0,     0,     0,     0,    87,     0,     3,     4,
      88,    89,    96,    97,    98,    99,    90,    91,   106,     0,
     107,   108,   109,    95,     0,   292,   294,   312,   314,   317,
     319,   322,   324,   326,   329,   334,   338,   340,   354,   380,
       0,   391,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   388,   393,   394,
     404,     0,   411,   337,     0,   388,    31,     0,   381,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   322,     0,   412,   291,
       0,    43,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,     0,   135,     0,     0,   100,     0,     0,
     101,     0,     0,     0,     3,     0,   378,    92,    94,   375,
     199,     0,     0,     0,     0,   389,   390,   336,   335,   407,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
      86,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     290,     0,   358,     0,     0,     0,     0,     0,   363,     0,
     362,   356,     0,     0,     0,     0,    26,     0,   407,     0,
       0,    34,     0,     0,     6,     0,     9,     0,    10,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      38,     0,     0,   113,     0,   125,     0,     0,   144,     0,
       0,     0,     3,   151,    41,    42,     0,     0,   147,     0,
       0,     0,     0,     0,   128,     0,   131,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   376,   377,   379,     0,
       0,   202,     0,     0,     0,   237,   408,     0,   256,   388,
     393,   394,   404,     0,     0,     0,     0,     0,     0,   255,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   298,   301,
     302,   295,   296,     0,   299,   300,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   313,   315,   316,   318,   320,
     321,   323,   325,   327,   328,   333,   330,   331,   332,   339,
     343,   353,   347,   348,   349,   350,   351,   352,   345,   346,
     344,   374,     0,   371,    47,     0,    81,    82,    83,    84,
      85,    44,     0,     0,   357,   398,   399,   400,    49,     0,
     366,     0,   355,     0,     0,     0,   290,    33,     0,   382,
       0,     0,     0,   385,   403,     0,     0,     0,     0,    23,
       0,    24,     0,    25,     0,   154,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   408,     0,     0,   116,   408,     0,     0,
     320,   321,   247,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   408,     0,   408,     0,     0,     0,
       0,   102,   103,   104,   105,   289,    93,     0,     0,   202,
       0,     0,     0,     0,     0,     0,   409,     0,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,   283,
       0,     0,   284,     0,     0,     0,     0,     0,     0,     0,
       0,   257,    51,    53,    55,    58,    56,    57,    52,    54,
      64,    66,    68,    71,    69,    70,    65,    67,     0,     0,
     367,     0,     0,     0,     0,   161,     0,     0,     0,     0,
     365,     0,   403,   361,     0,     0,   398,   399,   400,    32,
       5,     0,     0,    11,     0,     0,     0,     0,    16,     0,
      15,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     342,     0,    72,    75,    74,    78,    76,    77,    73,    36,
       0,    79,    80,    39,   110,   115,   114,     0,     0,   124,
     141,     0,   143,     0,     0,   248,     0,     0,   150,     0,
       0,     0,     0,     0,   162,     0,    28,    27,     0,     0,
     145,   401,   402,     0,     0,   126,   129,   130,   133,   134,
       0,     0,   200,   201,     0,     0,     0,     0,   198,   159,
       0,     0,     0,     0,     0,     0,     0,   236,   410,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   266,
     273,   280,   281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   293,   373,     0,   370,     0,
       0,    46,     0,    48,   160,     0,     0,     0,   364,     0,
     359,     0,   360,     0,     0,     0,   244,   243,     0,     0,
       0,    18,     0,    19,     0,    22,   153,     0,    30,    29,
       0,   410,   410,     0,   117,     0,   118,     0,     0,     0,
     249,   250,   149,   251,     0,     0,     0,   165,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,     0,     0,
      62,    63,     0,     0,     0,   197,     0,     0,     0,     0,
       0,     0,     0,     0,   158,   156,     0,     0,     0,     0,
       0,     0,     0,   398,   400,   277,     0,     0,     0,     0,
       0,     0,   285,   286,   287,   288,   258,   260,   262,   265,
     263,   264,   259,   261,   372,   369,   368,    45,    59,    60,
      61,   241,     0,     0,    13,    12,    17,     0,     0,     0,
       0,   152,     7,     0,    37,    40,   112,   111,   408,     0,
     122,   142,   139,   140,   252,     0,     0,     0,     0,     0,
     166,   167,   164,     0,     0,     0,     0,     0,     0,     0,
       0,   148,   146,   127,     0,     0,   138,     0,     3,   232,
       0,   229,     0,     0,     0,     0,   208,     0,     0,     0,
       0,   203,     0,     0,     0,     0,   204,   205,     0,   155,
     157,   238,   239,   240,     0,   161,     0,     0,     0,   267,
     269,   272,   270,   271,   268,     0,   410,   246,   245,     0,
      20,     0,   119,   120,   253,   254,     0,     0,     0,   168,
     170,   172,   175,   173,   174,   169,   171,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   219,     0,     0,
       0,   220,     0,     0,     0,     0,     0,   210,     0,     0,
       0,     0,   209,   206,   207,     0,   235,   276,   275,   160,
       0,     0,   242,    14,   410,     8,     0,   123,   176,   178,
     177,     0,   137,   231,     0,   180,     0,     0,     0,   179,
       0,   228,     0,   217,   218,   212,   215,   216,   211,     0,
       0,   221,     0,     0,     0,   222,     0,     0,     0,   274,
     278,   279,    21,   121,     0,   230,     0,     0,   183,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   227,
     225,   226,   214,   223,   224,   213,   234,     0,     0,     0,
       0,     0,   184,   185,   182,     0,     0,     0,     0,     0,
       0,     0,     0,   233,   136,     0,     0,     0,   186,   188,
     190,   193,   191,   192,   187,   189,   194,   196,   195
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -756,  -756,     1,  -756,  -467,  -610,  -756,  -756,  -756,  -756,
    -756,   222,  -756,  -756,  -756,  -756,  -756,  -269,  -755,  -756,
    -756,  -756,   676,  -756,  -756,   626,   758,  -313,   310,   931,
      -2,  -202,  -756,   530,   972,  -756,   620,   -16,   981,   982,
     751,    10,  -756,  -756,  -231,  -756,  -756,   -64,  -756
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   182,    59,    60,   476,    61,    62,    63,    64,
      65,   855,    66,    67,   489,    68,    69,   303,   859,    70,
      71,   330,   510,    72,   515,   574,   446,   485,   112,   162,
     118,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,   276,   411,    87,    88,   119,    89
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   413,   455,   734,   373,   507,   615,   137,   482,
     136,    24,   861,   483,   508,   288,   638,   639,   296,   512,
     128,   130,   132,   134,   135,   113,     3,   140,   142,   333,
     145,   482,   289,   149,   151,   483,   243,   621,   101,   288,
     586,   166,   252,   482,   169,   288,   482,   483,   104,   623,
     483,   527,   102,   260,   482,   180,   289,   482,   483,   482,
     114,   483,   289,   483,   177,   178,   297,   172,   528,    58,
     103,   624,   254,   862,   664,   665,   666,   334,   655,   656,
     155,   863,   657,   158,   173,   262,   223,   105,    58,   106,
     255,   235,   236,   237,   238,   239,   240,   156,   317,   244,
     159,   246,   247,   120,   121,   122,   640,   123,   223,   290,
     719,    58,   291,   318,    58,   259,   680,   309,   509,   720,
      58,   484,   864,   946,   124,   277,   830,   287,   125,   292,
     530,   294,   667,   290,   248,   310,   291,   249,   146,   290,
     841,   596,   291,   511,   147,   157,   313,   531,   315,   658,
     161,   320,   529,   321,   322,   595,   323,   324,   661,   325,
     326,   327,   325,   328,   963,   865,   919,   170,   971,   923,
     625,   987,   655,   656,   157,   171,   657,   160,   250,   866,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   834,
     359,   360,   361,   362,   363,   364,   365,   366,   422,   709,
     662,   663,   153,   867,   263,   389,   390,    58,   933,   995,
     311,   868,   174,   423,   126,   628,   934,  1009,   264,   221,
     222,   938,   468,   304,   160,   626,   627,   472,   312,   939,
     681,   532,  1016,   479,   175,   299,   300,   395,   396,   397,
     398,   399,   659,   765,   835,   836,   105,   176,   106,   102,
     265,   433,   869,   435,   301,   436,   582,   935,   223,   302,
     439,   440,  1033,   442,   266,   447,   826,   520,    91,    92,
     940,   267,   583,   517,   223,   584,   461,   462,   463,   464,
     465,   466,   467,   480,   481,   268,   470,   471,   181,    91,
      92,   781,   957,   448,   305,   870,   100,   450,   218,   219,
     936,   682,   257,   486,   487,   488,   782,   449,   491,   871,
     493,   451,    73,   941,   937,   497,   498,   100,   533,   452,
     454,   534,   535,   428,   231,   302,   726,   942,   536,   288,
     785,   537,   538,   453,   220,   539,   540,   138,   139,   225,
     226,   522,   523,   524,   526,   501,   289,   152,   154,   224,
     502,   563,   792,   857,   167,   168,   695,   793,   858,   566,
     742,   261,   336,   337,   262,   571,   673,   575,   749,   279,
     674,   750,   751,   143,   414,   415,   675,   144,   752,   676,
     677,   753,   754,   678,   837,   755,   756,   279,   838,   273,
     232,   774,   775,   274,   281,   503,   601,   282,   283,   504,
    1000,   839,   794,  1001,  1002,   610,   795,   482,   550,   551,
    1003,   483,   200,  1004,  1005,   796,   797,  1006,  1007,   541,
     564,   138,   227,   290,   767,   552,   291,   553,   554,   234,
    1023,   233,   798,   555,   799,   800,   228,   229,   230,   581,
     801,   241,   293,   180,   295,   298,   591,   860,   593,   556,
     594,   105,   858,   106,   597,   598,   802,   557,   821,   314,
     285,   316,   200,   286,   803,   305,   245,   679,   602,   757,
     604,   588,   842,   618,   589,   889,  1019,   890,   285,   279,
    1020,   286,   945,   253,   635,   840,   281,   858,   609,   282,
     283,   262,    73,  1021,   200,   697,   743,   700,   651,   278,
     744,  1008,   279,   280,   605,   424,   262,   879,   880,   281,
     256,   891,   282,   283,   606,   607,   284,   670,   671,   672,
     715,   892,   893,   608,   613,   683,   684,   262,   685,   686,
     894,   687,   688,   689,   690,   691,   692,   693,   694,   272,
     723,   550,   878,   724,   920,   551,   183,   924,   909,   184,
     185,   815,   910,   275,   262,   304,   186,   305,   192,   187,
     188,   193,   194,   189,   190,   705,   706,   707,   195,   306,
     285,   196,   197,   286,   308,   198,   199,  1022,   552,   553,
     714,   400,   329,   716,   717,   911,   912,   554,   555,   401,
     722,   402,   403,   285,   913,   914,   286,   730,   432,   262,
     404,   405,   210,   211,   212,   213,   214,   215,   406,   407,
     201,   202,   203,   204,   216,   735,   408,   409,   410,   556,
     205,   206,   207,   307,   217,   766,   915,   400,   331,   745,
     746,   748,   988,   332,   557,   401,   758,   402,   403,   506,
     191,   916,   208,   209,   474,   475,   404,   405,   763,   813,
    -411,  -411,   200,   822,   406,   407,   823,   335,   703,   602,
     820,   604,   408,   409,   410,   947,   958,   338,   959,   416,
     608,   786,   787,   788,   789,   790,   791,   960,   900,   808,
     955,   262,   417,   262,  -411,  -411,   990,  -411,  -411,  -411,
    -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,  -411,
     418,   810,   305,  -411,   269,   270,   271,   811,   991,  1024,
     550,  -411,  -411,  -411,  -411,   419,  -411,  1038,   886,  -411,
    -411,  -411,  -411,  -411,  -411,   420,  -411,  -411,  -411,   421,
    -411,   367,   368,   369,   370,   371,   372,   425,   374,   375,
     376,   377,   378,   379,   380,   381,   427,   843,   844,   845,
     846,   847,   848,   849,   850,   899,   551,   552,   400,   901,
     553,   412,   856,  1039,  1040,   554,   401,  1041,   402,   403,
     438,   262,  1042,   555,   556,   557,   426,   404,   405,   884,
    1043,  1044,  1045,   614,   616,   406,   407,   619,   620,   622,
     602,   604,   608,   408,   409,   410,    73,  1046,  1047,  1048,
     382,   383,   384,   428,   645,   646,   647,   768,   458,   459,
     769,   770,   771,   772,   429,   897,   898,    90,   431,    91,
      92,    93,    94,    95,    96,    97,   336,   460,   872,   336,
     469,   873,    98,   874,   875,   906,   907,   908,   386,   387,
     388,   473,   337,   477,   337,    99,   430,   100,   437,   494,
     337,   434,   917,   496,   337,   629,   630,   660,   630,   922,
     668,   262,   456,   928,     4,     5,     6,     7,   932,   702,
     262,   107,   108,   109,   110,    12,    13,    14,    15,    16,
      17,    18,   704,   262,   457,   950,   951,   708,   262,   710,
     711,   478,   111,   952,   712,   262,   336,   713,   718,   589,
     490,    90,    33,    91,    92,    93,    94,    95,    96,    97,
      41,    42,    43,   727,   630,    46,    98,   731,   262,   495,
     970,   732,   262,   733,   475,    50,   776,   630,   736,   518,
     492,   100,   829,   475,   982,   777,   630,   804,   262,   986,
     499,   805,   711,   806,   262,   336,   819,   828,   337,   759,
     896,   262,   949,   262,    52,    53,   954,   262,   500,   856,
     956,   475,   964,   630,   996,   997,   999,   513,    54,   972,
     630,  1017,   630,   163,   165,    55,   393,   394,   501,   502,
     503,   505,    57,   504,   521,   514,   519,    58,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,  1025,  1026,
    1027,  1028,  1029,  1030,  1031,  1032,   217,   552,   553,   554,
     555,   556,   558,   557,   223,   559,   560,  1035,  1036,  1037,
     561,   567,   576,   568,   569,   578,   579,   580,   577,   585,
     590,   599,   600,   602,   603,   302,   641,   643,   642,   644,
     604,   605,   606,   827,   607,   650,   608,   831,   832,   833,
     652,   653,   611,   669,   612,   636,   637,   648,   649,   737,
     701,   703,   721,   725,   728,   729,   738,   739,   851,   740,
     741,   760,   853,   761,   762,     4,     5,     6,     7,   764,
     773,   778,   339,   340,   341,   342,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   343,   344,    24,   779,
     780,    25,    26,    27,    28,   345,   346,    31,    32,   783,
     784,   807,   808,    33,    34,    35,    36,    37,    38,   347,
     348,    41,    42,    43,    44,    45,    46,    47,   809,   810,
     812,    48,    49,   814,   816,   817,    50,   818,   902,   903,
      51,   824,   852,   825,   881,   854,   876,   877,   882,   883,
     887,   888,   895,   904,   905,   918,   921,   948,   927,   925,
     926,   929,   930,   931,   979,    52,    53,   858,   943,   965,
     944,   953,   961,   973,   974,   975,   980,   983,   976,    54,
     984,   977,   978,   994,   981,   654,    55,   699,   985,   385,
     992,  1010,    56,    57,  1011,  1012,   989,  1013,    58,   349,
    1018,  1014,  1015,   587,   391,     0,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   962,     0,
       0,     0,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   631,   632,    24,     0,   993,    25,    26,    27,
      28,    29,   633,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,    48,    49,     0,
       0,     0,    50,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1034,     0,
       0,    52,    53,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,    55,     0,     0,     0,     0,     0,    56,    57,
       4,     5,     6,     7,    58,   634,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   966,   967,    24,     0,     0,    25,    26,    27,    28,
      29,   968,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
       0,    50,     0,     0,     0,    51,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   115,   108,   109,   110,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      52,    53,     0,     0,     0,     0,   111,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,    56,    57,    46,
       0,     0,     0,    58,   969,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   115,   108,   109,   110,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    52,    53,
       0,     0,     0,     0,   111,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,   141,    57,    46,     0,     0,
       0,    58,   525,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   115,   108,   109,   110,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    52,    53,     0,     0,
       0,     0,   111,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,   141,    57,    46,     0,     0,     0,    58,
     747,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,   141,    57,     4,     5,     6,     7,    58,   998,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
      26,    27,    28,    29,    30,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,     0,    50,     0,     0,     0,    51,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   115,   108,
     109,   110,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,   111,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
      56,    57,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   115,   108,   109,   110,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,   111,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,   116,   117,
      46,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   115,   108,   109,   110,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,   111,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,   141,    57,    46,     0,
       0,     0,    58,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   115,   108,   109,   110,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    52,    53,     0,
       0,     0,     0,     0,   111,    28,     0,     0,     0,     0,
       0,    54,     0,     0,    33,     0,     0,     0,    55,     0,
       0,     0,    41,    42,    43,    57,   179,    46,     0,     0,
      58,     4,     5,     6,     7,     0,     0,    50,   115,   108,
     109,   110,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   111,
      28,     0,     0,     0,     0,     0,    52,    53,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      54,     0,    46,     0,     0,     0,     0,    55,     0,     0,
       0,     0,    50,     0,    57,   242,     0,     0,     0,    58,
       4,     5,     6,     7,     0,     0,     0,   115,   108,   109,
     110,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    52,    53,     0,     0,     0,     0,     0,   111,    28,
       0,     0,     0,     0,     0,    54,     0,     0,    33,     0,
       0,     0,    55,     0,     0,     0,    41,    42,    43,    57,
     251,    46,     0,     0,    58,     4,     5,     6,     7,     0,
       0,    50,   115,   108,   109,   110,    12,    13,    14,    15,
      16,    17,    18,     0,     0,   319,     0,     0,     0,     0,
       0,     0,     0,   111,    28,     0,     0,     0,     0,     0,
      52,    53,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    54,     0,    46,     0,     0,     0,
       0,    55,     0,     0,     0,     0,    50,     0,    57,   258,
       0,     0,     0,    58,     4,     5,     6,     7,     0,     0,
       0,   115,   108,   109,   110,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    52,    53,     0,     0,     0,
       0,     0,   111,    28,     0,     0,     0,     0,     0,    54,
       0,     0,    33,     0,     0,     0,    55,     0,     0,     0,
      41,    42,    43,    57,     0,    46,     0,     0,    58,     0,
       0,     0,     0,     0,     0,    50,     0,     0,   181,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   115,
     108,   109,   110,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
     111,    28,     0,     0,     0,     0,     0,     0,    54,     0,
      33,     0,     0,     0,     0,    55,     0,     0,    41,    42,
      43,     0,    57,    46,     0,     0,     0,    58,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   115,   108,
     109,   110,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    52,    53,     0,     0,     0,     0,     0,   111,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   516,    46,     0,     0,    58,     4,     5,     6,     7,
       0,     0,    50,   115,   108,   109,   110,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,    28,     0,     0,     0,     0,
       0,    52,    53,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    54,     0,    46,     0,     0,
       0,     0,    55,     0,     0,     0,     0,    50,     0,    57,
     562,     0,     0,     0,    58,     4,     5,     6,     7,     0,
       0,     0,   115,   108,   109,   110,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    52,    53,     0,     0,
       0,     0,     0,   111,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   565,    46,     0,     0,    58,
       4,     5,     6,     7,     0,     0,    50,   115,   108,   109,
     110,   572,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,    28,
       0,     0,     0,     0,     0,    52,    53,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    54,
       0,    46,     0,     0,     0,     0,    55,     0,     0,     0,
       0,    50,     0,    57,   570,     0,     0,     0,    58,     4,
       5,   443,     7,     0,     0,     0,   115,   108,   109,   110,
     444,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      52,    53,     0,     0,     0,     0,     0,   111,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   573,
      46,     0,     0,    58,     4,     5,     6,     7,     0,     0,
      50,   115,   108,   109,   110,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,    28,     0,     0,     0,     0,     0,    52,
      53,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    54,     0,    46,     0,     0,     0,     0,
      55,     0,     0,     0,     0,    50,     0,    57,   179,     0,
       0,     0,    58,     4,     5,     6,     7,     0,     0,     0,
     115,   108,   109,   110,   572,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    52,    53,     0,     0,     0,     0,
       0,   111,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   696,    46,     0,     0,    58,     4,     5,
       6,     7,     0,     0,    50,   115,   108,   109,   110,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,    28,     0,     0,
       0,     0,     0,    52,    53,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    54,     0,    46,
       0,     0,     0,     0,    55,     0,     0,     0,     0,    50,
       0,    57,   698,     0,     0,     0,    58,     4,     5,     6,
       7,     0,     0,     0,   115,   108,   109,   110,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    52,    53,
       0,     0,     0,     0,     0,   111,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   885,    46,     0,
       0,    58,     4,     5,     6,     7,     0,     0,    50,   115,
     108,   109,   110,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,    28,     0,     0,     0,     0,     0,    52,    53,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    54,     0,    46,     0,     0,     0,     0,    55,     0,
       0,     0,     0,    50,     0,   127,     0,     0,     0,     0,
      58,     4,     5,     6,     7,     0,     0,     0,   115,   108,
     109,   110,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    52,    53,     0,     0,     0,     0,     0,   111,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
     129,     0,    46,     0,     0,    58,     4,     5,     6,     7,
       0,     0,    50,   115,   108,   109,   110,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   111,    28,     0,     0,     0,     0,
       0,    52,    53,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    54,     0,    46,     0,     0,
       0,     0,    55,     0,     0,     0,     0,    50,     0,   131,
       0,     0,     0,     0,    58,     4,     5,     6,     7,     0,
       0,     0,   115,   108,   109,   110,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    52,    53,     0,     0,
       0,     0,     0,   111,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,   133,     0,    46,     0,     0,    58,
       4,     5,     6,     7,     0,     0,    50,   115,   108,   109,
     110,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   111,    28,
       0,     0,     0,     0,     0,    52,    53,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    54,
       0,    46,     0,     0,     0,     0,    55,     0,     0,     0,
       0,    50,     0,    57,     0,     0,     0,     0,    58,     4,
       5,     6,     7,     0,     0,     0,   115,   108,   109,   110,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      52,    53,     0,     0,     0,     0,     0,   111,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,   148,     0,
      46,     0,     0,    58,     4,     5,     6,     7,     0,     0,
      50,   115,   108,   109,   110,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   111,    28,     0,     0,     0,     0,     0,    52,
      53,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    54,     0,    46,     0,     0,     0,     0,
      55,     0,     0,     0,     0,    50,     0,   150,     0,     0,
       0,     0,    58,     4,     5,     6,     7,     0,     0,     0,
     115,   108,   109,   110,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    52,    53,     0,     0,     0,     0,
       0,   111,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,     0,    46,     0,     0,   164,     4,     5,
     443,     7,     0,     0,    50,   115,   108,   109,   110,   444,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,    28,     0,     0,
       0,     0,     0,    52,    53,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    54,     0,    46,
       0,     0,     0,     0,    55,     0,     0,     0,     0,    50,
       0,   441,     0,     0,     0,     0,    58,     4,     5,     6,
       7,     0,     0,     0,   115,   108,   109,   110,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    52,    53,
       0,     0,     0,     0,     0,   111,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,   445,     0,    46,     0,
       0,    58,     4,     5,     6,     7,     0,     0,    50,   115,
     108,   109,   110,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,    28,     0,     0,     0,     0,     0,    52,    53,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    54,     0,    46,     0,     0,     0,     0,    55,     0,
       0,     0,     0,    50,     0,   592,     0,     0,     0,     0,
      58,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
     617,     0,     0,     0,     0,    58
};

static const yytype_int16 yycheck[] =
{
       2,     0,   233,   272,   614,   207,    14,   474,    14,    10,
      26,    26,   767,    14,    22,    26,    37,    38,    75,   332,
      22,    23,    24,    25,    26,    15,     0,    29,    30,    14,
      32,    10,    43,    35,    36,    14,   100,    75,   124,    26,
      72,    43,   106,    10,    46,    26,    10,    14,   106,    75,
      14,    26,   108,   117,    10,    57,    43,    10,    14,    10,
      14,    14,    43,    14,    54,    55,   123,    14,    43,   126,
     126,    75,   108,    14,    69,    70,    71,    62,    56,    57,
      26,    22,    60,    26,    31,   123,   112,   119,   126,   121,
     126,    93,    94,    95,    96,    97,    98,    43,    29,   101,
      43,   103,   104,    10,    11,    12,   127,    14,   112,   120,
       5,   126,   123,    44,   126,   117,   127,   108,   126,    14,
     126,   122,    63,   878,    31,   127,   736,   129,    35,   131,
      26,   133,   127,   120,    14,   126,   123,    17,    12,   120,
     127,   454,   123,   122,    12,   120,   148,    43,   150,   127,
     126,   153,   127,   155,   156,   122,   158,   159,   122,   161,
     162,   163,   164,   165,   919,   106,   122,    14,   923,   122,
      10,   122,    56,    57,   120,    14,    60,   120,    58,   120,
     182,   183,   184,   185,   186,   187,   188,   189,   190,    10,
     192,   193,   194,   195,   196,   197,   198,   199,   106,    72,
     513,   514,   121,    14,   106,   221,   222,   126,    14,   964,
     108,    22,    14,   121,   121,   484,    22,   972,   120,    73,
      74,    14,   286,   120,   120,    65,    66,   291,   126,    22,
     127,   127,   987,   297,    10,    73,    74,   227,   228,   229,
     230,   231,   511,   127,    65,    66,   119,    10,   121,   108,
     106,   253,    63,   255,   121,   257,   104,    63,   112,   126,
     262,   263,  1017,   265,   120,   267,   733,   126,    93,    94,
      63,   106,   120,   337,   112,   123,   278,   279,   280,   281,
     282,   283,   284,   299,   300,   120,   288,   289,    67,    93,
      94,   106,   902,   106,   120,   106,   121,   106,   110,   111,
     106,   127,   106,   302,   306,   307,   121,   120,   310,   120,
     312,   120,     2,   106,   120,   317,   318,   121,    26,   106,
     121,    29,    30,   120,   105,   126,   595,   120,    36,    26,
     127,    39,    40,   120,   100,    43,    44,    27,    28,   113,
     114,   343,   344,   345,   346,   120,    43,    37,    38,   103,
     120,   415,   127,   121,    44,    45,   558,   127,   126,   423,
     629,   120,   122,   123,   123,   429,    26,   431,    26,    29,
      30,    29,    30,    10,   120,   121,    36,    14,    36,    39,
      40,    39,    40,    43,    26,    43,    44,    29,    30,    10,
     119,   660,   661,    14,    36,   120,   460,    39,    40,   120,
      26,    43,   127,    29,    30,   469,   127,    10,   120,   120,
      36,    14,   120,    39,    40,   127,   127,    43,    44,   127,
     422,   111,   101,   120,   655,   120,   123,   120,   120,    14,
     127,   119,   127,   120,   127,   127,   115,   116,   117,   441,
     127,    14,   132,   445,   134,   135,   448,   121,   450,   120,
     452,   119,   126,   121,   456,   457,   127,   120,   727,   149,
     120,   151,   120,   123,   127,   120,    14,   127,   120,   127,
     120,   120,   127,   475,   123,   127,    26,   127,   120,    29,
      30,   123,   121,   124,   486,   127,    36,   126,   120,    39,
      40,   123,   182,    43,   120,   559,    10,   561,   500,    26,
      14,   127,    29,    30,   120,   122,   123,   776,   777,    36,
     120,   127,    39,    40,   120,   120,    43,   519,   520,   521,
     584,   127,   127,   120,   120,   527,   528,   123,   530,   531,
     127,   533,   534,   535,   536,   537,   538,   539,   540,    14,
     120,   120,   773,   123,   857,   120,    26,   860,   127,    29,
      30,   120,   127,    10,   123,   120,    36,   120,    26,    39,
      40,    29,    30,    43,    44,   567,   568,   569,    36,   106,
     120,    39,    40,   123,   126,    43,    44,   127,   120,   120,
     582,    14,    53,   585,   586,   127,   127,   120,   120,    22,
     592,    24,    25,   120,   127,   127,   123,   599,   122,   123,
      33,    34,    80,    81,    82,    83,    84,    85,    41,    42,
      76,    77,    78,    79,    92,   617,    49,    50,    51,   120,
      86,    87,    88,   106,   102,    58,   127,    14,   126,   631,
     632,   633,   945,   121,   120,    22,   638,    24,    25,   329,
     120,   127,   108,   109,    27,    28,    33,    34,   650,   713,
      39,    40,   120,   120,    41,    42,   123,   126,   120,   120,
     724,   120,    49,    50,    51,   127,   127,   127,   127,   120,
     120,   673,   674,   675,   676,   677,   678,   127,   120,   120,
     120,   123,   120,   123,    73,    74,   127,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
     120,   120,   120,    92,    10,    11,    12,   709,   127,   127,
     120,   100,   101,   102,   103,   120,   105,   127,   782,   108,
     109,   110,   111,   112,   113,   120,   115,   116,   117,   120,
     119,   201,   202,   203,   204,   205,   206,   125,   208,   209,
     210,   211,   212,   213,   214,   215,   127,   749,   750,   751,
     752,   753,   754,   755,   756,   819,   120,   120,    14,   823,
     120,    17,   764,   127,   127,   120,    22,   127,    24,    25,
     122,   123,   127,   120,   120,   120,   109,    33,    34,   781,
     127,   127,   127,   473,   474,    41,    42,   477,   478,   479,
     120,   120,   120,    49,    50,    51,   486,   127,   127,   127,
       7,     8,     9,   120,   494,   495,   496,    10,   122,   123,
      13,    14,    15,    16,   121,   817,   818,    91,   121,    93,
      94,    95,    96,    97,    98,    99,   122,   123,    10,   122,
     123,    13,   106,    15,    16,   837,   838,   839,   218,   219,
     220,   122,   123,   122,   123,   119,    14,   121,   120,   122,
     123,    14,   854,   122,   123,   122,   123,   122,   123,   858,
     122,   123,   106,   865,     3,     4,     5,     6,   870,   122,
     123,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,   122,   123,   106,   887,   888,   122,   123,   122,
     123,    10,    31,   895,   122,   123,   122,   123,   122,   123,
      14,    91,    41,    93,    94,    95,    96,    97,    98,    99,
      49,    50,    51,   122,   123,    54,   106,   122,   123,    27,
     922,   122,   123,    27,    28,    64,   122,   123,   618,   119,
      14,   121,    27,    28,   936,   122,   123,   122,   123,   941,
      10,   122,   123,   122,   123,   122,   123,   122,   123,   639,
     122,   123,   122,   123,    93,    94,   122,   123,   120,   961,
      27,    28,   122,   123,   966,   967,   968,   121,   107,   122,
     123,   122,   123,    42,    43,   114,   225,   226,   120,   120,
     120,   127,   121,   120,   106,   121,   124,   126,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,  1000,  1001,
    1002,  1003,  1004,  1005,  1006,  1007,   102,   120,   120,   120,
     120,   120,    89,   120,   112,   121,    14,  1019,  1020,  1021,
     121,   106,   125,   106,   106,   127,   120,   120,   109,    72,
     120,   106,    10,   120,   120,   126,   109,   109,   127,   127,
     120,   120,   120,   733,   120,   106,   120,   737,   738,   739,
      14,    14,   120,    14,   120,   120,   120,   120,   120,    10,
     120,   120,   120,   120,   120,   120,    10,    10,   758,    10,
      10,   120,   762,   120,    27,     3,     4,     5,     6,   120,
      56,    14,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    14,
      14,    29,    30,    31,    32,    33,    34,    35,    36,   125,
     127,   120,   120,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   120,   120,
      14,    59,    60,   120,   120,    72,    64,    72,   828,   829,
      68,   120,   127,   120,     7,   120,   120,   120,     7,     7,
     106,   106,    72,    10,    10,   122,    67,   127,   120,    14,
      14,    14,    14,   120,    14,    93,    94,   126,   120,   127,
     120,   120,   120,   120,   120,   120,    14,    14,   120,   107,
      14,   120,   120,   961,   120,   509,   114,   561,   120,   217,
     120,   120,   120,   121,   120,   120,   127,   120,   126,   127,
     122,   120,   120,   445,   223,    -1,   224,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   918,    -1,
      -1,    -1,   922,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,   956,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1018,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,   120,   121,
       3,     4,     5,     6,   126,   127,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    68,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    41,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    49,    50,    51,   120,   121,    54,
      -1,    -1,    -1,   126,   127,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    41,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    49,    50,    51,   120,   121,    54,    -1,    -1,
      -1,   126,   127,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    41,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      49,    50,    51,   120,   121,    54,    -1,    -1,    -1,   126,
     127,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,   120,   121,     3,     4,     5,     6,   126,   127,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    41,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    41,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,    41,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   107,    -1,    -1,    41,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    49,    50,    51,   121,   122,    54,    -1,    -1,
     126,     3,     4,     5,     6,    -1,    -1,    64,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     107,    -1,    54,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    64,    -1,   121,   122,    -1,    -1,    -1,   126,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    41,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    49,    50,    51,   121,
     122,    54,    -1,    -1,   126,     3,     4,     5,     6,    -1,
      -1,    64,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,   107,    -1,    54,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    64,    -1,   121,   122,
      -1,    -1,    -1,   126,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   107,
      -1,    -1,    41,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      49,    50,    51,   121,    -1,    54,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    67,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      41,    -1,    -1,    -1,    -1,   114,    -1,    -1,    49,    50,
      51,    -1,   121,    54,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    41,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    49,    50,    51,
     121,   122,    54,    -1,    -1,   126,     3,     4,     5,     6,
      -1,    -1,    64,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   107,    -1,    54,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    64,    -1,   121,
     122,    -1,    -1,    -1,   126,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    41,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    49,    50,    51,   121,   122,    54,    -1,    -1,   126,
       3,     4,     5,     6,    -1,    -1,    64,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,   107,
      -1,    54,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    64,    -1,   121,   122,    -1,    -1,    -1,   126,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    41,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    49,    50,    51,   121,   122,
      54,    -1,    -1,   126,     3,     4,     5,     6,    -1,    -1,
      64,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   107,    -1,    54,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    64,    -1,   121,   122,    -1,
      -1,    -1,   126,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    41,    -1,    -1,    -1,   114,    -1,    -1,    -1,    49,
      50,    51,   121,   122,    54,    -1,    -1,   126,     3,     4,
       5,     6,    -1,    -1,    64,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   107,    -1,    54,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    64,
      -1,   121,   122,    -1,    -1,    -1,   126,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    41,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    49,    50,    51,   121,   122,    54,    -1,
      -1,   126,     3,     4,     5,     6,    -1,    -1,    64,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   107,    -1,    54,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    64,    -1,   121,    -1,    -1,    -1,    -1,
     126,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    41,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    49,    50,    51,
     121,    -1,    54,    -1,    -1,   126,     3,     4,     5,     6,
      -1,    -1,    64,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   107,    -1,    54,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    64,    -1,   121,
      -1,    -1,    -1,    -1,   126,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     107,    -1,    -1,    41,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    49,    50,    51,   121,    -1,    54,    -1,    -1,   126,
       3,     4,     5,     6,    -1,    -1,    64,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,   107,
      -1,    54,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    64,    -1,   121,    -1,    -1,    -1,    -1,   126,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   107,    -1,    -1,    41,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    49,    50,    51,   121,    -1,
      54,    -1,    -1,   126,     3,     4,     5,     6,    -1,    -1,
      64,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   107,    -1,    54,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    64,    -1,   121,    -1,    -1,
      -1,    -1,   126,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   107,    -1,
      -1,    41,    -1,    -1,    -1,   114,    -1,    -1,    -1,    49,
      50,    51,   121,    -1,    54,    -1,    -1,   126,     3,     4,
       5,     6,    -1,    -1,    64,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   107,    -1,    54,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    64,
      -1,   121,    -1,    -1,    -1,    -1,   126,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   107,    -1,    -1,    41,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    49,    50,    51,   121,    -1,    54,    -1,
      -1,   126,     3,     4,     5,     6,    -1,    -1,    64,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   107,    -1,    54,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    64,    -1,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    -1,   126
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   129,   130,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      64,    68,    93,    94,   107,   114,   120,   121,   126,   131,
     132,   134,   135,   136,   137,   138,   140,   141,   143,   144,
     147,   148,   151,   156,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   173,   174,   176,
      91,    93,    94,    95,    96,    97,    98,    99,   106,   119,
     121,   124,   108,   126,   106,   119,   121,    10,    11,    12,
      13,    31,   156,   169,    14,    10,   120,   121,   158,   175,
      10,    11,    12,    14,    31,    35,   121,   121,   158,   121,
     158,   121,   158,   121,   158,   158,   165,    14,   156,   156,
     158,   120,   158,    10,    14,   158,    12,    12,   121,   158,
     121,   158,   156,   121,   156,    26,    43,   120,    26,    43,
     120,   126,   157,   157,   126,   157,   158,   156,   156,   158,
      14,    14,    14,    31,    14,    10,    10,   169,   169,   122,
     158,    67,   130,    26,    29,    30,    36,    39,    40,    43,
      44,   120,    26,    29,    30,    36,    39,    40,    43,    44,
     120,    76,    77,    78,    79,    86,    87,    88,   108,   109,
      80,    81,    82,    83,    84,    85,    92,   102,   110,   111,
     100,    73,    74,   112,   103,   113,   114,   101,   115,   116,
     117,   105,   119,   119,    14,   158,   158,   158,   158,   158,
     158,    14,   122,   175,   158,    14,   158,   158,    14,    17,
      58,   122,   175,   124,   108,   126,   120,   106,   122,   158,
     175,   120,   123,   106,   120,   106,   120,   106,   120,    10,
      11,    12,    14,    10,    14,    10,   171,   158,    26,    29,
      30,    36,    39,    40,    43,   120,   123,   158,    26,    43,
     120,   123,   158,   156,   158,   156,    75,   123,   156,    73,
      74,   121,   126,   145,   120,   120,   106,   106,   126,   108,
     126,   108,   126,   158,   156,   158,   156,    29,    44,    23,
     158,   158,   158,   158,   158,   158,   158,   158,   158,    53,
     149,   126,   121,    14,    62,   126,   122,   123,   127,    10,
      11,    12,    13,    24,    25,    33,    34,    47,    48,   127,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   161,   161,   161,
     161,   161,   161,   159,   161,   161,   161,   161,   161,   161,
     161,   161,     7,     8,     9,   162,   164,   164,   164,   165,
     165,   166,   167,   168,   168,   169,   169,   169,   169,   169,
      14,    22,    24,    25,    33,    34,    41,    42,    49,    50,
      51,   172,    17,   172,   120,   121,   120,   120,   120,   120,
     120,   120,   106,   121,   122,   125,   109,   127,   120,   121,
      14,   121,   122,   158,    14,   158,   158,   120,   122,   158,
     158,   121,   158,     5,    14,   121,   154,   158,   106,   120,
     106,   120,   106,   120,   121,   145,   106,   106,   122,   123,
     123,   158,   158,   158,   158,   158,   158,   158,   175,   123,
     158,   158,   175,   122,    27,    28,   133,   122,    10,   175,
     165,   165,    10,    14,   122,   155,   130,   158,   158,   142,
      14,   158,    14,   158,   122,    27,   122,   158,   158,    10,
     120,   120,   120,   120,   120,   127,   156,    14,    22,   126,
     150,   122,   155,   121,   121,   152,   122,   175,   119,   124,
     126,   106,   158,   158,   158,   127,   158,    26,    43,   127,
      26,    43,   127,    26,    29,    30,    36,    39,    40,    43,
      44,   127,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,    89,   121,
      14,   121,   122,   175,   158,   122,   175,   106,   106,   106,
     122,   175,    14,   122,   153,   175,   125,   109,   127,   120,
     120,   158,   104,   120,   123,    72,    72,   154,   120,   123,
     120,   158,   121,   158,   158,   122,   155,   158,   158,   106,
      10,   175,   120,   120,   120,   120,   120,   120,   120,   120,
     175,   120,   120,   120,   156,   132,   156,   121,   158,   156,
     156,    75,   156,    75,    75,    10,    65,    66,   145,   122,
     123,    24,    25,    34,   127,   158,   120,   120,    37,    38,
     127,   109,   127,   109,   127,   156,   156,   156,   120,   120,
     106,   158,    14,    14,   150,    56,    57,    60,   127,   145,
     122,   122,   155,   155,    69,    70,    71,   127,   122,    14,
     158,   158,   158,    26,    30,    36,    39,    40,    43,   127,
     127,   127,   127,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   122,   175,   122,   153,
     175,   120,   122,   120,   122,   158,   158,   158,   122,    72,
     122,   123,   122,   123,   158,   175,   158,   158,   122,     5,
      14,   120,   158,   120,   123,   120,   145,   122,   120,   120,
     158,   122,   122,    27,   133,   158,   156,    10,    10,    10,
      10,    10,   145,    10,    14,   158,   158,   127,   158,    26,
      29,    30,    36,    39,    40,    43,    44,   127,   158,   156,
     120,   120,    27,   158,   120,   127,    58,   172,    10,    13,
      14,    15,    16,    56,   145,   145,   122,   122,    14,    14,
      14,   106,   121,   125,   127,   127,   158,   158,   158,   158,
     158,   158,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   122,   122,   122,   120,   120,   120,
     120,   158,    14,   175,   120,   120,   120,    72,    72,   123,
     175,   145,   120,   123,   120,   120,   132,   156,   122,    27,
     133,   156,   156,   156,    10,    65,    66,    26,    30,    43,
     127,   127,   127,   158,   158,   158,   158,   158,   158,   158,
     158,   156,   127,   156,   120,   139,   158,   121,   126,   146,
     121,   146,    14,    22,    63,   106,   120,    14,    22,    63,
     106,   120,    10,    13,    15,    16,   120,   120,   172,   145,
     145,     7,     7,     7,   158,   122,   175,   106,   106,   127,
     127,   127,   127,   127,   127,    72,   122,   158,   158,   175,
     120,   175,   156,   156,    10,    10,   158,   158,   158,   127,
     127,   127,   127,   127,   127,   127,   127,   158,   122,   122,
     155,    67,   130,   122,   155,    14,    14,   120,   158,    14,
      14,   120,   158,    14,    22,    63,   106,   120,    14,    22,
      63,   106,   120,   120,   120,   121,   146,   127,   127,   122,
     158,   158,   158,   120,   122,   120,    27,   133,   127,   127,
     127,   120,   156,   146,   122,   127,    24,    25,    34,   127,
     158,   146,   122,   120,   120,   120,   120,   120,   120,    14,
      14,   120,   158,    14,    14,   120,   158,   122,   155,   127,
     127,   127,   120,   156,   139,   146,   158,   158,   127,   158,
      26,    29,    30,    36,    39,    40,    43,    44,   127,   146,
     120,   120,   120,   120,   120,   120,   146,   122,   122,    26,
      30,    43,   127,   127,   127,   158,   158,   158,   158,   158,
     158,   158,   158,   146,   156,   158,   158,   158,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   128,   129,   130,   130,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     132,   132,   132,   132,   132,   132,   132,   132,   133,   133,
     133,   133,   133,   133,   134,   134,   135,   135,   135,   135,
     136,   136,   137,   137,   137,   138,   138,   138,   139,   140,
     140,   140,   140,   140,   140,   141,   141,   142,   142,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     144,   144,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   147,   148,   149,
     149,   149,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   151,   152,   152,   152,
     152,   153,   153,   154,   154,   154,   154,   155,   155,   155,
     155,   155,   155,   155,   155,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   157,
     158,   158,   158,   159,   159,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   161,   161,   162,   162,   162,   163,   163,
     164,   164,   164,   165,   165,   166,   166,   167,   167,   167,
     168,   168,   168,   168,   168,   169,   169,   169,   169,   170,
     170,   171,   171,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   173,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   175,   175,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176
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
       5,     4,     4,     4,     4,     4,     2,     1,     1,     1,
       1,     1,     2,     4,     2,     1,     1,     1,     1,     1,
       2,     2,     4,     4,     4,     4,     1,     1,     1,     1,
       5,     7,     7,     3,     5,     5,     4,     6,     3,     5,
       5,     7,     4,     6,     5,     3,     5,     7,     3,     5,
       5,     3,     2,     5,     5,     2,    12,     9,     1,     7,
       7,     5,     7,     5,     3,     5,     7,     0,     4,     6,
       5,     3,     7,     6,     4,     7,     6,     7,     6,     5,
       6,     5,     3,     4,     5,     4,     5,     5,     6,     6,
       6,     6,     6,     6,     6,     6,     7,     7,     7,     3,
       3,     4,     5,     4,     5,     5,     6,     6,     6,     6,
       6,     6,     6,     6,     7,     7,     7,     6,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     6,     6,     6,     5,
       5,     6,     6,     7,     7,     7,     7,     7,     6,     4,
       7,     6,     4,     8,     7,     5,     5,     0,     4,     4,
       4,     3,     5,     3,     3,     5,     5,     1,     2,     3,
       3,     3,     4,     5,     5,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     2,     1,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     1,     2,     2,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     4,     3,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     4,     4,
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
#line 350 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2702 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 360 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2708 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 361 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2714 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 365 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2720 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 367 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2726 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 369 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2732 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 371 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2740 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 375 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2746 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 377 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2752 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 379 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2758 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 381 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2766 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 385 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2773 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 388 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2781 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 392 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2787 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 394 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2793 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 396 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2799 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 398 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2805 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 400 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2811 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 402 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2819 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 406 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2827 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 410 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2833 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 412 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2839 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2845 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 416 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2851 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 418 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2857 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 420 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2863 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 422 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2869 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 424 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2875 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 426 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2881 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 428 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2887 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 430 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2893 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 432 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2899 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 434 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2905 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 436 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2911 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 438 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2918 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 441 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2925 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 444 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2931 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 446 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2938 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 449 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2945 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 452 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2951 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 454 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2957 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 456 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2963 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 458 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2969 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 460 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2978 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 465 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2986 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 469 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2994 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 473 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3003 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 478 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3011 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 481 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3017 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 483 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3023 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 485 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3029 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 487 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3035 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 489 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3041 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 491 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3048 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 494 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3054 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 496 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3060 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 498 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3066 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 500 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3073 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 503 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3080 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 506 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3087 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 509 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3094 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 512 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3101 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 515 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3107 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 517 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3113 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 519 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3119 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 521 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3125 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 523 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3132 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 526 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3138 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 528 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3144 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 530 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3150 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 532 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3157 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 535 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3164 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 538 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3172 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 542 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3179 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 545 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3185 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 547 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3191 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 549 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3197 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 551 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3204 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 554 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3211 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 557 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3218 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 560 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3225 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 563 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3232 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 566 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3239 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 569 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3246 "raku.tab.c"
    break;

  case 86: /* stmt: expr ';'  */
#line 571 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3252 "raku.tab.c"
    break;

  case 87: /* stmt: ';'  */
#line 572 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3258 "raku.tab.c"
    break;

  case 88: /* stmt: if_stmt  */
#line 573 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3264 "raku.tab.c"
    break;

  case 89: /* stmt: while_stmt  */
#line 574 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3270 "raku.tab.c"
    break;

  case 90: /* stmt: for_stmt  */
#line 575 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3276 "raku.tab.c"
    break;

  case 91: /* stmt: given_stmt  */
#line 576 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3282 "raku.tab.c"
    break;

  case 92: /* stmt: KW_TRY block  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3288 "raku.tab.c"
    break;

  case 93: /* stmt: KW_TRY block KW_CATCH block  */
#line 580 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3294 "raku.tab.c"
    break;

  case 94: /* stmt: KW_CATCH block  */
#line 582 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3300 "raku.tab.c"
    break;

  case 95: /* stmt: block  */
#line 584 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3306 "raku.tab.c"
    break;

  case 96: /* stmt: unless_stmt  */
#line 585 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3312 "raku.tab.c"
    break;

  case 97: /* stmt: until_stmt  */
#line 586 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3318 "raku.tab.c"
    break;

  case 98: /* stmt: repeat_stmt  */
#line 587 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3324 "raku.tab.c"
    break;

  case 99: /* stmt: loop_stmt  */
#line 588 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3330 "raku.tab.c"
    break;

  case 100: /* stmt: KW_LAST ';'  */
#line 589 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3336 "raku.tab.c"
    break;

  case 101: /* stmt: KW_NEXT ';'  */
#line 590 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3342 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST KW_IF expr ';'  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3348 "raku.tab.c"
    break;

  case 103: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3354 "raku.tab.c"
    break;

  case 104: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 596 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3360 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 598 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3366 "raku.tab.c"
    break;

  case 106: /* stmt: sub_decl  */
#line 599 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3372 "raku.tab.c"
    break;

  case 107: /* stmt: class_decl  */
#line 600 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3378 "raku.tab.c"
    break;

  case 108: /* stmt: role_decl  */
#line 601 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3384 "raku.tab.c"
    break;

  case 109: /* stmt: grammar_decl  */
#line 602 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3390 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF '(' expr ')' block  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3396 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3402 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3408 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF expr block  */
#line 612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3414 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 614 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3420 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 616 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3426 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block elsif_tail  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3432 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 620 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3438 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF expr block  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 119: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3450 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3456 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 630 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3462 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 632 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3468 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 634 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3474 "raku.tab.c"
    break;

  case 124: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 638 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3480 "raku.tab.c"
    break;

  case 125: /* while_stmt: KW_WHILE expr block  */
#line 640 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3486 "raku.tab.c"
    break;

  case 126: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3492 "raku.tab.c"
    break;

  case 127: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3498 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS expr block  */
#line 648 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3504 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 650 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3510 "raku.tab.c"
    break;

  case 130: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3516 "raku.tab.c"
    break;

  case 131: /* until_stmt: KW_UNTIL expr block  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3522 "raku.tab.c"
    break;

  case 132: /* repeat_stmt: KW_REPEAT block  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3528 "raku.tab.c"
    break;

  case 133: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 662 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3534 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3540 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 668 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3546 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 670 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3552 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 672 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3558 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 675 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3564 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 679 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3574 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 685 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3584 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 691 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3593 "raku.tab.c"
    break;

  case 142: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 696 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3603 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 702 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3612 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR expr block  */
#line 707 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3619 "raku.tab.c"
    break;

  case 145: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 712 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3631 "raku.tab.c"
    break;

  case 146: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 720 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3644 "raku.tab.c"
    break;

  case 147: /* when_list: %empty  */
#line 730 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3650 "raku.tab.c"
    break;

  case 148: /* when_list: when_list KW_WHEN expr block  */
#line 732 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3658 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 738 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 746 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3680 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_SUB IDENT sub_body  */
#line 752 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3690 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 758 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3702 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 766 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 772 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3722 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 778 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3735 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 787 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 794 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3759 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 803 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3772 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 812 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3783 "raku.tab.c"
    break;

  case 160: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 821 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3794 "raku.tab.c"
    break;

  case 161: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 828 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3803 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list '}'  */
#line 834 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3809 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list expr '}'  */
#line 836 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3816 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 839 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3823 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 842 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3829 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 844 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3835 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 846 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3841 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3847 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 850 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3853 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 852 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3859 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 854 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3865 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 856 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3872 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 859 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3878 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 861 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3884 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 863 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3890 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 865 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3897 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 868 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3904 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 871 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3912 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list '}'  */
#line 876 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3918 "raku.tab.c"
    break;

  case 180: /* method_body: '{' YADA '}'  */
#line 877 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3924 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list expr '}'  */
#line 879 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3931 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 882 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3938 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 885 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3944 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 887 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3950 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 889 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3956 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3962 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 893 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3968 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 895 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3974 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 897 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3980 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 899 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3987 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 902 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3993 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 904 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3999 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 906 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4005 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 908 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4012 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 911 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4019 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 914 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4027 "raku.tab.c"
    break;

  case 197: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 920 "raku.y"
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
#line 4045 "raku.tab.c"
    break;

  case 198: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 936 "raku.y"
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
#line 4062 "raku.tab.c"
    break;

  case 199: /* is_clauses: %empty  */
#line 950 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4068 "raku.tab.c"
    break;

  case 200: /* is_clauses: is_clauses IDENT IDENT  */
#line 952 "raku.y"
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
#line 4084 "raku.tab.c"
    break;

  case 201: /* is_clauses: is_clauses TESTOP IDENT  */
#line 964 "raku.y"
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
#line 4100 "raku.tab.c"
    break;

  case 202: /* class_body_list: %empty  */
#line 977 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4106 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 979 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4113 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 982 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4120 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 985 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4127 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 988 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4134 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 991 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4141 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 994 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4148 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 997 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4155 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1000 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4162 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1003 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4169 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1006 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4176 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1009 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4183 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1012 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4190 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1015 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4201 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1022 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4212 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1029 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4223 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1036 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4234 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1043 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4242 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1047 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4250 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1051 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4258 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1055 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4266 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1059 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4277 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1066 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4288 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1073 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4299 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1080 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4310 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1087 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4324 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1097 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4336 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1105 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4348 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1113 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4361 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1122 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4372 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1129 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4383 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1136 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4397 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1146 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4409 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1154 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4421 "raku.tab.c"
    break;

  case 236: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1164 "raku.y"
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
#line 4438 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: %empty  */
#line 1178 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4444 "raku.tab.c"
    break;

  case 238: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1180 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4453 "raku.tab.c"
    break;

  case 239: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1185 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4462 "raku.tab.c"
    break;

  case 240: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1190 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4471 "raku.tab.c"
    break;

  case 241: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1197 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4479 "raku.tab.c"
    break;

  case 242: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1201 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4487 "raku.tab.c"
    break;

  case 243: /* pair_list: IDENT OP_FATARROW expr  */
#line 1207 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4493 "raku.tab.c"
    break;

  case 244: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1209 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4499 "raku.tab.c"
    break;

  case 245: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1211 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4505 "raku.tab.c"
    break;

  case 246: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1213 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4511 "raku.tab.c"
    break;

  case 247: /* param_list: VAR_SCALAR  */
#line 1216 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4517 "raku.tab.c"
    break;

  case 248: /* param_list: IDENT VAR_SCALAR  */
#line 1217 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4523 "raku.tab.c"
    break;

  case 249: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1218 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4529 "raku.tab.c"
    break;

  case 250: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1219 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4535 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' VAR_SCALAR  */
#line 1220 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4541 "raku.tab.c"
    break;

  case 252: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1221 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4547 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1222 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4553 "raku.tab.c"
    break;

  case 254: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1223 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4559 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list '}'  */
#line 1226 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4565 "raku.tab.c"
    break;

  case 256: /* block: '{' YADA '}'  */
#line 1227 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr '}'  */
#line 1229 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4577 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1231 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1233 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4589 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1235 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4595 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1237 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4601 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1239 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1242 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4614 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1244 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4620 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1246 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4626 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1248 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4632 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1250 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1253 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4646 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1256 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4654 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1260 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4661 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1263 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4668 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1266 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4675 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1269 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4681 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1271 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4689 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1275 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4696 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1278 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4703 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1281 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4710 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1284 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4717 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1287 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4724 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1290 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4730 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1292 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4736 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1294 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4742 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST '}'  */
#line 1296 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4748 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1298 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4754 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1300 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4760 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1302 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4766 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1304 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4772 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1306 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4778 "raku.tab.c"
    break;

  case 289: /* closure: '{' expr '}'  */
#line 1309 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4784 "raku.tab.c"
    break;

  case 290: /* expr: VAR_SCALAR '=' expr  */
#line 1312 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4790 "raku.tab.c"
    break;

  case 291: /* expr: KW_GATHER block  */
#line 1313 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4800 "raku.tab.c"
    break;

  case 292: /* expr: tern_expr  */
#line 1318 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4806 "raku.tab.c"
    break;

  case 293: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1322 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4812 "raku.tab.c"
    break;

  case 294: /* tern_expr: cmp_expr  */
#line 1323 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4818 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1326 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4824 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1327 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4830 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1328 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4836 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1329 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4842 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1330 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4848 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1331 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4854 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1332 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4860 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1333 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4866 "raku.tab.c"
    break;

  case 303: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1334 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4872 "raku.tab.c"
    break;

  case 304: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1335 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4878 "raku.tab.c"
    break;

  case 305: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1336 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4884 "raku.tab.c"
    break;

  case 306: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1337 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4890 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1338 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4896 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1339 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4902 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1341 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4912 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1347 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4922 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1353 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4932 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr  */
#line 1358 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 4938 "raku.tab.c"
    break;

  case 313: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1361 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4944 "raku.tab.c"
    break;

  case 314: /* divis_expr: jct_expr  */
#line 1362 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4950 "raku.tab.c"
    break;

  case 315: /* jct_expr: jct_expr '|' range_expr  */
#line 1365 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4956 "raku.tab.c"
    break;

  case 316: /* jct_expr: jct_expr '&' range_expr  */
#line 1366 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4962 "raku.tab.c"
    break;

  case 317: /* jct_expr: dor_expr  */
#line 1367 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4968 "raku.tab.c"
    break;

  case 318: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1371 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4974 "raku.tab.c"
    break;

  case 319: /* dor_expr: range_expr  */
#line 1372 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4980 "raku.tab.c"
    break;

  case 320: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1375 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4986 "raku.tab.c"
    break;

  case 321: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1376 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4992 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr  */
#line 1377 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4998 "raku.tab.c"
    break;

  case 323: /* add_expr: add_expr '~' repl_expr  */
#line 1380 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5004 "raku.tab.c"
    break;

  case 324: /* add_expr: repl_expr  */
#line 1381 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5010 "raku.tab.c"
    break;

  case 325: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1384 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5016 "raku.tab.c"
    break;

  case 326: /* repl_expr: addsub_expr  */
#line 1385 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5022 "raku.tab.c"
    break;

  case 327: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1388 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5028 "raku.tab.c"
    break;

  case 328: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1389 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5034 "raku.tab.c"
    break;

  case 329: /* addsub_expr: mul_expr  */
#line 1390 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5040 "raku.tab.c"
    break;

  case 330: /* mul_expr: mul_expr '*' unary_expr  */
#line 1393 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5046 "raku.tab.c"
    break;

  case 331: /* mul_expr: mul_expr '/' unary_expr  */
#line 1394 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5052 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '%' unary_expr  */
#line 1395 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5058 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1396 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5064 "raku.tab.c"
    break;

  case 334: /* mul_expr: unary_expr  */
#line 1397 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5070 "raku.tab.c"
    break;

  case 335: /* unary_expr: '-' unary_expr  */
#line 1400 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5076 "raku.tab.c"
    break;

  case 336: /* unary_expr: '!' unary_expr  */
#line 1401 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5082 "raku.tab.c"
    break;

  case 337: /* unary_expr: CARET unary_expr  */
#line 1402 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5088 "raku.tab.c"
    break;

  case 338: /* unary_expr: pow_expr  */
#line 1403 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5094 "raku.tab.c"
    break;

  case 339: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1406 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5100 "raku.tab.c"
    break;

  case 340: /* pow_expr: postfix_expr  */
#line 1407 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5106 "raku.tab.c"
    break;

  case 341: /* scalar_list: VAR_SCALAR  */
#line 1410 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5112 "raku.tab.c"
    break;

  case 342: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1411 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5118 "raku.tab.c"
    break;

  case 343: /* meth_name: IDENT  */
#line 1414 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5124 "raku.tab.c"
    break;

  case 344: /* meth_name: KW_SORT  */
#line 1415 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5130 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_MAP  */
#line 1416 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5136 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_GREP  */
#line 1417 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5142 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_SAY  */
#line 1418 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5148 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_PRINT  */
#line 1419 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5154 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_TAKE  */
#line 1420 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5160 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_RETURN  */
#line 1421 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5166 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_EXISTS  */
#line 1422 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5172 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_DELETE  */
#line 1423 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5178 "raku.tab.c"
    break;

  case 353: /* meth_name: TESTOP  */
#line 1424 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5184 "raku.tab.c"
    break;

  case 354: /* postfix_expr: call_expr  */
#line 1426 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5190 "raku.tab.c"
    break;

  case 355: /* call_expr: IDENT '(' arg_list ')'  */
#line 1429 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5199 "raku.tab.c"
    break;

  case 356: /* call_expr: IDENT '(' ')'  */
#line 1433 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5205 "raku.tab.c"
    break;

  case 357: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1435 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5214 "raku.tab.c"
    break;

  case 358: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1440 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5220 "raku.tab.c"
    break;

  case 359: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1442 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5230 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1448 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5240 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1454 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5248 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' KW_NEW  */
#line 1458 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5256 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' IDENT  */
#line 1462 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5265 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1467 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5276 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1474 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5285 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' CARET IDENT  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5295 "raku.tab.c"
    break;

  case 367: /* call_expr: atom '.' CARET IDENT  */
#line 1485 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5305 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1491 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5316 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1498 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5327 "raku.tab.c"
    break;

  case 370: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1505 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5336 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' meth_name  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5345 "raku.tab.c"
    break;

  case 372: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1515 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5356 "raku.tab.c"
    break;

  case 373: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1522 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5365 "raku.tab.c"
    break;

  case 374: /* call_expr: call_expr '.' meth_name  */
#line 1527 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5374 "raku.tab.c"
    break;

  case 375: /* call_expr: KW_DIE expr  */
#line 1532 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5380 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_MAP closure expr  */
#line 1534 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5386 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_GREP closure expr  */
#line 1536 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5392 "raku.tab.c"
    break;

  case 378: /* call_expr: KW_SORT expr  */
#line 1538 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5398 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_SORT closure expr  */
#line 1540 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5404 "raku.tab.c"
    break;

  case 380: /* call_expr: atom  */
#line 1541 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5410 "raku.tab.c"
    break;

  case 381: /* arg_list: expr  */
#line 1544 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5416 "raku.tab.c"
    break;

  case 382: /* arg_list: arg_list ',' expr  */
#line 1545 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5422 "raku.tab.c"
    break;

  case 383: /* atom: LIT_INT  */
#line 1548 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5428 "raku.tab.c"
    break;

  case 384: /* atom: LIT_FLOAT  */
#line 1549 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5434 "raku.tab.c"
    break;

  case 385: /* atom: LIT_STR  */
#line 1550 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5440 "raku.tab.c"
    break;

  case 386: /* atom: WORDLIST  */
#line 1552 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5452 "raku.tab.c"
    break;

  case 387: /* atom: LIT_INTERP_STR  */
#line 1559 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5458 "raku.tab.c"
    break;

  case 388: /* atom: VAR_SCALAR  */
#line 1560 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5464 "raku.tab.c"
    break;

  case 389: /* atom: OP_INC VAR_SCALAR  */
#line 1561 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5470 "raku.tab.c"
    break;

  case 390: /* atom: OP_DEC VAR_SCALAR  */
#line 1562 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5476 "raku.tab.c"
    break;

  case 391: /* atom: VAR_SCALAR OP_INC  */
#line 1563 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5482 "raku.tab.c"
    break;

  case 392: /* atom: VAR_SCALAR OP_DEC  */
#line 1564 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5488 "raku.tab.c"
    break;

  case 393: /* atom: VAR_ARRAY  */
#line 1565 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5494 "raku.tab.c"
    break;

  case 394: /* atom: VAR_HASH  */
#line 1566 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5500 "raku.tab.c"
    break;

  case 395: /* atom: VAR_CAPTURE  */
#line 1568 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5508 "raku.tab.c"
    break;

  case 396: /* atom: VAR_FH  */
#line 1572 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5516 "raku.tab.c"
    break;

  case 397: /* atom: VAR_NAMED_CAPTURE  */
#line 1576 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5523 "raku.tab.c"
    break;

  case 398: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1579 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5529 "raku.tab.c"
    break;

  case 399: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1581 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5535 "raku.tab.c"
    break;

  case 400: /* atom: VAR_HASH '{' expr '}'  */
#line 1583 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5541 "raku.tab.c"
    break;

  case 401: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1585 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5547 "raku.tab.c"
    break;

  case 402: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1587 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5553 "raku.tab.c"
    break;

  case 403: /* atom: IDENT  */
#line 1588 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5559 "raku.tab.c"
    break;

  case 404: /* atom: VAR_TWIGIL  */
#line 1590 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5567 "raku.tab.c"
    break;

  case 405: /* atom: VAR_ARRAY_TWIGIL  */
#line 1594 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5575 "raku.tab.c"
    break;

  case 406: /* atom: VAR_HASH_TWIGIL  */
#line 1598 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5583 "raku.tab.c"
    break;

  case 407: /* atom: '(' ')'  */
#line 1601 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5589 "raku.tab.c"
    break;

  case 408: /* atom: '(' expr ')'  */
#line 1602 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5595 "raku.tab.c"
    break;

  case 409: /* atom: '(' expr ',' ')'  */
#line 1604 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5601 "raku.tab.c"
    break;

  case 410: /* atom: '(' expr ',' arg_list ')'  */
#line 1606 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5608 "raku.tab.c"
    break;

  case 411: /* atom: block  */
#line 1608 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5614 "raku.tab.c"
    break;

  case 412: /* atom: KW_SUB block  */
#line 1609 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5620 "raku.tab.c"
    break;


#line 5624 "raku.tab.c"

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

#line 1611 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
