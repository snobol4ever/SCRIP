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
static tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_TO || rhs->n < 2) return rhs;
    tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]);
    return call;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_index(const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]);
        return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, idx); return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_pick(const char *arr, tree_t *i0, ExprList *rest) {
    tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(arr)); expr_add_child(call, i0);
    if (rest) { for (int i = 0; i < rest->count; i++) expr_add_child(call, rest->items[i]); exprlist_free(rest); }
    return call;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_end_index(const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_all(const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *lo = ast_node_new(TT_ILIT); lo->v.ival = 0;
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, lo); expr_add_child(call, rk_dec(el));
    return call;
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

#line 369 "raku.tab.c"

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
  YYSYMBOL_OP_REDUCE = 63,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 64,             /* ARR_ALL_SLICE  */
  YYSYMBOL_KW_HANDLES = 65,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 66,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 67,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 68,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 69,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 70,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 71,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 72,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 73,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 74,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 75,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 76,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 77,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 78,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 79,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 80,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 81,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 82,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 83,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 84,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 85,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 86,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 87,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 88,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 89,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 90,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 91,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 92,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 93,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 94,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 95,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 96,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 97,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 98,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 99,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 100,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 101,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 102,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 103,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 104,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 105,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 106,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 107,                   /* OP_POW  */
  YYSYMBOL_108_ = 108,                     /* '='  */
  YYSYMBOL_109_ = 109,                     /* '!'  */
  YYSYMBOL_110_ = 110,                     /* '<'  */
  YYSYMBOL_111_ = 111,                     /* '>'  */
  YYSYMBOL_112_ = 112,                     /* '|'  */
  YYSYMBOL_113_ = 113,                     /* '&'  */
  YYSYMBOL_114_ = 114,                     /* '~'  */
  YYSYMBOL_115_ = 115,                     /* '+'  */
  YYSYMBOL_116_ = 116,                     /* '-'  */
  YYSYMBOL_117_ = 117,                     /* '*'  */
  YYSYMBOL_118_ = 118,                     /* '/'  */
  YYSYMBOL_119_ = 119,                     /* '%'  */
  YYSYMBOL_UMINUS = 120,                   /* UMINUS  */
  YYSYMBOL_121_ = 121,                     /* '.'  */
  YYSYMBOL_122_ = 122,                     /* ';'  */
  YYSYMBOL_123_ = 123,                     /* '('  */
  YYSYMBOL_124_ = 124,                     /* ')'  */
  YYSYMBOL_125_ = 125,                     /* ','  */
  YYSYMBOL_126_ = 126,                     /* '['  */
  YYSYMBOL_127_ = 127,                     /* ']'  */
  YYSYMBOL_128_ = 128,                     /* '{'  */
  YYSYMBOL_129_ = 129,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 130,                 /* $accept  */
  YYSYMBOL_program = 131,                  /* program  */
  YYSYMBOL_stmt_list = 132,                /* stmt_list  */
  YYSYMBOL_stmt = 133,                     /* stmt  */
  YYSYMBOL_if_stmt = 134,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 135,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 136,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 137,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 138,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 139,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 140,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 141,                /* loop_incr  */
  YYSYMBOL_for_stmt = 142,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 143,               /* given_stmt  */
  YYSYMBOL_when_list = 144,                /* when_list  */
  YYSYMBOL_sub_decl = 145,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 146,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 147,                 /* sub_body  */
  YYSYMBOL_method_body = 148,              /* method_body  */
  YYSYMBOL_class_decl = 149,               /* class_decl  */
  YYSYMBOL_role_decl = 150,                /* role_decl  */
  YYSYMBOL_is_clauses = 151,               /* is_clauses  */
  YYSYMBOL_class_body_list = 152,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 153,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 154,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 155,           /* named_arg_list  */
  YYSYMBOL_pair_list = 156,                /* pair_list  */
  YYSYMBOL_param_list = 157,               /* param_list  */
  YYSYMBOL_block = 158,                    /* block  */
  YYSYMBOL_closure = 159,                  /* closure  */
  YYSYMBOL_expr = 160,                     /* expr  */
  YYSYMBOL_tern_expr = 161,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 162,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 163,               /* divis_expr  */
  YYSYMBOL_jct_expr = 164,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 165,                 /* dor_expr  */
  YYSYMBOL_range_expr = 166,               /* range_expr  */
  YYSYMBOL_add_expr = 167,                 /* add_expr  */
  YYSYMBOL_repl_expr = 168,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 169,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 170,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 171,               /* unary_expr  */
  YYSYMBOL_pow_expr = 172,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 173,              /* scalar_list  */
  YYSYMBOL_meth_name = 174,                /* meth_name  */
  YYSYMBOL_postfix_expr = 175,             /* postfix_expr  */
  YYSYMBOL_call_expr = 176,                /* call_expr  */
  YYSYMBOL_arg_list = 177,                 /* arg_list  */
  YYSYMBOL_atom = 178                      /* atom  */
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
#define YYLAST   4021

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  422
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1077

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   363


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   109,     2,     2,     2,   119,   113,     2,
     123,   124,   117,   115,   125,   116,   121,   118,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   122,
     110,   108,   111,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   128,   112,   129,   114,     2,     2,     2,
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
     105,   106,   107,   120
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   383,   383,   394,   395,   398,   400,   402,   404,   408,
     410,   412,   414,   418,   421,   425,   427,   431,   434,   438,
     440,   442,   444,   446,   448,   452,   456,   458,   460,   462,
     464,   466,   468,   470,   472,   474,   476,   478,   480,   482,
     484,   487,   490,   492,   495,   498,   500,   502,   504,   506,
     511,   515,   519,   524,   528,   529,   531,   533,   535,   537,
     540,   542,   544,   546,   549,   552,   555,   558,   561,   563,
     565,   567,   569,   572,   574,   576,   578,   581,   584,   588,
     591,   593,   595,   597,   600,   603,   606,   609,   612,   615,
     618,   619,   620,   621,   622,   623,   624,   626,   628,   630,
     632,   633,   634,   635,   636,   637,   638,   640,   642,   644,
     646,   647,   648,   649,   652,   654,   656,   658,   660,   662,
     664,   666,   670,   672,   674,   676,   678,   680,   684,   686,
     690,   692,   694,   696,   700,   702,   706,   708,   710,   714,
     716,   718,   722,   725,   731,   737,   742,   748,   753,   758,
     766,   777,   778,   784,   792,   798,   804,   812,   818,   824,
     833,   840,   849,   858,   867,   874,   881,   882,   885,   888,
     890,   892,   894,   896,   898,   900,   902,   905,   907,   909,
     911,   914,   917,   923,   924,   925,   928,   931,   933,   935,
     937,   939,   941,   943,   945,   948,   950,   952,   954,   957,
     960,   966,   982,   997,   998,  1010,  1024,  1025,  1028,  1031,
    1034,  1037,  1040,  1043,  1046,  1049,  1052,  1055,  1058,  1061,
    1068,  1075,  1082,  1089,  1093,  1097,  1101,  1105,  1112,  1119,
    1126,  1133,  1143,  1151,  1159,  1168,  1175,  1182,  1192,  1200,
    1210,  1225,  1226,  1231,  1236,  1243,  1247,  1253,  1255,  1257,
    1259,  1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1273,
    1274,  1275,  1277,  1279,  1281,  1283,  1285,  1288,  1290,  1292,
    1294,  1296,  1299,  1302,  1306,  1309,  1312,  1315,  1317,  1321,
    1324,  1327,  1330,  1333,  1336,  1338,  1340,  1342,  1344,  1346,
    1348,  1350,  1352,  1356,  1359,  1360,  1365,  1368,  1370,  1373,
    1374,  1375,  1376,  1377,  1378,  1379,  1380,  1381,  1382,  1383,
    1384,  1385,  1386,  1387,  1393,  1399,  1405,  1408,  1409,  1412,
    1413,  1414,  1417,  1419,  1422,  1423,  1424,  1427,  1428,  1431,
    1432,  1435,  1436,  1437,  1440,  1441,  1442,  1443,  1444,  1447,
    1448,  1449,  1450,  1455,  1458,  1459,  1462,  1463,  1466,  1467,
    1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1478,
    1480,  1485,  1486,  1491,  1493,  1499,  1505,  1509,  1513,  1518,
    1525,  1530,  1536,  1542,  1549,  1556,  1561,  1566,  1573,  1578,
    1583,  1585,  1587,  1589,  1591,  1593,  1596,  1597,  1598,  1601,
    1602,  1603,  1604,  1612,  1613,  1614,  1615,  1616,  1617,  1618,
    1619,  1620,  1624,  1628,  1631,  1633,  1635,  1637,  1639,  1641,
    1643,  1645,  1647,  1649,  1650,  1654,  1658,  1662,  1663,  1664,
    1666,  1669,  1670
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
  "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE", "ARR_ALL_SLICE",
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT", "OP_SGE",
  "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_DIVIS", "OP_REP_X",
  "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "'('", "')'",
  "','", "'['", "']'", "'{'", "'}'", "$accept", "program", "stmt_list",
  "stmt", "if_stmt", "elsif_tail", "while_stmt", "unless_stmt",
  "until_stmt", "repeat_stmt", "loop_stmt", "loop_incr", "for_stmt",
  "given_stmt", "when_list", "sub_decl", "scalar_methcall", "sub_body",
  "method_body", "class_decl", "role_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "cmp_expr", "divis_expr", "jct_expr", "dor_expr", "range_expr",
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

#define YYPACT_NINF (-781)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-422)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -781,    40,  1640,  -781,  -781,  -781,  -781,  -781,   835,   -58,
     -57,   -90,   284,  -781,  -781,  3893,  -781,  -781,  -781,    78,
    1710,    49,  2983,  3053,  3123,  3193,  3263,    24,   -13,  3263,
    1780,     5,  3263,   130,   158,  3333,  3403,   -13,   305,    95,
     185,    55,    55,  3473,   -13,   -13,  3263,   205,   347,    58,
    3893,  -781,  -781,   352,   170,   242,  3893,  3893,  -781,  1850,
     328,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,   531,  -781,  -781,  -781,   627,   582,  -781,   551,   494,
     -31,   313,  -781,   129,   325,   160,   251,  -781,  -781,   354,
     311,   376,   467,  -781,  -781,  3263,  3263,  3263,  3263,  3263,
    3263,   530,  1921,  3543,  1992,   601,  3263,  3263,    86,  2062,
     138,   493,    -4,  -781,   -13,  -781,  -781,   474,   213,  -781,
    2133,  -781,   212,   131,   211,   235,   712,   609,   289,   618,
    1850,   480,  1850,   301,  1850,   -13,  1850,   -13,   -40,   166,
     357,  -781,  -781,   511,  -781,   537,   562,   579,   570,     7,
     126,  1850,   -13,  1850,   -13,   201,  2204,  -781,  3263,  3263,
    -781,  3263,  3263,  -781,  3263,  3263,  3263,  2274,  3263,  -781,
     694,  -781,  -781,  -781,   624,   645,    65,  -781,   658,  -781,
    -781,  -781,  -781,  -781,   258,   643,  1089,  3263,  3263,  3263,
    3263,  3263,  3263,  3263,  3263,  -781,  3263,  3263,  3263,  3263,
    3263,  3263,  3263,  3263,  -781,  3893,  3893,  3893,  3893,  3893,
    3893,  3893,  3893,  3893,  3893,  3893,  3893,  3893,  3893,  3893,
     742,  3893,  3893,  3893,  3893,  3893,  3893,  3893,  3893,  3893,
    3893,  3893,  3893,  3893,  3893,  3893,   805,   782,   297,   670,
     675,   679,   681,   700,   739,   -97,  -781,   522,  1850,   157,
     439,   574,   754,   738,   747,   749,   875,   788,  -781,   549,
    1992,   904,  3263,  -781,  3263,   819,   258,   684,  -781,  3263,
    3263,  -781,  3613,  -781,  3683,  -781,   255,   265,   270,   368,
     845,   847,  -781,   689,   693,  3263,  3263,  3263,  3263,  3263,
    3263,  3263,  -781,  3263,   696,  3263,  3263,  -781,  3263,   701,
     807,   716,  -781,   947,  3263,  -781,  3893,  3893,     2,  -781,
    -781,  -781,  -781,  3263,  3263,  -781,   957,  3263,   972,  3263,
     720,   974,   724,  -781,  3263,  3263,   989,   887,   890,   891,
     892,   893,   889,  -781,  -781,  -781,   -13,    -5,  -781,    15,
     896,   897,  -781,  -781,  2344,  -781,   877,   143,   164,   913,
    2983,  3053,  3263,   888,   230,   238,  -781,    26,   900,   901,
     902,   903,   912,   914,   915,   916,   917,   918,   919,   920,
     921,   922,   926,   927,   931,   931,   931,   931,   931,   931,
     959,   931,   931,   931,   931,   931,   931,   931,   931,  -781,
    -781,  -781,   -31,  -781,  -781,  -781,   937,   937,   325,   160,
     251,   251,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,   929,  1039,
     933,  -781,  2415,  -781,  -781,  -781,  -781,  -781,  -781,  3263,
    2486,  -781,   726,  3263,  -781,  3263,  3263,  3263,  3263,   946,
     949,   950,  -781,  2557,  -781,  2628,  -781,   644,   944,   930,
    -781,  -781,   938,  -781,   940,  1850,   220,   990,   -10,  2699,
     231,   941,  3263,  -781,  3753,  -781,  3263,  -781,    21,  -781,
    3263,  3263,   960,  1059,  2344,   948,   951,   952,   953,   954,
     955,   956,   416,  2344,   958,   962,   447,   -13,   -18,  3823,
    -781,   -13,   -13,    54,   136,   152,  -781,   110,   943,   728,
    1258,   965,   966,     8,   968,   967,   970,   969,   -13,   -13,
     -13,   975,   994,   964,  3263,  -781,  -781,  -781,  -781,  -781,
    -781,  1068,  1069,  -781,    18,   943,   733,    22,   348,   102,
    -781,   735,  1071,  1992,  3263,  3263,   345,    93,   149,  -781,
     203,  3263,  3263,  -781,  3263,  3263,  -781,  3263,  3263,  3263,
    3263,  3263,  3263,  3263,  3263,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  3893,  2770,  -781,  2841,   995,   751,  1004,  -781,
     753,  2344,  1005,   472,  1001,  1002,   660,  3263,  3263,  3263,
    -781,   756,    -7,  -781,   758,   760,  -781,  -781,  -781,  -781,
    -781,   763,  3263,  -781,  3263,  3263,  3263,   772,  -781,   292,
    -781,  1023,  1850,   512,  1024,   943,   785,  1025,  1028,  3263,
    -781,   790,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
     792,  -781,  -781,  -781,   809,  -781,  -781,  1850,   -13,  -781,
    -781,  1079,  -781,  1081,  1141,  -781,  1144,  1146,  -781,   943,
     554,  2983,  3053,  1449,  -781,    47,  -781,  -781,  3263,   -13,
    -781,  -781,  -781,  1038,  1040,  1134,  -781,  -781,  -781,  -781,
    3263,  1041,  -781,  -781,    52,   630,   433,  1108,  -781,  -781,
     943,   943,   798,   800,  1151,  1152,  1153,  -781,  -781,   197,
     673,  1042,   269,  3263,  3263,  3263,  3263,  3263,  3263,  -781,
    -781,  -781,  -781,   272,   273,   295,   307,   316,   322,   330,
     344,   349,   355,   360,   361,  -781,  -781,   802,  -781,   820,
     822,  -781,  1046,  -781,  -781,   825,  -781,  -781,  -781,  -781,
    -781,  1047,  1048,  1050,  -781,  3263,  -781,  1159,  -781,  2344,
    1052,   520,  -781,  -781,  1053,  1102,  1103,  -781,   839,  -781,
    3263,  -781,  -781,   943,  -781,  -781,   535,  1056,  1057,   -18,
    -781,   841,   932,   -13,   -13,   -13,  -781,  -781,  -781,  -781,
     228,   350,   115,  -781,   364,  3263,  3263,  3263,  3263,  3263,
    3263,  3263,  3263,  -781,   -13,  1051,  -781,  -781,   -13,  1060,
    3263,  -781,   377,   390,    85,   192,   858,  1061,  1064,   805,
    -781,  -781,   943,   943,  1174,  1180,  1181,  3263,  2912,  1082,
    1083,  -781,   373,   379,   392,   393,   395,   405,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  1067,  -781,  -781,  -781,  -781,  1118,
     844,  -781,  -781,  -781,  3263,  3263,  2344,   564,  -781,  -781,
    3263,  -781,  -781,  -781,  -781,   -13,   -13,  -781,  -781,  -781,
    -781,  -781,  1183,  1184,  3263,  3263,  3263,  -781,  -781,  -781,
     406,   407,   408,   434,   444,   460,   461,   462,  -781,  -781,
    -781,  3263,  1072,  -781,    29,  1126,  -781,    37,  -781,  1185,
    1186,  1075,  3263,  -781,  1187,  1188,  1084,  3263,  -781,   194,
     233,  1085,  1086,  -781,  -781,   430,  -781,  -781,  -781,  -781,
    -781,   470,  1074,   855,  3263,  3263,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  3263,  1087,  -781,  -781,   857,  -781,   575,
     961,  -781,  -781,  -781,   484,   485,   487,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  1088,   -13,  1076,   866,  1090,
    1379,  1076,   871,  1091,  1092,  -781,  1093,  1094,  1098,  -781,
    1099,  1208,  1209,  1104,  3263,  -781,  1210,  1211,  1105,  3263,
    -781,  -781,  -781,    44,  -781,  -781,  -781,  1100,   498,   514,
    -781,  -781,  1106,  -781,   -13,  -781,  -781,  -781,  -781,  3263,
    -781,  -781,  1076,  -781,  2983,  3053,  1519,  -781,   370,  -781,
    1076,  -781,  -781,  -781,  -781,  -781,  -781,  1109,  1110,  -781,
    1111,  1112,  1113,  -781,  1114,  1076,   878,  -781,  -781,  -781,
    -781,  -781,  1115,  -781,   382,   206,  -781,   536,  3263,  3263,
    3263,  3263,  3263,  3263,  3263,  3263,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  1076,   -13,  3263,  3263,  3263,
    -781,  -781,  -781,   547,   553,   555,   556,   561,   596,   597,
     598,  -781,  -781,   606,   614,   662,  -781,  -781,  -781,  -781,
    -781,  -781,  -781,  -781,  -781,  -781,  -781
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   389,   390,   391,   393,   394,   399,
     400,   414,   413,   415,   416,     0,   401,   402,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   408,   392,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   296,   298,   316,
     318,   321,   323,   326,   328,   330,   333,   338,   343,   345,
     359,   385,     0,   397,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,   399,   400,   414,     0,   421,   341,     0,   394,    35,
       0,   386,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   326,
       0,   422,   295,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   139,     0,     0,
     104,     0,     0,   105,     0,     0,     0,     3,     0,   383,
      96,    98,   380,   203,     0,     0,     0,   342,     0,   395,
     396,   340,   339,   417,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   294,     0,   363,     0,     0,     0,
       0,     0,     0,     0,     0,   368,     0,   367,   361,     0,
       0,     0,     0,    30,     0,   417,     0,     0,    38,   388,
       0,     6,     0,     9,     0,    10,     0,     0,     0,     0,
       0,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,    42,     0,     0,
     117,     0,   129,     0,     0,   148,     0,     0,     0,     3,
     155,    45,    46,     0,     0,   151,     0,     0,     0,     0,
       0,   132,     0,   135,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   381,   382,   384,     0,     0,   206,     0,
       0,     0,   241,   418,     0,   260,   394,   399,   400,   414,
       0,     0,     0,     0,     0,     0,   259,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,   302,   305,   306,   299,   300,
       0,   303,   304,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   317,   319,   320,   322,   324,   325,   327,   329,
     331,   332,   337,   334,   335,   336,   344,   348,   358,   352,
     353,   354,   355,   356,   357,   350,   351,   349,   379,     0,
     376,    51,     0,    85,    86,    87,    88,    89,    48,     0,
       0,   362,     0,     0,    15,     0,     0,     0,     0,   404,
     409,   410,    53,     0,   371,     0,   360,     0,     0,     0,
     294,    37,     0,   387,     0,     0,     0,   391,   413,     0,
       0,     0,     0,    27,     0,    28,     0,    29,     0,   158,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   418,     0,     0,
     120,   418,     0,     0,   324,   325,   251,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   418,     0,
     418,     0,     0,     0,     0,   106,   107,   108,   109,   293,
      97,     0,     0,   206,     0,     0,     0,     0,     0,     0,
     419,     0,     0,     0,     0,     0,     0,     0,     0,   286,
       0,     0,     0,   287,     0,     0,   288,     0,     0,     0,
       0,     0,     0,     0,     0,   261,    55,    57,    59,    62,
      60,    61,    56,    58,    68,    70,    72,    75,    73,    74,
      69,    71,     0,     0,   372,     0,     0,     0,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     370,     0,   413,   366,     0,     0,   404,   409,   410,    36,
       5,     0,     0,    11,     0,     0,     0,     0,    20,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,     0,    76,    79,    78,    82,    80,    81,    77,    40,
       0,    83,    84,    43,   114,   119,   118,     0,     0,   128,
     145,     0,   147,     0,     0,   252,     0,     0,   154,     0,
       0,     0,     0,     0,   166,     0,    32,    31,     0,     0,
     149,   411,   412,     0,     0,   130,   133,   134,   137,   138,
       0,     0,   204,   205,     0,     0,     0,     0,   202,   163,
       0,     0,     0,     0,     0,     0,     0,   240,   420,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   270,
     277,   284,   285,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   297,   378,     0,   375,     0,
       0,    50,     0,    52,   164,     0,    17,    16,   407,   406,
     405,     0,     0,     0,   369,     0,   364,     0,   365,     0,
       0,     0,   248,   247,     0,     0,     0,    22,     0,    23,
       0,    26,   157,     0,    34,    33,     0,   420,   420,     0,
     121,     0,   122,     0,     0,     0,   253,   254,   153,   255,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,    66,    67,     0,     0,
       0,   201,     0,     0,     0,     0,     0,     0,     0,     0,
     162,   160,     0,     0,     0,     0,     0,     0,     0,   404,
     410,   281,     0,     0,     0,     0,     0,     0,   289,   290,
     291,   292,   262,   264,   266,   269,   267,   268,   263,   265,
     377,   374,   373,    49,   420,    63,    64,    65,   245,     0,
       0,    13,    12,    21,     0,     0,     0,     0,   156,     7,
       0,    41,    44,   116,   115,   418,     0,   126,   146,   143,
     144,   256,     0,     0,     0,     0,     0,   170,   171,   168,
       0,     0,     0,     0,     0,     0,     0,     0,   152,   150,
     131,     0,     0,   142,     0,     3,   236,     0,   233,     0,
       0,     0,     0,   212,     0,     0,     0,     0,   207,     0,
       0,     0,     0,   208,   209,     0,   159,   161,   242,   243,
     244,     0,   165,     0,     0,     0,   271,   273,   276,   274,
     275,   272,    18,     0,   420,   250,   249,     0,    24,     0,
     123,   124,   257,   258,     0,     0,     0,   172,   174,   176,
     179,   177,   178,   173,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,   224,
       0,     0,     0,     0,     0,   214,     0,     0,     0,     0,
     213,   210,   211,     0,   239,   280,   279,   164,     0,     0,
     246,    14,   420,     8,     0,   127,   180,   182,   181,     0,
     141,   235,     0,   184,     0,     0,     0,   183,     0,   232,
       0,   221,   222,   216,   219,   220,   215,     0,     0,   225,
       0,     0,     0,   226,     0,     0,     0,   278,   282,   283,
      25,   125,     0,   234,     0,     0,   187,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   185,   231,   229,   230,
     218,   227,   228,   217,   238,     0,     0,     0,     0,     0,
     188,   189,   186,     0,     0,     0,     0,     0,     0,     0,
       0,   237,   140,     0,     0,     0,   190,   192,   194,   197,
     195,   196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -781,  -781,     1,  -781,  -482,  -629,  -781,  -781,  -781,  -781,
    -781,   241,  -781,  -781,  -781,  -781,  -781,  -277,  -780,  -781,
    -781,  -781,   714,  -781,  -781,   663,   781,  -325,   302,   963,
      -2,  -207,  -781,   548,  1020,  -781,   559,   -16,  1015,  1016,
     778,    -8,  -781,  -781,  -234,  -781,  -781,    20,  -781
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   186,    61,    62,   490,    63,    64,    65,    66,
      67,   882,    68,    69,   503,    70,    71,   310,   886,    72,
      73,   337,   524,    74,   529,   594,   460,   499,   115,   165,
     121,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   283,   418,    89,    90,   122,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,     2,   469,   420,   380,   760,   635,   116,    24,   521,
     139,   429,   496,   888,   526,   146,   497,   522,   107,   147,
     131,   133,   135,   137,   138,   496,   430,   143,   145,   497,
     148,   496,   496,   152,   154,   497,   497,   303,   140,   496,
       3,   169,   177,   497,   172,   658,   659,   496,   181,   182,
     103,   497,   547,   105,   496,   548,   549,   184,   497,   123,
     124,   125,   550,   126,   606,   551,   552,   735,   104,   553,
     554,   106,   175,   775,   675,   676,   776,   777,   677,   340,
     127,   222,   223,   778,   128,   304,   779,   780,    60,   176,
     781,   782,   117,   239,   240,   241,   242,   243,   244,   889,
     255,   249,   251,   256,   253,   254,   261,   890,   675,   676,
      60,   108,   677,   109,   108,    60,   109,   316,   266,   295,
     645,   158,   247,   523,   262,   974,   498,   341,   284,   259,
     294,   641,   299,   857,   301,   317,   296,   660,   159,   525,
     267,   295,   149,   616,   257,   615,   681,   678,   204,   320,
     891,   322,    60,   947,   327,   555,   328,   329,   296,   330,
     331,   951,   332,   333,   334,   332,   335,   991,  1015,   204,
     150,   999,   129,   684,   685,   686,   783,   646,   647,   269,
     179,   791,    60,   164,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   892,   366,   367,   368,   369,   370,   371,
     372,   373,   682,   683,   225,   226,   894,   893,   961,   396,
     397,   161,  1023,   643,   895,   297,   962,   160,   298,   173,
    1037,   648,   700,   402,   403,   404,   405,   406,   162,   644,
     324,   687,   295,    93,    94,  1044,   318,   297,   861,   270,
     298,   306,   307,   227,   868,   325,   432,   966,   679,   296,
     227,   103,   180,   271,   319,   967,   541,   896,   447,   963,
     449,   102,   450,   433,   544,  1061,   227,   453,   454,   533,
     456,   311,   461,   542,   105,   229,   230,   853,   701,   434,
     227,   545,   435,   475,   476,   477,   478,   479,   480,   481,
     494,   495,   534,   484,   485,   862,   863,   745,   968,   280,
     897,   985,   964,   281,    75,   807,   746,   163,    93,    94,
     500,   501,   502,   482,   898,   505,   965,   507,   486,   272,
     808,   264,   511,   512,   493,   312,   602,   295,   297,   141,
     142,   298,   702,   273,   268,  1051,   102,   269,   752,   155,
     157,   969,   603,   274,   296,   604,   170,   171,   536,   537,
     538,   540,   160,   608,   231,   970,   609,   275,   496,   543,
     163,   174,   497,   462,   531,   715,   178,   546,   232,   233,
     234,   693,   768,   464,   286,   694,   864,   463,   466,   286,
     865,   695,   343,   344,   696,   697,   288,   465,   698,   289,
     290,   442,   467,   866,   515,   516,  1028,   185,   811,  1029,
    1030,   818,   819,   800,   801,   108,  1031,   109,  1047,  1032,
    1033,   286,  1048,  1034,  1035,   224,   141,   517,   288,   421,
     422,   289,   290,   297,   820,  1049,   298,   578,   156,   518,
     228,   582,   236,    60,   584,   585,   821,   300,   564,   302,
     305,   793,   577,   794,   565,   822,   795,   796,   797,   798,
     580,   823,   566,   601,   321,   583,   323,   184,   586,   824,
     611,   235,   613,   591,   614,   595,   567,   292,   617,   618,
     293,   568,   292,   825,   699,   293,   848,   569,   826,   867,
     308,   238,   570,   571,   827,   309,   312,   638,    75,   828,
     829,   468,   204,   869,   621,   622,   309,   237,   655,  1036,
     884,   624,   916,   630,   292,   885,   285,   293,   917,   286,
     287,  1050,   671,   887,   625,   626,   288,   627,   885,   289,
     290,   918,   919,   291,   920,   906,   907,   628,   564,   565,
     566,   690,   691,   692,   921,   937,   938,   939,   629,   703,
     704,   269,   705,   706,   245,   707,   708,   709,   710,   711,
     712,   713,   714,   973,   436,   437,   567,   187,   885,   948,
     188,   189,   952,   940,   769,   905,   568,   190,   770,   633,
     191,   192,   269,   941,   193,   194,   214,   215,   216,   217,
     218,   219,   569,   570,   571,   731,   732,   733,   220,   942,
     943,   944,   723,   717,   727,   720,   263,   269,   221,   975,
     740,   725,   292,   742,   743,   293,   622,   624,   196,   628,
     748,   197,   198,   986,   987,   252,   988,   756,   199,   260,
     835,   200,   201,   279,   741,   202,   203,  1018,   282,   205,
     206,   207,   208,   311,   749,   761,   837,   750,   520,   209,
     210,   211,   842,  1019,   407,   269,   431,   269,  1016,   771,
     772,   774,   408,   195,   409,   410,   784,   849,   312,   312,
     850,   212,   213,   411,   412,  1052,  -421,  -421,   789,   564,
     313,   413,   414,   446,   269,   565,  1066,   566,   567,   415,
     416,   417,  1067,   568,  1068,  1069,   928,   314,   792,   269,
    1070,   812,   813,   814,   815,   816,   817,   983,   315,   438,
     269,   439,  -421,  -421,   204,  -421,  -421,  -421,  -421,  -421,
    -421,  -421,  -421,  -421,  -421,  -421,  -421,  -421,   569,   570,
     571,  -421,   276,   277,   278,  1071,  1072,  1073,   622,  -421,
    -421,  -421,  -421,   838,  -421,  1074,   624,  -421,  -421,  -421,
    -421,  -421,  -421,  1075,  -421,  -421,  -421,   336,  -421,   389,
     390,   391,   338,   374,   375,   376,   377,   378,   379,   840,
     381,   382,   383,   384,   385,   386,   387,   388,   339,   438,
     847,   596,   345,   870,   871,   872,   873,   874,   875,   876,
     877,   393,   394,   395,   628,   269,   342,   730,   883,   634,
     636,  1076,   423,   639,   640,   642,   407,   424,   438,   419,
     809,   425,    75,   426,   408,   911,   409,   410,   452,   269,
     665,   666,   667,   472,   473,   411,   412,   343,   474,   407,
     343,   483,   427,   413,   414,   487,   344,   408,   913,   409,
     410,   415,   416,   417,   488,   489,   759,   489,   411,   412,
     491,   344,   925,   926,   508,   344,   413,   414,   510,   344,
     343,   581,   649,   650,   415,   416,   417,   680,   650,   688,
     269,   428,   934,   935,   936,   440,   927,   441,   899,   442,
     929,   900,   443,   901,   902,   722,   269,   724,   269,   945,
     734,   269,   736,   737,   738,   269,   950,   343,   739,   444,
     956,     4,     5,     6,     7,   960,   744,   609,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,   753,
     650,   445,   978,   979,   757,   269,   758,   269,   448,   114,
      28,   980,   802,   650,   803,   650,   830,   269,    92,    33,
      93,    94,    95,    96,    97,    98,    99,    41,    42,    43,
     762,   451,    46,   100,   831,   737,   832,   269,   998,   834,
     269,    50,    51,   470,    52,   471,   101,   492,   102,   856,
     489,   785,  1010,   343,   846,   855,   344,  1014,   924,   269,
      92,   504,    93,    94,    95,    96,    97,    98,    99,   977,
     269,   982,   269,    54,    55,   100,   506,   883,   984,   489,
     992,   650,  1024,  1025,  1027,  1000,   650,    56,   532,   513,
     102,   509,  1045,   650,    57,   166,   168,   400,   401,   514,
     144,    59,   515,   516,   517,   518,    60,   539,   519,   527,
     528,   535,   556,   557,   558,   559,  1053,  1054,  1055,  1056,
    1057,  1058,  1059,  1060,   560,   221,   561,   562,   563,   564,
     565,   566,   567,   568,   569,  1063,  1064,  1065,   570,   571,
     572,   227,   573,   574,   587,   597,   575,   588,   589,   598,
     599,   854,   600,   610,   605,   858,   859,   860,   619,   620,
     622,   309,   670,   623,   624,   625,   626,   627,   628,   661,
     631,   663,   672,   673,   632,   689,   878,   656,   657,   763,
     880,   764,     4,     5,     6,     7,   662,   668,   664,   346,
     347,   348,   349,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   350,   351,    24,   669,   721,    25,    26,
      27,    28,   352,   353,    31,    32,   723,   726,   728,   729,
      33,    34,    35,    36,    37,    38,   354,   355,    41,    42,
      43,    44,    45,    46,    47,   747,   751,   754,    48,    49,
     755,   765,    50,    51,   766,    52,   767,   930,   931,    53,
     786,   788,   787,   790,   799,   804,   805,   806,   833,   835,
     836,   810,   837,   839,   841,   843,   844,   845,   851,   852,
     879,   908,   881,   903,    54,    55,   904,   909,   910,   922,
     914,   915,   923,   932,   933,   949,   946,   955,    56,   953,
     954,   957,   958,   976,   885,    57,   959,   971,   972,   981,
     989,    58,    59,  1001,  1002,  1003,  1004,    60,   356,   993,
    1005,  1006,  1007,  1008,  1011,  1012,  1009,  1013,  1020,  1017,
    1022,  1038,  1039,  1040,  1041,  1042,  1043,   674,   719,  1046,
     607,   392,   398,     0,   399,     0,     0,     0,   990,     0,
       0,     0,    75,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   651,   652,    24,     0,  1021,    25,    26,    27,
      28,    29,   653,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,    48,    49,     0,
       0,    50,    51,     0,    52,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1062,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
      58,    59,     4,     5,     6,     7,    60,   654,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   994,   995,    24,     0,     0,    25,    26,
      27,    28,    29,   996,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,    50,    51,     0,    52,     0,     0,     0,    53,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,   997,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,   144,    59,    46,     0,     0,     0,    60,   773,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,   144,    59,     4,     5,     6,     7,    60,  1026,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
      26,    27,    28,    29,    30,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,    50,    51,     0,    52,     0,     0,     0,
      53,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,    58,    59,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,   119,   120,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,   144,    59,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   183,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   246,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,   250,
       0,    41,    42,    43,     0,    59,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   258,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,   326,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   265,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,   185,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   530,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   576,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     579,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,   592,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   590,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,   457,     7,     0,     0,     0,   118,
     111,   112,   113,   458,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   593,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   183,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,   592,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   716,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   718,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   912,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   130,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   132,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   134,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   136,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,    59,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   151,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   153,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,    59,    46,     0,     0,
       0,   167,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   248,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,   457,     7,
       0,     0,     0,   118,   111,   112,   113,   458,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   455,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   459,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   612,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   110,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   637,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,     0,    59,     0,     0,     0,
       0,    60
};

static const yytype_int16 yycheck[] =
{
       2,     0,   279,   237,   211,   634,   488,    15,    26,    14,
      26,   108,    10,   793,   339,    10,    14,    22,   108,    14,
      22,    23,    24,    25,    26,    10,   123,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    77,    14,    10,
       0,    43,    50,    14,    46,    37,    38,    10,    56,    57,
     108,    14,    26,   110,    10,    29,    30,    59,    14,    10,
      11,    12,    36,    14,    74,    39,    40,    74,   126,    43,
      44,   128,    14,    26,    56,    57,    29,    30,    60,    14,
      31,   112,   113,    36,    35,   125,    39,    40,   128,    31,
      43,    44,    14,    95,    96,    97,    98,    99,   100,    14,
      14,   103,   104,    17,   106,   107,   110,    22,    56,    57,
     128,   121,    60,   123,   121,   128,   123,   110,   120,    26,
      10,    26,   102,   128,   128,   905,   124,    62,   130,   109,
     132,    77,   134,   762,   136,   128,    43,   129,    43,   124,
     120,    26,    12,   468,    58,   124,   124,   129,   122,   151,
      65,   153,   128,   124,   156,   129,   158,   159,    43,   161,
     162,   124,   164,   165,   166,   167,   168,   947,   124,   122,
      12,   951,   123,    71,    72,    73,   129,    67,    68,   125,
      10,   129,   128,   128,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   108,   196,   197,   198,   199,   200,   201,
     202,   203,   527,   528,    75,    76,    14,   122,    14,   225,
     226,    26,   992,    77,    22,   122,    22,   122,   125,    14,
    1000,   498,   129,   231,   232,   233,   234,   235,    43,    77,
      29,   129,    26,    95,    96,  1015,   110,   122,    10,   108,
     125,    75,    76,   114,   129,    44,   248,    14,   525,    43,
     114,   108,    10,   122,   128,    22,    26,    65,   260,    65,
     262,   123,   264,   106,    26,  1045,   114,   269,   270,   126,
     272,   122,   274,    43,   110,   115,   116,   759,   129,   122,
     114,    43,   125,   285,   286,   287,   288,   289,   290,   291,
     306,   307,   128,   295,   296,    67,    68,     5,    65,    10,
     108,   930,   108,    14,     2,   108,    14,   122,    95,    96,
     309,   313,   314,   293,   122,   317,   122,   319,   298,   108,
     123,   108,   324,   325,   304,   122,   106,    26,   122,    27,
      28,   125,   129,   122,   122,   129,   123,   125,   615,    37,
      38,   108,   122,   108,    43,   125,    44,    45,   350,   351,
     352,   353,   122,   122,   103,   122,   125,   122,    10,   129,
     122,    14,    14,   108,   344,   572,    14,   129,   117,   118,
     119,    26,   649,   108,    29,    30,    26,   122,   108,    29,
      30,    36,   124,   125,    39,    40,    36,   122,    43,    39,
      40,   122,   122,    43,   122,   122,    26,    69,   129,    29,
      30,   129,   129,   680,   681,   121,    36,   123,    26,    39,
      40,    29,    30,    43,    44,   102,   114,   122,    36,   122,
     123,    39,    40,   122,   129,    43,   125,   429,   123,   122,
     105,   433,   121,   128,   436,   437,   129,   135,   122,   137,
     138,   675,   422,    10,   122,   129,    13,    14,    15,    16,
     430,   129,   122,   455,   152,   435,   154,   459,   438,   129,
     462,   107,   464,   443,   466,   445,   122,   122,   470,   471,
     125,   122,   122,   129,   129,   125,   753,   122,   129,   129,
     123,    14,   122,   122,   129,   128,   122,   489,   186,   129,
     129,   123,   122,   129,   474,   122,   128,   121,   500,   129,
     123,   122,   129,   483,   122,   128,    26,   125,   129,    29,
      30,   129,   514,   123,   122,   122,    36,   122,   128,    39,
      40,   129,   129,    43,   129,   802,   803,   122,   122,   122,
     122,   533,   534,   535,   129,   129,   129,   129,   122,   541,
     542,   125,   544,   545,    14,   547,   548,   549,   550,   551,
     552,   553,   554,   123,   115,   116,   122,    26,   128,   884,
      29,    30,   887,   129,    10,   799,   122,    36,    14,   122,
      39,    40,   125,   129,    43,    44,    82,    83,    84,    85,
      86,    87,   122,   122,   122,   587,   588,   589,    94,   129,
     129,   129,   122,   573,   122,   575,   122,   125,   104,   129,
     602,   581,   122,   605,   606,   125,   122,   122,    26,   122,
     612,    29,    30,   129,   129,    14,   129,   619,    36,   126,
     122,    39,    40,    14,   604,    43,    44,   129,    10,    78,
      79,    80,    81,   122,   122,   637,   122,   125,   336,    88,
      89,    90,   122,   129,    14,   125,   124,   125,   973,   651,
     652,   653,    22,   122,    24,    25,   658,   122,   122,   122,
     125,   110,   111,    33,    34,   129,    39,    40,   670,   122,
     108,    41,    42,   124,   125,   122,   129,   122,   122,    49,
      50,    51,   129,   122,   129,   129,   122,   108,    58,   125,
     129,   693,   694,   695,   696,   697,   698,   122,   128,   125,
     125,   127,    75,    76,   122,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,   122,   122,
     122,    94,    10,    11,    12,   129,   129,   129,   122,   102,
     103,   104,   105,   735,   107,   129,   122,   110,   111,   112,
     113,   114,   115,   129,   117,   118,   119,    53,   121,     7,
       8,     9,   128,   205,   206,   207,   208,   209,   210,   739,
     212,   213,   214,   215,   216,   217,   218,   219,   123,   125,
     750,   127,   129,   775,   776,   777,   778,   779,   780,   781,
     782,   222,   223,   224,   122,   125,   128,   127,   790,   487,
     488,   129,   122,   491,   492,   493,    14,   122,   125,    17,
     127,   122,   500,   122,    22,   807,    24,    25,   124,   125,
     508,   509,   510,   124,   125,    33,    34,   124,   125,    14,
     124,   125,   122,    41,    42,   124,   125,    22,   808,    24,
      25,    49,    50,    51,    27,    28,    27,    28,    33,    34,
     124,   125,   844,   845,   124,   125,    41,    42,   124,   125,
     124,   125,   124,   125,    49,    50,    51,   124,   125,   124,
     125,   122,   864,   865,   866,   111,   846,   129,    10,   122,
     850,    13,   123,    15,    16,   124,   125,   124,   125,   881,
     124,   125,   124,   125,   124,   125,   885,   124,   125,    14,
     892,     3,     4,     5,     6,   897,   124,   125,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   124,
     125,   123,   914,   915,   124,   125,   124,   125,    14,    31,
      32,   923,   124,   125,   124,   125,   124,   125,    93,    41,
      95,    96,    97,    98,    99,   100,   101,    49,    50,    51,
     638,   122,    54,   108,   124,   125,   124,   125,   950,   124,
     125,    63,    64,   108,    66,   108,   121,    10,   123,    27,
      28,   659,   964,   124,   125,   124,   125,   969,   124,   125,
      93,    14,    95,    96,    97,    98,    99,   100,   101,   124,
     125,   124,   125,    95,    96,   108,    14,   989,    27,    28,
     124,   125,   994,   995,   996,   124,   125,   109,   121,    10,
     123,    27,   124,   125,   116,    42,    43,   229,   230,   122,
     122,   123,   122,   122,   122,   122,   128,   129,   129,   123,
     123,   108,   122,   122,   122,   122,  1028,  1029,  1030,  1031,
    1032,  1033,  1034,  1035,   122,   104,   122,   122,   122,   122,
     122,   122,   122,   122,   122,  1047,  1048,  1049,   122,   122,
      91,   114,   123,    14,   108,   111,   123,   108,   108,   129,
     122,   759,   122,   122,    74,   763,   764,   765,   108,    10,
     122,   128,   108,   122,   122,   122,   122,   122,   122,   111,
     122,   111,    14,    14,   122,    14,   784,   122,   122,    10,
     788,    10,     3,     4,     5,     6,   129,   122,   129,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   122,   122,    29,    30,
      31,    32,    33,    34,    35,    36,   122,   122,   127,   127,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,   122,   122,   122,    59,    60,
     122,    10,    63,    64,    10,    66,    10,   855,   856,    70,
     122,    27,   122,   122,    56,    14,    14,    14,   122,   122,
     122,   129,   122,    14,   122,   122,    74,    74,   122,   122,
     129,     7,   122,   122,    95,    96,   122,     7,     7,   122,
     108,   108,    74,    10,    10,    69,   124,   122,   109,    14,
      14,    14,    14,   129,   128,   116,   122,   122,   122,   122,
     122,   122,   123,   122,   122,   122,   122,   128,   129,   129,
     122,   122,    14,    14,    14,    14,   122,   122,   122,   129,
     989,   122,   122,   122,   122,   122,   122,   523,   575,   124,
     459,   221,   227,    -1,   228,    -1,    -1,    -1,   946,    -1,
      -1,    -1,   950,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,   984,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1046,    -1,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
     122,   123,     3,     4,     5,     6,   128,   129,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    70,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,
      51,   122,   123,    54,    -1,    -1,    -1,   128,   129,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,    50,
      51,   122,   123,    54,    -1,    -1,    -1,   128,   129,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   122,   123,     3,     4,     5,     6,   128,   129,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      70,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,
      50,    51,   122,   123,    54,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,
      50,    51,   122,   123,    54,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    41,    -1,    -1,    -1,    -1,   116,    -1,    -1,    49,
      50,    51,   122,   123,    54,    -1,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      49,    50,    51,   123,   124,    54,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    49,    50,    51,   123,   124,    54,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,   117,
      -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    49,    50,    51,   123,   124,    54,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    23,    95,    96,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    41,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    49,    50,    51,   123,   124,    54,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    69,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,
     116,    -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,
      66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    95,
      96,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   109,    -1,    -1,    41,    -1,    -1,    -1,
     116,    -1,    -1,    -1,    49,    50,    51,   123,   124,    54,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,    -1,
      -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,   124,
      54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,    -1,
      -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,   123,
     124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    41,
      -1,    -1,    -1,   116,    -1,    -1,    -1,    49,    50,    51,
     123,   124,    54,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      41,    -1,    -1,    -1,   116,    -1,    -1,    -1,    49,    50,
      51,   123,   124,    54,    -1,    -1,   128,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,    41,    -1,    -1,    -1,   116,    -1,    -1,    -1,    49,
      50,    51,   123,   124,    54,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      49,    50,    51,   123,   124,    54,    -1,    -1,   128,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,    -1,
      -1,    49,    50,    51,   123,   124,    54,    -1,    -1,   128,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    95,    96,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    41,    -1,    -1,    -1,   116,    -1,
      -1,    -1,    49,    50,    51,   123,   124,    54,    -1,    -1,
     128,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    41,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    49,    50,    51,    -1,   123,    54,    -1,    -1,
      -1,   128,    -1,    -1,    -1,    -1,    63,    64,    -1,    66,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,   128
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   131,   132,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    66,    70,    95,    96,   109,   116,   122,   123,
     128,   133,   134,   136,   137,   138,   139,   140,   142,   143,
     145,   146,   149,   150,   153,   158,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   175,
     176,   178,    93,    95,    96,    97,    98,    99,   100,   101,
     108,   121,   123,   108,   126,   110,   128,   108,   121,   123,
      10,    11,    12,    13,    31,   158,   171,    14,    10,   122,
     123,   160,   177,    10,    11,    12,    14,    31,    35,   123,
     123,   160,   123,   160,   123,   160,   123,   160,   160,   167,
      14,   158,   158,   160,   122,   160,    10,    14,   160,    12,
      12,   123,   160,   123,   160,   158,   123,   158,    26,    43,
     122,    26,    43,   122,   128,   159,   159,   128,   159,   160,
     158,   158,   160,    14,    14,    14,    31,   171,    14,    10,
      10,   171,   171,   124,   160,    69,   132,    26,    29,    30,
      36,    39,    40,    43,    44,   122,    26,    29,    30,    36,
      39,    40,    43,    44,   122,    78,    79,    80,    81,    88,
      89,    90,   110,   111,    82,    83,    84,    85,    86,    87,
      94,   104,   112,   113,   102,    75,    76,   114,   105,   115,
     116,   103,   117,   118,   119,   107,   121,   121,    14,   160,
     160,   160,   160,   160,   160,    14,   124,   177,   123,   160,
     117,   160,    14,   160,   160,    14,    17,    58,   124,   177,
     126,   110,   128,   122,   108,   124,   160,   177,   122,   125,
     108,   122,   108,   122,   108,   122,    10,    11,    12,    14,
      10,    14,    10,   173,   160,    26,    29,    30,    36,    39,
      40,    43,   122,   125,   160,    26,    43,   122,   125,   160,
     158,   160,   158,    77,   125,   158,    75,    76,   123,   128,
     147,   122,   122,   108,   108,   128,   110,   128,   110,   128,
     160,   158,   160,   158,    29,    44,    23,   160,   160,   160,
     160,   160,   160,   160,   160,   160,    53,   151,   128,   123,
      14,    62,   128,   124,   125,   129,    10,    11,    12,    13,
      24,    25,    33,    34,    47,    48,   129,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   163,   163,   163,   163,   163,   163,
     161,   163,   163,   163,   163,   163,   163,   163,   163,     7,
       8,     9,   164,   166,   166,   166,   167,   167,   168,   169,
     170,   170,   171,   171,   171,   171,   171,    14,    22,    24,
      25,    33,    34,    41,    42,    49,    50,    51,   174,    17,
     174,   122,   123,   122,   122,   122,   122,   122,   122,   108,
     123,   124,   160,   106,   122,   125,   115,   116,   125,   127,
     111,   129,   122,   123,    14,   123,   124,   160,    14,   160,
     160,   122,   124,   160,   160,   123,   160,     5,    14,   123,
     156,   160,   108,   122,   108,   122,   108,   122,   123,   147,
     108,   108,   124,   125,   125,   160,   160,   160,   160,   160,
     160,   160,   177,   125,   160,   160,   177,   124,    27,    28,
     135,   124,    10,   177,   167,   167,    10,    14,   124,   157,
     132,   160,   160,   144,    14,   160,    14,   160,   124,    27,
     124,   160,   160,    10,   122,   122,   122,   122,   122,   129,
     158,    14,    22,   128,   152,   124,   157,   123,   123,   154,
     124,   177,   121,   126,   128,   108,   160,   160,   160,   129,
     160,    26,    43,   129,    26,    43,   129,    26,    29,    30,
      36,    39,    40,    43,    44,   129,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,    91,   123,    14,   123,   124,   177,   160,   124,
     177,   125,   160,   177,   160,   160,   177,   108,   108,   108,
     124,   177,    14,   124,   155,   177,   127,   111,   129,   122,
     122,   160,   106,   122,   125,    74,    74,   156,   122,   125,
     122,   160,   123,   160,   160,   124,   157,   160,   160,   108,
      10,   177,   122,   122,   122,   122,   122,   122,   122,   122,
     177,   122,   122,   122,   158,   134,   158,   123,   160,   158,
     158,    77,   158,    77,    77,    10,    67,    68,   147,   124,
     125,    24,    25,    34,   129,   160,   122,   122,    37,    38,
     129,   111,   129,   111,   129,   158,   158,   158,   122,   122,
     108,   160,    14,    14,   152,    56,    57,    60,   129,   147,
     124,   124,   157,   157,    71,    72,    73,   129,   124,    14,
     160,   160,   160,    26,    30,    36,    39,    40,    43,   129,
     129,   129,   129,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   161,   124,   177,   124,   155,
     177,   122,   124,   122,   124,   177,   122,   122,   127,   127,
     127,   160,   160,   160,   124,    74,   124,   125,   124,   125,
     160,   177,   160,   160,   124,     5,    14,   122,   160,   122,
     125,   122,   147,   124,   122,   122,   160,   124,   124,    27,
     135,   160,   158,    10,    10,    10,    10,    10,   147,    10,
      14,   160,   160,   129,   160,    26,    29,    30,    36,    39,
      40,    43,    44,   129,   160,   158,   122,   122,    27,   160,
     122,   129,    58,   174,    10,    13,    14,    15,    16,    56,
     147,   147,   124,   124,    14,    14,    14,   108,   123,   127,
     129,   129,   160,   160,   160,   160,   160,   160,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     124,   124,   124,   122,   124,   122,   122,   122,   160,    14,
     177,   122,   122,   122,    74,    74,   125,   177,   147,   122,
     125,   122,   122,   134,   158,   124,    27,   135,   158,   158,
     158,    10,    67,    68,    26,    30,    43,   129,   129,   129,
     160,   160,   160,   160,   160,   160,   160,   160,   158,   129,
     158,   122,   141,   160,   123,   128,   148,   123,   148,    14,
      22,    65,   108,   122,    14,    22,    65,   108,   122,    10,
      13,    15,    16,   122,   122,   174,   147,   147,     7,     7,
       7,   160,   124,   177,   108,   108,   129,   129,   129,   129,
     129,   129,   122,    74,   124,   160,   160,   177,   122,   177,
     158,   158,    10,    10,   160,   160,   160,   129,   129,   129,
     129,   129,   129,   129,   129,   160,   124,   124,   157,    69,
     132,   124,   157,    14,    14,   122,   160,    14,    14,   122,
     160,    14,    22,    65,   108,   122,    14,    22,    65,   108,
     122,   122,   122,   123,   148,   129,   129,   124,   160,   160,
     160,   122,   124,   122,    27,   135,   129,   129,   129,   122,
     158,   148,   124,   129,    24,    25,    34,   129,   160,   148,
     124,   122,   122,   122,   122,   122,   122,    14,    14,   122,
     160,    14,    14,   122,   160,   124,   157,   129,   129,   129,
     122,   158,   141,   148,   160,   160,   129,   160,    26,    29,
      30,    36,    39,    40,    43,    44,   129,   148,   122,   122,
     122,   122,   122,   122,   148,   124,   124,    26,    30,    43,
     129,   129,   129,   160,   160,   160,   160,   160,   160,   160,
     160,   148,   158,   160,   160,   160,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   130,   131,   132,   132,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   134,   134,   134,   134,   134,   134,
     134,   134,   135,   135,   135,   135,   135,   135,   136,   136,
     137,   137,   137,   137,   138,   138,   139,   139,   139,   140,
     140,   140,   141,   142,   142,   142,   142,   142,   142,   143,
     143,   144,   144,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   146,   146,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   149,   150,   151,   151,   151,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     153,   154,   154,   154,   154,   155,   155,   156,   156,   156,
     156,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   159,   160,   160,   160,   161,   161,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   163,   163,   164,
     164,   164,   165,   165,   166,   166,   166,   167,   167,   168,
     168,   169,   169,   169,   170,   170,   170,   170,   170,   171,
     171,   171,   171,   171,   172,   172,   173,   173,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   175,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   177,   177,   177,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     4,     6,     6,     8,     5,
       5,     7,     6,     6,     8,    10,     6,     4,     4,     4,
       3,     5,     5,     6,     6,     2,     5,     4,     3,     3,
       5,     7,     3,     5,     7,     3,     3,     2,     4,     7,
       6,     4,     6,     4,     2,     4,     4,     4,     4,     4,
       4,     4,     4,     7,     7,     7,     6,     6,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     1,     1,     2,     4,     2,     1,
       1,     1,     1,     1,     2,     2,     4,     4,     4,     4,
       1,     1,     1,     1,     5,     7,     7,     3,     5,     5,
       4,     6,     3,     5,     5,     7,     4,     6,     5,     3,
       5,     7,     3,     5,     5,     3,     2,     5,     5,     2,
      12,     9,     1,     7,     7,     5,     7,     5,     3,     5,
       7,     0,     4,     6,     5,     3,     7,     6,     4,     7,
       6,     7,     6,     5,     6,     5,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     3,     3,     4,     5,     4,     5,     5,
       6,     6,     6,     6,     6,     6,     6,     6,     7,     7,
       7,     6,     5,     0,     3,     3,     0,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       6,     6,     6,     5,     5,     6,     6,     7,     7,     7,
       7,     7,     6,     4,     7,     6,     4,     8,     7,     5,
       5,     0,     4,     4,     4,     3,     5,     3,     3,     5,
       5,     1,     2,     3,     3,     3,     4,     5,     5,     3,
       3,     4,     6,     6,     6,     6,     6,     6,     6,     6,
       5,     7,     7,     7,     7,     7,     7,     5,     9,     8,
       8,     6,     9,     9,     5,     5,     4,     4,     4,     6,
       6,     6,     6,     3,     3,     2,     1,     5,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       2,     3,     3,     2,     3,     1,     1,     3,     2,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     4,     6,     6,     6,     1,     4,
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
#line 384 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2827 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 394 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2833 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 395 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2839 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 399 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2845 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 401 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2851 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 403 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2857 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 405 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2865 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 409 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2871 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2877 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2883 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 415 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2891 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 419 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2898 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 422 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2906 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 426 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2912 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 428 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2920 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 432 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2927 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 435 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2935 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 439 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2941 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 441 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2947 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 443 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2953 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 445 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2959 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 447 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2965 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 449 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2973 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 453 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2981 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 457 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2987 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 459 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2993 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 461 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2999 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 463 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3005 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 465 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3011 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 467 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3017 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 469 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3023 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 471 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3029 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 473 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3035 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 475 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3041 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 477 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3047 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 479 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3053 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 481 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3059 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 483 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3065 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 485 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3072 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 488 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3079 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 491 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3085 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 493 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3092 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 496 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3099 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 499 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3105 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 501 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3111 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 503 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3117 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 505 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 3123 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 507 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3132 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 512 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3140 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 516 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3148 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 520 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3157 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 525 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3165 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 528 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3171 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 530 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3177 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 532 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3183 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 534 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3189 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 536 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3195 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 538 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3202 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 541 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3208 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 543 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3214 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 545 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3220 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3227 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 550 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3234 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 553 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3241 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 556 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3248 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 559 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3255 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 562 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3261 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 564 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3267 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 566 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3273 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 568 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3279 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 570 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3286 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 573 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3292 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 575 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3298 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 577 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3304 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 579 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3311 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 582 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3318 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 585 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3326 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 589 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3333 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 592 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3339 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 594 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3345 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 596 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3351 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 598 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3358 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 601 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3365 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 604 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3372 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 607 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3379 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 610 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3386 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 613 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3393 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 616 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3400 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 618 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3406 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 619 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3412 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 620 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3418 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 621 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3424 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 622 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3430 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 623 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3436 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 625 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3442 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 627 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3448 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 629 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3454 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 631 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3460 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 632 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3466 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 633 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3472 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 634 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3478 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 635 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3484 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 636 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3490 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 637 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3496 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 639 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3502 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 641 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3508 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 643 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3514 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 645 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3520 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 646 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3526 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 647 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3532 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 648 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3538 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 649 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3544 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 653 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3550 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 655 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3556 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3562 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 659 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3568 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3574 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3580 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3586 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3592 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3598 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 673 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3604 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 675 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3610 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 677 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3616 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 679 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3622 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3628 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 685 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3634 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 687 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3640 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 691 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3646 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3652 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3658 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 697 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3664 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3676 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 707 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3682 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 709 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3688 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 711 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3694 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 715 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3700 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 717 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3706 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 719 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3712 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 722 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3718 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 726 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3728 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 732 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3738 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 738 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3747 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 743 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3757 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 749 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3766 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 754 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3773 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 759 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3785 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 767 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3798 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 777 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3804 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 779 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3812 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 785 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3824 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 793 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3834 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 799 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3844 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 805 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3856 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 813 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3866 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 819 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3876 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 825 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3889 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 834 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3900 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 841 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3913 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 850 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3926 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 859 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3937 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 868 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3948 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 875 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3957 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 881 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3963 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 883 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3970 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 886 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3977 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 889 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3983 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 891 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3989 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 893 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3995 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4001 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 897 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4007 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 899 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4013 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 901 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4019 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 903 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4026 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 906 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4032 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 908 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4038 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 910 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4044 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 912 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4051 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 915 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4058 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 918 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4066 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 923 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4072 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 924 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4078 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 926 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4085 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 929 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4092 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 932 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4098 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 934 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4104 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 936 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4110 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 938 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4116 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 940 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4122 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 942 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4128 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 944 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4134 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 946 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4141 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 949 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4147 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 951 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4153 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 953 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4159 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 955 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4166 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 958 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4173 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 961 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4181 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 967 "raku.y"
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
#line 4199 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 983 "raku.y"
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
#line 4216 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 997 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4222 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 999 "raku.y"
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
#line 4238 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1011 "raku.y"
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
#line 4254 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1024 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4260 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1026 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4267 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1029 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4274 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1032 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4281 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1035 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4288 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1038 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4295 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1041 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4302 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1044 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4309 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1047 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4316 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1050 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4323 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1053 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4330 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1056 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4337 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1059 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4344 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1062 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4355 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1069 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4366 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1076 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4377 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1083 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4388 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1090 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4396 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1094 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4404 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1098 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4412 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1102 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4420 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1106 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4431 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1113 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4442 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1120 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4453 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1127 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4464 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1134 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4478 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1144 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4490 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1152 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4502 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1160 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4515 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1169 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4526 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1176 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4537 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1183 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4551 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1193 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4563 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1201 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4575 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1211 "raku.y"
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
#line 4592 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1225 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4598 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1227 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4607 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1232 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4616 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1237 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4625 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1244 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4633 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1248 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4641 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1254 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4647 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1256 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4653 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1258 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4659 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1260 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4665 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1263 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4671 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1264 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4677 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1265 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4683 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1266 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4689 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1267 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4695 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1268 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4701 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1269 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4707 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1270 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4713 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list '}'  */
#line 1273 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4719 "raku.tab.c"
    break;

  case 260: /* block: '{' YADA '}'  */
#line 1274 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4725 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr '}'  */
#line 1276 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4731 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1278 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4737 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1280 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4743 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1282 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4749 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1284 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4755 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1286 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4762 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1289 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4768 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1291 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4774 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1293 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4780 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1295 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4786 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1297 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4793 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1300 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4800 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1303 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4808 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1307 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4815 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1310 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4822 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1313 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4829 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1316 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4835 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1318 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4843 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1322 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4850 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1325 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4857 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1328 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4864 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1331 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4871 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1334 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4878 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1337 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4884 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1339 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4890 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1341 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4896 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_LAST '}'  */
#line 1343 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4902 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1345 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4908 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1347 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4914 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1349 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4920 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1351 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4926 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1353 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4932 "raku.tab.c"
    break;

  case 293: /* closure: '{' expr '}'  */
#line 1356 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4938 "raku.tab.c"
    break;

  case 294: /* expr: VAR_SCALAR '=' expr  */
#line 1359 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4944 "raku.tab.c"
    break;

  case 295: /* expr: KW_GATHER block  */
#line 1360 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4954 "raku.tab.c"
    break;

  case 296: /* expr: tern_expr  */
#line 1365 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4960 "raku.tab.c"
    break;

  case 297: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1369 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4966 "raku.tab.c"
    break;

  case 298: /* tern_expr: cmp_expr  */
#line 1370 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4972 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1373 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4978 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1374 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4984 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1375 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4990 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1376 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4996 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1377 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5002 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1378 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5008 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1379 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5014 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1380 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5020 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1381 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5026 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1382 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5032 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1383 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5038 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1384 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5044 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1385 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5050 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1386 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5056 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1388 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5066 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1394 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5076 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1400 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5086 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr  */
#line 1405 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5092 "raku.tab.c"
    break;

  case 317: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1408 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5098 "raku.tab.c"
    break;

  case 318: /* divis_expr: jct_expr  */
#line 1409 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5104 "raku.tab.c"
    break;

  case 319: /* jct_expr: jct_expr '|' range_expr  */
#line 1412 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5110 "raku.tab.c"
    break;

  case 320: /* jct_expr: jct_expr '&' range_expr  */
#line 1413 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5116 "raku.tab.c"
    break;

  case 321: /* jct_expr: dor_expr  */
#line 1414 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5122 "raku.tab.c"
    break;

  case 322: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1418 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5128 "raku.tab.c"
    break;

  case 323: /* dor_expr: range_expr  */
#line 1419 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5134 "raku.tab.c"
    break;

  case 324: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1422 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5140 "raku.tab.c"
    break;

  case 325: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1423 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5146 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr  */
#line 1424 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5152 "raku.tab.c"
    break;

  case 327: /* add_expr: add_expr '~' repl_expr  */
#line 1427 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5158 "raku.tab.c"
    break;

  case 328: /* add_expr: repl_expr  */
#line 1428 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5164 "raku.tab.c"
    break;

  case 329: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1431 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5170 "raku.tab.c"
    break;

  case 330: /* repl_expr: addsub_expr  */
#line 1432 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5176 "raku.tab.c"
    break;

  case 331: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1435 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5182 "raku.tab.c"
    break;

  case 332: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1436 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5188 "raku.tab.c"
    break;

  case 333: /* addsub_expr: mul_expr  */
#line 1437 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5194 "raku.tab.c"
    break;

  case 334: /* mul_expr: mul_expr '*' unary_expr  */
#line 1440 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5200 "raku.tab.c"
    break;

  case 335: /* mul_expr: mul_expr '/' unary_expr  */
#line 1441 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5206 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '%' unary_expr  */
#line 1442 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5212 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1443 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5218 "raku.tab.c"
    break;

  case 338: /* mul_expr: unary_expr  */
#line 1444 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5224 "raku.tab.c"
    break;

  case 339: /* unary_expr: '-' unary_expr  */
#line 1447 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5230 "raku.tab.c"
    break;

  case 340: /* unary_expr: '!' unary_expr  */
#line 1448 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5236 "raku.tab.c"
    break;

  case 341: /* unary_expr: CARET unary_expr  */
#line 1449 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5242 "raku.tab.c"
    break;

  case 342: /* unary_expr: OP_REDUCE unary_expr  */
#line 1451 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5251 "raku.tab.c"
    break;

  case 343: /* unary_expr: pow_expr  */
#line 1455 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5257 "raku.tab.c"
    break;

  case 344: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1458 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5263 "raku.tab.c"
    break;

  case 345: /* pow_expr: postfix_expr  */
#line 1459 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5269 "raku.tab.c"
    break;

  case 346: /* scalar_list: VAR_SCALAR  */
#line 1462 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5275 "raku.tab.c"
    break;

  case 347: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1463 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5281 "raku.tab.c"
    break;

  case 348: /* meth_name: IDENT  */
#line 1466 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5287 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_SORT  */
#line 1467 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5293 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_MAP  */
#line 1468 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5299 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_GREP  */
#line 1469 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5305 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_SAY  */
#line 1470 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5311 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_PRINT  */
#line 1471 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5317 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_TAKE  */
#line 1472 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5323 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_RETURN  */
#line 1473 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5329 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_EXISTS  */
#line 1474 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5335 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_DELETE  */
#line 1475 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5341 "raku.tab.c"
    break;

  case 358: /* meth_name: TESTOP  */
#line 1476 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5347 "raku.tab.c"
    break;

  case 359: /* postfix_expr: call_expr  */
#line 1478 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5353 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '(' arg_list ')'  */
#line 1481 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5362 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '(' ')'  */
#line 1485 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5368 "raku.tab.c"
    break;

  case 362: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1487 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5377 "raku.tab.c"
    break;

  case 363: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1492 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5383 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1494 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5393 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1500 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5403 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1506 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5411 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' KW_NEW  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5419 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' IDENT  */
#line 1514 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5428 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1519 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5439 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1526 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5448 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' CARET IDENT  */
#line 1531 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5458 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' CARET IDENT  */
#line 1537 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5468 "raku.tab.c"
    break;

  case 373: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1543 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5479 "raku.tab.c"
    break;

  case 374: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5490 "raku.tab.c"
    break;

  case 375: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1557 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5499 "raku.tab.c"
    break;

  case 376: /* call_expr: atom '.' meth_name  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5508 "raku.tab.c"
    break;

  case 377: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1567 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5519 "raku.tab.c"
    break;

  case 378: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1574 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5528 "raku.tab.c"
    break;

  case 379: /* call_expr: call_expr '.' meth_name  */
#line 1579 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5537 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_DIE expr  */
#line 1584 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5543 "raku.tab.c"
    break;

  case 381: /* call_expr: KW_MAP closure expr  */
#line 1586 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5549 "raku.tab.c"
    break;

  case 382: /* call_expr: KW_GREP closure expr  */
#line 1588 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5555 "raku.tab.c"
    break;

  case 383: /* call_expr: KW_SORT expr  */
#line 1590 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5561 "raku.tab.c"
    break;

  case 384: /* call_expr: KW_SORT closure expr  */
#line 1592 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5567 "raku.tab.c"
    break;

  case 385: /* call_expr: atom  */
#line 1593 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5573 "raku.tab.c"
    break;

  case 386: /* arg_list: expr  */
#line 1596 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5579 "raku.tab.c"
    break;

  case 387: /* arg_list: arg_list ',' expr  */
#line 1597 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5585 "raku.tab.c"
    break;

  case 388: /* arg_list: arg_list ','  */
#line 1598 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5591 "raku.tab.c"
    break;

  case 389: /* atom: LIT_INT  */
#line 1601 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5597 "raku.tab.c"
    break;

  case 390: /* atom: LIT_FLOAT  */
#line 1602 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5603 "raku.tab.c"
    break;

  case 391: /* atom: LIT_STR  */
#line 1603 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5609 "raku.tab.c"
    break;

  case 392: /* atom: WORDLIST  */
#line 1605 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5621 "raku.tab.c"
    break;

  case 393: /* atom: LIT_INTERP_STR  */
#line 1612 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5627 "raku.tab.c"
    break;

  case 394: /* atom: VAR_SCALAR  */
#line 1613 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5633 "raku.tab.c"
    break;

  case 395: /* atom: OP_INC VAR_SCALAR  */
#line 1614 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5639 "raku.tab.c"
    break;

  case 396: /* atom: OP_DEC VAR_SCALAR  */
#line 1615 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5645 "raku.tab.c"
    break;

  case 397: /* atom: VAR_SCALAR OP_INC  */
#line 1616 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5651 "raku.tab.c"
    break;

  case 398: /* atom: VAR_SCALAR OP_DEC  */
#line 1617 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5657 "raku.tab.c"
    break;

  case 399: /* atom: VAR_ARRAY  */
#line 1618 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5663 "raku.tab.c"
    break;

  case 400: /* atom: VAR_HASH  */
#line 1619 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5669 "raku.tab.c"
    break;

  case 401: /* atom: VAR_CAPTURE  */
#line 1621 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5677 "raku.tab.c"
    break;

  case 402: /* atom: VAR_FH  */
#line 1625 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5685 "raku.tab.c"
    break;

  case 403: /* atom: VAR_NAMED_CAPTURE  */
#line 1629 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5692 "raku.tab.c"
    break;

  case 404: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1632 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5698 "raku.tab.c"
    break;

  case 405: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1634 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5704 "raku.tab.c"
    break;

  case 406: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1636 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5710 "raku.tab.c"
    break;

  case 407: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1638 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5716 "raku.tab.c"
    break;

  case 408: /* atom: ARR_ALL_SLICE  */
#line 1640 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5722 "raku.tab.c"
    break;

  case 409: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1642 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5728 "raku.tab.c"
    break;

  case 410: /* atom: VAR_HASH '{' expr '}'  */
#line 1644 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5734 "raku.tab.c"
    break;

  case 411: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1646 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5740 "raku.tab.c"
    break;

  case 412: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1648 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5746 "raku.tab.c"
    break;

  case 413: /* atom: IDENT  */
#line 1649 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5752 "raku.tab.c"
    break;

  case 414: /* atom: VAR_TWIGIL  */
#line 1651 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5760 "raku.tab.c"
    break;

  case 415: /* atom: VAR_ARRAY_TWIGIL  */
#line 1655 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5768 "raku.tab.c"
    break;

  case 416: /* atom: VAR_HASH_TWIGIL  */
#line 1659 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5776 "raku.tab.c"
    break;

  case 417: /* atom: '(' ')'  */
#line 1662 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5782 "raku.tab.c"
    break;

  case 418: /* atom: '(' expr ')'  */
#line 1663 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5788 "raku.tab.c"
    break;

  case 419: /* atom: '(' expr ',' ')'  */
#line 1665 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5794 "raku.tab.c"
    break;

  case 420: /* atom: '(' expr ',' arg_list ')'  */
#line 1667 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5801 "raku.tab.c"
    break;

  case 421: /* atom: block  */
#line 1669 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5807 "raku.tab.c"
    break;

  case 422: /* atom: KW_SUB block  */
#line 1670 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5813 "raku.tab.c"
    break;


#line 5817 "raku.tab.c"

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

#line 1672 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
