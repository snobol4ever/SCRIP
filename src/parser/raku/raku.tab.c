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

#line 356 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 64,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 65,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 66,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 67,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 68,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 69,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 70,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 71,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 72,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 73,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 74,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 75,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 76,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 77,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 78,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 79,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 80,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 81,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 82,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 83,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 84,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 85,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 86,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 87,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 88,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 89,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 90,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 91,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 92,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 93,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 94,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 95,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 96,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 97,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 98,                 /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 99,                 /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 100,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 101,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 102,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 103,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 104,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 105,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 106,                   /* OP_POW  */
  YYSYMBOL_107_ = 107,                     /* '='  */
  YYSYMBOL_108_ = 108,                     /* '!'  */
  YYSYMBOL_109_ = 109,                     /* '<'  */
  YYSYMBOL_110_ = 110,                     /* '>'  */
  YYSYMBOL_111_ = 111,                     /* '|'  */
  YYSYMBOL_112_ = 112,                     /* '&'  */
  YYSYMBOL_113_ = 113,                     /* '~'  */
  YYSYMBOL_114_ = 114,                     /* '+'  */
  YYSYMBOL_115_ = 115,                     /* '-'  */
  YYSYMBOL_116_ = 116,                     /* '*'  */
  YYSYMBOL_117_ = 117,                     /* '/'  */
  YYSYMBOL_118_ = 118,                     /* '%'  */
  YYSYMBOL_UMINUS = 119,                   /* UMINUS  */
  YYSYMBOL_120_ = 120,                     /* '.'  */
  YYSYMBOL_121_ = 121,                     /* ';'  */
  YYSYMBOL_122_ = 122,                     /* '('  */
  YYSYMBOL_123_ = 123,                     /* ')'  */
  YYSYMBOL_124_ = 124,                     /* ','  */
  YYSYMBOL_125_ = 125,                     /* '['  */
  YYSYMBOL_126_ = 126,                     /* ']'  */
  YYSYMBOL_127_ = 127,                     /* '{'  */
  YYSYMBOL_128_ = 128,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 129,                 /* $accept  */
  YYSYMBOL_program = 130,                  /* program  */
  YYSYMBOL_stmt_list = 131,                /* stmt_list  */
  YYSYMBOL_stmt = 132,                     /* stmt  */
  YYSYMBOL_if_stmt = 133,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 134,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 135,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 136,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 137,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 138,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 139,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 140,                /* loop_incr  */
  YYSYMBOL_for_stmt = 141,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 142,               /* given_stmt  */
  YYSYMBOL_when_list = 143,                /* when_list  */
  YYSYMBOL_sub_decl = 144,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 145,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 146,                 /* sub_body  */
  YYSYMBOL_method_body = 147,              /* method_body  */
  YYSYMBOL_class_decl = 148,               /* class_decl  */
  YYSYMBOL_role_decl = 149,                /* role_decl  */
  YYSYMBOL_is_clauses = 150,               /* is_clauses  */
  YYSYMBOL_class_body_list = 151,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 152,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 153,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 154,           /* named_arg_list  */
  YYSYMBOL_pair_list = 155,                /* pair_list  */
  YYSYMBOL_param_list = 156,               /* param_list  */
  YYSYMBOL_block = 157,                    /* block  */
  YYSYMBOL_closure = 158,                  /* closure  */
  YYSYMBOL_expr = 159,                     /* expr  */
  YYSYMBOL_tern_expr = 160,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 161,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 162,               /* divis_expr  */
  YYSYMBOL_jct_expr = 163,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 164,                 /* dor_expr  */
  YYSYMBOL_range_expr = 165,               /* range_expr  */
  YYSYMBOL_add_expr = 166,                 /* add_expr  */
  YYSYMBOL_repl_expr = 167,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 168,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 169,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 170,               /* unary_expr  */
  YYSYMBOL_pow_expr = 171,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 172,              /* scalar_list  */
  YYSYMBOL_meth_name = 173,                /* meth_name  */
  YYSYMBOL_postfix_expr = 174,             /* postfix_expr  */
  YYSYMBOL_call_expr = 175,                /* call_expr  */
  YYSYMBOL_arg_list = 176,                 /* arg_list  */
  YYSYMBOL_atom = 177                      /* atom  */
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
#define YYLAST   3699

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  129
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  415
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1058

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   362


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
       2,     2,     2,   108,     2,     2,     2,   118,   112,     2,
     122,   123,   116,   114,   124,   115,   120,   117,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   121,
     109,   107,   110,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   125,     2,   126,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   127,   111,   128,   113,     2,     2,     2,
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
     105,   106,   119
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   369,   369,   380,   381,   384,   386,   388,   390,   394,
     396,   398,   400,   404,   407,   411,   413,   415,   417,   419,
     421,   425,   429,   431,   433,   435,   437,   439,   441,   443,
     445,   447,   449,   451,   453,   455,   457,   460,   463,   465,
     468,   471,   473,   475,   477,   479,   484,   488,   492,   497,
     501,   502,   504,   506,   508,   510,   513,   515,   517,   519,
     522,   525,   528,   531,   534,   536,   538,   540,   542,   545,
     547,   549,   551,   554,   557,   561,   564,   566,   568,   570,
     573,   576,   579,   582,   585,   588,   591,   592,   593,   594,
     595,   596,   597,   599,   601,   603,   605,   606,   607,   608,
     609,   610,   611,   613,   615,   617,   619,   620,   621,   622,
     625,   627,   629,   631,   633,   635,   637,   639,   643,   645,
     647,   649,   651,   653,   657,   659,   663,   665,   667,   669,
     673,   675,   679,   681,   683,   687,   689,   691,   695,   698,
     704,   710,   715,   721,   726,   731,   739,   750,   751,   757,
     765,   771,   777,   785,   791,   797,   806,   813,   822,   831,
     840,   847,   854,   855,   858,   861,   863,   865,   867,   869,
     871,   873,   875,   878,   880,   882,   884,   887,   890,   896,
     897,   898,   901,   904,   906,   908,   910,   912,   914,   916,
     918,   921,   923,   925,   927,   930,   933,   939,   955,   970,
     971,   983,   997,   998,  1001,  1004,  1007,  1010,  1013,  1016,
    1019,  1022,  1025,  1028,  1031,  1034,  1041,  1048,  1055,  1062,
    1066,  1070,  1074,  1078,  1085,  1092,  1099,  1106,  1116,  1124,
    1132,  1141,  1148,  1155,  1165,  1173,  1183,  1198,  1199,  1204,
    1209,  1216,  1220,  1226,  1228,  1230,  1232,  1236,  1237,  1238,
    1239,  1240,  1241,  1242,  1243,  1246,  1247,  1248,  1250,  1252,
    1254,  1256,  1258,  1261,  1263,  1265,  1267,  1269,  1272,  1275,
    1279,  1282,  1285,  1288,  1290,  1294,  1297,  1300,  1303,  1306,
    1309,  1311,  1313,  1315,  1317,  1319,  1321,  1323,  1325,  1329,
    1332,  1333,  1338,  1341,  1343,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,  1360,
    1366,  1372,  1378,  1381,  1382,  1385,  1386,  1387,  1390,  1392,
    1395,  1396,  1397,  1400,  1401,  1404,  1405,  1408,  1409,  1410,
    1413,  1414,  1415,  1416,  1417,  1420,  1421,  1422,  1423,  1428,
    1431,  1432,  1435,  1436,  1439,  1440,  1441,  1442,  1443,  1444,
    1445,  1446,  1447,  1448,  1449,  1451,  1453,  1458,  1459,  1464,
    1466,  1472,  1478,  1482,  1486,  1491,  1498,  1503,  1509,  1515,
    1522,  1529,  1534,  1539,  1546,  1551,  1556,  1558,  1560,  1562,
    1564,  1566,  1569,  1570,  1573,  1574,  1575,  1576,  1584,  1585,
    1586,  1587,  1588,  1589,  1590,  1591,  1592,  1596,  1600,  1603,
    1605,  1607,  1609,  1611,  1613,  1615,  1617,  1618,  1622,  1626,
    1630,  1631,  1632,  1634,  1637,  1638
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
  "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE", "KW_HANDLES", "WORDLIST",
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

#define YYPACT_NINF (-662)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-415)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -662,    64,  1565,  -662,  -662,  -662,  -662,  -662,   678,   117,
     -28,   249,   253,  -662,  -662,  3528,  -662,  -662,  -662,   350,
    1634,    49,  2778,  2847,  2903,  2972,  3028,     4,   278,  3028,
    1703,    76,  3028,   387,   429,  3097,  3153,   278,    52,    25,
      82,   325,   325,  3222,   278,   278,  3028,   448,   467,   243,
    3528,  -662,   476,   484,   501,  3528,  3528,  -662,  1772,   452,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
     406,  -662,  -662,  -662,  3579,   589,  -662,   272,   286,   359,
     415,  -662,   146,   425,   431,   237,  -662,  -662,   450,   515,
     520,   561,  -662,  -662,  3028,  3028,  3028,  3028,  3028,  3028,
     607,  1828,  1898,   612,  3028,  3028,    61,  1954,   220,   527,
     -26,  -662,   278,  -662,  -662,   539,   321,  -662,  2024,  -662,
    -109,   -16,   179,   209,   686,   634,   318,   655,  1772,   523,
    1772,    19,  1772,   278,  1772,   278,    51,   163,   182,  -662,
    -662,   594,  -662,   610,   584,   628,   615,   -20,    97,  1772,
     278,  1772,   278,   173,  2080,  -662,  3028,  3028,  -662,  3028,
    3028,  -662,  3028,  3028,  3028,  2149,  3028,  -662,   700,  -662,
    -662,  -662,   688,   626,     5,  -662,   701,  -662,  -662,  -662,
    -662,  -662,   -10,   609,  1026,  3028,  3028,  3028,  3028,  3028,
    3028,  3028,  3028,  -662,  3028,  3028,  3028,  3028,  3028,  3028,
    3028,  3028,  -662,  3528,  3528,  3528,  3528,  3528,  3528,  3528,
    3528,  3528,  3528,  3528,  3528,  3528,  3528,  3528,   715,  3528,
    3528,  3528,  3528,  3528,  3528,  3528,  3528,  3528,  3528,  3528,
    3528,  3528,  3528,  3528,   829,   815,   490,   680,   691,   709,
     717,   726,   729,   123,  -662,   549,   571,   629,   683,   671,
     731,   687,   822,   733,  -662,   603,  1898,   846,  3028,  -662,
    3028,   746,   -10,   621,  -662,  3028,  3028,  -662,  3278,  -662,
    3347,  -662,   259,   269,   291,   195,   762,   769,  -662,   623,
     641,  3028,  3028,  3028,  3028,  3028,  3028,  3028,  -662,  3028,
     643,  3028,  3028,  -662,  3028,   645,   685,   656,  -662,   871,
    3028,  -662,  3528,  3528,    -3,  -662,  -662,  -662,  -662,  3028,
    3028,  -662,   869,  3028,   879,  3028,   658,   882,   663,  -662,
    3028,  3028,   901,   798,   807,   832,   833,   834,   814,  -662,
    -662,  -662,   278,    -9,  -662,     0,   835,   836,  -662,  -662,
    2218,  -662,   790,   831,   138,   852,  2778,  2847,  3028,   689,
      44,    86,  -662,   297,   839,   840,   841,   842,   843,   844,
     845,   848,   849,   850,   851,   855,   856,   857,   858,   859,
     864,   864,   864,   864,   864,   864,   891,   864,   864,   864,
     864,   864,   864,   864,   864,  -662,  -662,  -662,   359,  -662,
    -662,  -662,   870,   870,   425,   431,   237,   237,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,   860,   970,   863,  -662,  2274,  -662,
    -662,  -662,  -662,  -662,  -662,  3028,  2344,  -662,  3028,  3028,
     880,   881,   883,  -662,  2400,  -662,  2470,  -662,   865,   876,
     861,  -662,  -662,   872,  -662,   873,  1772,   200,   922,   107,
    2526,    14,   878,  3028,  -662,  3403,  -662,  3028,  -662,    21,
    -662,  3028,  3028,   893,   991,  2218,   884,   885,   894,   895,
     897,   898,   899,    87,  2218,   902,   904,   120,   278,   -18,
    3472,  -662,   278,   278,   101,   -37,   -36,  -662,     6,   875,
     666,  1187,   912,   913,    11,   914,   925,   944,   935,   278,
     278,   278,   943,   945,   896,  3028,  -662,  -662,  -662,  -662,
    -662,  -662,   990,  1008,  -662,     9,   875,   668,    22,   470,
     -17,  -662,   672,  1021,  1898,  3028,  3028,   210,    45,   170,
    -662,   280,  3028,  3028,  -662,  3028,  3028,  -662,  3028,  3028,
    3028,  3028,  3028,  3028,  3028,  3028,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  3528,  2596,  -662,  2652,   961,   679,   962,
    -662,   682,   939,   958,  3028,  3028,  3028,  -662,   684,   140,
    -662,   690,   695,  -662,  -662,  -662,  -662,  -662,   697,  3028,
    -662,  3028,  3028,  3028,   699,  -662,   343,  -662,   969,  1772,
     149,   971,   875,   703,   972,   973,  3028,  -662,   718,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,   735,  -662,  -662,
    -662,   806,  -662,  -662,  1772,   278,  -662,  -662,  1086,  -662,
    1087,  1088,  -662,  1089,  1090,  -662,   875,   570,  2778,  2847,
    1376,  -662,   348,  -662,  -662,  3028,   278,  -662,  -662,  -662,
     980,   981,  1076,  -662,  -662,  -662,  -662,  3028,   983,  -662,
    -662,   144,   617,   674,  1049,  -662,  -662,   875,   875,   750,
     768,  1092,  1093,  1094,  -662,  -662,   203,   984,   985,   283,
    3028,  3028,  3028,  3028,  3028,  3028,  -662,  -662,  -662,  -662,
     292,   296,   310,   312,   326,   336,   337,   345,   346,   351,
     354,   364,  -662,  -662,   775,  -662,   777,   780,  -662,   988,
    -662,  -662,  -662,  -662,   993,   994,   995,  -662,  3028,  -662,
    1097,  -662,  2218,   996,   172,  -662,  -662,   997,  1039,  1046,
    -662,   782,  -662,  3028,  -662,  -662,   875,  -662,  -662,   298,
    1001,  1002,   -18,  -662,   784,   903,   278,   278,   278,  -662,
    -662,  -662,  -662,    75,   367,    91,  -662,   377,  3028,  3028,
    3028,  3028,  3028,  3028,  3028,  3028,  -662,   278,   998,  -662,
    -662,   278,  1003,  3028,  -662,   287,   334,    15,    60,   661,
    1004,  1006,   829,  -662,  -662,   875,   875,  1121,  1122,  1123,
    3028,  2722,  1024,  1025,  -662,   378,   381,   398,   404,   407,
     423,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662,
    -662,  1060,   809,  -662,  -662,  -662,  3028,  3028,  2218,   471,
    -662,  -662,  3028,  -662,  -662,  -662,  -662,   278,   278,  -662,
    -662,  -662,  -662,  -662,  1125,  1126,  3028,  3028,  3028,  -662,
    -662,  -662,   426,   427,   437,   440,   443,   449,   455,   458,
    -662,  -662,  -662,  3028,  1014,  -662,    28,  1070,  -662,    33,
    -662,  1128,  1129,  1018,  3028,  -662,  1130,  1131,  1019,  3028,
    -662,    84,   154,  1028,  1029,  -662,  -662,   388,  -662,  -662,
    -662,  -662,  -662,   460,  1023,   811,  3028,  3028,  -662,  -662,
    -662,  -662,  -662,  -662,  3028,  1031,  -662,  -662,   813,  -662,
     590,   911,  -662,  -662,  -662,   468,   472,   477,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  1034,   278,  1030,   817,
    1032,  1307,  1030,   821,  1035,  1037,  -662,  1038,  1040,  1041,
    -662,  1042,  1132,  1150,  1044,  3028,  -662,  1152,  1153,  1047,
    3028,  -662,  -662,  -662,    36,  -662,  -662,  -662,  1043,   480,
     482,  -662,  -662,  1048,  -662,   278,  -662,  -662,  -662,  -662,
    3028,  -662,  -662,  1030,  -662,  2778,  2847,  1445,  -662,   457,
    -662,  1030,  -662,  -662,  -662,  -662,  -662,  -662,  1051,  1052,
    -662,  1053,  1054,  1055,  -662,  1057,  1030,   823,  -662,  -662,
    -662,  -662,  -662,  1056,  -662,   478,   127,  -662,   486,  3028,
    3028,  3028,  3028,  3028,  3028,  3028,  3028,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  1030,   278,  3028,  3028,
    3028,  -662,  -662,  -662,   488,   492,   496,   502,   506,   525,
     528,   533,  -662,  -662,   536,   541,   542,  -662,  -662,  -662,
    -662,  -662,  -662,  -662,  -662,  -662,  -662,  -662
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   384,   385,   386,   388,   389,   394,
     395,   407,   406,   408,   409,     0,   396,   397,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   387,     0,     0,     0,     0,     0,    87,     0,     3,
       4,    88,    89,    96,    97,    98,    99,    90,    91,   106,
       0,   107,   108,   109,    95,     0,   292,   294,   312,   314,
     317,   319,   322,   324,   326,   329,   334,   339,   341,   355,
     381,     0,   392,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   389,   394,
     395,   407,     0,   414,   337,     0,   389,    31,     0,   382,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   322,     0,   415,
     291,     0,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,   100,     0,
       0,   101,     0,     0,     0,     3,     0,   379,    92,    94,
     376,   199,     0,     0,     0,   338,     0,   390,   391,   336,
     335,   410,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,    86,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,   359,     0,     0,     0,     0,     0,
       0,   364,     0,   363,   357,     0,     0,     0,     0,    26,
       0,   410,     0,     0,    34,     0,     0,     6,     0,     9,
       0,    10,     0,     0,     0,     0,     0,     0,   342,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,     0,
       0,     0,     0,    38,     0,     0,   113,     0,   125,     0,
       0,   144,     0,     0,     0,     3,   151,    41,    42,     0,
       0,   147,     0,     0,     0,     0,     0,   128,     0,   131,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   377,
     378,   380,     0,     0,   202,     0,     0,     0,   237,   411,
       0,   256,   389,   394,   395,   407,     0,     0,     0,     0,
       0,     0,   255,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     297,   298,   301,   302,   295,   296,     0,   299,   300,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   313,   315,
     316,   318,   320,   321,   323,   325,   327,   328,   333,   330,
     331,   332,   340,   344,   354,   348,   349,   350,   351,   352,
     353,   346,   347,   345,   375,     0,   372,    47,     0,    81,
      82,    83,    84,    85,    44,     0,     0,   358,     0,     0,
     399,   402,   403,    49,     0,   367,     0,   356,     0,     0,
       0,   290,    33,     0,   383,     0,     0,     0,   386,   406,
       0,     0,     0,     0,    23,     0,    24,     0,    25,     0,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   411,     0,
       0,   116,   411,     0,     0,   320,   321,   247,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   411,
       0,   411,     0,     0,     0,     0,   102,   103,   104,   105,
     289,    93,     0,     0,   202,     0,     0,     0,     0,     0,
       0,   412,     0,     0,     0,     0,     0,     0,     0,     0,
     282,     0,     0,     0,   283,     0,     0,   284,     0,     0,
       0,     0,     0,     0,     0,     0,   257,    51,    53,    55,
      58,    56,    57,    52,    54,    64,    66,    68,    71,    69,
      70,    65,    67,     0,     0,   368,     0,     0,     0,     0,
     161,     0,     0,     0,     0,     0,     0,   366,     0,   406,
     362,     0,     0,   399,   402,   403,    32,     5,     0,     0,
      11,     0,     0,     0,     0,    16,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343,     0,    72,
      75,    74,    78,    76,    77,    73,    36,     0,    79,    80,
      39,   110,   115,   114,     0,     0,   124,   141,     0,   143,
       0,     0,   248,     0,     0,   150,     0,     0,     0,     0,
       0,   162,     0,    28,    27,     0,     0,   145,   404,   405,
       0,     0,   126,   129,   130,   133,   134,     0,     0,   200,
     201,     0,     0,     0,     0,   198,   159,     0,     0,     0,
       0,     0,     0,     0,   236,   413,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   266,   273,   280,   281,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   293,   374,     0,   371,     0,     0,    46,     0,
      48,   160,   401,   400,     0,     0,     0,   365,     0,   360,
       0,   361,     0,     0,     0,   244,   243,     0,     0,     0,
      18,     0,    19,     0,    22,   153,     0,    30,    29,     0,
     413,   413,     0,   117,     0,   118,     0,     0,     0,   249,
     250,   149,   251,     0,     0,     0,   165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   163,     0,     0,    62,
      63,     0,     0,     0,   197,     0,     0,     0,     0,     0,
       0,     0,     0,   158,   156,     0,     0,     0,     0,     0,
       0,     0,   399,   403,   277,     0,     0,     0,     0,     0,
       0,   285,   286,   287,   288,   258,   260,   262,   265,   263,
     264,   259,   261,   373,   370,   369,    45,    59,    60,    61,
     241,     0,     0,    13,    12,    17,     0,     0,     0,     0,
     152,     7,     0,    37,    40,   112,   111,   411,     0,   122,
     142,   139,   140,   252,     0,     0,     0,     0,     0,   166,
     167,   164,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   146,   127,     0,     0,   138,     0,     3,   232,     0,
     229,     0,     0,     0,     0,   208,     0,     0,     0,     0,
     203,     0,     0,     0,     0,   204,   205,     0,   155,   157,
     238,   239,   240,     0,   161,     0,     0,     0,   267,   269,
     272,   270,   271,   268,     0,   413,   246,   245,     0,    20,
       0,   119,   120,   253,   254,     0,     0,     0,   168,   170,
     172,   175,   173,   174,   169,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   219,     0,     0,     0,
     220,     0,     0,     0,     0,     0,   210,     0,     0,     0,
       0,   209,   206,   207,     0,   235,   276,   275,   160,     0,
       0,   242,    14,   413,     8,     0,   123,   176,   178,   177,
       0,   137,   231,     0,   180,     0,     0,     0,   179,     0,
     228,     0,   217,   218,   212,   215,   216,   211,     0,     0,
     221,     0,     0,     0,   222,     0,     0,     0,   274,   278,
     279,    21,   121,     0,   230,     0,     0,   183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,   227,   225,
     226,   214,   223,   224,   213,   234,     0,     0,     0,     0,
       0,   184,   185,   182,     0,     0,     0,     0,     0,     0,
       0,     0,   233,   136,     0,     0,     0,   186,   188,   190,
     193,   191,   192,   187,   189,   194,   196,   195
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -662,  -662,     1,  -662,  -473,  -612,  -662,  -662,  -662,  -662,
    -662,   212,  -662,  -662,  -662,  -662,  -662,  -273,  -661,  -662,
    -662,  -662,   669,  -662,  -662,   604,   730,  -309,   250,   907,
      -2,  -205,  -662,   710,   965,  -662,   497,    -1,   960,   963,
     724,     2,  -662,  -662,  -232,  -662,  -662,     3,  -662
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   184,    60,    61,   481,    62,    63,    64,    65,
      66,   864,    67,    68,   494,    69,    70,   306,   868,    71,
      72,   333,   515,    73,   520,   581,   451,   490,   113,   163,
     119,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,   279,   414,    88,    89,   120,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,     2,   460,   416,   376,   512,   622,   487,    24,   743,
     487,   488,   264,   513,   488,   265,   632,   114,   138,   336,
     129,   131,   133,   135,   136,   137,   517,   141,   143,   871,
     146,   487,   487,   150,   152,   488,   488,   872,   487,   630,
     631,   167,   488,   487,   170,   291,   487,   488,   645,   646,
     488,   156,   175,   671,   672,   673,   182,   179,   180,   121,
     122,   123,   292,   124,     3,   662,   663,   337,   157,   664,
     532,   291,   633,   634,   876,   251,   225,   225,   252,   873,
     125,   103,   877,   257,   126,   843,   144,   533,   292,   312,
     145,   266,   237,   238,   239,   240,   241,   242,   942,   104,
     247,   258,   249,   250,   245,   267,   943,   313,   159,    59,
     255,   674,   535,   339,   340,   870,   262,   291,   514,   253,
     489,   263,   874,   516,   878,   160,   280,   299,   290,   536,
     295,    59,   297,   839,   292,   595,   875,   665,   596,   647,
     293,   844,   845,   294,   602,   668,   158,   316,   944,   318,
     603,   928,   323,   291,   324,   325,   932,   326,   327,   996,
     328,   329,   330,   328,   331,   158,   293,   879,   947,   294,
     292,   127,   534,   687,   154,   300,   948,   628,    59,    59,
     593,   880,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   945,   362,   363,   364,   365,   366,   367,   368,   369,
     662,   663,   320,   161,   664,   946,   314,   161,   616,   669,
     670,   265,   293,   718,   537,   294,   635,   321,   949,   850,
     223,   224,   392,   393,   315,   265,   955,   106,    59,   107,
     425,   398,   399,   400,   401,   402,   680,   302,   303,   282,
     681,   620,   102,   666,   265,   426,   682,   103,   293,   683,
     684,   294,    74,   685,   438,  1032,   440,   173,   441,   225,
     106,   950,   107,   444,   445,   525,   447,   972,   452,   835,
     732,   980,   774,   733,   174,   951,   225,   139,   140,   466,
     467,   468,   469,   470,   471,   472,   268,   153,   155,   475,
     476,   307,   473,   824,   168,   169,   265,   477,   688,   966,
     269,   485,   486,   484,   304,   589,   491,   492,   493,   305,
     790,   496,  1004,   498,    92,    93,   270,   459,   502,   503,
    1018,   590,   305,   538,   591,   791,   539,   540,   276,   735,
     271,   288,   277,   541,   289,  1025,   542,   543,   686,   229,
     544,   545,   101,   522,   527,   528,   529,   531,   728,   203,
     204,   205,   206,   230,   231,   232,   105,   729,   702,   207,
     208,   209,   139,   751,   115,  1042,   453,   212,   213,   214,
     215,   216,   217,   106,   758,   107,   455,   759,   760,   218,
     454,   210,   211,   296,   761,   298,   301,   762,   763,   219,
     456,   764,   765,   846,   783,   784,   282,   847,   457,   147,
     317,   308,   319,   284,   433,    59,   285,   286,   689,   866,
     848,   794,   458,   506,   867,    92,    93,   507,   202,   831,
     801,   568,   832,   569,   802,   546,   572,   573,   260,   571,
     776,   508,   185,   509,    74,   186,   187,   578,   803,   582,
     804,   148,   188,   101,   588,   189,   190,   555,   182,   191,
     192,   598,   162,   600,   805,   601,   869,   556,   557,   604,
     605,   867,   171,   830,   806,   807,   558,   559,   608,   202,
     220,   221,   560,   808,   809,   561,   766,   617,   625,   810,
     487,   172,   811,  1009,   488,   562,  1010,  1011,   288,   642,
     176,   289,   812,  1012,   177,   849,  1013,  1014,   308,   609,
    1015,  1016,   611,   658,  1028,   851,   898,   282,  1029,   899,
     954,   178,   888,   889,   284,   867,   222,   285,   286,   612,
     183,  1030,   677,   678,   679,   613,   900,   193,   614,   226,
     690,   691,   901,   692,   693,   902,   694,   695,   696,   697,
     698,   699,   700,   701,   615,   227,   228,   555,   556,   281,
     887,   903,   282,   283,   918,   919,   233,   929,   557,   284,
     933,   558,   285,   286,   559,   920,   287,   704,   921,   707,
     560,   922,   714,   715,   716,   236,   561,   923,   202,   562,
     752,   710,   511,   924,   753,  1017,   925,   723,   956,   609,
     725,   726,   909,   611,   724,   265,   967,   731,   615,   288,
     968,   817,   289,   819,   739,   969,  1031,   308,   999,   555,
    1000,   417,   418,   556,  1033,   194,  1047,   557,   195,   196,
    1048,   243,   744,   558,  1049,   197,   248,   559,   198,   199,
    1050,   403,   200,   201,  1051,   234,   754,   755,   757,   404,
     235,   405,   406,   767,   288,   997,   560,   289,   275,   561,
     407,   408,   256,  1052,   562,   772,  1053,   609,   409,   410,
     259,  1054,   611,   615,  1055,   278,   411,   412,   413,  1056,
    1057,   881,   427,   265,   882,   775,   883,   884,   795,   796,
     797,   798,   799,   800,   777,   428,   429,   778,   779,   780,
     781,   309,     4,     5,     6,     7,   272,   273,   274,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
     202,   964,   479,   480,   265,   307,   820,   389,   390,   391,
     112,    28,   385,   386,   387,   822,   437,   265,   621,   623,
      33,   308,   626,   627,   629,   310,   829,   341,    41,    42,
      43,    74,   311,    46,   443,   265,   463,   464,   335,   652,
     653,   654,    50,   332,    51,   430,   852,   853,   854,   855,
     856,   857,   858,   859,   339,   465,   339,   474,   478,   340,
      91,   865,    92,    93,    94,    95,    96,    97,    98,   482,
     340,   499,   340,    53,    54,    99,   501,   340,   893,   636,
     637,   667,   637,   431,   895,   675,   265,    55,   100,   432,
     101,   419,   709,   265,    56,   711,   265,   717,   265,   434,
     142,    58,   420,   719,   720,   334,    59,   530,   721,   265,
     339,   722,   727,   596,   906,   907,   736,   637,   338,   403,
     421,   908,   415,   742,   480,   910,   435,   404,   422,   405,
     406,   740,   265,   403,   915,   916,   917,   423,   407,   408,
     424,   404,   433,   405,   406,   436,   409,   410,   741,   265,
     439,   926,   407,   408,   411,   412,   413,   442,   931,   461,
     409,   410,   937,   785,   637,   745,   462,   941,   411,   412,
     413,   483,    91,   495,    92,    93,    94,    95,    96,    97,
      98,   786,   637,   497,   959,   960,   768,    99,   813,   265,
     814,   720,   961,   815,   265,   339,   828,   837,   340,   500,
     523,   504,   101,   370,   371,   372,   373,   374,   375,   505,
     377,   378,   379,   380,   381,   382,   383,   384,   506,   979,
     838,   480,   905,   265,   958,   265,   963,   265,   965,   480,
     973,   637,   510,   991,   981,   637,  1026,   637,   995,   164,
     166,   396,   397,   507,   508,   509,   524,   518,   519,   526,
     547,   548,   549,   550,   551,   552,   553,   219,   865,   554,
     555,   556,   557,  1005,  1006,  1008,   558,   559,   560,   561,
     562,   563,   564,   225,   565,   566,   584,   574,   575,   585,
     576,   583,   836,   586,   587,   592,   840,   841,   842,   597,
     606,   607,   305,   657,   659,   609,   610,  1034,  1035,  1036,
    1037,  1038,  1039,  1040,  1041,   611,   612,   860,   613,   614,
     615,   862,   660,   618,   648,   619,  1044,  1045,  1046,     4,
       5,     6,     7,   643,   644,   676,   342,   343,   344,   345,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     346,   347,    24,   649,   650,    25,    26,    27,    28,   348,
     349,    31,    32,   651,   655,   712,   656,    33,    34,    35,
      36,    37,    38,   350,   351,    41,    42,    43,    44,    45,
      46,    47,   708,   710,   713,    48,    49,   911,   912,    50,
     730,    51,   734,   737,   738,    52,   746,   747,   748,   749,
     750,   769,   770,   771,   773,   782,   787,   788,   789,   816,
     792,   821,   826,   793,   817,   818,   819,   823,   825,   827,
      53,    54,   833,   834,   863,   885,   861,   886,   890,   891,
     892,   896,   897,   904,    55,   913,   914,   927,   930,   936,
     940,    56,   934,   935,   938,   939,   988,    57,    58,   952,
     953,   957,   962,    59,   352,   970,   982,   867,   983,   984,
     974,   985,   986,   987,   989,   990,   992,   993,   994,  1001,
     706,   998,  1019,  1020,  1021,  1022,  1023,   971,  1024,  1027,
     594,    74,  1003,   661,   388,   394,     0,     0,     0,   395,
       4,     5,     6,     7,     0,     0,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   638,   639,    24,     0,  1002,    25,    26,    27,    28,
      29,   640,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
      50,     0,    51,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1043,     0,     0,
       0,    53,    54,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,    57,    58,
       4,     5,     6,     7,    59,   641,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   975,   976,    24,     0,     0,    25,    26,    27,    28,
      29,   977,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
      50,     0,    51,     0,     0,     0,    52,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   116,   109,   110,   111,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    53,    54,     0,     0,     0,     0,   112,    28,     0,
       0,     0,     0,     0,     0,    55,     0,    33,     0,     0,
       0,     0,    56,     0,     0,    41,    42,    43,    57,    58,
      46,     0,     0,     0,    59,   978,     0,     0,     0,    50,
       0,    51,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   116,   109,   110,   111,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      53,    54,     0,     0,     0,     0,   112,    28,     0,     0,
       0,     0,     0,     0,    55,     0,    33,     0,     0,     0,
       0,    56,     0,     0,    41,    42,    43,   142,    58,    46,
       0,     0,     0,    59,   756,     0,     0,     0,    50,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,   142,    58,     4,     5,
       6,     7,    59,  1007,     0,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     0,     0,    25,    26,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,     0,     0,     0,    48,    49,     0,     0,    50,     0,
      51,     0,     0,     0,    52,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    53,
      54,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,     0,    55,     0,    33,     0,     0,     0,     0,
      56,     0,     0,    41,    42,    43,    57,    58,    46,     0,
       0,     0,    59,     0,     0,     0,     0,    50,     0,    51,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    53,    54,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,     0,    55,     0,    33,     0,     0,     0,     0,    56,
       0,     0,    41,    42,    43,   117,   118,    46,     0,     0,
       0,    59,     0,     0,     0,     0,    50,     0,    51,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,   142,    58,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,    58,   181,     0,     0,     0,    59,
       0,     4,     5,     6,     7,     0,     0,     0,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    53,    54,     0,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,    55,     0,     0,    33,
       0,     0,     0,    56,     0,     0,     0,    41,    42,    43,
      58,   244,    46,     0,     0,    59,     0,     4,     5,     6,
       7,    50,     0,    51,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    53,    54,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    55,     0,    46,     0,
       0,     0,     0,    56,   246,     0,     0,    50,     0,    51,
      58,     0,     0,     0,     0,    59,     0,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    53,    54,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    55,     0,     0,    33,     0,     0,     0,    56,
       0,     0,     0,    41,    42,    43,    58,   254,    46,     0,
       0,    59,     0,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,   322,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,    58,   261,     0,     0,
       0,    59,     4,     5,     6,     7,     0,     0,     0,   116,
     109,   110,   111,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    53,    54,     0,     0,     0,     0,
     112,    28,     0,     0,     0,     0,     0,     0,    55,     0,
      33,     0,     0,     0,     0,    56,     0,     0,    41,    42,
      43,     0,    58,    46,     0,     0,     0,    59,     0,     0,
       0,     0,    50,     0,    51,     0,     0,   183,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    53,    54,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,     0,    55,     0,    33,
       0,     0,     0,     0,    56,     0,     0,    41,    42,    43,
       0,    58,    46,     0,     0,     0,    59,     4,     5,     6,
       7,    50,     0,    51,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    53,    54,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    55,     0,    46,     0,
       0,     0,     0,    56,     0,     0,     0,    50,     0,    51,
      58,   521,     0,     0,     0,    59,     0,     4,     5,     6,
       7,     0,     0,     0,   116,   109,   110,   111,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    53,    54,
       0,     0,     0,     0,     0,   112,    28,     0,     0,     0,
       0,     0,    55,     0,     0,    33,     0,     0,     0,    56,
       0,     0,     0,    41,    42,    43,    58,   567,    46,     0,
       0,    59,     0,     4,     5,     6,     7,    50,     0,    51,
     116,   109,   110,   111,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    53,    54,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    55,     0,    46,     0,     0,     0,     0,    56,
       0,     0,     0,    50,     0,    51,    58,   570,     0,     0,
       0,    59,     0,     4,     5,     6,     7,     0,     0,     0,
     116,   109,   110,   111,   579,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    53,    54,     0,     0,     0,     0,
       0,   112,    28,     0,     0,     0,     0,     0,    55,     0,
       0,    33,     0,     0,     0,    56,     0,     0,     0,    41,
      42,    43,    58,   577,    46,     0,     0,    59,     0,     4,
       5,   448,     7,    50,     0,    51,   116,   109,   110,   111,
     449,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,    28,     0,
       0,     0,     0,     0,    53,    54,     0,    33,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    55,     0,
      46,     0,     0,     0,     0,    56,     0,     0,     0,    50,
       0,    51,    58,   580,     0,     0,     0,    59,     0,     4,
       5,     6,     7,     0,     0,     0,   116,   109,   110,   111,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      53,    54,     0,     0,     0,     0,     0,   112,    28,     0,
       0,     0,     0,     0,    55,     0,     0,    33,     0,     0,
       0,    56,     0,     0,     0,    41,    42,    43,    58,   181,
      46,     0,     0,    59,     0,     4,     5,     6,     7,    50,
       0,    51,   116,   109,   110,   111,   579,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
      53,    54,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    55,     0,    46,     0,     0,     0,
       0,    56,     0,     0,     0,    50,     0,    51,    58,   703,
       0,     0,     0,    59,     0,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    53,    54,     0,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
      55,     0,     0,    33,     0,     0,     0,    56,     0,     0,
       0,    41,    42,    43,    58,   705,    46,     0,     0,    59,
       0,     4,     5,     6,     7,    50,     0,    51,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,    58,   894,     0,     0,     0,    59,
       4,     5,     6,     7,     0,     0,     0,   116,   109,   110,
     111,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
     128,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,   130,
       0,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   132,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,   134,     0,     0,     0,     0,    59,
       4,     5,     6,     7,     0,     0,     0,   116,   109,   110,
     111,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
      58,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,   149,
       0,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   151,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   116,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
      28,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,    58,     0,     0,     0,     0,   165,
       4,     5,   448,     7,     0,     0,     0,   116,   109,   110,
     111,   449,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    53,    54,     0,     0,     0,     0,   112,    28,
       0,     0,     0,     0,     0,     0,    55,     0,    33,     0,
       0,     0,     0,    56,     0,     0,    41,    42,    43,     0,
     446,    46,     0,     0,     0,    59,     4,     5,     6,     7,
      50,     0,    51,   116,   109,   110,   111,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    28,     0,     0,     0,     0,
       0,    53,    54,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    55,     0,    46,     0,     0,
       0,     0,    56,     0,     0,     0,    50,     0,    51,   450,
       0,     0,     0,     0,    59,     4,     5,     6,     7,     0,
       0,     0,   116,   109,   110,   111,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    53,    54,     0,
       0,     0,     0,   112,    28,     0,     0,     0,     0,     0,
       0,    55,     0,    33,     0,     0,     0,     0,    56,     0,
       0,    41,    42,    43,     0,   599,    46,     0,     0,     0,
      59,     4,     5,     6,     7,    50,     0,    51,   108,   109,
     110,   111,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
       0,     0,     0,     0,     0,     0,    53,    54,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      55,     0,    46,     0,     0,     0,     0,    56,     0,     0,
       0,    50,     0,    51,   624,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -414,  -414,
       0,     0,    53,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    55,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
      58,     0,     0,  -414,  -414,    59,  -414,  -414,  -414,  -414,
    -414,  -414,  -414,  -414,  -414,  -414,  -414,  -414,  -414,     0,
       0,     0,  -414,     0,     0,     0,     0,     0,     0,     0,
    -414,  -414,  -414,  -414,     0,  -414,     0,     0,  -414,  -414,
    -414,  -414,  -414,  -414,     0,  -414,  -414,  -414,     0,  -414
};

static const yytype_int16 yycheck[] =
{
       2,     0,   275,   235,   209,    14,   479,    10,    26,   621,
      10,    14,   121,    22,    14,   124,    10,    15,    14,    14,
      22,    23,    24,    25,    26,    26,   335,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    22,    10,    76,
      76,    43,    14,    10,    46,    26,    10,    14,    37,    38,
      14,    26,    50,    70,    71,    72,    58,    55,    56,    10,
      11,    12,    43,    14,     0,    56,    57,    62,    43,    60,
      26,    26,    66,    67,    14,    14,   113,   113,    17,    64,
      31,   109,    22,   109,    35,    10,    10,    43,    43,   109,
      14,   107,    94,    95,    96,    97,    98,    99,    14,   127,
     102,   127,   104,   105,   101,   121,    22,   127,    26,   127,
     107,   128,    26,   123,   124,   776,   118,    26,   127,    58,
     123,   118,   107,   123,    64,    43,   128,    76,   130,    43,
     132,   127,   134,   745,    43,   121,   121,   128,   124,   128,
     121,    66,    67,   124,   123,   123,   121,   149,    64,   151,
     459,   123,   154,    26,   156,   157,   123,   159,   160,   123,
     162,   163,   164,   165,   166,   121,   121,   107,    14,   124,
      43,   122,   128,   128,   122,   124,    22,    76,   127,   127,
      73,   121,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   107,   194,   195,   196,   197,   198,   199,   200,   201,
      56,    57,    29,   121,    60,   121,   109,   121,   121,   518,
     519,   124,   121,    73,   128,   124,   489,    44,    64,   128,
      74,    75,   223,   224,   127,   124,   887,   120,   127,   122,
     107,   229,   230,   231,   232,   233,    26,    74,    75,    29,
      30,   121,   125,   516,   124,   122,    36,   109,   121,    39,
      40,   124,     2,    43,   256,   128,   258,    14,   260,   113,
     120,   107,   122,   265,   266,   127,   268,   928,   270,   742,
     121,   932,   128,   124,    31,   121,   113,    27,    28,   281,
     282,   283,   284,   285,   286,   287,   107,    37,    38,   291,
     292,   121,   289,   121,    44,    45,   124,   294,   128,   911,
     121,   302,   303,   300,   122,   105,   305,   309,   310,   127,
     107,   313,   973,   315,    94,    95,   107,   122,   320,   321,
     981,   121,   127,    26,   124,   122,    29,    30,    10,   602,
     121,   121,    14,    36,   124,   996,    39,    40,   128,   102,
      43,    44,   122,   340,   346,   347,   348,   349,     5,    77,
      78,    79,    80,   116,   117,   118,   107,    14,   563,    87,
      88,    89,   112,   636,    14,  1026,   107,    81,    82,    83,
      84,    85,    86,   120,    26,   122,   107,    29,    30,    93,
     121,   109,   110,   133,    36,   135,   136,    39,    40,   103,
     121,    43,    44,    26,   667,   668,    29,    30,   107,    12,
     150,   121,   152,    36,   121,   127,    39,    40,   128,   122,
      43,   128,   121,   121,   127,    94,    95,   121,   121,   121,
     128,   418,   124,   425,   128,   128,   428,   429,   107,   426,
     662,   121,    26,   121,   184,    29,    30,   434,   128,   436,
     128,    12,    36,   122,   446,    39,    40,   121,   450,    43,
      44,   453,   127,   455,   128,   457,   122,   121,   121,   461,
     462,   127,    14,   736,   128,   128,   121,   121,   465,   121,
     111,   112,   121,   128,   128,   121,   128,   474,   480,   128,
      10,    14,   128,    26,    14,   121,    29,    30,   121,   491,
      14,   124,   128,    36,    10,   128,    39,    40,   121,   121,
      43,    44,   121,   505,    26,   128,   128,    29,    30,   128,
     122,    10,   785,   786,    36,   127,   101,    39,    40,   121,
      68,    43,   524,   525,   526,   121,   128,   121,   121,   104,
     532,   533,   128,   535,   536,   128,   538,   539,   540,   541,
     542,   543,   544,   545,   121,   114,   115,   121,   121,    26,
     782,   128,    29,    30,   128,   128,   106,   866,   121,    36,
     869,   121,    39,    40,   121,   128,    43,   564,   128,   566,
     121,   128,   574,   575,   576,    14,   121,   128,   121,   121,
      10,   121,   332,   128,    14,   128,   128,   589,   128,   121,
     592,   593,   121,   121,   591,   124,   128,   599,   121,   121,
     128,   121,   124,   121,   606,   128,   128,   121,   128,   121,
     128,   121,   122,   121,   128,    26,   128,   121,    29,    30,
     128,    14,   624,   121,   128,    36,    14,   121,    39,    40,
     128,    14,    43,    44,   128,   120,   638,   639,   640,    22,
     120,    24,    25,   645,   121,   954,   121,   124,    14,   121,
      33,    34,   125,   128,   121,   657,   128,   121,    41,    42,
     121,   128,   121,   121,   128,    10,    49,    50,    51,   128,
     128,    10,   123,   124,    13,    58,    15,    16,   680,   681,
     682,   683,   684,   685,    10,   114,   115,    13,    14,    15,
      16,   107,     3,     4,     5,     6,    10,    11,    12,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     121,   121,    27,    28,   124,   121,   718,   220,   221,   222,
      31,    32,     7,     8,     9,   722,   123,   124,   478,   479,
      41,   121,   482,   483,   484,   107,   733,   128,    49,    50,
      51,   491,   127,    54,   123,   124,   123,   124,   122,   499,
     500,   501,    63,    53,    65,   126,   758,   759,   760,   761,
     762,   763,   764,   765,   123,   124,   123,   124,   123,   124,
      92,   773,    94,    95,    96,    97,    98,    99,   100,   123,
     124,   123,   124,    94,    95,   107,   123,   124,   790,   123,
     124,   123,   124,   110,   791,   123,   124,   108,   120,   128,
     122,   121,   123,   124,   115,   123,   124,   123,   124,   122,
     121,   122,   121,   123,   124,   127,   127,   128,   123,   124,
     123,   124,   123,   124,   826,   827,   123,   124,   127,    14,
     121,   828,    17,    27,    28,   832,    14,    22,   121,    24,
      25,   123,   124,    14,   846,   847,   848,   121,    33,    34,
     121,    22,   121,    24,    25,   122,    41,    42,   123,   124,
      14,   863,    33,    34,    49,    50,    51,   121,   867,   107,
      41,    42,   874,   123,   124,   625,   107,   879,    49,    50,
      51,    10,    92,    14,    94,    95,    96,    97,    98,    99,
     100,   123,   124,    14,   896,   897,   646,   107,   123,   124,
     123,   124,   904,   123,   124,   123,   124,   123,   124,    27,
     120,    10,   122,   203,   204,   205,   206,   207,   208,   121,
     210,   211,   212,   213,   214,   215,   216,   217,   121,   931,
      27,    28,   123,   124,   123,   124,   123,   124,    27,    28,
     123,   124,   128,   945,   123,   124,   123,   124,   950,    42,
      43,   227,   228,   121,   121,   121,   125,   122,   122,   107,
     121,   121,   121,   121,   121,   121,   121,   103,   970,   121,
     121,   121,   121,   975,   976,   977,   121,   121,   121,   121,
     121,    90,   122,   113,    14,   122,   110,   107,   107,   128,
     107,   126,   742,   121,   121,    73,   746,   747,   748,   121,
     107,    10,   127,   107,    14,   121,   121,  1009,  1010,  1011,
    1012,  1013,  1014,  1015,  1016,   121,   121,   767,   121,   121,
     121,   771,    14,   121,   110,   121,  1028,  1029,  1030,     3,
       4,     5,     6,   121,   121,    14,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   128,   110,    29,    30,    31,    32,    33,
      34,    35,    36,   128,   121,   126,   121,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,   121,   121,   126,    59,    60,   837,   838,    63,
     121,    65,   121,   121,   121,    69,    10,    10,    10,    10,
      10,   121,   121,    27,   121,    56,    14,    14,    14,   121,
     126,    14,    73,   128,   121,   121,   121,   121,   121,    73,
      94,    95,   121,   121,   121,   121,   128,   121,     7,     7,
       7,   107,   107,    73,   108,    10,    10,   123,    68,   121,
     121,   115,    14,    14,    14,    14,    14,   121,   122,   121,
     121,   128,   121,   127,   128,   121,   121,   127,   121,   121,
     128,   121,   121,   121,    14,   121,    14,    14,   121,   121,
     566,   128,   121,   121,   121,   121,   121,   927,   121,   123,
     450,   931,   970,   514,   219,   225,    -1,    -1,    -1,   226,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,   965,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1027,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,   122,
       3,     4,     5,     6,   127,   128,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    -1,    -1,    69,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    49,    50,    51,   121,   122,
      54,    -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    63,
      -1,    65,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    49,    50,    51,   121,   122,    54,
      -1,    -1,    -1,   127,   128,    -1,    -1,    -1,    63,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,   121,   122,     3,     4,
       5,     6,   127,   128,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    -1,    -1,    69,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,
      95,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    49,    50,    51,   121,   122,    54,    -1,
      -1,    -1,   127,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    49,    50,    51,   121,   122,    54,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    63,    -1,    65,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,   121,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,   123,    -1,    -1,    -1,   127,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    41,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    49,    50,    51,
     122,   123,    54,    -1,    -1,   127,    -1,     3,     4,     5,
       6,    63,    -1,    65,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,
      -1,    -1,    -1,   115,   116,    -1,    -1,    63,    -1,    65,
     122,    -1,    -1,    -1,    -1,   127,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    41,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    49,    50,    51,   122,   123,    54,    -1,
      -1,   127,    -1,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,   123,    -1,    -1,
      -1,   127,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      41,    -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,
      51,    -1,   122,    54,    -1,    -1,    -1,   127,    -1,    -1,
      -1,    -1,    63,    -1,    65,    -1,    -1,    68,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,
      -1,   122,    54,    -1,    -1,    -1,   127,     3,     4,     5,
       6,    63,    -1,    65,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,
     122,   123,    -1,    -1,    -1,   127,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    94,    95,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   108,    -1,    -1,    41,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    49,    50,    51,   122,   123,    54,    -1,
      -1,   127,    -1,     3,     4,     5,     6,    63,    -1,    65,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    94,    95,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   108,    -1,    54,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    -1,    63,    -1,    65,   122,   123,    -1,    -1,
      -1,   127,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    94,    95,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,    41,    -1,    -1,    -1,   115,    -1,    -1,    -1,    49,
      50,    51,   122,   123,    54,    -1,    -1,   127,    -1,     3,
       4,     5,     6,    63,    -1,    65,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    94,    95,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,   108,    -1,
      54,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    63,
      -1,    65,   122,   123,    -1,    -1,    -1,   127,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      94,    95,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,    41,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    49,    50,    51,   122,   123,
      54,    -1,    -1,   127,    -1,     3,     4,     5,     6,    63,
      -1,    65,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,    -1,
      -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,   123,
      -1,    -1,    -1,   127,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    94,    95,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     108,    -1,    -1,    41,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    49,    50,    51,   122,   123,    54,    -1,    -1,   127,
      -1,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,   123,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    41,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    49,    50,    51,    -1,
     122,    54,    -1,    -1,    -1,   127,     3,     4,     5,     6,
      63,    -1,    65,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    94,    95,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   108,    -1,    54,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    -1,    63,    -1,    65,   122,
      -1,    -1,    -1,    -1,   127,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    94,    95,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    41,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    49,    50,    51,    -1,   122,    54,    -1,    -1,    -1,
     127,     3,     4,     5,     6,    63,    -1,    65,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     108,    -1,    54,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    63,    -1,    65,   122,    -1,    -1,    -1,    -1,   127,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,
      -1,    -1,    94,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    74,    75,   127,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   102,   103,   104,    -1,   106,    -1,    -1,   109,   110,
     111,   112,   113,   114,    -1,   116,   117,   118,    -1,   120
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   130,   131,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    65,    69,    94,    95,   108,   115,   121,   122,   127,
     132,   133,   135,   136,   137,   138,   139,   141,   142,   144,
     145,   148,   149,   152,   157,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   174,   175,
     177,    92,    94,    95,    96,    97,    98,    99,   100,   107,
     120,   122,   125,   109,   127,   107,   120,   122,    10,    11,
      12,    13,    31,   157,   170,    14,    10,   121,   122,   159,
     176,    10,    11,    12,    14,    31,    35,   122,   122,   159,
     122,   159,   122,   159,   122,   159,   159,   166,    14,   157,
     157,   159,   121,   159,    10,    14,   159,    12,    12,   122,
     159,   122,   159,   157,   122,   157,    26,    43,   121,    26,
      43,   121,   127,   158,   158,   127,   158,   159,   157,   157,
     159,    14,    14,    14,    31,   170,    14,    10,    10,   170,
     170,   123,   159,    68,   131,    26,    29,    30,    36,    39,
      40,    43,    44,   121,    26,    29,    30,    36,    39,    40,
      43,    44,   121,    77,    78,    79,    80,    87,    88,    89,
     109,   110,    81,    82,    83,    84,    85,    86,    93,   103,
     111,   112,   101,    74,    75,   113,   104,   114,   115,   102,
     116,   117,   118,   106,   120,   120,    14,   159,   159,   159,
     159,   159,   159,    14,   123,   176,   116,   159,    14,   159,
     159,    14,    17,    58,   123,   176,   125,   109,   127,   121,
     107,   123,   159,   176,   121,   124,   107,   121,   107,   121,
     107,   121,    10,    11,    12,    14,    10,    14,    10,   172,
     159,    26,    29,    30,    36,    39,    40,    43,   121,   124,
     159,    26,    43,   121,   124,   159,   157,   159,   157,    76,
     124,   157,    74,    75,   122,   127,   146,   121,   121,   107,
     107,   127,   109,   127,   109,   127,   159,   157,   159,   157,
      29,    44,    23,   159,   159,   159,   159,   159,   159,   159,
     159,   159,    53,   150,   127,   122,    14,    62,   127,   123,
     124,   128,    10,    11,    12,    13,    24,    25,    33,    34,
      47,    48,   128,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     162,   162,   162,   162,   162,   162,   160,   162,   162,   162,
     162,   162,   162,   162,   162,     7,     8,     9,   163,   165,
     165,   165,   166,   166,   167,   168,   169,   169,   170,   170,
     170,   170,   170,    14,    22,    24,    25,    33,    34,    41,
      42,    49,    50,    51,   173,    17,   173,   121,   122,   121,
     121,   121,   121,   121,   121,   107,   122,   123,   114,   115,
     126,   110,   128,   121,   122,    14,   122,   123,   159,    14,
     159,   159,   121,   123,   159,   159,   122,   159,     5,    14,
     122,   155,   159,   107,   121,   107,   121,   107,   121,   122,
     146,   107,   107,   123,   124,   124,   159,   159,   159,   159,
     159,   159,   159,   176,   124,   159,   159,   176,   123,    27,
      28,   134,   123,    10,   176,   166,   166,    10,    14,   123,
     156,   131,   159,   159,   143,    14,   159,    14,   159,   123,
      27,   123,   159,   159,    10,   121,   121,   121,   121,   121,
     128,   157,    14,    22,   127,   151,   123,   156,   122,   122,
     153,   123,   176,   120,   125,   127,   107,   159,   159,   159,
     128,   159,    26,    43,   128,    26,    43,   128,    26,    29,
      30,    36,    39,    40,    43,    44,   128,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   121,    90,   122,    14,   122,   123,   176,   159,
     123,   176,   159,   159,   107,   107,   107,   123,   176,    14,
     123,   154,   176,   126,   110,   128,   121,   121,   159,   105,
     121,   124,    73,    73,   155,   121,   124,   121,   159,   122,
     159,   159,   123,   156,   159,   159,   107,    10,   176,   121,
     121,   121,   121,   121,   121,   121,   121,   176,   121,   121,
     121,   157,   133,   157,   122,   159,   157,   157,    76,   157,
      76,    76,    10,    66,    67,   146,   123,   124,    24,    25,
      34,   128,   159,   121,   121,    37,    38,   128,   110,   128,
     110,   128,   157,   157,   157,   121,   121,   107,   159,    14,
      14,   151,    56,    57,    60,   128,   146,   123,   123,   156,
     156,    70,    71,    72,   128,   123,    14,   159,   159,   159,
      26,    30,    36,    39,    40,    43,   128,   128,   128,   128,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   160,   123,   176,   123,   154,   176,   121,   123,
     121,   123,   126,   126,   159,   159,   159,   123,    73,   123,
     124,   123,   124,   159,   176,   159,   159,   123,     5,    14,
     121,   159,   121,   124,   121,   146,   123,   121,   121,   159,
     123,   123,    27,   134,   159,   157,    10,    10,    10,    10,
      10,   146,    10,    14,   159,   159,   128,   159,    26,    29,
      30,    36,    39,    40,    43,    44,   128,   159,   157,   121,
     121,    27,   159,   121,   128,    58,   173,    10,    13,    14,
      15,    16,    56,   146,   146,   123,   123,    14,    14,    14,
     107,   122,   126,   128,   128,   159,   159,   159,   159,   159,
     159,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   123,   123,   123,   121,   121,   121,   121,
     159,    14,   176,   121,   121,   121,    73,    73,   124,   176,
     146,   121,   124,   121,   121,   133,   157,   123,    27,   134,
     157,   157,   157,    10,    66,    67,    26,    30,    43,   128,
     128,   128,   159,   159,   159,   159,   159,   159,   159,   159,
     157,   128,   157,   121,   140,   159,   122,   127,   147,   122,
     147,    14,    22,    64,   107,   121,    14,    22,    64,   107,
     121,    10,    13,    15,    16,   121,   121,   173,   146,   146,
       7,     7,     7,   159,   123,   176,   107,   107,   128,   128,
     128,   128,   128,   128,    73,   123,   159,   159,   176,   121,
     176,   157,   157,    10,    10,   159,   159,   159,   128,   128,
     128,   128,   128,   128,   128,   128,   159,   123,   123,   156,
      68,   131,   123,   156,    14,    14,   121,   159,    14,    14,
     121,   159,    14,    22,    64,   107,   121,    14,    22,    64,
     107,   121,   121,   121,   122,   147,   128,   128,   123,   159,
     159,   159,   121,   123,   121,    27,   134,   128,   128,   128,
     121,   157,   147,   123,   128,    24,    25,    34,   128,   159,
     147,   123,   121,   121,   121,   121,   121,   121,    14,    14,
     121,   159,    14,    14,   121,   159,   123,   156,   128,   128,
     128,   121,   157,   140,   147,   159,   159,   128,   159,    26,
      29,    30,    36,    39,    40,    43,    44,   128,   147,   121,
     121,   121,   121,   121,   121,   147,   123,   123,    26,    30,
      43,   128,   128,   128,   159,   159,   159,   159,   159,   159,
     159,   159,   147,   157,   159,   159,   159,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   129,   130,   131,   131,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     133,   133,   133,   133,   133,   133,   133,   133,   134,   134,
     134,   134,   134,   134,   135,   135,   136,   136,   136,   136,
     137,   137,   138,   138,   138,   139,   139,   139,   140,   141,
     141,   141,   141,   141,   141,   142,   142,   143,   143,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     145,   145,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   148,   149,   150,
     150,   150,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   152,   153,   153,   153,
     153,   154,   154,   155,   155,   155,   155,   156,   156,   156,
     156,   156,   156,   156,   156,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   157,   157,   158,
     159,   159,   159,   160,   160,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   162,   162,   163,   163,   163,   164,   164,
     165,   165,   165,   166,   166,   167,   167,   168,   168,   168,
     169,   169,   169,   169,   169,   170,   170,   170,   170,   170,
     171,   171,   172,   172,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   174,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   176,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177
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
       3,     3,     3,     3,     1,     2,     2,     2,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     4,     3,
       6,     6,     5,     3,     3,     6,     5,     4,     4,     6,
       6,     5,     3,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     4,
       6,     6,     4,     4,     5,     5,     1,     1,     1,     1,
       2,     3,     4,     5,     1,     2
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
#line 370 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2737 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 380 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2743 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 381 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2749 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 385 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2755 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 387 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2761 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 389 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2767 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 391 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2775 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 395 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2781 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 397 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2787 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 399 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2793 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 401 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2801 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 405 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2808 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 408 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2816 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 412 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2822 "raku.tab.c"
    break;

  case 16: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 414 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2828 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 416 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 2834 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 418 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2840 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 420 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 2846 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 422 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2854 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 426 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 2862 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 430 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 2868 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 432 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2874 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 434 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2880 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 436 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 2886 "raku.tab.c"
    break;

  case 26: /* stmt: KW_USE IDENT ';'  */
#line 438 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 2892 "raku.tab.c"
    break;

  case 27: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 440 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2898 "raku.tab.c"
    break;

  case 28: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 442 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2904 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 444 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2910 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 446 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 2916 "raku.tab.c"
    break;

  case 31: /* stmt: TESTOP ';'  */
#line 448 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 2922 "raku.tab.c"
    break;

  case 32: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 450 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2928 "raku.tab.c"
    break;

  case 33: /* stmt: TESTOP '(' ')' ';'  */
#line 452 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 2934 "raku.tab.c"
    break;

  case 34: /* stmt: TESTOP arg_list ';'  */
#line 454 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2940 "raku.tab.c"
    break;

  case 35: /* stmt: KW_SAY expr ';'  */
#line 456 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2946 "raku.tab.c"
    break;

  case 36: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 458 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2953 "raku.tab.c"
    break;

  case 37: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 461 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2960 "raku.tab.c"
    break;

  case 38: /* stmt: KW_PRINT expr ';'  */
#line 464 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 2966 "raku.tab.c"
    break;

  case 39: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 466 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2973 "raku.tab.c"
    break;

  case 40: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 469 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 2980 "raku.tab.c"
    break;

  case 41: /* stmt: KW_TAKE expr ';'  */
#line 472 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 2986 "raku.tab.c"
    break;

  case 42: /* stmt: KW_RETURN expr ';'  */
#line 474 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 2992 "raku.tab.c"
    break;

  case 43: /* stmt: KW_RETURN ';'  */
#line 476 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 2998 "raku.tab.c"
    break;

  case 44: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 478 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)); }
#line 3004 "raku.tab.c"
    break;

  case 45: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 480 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3013 "raku.tab.c"
    break;

  case 46: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 485 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3021 "raku.tab.c"
    break;

  case 47: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 489 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3029 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 493 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3038 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 498 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3046 "raku.tab.c"
    break;

  case 50: /* stmt: scalar_methcall ';'  */
#line 501 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3052 "raku.tab.c"
    break;

  case 51: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 503 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3058 "raku.tab.c"
    break;

  case 52: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 505 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3064 "raku.tab.c"
    break;

  case 53: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 507 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3070 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 509 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3076 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 511 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3083 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 514 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3089 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 516 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3095 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 518 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3101 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 520 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3108 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 523 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3115 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 526 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3122 "raku.tab.c"
    break;

  case 62: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 529 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3129 "raku.tab.c"
    break;

  case 63: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 532 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3136 "raku.tab.c"
    break;

  case 64: /* stmt: expr KW_IF expr ';'  */
#line 535 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3142 "raku.tab.c"
    break;

  case 65: /* stmt: expr KW_UNLESS expr ';'  */
#line 537 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3148 "raku.tab.c"
    break;

  case 66: /* stmt: expr KW_WHILE expr ';'  */
#line 539 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3154 "raku.tab.c"
    break;

  case 67: /* stmt: expr KW_UNTIL expr ';'  */
#line 541 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3160 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_FOR expr ';'  */
#line 543 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3167 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_WITH expr ';'  */
#line 546 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3173 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WITHOUT expr ';'  */
#line 548 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3179 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_GIVEN expr ';'  */
#line 550 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3185 "raku.tab.c"
    break;

  case 72: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 552 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3192 "raku.tab.c"
    break;

  case 73: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 555 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3199 "raku.tab.c"
    break;

  case 74: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 558 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3207 "raku.tab.c"
    break;

  case 75: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 562 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3214 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 565 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3220 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 567 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3226 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 569 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3232 "raku.tab.c"
    break;

  case 79: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 571 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3239 "raku.tab.c"
    break;

  case 80: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 574 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3246 "raku.tab.c"
    break;

  case 81: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 577 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3253 "raku.tab.c"
    break;

  case 82: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 580 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3260 "raku.tab.c"
    break;

  case 83: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 583 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3267 "raku.tab.c"
    break;

  case 84: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 586 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3274 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 589 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3281 "raku.tab.c"
    break;

  case 86: /* stmt: expr ';'  */
#line 591 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3287 "raku.tab.c"
    break;

  case 87: /* stmt: ';'  */
#line 592 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3293 "raku.tab.c"
    break;

  case 88: /* stmt: if_stmt  */
#line 593 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3299 "raku.tab.c"
    break;

  case 89: /* stmt: while_stmt  */
#line 594 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3305 "raku.tab.c"
    break;

  case 90: /* stmt: for_stmt  */
#line 595 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3311 "raku.tab.c"
    break;

  case 91: /* stmt: given_stmt  */
#line 596 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3317 "raku.tab.c"
    break;

  case 92: /* stmt: KW_TRY block  */
#line 598 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3323 "raku.tab.c"
    break;

  case 93: /* stmt: KW_TRY block KW_CATCH block  */
#line 600 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3329 "raku.tab.c"
    break;

  case 94: /* stmt: KW_CATCH block  */
#line 602 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3335 "raku.tab.c"
    break;

  case 95: /* stmt: block  */
#line 604 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3341 "raku.tab.c"
    break;

  case 96: /* stmt: unless_stmt  */
#line 605 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3347 "raku.tab.c"
    break;

  case 97: /* stmt: until_stmt  */
#line 606 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3353 "raku.tab.c"
    break;

  case 98: /* stmt: repeat_stmt  */
#line 607 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3359 "raku.tab.c"
    break;

  case 99: /* stmt: loop_stmt  */
#line 608 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3365 "raku.tab.c"
    break;

  case 100: /* stmt: KW_LAST ';'  */
#line 609 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3371 "raku.tab.c"
    break;

  case 101: /* stmt: KW_NEXT ';'  */
#line 610 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3377 "raku.tab.c"
    break;

  case 102: /* stmt: KW_LAST KW_IF expr ';'  */
#line 612 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3383 "raku.tab.c"
    break;

  case 103: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 614 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3389 "raku.tab.c"
    break;

  case 104: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 616 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3395 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 618 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3401 "raku.tab.c"
    break;

  case 106: /* stmt: sub_decl  */
#line 619 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3407 "raku.tab.c"
    break;

  case 107: /* stmt: class_decl  */
#line 620 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3413 "raku.tab.c"
    break;

  case 108: /* stmt: role_decl  */
#line 621 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3419 "raku.tab.c"
    break;

  case 109: /* stmt: grammar_decl  */
#line 622 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3425 "raku.tab.c"
    break;

  case 110: /* if_stmt: KW_IF '(' expr ')' block  */
#line 626 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3431 "raku.tab.c"
    break;

  case 111: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 628 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3437 "raku.tab.c"
    break;

  case 112: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 630 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3443 "raku.tab.c"
    break;

  case 113: /* if_stmt: KW_IF expr block  */
#line 632 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3449 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 634 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3455 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 636 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3461 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF expr block elsif_tail  */
#line 638 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3467 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3473 "raku.tab.c"
    break;

  case 118: /* elsif_tail: KW_ELSIF expr block  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3479 "raku.tab.c"
    break;

  case 119: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3485 "raku.tab.c"
    break;

  case 120: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 648 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3491 "raku.tab.c"
    break;

  case 121: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 650 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3497 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3503 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3509 "raku.tab.c"
    break;

  case 124: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 658 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3515 "raku.tab.c"
    break;

  case 125: /* while_stmt: KW_WHILE expr block  */
#line 660 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3521 "raku.tab.c"
    break;

  case 126: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3527 "raku.tab.c"
    break;

  case 127: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 666 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3533 "raku.tab.c"
    break;

  case 128: /* unless_stmt: KW_UNLESS expr block  */
#line 668 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3539 "raku.tab.c"
    break;

  case 129: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3545 "raku.tab.c"
    break;

  case 130: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3551 "raku.tab.c"
    break;

  case 131: /* until_stmt: KW_UNTIL expr block  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3557 "raku.tab.c"
    break;

  case 132: /* repeat_stmt: KW_REPEAT block  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3563 "raku.tab.c"
    break;

  case 133: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3569 "raku.tab.c"
    break;

  case 134: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 684 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3575 "raku.tab.c"
    break;

  case 135: /* loop_stmt: KW_LOOP block  */
#line 688 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3581 "raku.tab.c"
    break;

  case 136: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 690 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3587 "raku.tab.c"
    break;

  case 137: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 692 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3593 "raku.tab.c"
    break;

  case 138: /* loop_incr: expr  */
#line 695 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3599 "raku.tab.c"
    break;

  case 139: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 699 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3609 "raku.tab.c"
    break;

  case 140: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 705 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3619 "raku.tab.c"
    break;

  case 141: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 711 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3628 "raku.tab.c"
    break;

  case 142: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 716 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3638 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 722 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3647 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR expr block  */
#line 727 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3654 "raku.tab.c"
    break;

  case 145: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 732 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3666 "raku.tab.c"
    break;

  case 146: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 740 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3679 "raku.tab.c"
    break;

  case 147: /* when_list: %empty  */
#line 750 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3685 "raku.tab.c"
    break;

  case 148: /* when_list: when_list KW_WHEN expr block  */
#line 752 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3693 "raku.tab.c"
    break;

  case 149: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 758 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3705 "raku.tab.c"
    break;

  case 150: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 766 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3715 "raku.tab.c"
    break;

  case 151: /* sub_decl: KW_SUB IDENT sub_body  */
#line 772 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3725 "raku.tab.c"
    break;

  case 152: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 778 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3737 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 786 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3747 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 792 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3757 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 798 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3770 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 807 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3781 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 814 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3794 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 823 "raku.y"
        { ExprList *params=(yyvsp[-2].list); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3807 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 832 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3818 "raku.tab.c"
    break;

  case 160: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 841 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 3829 "raku.tab.c"
    break;

  case 161: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 848 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 3838 "raku.tab.c"
    break;

  case 162: /* sub_body: '{' stmt_list '}'  */
#line 854 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3844 "raku.tab.c"
    break;

  case 163: /* sub_body: '{' stmt_list expr '}'  */
#line 856 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3851 "raku.tab.c"
    break;

  case 164: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 859 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3858 "raku.tab.c"
    break;

  case 165: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 862 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3864 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 864 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3870 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 866 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3876 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 868 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3882 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 870 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3888 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 872 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3894 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 874 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3900 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 876 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3907 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 879 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3913 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 881 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3919 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 883 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3925 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 885 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3932 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 888 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3939 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 891 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3947 "raku.tab.c"
    break;

  case 179: /* method_body: '{' stmt_list '}'  */
#line 896 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 3953 "raku.tab.c"
    break;

  case 180: /* method_body: '{' YADA '}'  */
#line 897 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 3959 "raku.tab.c"
    break;

  case 181: /* method_body: '{' stmt_list expr '}'  */
#line 899 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3966 "raku.tab.c"
    break;

  case 182: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 902 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3973 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 905 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 3979 "raku.tab.c"
    break;

  case 184: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 907 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 3985 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 909 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 3991 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 911 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 3997 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 913 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4003 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 915 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4009 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4015 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 919 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4022 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 922 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4028 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 924 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4034 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 926 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4040 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 928 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4047 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 931 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4054 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 934 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4062 "raku.tab.c"
    break;

  case 197: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 940 "raku.y"
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
#line 4080 "raku.tab.c"
    break;

  case 198: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 956 "raku.y"
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
#line 4097 "raku.tab.c"
    break;

  case 199: /* is_clauses: %empty  */
#line 970 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4103 "raku.tab.c"
    break;

  case 200: /* is_clauses: is_clauses IDENT IDENT  */
#line 972 "raku.y"
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
#line 4119 "raku.tab.c"
    break;

  case 201: /* is_clauses: is_clauses TESTOP IDENT  */
#line 984 "raku.y"
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
#line 4135 "raku.tab.c"
    break;

  case 202: /* class_body_list: %empty  */
#line 997 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4141 "raku.tab.c"
    break;

  case 203: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 999 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4148 "raku.tab.c"
    break;

  case 204: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1002 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4155 "raku.tab.c"
    break;

  case 205: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1005 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4162 "raku.tab.c"
    break;

  case 206: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1008 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4169 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1011 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4176 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1014 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4183 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1017 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4190 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1020 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4197 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1023 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4204 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1026 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4211 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1029 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4218 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1032 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4225 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1035 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4236 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1042 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4247 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1049 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4258 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1056 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4269 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1063 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4277 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1067 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4285 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1071 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4293 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1075 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4301 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1079 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4312 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1086 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4323 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1093 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4334 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1100 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4345 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1107 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4359 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1117 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4371 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1125 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4383 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1133 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4396 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1142 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4407 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1149 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4418 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1156 "raku.y"
        { ExprList *params = (yyvsp[-2].list); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4432 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1166 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4444 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1174 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4456 "raku.tab.c"
    break;

  case 236: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1184 "raku.y"
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
#line 4473 "raku.tab.c"
    break;

  case 237: /* grammar_body_list: %empty  */
#line 1198 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4479 "raku.tab.c"
    break;

  case 238: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1200 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4488 "raku.tab.c"
    break;

  case 239: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1205 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4497 "raku.tab.c"
    break;

  case 240: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1210 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4506 "raku.tab.c"
    break;

  case 241: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1217 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4514 "raku.tab.c"
    break;

  case 242: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1221 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4522 "raku.tab.c"
    break;

  case 243: /* pair_list: IDENT OP_FATARROW expr  */
#line 1227 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4528 "raku.tab.c"
    break;

  case 244: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1229 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4534 "raku.tab.c"
    break;

  case 245: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1231 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4540 "raku.tab.c"
    break;

  case 246: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1233 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4546 "raku.tab.c"
    break;

  case 247: /* param_list: VAR_SCALAR  */
#line 1236 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4552 "raku.tab.c"
    break;

  case 248: /* param_list: IDENT VAR_SCALAR  */
#line 1237 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4558 "raku.tab.c"
    break;

  case 249: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1238 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4564 "raku.tab.c"
    break;

  case 250: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1239 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4570 "raku.tab.c"
    break;

  case 251: /* param_list: param_list ',' VAR_SCALAR  */
#line 1240 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4576 "raku.tab.c"
    break;

  case 252: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1241 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4582 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1242 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4588 "raku.tab.c"
    break;

  case 254: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1243 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4594 "raku.tab.c"
    break;

  case 255: /* block: '{' stmt_list '}'  */
#line 1246 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4600 "raku.tab.c"
    break;

  case 256: /* block: '{' YADA '}'  */
#line 1247 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4606 "raku.tab.c"
    break;

  case 257: /* block: '{' stmt_list expr '}'  */
#line 1249 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4612 "raku.tab.c"
    break;

  case 258: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1251 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4618 "raku.tab.c"
    break;

  case 259: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1253 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4624 "raku.tab.c"
    break;

  case 260: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1255 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4630 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1257 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4636 "raku.tab.c"
    break;

  case 262: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1259 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4643 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1262 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4649 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1264 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4655 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1266 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4661 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1268 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4667 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1270 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4674 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1273 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4681 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1276 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4689 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1280 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4696 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1283 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4703 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1286 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4710 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1289 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4716 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1291 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4724 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1295 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4731 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1298 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4738 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1301 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4745 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1304 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4752 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1307 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4759 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1310 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4765 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1312 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4771 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1314 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4777 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_LAST '}'  */
#line 1316 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4783 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1318 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4789 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1320 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4795 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1322 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4801 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1324 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4807 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4813 "raku.tab.c"
    break;

  case 289: /* closure: '{' expr '}'  */
#line 1329 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 4819 "raku.tab.c"
    break;

  case 290: /* expr: VAR_SCALAR '=' expr  */
#line 1332 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 4825 "raku.tab.c"
    break;

  case 291: /* expr: KW_GATHER block  */
#line 1333 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 4835 "raku.tab.c"
    break;

  case 292: /* expr: tern_expr  */
#line 1338 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4841 "raku.tab.c"
    break;

  case 293: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1342 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 4847 "raku.tab.c"
    break;

  case 294: /* tern_expr: cmp_expr  */
#line 1343 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 4853 "raku.tab.c"
    break;

  case 295: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1346 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4859 "raku.tab.c"
    break;

  case 296: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1347 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4865 "raku.tab.c"
    break;

  case 297: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1348 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 4871 "raku.tab.c"
    break;

  case 298: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1349 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 4877 "raku.tab.c"
    break;

  case 299: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1350 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 4883 "raku.tab.c"
    break;

  case 300: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1351 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 4889 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1352 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 4895 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1353 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 4901 "raku.tab.c"
    break;

  case 303: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1354 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4907 "raku.tab.c"
    break;

  case 304: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1355 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4913 "raku.tab.c"
    break;

  case 305: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1356 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4919 "raku.tab.c"
    break;

  case 306: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1357 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4925 "raku.tab.c"
    break;

  case 307: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1358 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4931 "raku.tab.c"
    break;

  case 308: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1359 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4937 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1361 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 4947 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1367 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 4957 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1373 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 4967 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr  */
#line 1378 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 4973 "raku.tab.c"
    break;

  case 313: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1381 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4979 "raku.tab.c"
    break;

  case 314: /* divis_expr: jct_expr  */
#line 1382 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 4985 "raku.tab.c"
    break;

  case 315: /* jct_expr: jct_expr '|' range_expr  */
#line 1385 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4991 "raku.tab.c"
    break;

  case 316: /* jct_expr: jct_expr '&' range_expr  */
#line 1386 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 4997 "raku.tab.c"
    break;

  case 317: /* jct_expr: dor_expr  */
#line 1387 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5003 "raku.tab.c"
    break;

  case 318: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1391 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5009 "raku.tab.c"
    break;

  case 319: /* dor_expr: range_expr  */
#line 1392 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5015 "raku.tab.c"
    break;

  case 320: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1395 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5021 "raku.tab.c"
    break;

  case 321: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1396 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5027 "raku.tab.c"
    break;

  case 322: /* range_expr: add_expr  */
#line 1397 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5033 "raku.tab.c"
    break;

  case 323: /* add_expr: add_expr '~' repl_expr  */
#line 1400 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5039 "raku.tab.c"
    break;

  case 324: /* add_expr: repl_expr  */
#line 1401 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5045 "raku.tab.c"
    break;

  case 325: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1404 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5051 "raku.tab.c"
    break;

  case 326: /* repl_expr: addsub_expr  */
#line 1405 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5057 "raku.tab.c"
    break;

  case 327: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1408 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5063 "raku.tab.c"
    break;

  case 328: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1409 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5069 "raku.tab.c"
    break;

  case 329: /* addsub_expr: mul_expr  */
#line 1410 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5075 "raku.tab.c"
    break;

  case 330: /* mul_expr: mul_expr '*' unary_expr  */
#line 1413 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5081 "raku.tab.c"
    break;

  case 331: /* mul_expr: mul_expr '/' unary_expr  */
#line 1414 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5087 "raku.tab.c"
    break;

  case 332: /* mul_expr: mul_expr '%' unary_expr  */
#line 1415 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5093 "raku.tab.c"
    break;

  case 333: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1416 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5099 "raku.tab.c"
    break;

  case 334: /* mul_expr: unary_expr  */
#line 1417 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5105 "raku.tab.c"
    break;

  case 335: /* unary_expr: '-' unary_expr  */
#line 1420 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5111 "raku.tab.c"
    break;

  case 336: /* unary_expr: '!' unary_expr  */
#line 1421 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5117 "raku.tab.c"
    break;

  case 337: /* unary_expr: CARET unary_expr  */
#line 1422 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5123 "raku.tab.c"
    break;

  case 338: /* unary_expr: OP_REDUCE unary_expr  */
#line 1424 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5132 "raku.tab.c"
    break;

  case 339: /* unary_expr: pow_expr  */
#line 1428 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5138 "raku.tab.c"
    break;

  case 340: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1431 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5144 "raku.tab.c"
    break;

  case 341: /* pow_expr: postfix_expr  */
#line 1432 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5150 "raku.tab.c"
    break;

  case 342: /* scalar_list: VAR_SCALAR  */
#line 1435 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5156 "raku.tab.c"
    break;

  case 343: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1436 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5162 "raku.tab.c"
    break;

  case 344: /* meth_name: IDENT  */
#line 1439 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5168 "raku.tab.c"
    break;

  case 345: /* meth_name: KW_SORT  */
#line 1440 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5174 "raku.tab.c"
    break;

  case 346: /* meth_name: KW_MAP  */
#line 1441 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5180 "raku.tab.c"
    break;

  case 347: /* meth_name: KW_GREP  */
#line 1442 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5186 "raku.tab.c"
    break;

  case 348: /* meth_name: KW_SAY  */
#line 1443 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5192 "raku.tab.c"
    break;

  case 349: /* meth_name: KW_PRINT  */
#line 1444 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5198 "raku.tab.c"
    break;

  case 350: /* meth_name: KW_TAKE  */
#line 1445 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5204 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_RETURN  */
#line 1446 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5210 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_EXISTS  */
#line 1447 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5216 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_DELETE  */
#line 1448 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5222 "raku.tab.c"
    break;

  case 354: /* meth_name: TESTOP  */
#line 1449 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5228 "raku.tab.c"
    break;

  case 355: /* postfix_expr: call_expr  */
#line 1451 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5234 "raku.tab.c"
    break;

  case 356: /* call_expr: IDENT '(' arg_list ')'  */
#line 1454 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5243 "raku.tab.c"
    break;

  case 357: /* call_expr: IDENT '(' ')'  */
#line 1458 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5249 "raku.tab.c"
    break;

  case 358: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1460 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5258 "raku.tab.c"
    break;

  case 359: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1465 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5264 "raku.tab.c"
    break;

  case 360: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1467 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5274 "raku.tab.c"
    break;

  case 361: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1473 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5284 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1479 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5292 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '.' KW_NEW  */
#line 1483 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5300 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '.' IDENT  */
#line 1487 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5309 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1492 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5320 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1499 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5329 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '.' CARET IDENT  */
#line 1504 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5339 "raku.tab.c"
    break;

  case 368: /* call_expr: atom '.' CARET IDENT  */
#line 1510 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5349 "raku.tab.c"
    break;

  case 369: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1516 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5360 "raku.tab.c"
    break;

  case 370: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1523 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5371 "raku.tab.c"
    break;

  case 371: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1530 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5380 "raku.tab.c"
    break;

  case 372: /* call_expr: atom '.' meth_name  */
#line 1535 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5389 "raku.tab.c"
    break;

  case 373: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1540 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5400 "raku.tab.c"
    break;

  case 374: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1547 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5409 "raku.tab.c"
    break;

  case 375: /* call_expr: call_expr '.' meth_name  */
#line 1552 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5418 "raku.tab.c"
    break;

  case 376: /* call_expr: KW_DIE expr  */
#line 1557 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5424 "raku.tab.c"
    break;

  case 377: /* call_expr: KW_MAP closure expr  */
#line 1559 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5430 "raku.tab.c"
    break;

  case 378: /* call_expr: KW_GREP closure expr  */
#line 1561 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5436 "raku.tab.c"
    break;

  case 379: /* call_expr: KW_SORT expr  */
#line 1563 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5442 "raku.tab.c"
    break;

  case 380: /* call_expr: KW_SORT closure expr  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5448 "raku.tab.c"
    break;

  case 381: /* call_expr: atom  */
#line 1566 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5454 "raku.tab.c"
    break;

  case 382: /* arg_list: expr  */
#line 1569 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5460 "raku.tab.c"
    break;

  case 383: /* arg_list: arg_list ',' expr  */
#line 1570 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5466 "raku.tab.c"
    break;

  case 384: /* atom: LIT_INT  */
#line 1573 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5472 "raku.tab.c"
    break;

  case 385: /* atom: LIT_FLOAT  */
#line 1574 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5478 "raku.tab.c"
    break;

  case 386: /* atom: LIT_STR  */
#line 1575 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5484 "raku.tab.c"
    break;

  case 387: /* atom: WORDLIST  */
#line 1577 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5496 "raku.tab.c"
    break;

  case 388: /* atom: LIT_INTERP_STR  */
#line 1584 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5502 "raku.tab.c"
    break;

  case 389: /* atom: VAR_SCALAR  */
#line 1585 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5508 "raku.tab.c"
    break;

  case 390: /* atom: OP_INC VAR_SCALAR  */
#line 1586 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5514 "raku.tab.c"
    break;

  case 391: /* atom: OP_DEC VAR_SCALAR  */
#line 1587 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5520 "raku.tab.c"
    break;

  case 392: /* atom: VAR_SCALAR OP_INC  */
#line 1588 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5526 "raku.tab.c"
    break;

  case 393: /* atom: VAR_SCALAR OP_DEC  */
#line 1589 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5532 "raku.tab.c"
    break;

  case 394: /* atom: VAR_ARRAY  */
#line 1590 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5538 "raku.tab.c"
    break;

  case 395: /* atom: VAR_HASH  */
#line 1591 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5544 "raku.tab.c"
    break;

  case 396: /* atom: VAR_CAPTURE  */
#line 1593 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5552 "raku.tab.c"
    break;

  case 397: /* atom: VAR_FH  */
#line 1597 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5560 "raku.tab.c"
    break;

  case 398: /* atom: VAR_NAMED_CAPTURE  */
#line 1601 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5567 "raku.tab.c"
    break;

  case 399: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1604 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5573 "raku.tab.c"
    break;

  case 400: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1606 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5579 "raku.tab.c"
    break;

  case 401: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1608 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5585 "raku.tab.c"
    break;

  case 402: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1610 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5591 "raku.tab.c"
    break;

  case 403: /* atom: VAR_HASH '{' expr '}'  */
#line 1612 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5597 "raku.tab.c"
    break;

  case 404: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1614 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5603 "raku.tab.c"
    break;

  case 405: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1616 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5609 "raku.tab.c"
    break;

  case 406: /* atom: IDENT  */
#line 1617 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5615 "raku.tab.c"
    break;

  case 407: /* atom: VAR_TWIGIL  */
#line 1619 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5623 "raku.tab.c"
    break;

  case 408: /* atom: VAR_ARRAY_TWIGIL  */
#line 1623 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5631 "raku.tab.c"
    break;

  case 409: /* atom: VAR_HASH_TWIGIL  */
#line 1627 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5639 "raku.tab.c"
    break;

  case 410: /* atom: '(' ')'  */
#line 1630 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5645 "raku.tab.c"
    break;

  case 411: /* atom: '(' expr ')'  */
#line 1631 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5651 "raku.tab.c"
    break;

  case 412: /* atom: '(' expr ',' ')'  */
#line 1633 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5657 "raku.tab.c"
    break;

  case 413: /* atom: '(' expr ',' arg_list ')'  */
#line 1635 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5664 "raku.tab.c"
    break;

  case 414: /* atom: block  */
#line 1637 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5670 "raku.tab.c"
    break;

  case 415: /* atom: KW_SUB block  */
#line 1638 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5676 "raku.tab.c"
    break;


#line 5680 "raku.tab.c"

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

#line 1640 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
