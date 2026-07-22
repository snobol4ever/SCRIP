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
#define YYLAST   3643

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  410
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1046

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
       0,   347,   347,   358,   359,   362,   364,   366,   368,   372,
     374,   376,   378,   382,   385,   389,   391,   393,   395,   397,
     399,   403,   407,   409,   411,   413,   415,   417,   419,   421,
     423,   425,   427,   429,   431,   433,   435,   438,   441,   443,
     446,   449,   451,   453,   455,   457,   462,   466,   470,   475,
     479,   480,   482,   484,   486,   488,   491,   493,   495,   497,
     500,   503,   506,   509,   512,   514,   516,   518,   520,   523,
     525,   527,   529,   532,   535,   539,   542,   544,   546,   548,
     551,   554,   557,   560,   563,   566,   569,   570,   571,   572,
     573,   574,   575,   577,   579,   581,   583,   584,   585,   586,
     587,   588,   589,   591,   593,   595,   597,   598,   599,   600,
     603,   605,   607,   609,   611,   613,   615,   617,   621,   623,
     625,   627,   629,   631,   635,   637,   641,   643,   645,   647,
     651,   653,   657,   659,   661,   665,   667,   669,   673,   676,
     682,   688,   693,   699,   704,   709,   717,   728,   729,   735,
     743,   749,   755,   763,   769,   775,   784,   791,   800,   809,
     818,   825,   832,   833,   836,   839,   841,   843,   845,   847,
     849,   851,   853,   856,   858,   860,   862,   865,   868,   874,
     875,   876,   879,   882,   884,   886,   888,   890,   892,   894,
     896,   899,   901,   903,   905,   908,   911,   917,   933,   948,
     949,   961,   975,   976,   979,   982,   985,   988,   991,   994,
     997,  1000,  1003,  1006,  1009,  1012,  1019,  1026,  1033,  1040,
    1044,  1048,  1052,  1056,  1063,  1070,  1077,  1084,  1094,  1102,
    1110,  1119,  1126,  1133,  1143,  1151,  1161,  1176,  1177,  1182,
    1187,  1194,  1198,  1204,  1206,  1208,  1210,  1214,  1215,  1216,
    1217,  1218,  1219,  1220,  1221,  1224,  1225,  1226,  1228,  1230,
    1232,  1234,  1236,  1239,  1241,  1243,  1245,  1247,  1250,  1253,
    1257,  1260,  1263,  1266,  1268,  1272,  1275,  1278,  1281,  1284,
    1287,  1289,  1291,  1293,  1295,  1297,  1299,  1301,  1303,  1307,
    1310,  1311,  1316,  1319,  1321,  1324,  1325,  1326,  1327,  1328,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1337,  1338,
    1344,  1350,  1356,  1359,  1360,  1361,  1364,  1366,  1369,  1370,
    1371,  1374,  1375,  1378,  1379,  1382,  1383,  1384,  1387,  1388,
    1389,  1390,  1391,  1394,  1395,  1396,  1397,  1400,  1401,  1404,
    1405,  1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,
    1417,  1418,  1420,  1422,  1427,  1428,  1433,  1435,  1441,  1447,
    1451,  1455,  1460,  1467,  1472,  1478,  1484,  1491,  1498,  1503,
    1508,  1515,  1520,  1525,  1527,  1529,  1531,  1533,  1535,  1538,
    1539,  1542,  1543,  1544,  1545,  1553,  1554,  1555,  1556,  1557,
    1558,  1559,  1560,  1561,  1565,  1569,  1572,  1574,  1576,  1578,
    1580,  1582,  1583,  1587,  1591,  1595,  1596,  1597,  1599,  1602,
    1603
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

#define YYPACT_NINF (-753)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-410)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -753,    15,  1519,  -753,  -753,  -753,  -753,  -753,   912,   -39,
     109,    95,    88,  -753,  -753,   858,  -753,  -753,  -753,   230,
    1586,    46,  2738,  2805,  2872,  2939,  3006,     2,   121,  3006,
    1653,    57,  3006,   275,   288,  3073,  3140,   121,   174,    35,
      59,   131,   131,  3207,   121,   121,  3006,   341,   351,    68,
    -753,   384,   306,   322,   858,   858,  -753,  1720,   277,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,   595,
    -753,  -753,  -753,  3525,   779,  -753,   881,   428,   308,  -753,
      37,   310,   154,   246,  -753,  -753,   328,   409,   434,   420,
    -753,  -753,  3006,  3006,  3006,  3006,  3006,  3006,   451,  1788,
    3006,   553,  3006,  3006,    12,  1856,   135,   323,   118,  -753,
     121,  -753,  -753,   460,   137,  -753,  1924,  -753,   193,   -60,
     -32,   186,   491,   575,   102,   600,  1720,   692,  1720,    53,
    1720,   121,  1720,   121,   -36,    60,   468,  -753,  -753,   513,
    -753,   523,   539,   571,   573,   140,   143,  1720,   121,  1720,
     121,   216,  1992,  -753,  3006,  3006,  -753,  3006,  3006,  -753,
    3006,  3006,  3006,  2059,  3006,  -753,   605,  -753,  -753,  -753,
     584,   580,    55,   590,  -753,  -753,  -753,  -753,  -753,    19,
     576,  1050,  3006,  3006,  3006,  3006,  3006,  3006,  3006,  3006,
    -753,  3006,  3006,  3006,  3006,  3006,  3006,  3006,  3006,  -753,
     858,   858,   858,   858,   858,   858,   858,   858,   858,   858,
     858,   858,   858,   858,   858,   763,   858,   858,   858,   858,
     858,   858,   858,   858,   858,   858,   858,   858,   858,   858,
     896,   803,    50,   604,   608,   615,   621,   623,   634,   178,
    -753,   200,   636,   665,   649,   661,   667,   785,   673,  -753,
     251,  3006,   790,  3006,  -753,  3006,   677,    19,   367,  -753,
    3006,  3006,  -753,  3274,  -753,  3341,  -753,   192,   212,   244,
     494,   705,   711,  -753,   544,   562,  3006,  3006,  3006,  3006,
    3006,  3006,  3006,  -753,  3006,   591,  3006,  3006,  -753,  3006,
     598,   728,   642,  -753,   811,  3006,  -753,   858,   858,    -6,
    -753,  -753,  -753,  -753,  3006,  3006,  -753,   810,  3006,   812,
    3006,   664,   804,   668,  -753,  3006,  3006,   825,   740,   762,
     769,   778,   781,   722,  -753,  -753,  -753,   121,    -3,  -753,
      -1,   735,   791,  -753,  -753,  2126,  -753,  1028,   796,   164,
     809,  2738,  2805,  3006,   365,    25,    92,  -753,   294,   807,
     820,   829,   831,   836,   860,   873,   874,   875,   885,   893,
     894,   899,   900,   901,   903,   279,   279,   279,   279,   279,
     279,   834,   279,   279,   279,   279,   279,   279,   279,   279,
    -753,  -753,  -753,  -753,  -753,  -753,   817,   817,   310,   154,
     246,   246,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,   850,   968,
     904,  -753,  2194,  -753,  -753,  -753,  -753,  -753,  -753,  3006,
    2262,  -753,   918,   923,   924,  -753,  2330,  -753,  2398,  -753,
     907,   925,   908,  -753,  -753,   917,  -753,   919,  1720,   224,
     963,   152,  2466,   234,   920,  3006,  -753,  3408,  -753,  3006,
    -753,     4,  -753,  3006,  3006,   932,  1030,  2126,   922,   926,
     928,   929,   930,   933,   938,   235,  2126,   939,   940,   355,
     121,    14,  3475,  -753,   121,   121,   101,   -28,   -23,  -753,
      43,   952,   670,  1216,   959,   969,    11,   934,   961,   935,
     964,   121,   121,   121,   970,   987,   946,  3006,  -753,  -753,
    -753,  -753,  -753,  -753,  1093,  1094,  -753,     6,   952,   676,
      21,   543,     5,  -753,   681,  1097,  3006,  3006,  3006,   424,
      93,   -13,  -753,    48,  3006,  3006,  -753,  3006,  3006,  -753,
    3006,  3006,  3006,  3006,  3006,  3006,  3006,  3006,  -753,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  -753,   858,  2534,  -753,  2602,   993,
     685,   994,  -753,   708,  3006,  3006,  3006,  -753,   717,   168,
    -753,   719,   721,  -753,  -753,  -753,  -753,  -753,   725,  3006,
    -753,  3006,  3006,  3006,   729,  -753,   196,  -753,   996,  1720,
     399,  1001,   952,   736,  1009,  1010,  3006,  -753,   745,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,   758,  -753,  -753,
    -753,   857,  -753,  -753,  1720,   121,  -753,  -753,  1034,  -753,
    1120,  1121,  -753,  1122,  1124,  -753,   952,   687,  2738,  2805,
     675,  -753,   530,  -753,  -753,  3006,   121,  -753,  -753,  -753,
    1016,  1017,  1110,  -753,  -753,  -753,  -753,  3006,  1019,  -753,
    -753,    47,   891,   647,  1083,  -753,  -753,   952,   952,   765,
     770,  1126,  1127,  1128,  -753,  -753,   187,  1021,  1023,   248,
    3006,  3006,  3006,  3006,  3006,  3006,  -753,  -753,  -753,  -753,
     267,   268,   273,   281,   284,   292,   302,   304,   307,   312,
     316,   318,  -753,  -753,   772,  -753,   774,   780,  -753,  1031,
    -753,  -753,  1032,  1033,  1035,  -753,  3006,  -753,  1133,  -753,
    2126,  1036,   417,  -753,  -753,  1038,  1081,  1086,  -753,   782,
    -753,  3006,  -753,  -753,   952,  -753,  -753,   533,  1040,  1041,
      14,  -753,   813,   916,   121,   121,   121,  -753,  -753,  -753,
    -753,   155,   577,   226,  -753,   329,  3006,  3006,  3006,  3006,
    3006,  3006,  3006,  3006,  -753,   121,  1039,  -753,  -753,   121,
    1042,  3006,  -753,   616,   618,    58,    83,   695,  1043,  1045,
     896,  -753,  -753,   952,   952,  1159,  1160,  1161,  3006,  2670,
    1066,  1067,  -753,   330,   342,   343,   347,   356,   357,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  -753,  1101,
     832,  -753,  -753,  -753,  3006,  3006,  2126,   545,  -753,  -753,
    3006,  -753,  -753,  -753,  -753,   121,   121,  -753,  -753,  -753,
    -753,  -753,  1164,  1173,  3006,  3006,  3006,  -753,  -753,  -753,
     360,   361,   373,   374,   378,   379,   398,   421,  -753,  -753,
    -753,  3006,  1071,  -753,    22,  1129,  -753,    28,  -753,  1179,
    1180,  1076,  3006,  -753,  1183,  1184,  1080,  3006,  -753,   185,
     191,  1082,  1084,  -753,  -753,   632,  -753,  -753,  -753,  -753,
    -753,   423,  1074,   844,  3006,  3006,  -753,  -753,  -753,  -753,
    -753,  -753,  3006,  1085,  -753,  -753,   851,  -753,   555,   947,
    -753,  -753,  -753,   429,   432,   435,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  1088,   121,  1077,   855,  1079,  1334,
    1077,   859,  1089,  1090,  -753,  1092,  1095,  1096,  -753,  1098,
    1198,  1199,  1099,  3006,  -753,  1202,  1209,  1105,  3006,  -753,
    -753,  -753,    36,  -753,  -753,  -753,  1117,   446,   449,  -753,
    -753,  1106,  -753,   121,  -753,  -753,  -753,  -753,  3006,  -753,
    -753,  1077,  -753,  2738,  2805,  1401,  -753,   556,  -753,  1077,
    -753,  -753,  -753,  -753,  -753,  -753,  1134,  1135,  -753,  1136,
    1137,  1153,  -753,  1154,  1077,   863,  -753,  -753,  -753,  -753,
    -753,  1156,  -753,   611,   283,  -753,   452,  3006,  3006,  3006,
    3006,  3006,  3006,  3006,  3006,  -753,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  1077,   121,  3006,  3006,  3006,  -753,
    -753,  -753,   457,   465,   471,   479,   482,   483,   485,   496,
    -753,  -753,   504,   517,   527,  -753,  -753,  -753,  -753,  -753,
    -753,  -753,  -753,  -753,  -753,  -753
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   381,   382,   383,   385,   386,   391,
     392,   402,   401,   403,   404,     0,   393,   394,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     384,     0,     0,     0,     0,     0,    87,     0,     3,     4,
      88,    89,    96,    97,    98,    99,    90,    91,   106,     0,
     107,   108,   109,    95,     0,   292,   294,   312,   315,   317,
     320,   322,   324,   327,   332,   336,   338,   352,   378,     0,
     389,   390,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   386,   391,   392,   402,
       0,   409,   335,     0,   386,    31,     0,   379,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   320,     0,   410,   291,     0,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   132,     0,   135,     0,     0,   100,     0,     0,   101,
       0,     0,     0,     3,     0,   376,    92,    94,   373,   199,
       0,     0,     0,     0,   387,   388,   334,   333,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   290,     0,
     356,     0,     0,     0,     0,     0,   361,     0,   360,   354,
       0,     0,     0,     0,    26,     0,   405,     0,     0,    34,
       0,     0,     6,     0,     9,     0,    10,     0,     0,     0,
       0,     0,     0,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    35,     0,     0,     0,     0,    38,     0,
       0,   113,     0,   125,     0,     0,   144,     0,     0,     0,
       3,   151,    41,    42,     0,     0,   147,     0,     0,     0,
       0,     0,   128,     0,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   374,   375,   377,     0,     0,   202,
       0,     0,     0,   237,   406,     0,   256,   386,   391,   392,
     402,     0,     0,     0,     0,     0,     0,   255,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   297,   298,   301,   302,   295,
     296,     0,   299,   300,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   313,   314,   316,   318,   319,   321,   323,
     325,   326,   331,   328,   329,   330,   337,   341,   351,   345,
     346,   347,   348,   349,   350,   343,   344,   342,   372,     0,
     369,    47,     0,    81,    82,    83,    84,    85,    44,     0,
       0,   355,   396,   397,   398,    49,     0,   364,     0,   353,
       0,     0,     0,   290,    33,     0,   380,     0,     0,     0,
     383,   401,     0,     0,     0,     0,    23,     0,    24,     0,
      25,     0,   154,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,     0,     0,   116,   406,     0,     0,   318,   319,   247,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,     0,   406,     0,     0,     0,     0,   102,   103,
     104,   105,   289,    93,     0,     0,   202,     0,     0,     0,
       0,     0,     0,   407,     0,     0,     0,     0,     0,     0,
       0,     0,   282,     0,     0,     0,   283,     0,     0,   284,
       0,     0,     0,     0,     0,     0,     0,     0,   257,    51,
      53,    55,    58,    56,    57,    52,    54,    64,    66,    68,
      71,    69,    70,    65,    67,     0,     0,   365,     0,     0,
       0,     0,   161,     0,     0,     0,     0,   363,     0,   401,
     359,     0,     0,   396,   397,   398,    32,     5,     0,     0,
      11,     0,     0,     0,     0,    16,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   340,     0,    72,
      75,    74,    78,    76,    77,    73,    36,     0,    79,    80,
      39,   110,   115,   114,     0,     0,   124,   141,     0,   143,
       0,     0,   248,     0,     0,   150,     0,     0,     0,     0,
       0,   162,     0,    28,    27,     0,     0,   145,   399,   400,
       0,     0,   126,   129,   130,   133,   134,     0,     0,   200,
     201,     0,     0,     0,     0,   198,   159,     0,     0,     0,
       0,     0,     0,     0,   236,   408,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,   273,   280,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   293,   371,     0,   368,     0,     0,    46,     0,
      48,   160,     0,     0,     0,   362,     0,   357,     0,   358,
       0,     0,     0,   244,   243,     0,     0,     0,    18,     0,
      19,     0,    22,   153,     0,    30,    29,     0,   408,   408,
       0,   117,     0,   118,     0,     0,     0,   249,   250,   149,
     251,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   163,     0,     0,    62,    63,     0,
       0,     0,   197,     0,     0,     0,     0,     0,     0,     0,
       0,   158,   156,     0,     0,     0,     0,     0,     0,     0,
     396,   398,   277,     0,     0,     0,     0,     0,     0,   285,
     286,   287,   288,   258,   260,   262,   265,   263,   264,   259,
     261,   370,   367,   366,    45,    59,    60,    61,   241,     0,
       0,    13,    12,    17,     0,     0,     0,     0,   152,     7,
       0,    37,    40,   112,   111,   406,     0,   122,   142,   139,
     140,   252,     0,     0,     0,     0,     0,   166,   167,   164,
       0,     0,     0,     0,     0,     0,     0,     0,   148,   146,
     127,     0,     0,   138,     0,     3,   232,     0,   229,     0,
       0,     0,     0,   208,     0,     0,     0,     0,   203,     0,
       0,     0,     0,   204,   205,     0,   155,   157,   238,   239,
     240,     0,   161,     0,     0,     0,   267,   269,   272,   270,
     271,   268,     0,   408,   246,   245,     0,    20,     0,   119,
     120,   253,   254,     0,     0,     0,   168,   170,   172,   175,
     173,   174,   169,   171,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   219,     0,     0,     0,   220,     0,
       0,     0,     0,     0,   210,     0,     0,     0,     0,   209,
     206,   207,     0,   235,   276,   275,   160,     0,     0,   242,
      14,   408,     8,     0,   123,   176,   178,   177,     0,   137,
     231,     0,   180,     0,     0,     0,   179,     0,   228,     0,
     217,   218,   212,   215,   216,   211,     0,     0,   221,     0,
       0,     0,   222,     0,     0,     0,   274,   278,   279,    21,
     121,     0,   230,     0,     0,   183,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,   227,   225,   226,   214,
     223,   224,   213,   234,     0,     0,     0,     0,     0,   184,
     185,   182,     0,     0,     0,     0,     0,     0,     0,     0,
     233,   136,     0,     0,     0,   186,   188,   190,   193,   191,
     192,   187,   189,   194,   196,   195
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -753,  -753,     1,  -753,  -466,  -604,  -753,  -753,  -753,  -753,
    -753,   320,  -753,  -753,  -753,  -753,  -753,  -267,  -752,  -753,
    -753,  -753,   768,  -753,  -753,   723,   837,  -313,   291,   944,
      -2,  -204,  -753,   977,  -753,   561,   -16,  1061,  1063,   767,
      10,  -753,  -753,  -225,  -753,  -753,   -62,  -753
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   181,    59,    60,   473,    61,    62,    63,    64,
      65,   852,    66,    67,   486,    68,    69,   301,   856,    70,
      71,   328,   507,    72,   512,   571,   443,   482,   111,   161,
     117,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,   274,   408,    86,    87,   118,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      74,     2,   371,   452,   479,   612,   410,   731,   480,   479,
     135,   504,   858,   480,   479,     3,   136,   509,   480,   505,
     127,   129,   131,   133,   134,   112,   246,   139,   141,   247,
     144,   479,   479,   148,   150,   480,   480,   241,   479,   294,
      24,   165,   480,   250,   168,   261,   479,   620,   635,   636,
     480,   524,   621,   622,   258,   179,   119,   120,   121,   262,
     122,   154,   652,   653,   176,   177,   654,   142,   525,   331,
     248,   143,   859,   263,   661,   662,   663,   123,   155,   286,
     860,   124,   171,   221,   100,   157,   295,   264,   221,    58,
     233,   234,   235,   236,   237,   238,   287,   864,   242,   172,
     244,   245,   158,   652,   653,   865,   302,   654,   623,   624,
     219,   220,   271,   678,   257,   481,   272,   332,   527,   286,
     508,   861,   506,   943,   275,   592,   285,    58,   290,   827,
     292,   664,   655,   297,   298,   528,   287,   637,   593,    58,
     334,   335,   658,   916,   156,   311,   866,   313,   221,   920,
     318,   526,   319,   320,   156,   321,   322,   984,   323,   324,
     325,   323,   326,   862,   960,   831,   125,   303,   968,   411,
     412,   221,   288,   762,   679,   289,   618,   863,   159,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   867,   357,
     358,   359,   360,   361,   362,   363,   364,   659,   660,   930,
     103,   716,   868,   386,   387,   935,   104,   931,   105,   992,
     717,   159,   288,   936,   625,   289,   101,  1006,   529,   677,
     832,   833,   465,   260,   583,   252,    58,   469,    90,    91,
      90,    91,  1013,   476,   102,   392,   393,   394,   395,   396,
     706,   656,   255,   253,   113,   315,    58,   307,   932,   430,
     309,   432,   286,   433,   937,    99,   160,    99,   436,   437,
     316,   439,  1030,   444,   823,   308,   223,   224,   310,   287,
     104,   101,   105,   514,   458,   459,   460,   461,   462,   463,
     464,   477,   478,   419,   467,   468,   104,   145,   105,   517,
     933,   265,   778,    73,   152,   954,   938,   445,   420,    58,
     146,   483,   484,   485,   934,   266,   488,   779,   490,   286,
     939,   446,   259,   494,   495,   260,   174,   447,   137,   138,
     530,   421,   260,   531,   532,   723,   287,   579,   151,   153,
     533,   448,   175,   534,   535,   166,   167,   536,   537,   519,
     520,   521,   523,   580,   180,   288,   581,   225,   289,   449,
     560,   692,   838,   585,   606,   169,   586,   260,   563,   739,
     226,   227,   228,   450,   568,   170,   572,   425,     4,     5,
       6,     7,   429,   260,   782,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,   498,   499,   216,   217,
     771,   772,   500,   789,   790,   598,   110,    28,   173,   791,
     501,   137,   288,   547,   607,   289,    33,   792,   218,  1020,
     793,   548,   222,   199,    41,    42,    43,   561,   794,    46,
     538,   549,   291,   550,   293,   296,   551,   764,   795,    50,
     796,   552,   229,   797,   232,   553,   578,   554,   798,   312,
     179,   314,   799,   588,   800,   590,   251,   591,   303,   599,
     670,   594,   595,   277,   671,   839,   886,   818,    52,    53,
     672,   601,   602,   673,   674,   239,   603,   675,   887,   888,
     615,    54,    73,   889,   610,   604,   605,   260,    55,   547,
     548,   632,   890,   891,   140,    57,   906,   907,   435,   260,
      58,   522,   549,   550,   694,   648,   697,   551,   552,   908,
     909,   267,   268,   269,   910,   911,   876,   877,   209,   210,
     211,   212,   213,   214,   667,   668,   669,   553,   720,   712,
     215,   721,   680,   681,   912,   682,   683,   230,   684,   685,
     686,   687,   688,   689,   690,   691,   812,   216,   217,   260,
     554,   917,   700,   283,   921,   875,   284,   913,   599,   944,
     676,   601,   231,   479,   605,   955,   746,   480,   956,   747,
     748,   957,   702,   703,   704,   805,   749,   243,   807,   750,
     751,   303,   987,   752,   753,   988,   547,   711,  1021,   254,
     713,   714,   997,  1035,   548,   998,   999,   719,   299,   270,
     549,  1036,  1000,   300,   727,  1001,  1002,  1037,   550,  1003,
    1004,   551,   552,   834,   553,  1038,   277,   835,  1039,  1040,
     273,  1041,   732,   279,   451,   554,   280,   281,   503,   300,
     836,   182,  1042,   599,   183,   184,   742,   743,   745,   985,
    1043,   185,   302,   755,   186,   187,   601,  1016,   188,   189,
     277,  1017,   303,  1044,   304,   760,   605,   279,   810,   199,
     280,   281,   819,  1045,  1018,   820,   754,   765,   327,   817,
     766,   767,   768,   769,   897,   455,   456,   260,   783,   784,
     785,   786,   787,   788,   952,   199,   305,   260,     4,     5,
       6,     7,  1005,   334,   457,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,   283,   740,   306,   284,
     330,   741,   336,   837,   808,   869,   110,    28,   870,   329,
     871,   872,   334,   466,   190,   333,    33,   883,   276,   470,
     335,   277,   278,   413,    41,    42,    43,   414,   279,    46,
     283,   280,   281,   284,   415,   282,   854,  1019,   857,    50,
     416,   855,   417,   855,   840,   841,   842,   843,   844,   845,
     846,   847,   942,   418,   896,   471,   472,   855,   898,   853,
     422,   611,   613,   474,   335,   616,   617,   619,    52,    53,
     380,   381,   382,   423,    73,   424,   881,   383,   384,   385,
     425,    54,   642,   643,   644,   491,   335,   426,    55,   493,
     335,   626,   627,   428,   140,    57,   434,   657,   627,   427,
      58,   744,   665,   260,   431,   191,   699,   260,   192,   193,
     453,   283,   894,   895,   284,   194,   454,   397,   195,   196,
     409,   475,   197,   198,   487,   398,   489,   399,   400,   701,
     260,   492,   903,   904,   905,   496,   401,   402,   705,   260,
     707,   708,   709,   260,   403,   404,   334,   710,   502,   914,
     715,   586,   405,   406,   407,   510,   919,   724,   627,   497,
     925,     4,     5,     6,     7,   929,   728,   260,   106,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,   729,
     260,   498,   947,   948,   730,   472,   773,   627,   499,   110,
     949,   774,   627,   801,   260,   802,   708,   500,   199,    33,
     501,   803,   260,   334,   816,   397,   733,    41,    42,    43,
     397,   511,    46,   398,   518,   399,   400,   967,   398,   516,
     399,   400,    50,   555,   401,   402,   539,   756,   221,   401,
     402,   979,   403,   404,   825,   335,   983,   403,   404,   540,
     405,   406,   407,   826,   472,   405,   406,   407,   541,   763,
     542,    52,    53,   893,   260,   543,   853,   200,   201,   202,
     203,   993,   994,   996,    54,   946,   260,   204,   205,   206,
     556,    55,   951,   260,   953,   472,   961,   627,    57,   544,
     969,   627,   557,    58,  1014,   627,   162,   164,   207,   208,
     390,   391,   545,   546,   547,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,    89,   548,    90,    91,    92,    93,    94,
      95,    96,   549,   550,  1032,  1033,  1034,    97,   551,   552,
     553,   824,   554,   564,   558,   828,   829,   830,   565,   566,
      98,   573,    99,   574,   575,   582,   576,   596,   577,   587,
     597,   599,   638,   640,   734,   600,   848,   601,   602,   603,
     850,   647,   604,     4,     5,     6,     7,   605,   608,   609,
     337,   338,   339,   340,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   341,   342,    24,   300,   633,    25,
      26,    27,    28,   343,   344,    31,    32,   639,   634,   645,
     641,    33,    34,    35,    36,    37,    38,   345,   346,    41,
      42,    43,    44,    45,    46,    47,   646,   649,   650,    48,
      49,   666,   698,   700,    50,   718,   899,   900,    51,    89,
     722,    90,    91,    92,    93,    94,    95,    96,   725,   726,
     735,   736,   737,    97,   738,   757,   758,   759,   761,   770,
     775,   776,   777,    52,    53,   780,   515,   809,    99,   781,
     804,   805,   806,   814,   807,   811,    54,   813,   815,   821,
     822,   851,   873,    55,   874,   849,   878,   879,   880,    56,
      57,   884,   885,   892,   901,    58,   347,   365,   366,   367,
     368,   369,   370,   902,   372,   373,   374,   375,   376,   377,
     378,   379,   915,   922,   923,   924,   918,   926,   927,   928,
     945,   940,   855,   941,   950,   962,   959,   958,   970,   971,
      73,   972,   976,   977,   973,   974,   980,   975,   978,     4,
       5,     6,     7,   981,   982,   989,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     628,   629,    24,   986,   990,    25,    26,    27,    28,    29,
     630,    31,    32,  1007,  1008,  1009,  1010,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,  1011,  1012,   651,    48,    49,  1015,   991,   584,
      50,   696,   388,     0,    51,   389,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1031,     0,     0,    52,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     0,     0,     0,    55,
       0,     0,     0,     0,     0,    56,    57,     4,     5,     6,
       7,    58,   631,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,   963,   964,
      24,     0,     0,    25,    26,    27,    28,    29,   965,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,     0,    50,     0,
       0,     0,    51,     0,     4,     5,     6,     7,     0,     0,
       0,   114,   107,   108,   109,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,    56,    57,    46,     0,     0,     0,    58,
     966,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    54,     0,     0,
       0,     0,     0,     0,    55,     0,     0,     0,     0,     0,
     140,    57,     4,     5,     6,     7,    58,   995,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     0,     0,    25,    26,
      27,    28,    29,    30,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,     0,    50,     0,     0,     0,    51,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,    56,    57,
      46,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    52,
      53,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,   115,   116,    46,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    52,    53,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,    54,
       0,    33,     0,     0,     0,     0,    55,     0,     0,    41,
      42,    43,   140,    57,    46,     0,     0,     0,    58,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   178,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   240,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   249,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,   317,     0,    52,    53,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   256,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    52,    53,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,    54,     0,
      33,     0,     0,     0,     0,    55,     0,     0,    41,    42,
      43,     0,    57,    46,     0,     0,     0,    58,     0,     0,
       0,     0,     0,    50,     0,     0,   180,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,     0,    57,
      46,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   513,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   559,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   562,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,   569,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   567,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     0,     4,
       5,   440,     7,     0,     0,     0,   114,   107,   108,   109,
     441,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    52,    53,     0,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,    54,     0,     0,    33,     0,     0,
       0,    55,     0,     0,     0,    41,    42,    43,    57,   570,
      46,     0,     0,    58,     0,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    52,
      53,     0,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,    54,     0,     0,    33,     0,     0,     0,    55,
       0,     0,     0,    41,    42,    43,    57,   178,    46,     0,
       0,    58,     0,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,   569,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    52,    53,     0,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
      54,     0,     0,    33,     0,     0,     0,    55,     0,     0,
       0,    41,    42,    43,    57,   693,    46,     0,     0,    58,
       0,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    52,    53,     0,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,    54,     0,
       0,    33,     0,     0,     0,    55,     0,     0,     0,    41,
      42,    43,    57,   695,    46,     0,     0,    58,     0,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    52,    53,     0,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,    54,     0,     0,    33,
       0,     0,     0,    55,     0,     0,     0,    41,    42,    43,
      57,   882,    46,     0,     0,    58,     0,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,     0,   126,    46,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   114,   107,   108,   109,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,    52,    53,
       0,     0,     0,   110,    28,     0,     0,     0,     0,     0,
       0,    54,     0,    33,     0,     0,     0,     0,    55,     0,
       0,    41,    42,    43,     0,   128,    46,     0,     0,     0,
      58,     0,     0,     0,     0,     0,    50,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   114,
     107,   108,   109,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,    52,    53,     0,     0,     0,
     110,    28,     0,     0,     0,     0,     0,     0,    54,     0,
      33,     0,     0,     0,     0,    55,     0,     0,    41,    42,
      43,     0,   130,    46,     0,     0,     0,    58,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   114,   107,   108,   109,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,    52,    53,     0,     0,     0,   110,    28,     0,
       0,     0,     0,     0,     0,    54,     0,    33,     0,     0,
       0,     0,    55,     0,     0,    41,    42,    43,     0,   132,
      46,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      50,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   114,   107,   108,   109,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,    52,
      53,     0,     0,     0,   110,    28,     0,     0,     0,     0,
       0,     0,    54,     0,    33,     0,     0,     0,     0,    55,
       0,     0,    41,    42,    43,     0,    57,    46,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    50,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     114,   107,   108,   109,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,    52,    53,     0,     0,
       0,   110,    28,     0,     0,     0,     0,     0,     0,    54,
       0,    33,     0,     0,     0,     0,    55,     0,     0,    41,
      42,    43,     0,   147,    46,     0,     0,     0,    58,     0,
       0,     0,     0,     0,    50,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   114,   107,   108,
     109,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,    52,    53,     0,     0,     0,   110,    28,
       0,     0,     0,     0,     0,     0,    54,     0,    33,     0,
       0,     0,     0,    55,     0,     0,    41,    42,    43,     0,
     149,    46,     0,     0,     0,    58,     0,     0,     0,     0,
       0,    50,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   114,   107,   108,   109,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
      52,    53,     0,     0,     0,   110,    28,     0,     0,     0,
       0,     0,     0,    54,     0,    33,     0,     0,     0,     0,
      55,     0,     0,    41,    42,    43,     0,    57,    46,     0,
       0,     0,   163,     0,     0,     0,     0,     0,    50,     0,
       0,     0,     0,     0,     4,     5,   440,     7,     0,     0,
       0,   114,   107,   108,   109,   441,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,    52,    53,     0,
       0,     0,   110,    28,     0,     0,     0,     0,     0,     0,
      54,     0,    33,     0,     0,     0,     0,    55,     0,     0,
      41,    42,    43,     0,   438,    46,     0,     0,     0,    58,
       0,     0,     0,     0,     0,    50,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   114,   107,
     108,   109,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,    52,    53,     0,     0,     0,   110,
      28,     0,     0,     0,     0,     0,     0,    54,     0,    33,
       0,     0,     0,     0,    55,     0,     0,    41,    42,    43,
       0,   442,    46,     0,     0,     0,    58,     0,     0,     0,
       0,     0,    50,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   114,   107,   108,   109,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,    52,    53,     0,     0,     0,   110,    28,     0,     0,
       0,     0,     0,     0,    54,     0,    33,     0,     0,     0,
       0,    55,     0,     0,    41,    42,    43,     0,   589,    46,
       0,     0,     0,    58,     0,     0,     0,     0,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -409,  -409,     0,     0,    52,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,     0,     0,     0,     0,     0,     0,    55,     0,
       0,     0,     0,     0,     0,   614,     0,     0,  -409,  -409,
      58,  -409,  -409,  -409,  -409,  -409,  -409,  -409,  -409,  -409,
    -409,  -409,  -409,  -409,     0,     0,     0,  -409,     0,     0,
       0,     0,     0,     0,     0,  -409,  -409,  -409,     0,  -409,
       0,     0,  -409,  -409,  -409,  -409,  -409,  -409,     0,  -409,
    -409,  -409,     0,  -409
};

static const yytype_int16 yycheck[] =
{
       2,     0,   206,   270,    10,   471,   231,   611,    14,    10,
      26,    14,   764,    14,    10,     0,    14,   330,    14,    22,
      22,    23,    24,    25,    26,    15,    14,    29,    30,    17,
      32,    10,    10,    35,    36,    14,    14,    99,    10,    75,
      26,    43,    14,   105,    46,   105,    10,    75,    37,    38,
      14,    26,    75,    10,   116,    57,    10,    11,    12,   119,
      14,    26,    56,    57,    54,    55,    60,    10,    43,    14,
      58,    14,    14,   105,    69,    70,    71,    31,    43,    26,
      22,    35,    14,   111,   123,    26,   122,   119,   111,   125,
      92,    93,    94,    95,    96,    97,    43,    14,   100,    31,
     102,   103,    43,    56,    57,    22,   119,    60,    65,    66,
      73,    74,    10,   126,   116,   121,    14,    62,    26,    26,
     121,    63,   125,   875,   126,   121,   128,   125,   130,   733,
     132,   126,   126,    73,    74,    43,    43,   126,   451,   125,
     121,   122,   121,   121,   119,   147,    63,   149,   111,   121,
     152,   126,   154,   155,   119,   157,   158,   121,   160,   161,
     162,   163,   164,   105,   916,    10,   120,   119,   920,   119,
     120,   111,   119,   126,   126,   122,    75,   119,   119,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   105,   191,
     192,   193,   194,   195,   196,   197,   198,   510,   511,    14,
     105,     5,   119,   219,   220,    14,   118,    22,   120,   961,
      14,   119,   119,    22,   481,   122,   107,   969,   126,   126,
      65,    66,   284,   122,    72,   107,   125,   289,    93,    94,
      93,    94,   984,   295,   125,   225,   226,   227,   228,   229,
      72,   508,   105,   125,    14,    29,   125,   107,    63,   251,
     107,   253,    26,   255,    63,   120,   125,   120,   260,   261,
      44,   263,  1014,   265,   730,   125,   112,   113,   125,    43,
     118,   107,   120,   335,   276,   277,   278,   279,   280,   281,
     282,   297,   298,   105,   286,   287,   118,    12,   120,   125,
     105,   105,   105,     2,   120,   899,   105,   105,   120,   125,
      12,   300,   304,   305,   119,   119,   308,   120,   310,    26,
     119,   119,   119,   315,   316,   122,    10,   105,    27,    28,
      26,   121,   122,    29,    30,   592,    43,   103,    37,    38,
      36,   119,    10,    39,    40,    44,    45,    43,    44,   341,
     342,   343,   344,   119,    67,   119,   122,   101,   122,   105,
     412,   555,   126,   119,   119,    14,   122,   122,   420,   626,
     114,   115,   116,   119,   426,    14,   428,   119,     3,     4,
       5,     6,   121,   122,   126,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   119,   119,   109,   110,
     657,   658,   119,   126,   126,   457,    31,    32,    14,   126,
     119,   110,   119,   119,   466,   122,    41,   126,   100,   126,
     126,   119,   102,   119,    49,    50,    51,   419,   126,    54,
     126,   119,   131,   119,   133,   134,   119,   652,   126,    64,
     126,   119,   104,   126,    14,   119,   438,   119,   126,   148,
     442,   150,   126,   445,   126,   447,   123,   449,   119,   119,
      26,   453,   454,    29,    30,   126,   126,   724,    93,    94,
      36,   119,   119,    39,    40,    14,   119,    43,   126,   126,
     472,   106,   181,   126,   119,   119,   119,   122,   113,   119,
     119,   483,   126,   126,   119,   120,   126,   126,   121,   122,
     125,   126,   119,   119,   556,   497,   558,   119,   119,   126,
     126,    10,    11,    12,   126,   126,   773,   774,    80,    81,
      82,    83,    84,    85,   516,   517,   518,   119,   119,   581,
      92,   122,   524,   525,   126,   527,   528,   118,   530,   531,
     532,   533,   534,   535,   536,   537,   119,   109,   110,   122,
     119,   854,   119,   119,   857,   770,   122,   126,   119,   126,
     126,   119,   118,    10,   119,   126,    26,    14,   126,    29,
      30,   126,   564,   565,   566,   119,    36,    14,   119,    39,
      40,   119,   126,    43,    44,   126,   119,   579,   126,   119,
     582,   583,    26,   126,   119,    29,    30,   589,   120,    14,
     119,   126,    36,   125,   596,    39,    40,   126,   119,    43,
      44,   119,   119,    26,   119,   126,    29,    30,   126,   126,
      10,   126,   614,    36,   120,   119,    39,    40,   327,   125,
      43,    26,   126,   119,    29,    30,   628,   629,   630,   942,
     126,    36,   119,   635,    39,    40,   119,    26,    43,    44,
      29,    30,   119,   126,   105,   647,   119,    36,   710,   119,
      39,    40,   119,   126,    43,   122,   126,    10,    53,   721,
      13,    14,    15,    16,   119,   121,   122,   122,   670,   671,
     672,   673,   674,   675,   119,   119,   105,   122,     3,     4,
       5,     6,   126,   121,   122,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   119,    10,   125,   122,
     120,    14,   126,   126,   706,    10,    31,    32,    13,   125,
      15,    16,   121,   122,   119,   125,    41,   779,    26,   121,
     122,    29,    30,   119,    49,    50,    51,   119,    36,    54,
     119,    39,    40,   122,   119,    43,   120,   126,   120,    64,
     119,   125,   119,   125,   746,   747,   748,   749,   750,   751,
     752,   753,   120,   119,   816,    27,    28,   125,   820,   761,
     124,   470,   471,   121,   122,   474,   475,   476,    93,    94,
       7,     8,     9,   108,   483,   126,   778,   216,   217,   218,
     119,   106,   491,   492,   493,   121,   122,   120,   113,   121,
     122,   121,   122,   120,   119,   120,   119,   121,   122,    14,
     125,   126,   121,   122,    14,    26,   121,   122,    29,    30,
     105,   119,   814,   815,   122,    36,   105,    14,    39,    40,
      17,    10,    43,    44,    14,    22,    14,    24,    25,   121,
     122,    27,   834,   835,   836,    10,    33,    34,   121,   122,
     121,   122,   121,   122,    41,    42,   121,   122,   126,   851,
     121,   122,    49,    50,    51,   120,   855,   121,   122,   119,
     862,     3,     4,     5,     6,   867,   121,   122,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   121,
     122,   119,   884,   885,    27,    28,   121,   122,   119,    31,
     892,   121,   122,   121,   122,   121,   122,   119,   119,    41,
     119,   121,   122,   121,   122,    14,   615,    49,    50,    51,
      14,   120,    54,    22,   105,    24,    25,   919,    22,   123,
      24,    25,    64,    89,    33,    34,   119,   636,   111,    33,
      34,   933,    41,    42,   121,   122,   938,    41,    42,   119,
      49,    50,    51,    27,    28,    49,    50,    51,   119,    58,
     119,    93,    94,   121,   122,   119,   958,    76,    77,    78,
      79,   963,   964,   965,   106,   121,   122,    86,    87,    88,
     120,   113,   121,   122,    27,    28,   121,   122,   120,   119,
     121,   122,    14,   125,   121,   122,    42,    43,   107,   108,
     223,   224,   119,   119,   119,   997,   998,   999,  1000,  1001,
    1002,  1003,  1004,    91,   119,    93,    94,    95,    96,    97,
      98,    99,   119,   119,  1016,  1017,  1018,   105,   119,   119,
     119,   730,   119,   105,   120,   734,   735,   736,   105,   105,
     118,   124,   120,   108,   126,    72,   119,   105,   119,   119,
      10,   119,   108,   108,    10,   119,   755,   119,   119,   119,
     759,   105,   119,     3,     4,     5,     6,   119,   119,   119,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,   125,   119,    29,
      30,    31,    32,    33,    34,    35,    36,   126,   119,   119,
     126,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,   119,    14,    14,    59,
      60,    14,   119,   119,    64,   119,   825,   826,    68,    91,
     119,    93,    94,    95,    96,    97,    98,    99,   119,   119,
      10,    10,    10,   105,    10,   119,   119,    27,   119,    56,
      14,    14,    14,    93,    94,   124,   118,    14,   120,   126,
     119,   119,   119,    72,   119,   119,   106,   119,    72,   119,
     119,   119,   119,   113,   119,   126,     7,     7,     7,   119,
     120,   105,   105,    72,    10,   125,   126,   200,   201,   202,
     203,   204,   205,    10,   207,   208,   209,   210,   211,   212,
     213,   214,   121,    14,    14,   119,    67,    14,    14,   119,
     126,   119,   125,   119,   119,   126,   915,   119,   119,   119,
     919,   119,    14,    14,   119,   119,    14,   119,   119,     3,
       4,     5,     6,    14,   119,   119,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   126,   953,    29,    30,    31,    32,    33,
      34,    35,    36,   119,   119,   119,   119,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,   119,   119,   506,    59,    60,   121,   958,   442,
      64,   558,   221,    -1,    68,   222,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1015,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     126,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,
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
      54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    49,    50,    51,   119,   120,    54,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,
      50,    51,   119,   120,    54,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    93,    94,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,
      51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    67,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,
      54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    -1,    41,    -1,    -1,
      -1,   113,    -1,    -1,    -1,    49,    50,    51,   120,   121,
      54,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    -1,    41,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    49,    50,    51,   120,   121,    54,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    -1,    41,    -1,    -1,    -1,   113,    -1,    -1,
      -1,    49,    50,    51,   120,   121,    54,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      -1,    41,    -1,    -1,    -1,   113,    -1,    -1,    -1,    49,
      50,    51,   120,   121,    54,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   106,    -1,    -1,    41,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    49,    50,    51,
     120,   121,    54,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,    54,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    93,    94,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,
      41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,    50,
      51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,
      -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,
      54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    93,
      94,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,   113,
      -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   106,
      -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,    49,
      50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    93,    94,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,    41,    -1,
      -1,    -1,    -1,   113,    -1,    -1,    49,    50,    51,    -1,
     120,    54,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      93,    94,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,    -1,
     113,    -1,    -1,    49,    50,    51,    -1,   120,    54,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    93,    94,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     106,    -1,    41,    -1,    -1,    -1,    -1,   113,    -1,    -1,
      49,    50,    51,    -1,   120,    54,    -1,    -1,    -1,   125,
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
      -1,    93,    94,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   106,    -1,    41,    -1,    -1,    -1,
      -1,   113,    -1,    -1,    49,    50,    51,    -1,   120,    54,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    93,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    73,    74,
     125,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,    -1,   118
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
     118,   118,    14,   157,   157,   157,   157,   157,   157,    14,
     121,   173,   157,    14,   157,   157,    14,    17,    58,   121,
     173,   123,   107,   125,   119,   105,   121,   157,   173,   119,
     122,   105,   119,   105,   119,   105,   119,    10,    11,    12,
      14,    10,    14,    10,   169,   157,    26,    29,    30,    36,
      39,    40,    43,   119,   122,   157,    26,    43,   119,   122,
     157,   155,   157,   155,    75,   122,   155,    73,    74,   120,
     125,   144,   119,   119,   105,   105,   125,   107,   125,   107,
     125,   157,   155,   157,   155,    29,    44,    23,   157,   157,
     157,   157,   157,   157,   157,   157,   157,    53,   148,   125,
     120,    14,    62,   125,   121,   122,   126,    10,    11,    12,
      13,    24,    25,    33,    34,    47,    48,   126,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   160,   160,   160,   160,   160,
     160,   158,   160,   160,   160,   160,   160,   160,   160,   160,
       7,     8,     9,   162,   162,   162,   163,   163,   164,   165,
     166,   166,   167,   167,   167,   167,   167,    14,    22,    24,
      25,    33,    34,    41,    42,    49,    50,    51,   170,    17,
     170,   119,   120,   119,   119,   119,   119,   119,   119,   105,
     120,   121,   124,   108,   126,   119,   120,    14,   120,   121,
     157,    14,   157,   157,   119,   121,   157,   157,   120,   157,
       5,    14,   120,   153,   157,   105,   119,   105,   119,   105,
     119,   120,   144,   105,   105,   121,   122,   122,   157,   157,
     157,   157,   157,   157,   157,   173,   122,   157,   157,   173,
     121,    27,    28,   132,   121,    10,   173,   163,   163,    10,
      14,   121,   154,   129,   157,   157,   141,    14,   157,    14,
     157,   121,    27,   121,   157,   157,    10,   119,   119,   119,
     119,   119,   126,   155,    14,    22,   125,   149,   121,   154,
     120,   120,   151,   121,   173,   118,   123,   125,   105,   157,
     157,   157,   126,   157,    26,    43,   126,    26,    43,   126,
      26,    29,    30,    36,    39,    40,    43,    44,   126,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,    89,   120,    14,   120,   121,
     173,   157,   121,   173,   105,   105,   105,   121,   173,    14,
     121,   152,   173,   124,   108,   126,   119,   119,   157,   103,
     119,   122,    72,    72,   153,   119,   122,   119,   157,   120,
     157,   157,   121,   154,   157,   157,   105,    10,   173,   119,
     119,   119,   119,   119,   119,   119,   119,   173,   119,   119,
     119,   155,   131,   155,   120,   157,   155,   155,    75,   155,
      75,    75,    10,    65,    66,   144,   121,   122,    24,    25,
      34,   126,   157,   119,   119,    37,    38,   126,   108,   126,
     108,   126,   155,   155,   155,   119,   119,   105,   157,    14,
      14,   149,    56,    57,    60,   126,   144,   121,   121,   154,
     154,    69,    70,    71,   126,   121,    14,   157,   157,   157,
      26,    30,    36,    39,    40,    43,   126,   126,   126,   126,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   158,   121,   173,   121,   152,   173,   119,   121,
     119,   121,   157,   157,   157,   121,    72,   121,   122,   121,
     122,   157,   173,   157,   157,   121,     5,    14,   119,   157,
     119,   122,   119,   144,   121,   119,   119,   157,   121,   121,
      27,   132,   157,   155,    10,    10,    10,    10,    10,   144,
      10,    14,   157,   157,   126,   157,    26,    29,    30,    36,
      39,    40,    43,    44,   126,   157,   155,   119,   119,    27,
     157,   119,   126,    58,   170,    10,    13,    14,    15,    16,
      56,   144,   144,   121,   121,    14,    14,    14,   105,   120,
     124,   126,   126,   157,   157,   157,   157,   157,   157,   126,
     126,   126,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   121,   121,   121,   119,   119,   119,   119,   157,    14,
     173,   119,   119,   119,    72,    72,   122,   173,   144,   119,
     122,   119,   119,   131,   155,   121,    27,   132,   155,   155,
     155,    10,    65,    66,    26,    30,    43,   126,   126,   126,
     157,   157,   157,   157,   157,   157,   157,   157,   155,   126,
     155,   119,   138,   157,   120,   125,   145,   120,   145,    14,
      22,    63,   105,   119,    14,    22,    63,   105,   119,    10,
      13,    15,    16,   119,   119,   170,   144,   144,     7,     7,
       7,   157,   121,   173,   105,   105,   126,   126,   126,   126,
     126,   126,    72,   121,   157,   157,   173,   119,   173,   155,
     155,    10,    10,   157,   157,   157,   126,   126,   126,   126,
     126,   126,   126,   126,   157,   121,   121,   154,    67,   129,
     121,   154,    14,    14,   119,   157,    14,    14,   119,   157,
      14,    22,    63,   105,   119,    14,    22,    63,   105,   119,
     119,   119,   120,   145,   126,   126,   121,   157,   157,   157,
     119,   121,   119,    27,   132,   126,   126,   126,   119,   155,
     145,   121,   126,    24,    25,    34,   126,   157,   145,   121,
     119,   119,   119,   119,   119,   119,    14,    14,   119,   157,
      14,    14,   119,   157,   121,   154,   126,   126,   126,   119,
     155,   138,   145,   157,   157,   126,   157,    26,    29,    30,
      36,    39,    40,    43,    44,   126,   145,   119,   119,   119,
     119,   119,   119,   145,   121,   121,    26,    30,    43,   126,
     126,   126,   157,   157,   157,   157,   157,   157,   157,   157,
     145,   155,   157,   157,   157,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126
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
     131,   131,   131,   131,   131,   131,   131,   131,   132,   132,
     132,   132,   132,   132,   133,   133,   134,   134,   134,   134,
     135,   135,   136,   136,   136,   137,   137,   137,   138,   139,
     139,   139,   139,   139,   139,   140,   140,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     143,   143,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   146,   147,   148,
     148,   148,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   150,   151,   151,   151,
     151,   152,   152,   153,   153,   153,   153,   154,   154,   154,
     154,   154,   154,   154,   154,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     157,   157,   157,   158,   158,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   160,   160,   160,   161,   161,   162,   162,
     162,   163,   163,   164,   164,   165,   165,   165,   166,   166,
     166,   166,   166,   167,   167,   167,   167,   168,   168,   169,
     169,   170,   170,   170,   170,   170,   170,   170,   170,   170,
     170,   170,   171,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   173,
     173,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174
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
       3,     3,     1,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     2,     2,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     4,     3,     6,     6,     5,
       3,     3,     6,     5,     4,     4,     6,     6,     5,     3,
       6,     5,     3,     2,     3,     3,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     4,     4,     4,     5,
       5,     1,     1,     1,     1,     2,     3,     4,     5,     1,
       2
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
#line 348 "raku.y"
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
#line 358 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2708 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 359 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2714 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 363 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2720 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 365 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2726 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 367 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2732 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 369 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2740 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 373 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2746 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 375 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2752 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 377 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2758 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 379 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2766 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 383 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2773 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 386 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2781 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 390 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2787 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 392 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2793 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 394 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2799 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 396 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2805 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 398 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2811 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 400 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2819 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 404 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2827 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 408 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2833 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 410 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2839 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 412 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2845 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 414 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2851 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 416 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2857 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 418 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2863 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 420 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2869 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 422 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2875 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 424 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2881 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 426 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2887 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 428 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2893 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 430 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2899 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 432 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2905 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 434 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2911 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 436 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2918 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 439 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2925 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 442 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2931 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 444 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2938 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 447 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2945 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 450 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2951 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 452 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2957 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 454 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2963 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 456 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 2969 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 458 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 2978 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 463 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 2986 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 467 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 2994 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 471 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3003 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 476 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3011 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 479 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3017 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 481 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3023 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 483 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3029 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 485 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3035 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 487 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3041 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 489 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3048 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 492 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3054 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 494 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3060 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 496 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3066 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 498 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3073 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 501 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3080 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 504 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3087 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3094 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 510 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3101 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 513 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3107 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 515 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3113 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 517 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3119 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 519 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3125 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 521 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3132 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 524 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3138 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 526 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3144 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 528 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3150 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 530 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3157 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 533 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3164 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 536 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3172 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 540 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3179 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 543 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3185 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 545 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3191 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 547 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3197 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 549 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3204 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 552 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3211 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 555 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3218 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 558 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3225 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 561 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3232 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 564 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3239 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 567 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3246 "raku.tab.c"
    break;

  case 86: /* stmt: expr ';'  */
#line 569 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3252 "raku.tab.c"
    break;

  case 87: /* stmt: ';'  */
#line 570 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3258 "raku.tab.c"
    break;

  case 88: /* stmt: if_stmt  */
#line 571 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3264 "raku.tab.c"
    break;

  case 89: /* stmt: while_stmt  */
#line 572 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3270 "raku.tab.c"
    break;

  case 90: /* stmt: for_stmt  */
#line 573 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3276 "raku.tab.c"
    break;

  case 91: /* stmt: given_stmt  */
#line 574 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3282 "raku.tab.c"
    break;

  case 92: /* stmt: KW_TRY block  */
#line 576 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3288 "raku.tab.c"
    break;

  case 93: /* stmt: KW_TRY block KW_CATCH block  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3294 "raku.tab.c"
    break;

  case 94: /* stmt: KW_CATCH block  */
#line 580 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3300 "raku.tab.c"
    break;

  case 95: /* stmt: block  */
#line 582 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3306 "raku.tab.c"
    break;

  case 96: /* stmt: unless_stmt  */
#line 583 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3312 "raku.tab.c"
    break;

  case 97: /* stmt: until_stmt  */
#line 584 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3318 "raku.tab.c"
    break;

  case 98: /* stmt: repeat_stmt  */
#line 585 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3324 "raku.tab.c"
    break;

  case 99: /* stmt: loop_stmt  */
#line 586 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3330 "raku.tab.c"
    break;

  case 100: /* stmt: KW_LAST ';'  */
#line 587 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3336 "raku.tab.c"
    break;

  case 101: /* stmt: KW_NEXT ';'  */
#line 588 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3342 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST KW_IF expr ';'  */
#line 590 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3348 "raku.tab.c"
    break;

  case 103: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 592 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3354 "raku.tab.c"
    break;

  case 104: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 594 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3360 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 596 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3366 "raku.tab.c"
    break;

  case 106: /* stmt: sub_decl  */
#line 597 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3372 "raku.tab.c"
    break;

  case 107: /* stmt: class_decl  */
#line 598 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3378 "raku.tab.c"
    break;

  case 108: /* stmt: role_decl  */
#line 599 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3384 "raku.tab.c"
    break;

  case 109: /* stmt: grammar_decl  */
#line 600 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3390 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF '(' expr ')' block  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3396 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3402 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 608 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3408 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF expr block  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3414 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3420 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 614 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3426 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block elsif_tail  */
#line 616 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3432 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3438 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF expr block  */
#line 622 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3444 "raku.tab.c"
    break;

  case 119: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 624 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3450 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3456 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3462 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 630 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3468 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 632 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3474 "raku.tab.c"
    break;

  case 124: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 636 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3480 "raku.tab.c"
    break;

  case 125: /* while_stmt: KW_WHILE expr block  */
#line 638 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3486 "raku.tab.c"
    break;

  case 126: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 642 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3492 "raku.tab.c"
    break;

  case 127: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3498 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS expr block  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3504 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 648 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3510 "raku.tab.c"
    break;

  case 130: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3516 "raku.tab.c"
    break;

  case 131: /* until_stmt: KW_UNTIL expr block  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3522 "raku.tab.c"
    break;

  case 132: /* repeat_stmt: KW_REPEAT block  */
#line 658 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3528 "raku.tab.c"
    break;

  case 133: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3534 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 662 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3540 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 666 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3546 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 668 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3552 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 670 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3558 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 673 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3564 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 677 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3574 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 683 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3584 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 689 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3593 "raku.tab.c"
    break;

  case 142: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 694 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3603 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 700 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3612 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR expr block  */
#line 705 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3619 "raku.tab.c"
    break;

  case 145: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 710 "raku.y"
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
#line 718 "raku.y"
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
#line 728 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3650 "raku.tab.c"
    break;

  case 148: /* when_list: when_list KW_WHEN expr block  */
#line 730 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3658 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 736 "raku.y"
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
#line 744 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3680 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_SUB IDENT sub_body  */
#line 750 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3690 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 756 "raku.y"
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
#line 764 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 770 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3722 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 776 "raku.y"
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
#line 785 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 792 "raku.y"
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
#line 801 "raku.y"
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
#line 810 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3783 "raku.tab.c"
    break;

  case 160: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 819 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3794 "raku.tab.c"
    break;

  case 161: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 826 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3803 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list '}'  */
#line 832 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3809 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list expr '}'  */
#line 834 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3816 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 837 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3823 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 840 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3829 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 842 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3835 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 844 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3841 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 846 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3847 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 848 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3853 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 850 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3859 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 852 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3865 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 854 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3872 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 857 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3878 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 859 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3884 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 861 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3890 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 863 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3897 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 866 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3904 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 869 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3912 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list '}'  */
#line 874 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3918 "raku.tab.c"
    break;

  case 180: /* method_body: '{' YADA '}'  */
#line 875 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3924 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list expr '}'  */
#line 877 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3931 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 880 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3938 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 883 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3944 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 885 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3950 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 887 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3956 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3962 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3968 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 893 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3974 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3980 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 897 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3987 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 900 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3993 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 902 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3999 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 904 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4005 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 906 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4012 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 909 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4019 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 912 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4027 "raku.tab.c"
    break;

  case 197: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 918 "raku.y"
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
#line 934 "raku.y"
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
#line 948 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4068 "raku.tab.c"
    break;

  case 200: /* is_clauses: is_clauses IDENT IDENT  */
#line 950 "raku.y"
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
#line 962 "raku.y"
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
#line 975 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4106 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 977 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4113 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 980 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4120 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 983 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4127 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 986 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4134 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 989 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4141 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 992 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4148 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 995 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4155 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 998 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4162 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1001 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4169 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1004 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4176 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1007 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4183 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1010 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4190 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1013 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4201 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1020 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4212 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1027 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4223 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1034 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4234 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1041 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4242 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1045 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4250 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1049 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4258 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1053 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4266 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1057 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4277 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1064 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4288 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1071 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4299 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1078 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4310 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1085 "raku.y"
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
#line 1095 "raku.y"
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
#line 1103 "raku.y"
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
#line 1111 "raku.y"
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
#line 1120 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4372 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1127 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4383 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1134 "raku.y"
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
#line 1144 "raku.y"
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
#line 1152 "raku.y"
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
#line 1162 "raku.y"
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
#line 1176 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4444 "raku.tab.c"
    break;

  case 238: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1178 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4453 "raku.tab.c"
    break;

  case 239: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1183 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4462 "raku.tab.c"
    break;

  case 240: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1188 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4471 "raku.tab.c"
    break;

  case 241: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1195 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4479 "raku.tab.c"
    break;

  case 242: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1199 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4487 "raku.tab.c"
    break;

  case 243: /* pair_list: IDENT OP_FATARROW expr  */
#line 1205 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4493 "raku.tab.c"
    break;

  case 244: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1207 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4499 "raku.tab.c"
    break;

  case 245: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1209 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4505 "raku.tab.c"
    break;

  case 246: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1211 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4511 "raku.tab.c"
    break;

  case 247: /* param_list: VAR_SCALAR  */
#line 1214 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4517 "raku.tab.c"
    break;

  case 248: /* param_list: IDENT VAR_SCALAR  */
#line 1215 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4523 "raku.tab.c"
    break;

  case 249: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1216 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4529 "raku.tab.c"
    break;

  case 250: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1217 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4535 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' VAR_SCALAR  */
#line 1218 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4541 "raku.tab.c"
    break;

  case 252: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1219 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4547 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1220 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4553 "raku.tab.c"
    break;

  case 254: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1221 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4559 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list '}'  */
#line 1224 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4565 "raku.tab.c"
    break;

  case 256: /* block: '{' YADA '}'  */
#line 1225 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4571 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr '}'  */
#line 1227 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4577 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1229 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4583 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1231 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4589 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1233 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4595 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1235 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4601 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1237 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4608 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1240 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4614 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1242 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4620 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1244 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4626 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1246 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4632 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1248 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4639 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1251 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4646 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1254 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4654 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1258 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4661 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1261 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4668 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1264 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4675 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1267 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4681 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1269 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4689 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1273 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4696 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1276 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4703 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1279 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4710 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1282 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4717 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1285 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4724 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1288 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4730 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1290 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4736 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1292 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4742 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST '}'  */
#line 1294 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4748 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1296 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4754 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1298 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4760 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1300 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4766 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1302 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4772 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1304 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4778 "raku.tab.c"
    break;

  case 289: /* closure: '{' expr '}'  */
#line 1307 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4784 "raku.tab.c"
    break;

  case 290: /* expr: VAR_SCALAR '=' expr  */
#line 1310 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4790 "raku.tab.c"
    break;

  case 291: /* expr: KW_GATHER block  */
#line 1311 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4800 "raku.tab.c"
    break;

  case 292: /* expr: tern_expr  */
#line 1316 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4806 "raku.tab.c"
    break;

  case 293: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1320 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4812 "raku.tab.c"
    break;

  case 294: /* tern_expr: cmp_expr  */
#line 1321 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4818 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_AND jct_expr  */
#line 1324 "raku.y"
                                { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4824 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr OP_OR jct_expr  */
#line 1325 "raku.y"
                                { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4830 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr OP_EQ jct_expr  */
#line 1326 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4836 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_NE jct_expr  */
#line 1327 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4842 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr '<' jct_expr  */
#line 1328 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4848 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr '>' jct_expr  */
#line 1329 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4854 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_LE jct_expr  */
#line 1330 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4860 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_GE jct_expr  */
#line 1331 "raku.y"
                                { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4866 "raku.tab.c"
    break;

  case 303: /* cmp_expr: jct_expr OP_SEQ jct_expr  */
#line 1332 "raku.y"
                                { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4872 "raku.tab.c"
    break;

  case 304: /* cmp_expr: jct_expr OP_SNE jct_expr  */
#line 1333 "raku.y"
                                { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4878 "raku.tab.c"
    break;

  case 305: /* cmp_expr: jct_expr OP_SLT jct_expr  */
#line 1334 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4884 "raku.tab.c"
    break;

  case 306: /* cmp_expr: jct_expr OP_SLE jct_expr  */
#line 1335 "raku.y"
                                { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4890 "raku.tab.c"
    break;

  case 307: /* cmp_expr: jct_expr OP_SGT jct_expr  */
#line 1336 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4896 "raku.tab.c"
    break;

  case 308: /* cmp_expr: jct_expr OP_SGE jct_expr  */
#line 1337 "raku.y"
                                { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4902 "raku.tab.c"
    break;

  case 309: /* cmp_expr: jct_expr OP_SMATCH LIT_REGEX  */
#line 1339 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4912 "raku.tab.c"
    break;

  case 310: /* cmp_expr: jct_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1345 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4922 "raku.tab.c"
    break;

  case 311: /* cmp_expr: jct_expr OP_SMATCH LIT_SUBST  */
#line 1351 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4932 "raku.tab.c"
    break;

  case 312: /* cmp_expr: jct_expr  */
#line 1356 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4938 "raku.tab.c"
    break;

  case 313: /* jct_expr: jct_expr '|' range_expr  */
#line 1359 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4944 "raku.tab.c"
    break;

  case 314: /* jct_expr: jct_expr '&' range_expr  */
#line 1360 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4950 "raku.tab.c"
    break;

  case 315: /* jct_expr: dor_expr  */
#line 1361 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4956 "raku.tab.c"
    break;

  case 316: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1365 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4962 "raku.tab.c"
    break;

  case 317: /* dor_expr: range_expr  */
#line 1366 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4968 "raku.tab.c"
    break;

  case 318: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1369 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4974 "raku.tab.c"
    break;

  case 319: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1370 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 4980 "raku.tab.c"
    break;

  case 320: /* range_expr: add_expr  */
#line 1371 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 4986 "raku.tab.c"
    break;

  case 321: /* add_expr: add_expr '~' repl_expr  */
#line 1374 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4992 "raku.tab.c"
    break;

  case 322: /* add_expr: repl_expr  */
#line 1375 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 4998 "raku.tab.c"
    break;

  case 323: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1378 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5004 "raku.tab.c"
    break;

  case 324: /* repl_expr: addsub_expr  */
#line 1379 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5010 "raku.tab.c"
    break;

  case 325: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1382 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5016 "raku.tab.c"
    break;

  case 326: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1383 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5022 "raku.tab.c"
    break;

  case 327: /* addsub_expr: mul_expr  */
#line 1384 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5028 "raku.tab.c"
    break;

  case 328: /* mul_expr: mul_expr '*' unary_expr  */
#line 1387 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5034 "raku.tab.c"
    break;

  case 329: /* mul_expr: mul_expr '/' unary_expr  */
#line 1388 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5040 "raku.tab.c"
    break;

  case 330: /* mul_expr: mul_expr '%' unary_expr  */
#line 1389 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5046 "raku.tab.c"
    break;

  case 331: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1390 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5052 "raku.tab.c"
    break;

  case 332: /* mul_expr: unary_expr  */
#line 1391 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5058 "raku.tab.c"
    break;

  case 333: /* unary_expr: '-' unary_expr  */
#line 1394 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5064 "raku.tab.c"
    break;

  case 334: /* unary_expr: '!' unary_expr  */
#line 1395 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5070 "raku.tab.c"
    break;

  case 335: /* unary_expr: CARET unary_expr  */
#line 1396 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5076 "raku.tab.c"
    break;

  case 336: /* unary_expr: pow_expr  */
#line 1397 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5082 "raku.tab.c"
    break;

  case 337: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1400 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5088 "raku.tab.c"
    break;

  case 338: /* pow_expr: postfix_expr  */
#line 1401 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5094 "raku.tab.c"
    break;

  case 339: /* scalar_list: VAR_SCALAR  */
#line 1404 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5100 "raku.tab.c"
    break;

  case 340: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1405 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5106 "raku.tab.c"
    break;

  case 341: /* meth_name: IDENT  */
#line 1408 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5112 "raku.tab.c"
    break;

  case 342: /* meth_name: KW_SORT  */
#line 1409 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5118 "raku.tab.c"
    break;

  case 343: /* meth_name: KW_MAP  */
#line 1410 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5124 "raku.tab.c"
    break;

  case 344: /* meth_name: KW_GREP  */
#line 1411 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5130 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_SAY  */
#line 1412 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5136 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_PRINT  */
#line 1413 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5142 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_TAKE  */
#line 1414 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5148 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_RETURN  */
#line 1415 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5154 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_EXISTS  */
#line 1416 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5160 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_DELETE  */
#line 1417 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5166 "raku.tab.c"
    break;

  case 351: /* meth_name: TESTOP  */
#line 1418 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5172 "raku.tab.c"
    break;

  case 352: /* postfix_expr: call_expr  */
#line 1420 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5178 "raku.tab.c"
    break;

  case 353: /* call_expr: IDENT '(' arg_list ')'  */
#line 1423 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5187 "raku.tab.c"
    break;

  case 354: /* call_expr: IDENT '(' ')'  */
#line 1427 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5193 "raku.tab.c"
    break;

  case 355: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1429 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5202 "raku.tab.c"
    break;

  case 356: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1434 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5208 "raku.tab.c"
    break;

  case 357: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1436 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5218 "raku.tab.c"
    break;

  case 358: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1442 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5228 "raku.tab.c"
    break;

  case 359: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1448 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5236 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW  */
#line 1452 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5244 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' IDENT  */
#line 1456 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5253 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1461 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5264 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1468 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5273 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' CARET IDENT  */
#line 1473 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5283 "raku.tab.c"
    break;

  case 365: /* call_expr: atom '.' CARET IDENT  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5293 "raku.tab.c"
    break;

  case 366: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1485 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5304 "raku.tab.c"
    break;

  case 367: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1492 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5315 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5324 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name  */
#line 1504 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5333 "raku.tab.c"
    break;

  case 370: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1509 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5344 "raku.tab.c"
    break;

  case 371: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1516 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5353 "raku.tab.c"
    break;

  case 372: /* call_expr: call_expr '.' meth_name  */
#line 1521 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5362 "raku.tab.c"
    break;

  case 373: /* call_expr: KW_DIE expr  */
#line 1526 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5368 "raku.tab.c"
    break;

  case 374: /* call_expr: KW_MAP closure expr  */
#line 1528 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5374 "raku.tab.c"
    break;

  case 375: /* call_expr: KW_GREP closure expr  */
#line 1530 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5380 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_SORT expr  */
#line 1532 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5386 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_SORT closure expr  */
#line 1534 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5392 "raku.tab.c"
    break;

  case 378: /* call_expr: atom  */
#line 1535 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5398 "raku.tab.c"
    break;

  case 379: /* arg_list: expr  */
#line 1538 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5404 "raku.tab.c"
    break;

  case 380: /* arg_list: arg_list ',' expr  */
#line 1539 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5410 "raku.tab.c"
    break;

  case 381: /* atom: LIT_INT  */
#line 1542 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5416 "raku.tab.c"
    break;

  case 382: /* atom: LIT_FLOAT  */
#line 1543 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5422 "raku.tab.c"
    break;

  case 383: /* atom: LIT_STR  */
#line 1544 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5428 "raku.tab.c"
    break;

  case 384: /* atom: WORDLIST  */
#line 1546 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5440 "raku.tab.c"
    break;

  case 385: /* atom: LIT_INTERP_STR  */
#line 1553 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5446 "raku.tab.c"
    break;

  case 386: /* atom: VAR_SCALAR  */
#line 1554 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5452 "raku.tab.c"
    break;

  case 387: /* atom: OP_INC VAR_SCALAR  */
#line 1555 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5458 "raku.tab.c"
    break;

  case 388: /* atom: OP_DEC VAR_SCALAR  */
#line 1556 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5464 "raku.tab.c"
    break;

  case 389: /* atom: VAR_SCALAR OP_INC  */
#line 1557 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5470 "raku.tab.c"
    break;

  case 390: /* atom: VAR_SCALAR OP_DEC  */
#line 1558 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5476 "raku.tab.c"
    break;

  case 391: /* atom: VAR_ARRAY  */
#line 1559 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5482 "raku.tab.c"
    break;

  case 392: /* atom: VAR_HASH  */
#line 1560 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5488 "raku.tab.c"
    break;

  case 393: /* atom: VAR_CAPTURE  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5496 "raku.tab.c"
    break;

  case 394: /* atom: VAR_FH  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5504 "raku.tab.c"
    break;

  case 395: /* atom: VAR_NAMED_CAPTURE  */
#line 1570 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5511 "raku.tab.c"
    break;

  case 396: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1573 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5517 "raku.tab.c"
    break;

  case 397: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1575 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5523 "raku.tab.c"
    break;

  case 398: /* atom: VAR_HASH '{' expr '}'  */
#line 1577 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5529 "raku.tab.c"
    break;

  case 399: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1579 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5535 "raku.tab.c"
    break;

  case 400: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1581 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5541 "raku.tab.c"
    break;

  case 401: /* atom: IDENT  */
#line 1582 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5547 "raku.tab.c"
    break;

  case 402: /* atom: VAR_TWIGIL  */
#line 1584 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5555 "raku.tab.c"
    break;

  case 403: /* atom: VAR_ARRAY_TWIGIL  */
#line 1588 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5563 "raku.tab.c"
    break;

  case 404: /* atom: VAR_HASH_TWIGIL  */
#line 1592 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5571 "raku.tab.c"
    break;

  case 405: /* atom: '(' ')'  */
#line 1595 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5577 "raku.tab.c"
    break;

  case 406: /* atom: '(' expr ')'  */
#line 1596 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5583 "raku.tab.c"
    break;

  case 407: /* atom: '(' expr ',' ')'  */
#line 1598 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5589 "raku.tab.c"
    break;

  case 408: /* atom: '(' expr ',' arg_list ')'  */
#line 1600 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5596 "raku.tab.c"
    break;

  case 409: /* atom: block  */
#line 1602 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5602 "raku.tab.c"
    break;

  case 410: /* atom: KW_SUB block  */
#line 1603 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5608 "raku.tab.c"
    break;


#line 5612 "raku.tab.c"

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

#line 1605 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
