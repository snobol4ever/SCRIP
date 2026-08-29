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
  YYSYMBOL_OP_BAND = 107,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 108,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 109,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 110,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 111,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 112,                   /* OP_POW  */
  YYSYMBOL_113_ = 113,                     /* '='  */
  YYSYMBOL_114_ = 114,                     /* '!'  */
  YYSYMBOL_115_ = 115,                     /* '<'  */
  YYSYMBOL_116_ = 116,                     /* '>'  */
  YYSYMBOL_117_ = 117,                     /* '|'  */
  YYSYMBOL_118_ = 118,                     /* '&'  */
  YYSYMBOL_119_ = 119,                     /* '~'  */
  YYSYMBOL_120_ = 120,                     /* '+'  */
  YYSYMBOL_121_ = 121,                     /* '-'  */
  YYSYMBOL_122_ = 122,                     /* '*'  */
  YYSYMBOL_123_ = 123,                     /* '/'  */
  YYSYMBOL_124_ = 124,                     /* '%'  */
  YYSYMBOL_UMINUS = 125,                   /* UMINUS  */
  YYSYMBOL_126_ = 126,                     /* '.'  */
  YYSYMBOL_127_ = 127,                     /* ';'  */
  YYSYMBOL_128_ = 128,                     /* '('  */
  YYSYMBOL_129_ = 129,                     /* ')'  */
  YYSYMBOL_130_ = 130,                     /* ','  */
  YYSYMBOL_131_ = 131,                     /* '['  */
  YYSYMBOL_132_ = 132,                     /* ']'  */
  YYSYMBOL_133_ = 133,                     /* '{'  */
  YYSYMBOL_134_ = 134,                     /* '}'  */
  YYSYMBOL_135_ = 135,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 136,                 /* $accept  */
  YYSYMBOL_program = 137,                  /* program  */
  YYSYMBOL_stmt_list = 138,                /* stmt_list  */
  YYSYMBOL_stmt = 139,                     /* stmt  */
  YYSYMBOL_if_stmt = 140,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 141,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 142,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 143,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 144,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 145,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 146,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 147,                /* loop_incr  */
  YYSYMBOL_for_stmt = 148,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 149,               /* given_stmt  */
  YYSYMBOL_when_list = 150,                /* when_list  */
  YYSYMBOL_sub_decl = 151,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 152,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 153,                 /* sub_body  */
  YYSYMBOL_method_body = 154,              /* method_body  */
  YYSYMBOL_class_decl = 155,               /* class_decl  */
  YYSYMBOL_role_decl = 156,                /* role_decl  */
  YYSYMBOL_is_clauses = 157,               /* is_clauses  */
  YYSYMBOL_class_body_list = 158,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 159,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 160,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 161,           /* named_arg_list  */
  YYSYMBOL_pair_list = 162,                /* pair_list  */
  YYSYMBOL_param_list = 163,               /* param_list  */
  YYSYMBOL_block = 164,                    /* block  */
  YYSYMBOL_closure = 165,                  /* closure  */
  YYSYMBOL_expr = 166,                     /* expr  */
  YYSYMBOL_tern_expr = 167,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 168,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 169,               /* divis_expr  */
  YYSYMBOL_jct_expr = 170,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 171,                 /* dor_expr  */
  YYSYMBOL_range_expr = 172,               /* range_expr  */
  YYSYMBOL_add_expr = 173,                 /* add_expr  */
  YYSYMBOL_repl_expr = 174,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 175,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 176,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 177,               /* unary_expr  */
  YYSYMBOL_pow_expr = 178,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 179,              /* scalar_list  */
  YYSYMBOL_meth_name = 180,                /* meth_name  */
  YYSYMBOL_postfix_expr = 181,             /* postfix_expr  */
  YYSYMBOL_call_expr = 182,                /* call_expr  */
  YYSYMBOL_arg_list = 183,                 /* arg_list  */
  YYSYMBOL_atom = 184                      /* atom  */
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
#define YYLAST   4104

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  136
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  450
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   368


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
       2,     2,     2,   114,     2,     2,     2,   124,   118,     2,
     128,   129,   122,   120,   130,   121,   126,   123,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   135,   127,
     115,   113,   116,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   131,     2,   132,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   133,   117,   134,   119,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   125
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   395,   395,   406,   407,   410,   412,   414,   416,   420,
     422,   424,   426,   428,   432,   435,   439,   441,   445,   448,
     452,   454,   456,   458,   460,   462,   466,   470,   472,   474,
     476,   478,   480,   482,   484,   486,   488,   490,   492,   494,
     496,   498,   500,   503,   506,   508,   511,   514,   516,   518,
     520,   523,   526,   528,   533,   537,   541,   546,   550,   551,
     553,   555,   557,   559,   562,   564,   566,   568,   571,   574,
     577,   580,   583,   585,   587,   589,   591,   594,   596,   598,
     600,   603,   606,   610,   613,   615,   617,   619,   622,   625,
     628,   631,   634,   637,   640,   641,   642,   643,   644,   645,
     646,   648,   650,   652,   654,   655,   656,   657,   658,   659,
     660,   662,   664,   666,   668,   669,   670,   671,   674,   676,
     678,   680,   682,   684,   686,   688,   692,   694,   696,   698,
     700,   702,   706,   708,   712,   714,   716,   718,   722,   724,
     728,   730,   732,   736,   738,   740,   744,   747,   753,   759,
     764,   770,   775,   780,   788,   799,   800,   806,   814,   820,
     826,   834,   840,   846,   855,   862,   871,   880,   889,   896,
     903,   904,   907,   910,   912,   914,   916,   918,   920,   922,
     924,   927,   929,   931,   933,   936,   939,   945,   946,   947,
     950,   953,   955,   957,   959,   961,   963,   965,   967,   970,
     972,   974,   976,   979,   982,   988,  1004,  1019,  1020,  1032,
    1046,  1047,  1050,  1053,  1056,  1059,  1062,  1065,  1068,  1071,
    1074,  1077,  1080,  1083,  1090,  1097,  1104,  1111,  1115,  1119,
    1123,  1127,  1134,  1141,  1148,  1155,  1165,  1173,  1181,  1190,
    1197,  1204,  1214,  1222,  1232,  1247,  1248,  1253,  1258,  1265,
    1269,  1273,  1277,  1283,  1285,  1287,  1289,  1293,  1294,  1295,
    1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,  1304,  1305,
    1306,  1307,  1308,  1309,  1310,  1311,  1312,  1315,  1316,  1317,
    1319,  1321,  1323,  1325,  1327,  1330,  1332,  1334,  1336,  1338,
    1341,  1344,  1348,  1351,  1354,  1357,  1359,  1363,  1366,  1369,
    1372,  1375,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1392,
    1394,  1398,  1401,  1402,  1403,  1404,  1405,  1406,  1407,  1408,
    1413,  1416,  1418,  1421,  1422,  1423,  1424,  1425,  1426,  1427,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1441,  1447,
    1453,  1456,  1457,  1460,  1461,  1462,  1465,  1467,  1470,  1471,
    1472,  1475,  1476,  1479,  1480,  1483,  1484,  1485,  1488,  1489,
    1490,  1491,  1492,  1494,  1496,  1499,  1500,  1501,  1502,  1507,
    1510,  1511,  1514,  1515,  1518,  1519,  1520,  1521,  1522,  1523,
    1524,  1525,  1526,  1527,  1528,  1530,  1532,  1537,  1538,  1540,
    1542,  1547,  1549,  1555,  1561,  1565,  1569,  1574,  1581,  1586,
    1592,  1598,  1605,  1612,  1617,  1622,  1629,  1634,  1639,  1641,
    1643,  1645,  1647,  1649,  1652,  1653,  1654,  1657,  1658,  1659,
    1660,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
    1681,  1685,  1688,  1690,  1692,  1694,  1696,  1698,  1700,  1702,
    1704,  1706,  1707,  1711,  1715,  1719,  1720,  1721,  1723,  1726,
    1727
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
  "OP_DOR", "OP_DIV", "OP_BAND", "OP_SHL", "OP_DIVIS", "OP_REP_X",
  "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'", "'&'", "'~'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'", "'('", "')'",
  "','", "'['", "']'", "'{'", "'}'", "':'", "$accept", "program",
  "stmt_list", "stmt", "if_stmt", "elsif_tail", "while_stmt",
  "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt", "loop_incr",
  "for_stmt", "given_stmt", "when_list", "sub_decl", "scalar_methcall",
  "sub_body", "method_body", "class_decl", "role_decl", "is_clauses",
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

#define YYPACT_NINF (-724)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-450)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -724,    19,  1852,  -724,  -724,  -724,  -724,  -724,   880,    52,
     -70,   -64,    21,  -724,  -724,  3925,  -724,  -724,  -724,   121,
    1927,   216,  3028,  3103,  3165,  3240,  3302,     4,    56,  3302,
    1989,    33,  3302,   139,   187,  3377,  3439,    56,    29,    16,
      46,    82,    82,  3514,    56,    56,  3302,   236,   311,    59,
    3925,  -724,  -724,   331,   343,   374,  3925,  3925,  -724,  2064,
     373,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,   632,  -724,  -724,  -724,  3978,   840,  -724,   712,   468,
      -5,   362,  -724,     6,   361,   173,   400,  -724,  -724,   365,
     391,   422,   572,  -724,  -724,  3302,  3302,  3302,  3302,  3302,
    3302,   581,  2126,  3576,  2202,   629,  3302,  3302,   423,   206,
     412,   141,   430,   122,  -724,   103,    56,  2264,  -724,  -724,
     491,   740,   168,  -724,  2340,  -724,   404,   129,   249,   284,
     739,   639,    69,   645,  2264,   595,  2264,   271,  2264,    56,
    2264,    56,   327,   146,   276,  -724,  -724,   546,  -724,    60,
     577,   615,   619,   183,   219,  2264,    56,  2264,    56,    77,
    2402,  -724,  3302,  3302,  -724,  3302,  3302,  -724,  3302,  3302,
    3302,  2477,  3302,  -724,   631,  -724,  -724,  -724,   651,   674,
      37,  -724,   675,  -724,  -724,  -724,  -724,   933,  -724,   291,
     474,   633,  1262,  3302,  3302,  3302,  3302,  3302,  3302,  3302,
    3302,  -724,  3302,  3302,  3302,  3302,  3302,  3302,  3302,  3302,
    -724,  3925,  3925,  3925,  3925,  3925,  3925,  3925,  3925,  3925,
    3925,  3925,  3925,  3925,  3925,  3925,   878,  3925,  3925,  3925,
    3925,  3925,  3925,  3925,  3925,  3925,  3925,  3925,  3925,  3925,
    3925,  3925,  3925,  3925,   919,   896,   609,   690,   693,   695,
     699,   719,   725,    13,  -724,   657,  2264,   209,   677,   185,
     738,   733,   744,  -724,   745,   876,   754,   -68,  -724,   889,
     770,   776,  2202,   895,  3302,  -724,  3302,  3302,  3302,  3302,
    3302,  3302,  3302,   765,   291,   787,  -724,  3302,  3302,  -724,
    3651,  -724,  3713,  -724,   357,   389,   392,   385,   799,   801,
    -724,   802,   805,  3302,  3302,  3302,  3302,  3302,  3302,  3302,
    -724,  3302,   828,  3302,  3302,  -724,  3302,   842,   958,   860,
    -724,   912,  3302,  -724,  3925,  3925,     2,  -724,  -724,  -724,
    3302,  3302,  -724,  3302,  3302,  -724,   922,  3302,   928,  3302,
     867,   923,   871,  -724,  3302,  3302,   929,   827,   839,   848,
     850,   864,   853,  -724,  -724,  -724,    56,    17,  -724,    15,
     866,   877,  -724,  -724,  2552,   894,  1011,  -724,  1073,   247,
     273,   937,  3028,  3103,  3302,  1589,    45,   240,  -724,   351,
     942,   979,  1037,  1040,  1041,  1055,  1057,  1058,  1060,  1062,
    1063,  1064,  1070,  1076,  1077,  1078,   943,   943,   943,   943,
     943,   943,  1100,   943,   943,   943,   943,   943,   943,   943,
     943,  -724,  -724,  -724,    -5,  -724,  -724,  -724,   957,   957,
     361,   173,   400,   400,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,   974,  1192,  1079,  -724,  2614,  -724,  -724,  -724,
    -724,  -724,  -724,  3302,  2690,  -724,   874,  3302,  -724,  3302,
    3302,  3302,  3302,  1095,  1096,  1098,  -724,  2752,  -724,   760,
    3302,  1082,  -724,    -7,  -724,  1129,   191,  1099,  1080,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  1085,  -724,  1086,
    2264,   371,  1139,   -62,  2828,   419,  1091,  3302,  -724,  3788,
    -724,  3302,  -724,    42,  -724,  3302,  3302,  1106,  2552,  1093,
    1102,  1103,  1104,  1105,  1107,  1108,   433,  2552,  1109,  1110,
     453,    56,    -9,  3850,  -724,    56,    56,   339,   -32,   102,
    1111,  -724,    40,  -724,  -724,  -724,  1089,   883,  1388,  1112,
    1113,  1114,  1115,    54,  1117,  1092,  1122,  1118,    56,    56,
      56,  1119,  1120,  1131,  3302,  -724,  -724,  -724,  -724,  -724,
    -724,  1209,  1211,  -724,   -20,  1089,   885,    51,   347,   137,
    -724,   887,  3302,  -724,  1231,  2202,  3302,  3302,   353,    12,
     155,  -724,   241,  3302,  3302,  -724,  3302,  3302,  -724,  3302,
    3302,  3302,  3302,  3302,  3302,  3302,  3302,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  3925,  2890,  -724,  1067,  1121,   890,
    1124,  -724,   897,  2552,  1126,   490,  1127,  1128,   278,  3302,
    3302,  3302,  -724,   899,  -724,   909,   911,  -724,  3302,  1172,
    1240,   913,  -724,  -724,  -724,  -724,  -724,   915,  3302,  -724,
    3302,  3302,  3302,   918,  -724,   460,  -724,  1134,  2264,   492,
    1136,  1089,   925,  1142,  1143,  3302,   930,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,   936,  -724,  -724,  -724,   982,
    -724,  -724,  2264,    56,  -724,  -724,  1246,  -724,  1248,  1261,
    3302,  1176,  1280,  1289,  -724,  1089,   424,  3028,  3103,  1651,
    -724,   499,  -724,  -724,  -724,  -724,  3302,    56,  -724,  -724,
    -724,  1173,  1174,  1275,  -724,  -724,  -724,  -724,  3302,  1193,
    -724,  -724,    18,   669,   949,  1263,  -724,  -724,  1089,  1089,
     938,   945,  1309,  1310,  1313,  -724,  -724,  1201,   387,   364,
    1195,   192,  3302,  3302,  3302,  3302,  3302,  3302,  -724,  -724,
    -724,  -724,   306,   314,   403,   417,   432,   442,   444,   445,
     471,   475,   479,   481,  -724,  -724,   959,  -724,   971,   975,
    -724,  1203,  -724,  -724,   983,  -724,  -724,  -724,  -724,  -724,
    1205,  1206,  1207,  -724,  -724,  -724,  1213,  3302,  1223,  -724,
    2552,  1225,   515,  -724,  -724,  1226,  1277,  1278,  -724,   985,
    -724,  3302,  -724,  -724,  1089,  -724,  -724,   524,  1229,  1230,
      -9,  -724,   990,  1097,    56,    56,    56,  -724,  3302,  -724,
    -724,  -724,  1245,  -724,   138,  -724,  -724,  -724,   571,   312,
    -724,   320,  3302,  3302,  3302,  3302,  3302,  3302,  3302,  3302,
    -724,    56,  1228,  -724,  -724,    56,  1232,  3302,  -724,   399,
     536,    32,   200,   862,  1236,  1237,   919,  -724,  -724,  1089,
    1089,  1358,  1359,  1360,  -724,  3302,  2966,  1255,  1256,  -724,
     489,   503,   505,   513,   514,   532,  -724,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724,  -724,  1243,  -724,  -724,  -724,  -724,  -724,  3302,   997,
    -724,  -724,  -724,  3302,  3302,  2552,   582,  -724,  -724,  3302,
    -724,  -724,  -724,  -724,    56,    56,  -724,  -724,  -724,  -724,
    -724,  3302,  1258,  1362,  1363,  3302,  3302,  3302,  -724,  -724,
    -724,   543,   547,   551,   552,   555,   565,   573,   587,  -724,
    -724,  -724,  3302,  1249,  -724,   109,  1302,  -724,   119,  -724,
    1361,  1365,  1250,  3302,  -724,  1366,  1367,  1257,  3302,  -724,
     227,   337,  1259,  1260,  -724,  -724,   580,  -724,  -724,  -724,
    -724,  -724,   590,  1251,   999,  3302,  3302,  -724,  -724,  -724,
    -724,  -724,  -724,  -724,  1253,  1270,  -724,  -724,  1008,  -724,
     596,  1123,  -724,  -724,  3302,  -724,  -724,   604,   605,   620,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  1288,    56,
    1283,  1023,  1254,  1514,  1283,  1025,  1298,  1299,  -724,  1300,
    1301,  1317,  -724,  1318,  1432,  1435,  1323,  3302,  -724,  1439,
    1440,  1328,  3302,  -724,  -724,  -724,   167,  -724,  -724,  -724,
    1322,   621,   626,  -724,  -724,  1331,  -724,    56,  -724,  -724,
    -724,  -724,  -724,  3302,  -724,  -724,  1283,  -724,  3028,  3103,
    1726,  -724,   821,  -724,  1283,  -724,  -724,  -724,  -724,  -724,
    -724,  1332,  1333,  -724,  1335,  1336,  1337,  -724,  1338,  1283,
    1027,  -724,  -724,  -724,  -724,  -724,  1339,  -724,   868,   322,
    -724,   359,  3302,  3302,  3302,  3302,  3302,  3302,  3302,  3302,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  1283,
      56,  3302,  3302,  3302,  -724,  -724,  -724,   634,   635,   654,
     655,   656,   672,   673,   685,  -724,  -724,   691,   722,   728,
    -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,  -724,
    -724
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   417,   418,   419,   421,   422,   427,
     428,   442,   441,   443,   444,     0,   429,   430,   431,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   420,     0,     0,     0,     0,     0,    95,     0,
       3,     4,    96,    97,   104,   105,   106,   107,    98,    99,
     114,     0,   115,   116,   117,   103,     0,   320,   322,   340,
     342,   345,   347,   350,   352,   354,   357,   364,   369,   371,
     385,   413,     0,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   422,   427,   428,   442,   441,     0,     0,   449,   367,
       0,   422,   427,    36,     0,   414,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,     0,   450,   319,     0,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   140,
       0,   143,     0,     0,   108,     0,     0,   109,     0,     0,
       0,     3,     0,   411,   100,   102,   408,   207,     0,     0,
       0,   368,     0,   423,   424,   366,   365,   422,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   314,   315,   316,
     317,   318,   312,     0,   391,     0,     0,   313,     0,     0,
       0,     0,     0,    40,   396,     0,   395,   441,   387,     0,
       0,     0,     0,     0,     0,    31,     0,     0,     0,     0,
       0,     0,     0,   445,     0,     0,    39,   416,     0,     6,
       0,    10,     0,    11,     0,     0,     0,     0,     0,     0,
     372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,    44,     0,     0,   121,     0,
     133,     0,     0,   152,     0,     0,     0,     3,   159,    47,
       0,     0,    48,     0,     0,   155,     0,     0,     0,     0,
       0,   136,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   409,   410,   412,     0,     0,   210,     0,
       0,     0,   245,   446,     0,     0,     0,   278,   422,   427,
     428,   442,     0,     0,     0,     0,     0,     0,   277,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   325,   326,   329,   330,
     323,   324,     0,   327,   328,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   341,   343,   344,   346,   348,   349,
     351,   353,   355,   356,   361,   362,   363,   358,   359,   360,
     370,   374,   384,   378,   379,   380,   381,   382,   383,   376,
     377,   375,   407,     0,   404,    55,     0,    89,    90,    91,
      92,    93,    52,     0,     0,   390,     0,     0,    16,     0,
       0,     0,     0,   432,   437,   438,    57,     0,   399,     0,
       0,     0,   388,     0,   386,   416,     0,     0,     0,   314,
     315,   316,   317,   318,   312,   313,    38,     0,   415,     0,
       0,     0,   419,   441,     0,     0,     0,     0,    28,     0,
      29,     0,    30,     0,   162,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   446,     0,     0,   124,   446,     0,     0,   348,   349,
     257,   258,     0,   271,   273,   275,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   446,     0,
     446,     0,     0,     0,     0,   110,   111,   112,   113,   311,
     101,     0,     0,   210,     0,     0,     0,     0,     0,     0,
     447,     0,     0,   373,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,     0,   305,     0,     0,   306,     0,
       0,     0,     0,     0,     0,     0,     0,   279,    59,    61,
      63,    66,    64,    65,    60,    62,    72,    74,    76,    79,
      77,    78,    73,    75,     0,     0,   400,     0,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,   394,     0,     0,   249,     0,     0,
       0,     0,   432,   437,   438,    37,     5,     0,     0,    12,
       0,     0,     0,     0,    21,     0,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    80,    83,    82,
      86,    84,    85,    81,    42,     0,    87,    88,    45,   118,
     123,   122,     0,     0,   132,   149,     0,   151,     0,     0,
       0,   260,     0,     0,   158,     0,     0,     0,     0,     0,
     170,     0,    50,    51,    33,    32,     0,     0,   153,   439,
     440,     0,     0,   134,   137,   138,   141,   142,     0,     0,
     208,   209,     0,     0,     0,     0,   206,   167,     0,     0,
       0,     0,     0,     0,     0,   244,   448,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   288,   295,
     302,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,   406,     0,   403,     0,     0,
      54,     0,    56,   168,     0,    18,    17,   435,   434,   433,
       0,     0,     0,   397,   392,   393,     0,     0,     0,   389,
       0,     0,     0,   254,   253,     0,     0,     0,    23,     0,
      24,     0,    27,   161,     0,    35,    34,     0,   448,   448,
       0,   125,     0,   126,     0,     0,     0,   267,     0,   261,
     262,   157,   263,   259,     0,   272,   274,   276,     0,     0,
     173,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,    70,    71,     0,     0,     0,   205,     0,
       0,     0,     0,     0,     0,     0,     0,   166,   164,     0,
       0,     0,     0,     0,     9,     0,     0,   432,   438,   299,
       0,     0,     0,     0,     0,     0,   307,   308,   309,   310,
     280,   282,   284,   287,   285,   286,   281,   283,   405,   402,
     401,    53,   448,    67,    68,    69,   250,   251,     0,     0,
      14,    13,    22,     0,     0,     0,     0,   160,     7,     0,
      43,    46,   120,   119,   446,     0,   130,   150,   147,   148,
     269,     0,   264,     0,     0,     0,     0,     0,   174,   175,
     172,     0,     0,     0,     0,     0,     0,     0,     0,   156,
     154,   135,     0,     0,   146,     0,     3,   240,     0,   237,
       0,     0,     0,     0,   216,     0,     0,     0,     0,   211,
       0,     0,     0,     0,   212,   213,     0,   163,   165,   246,
     247,   248,     0,   169,     0,     0,     0,   289,   291,   294,
     292,   293,   290,    19,     0,   448,   256,   255,     0,    25,
       0,   127,   128,   268,     0,   265,   266,     0,     0,     0,
     176,   178,   180,   183,   181,   182,   177,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   227,     0,
       0,     0,   228,     0,     0,     0,     0,     0,   218,     0,
       0,     0,     0,   217,   214,   215,     0,   243,   298,   297,
     168,     0,     0,   252,    15,   448,     8,     0,   131,   270,
     184,   186,   185,     0,   145,   239,     0,   188,     0,     0,
       0,   187,     0,   236,     0,   225,   226,   220,   223,   224,
     219,     0,     0,   229,     0,     0,     0,   230,     0,     0,
       0,   296,   300,   301,    26,   129,     0,   238,     0,     0,
     191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     189,   235,   233,   234,   222,   231,   232,   221,   242,     0,
       0,     0,     0,     0,   192,   193,   190,     0,     0,     0,
       0,     0,     0,     0,     0,   241,   144,     0,     0,     0,
     194,   196,   198,   201,   199,   200,   195,   197,   202,   204,
     203
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -724,  -724,     3,  -724,  -518,  -673,  -724,  -724,  -724,  -724,
    -724,   413,  -724,  -724,  -724,  -724,  -724,  -292,  -723,  -724,
    -724,  -724,   904,  -724,  -724,  -461,   976,  -349,   208,  1116,
      -2,  -215,  -724,  1125,  1242,  -724,   698,   -15,  1238,  1239,
     927,    20,  -724,  1341,  -244,  -724,  -724,   198,  -724
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   192,    61,    62,   524,    63,    64,    65,    66,
      67,   943,    68,    69,   543,    70,    71,   328,   947,    72,
      73,   357,   564,    74,   569,   270,   495,   537,   118,   169,
     125,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   190,   442,    89,    90,   126,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,   444,   402,     2,   680,   504,   811,   639,   635,   470,
     566,   143,   530,   531,   641,   652,   532,    24,   144,     3,
     135,   137,   139,   141,   142,   530,   531,   147,   149,   532,
     152,   561,   108,   156,   158,   119,   723,   724,   313,   562,
     725,   173,   162,   150,   176,   105,   950,   151,   688,   107,
     691,   360,   530,   531,   951,   314,   532,   189,   109,   163,
     110,   530,   531,   106,   109,   532,   110,   533,   534,   535,
     181,   583,   165,   179,   723,   724,   185,   186,   725,   298,
     533,   534,   535,   299,   231,   232,   330,   233,   584,   166,
     180,   706,   707,   247,   248,   249,   250,   251,   252,   361,
     952,   257,   259,   331,   261,   262,   344,   533,   534,   535,
     692,   693,   228,   229,   726,   189,   533,   534,   535,   530,
     531,   345,   284,   532,    60,   233,   453,   949,   640,   530,
     531,   536,   302,   532,   312,   120,   317,    60,   319,   315,
     916,   454,   316,   164,   565,   953,   749,   109,   922,   110,
     563,   153,   848,   340,   662,   342,   768,   160,   347,   954,
     348,   349,    60,   350,   351,   103,   352,   353,   354,   352,
     355,   661,   164,   167,   533,   534,   535,   530,   531,   585,
     729,   532,   689,   104,   533,   534,   535,   332,   708,    60,
     379,   380,   381,   382,   383,   384,   385,   386,   387,   154,
     388,   389,   390,   391,   392,   393,   394,   395,   923,   924,
      75,   732,   733,   734,   955,   168,   418,   419,   730,   731,
     264,   233,   956,   265,   324,   325,   127,   128,   129,   109,
     130,   110,   533,   534,   535,   145,   146,   273,  1010,    93,
      94,  1024,   288,  1037,   694,   159,   161,   131,  1014,  1025,
     177,   132,   174,   175,   456,   274,   289,   424,   425,   426,
     427,   428,   429,   430,   266,   233,   586,   330,   957,   102,
     476,   735,   478,   727,   479,   480,   481,   482,   483,   484,
     485,   282,   329,   587,   331,   488,   489,  1055,   491,   750,
     496,  1063,   912,   235,   236,  1026,  1079,   313,   336,   272,
     255,   509,   510,   511,   512,   513,   514,   515,   271,   528,
     529,   518,   519,   958,   314,   462,   337,   463,  1048,   466,
     457,   462,   285,   642,   145,   178,   869,   959,   539,   540,
     538,   541,   542,  1087,   338,   545,   458,   547,   313,   459,
    1027,  1101,   551,   552,   133,   182,   330,   318,   313,   320,
     323,  1029,   339,   183,  1028,   314,  1108,   530,   531,  1030,
     103,   532,   290,   331,   341,   314,   343,   167,   332,   803,
     578,   579,   580,   582,   588,   751,   291,   589,   575,   742,
     590,   591,   304,   743,   184,   330,  1125,   592,   105,   744,
     593,   594,   745,   746,   595,   596,   747,   292,   315,   764,
      75,   316,   331,   821,   326,  1031,   576,   321,   287,   327,
     779,   293,   533,   534,   535,     4,     5,     6,     7,   686,
     363,   364,   121,   122,   113,   114,   267,    13,    14,    15,
      16,    17,    18,   555,   822,   823,   857,   858,   824,   315,
     876,   556,   316,   116,    28,   191,   929,   332,   877,   315,
    1032,   620,   316,    33,   930,   624,  1115,   322,   626,   627,
      60,    41,    42,    43,  1033,   796,    46,   230,   637,   287,
     497,   234,    60,   488,   797,    50,    51,   243,   210,   850,
     310,    52,   648,   311,   498,   597,   332,   748,   647,   825,
     826,   827,   189,  1116,   462,   657,   867,   659,   649,   660,
     865,   650,   499,   663,   664,   501,   237,   238,   239,   516,
      54,    55,   907,   503,   520,   866,   500,   244,   327,   502,
     527,   683,   240,   241,   242,   832,    56,   945,   833,   834,
     557,   286,   946,    57,   287,   835,   701,   878,   836,   837,
     117,   268,   838,   839,   558,    60,   654,   269,   245,   655,
     263,   879,   719,   220,   221,   222,   223,   224,   225,   606,
     674,   272,   571,   287,   560,   226,   880,   967,   968,   607,
     737,   608,   609,   739,   740,   741,   881,   227,   882,   883,
     678,   752,   753,   287,   754,   755,   246,   756,   757,   758,
     759,   760,   761,   762,   763,   253,  1011,   925,   610,  1015,
     304,   926,   611,   365,   366,   884,   612,   306,   613,   885,
     307,   308,   966,   886,   927,   887,   667,   776,   275,   800,
     287,   303,   801,   977,   304,   305,   210,   780,   781,   782,
     669,   306,   670,   840,   307,   308,   786,   978,   309,   979,
     671,   672,   901,   260,   619,   287,   791,   980,   981,   793,
     794,   908,   622,   297,   909,   300,   799,   625,   193,   673,
     628,   194,   195,   807,   948,   633,   982,   636,   196,   946,
     606,   197,   198,   329,   607,   199,   200,  1000,   608,   609,
     812,  1001,   610,   431,   356,  1002,  1003,  1080,   817,  1004,
     333,   432,   611,   433,   434,   828,   829,   831,   310,  1005,
     612,   311,   435,   436,   841,   928,   666,  1006,  1036,   989,
     437,   438,   287,   946,   613,   675,   846,   772,   439,   440,
     441,  1007,   310,  1046,  1038,   311,   287,   849,   334,   679,
     681,   667,   669,   684,   685,   687,   445,   446,  1050,  1051,
     870,   871,   872,   873,   874,   875,    75,   673,   893,   294,
     295,   296,   335,   895,  1052,  1082,   713,   714,   715,   201,
    1083,   606,   607,     4,     5,     6,     7,   367,  1130,  1131,
     121,   122,   113,   114,   267,    13,    14,    15,    16,    17,
      18,   608,   609,   610,   358,   897,   455,   287,  1132,  1133,
    1134,   116,    28,   211,   212,   213,   214,   460,   461,   611,
     612,    33,   359,   215,   216,   217,  1135,  1136,   362,    41,
      42,    43,   613,   766,    46,   769,   920,   447,   667,  1137,
     448,   774,   449,    50,    51,  1138,   450,   218,   219,    52,
     931,   932,   933,   934,   935,   936,   937,   938,    93,    94,
     276,   277,   278,   279,   280,   944,   451,  1092,   792,   669,
    1093,  1094,   452,   281,   464,   673,  1139,  1095,    54,    55,
    1096,  1097,  1140,   972,  1098,  1099,   202,   465,   102,   203,
     204,   466,   960,   467,    56,   961,   205,   962,   963,   206,
     207,    57,   469,   208,   209,   411,   412,   413,   117,   634,
     468,   813,   486,    60,  1111,   269,   984,   304,  1112,   472,
     473,   986,   987,   471,   306,   474,   475,   307,   308,   477,
     431,  1113,   505,   443,   506,   842,   487,   287,   432,   993,
     433,   434,   526,   997,   998,   999,   415,   416,   417,   435,
     436,   507,   366,   431,   363,   508,   544,   437,   438,   553,
    1008,   432,   546,   433,   434,   439,   440,   441,   210,  1013,
     549,  1019,   435,   436,   554,  1100,  1023,   363,   517,   851,
     437,   438,   852,   853,   854,   855,   555,   210,   439,   440,
     441,   521,   364,  1041,  1042,   556,    92,   557,    93,    94,
      95,    96,    97,    98,    99,   522,   523,   559,   899,   525,
     364,   558,  1049,   100,   567,   310,   548,   364,   311,   906,
     550,   364,  1114,   363,   623,   568,   101,   572,   102,   810,
     523,  1062,   695,   696,   728,   696,   736,   287,   913,   771,
     287,   573,   917,   918,   919,  1074,   773,   287,   783,   287,
    1078,    93,    94,   276,   277,   278,   279,   280,   784,   473,
     785,   287,   789,   473,   363,   790,   281,   795,   655,   939,
     577,   944,   227,   941,   804,   696,  1088,  1089,  1091,   808,
     287,   102,  -372,  -372,   974,   809,   287,   859,   696,   598,
       4,     5,     6,     7,   860,   696,   233,   121,   122,   113,
     114,   267,    13,    14,    15,    16,    17,    18,   888,   287,
    1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,   116,    28,
     889,   473,   615,   988,   890,   287,   599,   990,    33,  1127,
    1128,  1129,   892,   287,   363,   905,    41,    42,    43,   914,
     364,    46,   991,   992,   915,   523,   985,   287,  1040,   287,
      50,    51,     4,     5,     6,     7,    52,  1045,   287,   121,
     122,   113,   114,   267,    13,    14,    15,    16,    17,    18,
    1047,   523,  1056,   696,  1064,   696,  1109,   696,   170,   172,
     116,    28,   422,   423,   600,    54,    55,   601,   602,    92,
      33,    93,    94,    95,    96,    97,    98,    99,    41,    42,
      43,    56,   603,    46,   604,   605,   100,   606,    57,   607,
     608,   609,    50,    51,   614,   117,   767,   610,    52,   574,
      60,   102,   269,   611,   612,   613,   616,   617,   629,   630,
     638,   631,   645,   646,   644,   643,   651,  1054,   656,   665,
     667,    75,   327,   720,   690,   721,   710,    54,    55,   668,
     669,   670,   671,   709,   672,   673,   676,   677,   711,   702,
     703,   704,   705,    56,   718,   738,   716,   717,   770,   787,
      57,   772,   712,   775,   788,  1085,   814,   117,   815,   777,
     778,   798,    60,   802,   269,     4,     5,     6,     7,   805,
     806,   816,   368,   369,   370,   371,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   372,   373,    24,   818,
     819,    25,    26,    27,    28,   374,   375,    31,    32,   820,
     843,   844,   845,    33,    34,    35,    36,    37,    38,   376,
     377,    41,    42,    43,    44,    45,    46,    47,  1126,   856,
     847,    48,    49,   861,   862,    50,    51,   863,   864,   868,
     891,    52,   893,   894,   895,    53,   396,   397,   398,   399,
     400,   401,   896,   403,   404,   405,   406,   407,   408,   409,
     410,   898,   900,   902,   903,   904,   910,   911,   921,   942,
      54,    55,   940,   964,   965,   969,   970,   971,   975,   976,
     983,   994,   995,   996,  1012,  1016,    56,  1018,  1009,  1017,
    1020,  1021,  1043,    57,  1022,  1039,  1034,  1035,  1057,    58,
      59,     4,     5,     6,     7,    60,   378,  1044,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   697,   698,    24,  1053,   946,    25,    26,    27,
      28,    29,   699,    31,    32,  1065,  1066,  1067,  1068,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,  1069,  1070,  1071,    48,    49,  1072,
    1073,    50,    51,  1075,  1076,  1077,  1081,    52,  1084,  1102,
    1103,    53,  1104,  1105,  1106,  1107,  1086,   722,  1110,   414,
     653,   420,     0,   421,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    56,     0,     0,     0,     0,     0,     0,    57,
       0,     0,     0,     0,     0,    58,    59,     4,     5,     6,
       7,    60,   700,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,  1058,  1059,
      24,     0,     0,    25,    26,    27,    28,    29,  1060,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
       0,     0,     0,    48,    49,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,    53,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   121,
     122,   113,   114,   115,    13,    14,    15,    16,    17,    18,
       0,     0,    54,    55,     0,     0,     0,     0,     0,     0,
     116,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,  1061,     0,
       0,     0,    50,    51,     4,     5,     6,     7,    52,     0,
       0,   121,   122,   113,   114,   115,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,    28,     0,     0,     0,    54,    55,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    56,     0,    46,     0,     0,     0,     0,
      57,     0,     0,     0,    50,    51,   148,   117,     0,     0,
      52,     0,    60,   581,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   121,   122,   113,   114,
     115,    13,    14,    15,    16,    17,    18,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,   116,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,   148,   117,
      46,     0,     0,     0,    60,   830,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,   148,   117,     4,     5,     6,     7,    60,
    1090,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,     0,
       0,    25,    26,    27,    28,    29,    30,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,    53,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   121,   122,   113,
     114,   115,    13,    14,    15,    16,    17,    18,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,   116,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,    58,
      59,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     4,     5,     6,     7,    52,     0,     0,   121,
     122,   113,   114,   115,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,    28,     0,     0,     0,    54,    55,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    56,     0,    46,     0,     0,     0,     0,    57,     0,
       0,     0,    50,    51,   123,   124,     0,     0,    52,     0,
      60,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   187,   122,   113,   114,   115,    13,
      14,    15,    16,    17,    18,     0,     0,    54,    55,     0,
       0,     0,     0,     0,     0,   116,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,   148,   117,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     4,
       5,     6,     7,    52,     0,     0,   121,   122,   113,   114,
     115,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,    28,     0,
       0,     0,    54,    55,     0,     0,     0,    33,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    56,     0,
      46,     0,     0,     0,     0,    57,     0,     0,     0,    50,
      51,     0,   117,   188,     0,    52,     0,    60,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   121,   122,   113,   114,   115,    13,    14,    15,
      16,    17,    18,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,   116,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,   117,   254,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     4,     5,     6,
       7,    52,     0,     0,   121,   122,   113,   114,   115,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,    28,     0,     0,     0,
      54,    55,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    56,     0,    46,     0,
       0,     0,     0,    57,   258,     0,     0,    50,    51,     0,
     117,     0,     0,    52,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     121,   122,   113,   114,   115,    13,    14,    15,    16,    17,
      18,     0,    54,    55,     0,     0,     0,     0,     0,     0,
       0,   116,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,   117,   188,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     4,     5,     6,     7,    52,
       0,     0,   121,   122,   113,   114,   115,    13,    14,    15,
      16,    17,    18,     0,     0,   346,     0,     0,     0,     0,
       0,     0,     0,   116,    28,     0,     0,     0,    54,    55,
       0,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    56,     0,    46,     0,     0,     0,
       0,    57,     0,     0,     0,    50,    51,     0,   117,   283,
       0,    52,     0,    60,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   121,   122,   113,
     114,   115,    13,    14,    15,    16,    17,    18,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,   116,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
     117,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,   191,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   121,   122,   113,   114,   115,    13,    14,    15,
      16,    17,    18,     0,     0,    54,    55,     0,     0,     0,
       0,     0,     0,   116,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   117,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     4,     5,     6,
       7,    52,     0,     0,   121,   122,   113,   114,   115,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   116,    28,     0,     0,     0,
      54,    55,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    56,     0,    46,     0,
       0,     0,     0,    57,     0,     0,     0,    50,    51,     0,
     117,   570,     0,    52,     0,    60,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     121,   122,   113,   114,   115,    13,    14,    15,    16,    17,
      18,     0,    54,    55,     0,     0,     0,     0,     0,     0,
       0,   116,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,   117,   618,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     4,     5,     6,     7,    52,
       0,     0,   121,   122,   113,   114,   115,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,    28,     0,     0,     0,    54,    55,
       0,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    56,     0,    46,     0,     0,     0,
       0,    57,     0,     0,     0,    50,    51,     0,   117,   621,
       0,    52,     0,    60,     0,     0,     0,     0,     0,     0,
       0,     4,     5,   492,     7,     0,     0,     0,   121,   122,
     113,   114,   493,    13,    14,    15,    16,    17,    18,     0,
      54,    55,     0,     0,     0,     0,     0,     0,     0,   116,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
     117,   632,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     4,     5,     6,     7,    52,     0,     0,
     121,   122,   113,   114,   115,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,    28,     0,     0,     0,    54,    55,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    56,     0,    46,     0,     0,     0,     0,    57,
       0,     0,     0,    50,    51,     0,   117,   188,     0,    52,
       0,    60,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   121,   122,   113,   114,
     115,    13,    14,    15,    16,    17,    18,     0,    54,    55,
       0,     0,     0,     0,     0,     0,     0,   116,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,   117,   765,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     4,     5,     6,     7,    52,     0,     0,   121,   122,
     113,   114,   115,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   116,
      28,     0,     0,     0,    54,    55,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      56,     0,    46,     0,     0,     0,     0,    57,     0,     0,
       0,    50,    51,     0,   117,   973,     0,    52,     0,    60,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   121,   122,   113,   114,   115,    13,    14,
      15,    16,    17,    18,     0,     0,    54,    55,     0,     0,
       0,     0,     0,     0,   116,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   134,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     4,     5,
       6,     7,    52,     0,     0,   121,   122,   113,   114,   115,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   116,    28,     0,     0,
       0,    54,    55,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    56,     0,    46,
       0,     0,     0,     0,    57,     0,     0,     0,    50,    51,
       0,   136,     0,     0,    52,     0,    60,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     121,   122,   113,   114,   115,    13,    14,    15,    16,    17,
      18,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,   116,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   138,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     4,     5,     6,     7,    52,
       0,     0,   121,   122,   113,   114,   115,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   116,    28,     0,     0,     0,    54,    55,
       0,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    56,     0,    46,     0,     0,     0,
       0,    57,     0,     0,     0,    50,    51,     0,   140,     0,
       0,    52,     0,    60,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   121,   122,   113,
     114,   115,    13,    14,    15,    16,    17,    18,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,   116,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
     117,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     4,     5,     6,     7,    52,     0,     0,   121,
     122,   113,   114,   115,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     116,    28,     0,     0,     0,    54,    55,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    56,     0,    46,     0,     0,     0,     0,    57,     0,
       0,     0,    50,    51,     0,   155,     0,     0,    52,     0,
      60,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   121,   122,   113,   114,   115,    13,
      14,    15,    16,    17,    18,     0,     0,    54,    55,     0,
       0,     0,     0,     0,     0,   116,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   157,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     4,
       5,     6,     7,    52,     0,     0,   121,   122,   113,   114,
     115,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,    28,     0,
       0,     0,    54,    55,     0,     0,     0,    33,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    56,     0,
      46,     0,     0,     0,     0,    57,     0,     0,     0,    50,
      51,     0,   117,     0,     0,    52,     0,   171,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   121,   122,   113,   114,   115,    13,    14,    15,    16,
      17,    18,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,   116,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   256,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     4,     5,   492,     7,
      52,     0,     0,   121,   122,   113,   114,   493,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   116,    28,     0,     0,     0,    54,
      55,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    56,     0,    46,     0,     0,
       0,     0,    57,     0,     0,     0,    50,    51,     0,   490,
       0,     0,    52,     0,    60,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   121,   122,
     113,   114,   115,    13,    14,    15,    16,    17,    18,     0,
       0,    54,    55,     0,     0,     0,     0,     0,     0,   116,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   494,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     4,     5,     6,     7,    52,     0,     0,
     121,   122,   113,   114,   115,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   116,    28,     0,     0,     0,    54,    55,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    56,     0,    46,     0,     0,     0,     0,    57,
       0,     0,     0,    50,    51,     0,   658,     0,     0,    52,
       0,    60,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   111,   112,   113,   114,   115,
      13,    14,    15,    16,    17,    18,     0,     0,    54,    55,
       0,     0,     0,     0,     0,     0,   116,     0,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,   682,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -449,  -449,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,   117,     0,     0,  -449,  -449,    60,  -449,
    -449,  -449,  -449,  -449,  -449,  -449,  -449,  -449,  -449,  -449,
    -449,  -449,     0,     0,     0,  -449,     0,     0,     0,     0,
       0,     0,     0,  -449,  -449,  -449,  -449,  -449,  -449,     0,
    -449,     0,     0,  -449,  -449,  -449,  -449,  -449,  -449,     0,
    -449,  -449,  -449,     0,  -449
};

static const yytype_int16 yycheck[] =
{
       2,   245,   217,     0,   522,   297,   679,    14,   469,    77,
     359,    26,    10,    11,   475,    77,    14,    26,    14,     0,
      22,    23,    24,    25,    26,    10,    11,    29,    30,    14,
      32,    14,    11,    35,    36,    15,    56,    57,    26,    22,
      60,    43,    26,    10,    46,   115,    14,    14,    80,   113,
      10,    14,    10,    11,    22,    43,    14,    59,   126,    43,
     128,    10,    11,   133,   126,    14,   128,    65,    66,    67,
      50,    26,    26,    14,    56,    57,    56,    57,    60,    10,
      65,    66,    67,    14,    78,    79,    26,   119,    43,    43,
      31,    37,    38,    95,    96,    97,    98,    99,   100,    62,
      68,   103,   104,    43,   106,   107,    29,    65,    66,    67,
      70,    71,   117,   118,   134,   117,    65,    66,    67,    10,
      11,    44,   124,    14,   133,   119,   113,   850,   135,    10,
      11,   129,   134,    14,   136,    14,   138,   133,   140,   127,
     813,   128,   130,   127,   129,   113,   134,   126,    10,   128,
     133,    12,   134,   155,   503,   157,   617,   128,   160,   127,
     162,   163,   133,   165,   166,   113,   168,   169,   170,   171,
     172,   129,   127,   127,    65,    66,    67,    10,    11,   134,
     129,    14,    80,   131,    65,    66,    67,   127,   134,   133,
     192,   193,   194,   195,   196,   197,   198,   199,   200,    12,
     202,   203,   204,   205,   206,   207,   208,   209,    70,    71,
       2,    74,    75,    76,    14,   133,   231,   232,   567,   568,
      14,   119,    22,    17,    78,    79,    10,    11,    12,   126,
      14,   128,    65,    66,    67,    27,    28,   115,   129,    98,
      99,    14,   113,   966,   536,    37,    38,    31,   129,    22,
      14,    35,    44,    45,   256,   133,   127,   237,   238,   239,
     240,   241,   242,   243,    58,   119,    26,    26,    68,   128,
     272,   134,   274,   565,   276,   277,   278,   279,   280,   281,
     282,   113,   127,    43,    43,   287,   288,  1010,   290,   134,
     292,  1014,   810,   120,   121,    68,   129,    26,   115,   131,
     102,   303,   304,   305,   306,   307,   308,   309,   110,   324,
     325,   313,   314,   113,    43,   130,   133,   132,   991,   127,
     111,   130,   124,   132,   116,    14,   134,   127,   330,   331,
     327,   333,   334,  1056,   115,   337,   127,   339,    26,   130,
     113,  1064,   344,   345,   128,    14,    26,   139,    26,   141,
     142,    14,   133,    10,   127,    43,  1079,    10,    11,    22,
     113,    14,   113,    43,   156,    43,   158,   127,   127,   661,
     372,   373,   374,   375,   134,   134,   127,    26,   131,    26,
      29,    30,    29,    30,    10,    26,  1109,    36,   115,    36,
      39,    40,    39,    40,    43,    44,    43,   113,   127,   614,
     192,   130,    43,   695,   128,    68,   133,    80,   130,   133,
     132,   127,    65,    66,    67,     3,     4,     5,     6,    80,
     129,   130,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   127,    10,    11,   728,   729,    14,   127,
     134,   127,   130,    31,    32,    72,   134,   127,   134,   127,
     113,   453,   130,    41,   134,   457,   134,   130,   460,   461,
     133,    49,    50,    51,   127,     5,    54,   105,   470,   130,
     113,   110,   133,   475,    14,    63,    64,   112,   127,   723,
     127,    69,   111,   130,   127,   134,   127,   134,   490,    65,
      66,    67,   494,   134,   130,   497,   132,   499,   127,   501,
     113,   130,   113,   505,   506,   113,   106,   107,   108,   311,
      98,    99,   804,   128,   316,   128,   127,   126,   133,   127,
     322,   523,   122,   123,   124,    26,   114,   128,    29,    30,
     127,   127,   133,   121,   130,    36,   538,   134,    39,    40,
     128,   129,    43,    44,   127,   133,   127,   135,   126,   130,
     127,   134,   554,    85,    86,    87,    88,    89,    90,   127,
     127,   131,   364,   130,   356,    97,   134,   859,   860,   127,
     572,   127,   127,   575,   576,   577,   134,   109,   134,   134,
     127,   583,   584,   130,   586,   587,    14,   589,   590,   591,
     592,   593,   594,   595,   596,    14,   945,    26,   127,   948,
      29,    30,   127,   129,   130,   134,   127,    36,   127,   134,
      39,    40,   856,   134,    43,   134,   127,   127,   127,   127,
     130,    26,   130,   134,    29,    30,   127,   629,   630,   631,
     127,    36,   127,   134,    39,    40,   638,   134,    43,   134,
     127,   127,   127,    14,   446,   130,   648,   134,   134,   651,
     652,   127,   454,    14,   130,    10,   658,   459,    26,   127,
     462,    29,    30,   665,   128,   467,   134,   469,    36,   133,
     127,    39,    40,   127,   127,    43,    44,   134,   127,   127,
     682,   134,   127,    14,    53,   134,   134,  1036,   690,   134,
     113,    22,   127,    24,    25,   697,   698,   699,   127,   134,
     127,   130,    33,    34,   706,   134,   508,   134,   128,   127,
      41,    42,   130,   133,   127,   517,   718,   127,    49,    50,
      51,   134,   127,   127,   134,   130,   130,    58,   113,   521,
     522,   127,   127,   525,   526,   527,   127,   128,   134,   134,
     742,   743,   744,   745,   746,   747,   538,   127,   127,    10,
      11,    12,   133,   127,   134,   134,   548,   549,   550,   127,
     134,   127,   127,     3,     4,     5,     6,   134,   134,   134,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   127,   127,   127,   133,   787,   129,   130,   134,   134,
     134,    31,    32,    81,    82,    83,    84,   120,   121,   127,
     127,    41,   128,    91,    92,    93,   134,   134,   133,    49,
      50,    51,   127,   615,    54,   617,   818,   127,   127,   134,
     127,   623,   127,    63,    64,   134,   127,   115,   116,    69,
     832,   833,   834,   835,   836,   837,   838,   839,    98,    99,
     100,   101,   102,   103,   104,   847,   127,    26,   650,   127,
      29,    30,   127,   113,   116,   127,   134,    36,    98,    99,
      39,    40,   134,   865,    43,    44,    26,   134,   128,    29,
      30,   127,    10,   128,   114,    13,    36,    15,    16,    39,
      40,   121,   128,    43,    44,     7,     8,     9,   128,   129,
      14,   683,   127,   133,    26,   135,   898,    29,    30,   129,
     130,   903,   904,    14,    36,   129,   130,    39,    40,    14,
      14,    43,   113,    17,   113,   707,   129,   130,    22,   921,
      24,    25,    10,   925,   926,   927,   228,   229,   230,    33,
      34,   129,   130,    14,   129,   130,    14,    41,    42,    10,
     942,    22,    14,    24,    25,    49,    50,    51,   127,   946,
      27,   953,    33,    34,   127,   134,   958,   129,   130,    10,
      41,    42,    13,    14,    15,    16,   127,   127,    49,    50,
      51,   129,   130,   975,   976,   127,    96,   127,    98,    99,
     100,   101,   102,   103,   104,    27,    28,   134,   790,   129,
     130,   127,   994,   113,   128,   127,   129,   130,   130,   801,
     129,   130,   134,   129,   130,   128,   126,   113,   128,    27,
      28,  1013,   129,   130,   129,   130,   129,   130,   810,   129,
     130,    10,   814,   815,   816,  1027,   129,   130,   129,   130,
    1032,    98,    99,   100,   101,   102,   103,   104,   129,   130,
     129,   130,   129,   130,   129,   130,   113,   129,   130,   841,
     113,  1053,   109,   845,   129,   130,  1058,  1059,  1060,   129,
     130,   128,   129,   130,   866,   129,   130,   129,   130,   127,
       3,     4,     5,     6,   129,   130,   119,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,   129,   130,
    1092,  1093,  1094,  1095,  1096,  1097,  1098,  1099,    31,    32,
     129,   130,   128,   905,   129,   130,   127,   909,    41,  1111,
    1112,  1113,   129,   130,   129,   130,    49,    50,    51,   129,
     130,    54,   914,   915,    27,    28,   129,   130,   129,   130,
      63,    64,     3,     4,     5,     6,    69,   129,   130,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      27,    28,   129,   130,   129,   130,   129,   130,    42,    43,
      31,    32,   235,   236,   127,    98,    99,   127,   127,    96,
      41,    98,    99,   100,   101,   102,   103,   104,    49,    50,
      51,   114,   127,    54,   127,   127,   113,   127,   121,   127,
     127,   127,    63,    64,    94,   128,   129,   127,    69,   126,
     133,   128,   135,   127,   127,   127,    14,   128,   113,   113,
     128,   113,   127,   127,   134,   116,    77,  1009,   127,   113,
     127,  1013,   133,    14,   113,    14,   134,    98,    99,   127,
     127,   127,   127,   116,   127,   127,   127,   127,   116,   127,
     127,   127,   127,   114,   113,    14,   127,   127,   127,    77,
     121,   127,   134,   127,    14,  1047,    10,   128,    10,   132,
     132,   127,   133,   127,   135,     3,     4,     5,     6,   127,
     127,    10,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   113,
      10,    29,    30,    31,    32,    33,    34,    35,    36,    10,
     127,   127,    27,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,  1110,    56,
     127,    59,    60,    14,    14,    63,    64,    14,   127,   134,
     127,    69,   127,   127,   127,    73,   211,   212,   213,   214,
     215,   216,   129,   218,   219,   220,   221,   222,   223,   224,
     225,   128,   127,   127,    77,    77,   127,   127,   113,   127,
      98,    99,   134,   127,   127,     7,     7,     7,   113,   113,
     127,   113,    10,    10,    72,    14,   114,   127,   129,    14,
      14,    14,   129,   121,   127,   134,   127,   127,   134,   127,
     128,     3,     4,     5,     6,   133,   134,   127,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   127,   133,    29,    30,    31,
      32,    33,    34,    35,    36,   127,   127,   127,   127,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,   127,   127,    14,    59,    60,    14,
     127,    63,    64,    14,    14,   127,   134,    69,   127,   127,
     127,    73,   127,   127,   127,   127,  1053,   563,   129,   227,
     494,   233,    -1,   234,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    -1,    -1,   127,   128,     3,     4,     5,
       6,   133,   134,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    -1,    -1,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      -1,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      41,    -1,    -1,    -1,    -1,   121,    -1,    -1,    49,    50,
      51,   127,   128,    54,    -1,    -1,    -1,   133,   134,    -1,
      -1,    -1,    63,    64,     3,     4,     5,     6,    69,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    63,    64,   127,   128,    -1,    -1,
      69,    -1,   133,   134,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    49,    50,    51,   127,   128,
      54,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    -1,    -1,   127,   128,     3,     4,     5,     6,   133,
     134,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    49,    50,    51,   127,
     128,    54,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      63,    64,     3,     4,     5,     6,    69,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    63,    64,   127,   128,    -1,    -1,    69,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    41,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    49,    50,    51,   127,   128,    54,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    63,    64,     3,
       4,     5,     6,    69,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,   114,    -1,
      54,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    63,
      64,    -1,   128,   129,    -1,    69,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    -1,    41,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    49,    50,    51,   128,   129,    54,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    63,    64,     3,     4,     5,
       6,    69,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,   114,    -1,    54,    -1,
      -1,    -1,    -1,   121,   122,    -1,    -1,    63,    64,    -1,
     128,    -1,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    41,    -1,    -1,    -1,   121,    -1,    -1,    -1,    49,
      50,    51,   128,   129,    54,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    63,    64,     3,     4,     5,     6,    69,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    63,    64,    -1,   128,   129,
      -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    49,    50,    51,    -1,
     128,    54,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    72,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    41,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    49,    50,    51,    -1,   128,    54,    -1,    -1,    -1,
     133,    -1,    -1,    -1,    -1,    63,    64,     3,     4,     5,
       6,    69,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,   114,    -1,    54,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    63,    64,    -1,
     128,   129,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    41,    -1,    -1,    -1,   121,    -1,    -1,    -1,    49,
      50,    51,   128,   129,    54,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    63,    64,     3,     4,     5,     6,    69,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    63,    64,    -1,   128,   129,
      -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    41,
      -1,    -1,    -1,   121,    -1,    -1,    -1,    49,    50,    51,
     128,   129,    54,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    63,    64,     3,     4,     5,     6,    69,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    63,    64,    -1,   128,   129,    -1,    69,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    -1,    41,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    49,    50,    51,   128,   129,
      54,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    63,
      64,     3,     4,     5,     6,    69,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    63,    64,    -1,   128,   129,    -1,    69,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    41,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    49,    50,    51,    -1,   128,    54,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    63,    64,     3,     4,
       5,     6,    69,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   114,    -1,    54,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    63,    64,
      -1,   128,    -1,    -1,    69,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    41,    -1,    -1,    -1,    -1,   121,    -1,    -1,    49,
      50,    51,    -1,   128,    54,    -1,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    63,    64,     3,     4,     5,     6,    69,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,
      -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,    63,    64,    -1,   128,    -1,
      -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    49,    50,    51,    -1,
     128,    54,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,
      63,    64,     3,     4,     5,     6,    69,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    63,    64,    -1,   128,    -1,    -1,    69,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    41,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    49,    50,    51,    -1,   128,    54,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    63,    64,     3,
       4,     5,     6,    69,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,   114,    -1,
      54,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    63,
      64,    -1,   128,    -1,    -1,    69,    -1,   133,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     114,    -1,    41,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      49,    50,    51,    -1,   128,    54,    -1,    -1,    -1,   133,
      -1,    -1,    -1,    -1,    63,    64,     3,     4,     5,     6,
      69,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   114,    -1,    54,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    63,    64,    -1,   128,
      -1,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    41,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    49,    50,    51,
      -1,   128,    54,    -1,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    63,    64,     3,     4,     5,     6,    69,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    63,    64,    -1,   128,    -1,    -1,    69,
      -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    41,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    49,    50,    51,    -1,   128,    54,
      -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,    -1,    78,    79,   133,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,   106,   107,   108,   109,   110,    -1,
     112,    -1,    -1,   115,   116,   117,   118,   119,   120,    -1,
     122,   123,   124,    -1,   126
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   137,   138,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    69,    73,    98,    99,   114,   121,   127,   128,
     133,   139,   140,   142,   143,   144,   145,   146,   148,   149,
     151,   152,   155,   156,   159,   164,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   181,
     182,   184,    96,    98,    99,   100,   101,   102,   103,   104,
     113,   126,   128,   113,   131,   115,   133,   113,    11,   126,
     128,    10,    11,    12,    13,    14,    31,   128,   164,   177,
      14,    10,    11,   127,   128,   166,   183,    10,    11,    12,
      14,    31,    35,   128,   128,   166,   128,   166,   128,   166,
     128,   166,   166,   173,    14,   164,   164,   166,   127,   166,
      10,    14,   166,    12,    12,   128,   166,   128,   166,   164,
     128,   164,    26,    43,   127,    26,    43,   127,   133,   165,
     165,   133,   165,   166,   164,   164,   166,    14,    14,    14,
      31,   177,    14,    10,    10,   177,   177,    10,   129,   166,
     179,    72,   138,    26,    29,    30,    36,    39,    40,    43,
      44,   127,    26,    29,    30,    36,    39,    40,    43,    44,
     127,    81,    82,    83,    84,    91,    92,    93,   115,   116,
      85,    86,    87,    88,    89,    90,    97,   109,   117,   118,
     105,    78,    79,   119,   110,   120,   121,   106,   107,   108,
     122,   123,   124,   112,   126,   126,    14,   166,   166,   166,
     166,   166,   166,    14,   129,   183,   128,   166,   122,   166,
      14,   166,   166,   127,    14,    17,    58,    14,   129,   135,
     161,   183,   131,   115,   133,   127,   100,   101,   102,   103,
     104,   113,   113,   129,   166,   183,   127,   130,   113,   127,
     113,   127,   113,   127,    10,    11,    12,    14,    10,    14,
      10,   179,   166,    26,    29,    30,    36,    39,    40,    43,
     127,   130,   166,    26,    43,   127,   130,   166,   164,   166,
     164,    80,   130,   164,    78,    79,   128,   133,   153,   127,
      26,    43,   127,   113,   113,   133,   115,   133,   115,   133,
     166,   164,   166,   164,    29,    44,    23,   166,   166,   166,
     166,   166,   166,   166,   166,   166,    53,   157,   133,   128,
      14,    62,   133,   129,   130,   129,   130,   134,    10,    11,
      12,    13,    24,    25,    33,    34,    47,    48,   134,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   169,   169,   169,   169,
     169,   169,   167,   169,   169,   169,   169,   169,   169,   169,
     169,     7,     8,     9,   170,   172,   172,   172,   173,   173,
     174,   175,   176,   176,   177,   177,   177,   177,   177,   177,
     177,    14,    22,    24,    25,    33,    34,    41,    42,    49,
      50,    51,   180,    17,   180,   127,   128,   127,   127,   127,
     127,   127,   127,   113,   128,   129,   166,   111,   127,   130,
     120,   121,   130,   132,   116,   134,   127,   128,    14,   128,
      77,    14,   129,   130,   129,   130,   166,    14,   166,   166,
     166,   166,   166,   166,   166,   166,   127,   129,   166,   166,
     128,   166,     5,    14,   128,   162,   166,   113,   127,   113,
     127,   113,   127,   128,   153,   113,   113,   129,   130,   166,
     166,   166,   166,   166,   166,   166,   183,   130,   166,   166,
     183,   129,    27,    28,   141,   129,    10,   183,   173,   173,
      10,    11,    14,    65,    66,    67,   129,   163,   138,   166,
     166,   166,   166,   150,    14,   166,    14,   166,   129,    27,
     129,   166,   166,    10,   127,   127,   127,   127,   127,   134,
     164,    14,    22,   133,   158,   129,   163,   128,   128,   160,
     129,   183,   113,    10,   126,   131,   133,   113,   166,   166,
     166,   134,   166,    26,    43,   134,    26,    43,   134,    26,
      29,    30,    36,    39,    40,    43,    44,   134,   127,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,    94,   128,    14,   128,   129,   183,
     166,   129,   183,   130,   166,   183,   166,   166,   183,   113,
     113,   113,   129,   183,   129,   161,   183,   166,   128,    14,
     135,   161,   132,   116,   134,   127,   127,   166,   111,   127,
     130,    77,    77,   162,   127,   130,   127,   166,   128,   166,
     166,   129,   163,   166,   166,   113,   183,   127,   127,   127,
     127,   127,   127,   127,   127,   183,   127,   127,   127,   164,
     140,   164,   128,   166,   164,   164,    80,   164,    80,    80,
     113,    10,    70,    71,   153,   129,   130,    24,    25,    34,
     134,   166,   127,   127,   127,   127,    37,    38,   134,   116,
     134,   116,   134,   164,   164,   164,   127,   127,   113,   166,
      14,    14,   158,    56,    57,    60,   134,   153,   129,   129,
     163,   163,    74,    75,    76,   134,   129,   166,    14,   166,
     166,   166,    26,    30,    36,    39,    40,    43,   134,   134,
     134,   134,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   167,   129,   183,   129,   161,   183,
     127,   129,   127,   129,   183,   127,   127,   132,   132,   132,
     166,   166,   166,   129,   129,   129,   166,    77,    14,   129,
     130,   166,   183,   166,   166,   129,     5,    14,   127,   166,
     127,   130,   127,   153,   129,   127,   127,   166,   129,   129,
      27,   141,   166,   164,    10,    10,    10,   166,   113,    10,
      10,   153,    10,    11,    14,    65,    66,    67,   166,   166,
     134,   166,    26,    29,    30,    36,    39,    40,    43,    44,
     134,   166,   164,   127,   127,    27,   166,   127,   134,    58,
     180,    10,    13,    14,    15,    16,    56,   153,   153,   129,
     129,    14,    14,    14,   127,   113,   128,   132,   134,   134,
     166,   166,   166,   166,   166,   166,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   129,   129,
     129,   127,   129,   127,   127,   127,   129,   166,   128,   183,
     127,   127,   127,    77,    77,   130,   183,   153,   127,   130,
     127,   127,   140,   164,   129,    27,   141,   164,   164,   164,
     166,   113,    10,    70,    71,    26,    30,    43,   134,   134,
     134,   166,   166,   166,   166,   166,   166,   166,   166,   164,
     134,   164,   127,   147,   166,   128,   133,   154,   128,   154,
      14,    22,    68,   113,   127,    14,    22,    68,   113,   127,
      10,    13,    15,    16,   127,   127,   180,   153,   153,     7,
       7,     7,   166,   129,   183,   113,   113,   134,   134,   134,
     134,   134,   134,   127,   166,   129,   166,   166,   183,   127,
     183,   164,   164,   166,   113,    10,    10,   166,   166,   166,
     134,   134,   134,   134,   134,   134,   134,   134,   166,   129,
     129,   163,    72,   138,   129,   163,    14,    14,   127,   166,
      14,    14,   127,   166,    14,    22,    68,   113,   127,    14,
      22,    68,   113,   127,   127,   127,   128,   154,   134,   134,
     129,   166,   166,   129,   127,   129,   127,    27,   141,   166,
     134,   134,   134,   127,   164,   154,   129,   134,    24,    25,
      34,   134,   166,   154,   129,   127,   127,   127,   127,   127,
     127,    14,    14,   127,   166,    14,    14,   127,   166,   129,
     163,   134,   134,   134,   127,   164,   147,   154,   166,   166,
     134,   166,    26,    29,    30,    36,    39,    40,    43,    44,
     134,   154,   127,   127,   127,   127,   127,   127,   154,   129,
     129,    26,    30,    43,   134,   134,   134,   166,   166,   166,
     166,   166,   166,   166,   166,   154,   164,   166,   166,   166,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   136,   137,   138,   138,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   140,   140,
     140,   140,   140,   140,   140,   140,   141,   141,   141,   141,
     141,   141,   142,   142,   143,   143,   143,   143,   144,   144,
     145,   145,   145,   146,   146,   146,   147,   148,   148,   148,
     148,   148,   148,   149,   149,   150,   150,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   152,   152,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   155,   156,   157,   157,   157,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   160,   160,   160,   160,   161,
     161,   161,   161,   162,   162,   162,   162,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   165,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   167,   167,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   169,   169,   170,   170,   170,   171,   171,   172,   172,
     172,   173,   173,   174,   174,   175,   175,   175,   176,   176,
     176,   176,   176,   176,   176,   177,   177,   177,   177,   177,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   181,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   183,   183,   183,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     7,     9,     4,     6,     6,     8,
       5,     5,     7,     6,     6,     8,    10,     6,     4,     4,
       4,     3,     5,     5,     6,     6,     2,     5,     4,     3,
       3,     3,     5,     7,     3,     5,     7,     3,     3,     2,
       5,     5,     4,     7,     6,     4,     6,     4,     2,     4,
       4,     4,     4,     4,     4,     4,     4,     7,     7,     7,
       6,     6,     4,     4,     4,     4,     4,     4,     4,     4,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     4,
       4,     4,     4,     4,     2,     1,     1,     1,     1,     1,
       2,     4,     2,     1,     1,     1,     1,     1,     2,     2,
       4,     4,     4,     4,     1,     1,     1,     1,     5,     7,
       7,     3,     5,     5,     4,     6,     3,     5,     5,     7,
       4,     6,     5,     3,     5,     7,     3,     5,     5,     3,
       2,     5,     5,     2,    12,     9,     1,     7,     7,     5,
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
       5,     5,     7,     3,     3,     5,     5,     1,     1,     3,
       2,     3,     3,     3,     4,     5,     5,     3,     5,     4,
       6,     1,     3,     1,     3,     1,     3,     3,     3,     4,
       6,     6,     6,     6,     6,     6,     6,     6,     5,     7,
       7,     7,     7,     7,     7,     5,     9,     8,     8,     6,
       9,     9,     5,     5,     4,     4,     4,     6,     6,     6,
       6,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       1,     5,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     2,     2,     1,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     4,     6,
       4,     3,     6,     6,     5,     3,     3,     6,     5,     4,
       4,     6,     6,     5,     3,     6,     5,     3,     2,     3,
       3,     2,     3,     1,     1,     3,     2,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     1,     1,     1,
       1,     1,     4,     6,     6,     6,     1,     4,     4,     5,
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
#line 396 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2891 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 406 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2897 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 407 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2903 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2909 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2915 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 415 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2921 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 417 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2929 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 421 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2935 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 423 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2941 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 425 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2947 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 427 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2953 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 429 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2961 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 433 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2968 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 436 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2976 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 440 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2982 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 442 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2990 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 446 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2997 "raku.tab.c"
    break;

  case 19: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 449 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3005 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 453 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3011 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 455 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3017 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 457 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3023 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3029 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 461 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3035 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 463 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3043 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 467 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3051 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 471 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3057 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 473 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3063 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 475 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3069 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 477 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3075 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE IDENT ';'  */
#line 479 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3081 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3087 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3093 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3099 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 487 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3105 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 489 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3111 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 491 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3117 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 493 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3123 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 495 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3129 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 497 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3135 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 499 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3141 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 501 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3148 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 504 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3155 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3161 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 509 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3168 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 512 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3175 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 515 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3181 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 517 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3187 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 519 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3193 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 521 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3200 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3207 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 527 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3213 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 529 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3222 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 534 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3230 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 538 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3238 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 542 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3247 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3255 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall ';'  */
#line 550 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3261 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 552 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3267 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 554 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3273 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 556 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3279 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 558 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3285 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 560 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3292 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 563 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3298 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 565 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3304 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 567 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3310 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 569 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3317 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 572 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3324 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 575 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3331 "raku.tab.c"
    break;

  case 70: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 578 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3338 "raku.tab.c"
    break;

  case 71: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 581 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3345 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_IF expr ';'  */
#line 584 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3351 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_UNLESS expr ';'  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3357 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WHILE expr ';'  */
#line 588 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3363 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_UNTIL expr ';'  */
#line 590 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3369 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_FOR expr ';'  */
#line 592 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3376 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WITH expr ';'  */
#line 595 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3382 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_WITHOUT expr ';'  */
#line 597 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3388 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_GIVEN expr ';'  */
#line 599 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3394 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 601 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3401 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 604 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3408 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 607 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3416 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 611 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3423 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 614 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3429 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 616 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3435 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 618 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3441 "raku.tab.c"
    break;

  case 87: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 620 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3448 "raku.tab.c"
    break;

  case 88: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 623 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3455 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 626 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3462 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 629 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3469 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 632 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3476 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 635 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3483 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 638 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3490 "raku.tab.c"
    break;

  case 94: /* stmt: expr ';'  */
#line 640 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3496 "raku.tab.c"
    break;

  case 95: /* stmt: ';'  */
#line 641 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3502 "raku.tab.c"
    break;

  case 96: /* stmt: if_stmt  */
#line 642 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3508 "raku.tab.c"
    break;

  case 97: /* stmt: while_stmt  */
#line 643 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3514 "raku.tab.c"
    break;

  case 98: /* stmt: for_stmt  */
#line 644 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3520 "raku.tab.c"
    break;

  case 99: /* stmt: given_stmt  */
#line 645 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3526 "raku.tab.c"
    break;

  case 100: /* stmt: KW_TRY block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3532 "raku.tab.c"
    break;

  case 101: /* stmt: KW_TRY block KW_CATCH block  */
#line 649 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3538 "raku.tab.c"
    break;

  case 102: /* stmt: KW_CATCH block  */
#line 651 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3544 "raku.tab.c"
    break;

  case 103: /* stmt: block  */
#line 653 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3550 "raku.tab.c"
    break;

  case 104: /* stmt: unless_stmt  */
#line 654 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3556 "raku.tab.c"
    break;

  case 105: /* stmt: until_stmt  */
#line 655 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3562 "raku.tab.c"
    break;

  case 106: /* stmt: repeat_stmt  */
#line 656 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3568 "raku.tab.c"
    break;

  case 107: /* stmt: loop_stmt  */
#line 657 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3574 "raku.tab.c"
    break;

  case 108: /* stmt: KW_LAST ';'  */
#line 658 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3580 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT ';'  */
#line 659 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3586 "raku.tab.c"
    break;

  case 110: /* stmt: KW_LAST KW_IF expr ';'  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3592 "raku.tab.c"
    break;

  case 111: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3598 "raku.tab.c"
    break;

  case 112: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 665 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3604 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3610 "raku.tab.c"
    break;

  case 114: /* stmt: sub_decl  */
#line 668 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3616 "raku.tab.c"
    break;

  case 115: /* stmt: class_decl  */
#line 669 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3622 "raku.tab.c"
    break;

  case 116: /* stmt: role_decl  */
#line 670 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3628 "raku.tab.c"
    break;

  case 117: /* stmt: grammar_decl  */
#line 671 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3634 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF '(' expr ')' block  */
#line 675 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3640 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 677 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3646 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 679 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3652 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF expr block  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3658 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3664 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 685 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block elsif_tail  */
#line 687 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3676 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 689 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3682 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3688 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3694 "raku.tab.c"
    break;

  case 128: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 697 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3700 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3706 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3712 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3718 "raku.tab.c"
    break;

  case 132: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 707 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3724 "raku.tab.c"
    break;

  case 133: /* while_stmt: KW_WHILE expr block  */
#line 709 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3730 "raku.tab.c"
    break;

  case 134: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 713 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 135: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 715 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3742 "raku.tab.c"
    break;

  case 136: /* unless_stmt: KW_UNLESS expr block  */
#line 717 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3748 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3754 "raku.tab.c"
    break;

  case 138: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 723 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3760 "raku.tab.c"
    break;

  case 139: /* until_stmt: KW_UNTIL expr block  */
#line 725 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3766 "raku.tab.c"
    break;

  case 140: /* repeat_stmt: KW_REPEAT block  */
#line 729 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3772 "raku.tab.c"
    break;

  case 141: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 731 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3778 "raku.tab.c"
    break;

  case 142: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 733 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3784 "raku.tab.c"
    break;

  case 143: /* loop_stmt: KW_LOOP block  */
#line 737 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3790 "raku.tab.c"
    break;

  case 144: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 739 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3796 "raku.tab.c"
    break;

  case 145: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 741 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3802 "raku.tab.c"
    break;

  case 146: /* loop_incr: expr  */
#line 744 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3808 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 748 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3818 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 754 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3828 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 760 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3837 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 765 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3847 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 771 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3856 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr block  */
#line 776 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3863 "raku.tab.c"
    break;

  case 153: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 781 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3875 "raku.tab.c"
    break;

  case 154: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 789 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3888 "raku.tab.c"
    break;

  case 155: /* when_list: %empty  */
#line 799 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3894 "raku.tab.c"
    break;

  case 156: /* when_list: when_list KW_WHEN expr block  */
#line 801 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3902 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 807 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3914 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 815 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3924 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_SUB IDENT sub_body  */
#line 821 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3934 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 827 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3946 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 835 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3956 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 841 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3966 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 847 "raku.y"
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

  case 164: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 856 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3990 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 863 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4003 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 872 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4016 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 881 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4027 "raku.tab.c"
    break;

  case 168: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 890 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4038 "raku.tab.c"
    break;

  case 169: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 897 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4047 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list '}'  */
#line 903 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4053 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list expr '}'  */
#line 905 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4060 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 908 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4067 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 911 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4073 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 913 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4079 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 915 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4085 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 917 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4091 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 919 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4097 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 921 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4103 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 923 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4109 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 925 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4116 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 928 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4122 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 930 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4128 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 932 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4134 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 934 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4141 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 937 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4148 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 940 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4156 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list '}'  */
#line 945 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4162 "raku.tab.c"
    break;

  case 188: /* method_body: '{' YADA '}'  */
#line 946 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4168 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list expr '}'  */
#line 948 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4175 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 951 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4182 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 954 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4188 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 956 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4194 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 958 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4200 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 960 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4206 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 962 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4212 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 964 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4218 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 966 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4224 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 968 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4231 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 971 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4237 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 973 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4243 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 975 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4249 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 977 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4256 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 980 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4263 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 983 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4271 "raku.tab.c"
    break;

  case 205: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 989 "raku.y"
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
#line 4289 "raku.tab.c"
    break;

  case 206: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1005 "raku.y"
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
#line 4306 "raku.tab.c"
    break;

  case 207: /* is_clauses: %empty  */
#line 1019 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4312 "raku.tab.c"
    break;

  case 208: /* is_clauses: is_clauses IDENT IDENT  */
#line 1021 "raku.y"
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
#line 4328 "raku.tab.c"
    break;

  case 209: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1033 "raku.y"
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
#line 4344 "raku.tab.c"
    break;

  case 210: /* class_body_list: %empty  */
#line 1046 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4350 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1048 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4357 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1051 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4364 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1054 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4371 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1057 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4378 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1060 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4385 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1063 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4392 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1066 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4399 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1069 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4406 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1072 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4413 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1075 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4420 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1078 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4427 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1081 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4434 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1084 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4445 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1091 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4456 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1098 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4467 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1105 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4478 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1112 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4486 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1116 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4494 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1120 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4502 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1124 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4510 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1128 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4521 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1135 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4532 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1142 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4543 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1149 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4554 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1156 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4568 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1166 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4580 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1174 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4592 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1182 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4605 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1191 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4616 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1198 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4627 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1205 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4641 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1215 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4653 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1223 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4665 "raku.tab.c"
    break;

  case 244: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1233 "raku.y"
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
#line 4682 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: %empty  */
#line 1247 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4688 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1249 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4697 "raku.tab.c"
    break;

  case 247: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1254 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4706 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1259 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4715 "raku.tab.c"
    break;

  case 249: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1266 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4723 "raku.tab.c"
    break;

  case 250: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1270 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4731 "raku.tab.c"
    break;

  case 251: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1274 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4739 "raku.tab.c"
    break;

  case 252: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1278 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4747 "raku.tab.c"
    break;

  case 253: /* pair_list: IDENT OP_FATARROW expr  */
#line 1284 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4753 "raku.tab.c"
    break;

  case 254: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1286 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4759 "raku.tab.c"
    break;

  case 255: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1288 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4765 "raku.tab.c"
    break;

  case 256: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1290 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4771 "raku.tab.c"
    break;

  case 257: /* param_list: VAR_SCALAR  */
#line 1293 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4777 "raku.tab.c"
    break;

  case 258: /* param_list: VAR_ARRAY  */
#line 1294 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4783 "raku.tab.c"
    break;

  case 259: /* param_list: param_list ',' VAR_ARRAY  */
#line 1295 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4789 "raku.tab.c"
    break;

  case 260: /* param_list: IDENT VAR_SCALAR  */
#line 1296 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4795 "raku.tab.c"
    break;

  case 261: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1297 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4801 "raku.tab.c"
    break;

  case 262: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1298 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4807 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' VAR_SCALAR  */
#line 1299 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4813 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1300 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4819 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1301 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4825 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1302 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4831 "raku.tab.c"
    break;

  case 267: /* param_list: VAR_SCALAR '=' expr  */
#line 1303 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4837 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1304 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4843 "raku.tab.c"
    break;

  case 269: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1305 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4849 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1306 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4855 "raku.tab.c"
    break;

  case 271: /* param_list: SLURPY_POS  */
#line 1307 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4861 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' SLURPY_POS  */
#line 1308 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4867 "raku.tab.c"
    break;

  case 273: /* param_list: SLURPY_LOL  */
#line 1309 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4873 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' SLURPY_LOL  */
#line 1310 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4879 "raku.tab.c"
    break;

  case 275: /* param_list: SLURPY_NAMED  */
#line 1311 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4885 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1312 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4891 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list '}'  */
#line 1315 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4897 "raku.tab.c"
    break;

  case 278: /* block: '{' YADA '}'  */
#line 1316 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4903 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list expr '}'  */
#line 1318 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4909 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1320 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4915 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1322 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4921 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1324 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4927 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4933 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1328 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4940 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1331 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4946 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1333 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4952 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1335 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4958 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1337 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4964 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1339 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4971 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1342 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4978 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1345 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4986 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1349 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4993 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1352 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5000 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1355 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5007 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1358 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5013 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1360 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5021 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1364 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5028 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1367 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5035 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1370 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5042 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1373 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5049 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1376 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5056 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1379 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5062 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1381 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5068 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1383 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5074 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_LAST '}'  */
#line 1385 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5080 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1387 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5086 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1389 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5092 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1391 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5098 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1393 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5104 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1395 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5110 "raku.tab.c"
    break;

  case 311: /* closure: '{' expr '}'  */
#line 1398 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5116 "raku.tab.c"
    break;

  case 312: /* expr: VAR_SCALAR '=' expr  */
#line 1401 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5122 "raku.tab.c"
    break;

  case 313: /* expr: VAR_ARRAY '=' expr  */
#line 1402 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5128 "raku.tab.c"
    break;

  case 314: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1403 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5134 "raku.tab.c"
    break;

  case 315: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1404 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5140 "raku.tab.c"
    break;

  case 316: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1405 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5146 "raku.tab.c"
    break;

  case 317: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1406 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5152 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1407 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5158 "raku.tab.c"
    break;

  case 319: /* expr: KW_GATHER block  */
#line 1408 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5168 "raku.tab.c"
    break;

  case 320: /* expr: tern_expr  */
#line 1413 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5174 "raku.tab.c"
    break;

  case 321: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1417 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5180 "raku.tab.c"
    break;

  case 322: /* tern_expr: cmp_expr  */
#line 1418 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5186 "raku.tab.c"
    break;

  case 323: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1421 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5192 "raku.tab.c"
    break;

  case 324: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1422 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5198 "raku.tab.c"
    break;

  case 325: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1423 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5204 "raku.tab.c"
    break;

  case 326: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1424 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5210 "raku.tab.c"
    break;

  case 327: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1425 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5216 "raku.tab.c"
    break;

  case 328: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1426 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5222 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1427 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5228 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1428 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5234 "raku.tab.c"
    break;

  case 331: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1429 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5240 "raku.tab.c"
    break;

  case 332: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1430 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5246 "raku.tab.c"
    break;

  case 333: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1431 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5252 "raku.tab.c"
    break;

  case 334: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1432 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5258 "raku.tab.c"
    break;

  case 335: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1433 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5264 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1434 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5270 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1436 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5280 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1442 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5290 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1448 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5300 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr  */
#line 1453 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5306 "raku.tab.c"
    break;

  case 341: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1456 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5312 "raku.tab.c"
    break;

  case 342: /* divis_expr: jct_expr  */
#line 1457 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5318 "raku.tab.c"
    break;

  case 343: /* jct_expr: jct_expr '|' range_expr  */
#line 1460 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5324 "raku.tab.c"
    break;

  case 344: /* jct_expr: jct_expr '&' range_expr  */
#line 1461 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5330 "raku.tab.c"
    break;

  case 345: /* jct_expr: dor_expr  */
#line 1462 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5336 "raku.tab.c"
    break;

  case 346: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1466 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5342 "raku.tab.c"
    break;

  case 347: /* dor_expr: range_expr  */
#line 1467 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5348 "raku.tab.c"
    break;

  case 348: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1470 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5354 "raku.tab.c"
    break;

  case 349: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1471 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5360 "raku.tab.c"
    break;

  case 350: /* range_expr: add_expr  */
#line 1472 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5366 "raku.tab.c"
    break;

  case 351: /* add_expr: add_expr '~' repl_expr  */
#line 1475 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5372 "raku.tab.c"
    break;

  case 352: /* add_expr: repl_expr  */
#line 1476 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5378 "raku.tab.c"
    break;

  case 353: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1479 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5384 "raku.tab.c"
    break;

  case 354: /* repl_expr: addsub_expr  */
#line 1480 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5390 "raku.tab.c"
    break;

  case 355: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1483 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5396 "raku.tab.c"
    break;

  case 356: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1484 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5402 "raku.tab.c"
    break;

  case 357: /* addsub_expr: mul_expr  */
#line 1485 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5408 "raku.tab.c"
    break;

  case 358: /* mul_expr: mul_expr '*' unary_expr  */
#line 1488 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5414 "raku.tab.c"
    break;

  case 359: /* mul_expr: mul_expr '/' unary_expr  */
#line 1489 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5420 "raku.tab.c"
    break;

  case 360: /* mul_expr: mul_expr '%' unary_expr  */
#line 1490 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5426 "raku.tab.c"
    break;

  case 361: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1491 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5432 "raku.tab.c"
    break;

  case 362: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1493 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5438 "raku.tab.c"
    break;

  case 363: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1495 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5444 "raku.tab.c"
    break;

  case 364: /* mul_expr: unary_expr  */
#line 1496 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5450 "raku.tab.c"
    break;

  case 365: /* unary_expr: '-' unary_expr  */
#line 1499 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5456 "raku.tab.c"
    break;

  case 366: /* unary_expr: '!' unary_expr  */
#line 1500 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5462 "raku.tab.c"
    break;

  case 367: /* unary_expr: CARET unary_expr  */
#line 1501 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5468 "raku.tab.c"
    break;

  case 368: /* unary_expr: OP_REDUCE unary_expr  */
#line 1503 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5477 "raku.tab.c"
    break;

  case 369: /* unary_expr: pow_expr  */
#line 1507 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5483 "raku.tab.c"
    break;

  case 370: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1510 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5489 "raku.tab.c"
    break;

  case 371: /* pow_expr: postfix_expr  */
#line 1511 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5495 "raku.tab.c"
    break;

  case 372: /* scalar_list: VAR_SCALAR  */
#line 1514 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5501 "raku.tab.c"
    break;

  case 373: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1515 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5507 "raku.tab.c"
    break;

  case 374: /* meth_name: IDENT  */
#line 1518 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5513 "raku.tab.c"
    break;

  case 375: /* meth_name: KW_SORT  */
#line 1519 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5519 "raku.tab.c"
    break;

  case 376: /* meth_name: KW_MAP  */
#line 1520 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5525 "raku.tab.c"
    break;

  case 377: /* meth_name: KW_GREP  */
#line 1521 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5531 "raku.tab.c"
    break;

  case 378: /* meth_name: KW_SAY  */
#line 1522 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5537 "raku.tab.c"
    break;

  case 379: /* meth_name: KW_PRINT  */
#line 1523 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5543 "raku.tab.c"
    break;

  case 380: /* meth_name: KW_TAKE  */
#line 1524 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5549 "raku.tab.c"
    break;

  case 381: /* meth_name: KW_RETURN  */
#line 1525 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5555 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_EXISTS  */
#line 1526 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5561 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_DELETE  */
#line 1527 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5567 "raku.tab.c"
    break;

  case 384: /* meth_name: TESTOP  */
#line 1528 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5573 "raku.tab.c"
    break;

  case 385: /* postfix_expr: call_expr  */
#line 1530 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5579 "raku.tab.c"
    break;

  case 386: /* call_expr: IDENT '(' arg_list ')'  */
#line 1533 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5588 "raku.tab.c"
    break;

  case 387: /* call_expr: IDENT '(' ')'  */
#line 1537 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5594 "raku.tab.c"
    break;

  case 388: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1539 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5600 "raku.tab.c"
    break;

  case 389: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1541 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5606 "raku.tab.c"
    break;

  case 390: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1543 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5615 "raku.tab.c"
    break;

  case 391: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1548 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5621 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5631 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1556 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5641 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5649 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '.' KW_NEW  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5657 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '.' IDENT  */
#line 1570 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5666 "raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1575 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5677 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1582 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5686 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' CARET IDENT  */
#line 1587 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5696 "raku.tab.c"
    break;

  case 400: /* call_expr: atom '.' CARET IDENT  */
#line 1593 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5706 "raku.tab.c"
    break;

  case 401: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1599 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5717 "raku.tab.c"
    break;

  case 402: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1606 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5728 "raku.tab.c"
    break;

  case 403: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1613 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5737 "raku.tab.c"
    break;

  case 404: /* call_expr: atom '.' meth_name  */
#line 1618 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5746 "raku.tab.c"
    break;

  case 405: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1623 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5757 "raku.tab.c"
    break;

  case 406: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1630 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5766 "raku.tab.c"
    break;

  case 407: /* call_expr: call_expr '.' meth_name  */
#line 1635 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5775 "raku.tab.c"
    break;

  case 408: /* call_expr: KW_DIE expr  */
#line 1640 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5781 "raku.tab.c"
    break;

  case 409: /* call_expr: KW_MAP closure expr  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5787 "raku.tab.c"
    break;

  case 410: /* call_expr: KW_GREP closure expr  */
#line 1644 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5793 "raku.tab.c"
    break;

  case 411: /* call_expr: KW_SORT expr  */
#line 1646 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5799 "raku.tab.c"
    break;

  case 412: /* call_expr: KW_SORT closure expr  */
#line 1648 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5805 "raku.tab.c"
    break;

  case 413: /* call_expr: atom  */
#line 1649 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5811 "raku.tab.c"
    break;

  case 414: /* arg_list: expr  */
#line 1652 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5817 "raku.tab.c"
    break;

  case 415: /* arg_list: arg_list ',' expr  */
#line 1653 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5823 "raku.tab.c"
    break;

  case 416: /* arg_list: arg_list ','  */
#line 1654 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5829 "raku.tab.c"
    break;

  case 417: /* atom: LIT_INT  */
#line 1657 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5835 "raku.tab.c"
    break;

  case 418: /* atom: LIT_FLOAT  */
#line 1658 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5841 "raku.tab.c"
    break;

  case 419: /* atom: LIT_STR  */
#line 1659 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5847 "raku.tab.c"
    break;

  case 420: /* atom: WORDLIST  */
#line 1661 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5860 "raku.tab.c"
    break;

  case 421: /* atom: LIT_INTERP_STR  */
#line 1669 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5866 "raku.tab.c"
    break;

  case 422: /* atom: VAR_SCALAR  */
#line 1670 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5872 "raku.tab.c"
    break;

  case 423: /* atom: OP_INC VAR_SCALAR  */
#line 1671 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5878 "raku.tab.c"
    break;

  case 424: /* atom: OP_DEC VAR_SCALAR  */
#line 1672 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5884 "raku.tab.c"
    break;

  case 425: /* atom: VAR_SCALAR OP_INC  */
#line 1673 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5890 "raku.tab.c"
    break;

  case 426: /* atom: VAR_SCALAR OP_DEC  */
#line 1674 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5896 "raku.tab.c"
    break;

  case 427: /* atom: VAR_ARRAY  */
#line 1675 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5902 "raku.tab.c"
    break;

  case 428: /* atom: VAR_HASH  */
#line 1676 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5908 "raku.tab.c"
    break;

  case 429: /* atom: VAR_CAPTURE  */
#line 1678 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5916 "raku.tab.c"
    break;

  case 430: /* atom: VAR_FH  */
#line 1682 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5924 "raku.tab.c"
    break;

  case 431: /* atom: VAR_NAMED_CAPTURE  */
#line 1686 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5931 "raku.tab.c"
    break;

  case 432: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1689 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5937 "raku.tab.c"
    break;

  case 433: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1691 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5943 "raku.tab.c"
    break;

  case 434: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1693 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5949 "raku.tab.c"
    break;

  case 435: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1695 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5955 "raku.tab.c"
    break;

  case 436: /* atom: ARR_ALL_SLICE  */
#line 1697 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5961 "raku.tab.c"
    break;

  case 437: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1699 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5967 "raku.tab.c"
    break;

  case 438: /* atom: VAR_HASH '{' expr '}'  */
#line 1701 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5973 "raku.tab.c"
    break;

  case 439: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1703 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5979 "raku.tab.c"
    break;

  case 440: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1705 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5985 "raku.tab.c"
    break;

  case 441: /* atom: IDENT  */
#line 1706 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5991 "raku.tab.c"
    break;

  case 442: /* atom: VAR_TWIGIL  */
#line 1708 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5999 "raku.tab.c"
    break;

  case 443: /* atom: VAR_ARRAY_TWIGIL  */
#line 1712 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6007 "raku.tab.c"
    break;

  case 444: /* atom: VAR_HASH_TWIGIL  */
#line 1716 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6015 "raku.tab.c"
    break;

  case 445: /* atom: '(' ')'  */
#line 1719 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6021 "raku.tab.c"
    break;

  case 446: /* atom: '(' expr ')'  */
#line 1720 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6027 "raku.tab.c"
    break;

  case 447: /* atom: '(' expr ',' ')'  */
#line 1722 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6033 "raku.tab.c"
    break;

  case 448: /* atom: '(' expr ',' arg_list ')'  */
#line 1724 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6040 "raku.tab.c"
    break;

  case 449: /* atom: block  */
#line 1726 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6046 "raku.tab.c"
    break;

  case 450: /* atom: KW_SUB block  */
#line 1727 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6052 "raku.tab.c"
    break;


#line 6056 "raku.tab.c"

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

#line 1729 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
