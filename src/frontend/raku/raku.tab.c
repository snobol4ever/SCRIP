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
#line 11 "raku.y"

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
static ExprList *exprlist_new(void) {
    ExprList *l = calloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = realloc(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
static void exprlist_free(ExprList *l) { if (l) { free(l->items); free(l); } }
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}
static tree_t *var_node(const char *name) {
    return leaf_sval(TT_VAR, strip_sigil(name));
}
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
static tree_t *make_call(const char *name) {
    tree_t *e = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(e, n);
    return e;
}
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
static const char *rk_multi_mangle(const char *base, ExprList *params) {
    static char buf[512]; int np = params ? params->count : 0;
    int pos = snprintf(buf, sizeof buf, "%s$%d", base, np);
    for (int i = 0; i < np; i++) { tree_t *p = params->items[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        if (!strcmp(ty, "*@") || !strcmp(ty, "**@")) ty = "Slurpy";
        char safe[64]; int j = 0;
        for (const char *c = ty; *c && j < 63; c++, j++) safe[j] = (*c == ':') ? '_' : *c; safe[j] = 0;
        pos += snprintf(buf + pos, sizeof buf - pos, "$%s", safe); }
    return intern(buf);
}
static tree_t *rk_typed_param(const char *type, const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p;
}
static tree_t *rk_typed_def_param(const char *type, const char *def, const char *name) {
    char buf[160]; snprintf(buf, sizeof buf, "%s%s", type, def);
    return rk_typed_param(intern(buf), name);
}
static tree_t *make_seq(ExprList *stmts) {
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    if (stmts) {
        for (int i = 0; i < stmts->count; i++) expr_add_child(seq, stmts->items[i]);
        exprlist_free(stmts);
    }
    return seq;
}
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
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
static tree_t *rk_range_ex(tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return expr_binary(TT_TO, lo, d); }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
static tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_TO || rhs->n < 2) return rhs;
    tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]);
    return call;
}
static tree_t *rk_arr_index(const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]);
        return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, idx); return c;
}
static tree_t *rk_arr_pick(const char *arr, tree_t *i0, ExprList *rest) {
    tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(arr)); expr_add_child(call, i0);
    if (rest) { for (int i = 0; i < rest->count; i++) expr_add_child(call, rest->items[i]); exprlist_free(rest); }
    return call;
}
static tree_t *rk_arr_end_index(const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
static tree_t *rk_dec(tree_t *hi) {
    if (hi && hi->t == TT_ILIT) { tree_t *d = ast_node_new(TT_ILIT); d->v.ival = hi->v.ival - 1; return d; }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_SUB, hi, one);
}
static tree_t *rk_arr_all(const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *lo = ast_node_new(TT_ILIT); lo->v.ival = 0;
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(arr)); expr_add_child(call, lo); expr_add_child(call, rk_dec(el));
    return call;
}
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
static tree_t *rk_slurpy_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("*@"))); return p;
}
static tree_t *rk_slurpy_lol_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("**@"))); return p;
}
static tree_t *rk_slurpy_named_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("*%"))); return p;
}
static tree_t *rk_param_default(tree_t *p, tree_t *dflt) {
    return expr_binary(TT_ASSIGN, p, dflt);
}
static tree_t *rk_scalar_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_XREP || rhs->n < 2) return rhs;
    tree_t *c = make_call("__rk_rep"); expr_add_child(c, rhs->c[0]); expr_add_child(c, rhs->c[1]); return c;
}
static tree_t *rk_named_call(const char *fname, ExprList *pos, ExprList *named) {
    tree_t *c = make_call("__rk_named_call");
    expr_add_child(c, leaf_sval(TT_QLIT, fname));
    tree_t *n = ast_node_new(TT_ILIT); n->v.ival = pos ? pos->count : 0;
    expr_add_child(c, n);
    if (pos) { for (int i = 0; i < pos->count; i++) expr_add_child(c, pos->items[i]); exprlist_free(pos); }
    if (named) { for (int i = 0; i < named->count; i++) expr_add_child(c, named->items[i]); exprlist_free(named); }
    return c;
}
static tree_t *rk_defaults_prologue(ExprList *params, tree_t *body) {
    if (!params) return body;
    ExprList *pro = NULL;
    for (int i = 0; i < params->count; i++) {
        tree_t *p = params->items[i];
        if (!p || p->t != TT_ASSIGN || p->n < 2) continue;
        tree_t *pv = p->c[0]; tree_t *dv = p->c[1];
        params->items[i] = pv;
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_tree_clone(pv)); ast_push(mc, leaf_sval(TT_QLIT, "defined"));
        tree_t *un = ast_node_new(TT_UNLESS); ast_push(un, mc); ast_push(un, seq1(expr_binary(TT_ASSIGN, rk_tree_clone(pv), dv)));
        if (!pro) pro = exprlist_new();
        exprlist_append(pro, un);
    }
    if (!pro) return body;
    for (int i = 0; body && i < body->n; i++) exprlist_append(pro, body->c[i]);
    return make_seq(pro);
}
static int rk_is_chain_cmp(tree_e k) {
    return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE;
}
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return expr_binary(TT_SEQ, left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
}
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
const char *raku_meth_lookup(const char *classname, const char *methname) {
    char key[128];
    snprintf(key, sizeof key, "%s::%s", classname, methname);
    for (int i = 0; i < raku_meth_ntypes; i++)
        if (strcmp(raku_meth_table[i].key, key) == 0)
            return raku_meth_table[i].procname;
    return NULL;
}

#line 380 "raku.tab.c"

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
  YYSYMBOL_SLURPY_POS = 65,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 66,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 67,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 68,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 69,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 70,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 71,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 72,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 73,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 74,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 75,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 76,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 77,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 78,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 79,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 80,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 81,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 82,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 83,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 84,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 85,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 86,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 87,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 88,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 89,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 90,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 91,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 92,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 93,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 94,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 95,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 96,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 97,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 98,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 99,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 100,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 101,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 102,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 103,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 104,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 105,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 106,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 107,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 108,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 109,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 110,                   /* OP_POW  */
  YYSYMBOL_111_ = 111,                     /* '='  */
  YYSYMBOL_112_ = 112,                     /* '!'  */
  YYSYMBOL_113_ = 113,                     /* '<'  */
  YYSYMBOL_114_ = 114,                     /* '>'  */
  YYSYMBOL_115_ = 115,                     /* '|'  */
  YYSYMBOL_116_ = 116,                     /* '&'  */
  YYSYMBOL_117_ = 117,                     /* '~'  */
  YYSYMBOL_118_ = 118,                     /* '+'  */
  YYSYMBOL_119_ = 119,                     /* '-'  */
  YYSYMBOL_120_ = 120,                     /* '*'  */
  YYSYMBOL_121_ = 121,                     /* '/'  */
  YYSYMBOL_122_ = 122,                     /* '%'  */
  YYSYMBOL_UMINUS = 123,                   /* UMINUS  */
  YYSYMBOL_124_ = 124,                     /* '.'  */
  YYSYMBOL_125_ = 125,                     /* ';'  */
  YYSYMBOL_126_ = 126,                     /* '('  */
  YYSYMBOL_127_ = 127,                     /* ')'  */
  YYSYMBOL_128_ = 128,                     /* ','  */
  YYSYMBOL_129_ = 129,                     /* '['  */
  YYSYMBOL_130_ = 130,                     /* ']'  */
  YYSYMBOL_131_ = 131,                     /* '{'  */
  YYSYMBOL_132_ = 132,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 133,                 /* $accept  */
  YYSYMBOL_program = 134,                  /* program  */
  YYSYMBOL_stmt_list = 135,                /* stmt_list  */
  YYSYMBOL_stmt = 136,                     /* stmt  */
  YYSYMBOL_if_stmt = 137,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 138,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 139,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 140,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 141,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 142,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 143,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 144,                /* loop_incr  */
  YYSYMBOL_for_stmt = 145,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 146,               /* given_stmt  */
  YYSYMBOL_when_list = 147,                /* when_list  */
  YYSYMBOL_sub_decl = 148,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 149,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 150,                 /* sub_body  */
  YYSYMBOL_method_body = 151,              /* method_body  */
  YYSYMBOL_class_decl = 152,               /* class_decl  */
  YYSYMBOL_role_decl = 153,                /* role_decl  */
  YYSYMBOL_is_clauses = 154,               /* is_clauses  */
  YYSYMBOL_class_body_list = 155,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 156,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 157,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 158,           /* named_arg_list  */
  YYSYMBOL_pair_list = 159,                /* pair_list  */
  YYSYMBOL_param_list = 160,               /* param_list  */
  YYSYMBOL_block = 161,                    /* block  */
  YYSYMBOL_closure = 162,                  /* closure  */
  YYSYMBOL_expr = 163,                     /* expr  */
  YYSYMBOL_tern_expr = 164,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 165,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 166,               /* divis_expr  */
  YYSYMBOL_jct_expr = 167,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 168,                 /* dor_expr  */
  YYSYMBOL_range_expr = 169,               /* range_expr  */
  YYSYMBOL_add_expr = 170,                 /* add_expr  */
  YYSYMBOL_repl_expr = 171,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 172,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 173,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 174,               /* unary_expr  */
  YYSYMBOL_pow_expr = 175,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 176,              /* scalar_list  */
  YYSYMBOL_meth_name = 177,                /* meth_name  */
  YYSYMBOL_postfix_expr = 178,             /* postfix_expr  */
  YYSYMBOL_call_expr = 179,                /* call_expr  */
  YYSYMBOL_arg_list = 180,                 /* arg_list  */
  YYSYMBOL_atom = 181                      /* atom  */
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
#define YYLAST   4227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  437
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1101

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   366


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
       2,     2,     2,   112,     2,     2,     2,   122,   116,     2,
     126,   127,   120,   118,   128,   119,   124,   121,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   125,
     113,   111,   114,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   129,     2,   130,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   131,   115,   132,   117,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   123
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   394,   394,   405,   406,   409,   411,   413,   415,   419,
     421,   423,   425,   429,   432,   436,   438,   442,   445,   449,
     451,   453,   455,   457,   459,   463,   467,   469,   471,   473,
     475,   477,   479,   481,   483,   485,   487,   489,   491,   493,
     495,   498,   501,   503,   506,   509,   511,   513,   515,   517,
     522,   526,   530,   535,   539,   540,   542,   544,   546,   548,
     551,   553,   555,   557,   560,   563,   566,   569,   572,   574,
     576,   578,   580,   583,   585,   587,   589,   592,   595,   599,
     602,   604,   606,   608,   611,   614,   617,   620,   623,   626,
     629,   630,   631,   632,   633,   634,   635,   637,   639,   641,
     643,   644,   645,   646,   647,   648,   649,   651,   653,   655,
     657,   658,   659,   660,   663,   665,   667,   669,   671,   673,
     675,   677,   681,   683,   685,   687,   689,   691,   695,   697,
     701,   703,   705,   707,   711,   713,   717,   719,   721,   725,
     727,   729,   733,   736,   742,   748,   753,   759,   764,   769,
     777,   788,   789,   795,   803,   809,   815,   823,   829,   835,
     844,   851,   860,   869,   878,   885,   892,   893,   896,   899,
     901,   903,   905,   907,   909,   911,   913,   916,   918,   920,
     922,   925,   928,   934,   935,   936,   939,   942,   944,   946,
     948,   950,   952,   954,   956,   959,   961,   963,   965,   968,
     971,   977,   993,  1008,  1009,  1021,  1035,  1036,  1039,  1042,
    1045,  1048,  1051,  1054,  1057,  1060,  1063,  1066,  1069,  1072,
    1079,  1086,  1093,  1100,  1104,  1108,  1112,  1116,  1123,  1130,
    1137,  1144,  1154,  1162,  1170,  1179,  1186,  1193,  1203,  1211,
    1221,  1236,  1237,  1242,  1247,  1254,  1258,  1264,  1266,  1268,
    1270,  1274,  1275,  1276,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,
    1293,  1296,  1297,  1298,  1300,  1302,  1304,  1306,  1308,  1311,
    1313,  1315,  1317,  1319,  1322,  1325,  1329,  1332,  1335,  1338,
    1340,  1344,  1347,  1350,  1353,  1356,  1359,  1361,  1363,  1365,
    1367,  1369,  1371,  1373,  1375,  1379,  1382,  1383,  1384,  1389,
    1392,  1394,  1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,
    1405,  1406,  1407,  1408,  1409,  1410,  1411,  1417,  1423,  1429,
    1432,  1433,  1436,  1437,  1438,  1441,  1443,  1446,  1447,  1448,
    1451,  1452,  1455,  1456,  1459,  1460,  1461,  1464,  1465,  1466,
    1467,  1468,  1471,  1472,  1473,  1474,  1479,  1482,  1483,  1486,
    1487,  1490,  1491,  1492,  1493,  1494,  1495,  1496,  1497,  1498,
    1499,  1500,  1502,  1504,  1509,  1510,  1512,  1514,  1519,  1521,
    1527,  1533,  1537,  1541,  1546,  1553,  1558,  1564,  1570,  1577,
    1584,  1589,  1594,  1601,  1606,  1611,  1613,  1615,  1617,  1619,
    1621,  1624,  1625,  1626,  1629,  1630,  1631,  1632,  1641,  1642,
    1643,  1644,  1645,  1646,  1647,  1648,  1649,  1653,  1657,  1660,
    1662,  1664,  1666,  1668,  1670,  1672,  1674,  1676,  1678,  1679,
    1683,  1687,  1691,  1692,  1693,  1695,  1698,  1699
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
  "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED", "KW_HANDLES", "WORDLIST",
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

#define YYPACT_NINF (-797)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-437)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -797,    11,  1630,  -797,  -797,  -797,  -797,  -797,   639,   -75,
     -68,   -73,   367,  -797,  -797,  4052,  -797,  -797,  -797,    44,
    1703,   196,  3030,  3103,  3176,  3249,  3322,    -4,   -28,  3322,
    1776,   746,  3322,   110,   141,  3395,  3468,   -28,   -47,    22,
      24,    45,    45,  3541,   -28,   -28,  3322,   293,   315,   205,
    4052,  -797,  -797,   332,   232,   347,  4052,  4052,  -797,  1849,
     327,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,
    -797,   491,  -797,  -797,  -797,  4103,   565,  -797,   635,   686,
     118,   284,  -797,    42,   324,    -7,   393,  -797,  -797,   331,
     335,   350,   436,  -797,  -797,  3322,  3322,  3322,  3322,  3322,
    3322,   469,  1923,  3614,  1997,   490,  3322,  3322,    18,  2070,
      85,   389,   131,  -797,   -28,  -797,  -797,   403,   214,   220,
    -797,  2144,  -797,    84,   275,   283,   326,   848,   552,   776,
     613,  1849,   339,  1849,    13,  1849,   -28,  1849,   -28,    20,
     245,   412,  -797,  -797,   506,  -797,   519,   542,   572,   546,
     228,   250,  1849,   -28,  1849,   -28,   306,  2218,  -797,  3322,
    3322,  -797,  3322,  3322,  -797,  3322,  3322,  3322,  2291,  3322,
    -797,   643,  -797,  -797,  -797,   582,   723,    23,  -797,   615,
    -797,  -797,  -797,  -797,  -797,   465,   719,  1072,  3322,  3322,
    3322,  3322,  3322,  3322,  3322,  3322,  -797,  3322,  3322,  3322,
    3322,  3322,  3322,  3322,  3322,  -797,  4052,  4052,  4052,  4052,
    4052,  4052,  4052,  4052,  4052,  4052,  4052,  4052,  4052,  4052,
    4052,   860,  4052,  4052,  4052,  4052,  4052,  4052,  4052,  4052,
    4052,  4052,  4052,  4052,  4052,  4052,  4052,   681,   637,   574,
     644,   740,   748,   752,   759,   800,   262,  -797,   606,  1849,
     208,   736,   409,   838,   769,   824,   859,   973,   864,     0,
    -797,   734,   755,  1997,   996,  3322,  -797,  3322,  3322,   898,
     465,   763,  -797,  3322,  3322,  -797,  3687,  -797,  3760,  -797,
     343,   381,   397,   480,   913,   914,  -797,   765,   768,  3322,
    3322,  3322,  3322,  3322,  3322,  3322,  -797,  3322,   770,  3322,
    3322,  -797,  3322,   772,   876,   778,  -797,  1016,  3322,  -797,
    4052,  4052,     3,  -797,  -797,  -797,  -797,  3322,  3322,  -797,
    1013,  3322,  1014,  3322,   780,  1002,   784,  -797,  3322,  3322,
    1020,   906,   908,   909,   910,   915,   907,  -797,  -797,  -797,
     -28,    -6,  -797,    15,   916,   917,  -797,  -797,  2364,  -797,
     722,   255,   261,   930,  3030,  3103,  3322,   825,   277,   302,
    -797,   375,   919,   920,   921,   922,   923,   924,   934,   935,
     936,   937,   939,   940,   941,   943,   947,   948,   951,   951,
     951,   951,   951,   951,   980,   951,   951,   951,   951,   951,
     951,   951,   951,  -797,  -797,  -797,   118,  -797,  -797,  -797,
     962,   962,   324,    -7,   393,   393,  -797,  -797,  -797,  -797,
    -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,
    -797,  -797,   954,  1067,   974,  -797,  2438,  -797,  -797,  -797,
    -797,  -797,  -797,  3322,  2512,  -797,   787,  3322,  -797,  3322,
    3322,  3322,  3322,   988,   998,   999,  -797,  2586,  -797,  2660,
    3322,  -797,  1097,  -797,  3833,   687,  1015,  1005,  -797,  -797,
    -797,   987,  -797,  1003,  1849,   233,  1053,   180,  2734,   242,
    1017,  3322,  -797,  3906,  -797,  3322,  -797,    41,  -797,  3322,
    3322,  1027,  1129,  2364,  1018,  1019,  1021,  1022,  1023,  1024,
    1025,   301,  2364,  1026,  1028,   596,   -28,    -8,  3979,  -797,
     -28,   -28,   152,   -18,   101,  1029,  -797,    39,  -797,  -797,
    -797,  1030,   789,  1236,  1031,  1032,    37,  1038,  1033,  1040,
    1034,   -28,   -28,   -28,  1035,  1037,  1044,  3322,  -797,  -797,
    -797,  -797,  -797,  -797,  1144,  1145,  -797,   -14,  1030,   792,
      50,   244,    98,  -797,   794,  1149,  1997,  3322,  3322,   308,
     113,   -54,  -797,   265,  3322,  3322,  -797,  3322,  3322,  -797,
    3322,  3322,  3322,  3322,  3322,  3322,  3322,  3322,  -797,  -797,
    -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,
    -797,  -797,  -797,  -797,  -797,  4052,  2808,  -797,  2882,  1039,
     799,  1042,  -797,   801,  2364,  1043,   619,  1045,  1046,   704,
    3322,  3322,  3322,  -797,   803,  -797,   806,   811,  -797,  1092,
     813,  -797,  -797,  -797,  -797,  -797,   815,  3322,  -797,  3322,
    3322,  3322,   818,  -797,   291,  -797,  1047,  1849,   663,  1048,
    1030,   820,  1049,  1052,  3322,  -797,   827,  -797,  -797,  -797,
    -797,  -797,  -797,  -797,  -797,   831,  -797,  -797,  -797,   933,
    -797,  -797,  1849,   -28,  -797,  -797,  1168,  -797,  1169,  1170,
    3322,  1070,  1172,  1173,  -797,  1030,   414,  3030,  3103,  1433,
    -797,   417,  -797,  -797,  3322,   -28,  -797,  -797,  -797,  1060,
    1061,  1160,  -797,  -797,  -797,  -797,  3322,  1063,  -797,  -797,
      30,   588,   798,  1133,  -797,  -797,  1030,  1030,   835,   837,
    1176,  1178,  1179,  -797,  -797,   272,   750,  1062,   333,  3322,
    3322,  3322,  3322,  3322,  3322,  -797,  -797,  -797,  -797,   338,
     378,   387,   391,   404,   408,   416,   422,   442,   443,   444,
     447,  -797,  -797,   854,  -797,   856,   861,  -797,  1071,  -797,
    -797,   865,  -797,  -797,  -797,  -797,  -797,  1074,  1075,  1076,
    -797,  -797,  -797,  3322,  -797,  2364,  1077,   678,  -797,  -797,
    1080,  1118,  1130,  -797,   867,  -797,  3322,  -797,  -797,  1030,
    -797,  -797,   679,  1081,  1083,    -8,  -797,   869,   971,   -28,
     -28,   -28,  -797,  3322,  -797,  -797,  -797,  1098,  -797,    43,
    -797,  -797,  -797,   377,   128,  -797,   452,  3322,  3322,  3322,
    3322,  3322,  3322,  3322,  3322,  -797,   -28,  1078,  -797,  -797,
     -28,  1086,  3322,  -797,   571,   594,    69,   114,   751,  1087,
    1088,   681,  -797,  -797,  1030,  1030,  1207,  1208,  1209,  3322,
    2956,  1106,  1107,  -797,   455,   456,   457,   471,   482,   492,
    -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,
    -797,  -797,  -797,  -797,  -797,  -797,  1094,  -797,  -797,  -797,
    -797,   873,  -797,  -797,  -797,  3322,  3322,  2364,   691,  -797,
    -797,  3322,  -797,  -797,  -797,  -797,   -28,   -28,  -797,  -797,
    -797,  -797,  -797,  3322,  1109,  1211,  1212,  3322,  3322,  3322,
    -797,  -797,  -797,   495,   501,   503,   509,   511,   515,   517,
     520,  -797,  -797,  -797,  3322,  1096,  -797,    78,  1152,  -797,
     203,  -797,  1214,  1215,  1100,  3322,  -797,  1216,  1218,  1101,
    3322,  -797,   156,   207,  1108,  1110,  -797,  -797,   721,  -797,
    -797,  -797,  -797,  -797,   523,  1102,   875,  3322,  3322,  -797,
    -797,  -797,  -797,  -797,  -797,  -797,  1111,  -797,  -797,   877,
    -797,   725,   979,  -797,  -797,  3322,  -797,  -797,   531,   532,
     543,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  1112,
     -28,  1113,   881,  1131,  1360,  1113,   885,  1120,  1148,  -797,
    1150,  1151,  1167,  -797,  1177,  1224,  1229,  1181,  3322,  -797,
    1260,  1279,  1182,  3322,  -797,  -797,  -797,   212,  -797,  -797,
    -797,  1162,   544,   548,  -797,  1183,  -797,   -28,  -797,  -797,
    -797,  -797,  -797,  3322,  -797,  -797,  1113,  -797,  3030,  3103,
    1506,  -797,   446,  -797,  1113,  -797,  -797,  -797,  -797,  -797,
    -797,  1185,  1186,  -797,  1187,  1188,  1189,  -797,  1190,  1113,
     887,  -797,  -797,  -797,  -797,  -797,  1171,  -797,   458,   317,
    -797,   549,  3322,  3322,  3322,  3322,  3322,  3322,  3322,  3322,
    -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  1113,
     -28,  3322,  3322,  3322,  -797,  -797,  -797,   557,   560,   566,
     569,   604,   620,   630,   652,  -797,  -797,   657,   660,   662,
    -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,  -797,
    -797
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   404,   405,   406,   408,   409,   414,
     415,   429,   428,   430,   431,     0,   416,   417,   418,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   423,   407,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   309,   311,   329,
     331,   334,   336,   339,   341,   343,   346,   351,   356,   358,
     372,   400,     0,   412,   413,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     409,   414,   415,   429,     0,   436,   354,     0,   409,   414,
      35,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     339,     0,   437,   308,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   136,     0,   139,     0,
       0,   104,     0,     0,   105,     0,     0,     0,     3,     0,
     398,    96,    98,   395,   203,     0,     0,     0,   355,     0,
     410,   411,   353,   352,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,   378,     0,     0,
     307,     0,     0,     0,     0,     0,   383,     0,   382,   428,
     374,     0,     0,     0,     0,     0,    30,     0,     0,   432,
       0,     0,    38,   403,     0,     6,     0,     9,     0,    10,
       0,     0,     0,     0,     0,     0,   359,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,    42,     0,     0,   117,     0,   129,     0,     0,   148,
       0,     0,     0,     3,   155,    45,    46,     0,     0,   151,
       0,     0,     0,     0,     0,   132,     0,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   396,   397,   399,
       0,     0,   206,     0,     0,     0,   241,   433,     0,   272,
     409,   414,   415,   429,     0,     0,     0,     0,     0,     0,
     271,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   314,   315,
     318,   319,   312,   313,     0,   316,   317,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   330,   332,   333,   335,
     337,   338,   340,   342,   344,   345,   350,   347,   348,   349,
     357,   361,   371,   365,   366,   367,   368,   369,   370,   363,
     364,   362,   394,     0,   391,    51,     0,    85,    86,    87,
      88,    89,    48,     0,     0,   377,     0,     0,    15,     0,
       0,     0,     0,   419,   424,   425,    53,     0,   386,     0,
       0,   375,     0,   373,   403,     0,     0,     0,   306,   307,
      37,     0,   402,     0,     0,     0,   406,   428,     0,     0,
       0,     0,    27,     0,    28,     0,    29,     0,   158,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,   120,
     433,     0,     0,   337,   338,   251,   252,     0,   265,   267,
     269,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,   433,     0,     0,     0,     0,   106,   107,
     108,   109,   305,    97,     0,     0,   206,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,     0,     0,     0,
       0,     0,   298,     0,     0,     0,   299,     0,     0,   300,
       0,     0,     0,     0,     0,     0,     0,     0,   273,    55,
      57,    59,    62,    60,    61,    56,    58,    68,    70,    72,
      75,    73,    74,    69,    71,     0,     0,   387,     0,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   385,     0,   381,     0,     0,   245,     0,
       0,   419,   424,   425,    36,     5,     0,     0,    11,     0,
       0,     0,     0,    20,     0,    19,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   360,     0,    76,    79,    78,
      82,    80,    81,    77,    40,     0,    83,    84,    43,   114,
     119,   118,     0,     0,   128,   145,     0,   147,     0,     0,
       0,   254,     0,     0,   154,     0,     0,     0,     0,     0,
     166,     0,    32,    31,     0,     0,   149,   426,   427,     0,
       0,   130,   133,   134,   137,   138,     0,     0,   204,   205,
       0,     0,     0,     0,   202,   163,     0,     0,     0,     0,
       0,     0,     0,   240,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   289,   296,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   310,   393,     0,   390,     0,     0,    50,     0,    52,
     164,     0,    17,    16,   422,   421,   420,     0,     0,     0,
     384,   379,   380,     0,   376,     0,     0,     0,   248,   247,
       0,     0,     0,    22,     0,    23,     0,    26,   157,     0,
      34,    33,     0,   435,   435,     0,   121,     0,   122,     0,
       0,     0,   261,     0,   255,   256,   153,   257,   253,     0,
     266,   268,   270,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,    66,    67,
       0,     0,     0,   201,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   160,     0,     0,     0,     0,     0,     0,
       0,   419,   425,   293,     0,     0,     0,     0,     0,     0,
     301,   302,   303,   304,   274,   276,   278,   281,   279,   280,
     275,   277,   392,   389,   388,    49,   435,    63,    64,    65,
     246,     0,    13,    12,    21,     0,     0,     0,     0,   156,
       7,     0,    41,    44,   116,   115,   433,     0,   126,   146,
     143,   144,   263,     0,   258,     0,     0,     0,     0,     0,
     170,   171,   168,     0,     0,     0,     0,     0,     0,     0,
       0,   152,   150,   131,     0,     0,   142,     0,     3,   236,
       0,   233,     0,     0,     0,     0,   212,     0,     0,     0,
       0,   207,     0,     0,     0,     0,   208,   209,     0,   159,
     161,   242,   243,   244,     0,   165,     0,     0,     0,   283,
     285,   288,   286,   287,   284,    18,   435,   250,   249,     0,
      24,     0,   123,   124,   262,     0,   259,   260,     0,     0,
       0,   172,   174,   176,   179,   177,   178,   173,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,   213,   210,   211,     0,   239,   292,
     291,   164,     0,     0,    14,   435,     8,     0,   127,   264,
     180,   182,   181,     0,   141,   235,     0,   184,     0,     0,
       0,   183,     0,   232,     0,   221,   222,   216,   219,   220,
     215,     0,     0,   225,     0,     0,     0,   226,     0,     0,
       0,   290,   294,   295,    25,   125,     0,   234,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     185,   231,   229,   230,   218,   227,   228,   217,   238,     0,
       0,     0,     0,     0,   188,   189,   186,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   140,     0,     0,     0,
     190,   192,   194,   197,   195,   196,   191,   193,   198,   200,
     199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -797,  -797,     1,  -797,  -493,  -644,  -797,  -797,  -797,  -797,
    -797,   288,  -797,  -797,  -797,  -797,  -797,  -274,  -796,  -797,
    -797,  -797,   761,  -797,  -797,  -442,   836,  -337,   257,   977,
      -2,  -209,  -797,   760,  1095,  -797,   647,   -11,  1090,  1091,
     791,    16,  -797,  -797,  -236,  -797,  -797,   -62,  -797
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   187,    61,    62,   499,    63,    64,    65,    66,
      67,   905,    68,    69,   516,    70,    71,   314,   909,    72,
      73,   341,   537,    74,   542,   261,   469,   512,   115,   166,
     122,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   287,   422,    89,    90,   123,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,     2,   424,   384,   650,   776,   539,   606,   534,   478,
     141,     3,   610,   505,   506,   140,   535,   507,    24,   911,
     132,   134,   136,   138,   139,   505,   506,   144,   146,   507,
     149,   116,   256,   153,   155,   257,   103,   344,   107,   299,
     248,   170,   691,   692,   173,   105,   693,   262,   159,   661,
     162,   505,   506,   884,   104,   507,   300,   185,   117,   271,
     505,   506,   658,   106,   507,   160,   178,   163,   508,   509,
     510,   315,   182,   183,   674,   675,   258,   450,   717,   157,
     508,   509,   510,   912,    60,   345,   691,   692,   505,   506,
     693,   913,   507,   240,   241,   242,   243,   244,   245,   228,
     307,   250,   252,    60,   254,   255,   508,   509,   510,   662,
     663,   230,   231,   885,   886,   508,   509,   510,   694,   270,
     226,   227,   150,    60,   108,   536,   109,    60,   917,   288,
     511,   298,   998,   303,   878,   305,   918,   914,   301,   299,
     631,   302,   538,   508,   509,   510,   735,   161,   308,   164,
     324,    60,   326,   151,   299,   331,   300,   332,   333,   228,
     334,   335,   813,   336,   337,   338,   336,   339,   630,   676,
     985,   300,   700,   701,   702,  1015,   165,   697,   986,  1023,
     915,   659,   919,    93,    94,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   916,   370,   371,   372,   373,   374,
     375,   376,   377,   698,   699,   971,   124,   125,   126,   272,
     127,   102,   273,   505,   506,   400,   401,   507,   228,   176,
    1047,   990,   505,   506,   987,   920,   507,   128,  1061,   991,
     703,   129,   656,   223,   224,   491,   177,   664,   301,   921,
     495,   302,   180,  1068,   264,   716,   502,   436,   406,   407,
     408,   409,   410,   301,   505,   506,   302,   621,   507,    75,
     891,   455,   265,   457,   695,   458,   459,   988,   508,   509,
     510,   462,   463,  1085,   465,   992,   470,   508,   509,   510,
     273,   989,   874,    60,   142,   143,   544,   484,   485,   486,
     487,   488,   489,   490,   156,   158,   761,   493,   494,   503,
     504,   171,   172,   554,   108,   762,   109,   174,  1008,   508,
     509,   510,    93,    94,   513,   514,   515,   437,   993,   518,
     555,   520,   130,   310,   311,   267,   524,   525,   557,   175,
     975,   268,   994,   438,   709,   328,   439,   290,   710,  1039,
     102,   320,   617,   299,   711,   558,   179,   712,   713,   263,
     329,   714,   549,   550,   551,   553,   768,   181,   618,   321,
     300,   619,   228,   322,   590,   289,   103,   623,   290,   291,
     624,   142,   593,   433,   105,   292,   731,   596,   293,   294,
     599,   323,   295,   829,   546,   604,   274,   607,   434,   225,
     316,   786,   547,   304,   276,   306,   309,   718,   830,   186,
     275,   560,   161,   887,   561,   562,   290,   888,   277,   556,
     325,   563,   327,   292,   564,   565,   293,   294,   566,   567,
     889,   636,   822,   823,   787,   788,   644,   164,   789,   273,
     645,   591,   229,   296,   559,   595,   297,   278,   597,   598,
     715,   236,   301,   797,    75,   302,   798,   799,   608,  1075,
     239,   279,   462,   800,   471,   815,   801,   802,   446,   237,
     803,   804,   616,   528,   296,   833,   185,   297,   472,   626,
     840,   628,  1052,   629,   238,  1053,  1054,   632,   633,   790,
     791,   792,  1055,   246,  1071,  1056,  1057,   290,  1072,  1058,
    1059,   108,   473,   109,   292,   869,   653,   293,   294,   232,
     205,  1073,   296,   529,   253,   297,   474,   568,   475,   890,
     841,   671,   530,   233,   234,   235,   531,   188,   263,   842,
     189,   190,   476,   843,   733,   687,   736,   191,   266,   577,
     192,   193,   741,   578,   194,   195,   844,   442,   312,   443,
     845,   579,   205,   313,   706,   707,   708,   580,   846,   805,
     929,   930,   719,   720,   847,   721,   722,   757,   723,   724,
     725,   726,   727,   728,   729,   730,   283,   581,   582,   583,
     972,   205,   584,   976,   848,   849,   850,   316,  1060,   851,
     637,   639,   640,   296,   892,   928,   297,   939,   940,   941,
    1074,   197,   347,   348,   198,   199,   641,   533,   747,   748,
     749,   200,   411,   942,   201,   202,   477,   642,   203,   204,
     412,   313,   413,   414,   943,   756,   196,   643,   758,   759,
     577,   415,   416,   286,   944,   764,   578,   961,   579,   417,
     418,   315,   772,   962,   580,   963,   581,   419,   420,   421,
     582,   964,   583,   965,   316,   584,   814,   966,   739,   967,
     777,   411,   968,   317,   423,   999,   637,   639,   782,   412,
    1040,   413,   414,  1010,  1011,   793,   794,   796,   643,   857,
     415,   416,   806,   859,   316,  1012,  1042,   319,   417,   418,
    1043,  1076,   577,   318,   811,   578,   419,   420,   421,  1090,
     205,   579,  1091,   861,   580,   411,   340,   907,  1092,   425,
     426,  1093,   908,   412,   868,   413,   414,   834,   835,   836,
     837,   838,   839,   342,   415,   416,   206,   207,   208,   209,
     910,   648,   417,   418,   273,   908,   210,   211,   212,   581,
     419,   420,   421,   435,   273,    92,  1094,    93,    94,    95,
      96,    97,    98,    99,   743,   582,   346,   273,   213,   214,
     100,   860,  1095,   649,   651,   583,   147,   654,   655,   657,
     148,   922,  1096,   101,   923,   102,   924,   925,   936,   427,
      75,   215,   216,   217,   218,   219,   220,   584,   681,   682,
     683,   882,   637,   221,  1097,   639,   284,   643,   765,  1098,
     285,   766,  1099,   222,  1100,   893,   894,   895,   896,   897,
     898,   899,   900,   863,   870,   949,   273,   871,   816,   951,
     906,   817,   818,   819,   820,   442,   950,   611,    92,   273,
      93,    94,    95,    96,    97,    98,    99,   934,     4,     5,
       6,     7,   273,   100,   746,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,   545,   997,   102,   343,
    1006,   349,   908,   273,   440,   441,   114,    28,   280,   281,
     282,   451,   452,   947,   948,   428,    33,   393,   394,   395,
     397,   398,   399,   429,    41,    42,    43,   430,   442,    46,
     831,   954,   453,   454,   431,   958,   959,   960,    50,    51,
     461,   273,   481,   482,    52,   347,   483,   347,   492,   496,
     348,   445,   969,   497,   498,   500,   348,   521,   348,   974,
     778,   523,   348,   980,   347,   594,   665,   666,   984,   696,
     666,   704,   273,    54,    55,   432,   738,   273,   740,   273,
     750,   273,   807,   751,   452,  1002,  1003,    56,   752,   273,
     754,   452,   347,   755,    57,   760,   624,   769,   666,   446,
     145,    59,   444,  1009,   773,   273,    60,   552,   774,   273,
     775,   498,   824,   666,   825,   666,   378,   379,   380,   381,
     382,   383,  1022,   385,   386,   387,   388,   389,   390,   391,
     392,   852,   273,   853,   452,   447,  1034,   448,   854,   273,
     449,  1038,   856,   273,   347,   867,   876,   348,   877,   498,
     946,   273,  1001,   273,  1005,   273,  1007,   498,  1016,   666,
     456,   906,  1024,   666,  1069,   666,  1048,  1049,  1051,   167,
     169,   404,   405,   460,   479,   480,   501,   517,   519,   522,
     526,   527,   875,   528,   529,   530,   879,   880,   881,   532,
     531,   548,   540,   541,   569,   570,   571,   572,   573,   574,
    1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,   222,   575,
     576,   577,   578,   901,   579,   580,   581,   903,   582,  1087,
    1088,  1089,   583,   584,   585,     4,     5,     6,     7,   228,
     586,   587,   350,   351,   352,   353,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   354,   355,    24,   600,
     588,    25,    26,    27,    28,   356,   357,    31,    32,   601,
     602,   609,   614,    33,    34,    35,    36,    37,    38,   358,
     359,    41,    42,    43,    44,    45,    46,    47,   615,   612,
     620,    48,    49,   952,   953,    50,    51,   613,   634,   635,
     660,    52,   625,   637,   638,    53,   639,   640,   641,   642,
     643,   646,   677,   647,   679,   686,   672,   673,   688,   689,
     684,   313,   685,   705,   737,   678,   680,   739,   742,   753,
      54,    55,   763,   767,   770,   744,   745,   771,   779,   780,
     781,   783,   784,   785,    56,   808,   809,   810,   812,   821,
     826,    57,   827,   828,   832,   865,   855,    58,    59,   857,
     858,   859,   862,    60,   360,   864,   872,   866,   873,   883,
     902,   904,   926,   927,   931,   932,   933,   937,   938,   945,
     955,   956,   957,   970,   973,   979,   983,  1014,   977,   978,
     981,    75,   982,   995,  1000,   996,  1004,  1013,  1031,     4,
       5,     6,     7,  1032,   908,  1025,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     667,   668,    24,  1017,  1045,    25,    26,    27,    28,    29,
     669,    31,    32,  1026,  1035,  1027,  1028,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,  1029,  1036,  1041,    48,    49,   690,  1070,    50,
      51,  1046,  1030,     0,   622,    52,  1033,  1037,  1044,    53,
    1062,  1063,  1064,  1065,  1066,  1067,     0,   396,   402,     0,
     403,     0,     0,     0,     0,     0,     0,  1086,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,    58,    59,     4,     5,     6,     7,    60,   670,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,  1018,  1019,    24,     0,     0,    25,
      26,    27,    28,    29,  1020,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,    53,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,    58,    59,    46,     0,     0,
       0,    60,  1021,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,   145,    59,
      46,     0,     0,     0,    60,   795,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,   145,    59,     4,     5,     6,     7,    60,  1050,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,     0,     0,    25,
      26,    27,    28,    29,    30,    31,    32,     0,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,     0,     0,     0,    48,
      49,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,    53,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,    58,    59,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,   120,   121,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,   145,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   184,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     247,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,   259,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,   251,     0,    41,
      42,    43,     0,    59,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   260,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,   330,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   269,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,   186,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   543,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   589,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   592,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,   259,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   603,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,   466,
       7,     0,     0,     0,   118,   119,   112,   113,   467,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   605,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   184,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,   259,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   732,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   734,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   935,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   131,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   133,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   135,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   137,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   152,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   154,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,   168,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
     249,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,   466,     7,     0,     0,     0,
     118,   119,   112,   113,   467,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   464,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,   259,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   468,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,    59,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   627,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   110,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,     0,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   652,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -436,  -436,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,     0,    59,     0,
       0,  -436,  -436,    60,  -436,  -436,  -436,  -436,  -436,  -436,
    -436,  -436,  -436,  -436,  -436,  -436,  -436,     0,     0,     0,
    -436,     0,     0,     0,     0,     0,     0,     0,  -436,  -436,
    -436,  -436,     0,  -436,     0,     0,  -436,  -436,  -436,  -436,
    -436,  -436,     0,  -436,  -436,  -436,     0,  -436
};

static const yytype_int16 yycheck[] =
{
       2,     0,   238,   212,   497,   649,   343,   449,    14,   283,
      14,     0,   454,    10,    11,    26,    22,    14,    26,   815,
      22,    23,    24,    25,    26,    10,    11,    29,    30,    14,
      32,    15,    14,    35,    36,    17,   111,    14,   111,    26,
     102,    43,    56,    57,    46,   113,    60,   109,    26,    10,
      26,    10,    11,    10,   129,    14,    43,    59,    14,   121,
      10,    11,    80,   131,    14,    43,    50,    43,    65,    66,
      67,   125,    56,    57,    37,    38,    58,    77,   132,   126,
      65,    66,    67,    14,   131,    62,    56,    57,    10,    11,
      60,    22,    14,    95,    96,    97,    98,    99,   100,   117,
      80,   103,   104,   131,   106,   107,    65,    66,    67,    70,
      71,   118,   119,    70,    71,    65,    66,    67,   132,   121,
      78,    79,    12,   131,   124,   131,   126,   131,    14,   131,
     127,   133,   928,   135,   778,   137,    22,    68,   125,    26,
     477,   128,   127,    65,    66,    67,   588,   125,   128,   125,
     152,   131,   154,    12,    26,   157,    43,   159,   160,   117,
     162,   163,   132,   165,   166,   167,   168,   169,   127,   132,
      14,    43,    74,    75,    76,   971,   131,   127,    22,   975,
     111,    80,    68,    98,    99,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   125,   197,   198,   199,   200,   201,
     202,   203,   204,   540,   541,   127,    10,    11,    12,   125,
      14,   126,   128,    10,    11,   226,   227,    14,   117,    14,
    1016,    14,    10,    11,    68,   111,    14,    31,  1024,    22,
     132,    35,    80,   115,   116,   297,    31,   511,   125,   125,
     302,   128,    10,  1039,   113,   132,   308,   249,   232,   233,
     234,   235,   236,   125,    10,    11,   128,    77,    14,     2,
     132,   263,   131,   265,   538,   267,   268,   111,    65,    66,
      67,   273,   274,  1069,   276,    68,   278,    65,    66,    67,
     128,   125,   775,   131,    27,    28,   348,   289,   290,   291,
     292,   293,   294,   295,    37,    38,     5,   299,   300,   310,
     311,    44,    45,    26,   124,    14,   126,    14,   952,    65,
      66,    67,    98,    99,   313,   317,   318,   109,   111,   321,
      43,   323,   126,    78,    79,   111,   328,   329,    26,    14,
     127,   111,   125,   125,    26,    29,   128,    29,    30,   127,
     126,   113,   109,    26,    36,    43,    14,    39,    40,   129,
      44,    43,   354,   355,   356,   357,   630,    10,   125,   131,
      43,   128,   117,   113,   426,    26,   111,   125,    29,    30,
     128,   114,   434,   111,   113,    36,   585,   439,    39,    40,
     442,   131,    43,   111,   129,   447,   111,   449,   126,   105,
     125,   665,   131,   136,   111,   138,   139,   132,   126,    72,
     125,    26,   125,    26,    29,    30,    29,    30,   125,   132,
     153,    36,   155,    36,    39,    40,    39,    40,    43,    44,
      43,   483,   696,   697,    10,    11,   125,   125,    14,   128,
     492,   433,   108,   125,   132,   437,   128,   111,   440,   441,
     132,   110,   125,    26,   187,   128,    29,    30,   450,   132,
      14,   125,   454,    36,   111,   691,    39,    40,   125,   124,
      43,    44,   464,   125,   125,   132,   468,   128,   125,   471,
     132,   473,    26,   475,   124,    29,    30,   479,   480,    65,
      66,    67,    36,    14,    26,    39,    40,    29,    30,    43,
      44,   124,   111,   126,    36,   769,   498,    39,    40,   106,
     125,    43,   125,   125,    14,   128,   125,   132,   111,   132,
     132,   513,   125,   120,   121,   122,   125,    26,   129,   132,
      29,    30,   125,   132,   586,   527,   588,    36,   125,   125,
      39,    40,   594,   125,    43,    44,   132,   128,   126,   130,
     132,   125,   125,   131,   546,   547,   548,   125,   132,   132,
     824,   825,   554,   555,   132,   557,   558,   619,   560,   561,
     562,   563,   564,   565,   566,   567,    14,   125,   125,   125,
     907,   125,   125,   910,   132,   132,   132,   125,   132,   132,
     125,   125,   125,   125,   132,   821,   128,   132,   132,   132,
     132,    26,   127,   128,    29,    30,   125,   340,   600,   601,
     602,    36,    14,   132,    39,    40,   126,   125,    43,    44,
      22,   131,    24,    25,   132,   617,   125,   125,   620,   621,
     125,    33,    34,    10,   132,   627,   125,   132,   125,    41,
      42,   125,   634,   132,   125,   132,   125,    49,    50,    51,
     125,   132,   125,   132,   125,   125,    58,   132,   125,   132,
     652,    14,   132,   111,    17,   132,   125,   125,   660,    22,
     997,    24,    25,   132,   132,   667,   668,   669,   125,   125,
      33,    34,   674,   125,   125,   132,   132,   131,    41,    42,
     132,   132,   125,   111,   686,   125,    49,    50,    51,   132,
     125,   125,   132,   755,   125,    14,    53,   126,   132,   125,
     126,   132,   131,    22,   766,    24,    25,   709,   710,   711,
     712,   713,   714,   131,    33,    34,    81,    82,    83,    84,
     126,   125,    41,    42,   128,   131,    91,    92,    93,   125,
      49,    50,    51,   127,   128,    96,   132,    98,    99,   100,
     101,   102,   103,   104,   125,   125,   131,   128,   113,   114,
     111,   753,   132,   496,   497,   125,    10,   500,   501,   502,
      14,    10,   132,   124,    13,   126,    15,    16,   830,   125,
     513,    85,    86,    87,    88,    89,    90,   125,   521,   522,
     523,   783,   125,    97,   132,   125,    10,   125,   125,   132,
      14,   128,   132,   107,   132,   797,   798,   799,   800,   801,
     802,   803,   804,   125,   125,   867,   128,   128,    10,   871,
     812,    13,    14,    15,    16,   128,   125,   130,    96,   128,
      98,    99,   100,   101,   102,   103,   104,   829,     3,     4,
       5,     6,   128,   111,   130,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   124,   126,   126,   126,
     125,   132,   131,   128,   118,   119,    31,    32,    10,    11,
      12,   127,   128,   865,   866,   125,    41,     7,     8,     9,
     223,   224,   225,   125,    49,    50,    51,   125,   128,    54,
     130,   883,   127,   128,   125,   887,   888,   889,    63,    64,
     127,   128,   127,   128,    69,   127,   128,   127,   128,   127,
     128,   132,   904,    27,    28,   127,   128,   127,   128,   908,
     653,   127,   128,   915,   127,   128,   127,   128,   920,   127,
     128,   127,   128,    98,    99,   125,   127,   128,   127,   128,
     127,   128,   675,   127,   128,   937,   938,   112,   127,   128,
     127,   128,   127,   128,   119,   127,   128,   127,   128,   125,
     125,   126,   114,   955,   127,   128,   131,   132,   127,   128,
      27,    28,   127,   128,   127,   128,   206,   207,   208,   209,
     210,   211,   974,   213,   214,   215,   216,   217,   218,   219,
     220,   127,   128,   127,   128,   126,   988,    14,   127,   128,
     126,   993,   127,   128,   127,   128,   127,   128,    27,    28,
     127,   128,   127,   128,   127,   128,    27,    28,   127,   128,
      14,  1013,   127,   128,   127,   128,  1018,  1019,  1020,    42,
      43,   230,   231,   125,   111,   111,    10,    14,    14,    27,
      10,   125,   775,   125,   125,   125,   779,   780,   781,   132,
     125,   111,   126,   126,   125,   125,   125,   125,   125,   125,
    1052,  1053,  1054,  1055,  1056,  1057,  1058,  1059,   107,   125,
     125,   125,   125,   806,   125,   125,   125,   810,   125,  1071,
    1072,  1073,   125,   125,    94,     3,     4,     5,     6,   117,
     126,    14,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   111,
     126,    29,    30,    31,    32,    33,    34,    35,    36,   111,
     111,    14,   125,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   125,   114,
      77,    59,    60,   876,   877,    63,    64,   132,   111,    10,
     111,    69,   125,   125,   125,    73,   125,   125,   125,   125,
     125,   125,   114,   125,   114,   111,   125,   125,    14,    14,
     125,   131,   125,    14,   125,   132,   132,   125,   125,    77,
      98,    99,   125,   125,   125,   130,   130,   125,    10,    10,
      10,   111,    10,    10,   112,   125,   125,    27,   125,    56,
      14,   119,    14,    14,   132,    77,   125,   125,   126,   125,
     125,   125,   125,   131,   132,   125,   125,    77,   125,   111,
     132,   125,   125,   125,     7,     7,     7,   111,   111,   125,
     111,    10,    10,   127,    72,   125,   125,   970,    14,    14,
      14,   974,    14,   125,   132,   125,   125,   125,    14,     3,
       4,     5,     6,    14,   131,   125,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   132,  1007,    29,    30,    31,    32,    33,
      34,    35,    36,   125,    14,   125,   125,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,   125,    14,   132,    59,    60,   536,   127,    63,
      64,  1013,   125,    -1,   468,    69,   125,   125,   125,    73,
     125,   125,   125,   125,   125,   125,    -1,   222,   228,    -1,
     229,    -1,    -1,    -1,    -1,    -1,    -1,  1070,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,   125,   126,     3,     4,     5,     6,   131,   132,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    73,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,   125,   126,    54,    -1,    -1,
      -1,   131,   132,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,   125,   126,
      54,    -1,    -1,    -1,   131,   132,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,
      -1,   125,   126,     3,     4,     5,     6,   131,   132,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,    -1,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    -1,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    73,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,   125,   126,    54,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,   125,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,   125,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    -1,    41,    -1,    -1,    -1,   119,    -1,
      -1,    -1,    49,    50,    51,   126,   127,    54,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,
      -1,    -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,
     127,    54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    41,    -1,    -1,    -1,    -1,   119,   120,    -1,    49,
      50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    41,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    49,    50,    51,   126,   127,    54,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    23,    98,    99,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    41,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,    51,
     126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    72,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    41,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,    51,
     126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    41,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    49,    50,    51,   126,   127,    54,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,   127,
      54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,
      50,    51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    -1,    41,    -1,    -1,    -1,   119,
      -1,    -1,    -1,    49,    50,    51,   126,   127,    54,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    41,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,    51,
     126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    -1,    41,    -1,    -1,    -1,   119,    -1,    -1,
      -1,    49,    50,    51,   126,   127,    54,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    41,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    49,    50,    51,   126,   127,
      54,    -1,    -1,   131,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,
      50,    51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,    54,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,
      -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,
     126,    54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,
      -1,    41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,
      50,    51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,
      -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,    -1,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    78,    79,   131,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    -1,    -1,    -1,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   106,
     107,   108,    -1,   110,    -1,    -1,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,   122,    -1,   124
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   134,   135,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    69,    73,    98,    99,   112,   119,   125,   126,
     131,   136,   137,   139,   140,   141,   142,   143,   145,   146,
     148,   149,   152,   153,   156,   161,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   178,
     179,   181,    96,    98,    99,   100,   101,   102,   103,   104,
     111,   124,   126,   111,   129,   113,   131,   111,   124,   126,
      10,    11,    12,    13,    31,   161,   174,    14,    10,    11,
     125,   126,   163,   180,    10,    11,    12,    14,    31,    35,
     126,   126,   163,   126,   163,   126,   163,   126,   163,   163,
     170,    14,   161,   161,   163,   125,   163,    10,    14,   163,
      12,    12,   126,   163,   126,   163,   161,   126,   161,    26,
      43,   125,    26,    43,   125,   131,   162,   162,   131,   162,
     163,   161,   161,   163,    14,    14,    14,    31,   174,    14,
      10,    10,   174,   174,   127,   163,    72,   135,    26,    29,
      30,    36,    39,    40,    43,    44,   125,    26,    29,    30,
      36,    39,    40,    43,    44,   125,    81,    82,    83,    84,
      91,    92,    93,   113,   114,    85,    86,    87,    88,    89,
      90,    97,   107,   115,   116,   105,    78,    79,   117,   108,
     118,   119,   106,   120,   121,   122,   110,   124,   124,    14,
     163,   163,   163,   163,   163,   163,    14,   127,   180,   126,
     163,   120,   163,    14,   163,   163,    14,    17,    58,    14,
     127,   158,   180,   129,   113,   131,   125,   111,   111,   127,
     163,   180,   125,   128,   111,   125,   111,   125,   111,   125,
      10,    11,    12,    14,    10,    14,    10,   176,   163,    26,
      29,    30,    36,    39,    40,    43,   125,   128,   163,    26,
      43,   125,   128,   163,   161,   163,   161,    80,   128,   161,
      78,    79,   126,   131,   150,   125,   125,   111,   111,   131,
     113,   131,   113,   131,   163,   161,   163,   161,    29,    44,
      23,   163,   163,   163,   163,   163,   163,   163,   163,   163,
      53,   154,   131,   126,    14,    62,   131,   127,   128,   132,
      10,    11,    12,    13,    24,    25,    33,    34,    47,    48,
     132,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   166,   166,
     166,   166,   166,   166,   164,   166,   166,   166,   166,   166,
     166,   166,   166,     7,     8,     9,   167,   169,   169,   169,
     170,   170,   171,   172,   173,   173,   174,   174,   174,   174,
     174,    14,    22,    24,    25,    33,    34,    41,    42,    49,
      50,    51,   177,    17,   177,   125,   126,   125,   125,   125,
     125,   125,   125,   111,   126,   127,   163,   109,   125,   128,
     118,   119,   128,   130,   114,   132,   125,   126,    14,   126,
      77,   127,   128,   127,   128,   163,    14,   163,   163,   163,
     125,   127,   163,   163,   126,   163,     5,    14,   126,   159,
     163,   111,   125,   111,   125,   111,   125,   126,   150,   111,
     111,   127,   128,   128,   163,   163,   163,   163,   163,   163,
     163,   180,   128,   163,   163,   180,   127,    27,    28,   138,
     127,    10,   180,   170,   170,    10,    11,    14,    65,    66,
      67,   127,   160,   135,   163,   163,   147,    14,   163,    14,
     163,   127,    27,   127,   163,   163,    10,   125,   125,   125,
     125,   125,   132,   161,    14,    22,   131,   155,   127,   160,
     126,   126,   157,   127,   180,   124,   129,   131,   111,   163,
     163,   163,   132,   163,    26,    43,   132,    26,    43,   132,
      26,    29,    30,    36,    39,    40,    43,    44,   132,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,    94,   126,    14,   126,   127,
     180,   163,   127,   180,   128,   163,   180,   163,   163,   180,
     111,   111,   111,   127,   180,   127,   158,   180,   163,    14,
     158,   130,   114,   132,   125,   125,   163,   109,   125,   128,
      77,    77,   159,   125,   128,   125,   163,   126,   163,   163,
     127,   160,   163,   163,   111,    10,   180,   125,   125,   125,
     125,   125,   125,   125,   125,   180,   125,   125,   125,   161,
     137,   161,   126,   163,   161,   161,    80,   161,    80,    80,
     111,    10,    70,    71,   150,   127,   128,    24,    25,    34,
     132,   163,   125,   125,    37,    38,   132,   114,   132,   114,
     132,   161,   161,   161,   125,   125,   111,   163,    14,    14,
     155,    56,    57,    60,   132,   150,   127,   127,   160,   160,
      74,    75,    76,   132,   127,    14,   163,   163,   163,    26,
      30,    36,    39,    40,    43,   132,   132,   132,   132,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   164,   127,   180,   127,   158,   180,   125,   127,   125,
     127,   180,   125,   125,   130,   130,   130,   163,   163,   163,
     127,   127,   127,    77,   127,   128,   163,   180,   163,   163,
     127,     5,    14,   125,   163,   125,   128,   125,   150,   127,
     125,   125,   163,   127,   127,    27,   138,   163,   161,    10,
      10,    10,   163,   111,    10,    10,   150,    10,    11,    14,
      65,    66,    67,   163,   163,   132,   163,    26,    29,    30,
      36,    39,    40,    43,    44,   132,   163,   161,   125,   125,
      27,   163,   125,   132,    58,   177,    10,    13,    14,    15,
      16,    56,   150,   150,   127,   127,    14,    14,    14,   111,
     126,   130,   132,   132,   163,   163,   163,   163,   163,   163,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132,   132,   127,   127,   127,   125,   127,   125,   125,   125,
     163,   180,   125,   125,   125,    77,    77,   128,   180,   150,
     125,   128,   125,   125,   137,   161,   127,    27,   138,   161,
     161,   161,   163,   111,    10,    70,    71,    26,    30,    43,
     132,   132,   132,   163,   163,   163,   163,   163,   163,   163,
     163,   161,   132,   161,   125,   144,   163,   126,   131,   151,
     126,   151,    14,    22,    68,   111,   125,    14,    22,    68,
     111,   125,    10,    13,    15,    16,   125,   125,   177,   150,
     150,     7,     7,     7,   163,   127,   180,   111,   111,   132,
     132,   132,   132,   132,   132,   125,   127,   163,   163,   180,
     125,   180,   161,   161,   163,   111,    10,    10,   163,   163,
     163,   132,   132,   132,   132,   132,   132,   132,   132,   163,
     127,   127,   160,    72,   135,   127,   160,    14,    14,   125,
     163,    14,    14,   125,   163,    14,    22,    68,   111,   125,
      14,    22,    68,   111,   125,   125,   125,   126,   151,   132,
     132,   127,   163,   163,   125,   127,   125,    27,   138,   163,
     132,   132,   132,   125,   161,   151,   127,   132,    24,    25,
      34,   132,   163,   151,   127,   125,   125,   125,   125,   125,
     125,    14,    14,   125,   163,    14,    14,   125,   163,   127,
     160,   132,   132,   132,   125,   161,   144,   151,   163,   163,
     132,   163,    26,    29,    30,    36,    39,    40,    43,    44,
     132,   151,   125,   125,   125,   125,   125,   125,   151,   127,
     127,    26,    30,    43,   132,   132,   132,   163,   163,   163,
     163,   163,   163,   163,   163,   151,   161,   163,   163,   163,
     132,   132,   132,   132,   132,   132,   132,   132,   132,   132,
     132
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   133,   134,   135,   135,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   137,   137,   137,   137,   137,   137,
     137,   137,   138,   138,   138,   138,   138,   138,   139,   139,
     140,   140,   140,   140,   141,   141,   142,   142,   142,   143,
     143,   143,   144,   145,   145,   145,   145,   145,   145,   146,
     146,   147,   147,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   149,   149,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   152,   153,   154,   154,   154,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     156,   157,   157,   157,   157,   158,   158,   159,   159,   159,
     159,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   162,   163,   163,   163,   163,
     164,   164,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     166,   166,   167,   167,   167,   168,   168,   169,   169,   169,
     170,   170,   171,   171,   172,   172,   172,   173,   173,   173,
     173,   173,   174,   174,   174,   174,   174,   175,   175,   176,
     176,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   178,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   180,   180,   180,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181
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
       5,     1,     1,     3,     2,     3,     3,     3,     4,     5,
       5,     3,     5,     4,     6,     1,     3,     1,     3,     1,
       3,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       9,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     3,     2,     1,
       5,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     1,     2,     2,     2,     2,     1,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     4,     6,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     2,     3,     3,     2,     3,
       1,     1,     3,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     4,
       6,     6,     6,     1,     4,     4,     5,     5,     1,     1,
       1,     1,     2,     3,     4,     5,     1,     2
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
#line 395 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2893 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 405 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2899 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 406 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2905 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 410 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2911 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 412 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2917 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 414 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2923 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 416 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2931 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 420 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2937 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 422 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2943 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 424 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2949 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 426 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2957 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 430 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2964 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 433 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2972 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 437 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2978 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 439 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2986 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 443 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2993 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 446 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3001 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 450 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3007 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 452 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3013 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 454 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3019 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 456 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3025 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 458 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3031 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 460 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3039 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 464 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3047 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 468 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3053 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 470 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3059 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 472 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3065 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 474 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3071 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 476 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3077 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 478 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3083 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 480 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3089 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 482 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3095 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 484 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3101 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 486 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3107 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 488 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3113 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 490 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3119 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 492 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3125 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 494 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3131 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 496 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3138 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 499 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3145 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 502 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3151 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 504 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3158 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3165 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 510 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3171 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 512 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3177 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 514 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3183 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 516 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3189 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 518 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3198 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 523 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3206 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 527 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3214 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 531 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3223 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 536 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3231 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 539 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3237 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 541 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3243 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 543 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3249 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 545 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3255 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3261 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 549 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3268 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 552 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3274 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 554 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3280 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 556 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3286 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 558 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3293 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 561 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3300 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 564 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3307 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 567 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3314 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 570 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3321 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 573 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3327 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 575 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3333 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 577 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3339 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3345 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 581 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3352 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 584 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3358 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 586 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3364 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 588 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3370 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 590 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3377 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 593 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3384 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 596 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3392 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 600 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3399 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 603 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3405 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 605 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3411 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 607 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3417 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 609 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3424 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 612 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3431 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 615 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3438 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 618 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3445 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 621 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3452 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 624 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3459 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 627 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3466 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 629 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3472 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 630 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3478 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 631 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3484 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 632 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3490 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 633 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3496 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 634 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3502 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 636 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3508 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 638 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3514 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 640 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3520 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 642 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3526 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 643 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3532 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 644 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3538 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 645 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3544 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 646 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3550 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 647 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3556 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 648 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3562 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 650 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3568 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3574 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3580 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3586 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 657 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3592 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 658 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3598 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 659 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3604 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 660 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3610 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3616 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 666 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3622 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 668 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3628 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3634 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3640 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3646 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3652 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3658 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3664 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 684 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 686 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3676 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3682 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3688 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3694 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 696 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3700 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 698 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3706 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 702 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 704 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3718 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 706 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3724 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3730 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 712 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3742 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3748 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3754 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3760 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 726 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3766 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 728 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3772 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 730 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3778 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 733 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3784 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 737 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3794 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 743 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3804 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 749 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3813 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 754 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3823 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 760 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3832 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 765 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3839 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 770 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3851 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 778 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3864 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 788 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3870 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 790 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3878 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 796 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3890 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 804 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3900 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 810 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3910 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 816 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3922 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 824 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3932 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 830 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3942 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 836 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3955 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 845 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3966 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 852 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3979 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 861 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3992 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 870 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4003 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 879 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4014 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 886 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4023 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 892 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4029 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 894 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4036 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 897 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4043 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 900 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4049 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 902 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4055 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 904 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4061 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 906 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4067 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 908 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4073 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 910 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4079 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 912 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4085 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 914 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4092 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 917 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4098 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 919 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4104 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 921 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4110 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 923 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4117 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 926 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4124 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 929 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4132 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 934 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4138 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 935 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4144 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 937 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4151 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 940 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4158 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 943 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4164 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 945 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4170 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 947 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4176 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 949 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4182 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 951 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4188 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 953 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4194 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 955 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4200 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 957 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4207 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 960 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4213 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 962 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4219 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 964 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4225 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 966 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4232 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 969 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4239 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 972 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4247 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 978 "raku.y"
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
#line 4265 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 994 "raku.y"
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
#line 4282 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 1008 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4288 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 1010 "raku.y"
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
#line 4304 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1022 "raku.y"
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
#line 4320 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1035 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4326 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1037 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4333 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1040 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4340 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1043 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4347 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1046 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4354 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1049 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4361 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1052 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4368 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1055 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4375 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1058 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4382 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1061 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4389 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1064 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4396 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1067 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4403 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1070 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4410 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1073 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4421 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1080 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4432 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1087 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4443 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1094 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4454 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1101 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4462 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1105 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4470 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1109 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4478 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1113 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4486 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1117 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4497 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1124 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4508 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1131 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4519 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1138 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4530 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1145 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4544 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1155 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4556 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1163 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4568 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1171 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4581 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1180 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4592 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1187 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4603 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1194 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4617 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1204 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4629 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1212 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4641 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1222 "raku.y"
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
#line 4658 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1236 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4664 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1238 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4673 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1243 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4682 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1248 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4691 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1255 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4699 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1259 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4707 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1265 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4713 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1267 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4719 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1269 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4725 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1271 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4731 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1274 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4737 "raku.tab.c"
    break;

  case 252: /* param_list: VAR_ARRAY  */
#line 1275 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4743 "raku.tab.c"
    break;

  case 253: /* param_list: param_list ',' VAR_ARRAY  */
#line 1276 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4749 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT VAR_SCALAR  */
#line 1277 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4755 "raku.tab.c"
    break;

  case 255: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1278 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4761 "raku.tab.c"
    break;

  case 256: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1279 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4767 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' VAR_SCALAR  */
#line 1280 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4773 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1281 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4779 "raku.tab.c"
    break;

  case 259: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1282 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4785 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1283 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4791 "raku.tab.c"
    break;

  case 261: /* param_list: VAR_SCALAR '=' expr  */
#line 1284 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4797 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1285 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4803 "raku.tab.c"
    break;

  case 263: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1286 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4809 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1287 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4815 "raku.tab.c"
    break;

  case 265: /* param_list: SLURPY_POS  */
#line 1288 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4821 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' SLURPY_POS  */
#line 1289 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4827 "raku.tab.c"
    break;

  case 267: /* param_list: SLURPY_LOL  */
#line 1290 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4833 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' SLURPY_LOL  */
#line 1291 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4839 "raku.tab.c"
    break;

  case 269: /* param_list: SLURPY_NAMED  */
#line 1292 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4845 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1293 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4851 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list '}'  */
#line 1296 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4857 "raku.tab.c"
    break;

  case 272: /* block: '{' YADA '}'  */
#line 1297 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4863 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list expr '}'  */
#line 1299 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4869 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1301 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4875 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1303 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4881 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1305 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4887 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1307 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4893 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1309 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4900 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1312 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4906 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1314 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4912 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1316 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4918 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1318 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4924 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1320 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4931 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1323 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4938 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1326 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4946 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1330 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4953 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1333 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4960 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1336 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4967 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1339 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4973 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1341 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4981 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1345 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4988 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1348 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4995 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1351 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5002 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1354 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5009 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1357 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5016 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1360 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5022 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1362 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5028 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1364 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5034 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_LAST '}'  */
#line 1366 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5040 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1368 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5046 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1370 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5052 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1372 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5058 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1374 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5064 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1376 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5070 "raku.tab.c"
    break;

  case 305: /* closure: '{' expr '}'  */
#line 1379 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5076 "raku.tab.c"
    break;

  case 306: /* expr: VAR_SCALAR '=' expr  */
#line 1382 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5082 "raku.tab.c"
    break;

  case 307: /* expr: VAR_ARRAY '=' expr  */
#line 1383 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5088 "raku.tab.c"
    break;

  case 308: /* expr: KW_GATHER block  */
#line 1384 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5098 "raku.tab.c"
    break;

  case 309: /* expr: tern_expr  */
#line 1389 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5104 "raku.tab.c"
    break;

  case 310: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1393 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5110 "raku.tab.c"
    break;

  case 311: /* tern_expr: cmp_expr  */
#line 1394 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5116 "raku.tab.c"
    break;

  case 312: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1397 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5122 "raku.tab.c"
    break;

  case 313: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1398 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5128 "raku.tab.c"
    break;

  case 314: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1399 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5134 "raku.tab.c"
    break;

  case 315: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1400 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5140 "raku.tab.c"
    break;

  case 316: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1401 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5146 "raku.tab.c"
    break;

  case 317: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1402 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5152 "raku.tab.c"
    break;

  case 318: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1403 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5158 "raku.tab.c"
    break;

  case 319: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1404 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5164 "raku.tab.c"
    break;

  case 320: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1405 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5170 "raku.tab.c"
    break;

  case 321: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1406 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5176 "raku.tab.c"
    break;

  case 322: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1407 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5182 "raku.tab.c"
    break;

  case 323: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1408 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5188 "raku.tab.c"
    break;

  case 324: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1409 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5194 "raku.tab.c"
    break;

  case 325: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1410 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5200 "raku.tab.c"
    break;

  case 326: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1412 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5210 "raku.tab.c"
    break;

  case 327: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1418 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5220 "raku.tab.c"
    break;

  case 328: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1424 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5230 "raku.tab.c"
    break;

  case 329: /* cmp_expr: divis_expr  */
#line 1429 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5236 "raku.tab.c"
    break;

  case 330: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1432 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5242 "raku.tab.c"
    break;

  case 331: /* divis_expr: jct_expr  */
#line 1433 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5248 "raku.tab.c"
    break;

  case 332: /* jct_expr: jct_expr '|' range_expr  */
#line 1436 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5254 "raku.tab.c"
    break;

  case 333: /* jct_expr: jct_expr '&' range_expr  */
#line 1437 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5260 "raku.tab.c"
    break;

  case 334: /* jct_expr: dor_expr  */
#line 1438 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5266 "raku.tab.c"
    break;

  case 335: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1442 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5272 "raku.tab.c"
    break;

  case 336: /* dor_expr: range_expr  */
#line 1443 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5278 "raku.tab.c"
    break;

  case 337: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1446 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5284 "raku.tab.c"
    break;

  case 338: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1447 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5290 "raku.tab.c"
    break;

  case 339: /* range_expr: add_expr  */
#line 1448 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5296 "raku.tab.c"
    break;

  case 340: /* add_expr: add_expr '~' repl_expr  */
#line 1451 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5302 "raku.tab.c"
    break;

  case 341: /* add_expr: repl_expr  */
#line 1452 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5308 "raku.tab.c"
    break;

  case 342: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1455 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5314 "raku.tab.c"
    break;

  case 343: /* repl_expr: addsub_expr  */
#line 1456 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5320 "raku.tab.c"
    break;

  case 344: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1459 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5326 "raku.tab.c"
    break;

  case 345: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1460 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5332 "raku.tab.c"
    break;

  case 346: /* addsub_expr: mul_expr  */
#line 1461 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5338 "raku.tab.c"
    break;

  case 347: /* mul_expr: mul_expr '*' unary_expr  */
#line 1464 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5344 "raku.tab.c"
    break;

  case 348: /* mul_expr: mul_expr '/' unary_expr  */
#line 1465 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5350 "raku.tab.c"
    break;

  case 349: /* mul_expr: mul_expr '%' unary_expr  */
#line 1466 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5356 "raku.tab.c"
    break;

  case 350: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1467 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5362 "raku.tab.c"
    break;

  case 351: /* mul_expr: unary_expr  */
#line 1468 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5368 "raku.tab.c"
    break;

  case 352: /* unary_expr: '-' unary_expr  */
#line 1471 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5374 "raku.tab.c"
    break;

  case 353: /* unary_expr: '!' unary_expr  */
#line 1472 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5380 "raku.tab.c"
    break;

  case 354: /* unary_expr: CARET unary_expr  */
#line 1473 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5386 "raku.tab.c"
    break;

  case 355: /* unary_expr: OP_REDUCE unary_expr  */
#line 1475 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5395 "raku.tab.c"
    break;

  case 356: /* unary_expr: pow_expr  */
#line 1479 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5401 "raku.tab.c"
    break;

  case 357: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1482 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5407 "raku.tab.c"
    break;

  case 358: /* pow_expr: postfix_expr  */
#line 1483 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5413 "raku.tab.c"
    break;

  case 359: /* scalar_list: VAR_SCALAR  */
#line 1486 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5419 "raku.tab.c"
    break;

  case 360: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1487 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5425 "raku.tab.c"
    break;

  case 361: /* meth_name: IDENT  */
#line 1490 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5431 "raku.tab.c"
    break;

  case 362: /* meth_name: KW_SORT  */
#line 1491 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5437 "raku.tab.c"
    break;

  case 363: /* meth_name: KW_MAP  */
#line 1492 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5443 "raku.tab.c"
    break;

  case 364: /* meth_name: KW_GREP  */
#line 1493 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5449 "raku.tab.c"
    break;

  case 365: /* meth_name: KW_SAY  */
#line 1494 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5455 "raku.tab.c"
    break;

  case 366: /* meth_name: KW_PRINT  */
#line 1495 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5461 "raku.tab.c"
    break;

  case 367: /* meth_name: KW_TAKE  */
#line 1496 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5467 "raku.tab.c"
    break;

  case 368: /* meth_name: KW_RETURN  */
#line 1497 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5473 "raku.tab.c"
    break;

  case 369: /* meth_name: KW_EXISTS  */
#line 1498 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5479 "raku.tab.c"
    break;

  case 370: /* meth_name: KW_DELETE  */
#line 1499 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5485 "raku.tab.c"
    break;

  case 371: /* meth_name: TESTOP  */
#line 1500 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5491 "raku.tab.c"
    break;

  case 372: /* postfix_expr: call_expr  */
#line 1502 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5497 "raku.tab.c"
    break;

  case 373: /* call_expr: IDENT '(' arg_list ')'  */
#line 1505 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5506 "raku.tab.c"
    break;

  case 374: /* call_expr: IDENT '(' ')'  */
#line 1509 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5512 "raku.tab.c"
    break;

  case 375: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1511 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5518 "raku.tab.c"
    break;

  case 376: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1513 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5524 "raku.tab.c"
    break;

  case 377: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1515 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5533 "raku.tab.c"
    break;

  case 378: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1520 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5539 "raku.tab.c"
    break;

  case 379: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1522 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5549 "raku.tab.c"
    break;

  case 380: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1528 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5559 "raku.tab.c"
    break;

  case 381: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1534 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5567 "raku.tab.c"
    break;

  case 382: /* call_expr: IDENT '.' KW_NEW  */
#line 1538 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5575 "raku.tab.c"
    break;

  case 383: /* call_expr: IDENT '.' IDENT  */
#line 1542 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5584 "raku.tab.c"
    break;

  case 384: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1547 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5595 "raku.tab.c"
    break;

  case 385: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1554 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5604 "raku.tab.c"
    break;

  case 386: /* call_expr: IDENT '.' CARET IDENT  */
#line 1559 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5614 "raku.tab.c"
    break;

  case 387: /* call_expr: atom '.' CARET IDENT  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5624 "raku.tab.c"
    break;

  case 388: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5635 "raku.tab.c"
    break;

  case 389: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1578 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5646 "raku.tab.c"
    break;

  case 390: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1585 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5655 "raku.tab.c"
    break;

  case 391: /* call_expr: atom '.' meth_name  */
#line 1590 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5664 "raku.tab.c"
    break;

  case 392: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1595 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5675 "raku.tab.c"
    break;

  case 393: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1602 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5684 "raku.tab.c"
    break;

  case 394: /* call_expr: call_expr '.' meth_name  */
#line 1607 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5693 "raku.tab.c"
    break;

  case 395: /* call_expr: KW_DIE expr  */
#line 1612 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5699 "raku.tab.c"
    break;

  case 396: /* call_expr: KW_MAP closure expr  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5705 "raku.tab.c"
    break;

  case 397: /* call_expr: KW_GREP closure expr  */
#line 1616 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5711 "raku.tab.c"
    break;

  case 398: /* call_expr: KW_SORT expr  */
#line 1618 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5717 "raku.tab.c"
    break;

  case 399: /* call_expr: KW_SORT closure expr  */
#line 1620 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5723 "raku.tab.c"
    break;

  case 400: /* call_expr: atom  */
#line 1621 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5729 "raku.tab.c"
    break;

  case 401: /* arg_list: expr  */
#line 1624 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5735 "raku.tab.c"
    break;

  case 402: /* arg_list: arg_list ',' expr  */
#line 1625 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5741 "raku.tab.c"
    break;

  case 403: /* arg_list: arg_list ','  */
#line 1626 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5747 "raku.tab.c"
    break;

  case 404: /* atom: LIT_INT  */
#line 1629 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5753 "raku.tab.c"
    break;

  case 405: /* atom: LIT_FLOAT  */
#line 1630 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5759 "raku.tab.c"
    break;

  case 406: /* atom: LIT_STR  */
#line 1631 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5765 "raku.tab.c"
    break;

  case 407: /* atom: WORDLIST  */
#line 1633 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5778 "raku.tab.c"
    break;

  case 408: /* atom: LIT_INTERP_STR  */
#line 1641 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5784 "raku.tab.c"
    break;

  case 409: /* atom: VAR_SCALAR  */
#line 1642 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5790 "raku.tab.c"
    break;

  case 410: /* atom: OP_INC VAR_SCALAR  */
#line 1643 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5796 "raku.tab.c"
    break;

  case 411: /* atom: OP_DEC VAR_SCALAR  */
#line 1644 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5802 "raku.tab.c"
    break;

  case 412: /* atom: VAR_SCALAR OP_INC  */
#line 1645 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5808 "raku.tab.c"
    break;

  case 413: /* atom: VAR_SCALAR OP_DEC  */
#line 1646 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5814 "raku.tab.c"
    break;

  case 414: /* atom: VAR_ARRAY  */
#line 1647 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5820 "raku.tab.c"
    break;

  case 415: /* atom: VAR_HASH  */
#line 1648 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5826 "raku.tab.c"
    break;

  case 416: /* atom: VAR_CAPTURE  */
#line 1650 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5834 "raku.tab.c"
    break;

  case 417: /* atom: VAR_FH  */
#line 1654 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5842 "raku.tab.c"
    break;

  case 418: /* atom: VAR_NAMED_CAPTURE  */
#line 1658 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5849 "raku.tab.c"
    break;

  case 419: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1661 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5855 "raku.tab.c"
    break;

  case 420: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1663 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5861 "raku.tab.c"
    break;

  case 421: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1665 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5867 "raku.tab.c"
    break;

  case 422: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1667 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5873 "raku.tab.c"
    break;

  case 423: /* atom: ARR_ALL_SLICE  */
#line 1669 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5879 "raku.tab.c"
    break;

  case 424: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1671 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5885 "raku.tab.c"
    break;

  case 425: /* atom: VAR_HASH '{' expr '}'  */
#line 1673 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5891 "raku.tab.c"
    break;

  case 426: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1675 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5897 "raku.tab.c"
    break;

  case 427: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1677 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5903 "raku.tab.c"
    break;

  case 428: /* atom: IDENT  */
#line 1678 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5909 "raku.tab.c"
    break;

  case 429: /* atom: VAR_TWIGIL  */
#line 1680 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5917 "raku.tab.c"
    break;

  case 430: /* atom: VAR_ARRAY_TWIGIL  */
#line 1684 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5925 "raku.tab.c"
    break;

  case 431: /* atom: VAR_HASH_TWIGIL  */
#line 1688 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5933 "raku.tab.c"
    break;

  case 432: /* atom: '(' ')'  */
#line 1691 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5939 "raku.tab.c"
    break;

  case 433: /* atom: '(' expr ')'  */
#line 1692 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5945 "raku.tab.c"
    break;

  case 434: /* atom: '(' expr ',' ')'  */
#line 1694 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5951 "raku.tab.c"
    break;

  case 435: /* atom: '(' expr ',' arg_list ')'  */
#line 1696 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5958 "raku.tab.c"
    break;

  case 436: /* atom: block  */
#line 1698 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5964 "raku.tab.c"
    break;

  case 437: /* atom: KW_SUB block  */
#line 1699 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5970 "raku.tab.c"
    break;


#line 5974 "raku.tab.c"

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

#line 1701 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
