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
#define YYLAST   4183

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  136
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  448
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1131

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
     422,   424,   426,   430,   433,   437,   439,   443,   446,   450,
     452,   454,   456,   458,   460,   464,   468,   470,   472,   474,
     476,   478,   480,   482,   484,   486,   488,   490,   492,   494,
     496,   499,   502,   504,   507,   510,   512,   514,   516,   519,
     522,   524,   529,   533,   537,   542,   546,   547,   549,   551,
     553,   555,   558,   560,   562,   564,   567,   570,   573,   576,
     579,   581,   583,   585,   587,   590,   592,   594,   596,   599,
     602,   606,   609,   611,   613,   615,   618,   621,   624,   627,
     630,   633,   636,   637,   638,   639,   640,   641,   642,   644,
     646,   648,   650,   651,   652,   653,   654,   655,   656,   658,
     660,   662,   664,   665,   666,   667,   670,   672,   674,   676,
     678,   680,   682,   684,   688,   690,   692,   694,   696,   698,
     702,   704,   708,   710,   712,   714,   718,   720,   724,   726,
     728,   732,   734,   736,   740,   743,   749,   755,   760,   766,
     771,   776,   784,   795,   796,   802,   810,   816,   822,   830,
     836,   842,   851,   858,   867,   876,   885,   892,   899,   900,
     903,   906,   908,   910,   912,   914,   916,   918,   920,   923,
     925,   927,   929,   932,   935,   941,   942,   943,   946,   949,
     951,   953,   955,   957,   959,   961,   963,   966,   968,   970,
     972,   975,   978,   984,  1000,  1015,  1016,  1028,  1042,  1043,
    1046,  1049,  1052,  1055,  1058,  1061,  1064,  1067,  1070,  1073,
    1076,  1079,  1086,  1093,  1100,  1107,  1111,  1115,  1119,  1123,
    1130,  1137,  1144,  1151,  1161,  1169,  1177,  1186,  1193,  1200,
    1210,  1218,  1228,  1243,  1244,  1249,  1254,  1261,  1265,  1269,
    1273,  1279,  1281,  1283,  1285,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,  1303,
    1304,  1305,  1306,  1307,  1308,  1311,  1312,  1313,  1315,  1317,
    1319,  1321,  1323,  1326,  1328,  1330,  1332,  1334,  1337,  1340,
    1344,  1347,  1350,  1353,  1355,  1359,  1362,  1365,  1368,  1371,
    1374,  1376,  1378,  1380,  1382,  1384,  1386,  1388,  1390,  1394,
    1397,  1398,  1399,  1400,  1401,  1402,  1403,  1404,  1409,  1412,
    1414,  1417,  1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1437,  1443,  1449,  1452,
    1453,  1456,  1457,  1458,  1461,  1463,  1466,  1467,  1468,  1471,
    1472,  1475,  1476,  1479,  1480,  1481,  1484,  1485,  1486,  1487,
    1488,  1490,  1492,  1495,  1496,  1497,  1498,  1503,  1506,  1507,
    1510,  1511,  1514,  1515,  1516,  1517,  1518,  1519,  1520,  1521,
    1522,  1523,  1524,  1526,  1528,  1533,  1534,  1536,  1538,  1543,
    1545,  1551,  1557,  1561,  1565,  1570,  1577,  1582,  1588,  1594,
    1601,  1608,  1613,  1618,  1625,  1630,  1635,  1637,  1639,  1641,
    1643,  1645,  1648,  1649,  1650,  1653,  1654,  1655,  1656,  1665,
    1666,  1667,  1668,  1669,  1670,  1671,  1672,  1673,  1677,  1681,
    1684,  1686,  1688,  1690,  1692,  1694,  1696,  1698,  1700,  1702,
    1703,  1707,  1711,  1715,  1716,  1717,  1719,  1722,  1723
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

#define YYPACT_NINF (-823)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-448)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -823,    45,  1951,  -823,  -823,  -823,  -823,  -823,   532,   -59,
     -55,   -26,   400,  -823,  -823,  4004,  -823,  -823,  -823,    98,
    2026,   114,  3107,  3182,  3244,  3319,  3381,     4,   -10,  3381,
    2088,     0,  3381,   131,   135,  3456,  3518,   -10,    40,    21,
     110,    50,    50,  3593,   -10,   -10,  3381,   193,   203,   200,
    4004,  -823,  -823,   227,   266,   302,  4004,  4004,  -823,  2163,
     211,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,
    -823,   613,  -823,  -823,  -823,  4057,   756,  -823,   749,   596,
     -50,   236,  -823,   249,   237,   -21,   910,  -823,  -823,   287,
     253,   327,   415,  -823,  -823,  3381,  3381,  3381,  3381,  3381,
    3381,   447,  2225,  3655,  2301,   459,  3381,  3381,   334,   371,
     205,   355,   -54,  -823,   -10,  -823,  -823,   384,   870,   -39,
    -823,  2363,  -823,   240,   222,   233,   283,   884,   518,     2,
     491,  2163,   776,  2163,   406,  2163,   -10,  2163,   -10,    76,
     335,   290,  -823,  -823,   430,  -823,   192,   446,   456,   468,
      90,   106,  2163,   -10,  2163,   -10,    59,  2494,  -823,  3381,
    3381,  -823,  3381,  3381,  -823,  3381,  3381,  3381,  2556,  3381,
    -823,   524,  -823,  -823,  -823,   477,   510,    29,  -823,   513,
    -823,  -823,  -823,  -823,  -823,   413,   481,  1361,  3381,  3381,
    3381,  3381,  3381,  3381,  3381,  3381,  -823,  3381,  3381,  3381,
    3381,  3381,  3381,  3381,  3381,  -823,  4004,  4004,  4004,  4004,
    4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,
    4004,   890,  4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,
    4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,  4004,   953,
     909,   460,   523,   549,   574,   579,   584,   601,    66,  -823,
     538,  2163,   267,   677,   157,   543,   605,   638,   660,   779,
     701,   183,  -823,   800,   705,   717,  2301,   845,  3381,  -823,
    3381,  3381,  3381,  3381,  3381,  3381,  3381,   743,   413,   737,
    -823,  3381,  3381,  -823,  3730,  -823,  3792,  -823,   303,   318,
     339,   300,   759,   763,  -823,   818,   823,  3381,  3381,  3381,
    3381,  3381,  3381,  3381,  -823,  3381,   825,  3381,  3381,  -823,
    3381,   832,   938,   850,  -823,   869,  3381,  -823,  4004,  4004,
      15,  -823,  -823,  -823,  3381,  3381,  -823,  3381,  3381,  -823,
     871,  3381,   887,  3381,   855,   880,   859,  -823,  3381,  3381,
     902,   794,   798,   802,   805,   811,   806,  -823,  -823,  -823,
     -10,    18,  -823,    41,   817,   821,  -823,  -823,  2631,  -823,
     923,   132,   219,   843,  3107,  3182,  3381,  1688,     5,    47,
    -823,   479,   849,   854,   872,   883,   908,   918,   931,   940,
     971,   977,   982,  1023,  1032,  1033,  1034,  1035,   888,   888,
     888,   888,   888,   888,   920,   888,   888,   888,   888,   888,
     888,   888,   888,  -823,  -823,  -823,   -50,  -823,  -823,  -823,
    1044,  1044,   237,   -21,   910,   910,  -823,  -823,  -823,  -823,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,
    -823,  -823,  -823,  -823,   922,  1150,  1039,  -823,  2693,  -823,
    -823,  -823,  -823,  -823,  -823,  3381,  2769,  -823,   861,  3381,
    -823,  3381,  3381,  3381,  3381,   924,  1055,  1056,  -823,  2831,
    -823,   740,  3381,  1042,  -823,    -5,  -823,  1235,   647,  1057,
    1038,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  1047,
    -823,  1048,  2163,   297,  1099,   204,  2907,   386,  1050,  3381,
    -823,  3867,  -823,  3381,  -823,    48,  -823,  3381,  3381,  1065,
    1169,  2631,  1053,  1054,  1058,  1059,  1060,  1061,  1062,   444,
    2631,  1063,  1064,   496,   -10,   -15,  3929,  -823,   -10,   -10,
     158,   -63,   -44,  1069,  -823,   149,  -823,  -823,  -823,  1051,
     863,  1487,  1066,  1067,  1068,  1070,    46,  1076,  1049,  1080,
    1072,   -10,   -10,   -10,  1071,  1075,  1086,  3381,  -823,  -823,
    -823,  -823,  -823,  -823,  1189,  1190,  -823,   -18,  1051,   876,
      55,   483,    35,  -823,   879,  1193,  2301,  3381,  3381,   495,
     312,   102,  -823,   187,  3381,  3381,  -823,  3381,  3381,  -823,
    3381,  3381,  3381,  3381,  3381,  3381,  3381,  3381,  -823,  -823,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,
    -823,  -823,  -823,  -823,  -823,  4004,  2969,  -823,  1102,  1081,
     899,  1083,  -823,   901,  2631,  1084,   600,  1082,  1085,   772,
    3381,  3381,  3381,  -823,   914,  -823,   939,   941,  -823,  3381,
    1135,  1199,   943,  -823,  -823,  -823,  -823,  -823,   945,  3381,
    -823,  3381,  3381,  3381,   947,  -823,    32,  -823,  1088,  2163,
     611,  1091,  1051,   949,  1092,  1093,  3381,  -823,   959,  -823,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,   961,  -823,  -823,
    -823,  1012,  -823,  -823,  2163,   -10,  -823,  -823,  1211,  -823,
    1212,  1214,  3381,  1113,  1215,  1217,  -823,  1051,   530,  3107,
    3182,  1750,  -823,   635,  -823,  -823,  -823,  -823,  3381,   -10,
    -823,  -823,  -823,  1101,  1105,  1202,  -823,  -823,  -823,  -823,
    3381,  1106,  -823,  -823,    -7,   886,   874,  1178,  -823,  -823,
    1051,  1051,   963,   965,  1222,  1228,  1230,  -823,  -823,   165,
     792,  1122,   216,  3381,  3381,  3381,  3381,  3381,  3381,  -823,
    -823,  -823,  -823,   239,   271,   273,   329,   330,   340,   341,
     344,   345,   354,   356,   403,  -823,  -823,   967,  -823,   973,
     994,  -823,  1130,  -823,  -823,   996,  -823,  -823,  -823,  -823,
    -823,  1131,  1132,  1133,  -823,  -823,  -823,  1134,  3381,  1136,
    -823,  2631,  1138,   636,  -823,  -823,  1141,  1184,  1185,  -823,
     998,  -823,  3381,  -823,  -823,  1051,  -823,  -823,   637,  1142,
    1143,   -15,  -823,  1000,  1104,   -10,   -10,   -10,  -823,  3381,
    -823,  -823,  -823,  1158,  -823,   215,  -823,  -823,  -823,   564,
     314,  -823,   383,  3381,  3381,  3381,  3381,  3381,  3381,  3381,
    3381,  -823,   -10,  1139,  -823,  -823,   -10,  1145,  3381,  -823,
     349,   370,    49,   226,   847,  1147,  1148,   953,  -823,  -823,
    1051,  1051,  1270,  1271,  1272,  3381,  3045,  1167,  1168,  -823,
     419,   424,   427,   436,   464,   465,  -823,  -823,  -823,  -823,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,
    -823,  -823,  1155,  -823,  -823,  -823,  -823,  -823,  3381,  1006,
    -823,  -823,  -823,  3381,  3381,  2631,   648,  -823,  -823,  3381,
    -823,  -823,  -823,  -823,   -10,   -10,  -823,  -823,  -823,  -823,
    -823,  3381,  1170,  1277,  1278,  3381,  3381,  3381,  -823,  -823,
    -823,   478,   482,   487,   490,   517,   521,   535,   536,  -823,
    -823,  -823,  3381,  1161,  -823,    75,  1219,  -823,   161,  -823,
    1279,  1280,  1165,  3381,  -823,  1281,  1282,  1173,  3381,  -823,
     245,   288,  1174,  1175,  -823,  -823,   374,  -823,  -823,  -823,
    -823,  -823,   539,  1163,  1008,  3381,  3381,  -823,  -823,  -823,
    -823,  -823,  -823,  -823,  1176,  1180,  -823,  -823,  1010,  -823,
     750,  1114,  -823,  -823,  3381,  -823,  -823,   563,   565,   568,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  1181,   -10,
    1177,  1015,  1179,  1613,  1177,  1017,  1182,  1187,  -823,  1188,
    1191,  1192,  -823,  1194,  1289,  1297,  1195,  3381,  -823,  1298,
    1302,  1196,  3381,  -823,  -823,  -823,   251,  -823,  -823,  -823,
    1183,   585,   589,  -823,  -823,  1197,  -823,   -10,  -823,  -823,
    -823,  -823,  -823,  3381,  -823,  -823,  1177,  -823,  3107,  3182,
    1825,  -823,   674,  -823,  1177,  -823,  -823,  -823,  -823,  -823,
    -823,  1198,  1200,  -823,  1201,  1203,  1204,  -823,  1205,  1177,
    1019,  -823,  -823,  -823,  -823,  -823,  1206,  -823,   744,   328,
    -823,   393,  3381,  3381,  3381,  3381,  3381,  3381,  3381,  3381,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  1177,
     -10,  3381,  3381,  3381,  -823,  -823,  -823,   595,   634,   683,
     684,   686,   710,   716,   718,  -823,  -823,   721,   722,   724,
    -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,  -823,
    -823
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   415,   416,   417,   419,   420,   425,
     426,   440,   439,   441,   442,     0,   427,   428,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   418,     0,     0,     0,     0,     0,    93,     0,
       3,     4,    94,    95,   102,   103,   104,   105,    96,    97,
     112,     0,   113,   114,   115,   101,     0,   318,   320,   338,
     340,   343,   345,   348,   350,   352,   355,   362,   367,   369,
     383,   411,     0,   423,   424,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     420,   425,   426,   440,     0,   447,   365,     0,   420,   425,
      35,     0,   412,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,     0,   448,   317,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,     0,   141,     0,
       0,   106,     0,     0,   107,     0,     0,     0,     3,     0,
     409,    98,   100,   406,   205,     0,     0,     0,   366,     0,
     421,   422,   364,   363,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   312,   313,   314,   315,   316,   310,     0,   389,
       0,     0,   311,     0,     0,     0,     0,     0,   394,     0,
     393,   439,   385,     0,     0,     0,     0,     0,     0,    30,
       0,     0,     0,     0,     0,     0,     0,   443,     0,     0,
      38,   414,     0,     6,     0,     9,     0,    10,     0,     0,
       0,     0,     0,     0,   370,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    42,
       0,     0,   119,     0,   131,     0,     0,   150,     0,     0,
       0,     3,   157,    45,     0,     0,    46,     0,     0,   153,
       0,     0,     0,     0,     0,   134,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,   408,   410,
       0,     0,   208,     0,     0,     0,   243,   444,     0,   276,
     420,   425,   426,   440,     0,     0,     0,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   324,
     327,   328,   321,   322,     0,   325,   326,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   339,   341,   342,   344,
     346,   347,   349,   351,   353,   354,   359,   360,   361,   356,
     357,   358,   368,   372,   382,   376,   377,   378,   379,   380,
     381,   374,   375,   373,   405,     0,   402,    53,     0,    87,
      88,    89,    90,    91,    50,     0,     0,   388,     0,     0,
      15,     0,     0,     0,     0,   430,   435,   436,    55,     0,
     397,     0,     0,     0,   386,     0,   384,   414,     0,     0,
       0,   312,   313,   314,   315,   316,   310,   311,    37,     0,
     413,     0,     0,     0,   417,   439,     0,     0,     0,     0,
      27,     0,    28,     0,    29,     0,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,   122,   444,     0,
       0,   346,   347,   255,   256,     0,   269,   271,   273,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,     0,   444,     0,     0,     0,     0,   108,   109,
     110,   111,   309,    99,     0,     0,   208,     0,     0,     0,
       0,     0,     0,   445,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,     0,     0,   303,     0,     0,   304,
       0,     0,     0,     0,     0,     0,     0,     0,   277,    57,
      59,    61,    64,    62,    63,    58,    60,    70,    72,    74,
      77,    75,    76,    71,    73,     0,     0,   398,     0,     0,
       0,     0,   167,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,   392,     0,     0,   247,     0,
       0,     0,     0,   430,   435,   436,    36,     5,     0,     0,
      11,     0,     0,     0,     0,    20,     0,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,    78,
      81,    80,    84,    82,    83,    79,    40,     0,    85,    86,
      43,   116,   121,   120,     0,     0,   130,   147,     0,   149,
       0,     0,     0,   258,     0,     0,   156,     0,     0,     0,
       0,     0,   168,     0,    48,    49,    32,    31,     0,     0,
     151,   437,   438,     0,     0,   132,   135,   136,   139,   140,
       0,     0,   206,   207,     0,     0,     0,     0,   204,   165,
       0,     0,     0,     0,     0,     0,     0,   242,   446,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   286,
     293,   300,   301,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,   404,     0,   401,     0,
       0,    52,     0,    54,   166,     0,    17,    16,   433,   432,
     431,     0,     0,     0,   395,   390,   391,     0,     0,     0,
     387,     0,     0,     0,   252,   251,     0,     0,     0,    22,
       0,    23,     0,    26,   159,     0,    34,    33,     0,   446,
     446,     0,   123,     0,   124,     0,     0,     0,   265,     0,
     259,   260,   155,   261,   257,     0,   270,   272,   274,     0,
       0,   171,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,    68,    69,     0,     0,     0,   203,
       0,     0,     0,     0,     0,     0,     0,     0,   164,   162,
       0,     0,     0,     0,     0,     0,     0,   430,   436,   297,
       0,     0,     0,     0,     0,     0,   305,   306,   307,   308,
     278,   280,   282,   285,   283,   284,   279,   281,   403,   400,
     399,    51,   446,    65,    66,    67,   248,   249,     0,     0,
      13,    12,    21,     0,     0,     0,     0,   158,     7,     0,
      41,    44,   118,   117,   444,     0,   128,   148,   145,   146,
     267,     0,   262,     0,     0,     0,     0,     0,   172,   173,
     170,     0,     0,     0,     0,     0,     0,     0,     0,   154,
     152,   133,     0,     0,   144,     0,     3,   238,     0,   235,
       0,     0,     0,     0,   214,     0,     0,     0,     0,   209,
       0,     0,     0,     0,   210,   211,     0,   161,   163,   244,
     245,   246,     0,   167,     0,     0,     0,   287,   289,   292,
     290,   291,   288,    18,     0,   446,   254,   253,     0,    24,
       0,   125,   126,   266,     0,   263,   264,     0,     0,     0,
     174,   176,   178,   181,   179,   180,   175,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   225,     0,
       0,     0,   226,     0,     0,     0,     0,     0,   216,     0,
       0,     0,     0,   215,   212,   213,     0,   241,   296,   295,
     166,     0,     0,   250,    14,   446,     8,     0,   129,   268,
     182,   184,   183,     0,   143,   237,     0,   186,     0,     0,
       0,   185,     0,   234,     0,   223,   224,   218,   221,   222,
     217,     0,     0,   227,     0,     0,     0,   228,     0,     0,
       0,   294,   298,   299,    25,   127,     0,   236,     0,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,   233,   231,   232,   220,   229,   230,   219,   240,     0,
       0,     0,     0,     0,   190,   191,   188,     0,     0,     0,
       0,     0,     0,     0,     0,   239,   142,     0,     0,     0,
     192,   194,   196,   199,   197,   198,   193,   195,   200,   202,
     201
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -823,  -823,     3,  -823,  -509,  -666,  -823,  -823,  -823,  -823,
    -823,   277,  -823,  -823,  -823,  -823,  -823,  -283,  -822,  -823,
    -823,  -823,   770,  -823,  -823,  -454,   851,  -349,   206,  1112,
      -2,  -210,  -823,   846,  1107,  -823,   693,   -11,  1108,  1109,
     927,    20,  -823,  -823,  -239,  -823,  -823,   101,  -823
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   187,    61,    62,   517,    63,    64,    65,    66,
      67,   933,    68,    69,   536,    70,    71,   322,   937,    72,
      73,   351,   557,    74,   562,   264,   487,   530,   115,   166,
     122,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   295,   434,    89,    90,   123,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,   436,   394,     2,   559,   802,   672,   626,   496,   630,
     147,    24,   292,   632,   148,   140,   293,   680,   141,   939,
     132,   134,   136,   138,   139,   523,   524,   144,   146,   525,
     149,   574,   554,   153,   155,   116,   681,   787,   715,   716,
     555,   170,   717,   354,   173,     3,   788,   159,   575,   715,
     716,   523,   524,   717,   103,   525,   228,   185,   523,   524,
     105,   267,   525,   940,   160,   523,   524,   223,   224,   525,
     178,   941,   104,   577,   276,   228,   182,   183,   106,   268,
     526,   527,   528,   698,   699,   523,   524,   107,   338,   525,
     578,   355,   266,   242,   243,   244,   245,   246,   247,   230,
     231,   252,   254,   339,   256,   257,   526,   527,   528,   724,
     725,   726,   117,   526,   527,   528,   718,   942,    60,   278,
     526,   527,   528,    60,   124,   125,   126,   839,   127,   296,
     631,   306,   161,   311,  1027,   313,   162,    60,   906,   576,
     526,   527,   528,   150,   529,   128,   653,   151,   161,   129,
     334,   556,   336,   163,   759,   341,   315,   342,   343,   683,
     344,   345,   943,   346,   347,   348,   346,   349,   157,   727,
     558,   523,   524,    60,   164,   525,   944,   652,  1045,   445,
     700,   579,  1053,   165,   721,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   446,   380,   381,   382,   383,   384,
     385,   386,   387,   250,  1000,   330,   316,   174,    75,    60,
     265,   722,   723,   324,   176,   410,   411,   175,   324,   684,
     685,   332,   279,   331,  1077,   912,   526,   527,   528,   323,
     325,   177,  1091,   142,   143,   325,   741,   164,   678,   333,
     945,   179,   130,   156,   158,   103,   686,  1098,   946,   448,
     171,   172,   416,   417,   418,   419,   420,   421,   422,  1014,
     462,   523,   524,   566,   468,   525,   470,  1015,   471,   472,
     473,   474,   475,   476,   477,   719,   180,  1115,   855,   480,
     481,   643,   483,   186,   488,   913,   914,   454,   281,   455,
    1004,    60,   902,   856,   947,   502,   503,   504,   505,   506,
     507,   508,  1019,    93,    94,   511,   512,   521,   522,   108,
    1020,   109,   181,  1016,   326,  1038,   526,   527,   528,   326,
     142,   742,   532,   533,   531,   534,   535,   226,   227,   538,
     108,   540,   109,   102,   105,   282,   544,   545,   307,   948,
     307,   225,   312,   458,   314,   317,   284,   229,   258,   283,
     859,   259,   567,   949,   307,   308,  1021,   308,  1017,   335,
     285,   337,   569,   570,   571,   573,   548,   280,   228,   794,
     281,   308,  1018,   866,     4,     5,     6,     7,   449,   239,
    1069,   118,   119,   112,   113,   261,    13,    14,    15,    16,
      17,    18,   260,    75,   450,   755,   286,   451,   549,   238,
     550,  1022,   114,    28,   812,   867,   509,   868,   639,   324,
     287,   513,    33,   318,   319,  1023,   489,   520,   320,   324,
      41,    42,    43,   321,   640,    46,   325,   641,   495,   241,
     490,   491,   307,   321,    50,    51,   325,   848,   849,   309,
      52,   309,   310,   611,   310,   492,   740,   615,   919,   308,
     617,   618,   493,   240,   228,   309,   551,   597,   310,   564,
     628,   248,  1105,   869,   870,   480,   494,   598,   599,    54,
      55,   600,   601,   255,   871,   872,   841,   935,   873,   874,
     638,   602,   936,   603,   185,    56,   266,   648,   875,   650,
     876,   651,    57,   523,   524,   654,   655,   525,   938,    59,
     262,   294,  1026,   936,    60,   580,   263,   936,   581,   582,
     326,   269,   897,   645,   675,   583,   646,   920,   584,   585,
     326,   733,   586,   587,   298,   734,   108,  1106,   109,   693,
     604,   735,   291,   309,   736,   737,   310,   877,   738,   610,
     813,   814,   357,   358,   815,   711,   659,   613,   526,   527,
     528,   661,   616,   967,   662,   619,   553,   323,   968,   327,
     624,   969,   627,   663,   730,   731,   732,   957,   958,   328,
     970,   666,   743,   744,   281,   745,   746,   350,   747,   748,
     749,   750,   751,   752,   753,   754,  1001,   437,   438,  1005,
     915,   664,   665,   298,   916,   816,   817,   818,   971,   972,
     300,   329,   658,   301,   302,   597,   205,   917,   956,   598,
     352,   667,   990,   588,   599,   359,   991,   600,   771,   772,
     773,   992,   304,   670,   993,   305,   281,   777,    92,   739,
      93,    94,    95,    96,    97,    98,    99,   782,   353,   188,
     784,   785,   189,   190,   601,   100,   356,   790,   602,   191,
     439,   994,   192,   193,   798,   995,   194,   195,   101,   456,
     102,   823,   603,   604,   824,   825,   763,   447,   281,   996,
     997,   826,   803,  1028,   827,   828,   440,  1070,   829,   830,
     808,   215,   216,   217,   218,   219,   220,   819,   820,   822,
     659,   304,   661,   221,   305,   665,   832,  1040,   918,  1041,
    1082,   441,  1042,  1083,  1084,   222,   442,   757,   837,   760,
    1085,   443,   883,  1086,  1087,   765,   885,  1088,  1089,  1072,
     671,   673,   597,  1073,   676,   677,   679,   767,   444,  1120,
     281,   860,   861,   862,   863,   864,   865,    75,   791,   457,
     196,   792,   783,     4,     5,     6,     7,   705,   706,   707,
     118,   119,   112,   113,   261,    13,    14,    15,    16,    17,
      18,   598,   205,   891,   898,   458,   281,   899,  1121,   831,
    1101,   114,    28,   298,  1102,   979,   887,   454,   281,   633,
     300,    33,   197,   301,   302,   198,   199,  1103,   459,    41,
      42,    43,   200,   460,    46,   201,   202,   452,   453,   203,
     204,   205,   297,    50,    51,   298,   299,   910,  1090,    52,
     599,   600,   300,   601,   463,   301,   302,  1122,  1123,   303,
    1124,   921,   922,   923,   924,   925,   926,   927,   928,   461,
     206,   207,   208,   209,   464,   465,   934,   602,    54,    55,
     210,   211,   212,   603,  1125,   604,   466,   467,   659,   661,
    1126,   665,  1127,   962,    56,  1128,  1129,   950,  1130,   469,
     951,    57,   952,   953,   213,   214,   479,   281,    59,   625,
     478,   304,   497,    60,   305,   263,   498,  1036,  1104,   519,
     281,   804,   889,   205,   842,   537,   974,   843,   844,   845,
     846,   976,   977,   896,   288,   289,   290,   403,   404,   405,
     423,   539,   281,   304,   770,   833,   305,   542,   424,   983,
     425,   426,   546,   987,   988,   989,   407,   408,   409,   427,
     428,   547,   454,   423,   857,   548,   435,   429,   430,   549,
     998,   424,   550,   425,   426,   431,   432,   433,   551,  1003,
     552,  1009,   427,   428,   840,   560,  1013,   499,   500,   561,
     429,   430,   357,   501,   357,   510,   568,   964,   431,   432,
     433,   514,   358,  1031,  1032,   515,   516,   423,    93,    94,
     270,   271,   272,   273,   274,   424,   589,   425,   426,   518,
     358,   590,  1039,   275,   541,   358,   427,   428,   543,   358,
     357,   614,   687,   688,   429,   430,   978,   222,   102,   591,
     980,  1052,   431,   432,   433,   720,   688,   903,   728,   281,
     592,   907,   908,   909,   605,  1064,   232,   233,   234,    92,
    1068,    93,    94,    95,    96,    97,    98,    99,   762,   281,
     764,   281,   235,   236,   237,   593,   100,   620,   929,   801,
     516,   934,   931,   774,   281,   594,  1078,  1079,  1081,   565,
     606,   102,   388,   389,   390,   391,   392,   393,   595,   395,
     396,   397,   398,   399,   400,   401,   402,   596,   775,   465,
     776,   281,   780,   465,   357,   781,   786,   646,   795,   688,
    1107,  1108,  1109,  1110,  1111,  1112,  1113,  1114,   799,   281,
     800,   281,   850,   688,   851,   688,   878,   281,   597,  1117,
    1118,  1119,   879,   465,   598,     4,     5,     6,     7,   599,
     981,   982,   118,   119,   112,   113,   261,    13,    14,    15,
      16,    17,    18,   880,   281,   882,   281,   357,   895,   904,
     358,   905,   516,   114,    28,   975,   281,  1030,   281,  1035,
     281,  1037,   516,    33,  1046,   688,  1054,   688,  1099,   688,
     600,    41,    42,    43,   167,   169,    46,   414,   415,   601,
     602,   603,   604,   228,   607,    50,    51,   608,   621,   622,
     629,    52,   635,   634,   636,   637,   642,   647,   656,   657,
     659,   660,   682,   702,   321,   661,   662,   663,   664,   665,
     668,   669,   701,   694,   695,   696,   703,   697,   708,   710,
      54,    55,   709,   712,   713,  1044,   704,   729,   761,    75,
     763,   766,   778,   779,   768,   789,    56,   769,   793,   796,
     797,   805,   806,    57,   807,   810,   809,   811,   834,   836,
      59,   758,   835,   838,   847,    60,   852,   263,     4,     5,
       6,     7,   853,  1075,   854,   118,   119,   112,   113,   261,
      13,    14,    15,    16,    17,    18,   858,   881,   883,   884,
     885,   893,   894,   886,   888,   890,   114,    28,   892,   900,
     901,   911,   932,   930,   954,   955,    33,   959,   960,   961,
     965,   966,   973,   984,    41,    42,    43,   985,   986,    46,
     999,  1002,  1008,  1006,  1007,  1010,  1011,  1029,    50,    51,
    1012,  1024,  1025,  1061,    52,  1033,  1116,  1034,  1043,  1055,
     936,  1062,  1065,  1047,  1056,  1057,  1066,  1071,  1058,  1059,
    1076,  1060,  1063,  1067,  1074,  1092,   714,  1093,  1094,   406,
    1095,  1096,  1097,    54,    55,  1100,   412,   644,   413,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,    59,     4,     5,     6,     7,    60,     0,
     263,   360,   361,   362,   363,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   364,   365,    24,     0,     0,
      25,    26,    27,    28,   366,   367,    31,    32,     0,     0,
       0,     0,    33,    34,    35,    36,    37,    38,   368,   369,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
      48,    49,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,    58,    59,
       4,     5,     6,     7,    60,   370,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,   689,   690,    24,     0,     0,    25,    26,    27,    28,
      29,   691,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,    58,    59,     4,     5,     6,     7,
      60,   692,     0,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,  1048,  1049,    24,
       0,     0,    25,    26,    27,    28,    29,  1050,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     0,
       0,     0,    48,    49,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,    53,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,    54,    55,     0,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
      58,    59,    46,     0,     0,     0,    60,  1051,     0,     0,
       0,    50,    51,     4,     5,     6,     7,    52,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,    54,    55,     0,     0,
       0,    33,     0,     0,     0,     0,     0,     0,     0,    41,
      42,    43,    56,     0,    46,     0,     0,     0,     0,    57,
       0,     0,     0,    50,    51,   145,    59,     0,     0,    52,
       0,    60,   572,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,    54,    55,
       0,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   145,    59,    46,
       0,     0,     0,    60,   821,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,   145,    59,     4,     5,     6,     7,    60,  1080,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     0,     0,
      25,    26,    27,    28,    29,    30,    31,    32,     0,     0,
       0,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     0,     0,     0,
      48,    49,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,    53,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,    58,    59,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     4,     5,     6,     7,    52,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,    54,    55,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      56,     0,    46,     0,     0,     0,     0,    57,     0,     0,
       0,    50,    51,   120,   121,     0,     0,    52,     0,    60,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,    54,    55,     0,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,   145,    59,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     4,     5,
       6,     7,    52,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,    54,    55,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    56,     0,    46,
       0,     0,     0,     0,    57,     0,     0,     0,    50,    51,
       0,    59,   184,     0,    52,     0,    60,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   249,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     4,     5,     6,     7,
      52,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,    54,
      55,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    56,     0,    46,     0,     0,
       0,     0,    57,   253,     0,     0,    50,    51,     0,    59,
       0,     0,    52,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,    59,   277,     0,     0,     0,    60,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    51,     4,
       5,     6,     7,    52,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,    54,    55,     0,     0,     0,    33,     0,     0,
       0,     0,     0,     0,     0,    41,    42,    43,    56,     0,
      46,     0,     0,     0,     0,    57,     0,     0,     0,    50,
      51,     0,    59,     0,     0,    52,     0,    60,   186,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     4,     5,     6,     7,
      52,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,    54,
      55,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,    56,     0,    46,     0,     0,
       0,     0,    57,     0,     0,     0,    50,    51,     0,    59,
     563,     0,    52,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,    54,    55,     0,     0,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   609,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     4,     5,     6,     7,    52,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,    54,    55,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    56,     0,    46,     0,     0,     0,     0,
      57,     0,     0,     0,    50,    51,     0,    59,   612,     0,
      52,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       4,     5,   484,     7,     0,     0,     0,   118,   119,   112,
     113,   485,    13,    14,    15,    16,    17,    18,     0,    54,
      55,     0,     0,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     623,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     4,     5,     6,     7,    52,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,    54,    55,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    56,     0,    46,     0,     0,     0,     0,    57,     0,
       0,     0,    50,    51,     0,    59,   184,     0,    52,     0,
      60,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,    54,    55,     0,
       0,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   756,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       4,     5,     6,     7,    52,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,    54,    55,     0,     0,     0,    33,     0,
       0,     0,     0,     0,     0,     0,    41,    42,    43,    56,
       0,    46,     0,     0,     0,     0,    57,     0,     0,     0,
      50,    51,     0,    59,   963,     0,    52,     0,    60,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,    54,    55,     0,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   131,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     4,     5,     6,
       7,    52,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
      54,    55,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,     0,    41,    42,    43,    56,     0,    46,     0,
       0,     0,     0,    57,     0,     0,     0,    50,    51,     0,
     133,     0,     0,    52,     0,    60,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,    54,    55,     0,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   135,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     4,     5,     6,     7,    52,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,    54,    55,     0,
       0,     0,    33,     0,     0,     0,     0,     0,     0,     0,
      41,    42,    43,    56,     0,    46,     0,     0,     0,     0,
      57,     0,     0,     0,    50,    51,     0,   137,     0,     0,
      52,     0,    60,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,    59,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     4,     5,     6,     7,    52,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,    54,    55,     0,     0,     0,    33,
       0,     0,     0,     0,     0,     0,     0,    41,    42,    43,
      56,     0,    46,     0,     0,     0,     0,    57,     0,     0,
       0,    50,    51,     0,   152,     0,     0,    52,     0,    60,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,    54,    55,     0,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   154,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     4,     5,
       6,     7,    52,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,    54,    55,     0,     0,     0,    33,     0,     0,     0,
       0,     0,     0,     0,    41,    42,    43,    56,     0,    46,
       0,     0,     0,     0,    57,     0,     0,     0,    50,    51,
       0,    59,     0,     0,    52,     0,   168,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   251,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     4,     5,   484,     7,    52,
       0,     0,   118,   119,   112,   113,   485,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,    54,    55,
       0,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,    41,    42,    43,    56,     0,    46,     0,     0,     0,
       0,    57,     0,     0,     0,    50,    51,     0,   482,     0,
       0,    52,     0,    60,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
     486,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     4,     5,     6,     7,    52,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,    54,    55,     0,     0,     0,
      33,     0,     0,     0,     0,     0,     0,     0,    41,    42,
      43,    56,     0,    46,     0,     0,     0,     0,    57,     0,
       0,     0,    50,    51,     0,   649,     0,     0,    52,     0,
      60,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   110,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,    54,    55,     0,
       0,     0,     0,     0,     0,   114,     0,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   674,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -447,  -447,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,    59,     0,     0,  -447,  -447,    60,  -447,  -447,
    -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,  -447,
    -447,     0,     0,     0,  -447,     0,     0,     0,     0,     0,
       0,     0,  -447,  -447,  -447,  -447,  -447,  -447,     0,  -447,
       0,     0,  -447,  -447,  -447,  -447,  -447,  -447,     0,  -447,
    -447,  -447,     0,  -447
};

static const yytype_int16 yycheck[] =
{
       2,   240,   212,     0,   353,   671,   515,   461,   291,    14,
      10,    26,    10,   467,    14,    26,    14,    80,    14,   841,
      22,    23,    24,    25,    26,    10,    11,    29,    30,    14,
      32,    26,    14,    35,    36,    15,    80,     5,    56,    57,
      22,    43,    60,    14,    46,     0,    14,    26,    43,    56,
      57,    10,    11,    60,   113,    14,   119,    59,    10,    11,
     115,   115,    14,    14,    43,    10,    11,   117,   118,    14,
      50,    22,   131,    26,   113,   119,    56,    57,   133,   133,
      65,    66,    67,    37,    38,    10,    11,   113,    29,    14,
      43,    62,   131,    95,    96,    97,    98,    99,   100,   120,
     121,   103,   104,    44,   106,   107,    65,    66,    67,    74,
      75,    76,    14,    65,    66,    67,   134,    68,   133,   121,
      65,    66,    67,   133,    10,    11,    12,   134,    14,   131,
     135,   133,   127,   135,   956,   137,    26,   133,   804,   134,
      65,    66,    67,    12,   129,    31,   495,    12,   127,    35,
     152,   133,   154,    43,   608,   157,    80,   159,   160,    10,
     162,   163,   113,   165,   166,   167,   168,   169,   128,   134,
     129,    10,    11,   133,   127,    14,   127,   129,  1000,   113,
     134,   134,  1004,   133,   129,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   128,   197,   198,   199,   200,   201,
     202,   203,   204,   102,   129,   115,   130,    14,     2,   133,
     109,   560,   561,    26,    14,   226,   227,    14,    26,    70,
      71,   115,   121,   133,  1046,    10,    65,    66,    67,   127,
      43,    31,  1054,    27,    28,    43,   134,   127,    80,   133,
      14,    14,   128,    37,    38,   113,   529,  1069,    22,   251,
      44,    45,   232,   233,   234,   235,   236,   237,   238,    14,
      77,    10,    11,   131,   266,    14,   268,    22,   270,   271,
     272,   273,   274,   275,   276,   558,    10,  1099,   113,   281,
     282,    77,   284,    72,   286,    70,    71,   130,   130,   132,
     129,   133,   801,   128,    68,   297,   298,   299,   300,   301,
     302,   303,    14,    98,    99,   307,   308,   318,   319,   126,
      22,   128,    10,    68,   127,   981,    65,    66,    67,   127,
     114,   134,   324,   325,   321,   327,   328,    78,    79,   331,
     126,   333,   128,   128,   115,   113,   338,   339,    26,   113,
      26,   105,   136,   127,   138,   139,   113,   110,    14,   127,
     134,    17,   133,   127,    26,    43,    68,    43,   113,   153,
     127,   155,   364,   365,   366,   367,   127,   127,   119,   652,
     130,    43,   127,   134,     3,     4,     5,     6,   111,   126,
     129,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    58,   187,   127,   605,   113,   130,   127,   112,
     127,   113,    31,    32,   687,   134,   305,   134,   111,    26,
     127,   310,    41,    78,    79,   127,   113,   316,   128,    26,
      49,    50,    51,   133,   127,    54,    43,   130,   128,    14,
     127,   113,    26,   133,    63,    64,    43,   720,   721,   127,
      69,   127,   130,   445,   130,   127,   134,   449,   134,    43,
     452,   453,   113,   126,   119,   127,   127,   127,   130,   358,
     462,    14,   134,   134,   134,   467,   127,   127,   127,    98,
      99,   127,   127,    14,   134,   134,   715,   128,   134,   134,
     482,   127,   133,   127,   486,   114,   131,   489,   134,   491,
     134,   493,   121,    10,    11,   497,   498,    14,   128,   128,
     129,    10,   128,   133,   133,    26,   135,   133,    29,    30,
     127,   127,   795,   127,   516,    36,   130,   134,    39,    40,
     127,    26,    43,    44,    29,    30,   126,   134,   128,   531,
     127,    36,    14,   127,    39,    40,   130,   134,    43,   438,
      10,    11,   129,   130,    14,   547,   127,   446,    65,    66,
      67,   127,   451,   134,   127,   454,   350,   127,   134,   113,
     459,   134,   461,   127,   566,   567,   568,   850,   851,   113,
     134,   127,   574,   575,   130,   577,   578,    53,   580,   581,
     582,   583,   584,   585,   586,   587,   935,   127,   128,   938,
      26,   127,   127,    29,    30,    65,    66,    67,   134,   134,
      36,   133,   501,    39,    40,   127,   127,    43,   847,   127,
     133,   510,   134,   134,   127,   134,   134,   127,   620,   621,
     622,   134,   127,   127,   134,   130,   130,   629,    96,   134,
      98,    99,   100,   101,   102,   103,   104,   639,   128,    26,
     642,   643,    29,    30,   127,   113,   133,   649,   127,    36,
     127,   134,    39,    40,   656,   134,    43,    44,   126,   116,
     128,    26,   127,   127,    29,    30,   127,   129,   130,   134,
     134,    36,   674,   134,    39,    40,   127,  1026,    43,    44,
     682,    85,    86,    87,    88,    89,    90,   689,   690,   691,
     127,   127,   127,    97,   130,   127,   698,   134,   134,   134,
      26,   127,   134,    29,    30,   109,   127,   606,   710,   608,
      36,   127,   127,    39,    40,   614,   127,    43,    44,   134,
     514,   515,   127,   134,   518,   519,   520,   127,   127,   134,
     130,   733,   734,   735,   736,   737,   738,   531,   127,   134,
     127,   130,   641,     3,     4,     5,     6,   541,   542,   543,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,   127,   127,   127,   127,   127,   130,   130,   134,   134,
      26,    31,    32,    29,    30,   127,   778,   130,   130,   132,
      36,    41,    26,    39,    40,    29,    30,    43,   128,    49,
      50,    51,    36,    14,    54,    39,    40,   120,   121,    43,
      44,   127,    26,    63,    64,    29,    30,   809,   134,    69,
     127,   127,    36,   127,    14,    39,    40,   134,   134,    43,
     134,   823,   824,   825,   826,   827,   828,   829,   830,   128,
      81,    82,    83,    84,   129,   130,   838,   127,    98,    99,
      91,    92,    93,   127,   134,   127,   129,   130,   127,   127,
     134,   127,   134,   855,   114,   134,   134,    10,   134,    14,
      13,   121,    15,    16,   115,   116,   129,   130,   128,   129,
     127,   127,   113,   133,   130,   135,   113,   127,   134,    10,
     130,   675,   781,   127,    10,    14,   888,    13,    14,    15,
      16,   893,   894,   792,    10,    11,    12,     7,     8,     9,
      14,    14,   130,   127,   132,   699,   130,    27,    22,   911,
      24,    25,    10,   915,   916,   917,   223,   224,   225,    33,
      34,   127,   130,    14,   132,   127,    17,    41,    42,   127,
     932,    22,   127,    24,    25,    49,    50,    51,   127,   936,
     134,   943,    33,    34,    58,   128,   948,   129,   130,   128,
      41,    42,   129,   130,   129,   130,   113,   856,    49,    50,
      51,   129,   130,   965,   966,    27,    28,    14,    98,    99,
     100,   101,   102,   103,   104,    22,   127,    24,    25,   129,
     130,   127,   984,   113,   129,   130,    33,    34,   129,   130,
     129,   130,   129,   130,    41,    42,   895,   109,   128,   127,
     899,  1003,    49,    50,    51,   129,   130,   801,   129,   130,
     127,   805,   806,   807,    94,  1017,   106,   107,   108,    96,
    1022,    98,    99,   100,   101,   102,   103,   104,   129,   130,
     129,   130,   122,   123,   124,   127,   113,   113,   832,    27,
      28,  1043,   836,   129,   130,   127,  1048,  1049,  1050,   126,
     128,   128,   206,   207,   208,   209,   210,   211,   127,   213,
     214,   215,   216,   217,   218,   219,   220,   127,   129,   130,
     129,   130,   129,   130,   129,   130,   129,   130,   129,   130,
    1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,   129,   130,
     129,   130,   129,   130,   129,   130,   129,   130,   127,  1101,
    1102,  1103,   129,   130,   127,     3,     4,     5,     6,   127,
     904,   905,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   129,   130,   129,   130,   129,   130,   129,
     130,    27,    28,    31,    32,   129,   130,   129,   130,   129,
     130,    27,    28,    41,   129,   130,   129,   130,   129,   130,
     127,    49,    50,    51,    42,    43,    54,   230,   231,   127,
     127,   127,   127,   119,    14,    63,    64,   128,   113,   113,
     128,    69,   134,   116,   127,   127,    77,   127,   113,    10,
     127,   127,   113,   134,   133,   127,   127,   127,   127,   127,
     127,   127,   116,   127,   127,   127,   116,   127,   127,   113,
      98,    99,   127,    14,    14,   999,   134,    14,   127,  1003,
     127,   127,    77,    14,   132,   127,   114,   132,   127,   127,
     127,    10,    10,   121,    10,    10,   113,    10,   127,    27,
     128,   129,   127,   127,    56,   133,    14,   135,     3,     4,
       5,     6,    14,  1037,    14,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,   134,   127,   127,   127,
     127,    77,    77,   129,   128,   127,    31,    32,   127,   127,
     127,   113,   127,   134,   127,   127,    41,     7,     7,     7,
     113,   113,   127,   113,    49,    50,    51,    10,    10,    54,
     129,    72,   127,    14,    14,    14,    14,   134,    63,    64,
     127,   127,   127,    14,    69,   129,  1100,   127,   127,   127,
     133,    14,    14,   134,   127,   127,    14,   134,   127,   127,
    1043,   127,   127,   127,   127,   127,   556,   127,   127,   222,
     127,   127,   127,    98,    99,   129,   228,   486,   229,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,    -1,   128,     3,     4,     5,     6,   133,    -1,
     135,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,   127,   128,
       3,     4,     5,     6,   133,   134,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    -1,    -1,   127,   128,     3,     4,     5,     6,
     133,   134,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      -1,    -1,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    69,    -1,    -1,    -1,    73,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    41,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    49,    50,    51,
     127,   128,    54,    -1,    -1,    -1,   133,   134,    -1,    -1,
      -1,    63,    64,     3,     4,     5,     6,    69,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,
      50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    -1,    63,    64,   127,   128,    -1,    -1,    69,
      -1,   133,   134,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   114,    -1,    41,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    49,    50,    51,   127,   128,    54,
      -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      -1,    -1,   127,   128,     3,     4,     5,     6,   133,   134,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    73,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    49,    50,    51,   127,   128,
      54,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    63,
      64,     3,     4,     5,     6,    69,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    63,    64,   127,   128,    -1,    -1,    69,    -1,   133,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    98,    99,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   114,    -1,    41,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    49,    50,    51,   127,   128,    54,    -1,    -1,
      -1,   133,    -1,    -1,    -1,    -1,    63,    64,     3,     4,
       5,     6,    69,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    50,    51,   114,    -1,    54,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    63,    64,
      -1,   128,   129,    -1,    69,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   114,
      -1,    -1,    41,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      49,    50,    51,   128,   129,    54,    -1,    -1,   133,    -1,
      -1,    -1,    -1,    -1,    63,    64,     3,     4,     5,     6,
      69,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    98,
      99,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,   114,    -1,    54,    -1,    -1,
      -1,    -1,   121,   122,    -1,    -1,    63,    64,    -1,   128,
      -1,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,   128,   129,    -1,    -1,    -1,   133,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    51,    -1,    -1,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,     3,
       4,     5,     6,    69,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    51,   114,    -1,
      54,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    63,
      64,    -1,   128,    -1,    -1,    69,    -1,   133,    72,    -1,
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
     129,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,
      41,    -1,    -1,    -1,   121,    -1,    -1,    -1,    49,    50,
      51,   128,   129,    54,    -1,    -1,   133,    -1,    -1,    -1,
      -1,    -1,    63,    64,     3,     4,     5,     6,    69,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    63,    64,    -1,   128,   129,    -1,
      69,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    -1,    41,    -1,
      -1,    -1,   121,    -1,    -1,    -1,    49,    50,    51,   128,
     129,    54,    -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,
      63,    64,     3,     4,     5,     6,    69,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      51,   114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,    63,    64,    -1,   128,   129,    -1,    69,    -1,
     133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    98,    99,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    -1,    41,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    49,    50,    51,   128,   129,    54,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    -1,    63,    64,
       3,     4,     5,     6,    69,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,   114,
      -1,    54,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
      63,    64,    -1,   128,   129,    -1,    69,    -1,   133,    -1,
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
     128,    -1,    -1,    69,    -1,   133,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      41,    -1,    -1,    -1,    -1,   121,    -1,    -1,    49,    50,
      51,    -1,   128,    54,    -1,    -1,    -1,   133,    -1,    -1,
      -1,    -1,    63,    64,     3,     4,     5,     6,    69,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    50,    51,   114,    -1,    54,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    -1,    63,    64,    -1,   128,    -1,    -1,
      69,    -1,   133,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    98,
      99,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   114,    -1,    41,    -1,    -1,
      -1,    -1,   121,    -1,    -1,    49,    50,    51,    -1,   128,
      54,    -1,    -1,    -1,   133,    -1,    -1,    -1,    -1,    63,
      64,     3,     4,     5,     6,    69,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    41,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    51,
     114,    -1,    54,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      -1,    63,    64,    -1,   128,    -1,    -1,    69,    -1,   133,
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
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,    -1,    41,    -1,    -1,    -1,    -1,
     121,    -1,    -1,    49,    50,    51,    -1,   128,    54,    -1,
      -1,    -1,   133,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,    -1,    78,    79,   133,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   105,   106,   107,   108,   109,   110,    -1,   112,
      -1,    -1,   115,   116,   117,   118,   119,   120,    -1,   122,
     123,   124,    -1,   126
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
     113,   126,   128,   113,   131,   115,   133,   113,   126,   128,
      10,    11,    12,    13,    31,   164,   177,    14,    10,    11,
     127,   128,   166,   183,    10,    11,    12,    14,    31,    35,
     128,   128,   166,   128,   166,   128,   166,   128,   166,   166,
     173,    14,   164,   164,   166,   127,   166,    10,    14,   166,
      12,    12,   128,   166,   128,   166,   164,   128,   164,    26,
      43,   127,    26,    43,   127,   133,   165,   165,   133,   165,
     166,   164,   164,   166,    14,    14,    14,    31,   177,    14,
      10,    10,   177,   177,   129,   166,    72,   138,    26,    29,
      30,    36,    39,    40,    43,    44,   127,    26,    29,    30,
      36,    39,    40,    43,    44,   127,    81,    82,    83,    84,
      91,    92,    93,   115,   116,    85,    86,    87,    88,    89,
      90,    97,   109,   117,   118,   105,    78,    79,   119,   110,
     120,   121,   106,   107,   108,   122,   123,   124,   112,   126,
     126,    14,   166,   166,   166,   166,   166,   166,    14,   129,
     183,   128,   166,   122,   166,    14,   166,   166,    14,    17,
      58,    14,   129,   135,   161,   183,   131,   115,   133,   127,
     100,   101,   102,   103,   104,   113,   113,   129,   166,   183,
     127,   130,   113,   127,   113,   127,   113,   127,    10,    11,
      12,    14,    10,    14,    10,   179,   166,    26,    29,    30,
      36,    39,    40,    43,   127,   130,   166,    26,    43,   127,
     130,   166,   164,   166,   164,    80,   130,   164,    78,    79,
     128,   133,   153,   127,    26,    43,   127,   113,   113,   133,
     115,   133,   115,   133,   166,   164,   166,   164,    29,    44,
      23,   166,   166,   166,   166,   166,   166,   166,   166,   166,
      53,   157,   133,   128,    14,    62,   133,   129,   130,   134,
      10,    11,    12,    13,    24,    25,    33,    34,    47,    48,
     134,   166,   166,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   166,   169,   169,
     169,   169,   169,   169,   167,   169,   169,   169,   169,   169,
     169,   169,   169,     7,     8,     9,   170,   172,   172,   172,
     173,   173,   174,   175,   176,   176,   177,   177,   177,   177,
     177,   177,   177,    14,    22,    24,    25,    33,    34,    41,
      42,    49,    50,    51,   180,    17,   180,   127,   128,   127,
     127,   127,   127,   127,   127,   113,   128,   129,   166,   111,
     127,   130,   120,   121,   130,   132,   116,   134,   127,   128,
      14,   128,    77,    14,   129,   130,   129,   130,   166,    14,
     166,   166,   166,   166,   166,   166,   166,   166,   127,   129,
     166,   166,   128,   166,     5,    14,   128,   162,   166,   113,
     127,   113,   127,   113,   127,   128,   153,   113,   113,   129,
     130,   130,   166,   166,   166,   166,   166,   166,   166,   183,
     130,   166,   166,   183,   129,    27,    28,   141,   129,    10,
     183,   173,   173,    10,    11,    14,    65,    66,    67,   129,
     163,   138,   166,   166,   166,   166,   150,    14,   166,    14,
     166,   129,    27,   129,   166,   166,    10,   127,   127,   127,
     127,   127,   134,   164,    14,    22,   133,   158,   129,   163,
     128,   128,   160,   129,   183,   126,   131,   133,   113,   166,
     166,   166,   134,   166,    26,    43,   134,    26,    43,   134,
      26,    29,    30,    36,    39,    40,    43,    44,   134,   127,
     127,   127,   127,   127,   127,   127,   127,   127,   127,   127,
     127,   127,   127,   127,   127,    94,   128,    14,   128,   129,
     183,   166,   129,   183,   130,   166,   183,   166,   166,   183,
     113,   113,   113,   129,   183,   129,   161,   183,   166,   128,
      14,   135,   161,   132,   116,   134,   127,   127,   166,   111,
     127,   130,    77,    77,   162,   127,   130,   127,   166,   128,
     166,   166,   129,   163,   166,   166,   113,    10,   183,   127,
     127,   127,   127,   127,   127,   127,   127,   183,   127,   127,
     127,   164,   140,   164,   128,   166,   164,   164,    80,   164,
      80,    80,   113,    10,    70,    71,   153,   129,   130,    24,
      25,    34,   134,   166,   127,   127,   127,   127,    37,    38,
     134,   116,   134,   116,   134,   164,   164,   164,   127,   127,
     113,   166,    14,    14,   158,    56,    57,    60,   134,   153,
     129,   129,   163,   163,    74,    75,    76,   134,   129,    14,
     166,   166,   166,    26,    30,    36,    39,    40,    43,   134,
     134,   134,   134,   166,   166,   166,   166,   166,   166,   166,
     166,   166,   166,   166,   166,   167,   129,   183,   129,   161,
     183,   127,   129,   127,   129,   183,   127,   127,   132,   132,
     132,   166,   166,   166,   129,   129,   129,   166,    77,    14,
     129,   130,   166,   183,   166,   166,   129,     5,    14,   127,
     166,   127,   130,   127,   153,   129,   127,   127,   166,   129,
     129,    27,   141,   166,   164,    10,    10,    10,   166,   113,
      10,    10,   153,    10,    11,    14,    65,    66,    67,   166,
     166,   134,   166,    26,    29,    30,    36,    39,    40,    43,
      44,   134,   166,   164,   127,   127,    27,   166,   127,   134,
      58,   180,    10,    13,    14,    15,    16,    56,   153,   153,
     129,   129,    14,    14,    14,   113,   128,   132,   134,   134,
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
     139,   139,   139,   139,   139,   139,   140,   140,   140,   140,
     140,   140,   140,   140,   141,   141,   141,   141,   141,   141,
     142,   142,   143,   143,   143,   143,   144,   144,   145,   145,
     145,   146,   146,   146,   147,   148,   148,   148,   148,   148,
     148,   149,   149,   150,   150,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   152,   152,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   155,   156,   157,   157,   157,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   159,   160,   160,   160,   160,   161,   161,   161,
     161,   162,   162,   162,   162,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   165,
     166,   166,   166,   166,   166,   166,   166,   166,   166,   167,
     167,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   169,
     169,   170,   170,   170,   171,   171,   172,   172,   172,   173,
     173,   174,   174,   175,   175,   175,   176,   176,   176,   176,
     176,   176,   176,   177,   177,   177,   177,   177,   178,   178,
     179,   179,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   182,   182,   182,   182,   182,   182,   182,   182,
     182,   182,   183,   183,   183,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     3,
       3,     5,     7,     7,     9,     4,     6,     6,     8,     5,
       5,     7,     6,     6,     8,    10,     6,     4,     4,     4,
       3,     5,     5,     6,     6,     2,     5,     4,     3,     3,
       5,     7,     3,     5,     7,     3,     3,     2,     5,     5,
       4,     7,     6,     4,     6,     4,     2,     4,     4,     4,
       4,     4,     4,     4,     4,     7,     7,     7,     6,     6,
       4,     4,     4,     4,     4,     4,     4,     4,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     4,     4,     4,
       4,     4,     2,     1,     1,     1,     1,     1,     2,     4,
       2,     1,     1,     1,     1,     1,     2,     2,     4,     4,
       4,     4,     1,     1,     1,     1,     5,     7,     7,     3,
       5,     5,     4,     6,     3,     5,     5,     7,     4,     6,
       5,     3,     5,     7,     3,     5,     5,     3,     2,     5,
       5,     2,    12,     9,     1,     7,     7,     5,     7,     5,
       3,     5,     7,     0,     4,     6,     5,     3,     7,     6,
       4,     7,     6,     7,     6,     5,     6,     5,     3,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     3,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     6,     5,     0,     3,     3,     0,     4,
       4,     4,     5,     5,     4,     5,     5,     6,     6,     7,
       7,     6,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     4,     7,     6,     4,     8,
       7,     5,     5,     0,     4,     4,     4,     3,     5,     5,
       7,     3,     3,     5,     5,     1,     1,     3,     2,     3,
       3,     3,     4,     5,     5,     3,     5,     4,     6,     1,
       3,     1,     3,     1,     3,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     2,     2,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     4,     6,     4,     3,
       6,     6,     5,     3,     3,     6,     5,     4,     4,     6,
       6,     5,     3,     6,     5,     3,     2,     3,     3,     2,
       3,     1,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       4,     6,     6,     6,     1,     4,     4,     5,     5,     1,
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
#line 396 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2901 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 406 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2907 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 407 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2913 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2919 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2925 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 415 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2931 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 417 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2939 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 421 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2945 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 423 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2951 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 425 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2957 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 427 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2965 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 431 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2972 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 434 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2980 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 438 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2986 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 440 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2994 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 444 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3001 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 447 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3009 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 451 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3015 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 453 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3021 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 455 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3027 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 457 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3033 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3039 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 461 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3047 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 465 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3055 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 469 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3061 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 471 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3067 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 473 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3073 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 475 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3079 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 477 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3085 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 479 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3091 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3097 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3103 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3109 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 487 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3115 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 489 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3121 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 491 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3127 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 493 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3133 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 495 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3139 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 497 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3146 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 500 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3153 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 503 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3159 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 505 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3166 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 508 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3173 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 511 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3179 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 513 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3185 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 515 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3191 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 517 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3198 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 520 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3205 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 523 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3211 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 525 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3220 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 530 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3228 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 534 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3236 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 538 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3245 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 543 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3253 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall ';'  */
#line 546 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3259 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 548 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3265 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 550 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3271 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 552 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3277 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 554 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3283 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 556 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3290 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 559 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3296 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 561 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3302 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 563 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3308 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 565 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3315 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 568 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3322 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 571 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3329 "raku.tab.c"
    break;

  case 68: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 574 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3336 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 577 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3343 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_IF expr ';'  */
#line 580 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3349 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNLESS expr ';'  */
#line 582 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3355 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_WHILE expr ';'  */
#line 584 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3361 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_UNTIL expr ';'  */
#line 586 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3367 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_FOR expr ';'  */
#line 588 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3374 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_WITH expr ';'  */
#line 591 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3380 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITHOUT expr ';'  */
#line 593 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3386 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_GIVEN expr ';'  */
#line 595 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3392 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 597 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3399 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 600 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3406 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 603 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3414 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 607 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3421 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 610 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3427 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 612 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3433 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 614 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3439 "raku.tab.c"
    break;

  case 85: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 616 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3446 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 619 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3453 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 622 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3460 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 625 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3467 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 628 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3474 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 631 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3481 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 634 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3488 "raku.tab.c"
    break;

  case 92: /* stmt: expr ';'  */
#line 636 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3494 "raku.tab.c"
    break;

  case 93: /* stmt: ';'  */
#line 637 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3500 "raku.tab.c"
    break;

  case 94: /* stmt: if_stmt  */
#line 638 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3506 "raku.tab.c"
    break;

  case 95: /* stmt: while_stmt  */
#line 639 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3512 "raku.tab.c"
    break;

  case 96: /* stmt: for_stmt  */
#line 640 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3518 "raku.tab.c"
    break;

  case 97: /* stmt: given_stmt  */
#line 641 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3524 "raku.tab.c"
    break;

  case 98: /* stmt: KW_TRY block  */
#line 643 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3530 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block KW_CATCH block  */
#line 645 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3536 "raku.tab.c"
    break;

  case 100: /* stmt: KW_CATCH block  */
#line 647 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3542 "raku.tab.c"
    break;

  case 101: /* stmt: block  */
#line 649 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3548 "raku.tab.c"
    break;

  case 102: /* stmt: unless_stmt  */
#line 650 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3554 "raku.tab.c"
    break;

  case 103: /* stmt: until_stmt  */
#line 651 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3560 "raku.tab.c"
    break;

  case 104: /* stmt: repeat_stmt  */
#line 652 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3566 "raku.tab.c"
    break;

  case 105: /* stmt: loop_stmt  */
#line 653 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3572 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST ';'  */
#line 654 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3578 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT ';'  */
#line 655 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3584 "raku.tab.c"
    break;

  case 108: /* stmt: KW_LAST KW_IF expr ';'  */
#line 657 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3590 "raku.tab.c"
    break;

  case 109: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 659 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3596 "raku.tab.c"
    break;

  case 110: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 661 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3602 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 663 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3608 "raku.tab.c"
    break;

  case 112: /* stmt: sub_decl  */
#line 664 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3614 "raku.tab.c"
    break;

  case 113: /* stmt: class_decl  */
#line 665 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3620 "raku.tab.c"
    break;

  case 114: /* stmt: role_decl  */
#line 666 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3626 "raku.tab.c"
    break;

  case 115: /* stmt: grammar_decl  */
#line 667 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3632 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3638 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 673 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3644 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 675 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3650 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block  */
#line 677 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3656 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 679 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3662 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3668 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF expr block elsif_tail  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3674 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 685 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3680 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block  */
#line 689 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3686 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 691 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3692 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 693 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3698 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3704 "raku.tab.c"
    break;

  case 128: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 697 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3710 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3716 "raku.tab.c"
    break;

  case 130: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 703 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3722 "raku.tab.c"
    break;

  case 131: /* while_stmt: KW_WHILE expr block  */
#line 705 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3728 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 709 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3734 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 711 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3740 "raku.tab.c"
    break;

  case 134: /* unless_stmt: KW_UNLESS expr block  */
#line 713 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3746 "raku.tab.c"
    break;

  case 135: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 715 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3752 "raku.tab.c"
    break;

  case 136: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3758 "raku.tab.c"
    break;

  case 137: /* until_stmt: KW_UNTIL expr block  */
#line 721 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3764 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block  */
#line 725 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3770 "raku.tab.c"
    break;

  case 139: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 727 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3776 "raku.tab.c"
    break;

  case 140: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 729 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3782 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP block  */
#line 733 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3788 "raku.tab.c"
    break;

  case 142: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 735 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3794 "raku.tab.c"
    break;

  case 143: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 737 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3800 "raku.tab.c"
    break;

  case 144: /* loop_incr: expr  */
#line 740 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3806 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 744 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3816 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 750 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3826 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 756 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3835 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 761 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3845 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 767 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3854 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr block  */
#line 772 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3861 "raku.tab.c"
    break;

  case 151: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 777 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3873 "raku.tab.c"
    break;

  case 152: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 785 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3886 "raku.tab.c"
    break;

  case 153: /* when_list: %empty  */
#line 795 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3892 "raku.tab.c"
    break;

  case 154: /* when_list: when_list KW_WHEN expr block  */
#line 797 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3900 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 803 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3912 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 811 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3922 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT sub_body  */
#line 817 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3932 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 823 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3944 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 831 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3954 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 837 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3964 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 843 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3977 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 852 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3988 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 859 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4001 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 868 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4014 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 877 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4025 "raku.tab.c"
    break;

  case 166: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 886 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4036 "raku.tab.c"
    break;

  case 167: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 893 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4045 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list '}'  */
#line 899 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4051 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr '}'  */
#line 901 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4058 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 904 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4065 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 907 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4071 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 909 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4077 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 911 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4083 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 913 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4089 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 915 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4095 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 917 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4101 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 919 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4107 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 921 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4114 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 924 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4120 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 926 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4126 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 928 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4132 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 930 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4139 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 933 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4146 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 936 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4154 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list '}'  */
#line 941 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4160 "raku.tab.c"
    break;

  case 186: /* method_body: '{' YADA '}'  */
#line 942 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4166 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr '}'  */
#line 944 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4173 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 947 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4180 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 950 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4186 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 952 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4192 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 954 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4198 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 956 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4204 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 958 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4210 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 960 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4216 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 962 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4222 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 964 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4229 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 967 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4235 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 969 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4241 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 971 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4247 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 973 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4254 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 976 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4261 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 979 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4269 "raku.tab.c"
    break;

  case 203: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 985 "raku.y"
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
#line 4287 "raku.tab.c"
    break;

  case 204: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1001 "raku.y"
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
#line 4304 "raku.tab.c"
    break;

  case 205: /* is_clauses: %empty  */
#line 1015 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4310 "raku.tab.c"
    break;

  case 206: /* is_clauses: is_clauses IDENT IDENT  */
#line 1017 "raku.y"
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
#line 4326 "raku.tab.c"
    break;

  case 207: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1029 "raku.y"
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
#line 4342 "raku.tab.c"
    break;

  case 208: /* class_body_list: %empty  */
#line 1042 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4348 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1044 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4355 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1047 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4362 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1050 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4369 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1053 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4376 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1056 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4383 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1059 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4390 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1062 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4397 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1065 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4404 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1068 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4411 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1071 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4418 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1074 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4425 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1077 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4432 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1080 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4443 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1087 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4454 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1094 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4465 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1101 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4476 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1108 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4484 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1112 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4492 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1116 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4500 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1120 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4508 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1124 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4519 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1131 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4530 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1138 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4541 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1145 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4552 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1152 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4566 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1162 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4578 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1170 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4590 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1178 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4603 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1187 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4614 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1194 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4625 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1201 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4639 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1211 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4651 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1219 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4663 "raku.tab.c"
    break;

  case 242: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1229 "raku.y"
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
#line 4680 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: %empty  */
#line 1243 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4686 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1245 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4695 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1250 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4704 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1255 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4713 "raku.tab.c"
    break;

  case 247: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1262 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4721 "raku.tab.c"
    break;

  case 248: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1266 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4729 "raku.tab.c"
    break;

  case 249: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1270 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4737 "raku.tab.c"
    break;

  case 250: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1274 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4745 "raku.tab.c"
    break;

  case 251: /* pair_list: IDENT OP_FATARROW expr  */
#line 1280 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4751 "raku.tab.c"
    break;

  case 252: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1282 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4757 "raku.tab.c"
    break;

  case 253: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1284 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4763 "raku.tab.c"
    break;

  case 254: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1286 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4769 "raku.tab.c"
    break;

  case 255: /* param_list: VAR_SCALAR  */
#line 1289 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4775 "raku.tab.c"
    break;

  case 256: /* param_list: VAR_ARRAY  */
#line 1290 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4781 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' VAR_ARRAY  */
#line 1291 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4787 "raku.tab.c"
    break;

  case 258: /* param_list: IDENT VAR_SCALAR  */
#line 1292 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4793 "raku.tab.c"
    break;

  case 259: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1293 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4799 "raku.tab.c"
    break;

  case 260: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1294 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4805 "raku.tab.c"
    break;

  case 261: /* param_list: param_list ',' VAR_SCALAR  */
#line 1295 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4811 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1296 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4817 "raku.tab.c"
    break;

  case 263: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1297 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4823 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1298 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4829 "raku.tab.c"
    break;

  case 265: /* param_list: VAR_SCALAR '=' expr  */
#line 1299 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4835 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1300 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4841 "raku.tab.c"
    break;

  case 267: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1301 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4847 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1302 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4853 "raku.tab.c"
    break;

  case 269: /* param_list: SLURPY_POS  */
#line 1303 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4859 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' SLURPY_POS  */
#line 1304 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4865 "raku.tab.c"
    break;

  case 271: /* param_list: SLURPY_LOL  */
#line 1305 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4871 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' SLURPY_LOL  */
#line 1306 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4877 "raku.tab.c"
    break;

  case 273: /* param_list: SLURPY_NAMED  */
#line 1307 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4883 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1308 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4889 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list '}'  */
#line 1311 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4895 "raku.tab.c"
    break;

  case 276: /* block: '{' YADA '}'  */
#line 1312 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4901 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list expr '}'  */
#line 1314 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4907 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1316 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4913 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1318 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4919 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1320 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4925 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1322 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4931 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1324 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4938 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1327 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4944 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1329 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4950 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1331 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4956 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1333 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4962 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1335 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4969 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1338 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4976 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1341 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4984 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1345 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4991 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1348 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4998 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1351 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5005 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1354 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5011 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1356 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5019 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1360 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5026 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1363 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5033 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1366 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5040 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1369 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5047 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1372 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5054 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1375 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5060 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1377 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5066 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1379 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5072 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_LAST '}'  */
#line 1381 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5078 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1383 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5084 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1385 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5090 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1387 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5096 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1389 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5102 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1391 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5108 "raku.tab.c"
    break;

  case 309: /* closure: '{' expr '}'  */
#line 1394 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5114 "raku.tab.c"
    break;

  case 310: /* expr: VAR_SCALAR '=' expr  */
#line 1397 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5120 "raku.tab.c"
    break;

  case 311: /* expr: VAR_ARRAY '=' expr  */
#line 1398 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5126 "raku.tab.c"
    break;

  case 312: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1399 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5132 "raku.tab.c"
    break;

  case 313: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1400 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5138 "raku.tab.c"
    break;

  case 314: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1401 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5144 "raku.tab.c"
    break;

  case 315: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1402 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5150 "raku.tab.c"
    break;

  case 316: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1403 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5156 "raku.tab.c"
    break;

  case 317: /* expr: KW_GATHER block  */
#line 1404 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5166 "raku.tab.c"
    break;

  case 318: /* expr: tern_expr  */
#line 1409 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5172 "raku.tab.c"
    break;

  case 319: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1413 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5178 "raku.tab.c"
    break;

  case 320: /* tern_expr: cmp_expr  */
#line 1414 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5184 "raku.tab.c"
    break;

  case 321: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1417 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5190 "raku.tab.c"
    break;

  case 322: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1418 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5196 "raku.tab.c"
    break;

  case 323: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1419 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5202 "raku.tab.c"
    break;

  case 324: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1420 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 325: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1421 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5214 "raku.tab.c"
    break;

  case 326: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1422 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5220 "raku.tab.c"
    break;

  case 327: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1423 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5226 "raku.tab.c"
    break;

  case 328: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1424 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5232 "raku.tab.c"
    break;

  case 329: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1425 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5238 "raku.tab.c"
    break;

  case 330: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1426 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5244 "raku.tab.c"
    break;

  case 331: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1427 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5250 "raku.tab.c"
    break;

  case 332: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1428 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5256 "raku.tab.c"
    break;

  case 333: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1429 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5262 "raku.tab.c"
    break;

  case 334: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1430 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5268 "raku.tab.c"
    break;

  case 335: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1432 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5278 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1438 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5288 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1444 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5298 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr  */
#line 1449 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5304 "raku.tab.c"
    break;

  case 339: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1452 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5310 "raku.tab.c"
    break;

  case 340: /* divis_expr: jct_expr  */
#line 1453 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5316 "raku.tab.c"
    break;

  case 341: /* jct_expr: jct_expr '|' range_expr  */
#line 1456 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5322 "raku.tab.c"
    break;

  case 342: /* jct_expr: jct_expr '&' range_expr  */
#line 1457 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5328 "raku.tab.c"
    break;

  case 343: /* jct_expr: dor_expr  */
#line 1458 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5334 "raku.tab.c"
    break;

  case 344: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1462 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5340 "raku.tab.c"
    break;

  case 345: /* dor_expr: range_expr  */
#line 1463 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5346 "raku.tab.c"
    break;

  case 346: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1466 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5352 "raku.tab.c"
    break;

  case 347: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1467 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5358 "raku.tab.c"
    break;

  case 348: /* range_expr: add_expr  */
#line 1468 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5364 "raku.tab.c"
    break;

  case 349: /* add_expr: add_expr '~' repl_expr  */
#line 1471 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5370 "raku.tab.c"
    break;

  case 350: /* add_expr: repl_expr  */
#line 1472 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5376 "raku.tab.c"
    break;

  case 351: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1475 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5382 "raku.tab.c"
    break;

  case 352: /* repl_expr: addsub_expr  */
#line 1476 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5388 "raku.tab.c"
    break;

  case 353: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1479 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5394 "raku.tab.c"
    break;

  case 354: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1480 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5400 "raku.tab.c"
    break;

  case 355: /* addsub_expr: mul_expr  */
#line 1481 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5406 "raku.tab.c"
    break;

  case 356: /* mul_expr: mul_expr '*' unary_expr  */
#line 1484 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5412 "raku.tab.c"
    break;

  case 357: /* mul_expr: mul_expr '/' unary_expr  */
#line 1485 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5418 "raku.tab.c"
    break;

  case 358: /* mul_expr: mul_expr '%' unary_expr  */
#line 1486 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5424 "raku.tab.c"
    break;

  case 359: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1487 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5430 "raku.tab.c"
    break;

  case 360: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1489 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5436 "raku.tab.c"
    break;

  case 361: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1491 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5442 "raku.tab.c"
    break;

  case 362: /* mul_expr: unary_expr  */
#line 1492 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5448 "raku.tab.c"
    break;

  case 363: /* unary_expr: '-' unary_expr  */
#line 1495 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5454 "raku.tab.c"
    break;

  case 364: /* unary_expr: '!' unary_expr  */
#line 1496 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5460 "raku.tab.c"
    break;

  case 365: /* unary_expr: CARET unary_expr  */
#line 1497 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5466 "raku.tab.c"
    break;

  case 366: /* unary_expr: OP_REDUCE unary_expr  */
#line 1499 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5475 "raku.tab.c"
    break;

  case 367: /* unary_expr: pow_expr  */
#line 1503 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5481 "raku.tab.c"
    break;

  case 368: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1506 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5487 "raku.tab.c"
    break;

  case 369: /* pow_expr: postfix_expr  */
#line 1507 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5493 "raku.tab.c"
    break;

  case 370: /* scalar_list: VAR_SCALAR  */
#line 1510 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5499 "raku.tab.c"
    break;

  case 371: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1511 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5505 "raku.tab.c"
    break;

  case 372: /* meth_name: IDENT  */
#line 1514 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5511 "raku.tab.c"
    break;

  case 373: /* meth_name: KW_SORT  */
#line 1515 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5517 "raku.tab.c"
    break;

  case 374: /* meth_name: KW_MAP  */
#line 1516 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5523 "raku.tab.c"
    break;

  case 375: /* meth_name: KW_GREP  */
#line 1517 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5529 "raku.tab.c"
    break;

  case 376: /* meth_name: KW_SAY  */
#line 1518 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5535 "raku.tab.c"
    break;

  case 377: /* meth_name: KW_PRINT  */
#line 1519 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5541 "raku.tab.c"
    break;

  case 378: /* meth_name: KW_TAKE  */
#line 1520 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5547 "raku.tab.c"
    break;

  case 379: /* meth_name: KW_RETURN  */
#line 1521 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5553 "raku.tab.c"
    break;

  case 380: /* meth_name: KW_EXISTS  */
#line 1522 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5559 "raku.tab.c"
    break;

  case 381: /* meth_name: KW_DELETE  */
#line 1523 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5565 "raku.tab.c"
    break;

  case 382: /* meth_name: TESTOP  */
#line 1524 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5571 "raku.tab.c"
    break;

  case 383: /* postfix_expr: call_expr  */
#line 1526 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5577 "raku.tab.c"
    break;

  case 384: /* call_expr: IDENT '(' arg_list ')'  */
#line 1529 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5586 "raku.tab.c"
    break;

  case 385: /* call_expr: IDENT '(' ')'  */
#line 1533 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5592 "raku.tab.c"
    break;

  case 386: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1535 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5598 "raku.tab.c"
    break;

  case 387: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1537 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5604 "raku.tab.c"
    break;

  case 388: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1539 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5613 "raku.tab.c"
    break;

  case 389: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1544 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5619 "raku.tab.c"
    break;

  case 390: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1546 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5629 "raku.tab.c"
    break;

  case 391: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1552 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5639 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1558 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5647 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '.' KW_NEW  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5655 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '.' IDENT  */
#line 1566 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5664 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5675 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1578 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5684 "raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '.' CARET IDENT  */
#line 1583 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5694 "raku.tab.c"
    break;

  case 398: /* call_expr: atom '.' CARET IDENT  */
#line 1589 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5704 "raku.tab.c"
    break;

  case 399: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1595 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5715 "raku.tab.c"
    break;

  case 400: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1602 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5726 "raku.tab.c"
    break;

  case 401: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1609 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5735 "raku.tab.c"
    break;

  case 402: /* call_expr: atom '.' meth_name  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5744 "raku.tab.c"
    break;

  case 403: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1619 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5755 "raku.tab.c"
    break;

  case 404: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1626 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5764 "raku.tab.c"
    break;

  case 405: /* call_expr: call_expr '.' meth_name  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5773 "raku.tab.c"
    break;

  case 406: /* call_expr: KW_DIE expr  */
#line 1636 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5779 "raku.tab.c"
    break;

  case 407: /* call_expr: KW_MAP closure expr  */
#line 1638 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5785 "raku.tab.c"
    break;

  case 408: /* call_expr: KW_GREP closure expr  */
#line 1640 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5791 "raku.tab.c"
    break;

  case 409: /* call_expr: KW_SORT expr  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5797 "raku.tab.c"
    break;

  case 410: /* call_expr: KW_SORT closure expr  */
#line 1644 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5803 "raku.tab.c"
    break;

  case 411: /* call_expr: atom  */
#line 1645 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5809 "raku.tab.c"
    break;

  case 412: /* arg_list: expr  */
#line 1648 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5815 "raku.tab.c"
    break;

  case 413: /* arg_list: arg_list ',' expr  */
#line 1649 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5821 "raku.tab.c"
    break;

  case 414: /* arg_list: arg_list ','  */
#line 1650 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5827 "raku.tab.c"
    break;

  case 415: /* atom: LIT_INT  */
#line 1653 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5833 "raku.tab.c"
    break;

  case 416: /* atom: LIT_FLOAT  */
#line 1654 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5839 "raku.tab.c"
    break;

  case 417: /* atom: LIT_STR  */
#line 1655 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5845 "raku.tab.c"
    break;

  case 418: /* atom: WORDLIST  */
#line 1657 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5858 "raku.tab.c"
    break;

  case 419: /* atom: LIT_INTERP_STR  */
#line 1665 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5864 "raku.tab.c"
    break;

  case 420: /* atom: VAR_SCALAR  */
#line 1666 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5870 "raku.tab.c"
    break;

  case 421: /* atom: OP_INC VAR_SCALAR  */
#line 1667 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5876 "raku.tab.c"
    break;

  case 422: /* atom: OP_DEC VAR_SCALAR  */
#line 1668 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5882 "raku.tab.c"
    break;

  case 423: /* atom: VAR_SCALAR OP_INC  */
#line 1669 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5888 "raku.tab.c"
    break;

  case 424: /* atom: VAR_SCALAR OP_DEC  */
#line 1670 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5894 "raku.tab.c"
    break;

  case 425: /* atom: VAR_ARRAY  */
#line 1671 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5900 "raku.tab.c"
    break;

  case 426: /* atom: VAR_HASH  */
#line 1672 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5906 "raku.tab.c"
    break;

  case 427: /* atom: VAR_CAPTURE  */
#line 1674 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5914 "raku.tab.c"
    break;

  case 428: /* atom: VAR_FH  */
#line 1678 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5922 "raku.tab.c"
    break;

  case 429: /* atom: VAR_NAMED_CAPTURE  */
#line 1682 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5929 "raku.tab.c"
    break;

  case 430: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1685 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5935 "raku.tab.c"
    break;

  case 431: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1687 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5941 "raku.tab.c"
    break;

  case 432: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1689 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5947 "raku.tab.c"
    break;

  case 433: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1691 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5953 "raku.tab.c"
    break;

  case 434: /* atom: ARR_ALL_SLICE  */
#line 1693 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5959 "raku.tab.c"
    break;

  case 435: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1695 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5965 "raku.tab.c"
    break;

  case 436: /* atom: VAR_HASH '{' expr '}'  */
#line 1697 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5971 "raku.tab.c"
    break;

  case 437: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1699 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5977 "raku.tab.c"
    break;

  case 438: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1701 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5983 "raku.tab.c"
    break;

  case 439: /* atom: IDENT  */
#line 1702 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5989 "raku.tab.c"
    break;

  case 440: /* atom: VAR_TWIGIL  */
#line 1704 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5997 "raku.tab.c"
    break;

  case 441: /* atom: VAR_ARRAY_TWIGIL  */
#line 1708 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6005 "raku.tab.c"
    break;

  case 442: /* atom: VAR_HASH_TWIGIL  */
#line 1712 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6013 "raku.tab.c"
    break;

  case 443: /* atom: '(' ')'  */
#line 1715 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6019 "raku.tab.c"
    break;

  case 444: /* atom: '(' expr ')'  */
#line 1716 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6025 "raku.tab.c"
    break;

  case 445: /* atom: '(' expr ',' ')'  */
#line 1718 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6031 "raku.tab.c"
    break;

  case 446: /* atom: '(' expr ',' arg_list ')'  */
#line 1720 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6038 "raku.tab.c"
    break;

  case 447: /* atom: block  */
#line 1722 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6044 "raku.tab.c"
    break;

  case 448: /* atom: KW_SUB block  */
#line 1723 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6050 "raku.tab.c"
    break;


#line 6054 "raku.tab.c"

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

#line 1725 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
