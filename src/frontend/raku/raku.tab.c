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
  YYSYMBOL_KW_EXIT = 35,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 36,               /* KW_CONSTANT  */
  YYSYMBOL_KW_GIVEN = 37,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 38,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 39,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 40,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 41,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 42,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 43,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 44,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 45,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 46,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 47,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 48,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 49,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 50,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 51,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 52,                   /* KW_SORT  */
  YYSYMBOL_KW_TRY = 53,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 54,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 55,                    /* KW_DIE  */
  YYSYMBOL_KW_CLASS = 56,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 57,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 58,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 59,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 60,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 61,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 62,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 63,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 64,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 65,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 66,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 67,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 68,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 69,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 70,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 71,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 72,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 73,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 74,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 75,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 76,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 77,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 78,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 79,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 80,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 81,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 82,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 83,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 84,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 85,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 86,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 87,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 88,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 89,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 90,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 91,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 92,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 93,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 94,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 95,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 96,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 97,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 98,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 99,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 100,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 101,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 102,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 103,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 104,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 105,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 106,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 107,                   /* OP_DIV  */
  YYSYMBOL_OP_BAND = 108,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 109,                   /* OP_SHL  */
  YYSYMBOL_OP_DIVIS = 110,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 111,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 112,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 113,                   /* OP_POW  */
  YYSYMBOL_114_ = 114,                     /* '='  */
  YYSYMBOL_115_ = 115,                     /* '!'  */
  YYSYMBOL_116_ = 116,                     /* '<'  */
  YYSYMBOL_117_ = 117,                     /* '>'  */
  YYSYMBOL_118_ = 118,                     /* '|'  */
  YYSYMBOL_119_ = 119,                     /* '&'  */
  YYSYMBOL_120_ = 120,                     /* '~'  */
  YYSYMBOL_121_ = 121,                     /* '+'  */
  YYSYMBOL_122_ = 122,                     /* '-'  */
  YYSYMBOL_123_ = 123,                     /* '*'  */
  YYSYMBOL_124_ = 124,                     /* '/'  */
  YYSYMBOL_125_ = 125,                     /* '%'  */
  YYSYMBOL_UMINUS = 126,                   /* UMINUS  */
  YYSYMBOL_127_ = 127,                     /* '.'  */
  YYSYMBOL_128_ = 128,                     /* ';'  */
  YYSYMBOL_129_ = 129,                     /* '('  */
  YYSYMBOL_130_ = 130,                     /* ')'  */
  YYSYMBOL_131_ = 131,                     /* ','  */
  YYSYMBOL_132_ = 132,                     /* '['  */
  YYSYMBOL_133_ = 133,                     /* ']'  */
  YYSYMBOL_134_ = 134,                     /* '{'  */
  YYSYMBOL_135_ = 135,                     /* '}'  */
  YYSYMBOL_136_ = 136,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 137,                 /* $accept  */
  YYSYMBOL_program = 138,                  /* program  */
  YYSYMBOL_stmt_list = 139,                /* stmt_list  */
  YYSYMBOL_stmt = 140,                     /* stmt  */
  YYSYMBOL_if_stmt = 141,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 142,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 143,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 144,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 145,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 146,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 147,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 148,                /* loop_incr  */
  YYSYMBOL_for_stmt = 149,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 150,               /* given_stmt  */
  YYSYMBOL_when_list = 151,                /* when_list  */
  YYSYMBOL_sub_decl = 152,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 153,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 154,                 /* sub_body  */
  YYSYMBOL_method_body = 155,              /* method_body  */
  YYSYMBOL_class_decl = 156,               /* class_decl  */
  YYSYMBOL_role_decl = 157,                /* role_decl  */
  YYSYMBOL_is_clauses = 158,               /* is_clauses  */
  YYSYMBOL_class_body_list = 159,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 160,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 161,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 162,           /* named_arg_list  */
  YYSYMBOL_pair_list = 163,                /* pair_list  */
  YYSYMBOL_param_list = 164,               /* param_list  */
  YYSYMBOL_block = 165,                    /* block  */
  YYSYMBOL_closure = 166,                  /* closure  */
  YYSYMBOL_expr = 167,                     /* expr  */
  YYSYMBOL_tern_expr = 168,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 169,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 170,               /* divis_expr  */
  YYSYMBOL_jct_expr = 171,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 172,                 /* dor_expr  */
  YYSYMBOL_range_expr = 173,               /* range_expr  */
  YYSYMBOL_add_expr = 174,                 /* add_expr  */
  YYSYMBOL_repl_expr = 175,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 176,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 177,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 178,               /* unary_expr  */
  YYSYMBOL_pow_expr = 179,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 180,              /* scalar_list  */
  YYSYMBOL_meth_name = 181,                /* meth_name  */
  YYSYMBOL_postfix_expr = 182,             /* postfix_expr  */
  YYSYMBOL_call_expr = 183,                /* call_expr  */
  YYSYMBOL_arg_list = 184,                 /* arg_list  */
  YYSYMBOL_atom = 185                      /* atom  */
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
#define YYLAST   4664

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  137
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  452
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1145

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   369


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
       2,     2,     2,   115,     2,     2,     2,   125,   119,     2,
     129,   130,   123,   121,   131,   122,   127,   124,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   136,   128,
     116,   114,   117,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   132,     2,   133,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   134,   118,   135,   120,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   126
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
     520,   523,   526,   528,   531,   533,   538,   542,   546,   551,
     555,   556,   558,   560,   562,   564,   567,   569,   571,   573,
     576,   579,   582,   585,   588,   590,   592,   594,   596,   599,
     601,   603,   605,   608,   611,   615,   618,   620,   622,   624,
     627,   630,   633,   636,   639,   642,   645,   646,   647,   648,
     649,   650,   651,   653,   655,   657,   659,   660,   661,   662,
     663,   664,   665,   667,   669,   671,   673,   674,   675,   676,
     679,   681,   683,   685,   687,   689,   691,   693,   697,   699,
     701,   703,   705,   707,   711,   713,   717,   719,   721,   723,
     727,   729,   733,   735,   737,   741,   743,   745,   749,   752,
     758,   764,   769,   775,   780,   785,   793,   804,   805,   811,
     819,   825,   831,   839,   845,   851,   860,   867,   876,   885,
     894,   901,   908,   909,   912,   915,   917,   919,   921,   923,
     925,   927,   929,   932,   934,   936,   938,   941,   944,   950,
     951,   952,   955,   958,   960,   962,   964,   966,   968,   970,
     972,   975,   977,   979,   981,   984,   987,   993,  1009,  1024,
    1025,  1037,  1051,  1052,  1055,  1058,  1061,  1064,  1067,  1070,
    1073,  1076,  1079,  1082,  1085,  1088,  1095,  1102,  1109,  1116,
    1120,  1124,  1128,  1132,  1139,  1146,  1153,  1160,  1170,  1178,
    1186,  1195,  1202,  1209,  1219,  1227,  1237,  1252,  1253,  1258,
    1263,  1270,  1274,  1278,  1282,  1288,  1290,  1292,  1294,  1298,
    1299,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,
    1309,  1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1320,
    1321,  1322,  1324,  1326,  1328,  1330,  1332,  1335,  1337,  1339,
    1341,  1343,  1346,  1349,  1353,  1356,  1359,  1362,  1364,  1368,
    1371,  1374,  1377,  1380,  1383,  1385,  1387,  1389,  1391,  1393,
    1395,  1397,  1399,  1403,  1406,  1407,  1408,  1409,  1410,  1411,
    1412,  1413,  1418,  1421,  1423,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,
    1446,  1452,  1458,  1461,  1462,  1465,  1466,  1467,  1470,  1472,
    1475,  1476,  1477,  1480,  1481,  1484,  1485,  1488,  1489,  1490,
    1493,  1494,  1495,  1496,  1497,  1499,  1501,  1504,  1505,  1506,
    1507,  1512,  1515,  1516,  1519,  1520,  1523,  1524,  1525,  1526,
    1527,  1528,  1529,  1530,  1531,  1532,  1533,  1535,  1537,  1542,
    1543,  1545,  1547,  1552,  1554,  1560,  1566,  1570,  1574,  1579,
    1586,  1591,  1597,  1603,  1610,  1617,  1622,  1627,  1634,  1639,
    1644,  1646,  1648,  1650,  1652,  1654,  1657,  1658,  1659,  1662,
    1663,  1664,  1665,  1674,  1675,  1676,  1677,  1678,  1679,  1680,
    1681,  1682,  1686,  1690,  1693,  1695,  1697,  1699,  1701,  1703,
    1705,  1707,  1709,  1711,  1712,  1716,  1720,  1724,  1725,  1726,
    1728,  1731,  1732
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
  "KW_SUB", "KW_GATHER", "KW_TAKE", "KW_RETURN", "KW_EXIT", "KW_CONSTANT",
  "KW_GIVEN", "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT",
  "KW_EXISTS", "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT",
  "KW_LOOP", "KW_LAST", "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT",
  "KW_TRY", "KW_CATCH", "KW_DIE", "KW_CLASS", "KW_METHOD", "KW_HAS",
  "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE",
  "ARR_ALL_SLICE", "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED",
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
  "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE", "OP_LE",
  "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT", "OP_SGE",
  "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ",
  "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ",
  "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_BAND", "OP_SHL",
  "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'",
  "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'", "':'",
  "$accept", "program", "stmt_list", "stmt", "if_stmt", "elsif_tail",
  "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt", "loop_stmt",
  "loop_incr", "for_stmt", "given_stmt", "when_list", "sub_decl",
  "scalar_methcall", "sub_body", "method_body", "class_decl", "role_decl",
  "is_clauses", "class_body_list", "grammar_decl", "grammar_body_list",
  "named_arg_list", "pair_list", "param_list", "block", "closure", "expr",
  "tern_expr", "cmp_expr", "divis_expr", "jct_expr", "dor_expr",
  "range_expr", "add_expr", "repl_expr", "addsub_expr", "mul_expr",
  "unary_expr", "pow_expr", "scalar_list", "meth_name", "postfix_expr",
  "call_expr", "arg_list", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-829)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-452)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -829,    32,  2138,  -829,  -829,  -829,  -829,  -829,   636,   -95,
     -80,   -61,    14,  -829,  -829,  4530,  -829,  -829,  -829,    50,
    2213,   314,  3555,  3630,  3705,  3780,  3855,     2,    -8,  3855,
    2288,  2363,   141,  3855,   142,   202,  3930,  4005,    -8,    53,
      64,   191,    11,    11,  4080,    -8,    -8,  3855,   206,   212,
     145,  4530,  -829,  -829,   276,   227,   240,  4530,  4530,  -829,
    2438,   256,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,   509,  -829,  -829,  -829,   772,   575,  -829,   564,
     900,   346,   195,  -829,    69,   238,   437,   499,  -829,  -829,
     225,   267,   271,   396,  -829,  -829,  3855,  3855,  3855,  3855,
    3855,  3855,   407,  2514,  4155,  2590,   411,  3855,  3855,   310,
      65,  1119,   368,   342,   158,  -829,   358,    -8,  2665,  -829,
    -829,   316,   569,   162,  -829,  2741,  -829,   453,   -41,   277,
     303,   653,   479,   288,   503,  2665,    37,  2665,    60,  2665,
      -8,  2665,    -8,   -24,   133,   236,  -829,  -829,   381,  -829,
     223,  -829,   391,   414,   423,   466,   221,   244,  2665,    -8,
    2665,    -8,   241,  2873,  -829,  3855,  3855,  -829,  3855,  3855,
    -829,  3855,  3855,  3855,  2948,  3855,  -829,   513,  -829,  -829,
    -829,   493,   480,    26,  -829,   557,  -829,  -829,  -829,  -829,
     817,  -829,   441,   713,   508,  1532,  3855,  3855,  3855,  3855,
    3855,  3855,  3855,  3855,  -829,  3855,  3855,  3855,  3855,  3855,
    3855,  3855,  3855,  -829,  4530,  4530,  4530,  4530,  4530,  4530,
    4530,  4530,  4530,  4530,  4530,  4530,  4530,  4530,  4530,   868,
    4530,  4530,  4530,  4530,  4530,  4530,  4530,  4530,  4530,  4530,
    4530,  4530,  4530,  4530,  4530,  4530,  4530,   776,   754,   595,
     579,   590,   604,   615,   628,   642,   122,  -829,   778,  2665,
     103,   803,   545,   674,   659,   675,  -829,   670,   797,   687,
     279,  -829,   807,   804,   806,  2590,   880,  3855,  -829,  3855,
    3855,  3855,  3855,  3855,  3855,  3855,   697,   441,   809,  -829,
    3855,  3855,  -829,  4230,  -829,  4305,  -829,   323,   356,   372,
     237,   717,   799,  -829,   811,   819,  3855,  3855,  3855,  3855,
    3855,  3855,  3855,  -829,  3855,   821,  3855,  3855,  -829,  3855,
     826,   931,   831,  -829,   923,  3855,  -829,  4530,  4530,     4,
    -829,  -829,  -829,  3855,  3855,  -829,  -829,  3855,  3855,  -829,
     912,  3855,   929,  3855,   833,   888,   850,  -829,  3855,  3855,
     935,   838,   840,   848,   851,   857,   865,  -829,  -829,  -829,
      -8,    -5,  -829,    45,   825,   884,  -829,  -829,  3023,   919,
     999,  -829,   870,   254,   324,   934,  3555,  3630,  3855,  1861,
      88,   228,  -829,   343,   902,   941,   974,   978,   981,   982,
     983,   984,   989,   992,   993,   998,  1000,  1012,  1013,  1014,
     944,   944,   944,   944,   944,   944,  1032,   944,   944,   944,
     944,   944,   944,   944,   944,  -829,  -829,  -829,   346,  -829,
    -829,  -829,  1023,  1023,   238,   437,   499,   499,  -829,  -829,
    -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  -829,  -829,  -829,  -829,  1015,  1131,  1017,  -829,
    3099,  -829,  -829,  -829,  -829,  -829,  -829,  3855,  3175,  -829,
     852,  3855,  -829,  3855,  3855,  3855,  3855,  1033,  1034,  1035,
    -829,  3251,  -829,  1253,  3855,  1024,  -829,    -6,  -829,  1405,
     650,  1037,  1020,  -829,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  1028,  -829,  1029,  2665,   156,  1074,   312,  3327,   459,
    1030,  3855,  -829,  4380,  -829,  3855,  -829,    51,  -829,  3855,
    3855,  1045,  3023,  1036,  1038,  1039,  1040,  1044,  1047,  1048,
     498,  3023,  1049,  1050,   511,    -8,   -14,  4455,  -829,    -8,
      -8,    99,   -40,   -37,  1046,  -829,    20,  -829,  -829,  -829,
    1031,   862,  1659,  1052,  1054,  1057,  1058,     9,  1056,  1027,
    1062,  1053,    -8,    -8,    -8,  1063,  1064,  1073,  3855,  -829,
    -829,  -829,  -829,  -829,  -829,  1149,  1176,  -829,    27,  1031,
     864,   111,   438,   -26,  -829,   871,  3855,  -829,  1179,  2590,
    3855,  3855,   363,   185,   319,  -829,   353,  3855,  3855,  -829,
    3855,  3855,  -829,  3855,  3855,  3855,  3855,  3855,  3855,  3855,
    3855,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  4530,  3403,
    -829,  1329,  1066,   873,  1067,  -829,   875,  3023,  1068,   644,
    1065,  1069,   753,  3855,  3855,  3855,  -829,   877,  -829,   881,
     886,  -829,  3855,  1121,  1183,   891,  -829,  -829,  -829,  -829,
    -829,   893,  3855,  -829,  3855,  3855,  3855,   895,  -829,    38,
    -829,  1072,  2665,   702,  1075,  1031,   897,  1076,  1077,  3855,
     901,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,   905,
    -829,  -829,  -829,  1010,  -829,  -829,  2665,    -8,  -829,  -829,
    1191,  -829,  1196,  1197,  3855,  1094,  1199,  1200,  -829,  1031,
     465,  3555,  3630,  1936,  -829,   383,  -829,  -829,  -829,  -829,
    3855,    -8,  -829,  -829,  -829,  1083,  1084,  1187,  -829,  -829,
    -829,  -829,  3855,  1087,  -829,  -829,    89,   663,   832,  1159,
    -829,  -829,  1031,  1031,   909,   911,  1207,  1208,  1209,  -829,
    -829,  1097,   282,   781,  1091,   354,  3855,  3855,  3855,  3855,
    3855,  3855,  -829,  -829,  -829,  -829,   367,   386,   394,   408,
     417,   420,   429,   433,   434,   435,   445,   447,  -829,  -829,
     913,  -829,   916,   920,  -829,  1099,  -829,  -829,   922,  -829,
    -829,  -829,  -829,  -829,  1100,  1101,  1102,  -829,  -829,  -829,
    1103,  3855,  1106,  -829,  3023,  1104,   722,  -829,  -829,  1108,
    1153,  1160,  -829,   926,  -829,  3855,  -829,  -829,  1031,  -829,
    -829,   741,  1109,  1111,   -14,  -829,   928,  1051,    -8,    -8,
      -8,  -829,  3855,  -829,  -829,  -829,  1126,  -829,    29,  -829,
    -829,  -829,   389,   199,  -829,   355,  3855,  3855,  3855,  3855,
    3855,  3855,  3855,  3855,  -829,    -8,  1107,  -829,  -829,    -8,
    1115,  3855,  -829,   413,   532,    24,    46,   858,  1116,  1117,
     776,  -829,  -829,  1031,  1031,  1239,  1240,  1243,  -829,  3855,
    3479,  1138,  1140,  -829,   448,   475,   483,   500,   506,   510,
    -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  -829,  -829,  -829,  -829,  1132,  -829,  -829,  -829,
    -829,  -829,  3855,   950,  -829,  -829,  -829,  3855,  3855,  3023,
     770,  -829,  -829,  3855,  -829,  -829,  -829,  -829,    -8,    -8,
    -829,  -829,  -829,  -829,  -829,  3855,  1147,  1252,  1264,  3855,
    3855,  3855,  -829,  -829,  -829,   516,   524,   527,   547,   558,
     567,   576,   581,  -829,  -829,  -829,  3855,  1145,  -829,   117,
    1203,  -829,   255,  -829,  1263,  1265,  1150,  3855,  -829,  1266,
    1267,  1154,  3855,  -829,   230,   234,  1155,  1158,  -829,  -829,
     560,  -829,  -829,  -829,  -829,  -829,   582,  1152,   952,  3855,
    3855,  -829,  -829,  -829,  -829,  -829,  -829,  -829,  1161,  1162,
    -829,  -829,   954,  -829,   779,  1059,  -829,  -829,  3855,  -829,
    -829,   584,   593,   599,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  1164,    -8,  1163,   958,  1165,  1786,  1163,   960,
    1166,  1168,  -829,  1170,  1171,  1173,  -829,  1174,  1274,  1275,
    1178,  3855,  -829,  1279,  1293,  1181,  3855,  -829,  -829,  -829,
     286,  -829,  -829,  -829,  1175,   623,   627,  -829,  -829,  1184,
    -829,    -8,  -829,  -829,  -829,  -829,  -829,  3855,  -829,  -829,
    1163,  -829,  3555,  3630,  2011,  -829,   416,  -829,  1163,  -829,
    -829,  -829,  -829,  -829,  -829,  1185,  1188,  -829,  1192,  1193,
    1194,  -829,  1198,  1163,   962,  -829,  -829,  -829,  -829,  -829,
    1189,  -829,   486,   338,  -829,   406,  3855,  3855,  3855,  3855,
    3855,  3855,  3855,  3855,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  -829,  1163,    -8,  3855,  3855,  3855,  -829,  -829,
    -829,   629,   631,   632,   645,   657,   658,   667,   679,  -829,
    -829,   680,   689,   694,  -829,  -829,  -829,  -829,  -829,  -829,
    -829,  -829,  -829,  -829,  -829
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   419,   420,   421,   423,   424,   429,
     430,   444,   443,   445,   446,     0,   431,   432,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,   422,     0,     0,     0,     0,     0,    97,
       0,     3,     4,    98,    99,   106,   107,   108,   109,   100,
     101,   116,     0,   117,   118,   119,   105,     0,   322,   324,
     342,   344,   347,   349,   352,   354,   356,   359,   366,   371,
     373,   387,   415,     0,   427,   428,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   424,   429,   430,   444,   443,     0,     0,   451,
     369,     0,   424,   429,    36,     0,   416,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   352,     0,   452,   321,     0,    49,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,   145,     0,     0,   110,     0,     0,
     111,     0,     0,     0,     3,     0,   413,   102,   104,   410,
     209,     0,     0,     0,   370,     0,   425,   426,   368,   367,
     424,   447,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     316,   317,   318,   319,   320,   314,     0,   393,     0,     0,
     315,     0,     0,     0,     0,     0,    40,   398,     0,   397,
     443,   389,     0,     0,     0,     0,     0,     0,    31,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,    39,
     418,     0,     6,     0,    10,     0,    11,     0,     0,     0,
       0,     0,     0,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,    44,     0,
       0,   123,     0,   135,     0,     0,   154,     0,     0,     0,
       3,   161,    47,     0,     0,    48,    52,     0,     0,   157,
       0,     0,     0,     0,     0,   138,     0,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   411,   412,   414,
       0,     0,   212,     0,     0,     0,   247,   448,     0,     0,
       0,   280,   424,   429,   430,   444,     0,     0,     0,     0,
       0,     0,   279,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     327,   328,   331,   332,   325,   326,     0,   329,   330,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   343,   345,
     346,   348,   350,   351,   353,   355,   357,   358,   363,   364,
     365,   360,   361,   362,   372,   376,   386,   380,   381,   382,
     383,   384,   385,   378,   379,   377,   409,     0,   406,    57,
       0,    91,    92,    93,    94,    95,    54,     0,     0,   392,
       0,     0,    16,     0,     0,     0,     0,   434,   439,   440,
      59,     0,   401,     0,     0,     0,   390,     0,   388,   418,
       0,     0,     0,   316,   317,   318,   319,   320,   314,   315,
      38,     0,   417,     0,     0,     0,   421,   443,     0,     0,
       0,     0,    28,     0,    29,     0,    30,     0,   164,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,     0,     0,   126,   448,
       0,     0,   350,   351,   259,   260,     0,   273,   275,   277,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   448,     0,   448,     0,     0,     0,     0,   112,
     113,   114,   115,   313,   103,     0,     0,   212,     0,     0,
       0,     0,     0,     0,   449,     0,     0,   375,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,   307,
       0,     0,   308,     0,     0,     0,     0,     0,     0,     0,
       0,   281,    61,    63,    65,    68,    66,    67,    62,    64,
      74,    76,    78,    81,    79,    80,    75,    77,     0,     0,
     402,     0,     0,     0,     0,   171,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,   396,     0,
       0,   251,     0,     0,     0,     0,   434,   439,   440,    37,
       5,     0,     0,    12,     0,     0,     0,     0,    21,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,    85,    84,    88,    86,    87,    83,    42,     0,
      89,    90,    45,   120,   125,   124,     0,     0,   134,   151,
       0,   153,     0,     0,     0,   262,     0,     0,   160,     0,
       0,     0,     0,     0,   172,     0,    50,    51,    33,    32,
       0,     0,   155,   441,   442,     0,     0,   136,   139,   140,
     143,   144,     0,     0,   210,   211,     0,     0,     0,     0,
     208,   169,     0,     0,     0,     0,     0,     0,     0,   246,
     450,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,   297,   304,   305,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   323,   408,
       0,   405,     0,     0,    56,     0,    58,   170,     0,    18,
      17,   437,   436,   435,     0,     0,     0,   399,   394,   395,
       0,     0,     0,   391,     0,     0,     0,   256,   255,     0,
       0,     0,    23,     0,    24,     0,    27,   163,     0,    35,
      34,     0,   450,   450,     0,   127,     0,   128,     0,     0,
       0,   269,     0,   263,   264,   159,   265,   261,     0,   274,
     276,   278,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,    72,    73,     0,
       0,     0,   207,     0,     0,     0,     0,     0,     0,     0,
       0,   168,   166,     0,     0,     0,     0,     0,     9,     0,
       0,   434,   440,   301,     0,     0,     0,     0,     0,     0,
     309,   310,   311,   312,   282,   284,   286,   289,   287,   288,
     283,   285,   407,   404,   403,    55,   450,    69,    70,    71,
     252,   253,     0,     0,    14,    13,    22,     0,     0,     0,
       0,   162,     7,     0,    43,    46,   122,   121,   448,     0,
     132,   152,   149,   150,   271,     0,   266,     0,     0,     0,
       0,     0,   176,   177,   174,     0,     0,     0,     0,     0,
       0,     0,     0,   158,   156,   137,     0,     0,   148,     0,
       3,   242,     0,   239,     0,     0,     0,     0,   218,     0,
       0,     0,     0,   213,     0,     0,     0,     0,   214,   215,
       0,   165,   167,   248,   249,   250,     0,   171,     0,     0,
       0,   291,   293,   296,   294,   295,   292,    19,     0,   450,
     258,   257,     0,    25,     0,   129,   130,   270,     0,   267,
     268,     0,     0,     0,   178,   180,   182,   185,   183,   184,
     179,   181,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   229,     0,     0,     0,   230,     0,     0,     0,
       0,     0,   220,     0,     0,     0,     0,   219,   216,   217,
       0,   245,   300,   299,   170,     0,     0,   254,    15,   450,
       8,     0,   133,   272,   186,   188,   187,     0,   147,   241,
       0,   190,     0,     0,     0,   189,     0,   238,     0,   227,
     228,   222,   225,   226,   221,     0,     0,   231,     0,     0,
       0,   232,     0,     0,     0,   298,   302,   303,    26,   131,
       0,   240,     0,     0,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,   237,   235,   236,   224,   233,
     234,   223,   244,     0,     0,     0,     0,     0,   194,   195,
     192,     0,     0,     0,     0,     0,     0,     0,     0,   243,
     146,     0,     0,     0,   196,   198,   200,   203,   201,   202,
     197,   199,   204,   206,   205
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -829,  -829,     3,  -829,  -519,  -677,  -829,  -829,  -829,  -829,
    -829,   258,  -829,  -829,  -829,  -829,  -829,  -298,  -828,  -829,
    -829,  -829,   744,  -829,  -829,  -468,   827,  -350,   200,  1060,
      -2,  -216,  -829,   849,  1098,  -829,   671,   -16,  1088,  1090,
     869,    18,  -829,  1195,  -247,  -829,  -829,   311,  -829
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   195,    62,    63,   528,    64,    65,    66,    67,
      68,   947,    69,    70,   547,    71,    72,   331,   951,    73,
      74,   361,   568,    75,   573,   273,   499,   541,   119,   172,
     126,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,   193,   446,    90,    91,   127,    92
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      77,   448,   508,     2,   406,   639,   815,   684,   643,   565,
     144,   645,    24,   570,   534,   535,   145,   566,   536,   104,
     136,   138,   140,   142,   143,   109,   953,   148,   150,   152,
     695,   155,     3,   120,   159,   161,   106,   105,   954,   926,
     364,   692,   176,   800,   693,   179,   955,   710,   711,   736,
     737,   738,   801,   108,   107,   534,   535,   324,   192,   536,
     959,   534,   535,   306,   121,   536,   307,   308,   960,   184,
     537,   538,   539,   291,   309,   188,   189,   310,   311,   267,
     236,   312,   268,   236,   727,   728,   316,   292,   729,   365,
     165,   696,   697,   956,   250,   251,   252,   253,   254,   255,
     927,   928,   260,   262,   317,   264,   265,   325,   166,   739,
      61,   537,   538,   539,   587,   961,   192,   537,   538,   539,
      61,   534,   535,   287,   269,   536,    61,   534,   535,   567,
     644,   536,   588,   305,   540,   315,    61,   320,   957,   322,
     920,   110,  1041,   111,   712,   171,   727,   728,   234,   235,
     729,   153,   958,   772,   156,   154,   344,   666,   346,   182,
     962,   351,   730,   352,   353,   313,   354,   355,   314,   356,
     357,   358,   356,   359,   963,   569,   183,   537,   538,   539,
     690,   665,   163,   537,   538,   539,  1059,    61,   318,   236,
    1067,   319,   167,   383,   384,   385,   386,   387,   388,   389,
     390,   391,    76,   392,   393,   394,   395,   396,   397,   398,
     399,   316,   327,   328,   157,   461,   167,   168,   422,   423,
     180,   734,   735,   589,   852,   316,   181,   146,   147,   317,
     290,   462,  1091,    61,   463,   169,   457,   186,   162,   164,
    1105,   733,   698,   317,  1028,   177,   178,  1014,  1033,   333,
     187,   458,  1029,   236,   590,  1112,  1034,   460,   428,   429,
     430,   431,   432,   433,   434,   534,   535,   334,   652,   536,
     348,   731,   591,   480,   276,   482,   285,   483,   484,   485,
     486,   487,   488,   489,   653,  1129,   349,   654,   492,   493,
     185,   495,   277,   500,   275,   916,   534,   535,   301,  1030,
     536,   233,   302,  1035,   513,   514,   515,   516,   517,   518,
     519,   532,   533,   318,   522,   523,   319,   146,  1052,   170,
     753,   537,   538,   539,   128,   129,   130,   318,   131,   194,
     319,   543,   544,   542,   933,   545,   546,   340,   246,   549,
     321,   551,   323,   326,  1031,   132,   555,   556,  1036,   237,
     133,   335,   537,   538,   539,   341,   170,   474,  1032,   345,
     342,   347,  1037,   592,   316,   329,   507,   807,   104,   593,
     330,   330,   594,   595,   582,   583,   584,   586,   343,   333,
     596,   333,   317,   597,   598,  1018,   579,   599,   600,   746,
     656,   293,   307,   747,   247,    76,   869,   334,   248,   334,
     748,   825,   768,   749,   750,   294,   110,   751,   111,   836,
     249,   870,   837,   838,   258,   929,  1083,   295,   307,   930,
     839,   256,   274,   840,   841,   263,   309,   842,   843,   310,
     311,   296,   333,   931,   861,   862,   288,   501,   266,   110,
     106,   111,  1096,   134,   278,  1097,  1098,   332,   534,   535,
     334,   502,   536,  1099,   754,   624,  1100,  1101,   580,   628,
    1102,  1103,   630,   631,   231,   232,   318,    94,    95,   319,
     503,   213,   641,  1119,   275,   826,   827,   492,   601,   828,
     854,   335,   470,   335,   504,   110,   505,   111,   755,   873,
     934,   313,   651,   300,   314,   559,   192,   103,   752,   661,
     506,   663,   880,   664,   537,   538,   539,   667,   668,   332,
     911,   213,  1115,   303,   560,   307,  1116,   313,   844,   336,
     314,   881,   561,   309,   932,   687,   310,   311,   337,   882,
    1117,   829,   830,   831,   335,   196,   562,   338,   197,   198,
     705,  1120,   949,   883,   213,   610,   199,   950,   611,   200,
     201,  1104,   884,   202,   203,   885,   723,   612,   238,   239,
     564,   613,   614,   615,   886,   971,   972,   360,   887,   888,
     889,   367,   368,   616,   741,   617,   671,   743,   744,   745,
     890,   289,   891,   981,   290,   756,   757,   658,   758,   759,
     659,   760,   761,   762,   763,   764,   765,   766,   767,  1015,
     339,   205,  1019,   673,   206,   207,   240,   241,   242,   363,
     982,   674,   208,   970,   313,   209,   210,   314,   983,   211,
     212,  1118,   243,   244,   245,   520,   678,   362,   675,   290,
     524,   784,   785,   786,   676,   984,   531,   204,   677,   682,
     790,   985,   290,   371,   610,   986,   214,   215,   216,   217,
     795,  1004,   611,   797,   798,   612,   218,   219,   220,  1005,
     803,   952,  1006,   297,   298,   299,   950,   811,    94,    95,
     279,   280,   281,   282,   283,   613,   466,   435,   467,   575,
     221,   222,  1007,   284,   816,   436,   614,   437,   438,  1040,
    1084,   366,   821,  1008,   950,   615,   439,   440,   103,   832,
     833,   835,  1009,   213,   616,   441,   442,   451,   845,   617,
     776,  1010,   671,   443,   444,   445,  1011,  1042,   452,  1054,
     850,   673,   853,   449,   450,   683,   685,   677,  1055,   688,
     689,   691,   453,    93,  1056,    94,    95,    96,    97,    98,
      99,   100,    76,   454,   874,   875,   876,   877,   878,   879,
     101,   897,   717,   718,   719,   899,   455,   610,  1086,   611,
     612,   623,  1087,   102,  1134,   103,  1135,  1136,   435,   626,
     456,   447,   780,   613,   629,   290,   436,   632,   437,   438,
    1137,   466,   637,   646,   640,   614,   615,   439,   440,   901,
     435,   468,  1138,  1139,   469,   616,   441,   442,   436,   471,
     437,   438,  1140,   470,   443,   444,   445,   617,   671,   439,
     440,   472,  -451,  -451,  1141,  1142,   473,   673,   441,   442,
     924,   475,   677,   670,  1143,   490,   443,   444,   445,  1144,
     804,   509,   679,   805,   935,   936,   937,   938,   939,   940,
     941,   942,   855,   369,   370,   856,   857,   858,   859,   948,
     905,  -451,  -451,   290,  -451,  -451,  -451,  -451,  -451,  -451,
    -451,  -451,  -451,  -451,  -451,  -451,  -451,   976,   964,   912,
    -451,   965,   913,   966,   967,   415,   416,   417,  -451,  -451,
    -451,  -451,  -451,  -451,   290,  -451,   783,   817,  -451,  -451,
    -451,  -451,  -451,  -451,   481,  -451,  -451,  -451,   993,  -451,
     988,   290,   419,   420,   421,   990,   991,  1050,   459,   290,
     290,   846,   466,   510,   871,   553,    94,    95,   279,   280,
     281,   282,   283,   997,   464,   465,   548,  1001,  1002,  1003,
     770,   284,   773,   530,   476,   477,   478,   479,   778,   491,
     290,   511,   370,   550,  1012,   557,   103,  -374,  -374,   367,
     512,   367,   521,  1017,   571,  1023,   525,   368,   526,   527,
    1027,   529,   368,   552,   368,   796,   558,    93,   559,    94,
      95,    96,    97,    98,    99,   100,   560,  1045,  1046,   561,
     554,   368,   367,   627,   101,   562,   223,   224,   225,   226,
     227,   228,   699,   700,   732,   700,  1053,   578,   229,   103,
     563,   740,   290,   775,   290,   777,   290,   787,   290,   577,
     230,   788,   477,   572,   917,  1066,   789,   290,   921,   922,
     923,   793,   477,   367,   794,   799,   659,   808,   700,  1078,
     602,   812,   290,   576,  1082,   813,   290,   814,   527,   863,
     700,   864,   700,   892,   290,   943,   893,   477,   581,   945,
     894,   290,   896,   290,   230,   948,   367,   909,   918,   368,
    1092,  1093,  1095,   400,   401,   402,   403,   404,   405,   603,
     407,   408,   409,   410,   411,   412,   413,   414,   919,   527,
     989,   290,  1044,   290,  1049,   290,  1051,   527,  1060,   700,
    1068,   700,  1113,   700,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,   604,   173,   175,   903,   605,   426,   427,   606,
     607,   608,   609,  1131,  1132,  1133,   910,   610,   995,   996,
     611,   612,     4,     5,     6,     7,   613,   618,   614,   122,
     123,   114,   115,   270,    13,    14,    15,    16,    17,    18,
     615,   616,   617,   236,   619,   620,   621,   633,   634,   635,
     117,    28,   655,   642,   647,   648,   649,   650,   660,   669,
     694,    34,   714,   724,   671,   330,   672,   673,   674,    42,
      43,    44,   675,   713,    47,   676,   677,   680,   681,   715,
     706,   978,   707,    51,    52,   708,   709,   722,   716,    53,
     725,   720,   721,   742,   774,   776,   779,   792,   781,   791,
     802,   818,   782,   806,   809,   810,   819,   820,   822,   823,
     824,   847,   848,  1058,   849,   851,   860,    76,    55,    56,
     992,   865,   866,   867,   994,   868,   872,   895,   897,   898,
     899,   907,   904,   900,    57,   902,   906,   914,   908,   915,
     925,    58,   944,   946,   968,   969,   973,   974,   118,   271,
     975,  1089,   979,    61,   980,   272,     4,     5,     6,     7,
     987,   998,   999,   122,   123,   114,   115,   270,    13,    14,
      15,    16,    17,    18,  1000,  1013,  1016,  1020,  1022,  1021,
    1024,  1025,  1026,  1038,   117,    28,  1039,  1043,  1075,  1076,
    1048,  1047,  1057,  1079,  1069,    34,  1070,   950,  1071,  1072,
    1061,  1073,  1074,    42,    43,    44,  1077,  1080,    47,  1081,
    1085,   726,  1088,  1106,  1130,  1090,  1107,    51,    52,  1114,
    1108,  1109,  1110,    53,   424,   657,  1111,   425,   418,   304,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   270,    13,    14,    15,    16,    17,    18,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,    57,     0,
       0,    34,     0,     0,     0,    58,     0,     0,     0,    42,
      43,    44,   118,   638,    47,     0,     0,    61,     0,   272,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   270,
      13,    14,    15,    16,    17,    18,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,    57,     0,     0,    34,     0,     0,
       0,    58,     0,     0,     0,    42,    43,    44,   118,   771,
      47,     0,     0,    61,     0,   272,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,    58,     0,     0,
       0,     0,     0,     0,   118,     4,     5,     6,     7,    61,
       0,   272,   372,   373,   374,   375,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   376,   377,    24,     0,
       0,    25,    26,    27,    28,   378,   379,    31,    32,    33,
       0,     0,     0,     0,    34,    35,    36,    37,    38,    39,
     380,   381,    42,    43,    44,    45,    46,    47,    48,     0,
       0,     0,    49,    50,     0,     0,    51,    52,     0,     0,
       0,     0,    53,     0,     0,     0,    54,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    55,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,    58,     0,     0,     0,     0,     0,
      59,    60,     4,     5,     6,     7,    61,   382,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   701,   702,    24,     0,     0,    25,    26,
      27,    28,    29,   703,    31,    32,    33,     0,     0,     0,
       0,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,     0,     0,     0,    49,
      50,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,    54,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,    58,     0,     0,     0,     0,     0,    59,    60,     4,
       5,     6,     7,    61,   704,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    1062,  1063,    24,     0,     0,    25,    26,    27,    28,    29,
    1064,    31,    32,    33,     0,     0,     0,     0,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,     0,     0,     0,    49,    50,     0,     0,
      51,    52,     0,     0,     0,     0,    53,     0,     0,     0,
      54,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   122,   123,   114,   115,   116,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    55,    56,     0,     0,     0,
       0,     0,   117,    28,     0,     0,     0,     0,     0,     0,
       0,    57,     0,    34,     0,     0,     0,     0,    58,     0,
       0,    42,    43,    44,    59,    60,    47,     0,     0,     0,
      61,  1065,     0,     0,     0,    51,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   122,   123,   114,   115,
     116,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      55,    56,     0,     0,     0,     0,     0,   117,    28,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    34,     0,
       0,     0,     0,    58,     0,     0,    42,    43,    44,   149,
     118,    47,     0,     0,     0,    61,   585,     0,     0,     0,
      51,    52,     0,     0,     0,     0,    53,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   122,   123,   114,   115,   116,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    55,    56,     0,     0,     0,
       0,     0,   117,    28,     0,     0,     0,     0,     0,     0,
       0,    57,     0,    34,     0,     0,     0,     0,    58,     0,
       0,    42,    43,    44,   149,   118,    47,     0,     0,     0,
      61,   834,     0,     0,     0,    51,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,   149,
     118,     4,     5,     6,     7,    61,  1094,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,     0,     0,    25,    26,    27,
      28,    29,    30,    31,    32,    33,     0,     0,     0,     0,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,     0,     0,     0,    49,    50,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,    54,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    34,     0,     0,     0,     0,
      58,     0,     0,    42,    43,    44,    59,    60,    47,     0,
       0,     0,    61,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   122,   123,
     114,   115,   116,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,   117,
      28,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      34,     0,     0,     0,     0,    58,     0,     0,    42,    43,
      44,   124,   125,    47,     0,     0,     0,    61,     0,     0,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    34,     0,     0,     0,     0,
      58,     0,     0,    42,    43,    44,   149,   118,    47,     0,
       0,     0,    61,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   190,   123,
     114,   115,   116,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,   117,
      28,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      34,     0,     0,     0,     0,    58,     0,     0,    42,    43,
      44,   151,   118,    47,     0,     0,     0,    61,     0,     0,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   122,   123,   114,   115,   116,    13,
      14,    15,    16,    17,    18,     0,     0,    55,    56,     0,
       0,     0,     0,     0,     0,   117,    28,     0,     0,     0,
       0,     0,     0,    57,     0,     0,    34,     0,     0,     0,
      58,     0,     0,     0,    42,    43,    44,   118,   191,    47,
       0,     0,    61,     0,     0,     0,     0,     0,    51,    52,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,    55,    56,     0,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,    57,
       0,     0,    34,     0,     0,     0,    58,     0,     0,     0,
      42,    43,    44,   118,   257,    47,     0,     0,    61,     0,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,   261,     0,    42,    43,    44,     0,   118,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   122,   123,   114,   115,   116,    13,    14,    15,    16,
      17,    18,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,   117,    28,     0,     0,     0,     0,     0,     0,
      57,     0,     0,    34,     0,     0,     0,    58,     0,     0,
       0,    42,    43,    44,   118,   191,    47,     0,     0,    61,
       0,     0,     0,     0,     0,    51,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,     0,     0,     0,
       0,     0,     0,    58,     0,     0,     0,     0,     0,     0,
     118,   286,     0,     0,     0,    61,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,   350,     0,     0,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,    42,    43,    44,     0,     0,    47,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   122,   123,
     114,   115,   116,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,   117,
      28,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      34,     0,     0,     0,     0,    58,     0,     0,    42,    43,
      44,     0,   118,    47,     0,     0,     0,    61,     0,     0,
       0,     0,    51,    52,     0,     0,     0,     0,    53,     0,
       0,   194,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    55,    56,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,     0,    57,     0,    34,     0,     0,     0,     0,
      58,     0,     0,    42,    43,    44,     0,   118,    47,     0,
       0,     0,    61,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,    57,     0,
       0,    34,     0,     0,     0,    58,     0,     0,     0,    42,
      43,    44,   118,   574,    47,     0,     0,    61,     0,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,    57,     0,     0,    34,     0,     0,
       0,    58,     0,     0,     0,    42,    43,    44,   118,   622,
      47,     0,     0,    61,     0,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   122,   123,   114,   115,   116,    13,    14,    15,    16,
      17,    18,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,   117,    28,     0,     0,     0,     0,     0,     0,
      57,     0,     0,    34,     0,     0,     0,    58,     0,     0,
       0,    42,    43,    44,   118,   625,    47,     0,     0,    61,
       0,     0,     0,     0,     0,    51,    52,     0,     0,     0,
       0,    53,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,   496,     7,     0,     0,     0,   122,   123,   114,
     115,   497,    13,    14,    15,    16,    17,    18,     0,     0,
      55,    56,     0,     0,     0,     0,     0,     0,   117,    28,
       0,     0,     0,     0,     0,     0,    57,     0,     0,    34,
       0,     0,     0,    58,     0,     0,     0,    42,    43,    44,
     118,   636,    47,     0,     0,    61,     0,     0,     0,     0,
       0,    51,    52,     0,     0,     0,     0,    53,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   122,   123,   114,   115,   116,    13,    14,
      15,    16,    17,    18,     0,     0,    55,    56,     0,     0,
       0,     0,     0,     0,   117,    28,     0,     0,     0,     0,
       0,     0,    57,     0,     0,    34,     0,     0,     0,    58,
       0,     0,     0,    42,    43,    44,   118,   191,    47,     0,
       0,    61,     0,     0,     0,     0,     0,    51,    52,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   122,
     123,   114,   115,   116,    13,    14,    15,    16,    17,    18,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
     117,    28,     0,     0,     0,     0,     0,     0,    57,     0,
       0,    34,     0,     0,     0,    58,     0,     0,     0,    42,
      43,    44,   118,   769,    47,     0,     0,    61,     0,     0,
       0,     0,     0,    51,    52,     0,     0,     0,     0,    53,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,    55,    56,
       0,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,    57,     0,     0,    34,     0,     0,
       0,    58,     0,     0,     0,    42,    43,    44,   118,   977,
      47,     0,     0,    61,     0,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   135,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   137,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   139,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   141,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   118,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   158,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   160,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   118,
      47,     0,     0,     0,   174,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   259,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
     496,     7,     0,     0,     0,   122,   123,   114,   115,   497,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   494,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     122,   123,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,    28,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   498,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   122,   123,   114,   115,   116,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,   117,    28,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    34,     0,     0,
       0,     0,    58,     0,     0,    42,    43,    44,     0,   662,
      47,     0,     0,     0,    61,     0,     0,     0,     0,    51,
      52,     0,     0,     0,     0,    53,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     112,   113,   114,   115,   116,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,   117,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    34,     0,     0,     0,     0,    58,     0,     0,
      42,    43,    44,     0,   686,    47,     0,     0,     0,    61,
       0,     0,     0,     0,    51,    52,     0,     0,     0,     0,
      53,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,    58,     0,     0,     0,     0,     0,     0,   118,
       0,     0,     0,     0,    61
};

static const yytype_int16 yycheck[] =
{
       2,   248,   300,     0,   220,   473,   683,   526,    14,    14,
      26,   479,    26,   363,    10,    11,    14,    22,    14,   114,
      22,    23,    24,    25,    26,    11,   854,    29,    30,    31,
      10,    33,     0,    15,    36,    37,   116,   132,    14,    10,
      14,    81,    44,     5,    81,    47,    22,    38,    39,    75,
      76,    77,    14,   114,   134,    10,    11,    81,    60,    14,
      14,    10,    11,    26,    14,    14,    29,    30,    22,    51,
      66,    67,    68,   114,    37,    57,    58,    40,    41,    14,
     120,    44,    17,   120,    57,    58,    26,   128,    61,    63,
      26,    71,    72,    69,    96,    97,    98,    99,   100,   101,
      71,    72,   104,   105,    44,   107,   108,   131,    44,   135,
     134,    66,    67,    68,    26,    69,   118,    66,    67,    68,
     134,    10,    11,   125,    59,    14,   134,    10,    11,   134,
     136,    14,    44,   135,   130,   137,   134,   139,   114,   141,
     817,   127,   970,   129,   135,   134,    57,    58,    79,    80,
      61,    10,   128,   621,    12,    14,   158,   507,   160,    14,
     114,   163,   135,   165,   166,   128,   168,   169,   131,   171,
     172,   173,   174,   175,   128,   130,    31,    66,    67,    68,
      81,   130,   129,    66,    67,    68,  1014,   134,   128,   120,
    1018,   131,   128,   195,   196,   197,   198,   199,   200,   201,
     202,   203,     2,   205,   206,   207,   208,   209,   210,   211,
     212,    26,    79,    80,    12,   112,   128,    26,   234,   235,
      14,   571,   572,   135,   135,    26,    14,    27,    28,    44,
     131,   128,  1060,   134,   131,    44,   114,    10,    38,    39,
    1068,   130,   540,    44,    14,    45,    46,   130,    14,    26,
      10,   129,    22,   120,    26,  1083,    22,   259,   240,   241,
     242,   243,   244,   245,   246,    10,    11,    44,   112,    14,
      29,   569,    44,   275,   116,   277,   114,   279,   280,   281,
     282,   283,   284,   285,   128,  1113,    45,   131,   290,   291,
      14,   293,   134,   295,   132,   814,    10,    11,    10,    69,
      14,   106,    14,    69,   306,   307,   308,   309,   310,   311,
     312,   327,   328,   128,   316,   317,   131,   117,   995,   128,
     135,    66,    67,    68,    10,    11,    12,   128,    14,    73,
     131,   333,   334,   330,   135,   337,   338,   116,   113,   341,
     140,   343,   142,   143,   114,    31,   348,   349,   114,   111,
      36,   128,    66,    67,    68,   134,   128,    78,   128,   159,
     116,   161,   128,   135,    26,   129,   129,   665,   114,    26,
     134,   134,    29,    30,   376,   377,   378,   379,   134,    26,
      37,    26,    44,    40,    41,   130,   132,    44,    45,    26,
      78,   114,    29,    30,   127,   195,   114,    44,   127,    44,
      37,   699,   618,    40,    41,   128,   127,    44,   129,    26,
      14,   129,    29,    30,   103,    26,   130,   114,    29,    30,
      37,    14,   111,    40,    41,    14,    37,    44,    45,    40,
      41,   128,    26,    44,   732,   733,   125,   114,   128,   127,
     116,   129,    26,   129,   128,    29,    30,   128,    10,    11,
      44,   128,    14,    37,   135,   457,    40,    41,   134,   461,
      44,    45,   464,   465,   118,   119,   128,    99,   100,   131,
     114,   128,   474,   135,   132,    10,    11,   479,   135,    14,
     727,   128,   128,   128,   128,   127,   114,   129,   135,   135,
     135,   128,   494,    14,   131,   128,   498,   129,   135,   501,
     128,   503,   135,   505,    66,    67,    68,   509,   510,   128,
     808,   128,    26,    10,   128,    29,    30,   128,   135,   128,
     131,   135,   128,    37,   135,   527,    40,    41,   114,   135,
      44,    66,    67,    68,   128,    26,   128,   114,    29,    30,
     542,   135,   129,   135,   128,   128,    37,   134,   128,    40,
      41,   135,   135,    44,    45,   135,   558,   128,   121,   122,
     360,   128,   128,   128,   135,   863,   864,    54,   135,   135,
     135,   130,   131,   128,   576,   128,   128,   579,   580,   581,
     135,   128,   135,   135,   131,   587,   588,   128,   590,   591,
     131,   593,   594,   595,   596,   597,   598,   599,   600,   949,
     134,    26,   952,   128,    29,    30,   107,   108,   109,   129,
     135,   128,    37,   860,   128,    40,    41,   131,   135,    44,
      45,   135,   123,   124,   125,   314,   128,   134,   128,   131,
     319,   633,   634,   635,   128,   135,   325,   128,   128,   128,
     642,   135,   131,   135,   128,   135,    82,    83,    84,    85,
     652,   135,   128,   655,   656,   128,    92,    93,    94,   135,
     662,   129,   135,    10,    11,    12,   134,   669,    99,   100,
     101,   102,   103,   104,   105,   128,   131,    14,   133,   368,
     116,   117,   135,   114,   686,    22,   128,    24,    25,   129,
    1040,   134,   694,   135,   134,   128,    33,    34,   129,   701,
     702,   703,   135,   128,   128,    42,    43,   128,   710,   128,
     128,   135,   128,    50,    51,    52,   135,   135,   128,   135,
     722,   128,    59,   128,   129,   525,   526,   128,   135,   529,
     530,   531,   128,    97,   135,    99,   100,   101,   102,   103,
     104,   105,   542,   128,   746,   747,   748,   749,   750,   751,
     114,   128,   552,   553,   554,   128,   128,   128,   135,   128,
     128,   450,   135,   127,   135,   129,   135,   135,    14,   458,
     128,    17,   128,   128,   463,   131,    22,   466,    24,    25,
     135,   131,   471,   133,   473,   128,   128,    33,    34,   791,
      14,   117,   135,   135,   135,   128,    42,    43,    22,   129,
      24,    25,   135,   128,    50,    51,    52,   128,   128,    33,
      34,    14,    40,    41,   135,   135,   129,   128,    42,    43,
     822,    14,   128,   512,   135,   128,    50,    51,    52,   135,
     128,   114,   521,   131,   836,   837,   838,   839,   840,   841,
     842,   843,    10,   130,   131,    13,    14,    15,    16,   851,
     128,    79,    80,   131,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,   869,    10,   128,
      98,    13,   131,    15,    16,     7,     8,     9,   106,   107,
     108,   109,   110,   111,   131,   113,   133,   687,   116,   117,
     118,   119,   120,   121,    14,   123,   124,   125,   128,   127,
     902,   131,   231,   232,   233,   907,   908,   128,   130,   131,
     131,   711,   131,   114,   133,    27,    99,   100,   101,   102,
     103,   104,   105,   925,   121,   122,    14,   929,   930,   931,
     619,   114,   621,    10,   130,   131,   130,   131,   627,   130,
     131,   130,   131,    14,   946,    10,   129,   130,   131,   130,
     131,   130,   131,   950,   129,   957,   130,   131,    27,    28,
     962,   130,   131,   130,   131,   654,   128,    97,   128,    99,
     100,   101,   102,   103,   104,   105,   128,   979,   980,   128,
     130,   131,   130,   131,   114,   128,    86,    87,    88,    89,
      90,    91,   130,   131,   130,   131,   998,   127,    98,   129,
     135,   130,   131,   130,   131,   130,   131,   130,   131,    10,
     110,   130,   131,   129,   814,  1017,   130,   131,   818,   819,
     820,   130,   131,   130,   131,   130,   131,   130,   131,  1031,
     128,   130,   131,   114,  1036,   130,   131,    27,    28,   130,
     131,   130,   131,   130,   131,   845,   130,   131,   114,   849,
     130,   131,   130,   131,   110,  1057,   130,   131,   130,   131,
    1062,  1063,  1064,   214,   215,   216,   217,   218,   219,   128,
     221,   222,   223,   224,   225,   226,   227,   228,    27,    28,
     130,   131,   130,   131,   130,   131,    27,    28,   130,   131,
     130,   131,   130,   131,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,   128,    43,    44,   794,   128,   238,   239,   128,
     128,   128,   128,  1115,  1116,  1117,   805,   128,   918,   919,
     128,   128,     3,     4,     5,     6,   128,    95,   128,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     128,   128,   128,   120,   129,    14,   129,   114,   114,   114,
      31,    32,    78,   129,   117,   135,   128,   128,   128,   114,
     114,    42,   135,    14,   128,   134,   128,   128,   128,    50,
      51,    52,   128,   117,    55,   128,   128,   128,   128,   117,
     128,   870,   128,    64,    65,   128,   128,   114,   135,    70,
      14,   128,   128,    14,   128,   128,   128,    14,   133,    78,
     128,    10,   133,   128,   128,   128,    10,    10,   114,    10,
      10,   128,   128,  1013,    27,   128,    57,  1017,    99,   100,
     909,    14,    14,    14,   913,   128,   135,   128,   128,   128,
     128,    78,   128,   130,   115,   129,   128,   128,    78,   128,
     114,   122,   135,   128,   128,   128,     7,     7,   129,   130,
       7,  1051,   114,   134,   114,   136,     3,     4,     5,     6,
     128,   114,    10,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    10,   130,    73,    14,   128,    14,
      14,    14,   128,   128,    31,    32,   128,   135,    14,    14,
     128,   130,   128,    14,   128,    42,   128,   134,   128,   128,
     135,   128,   128,    50,    51,    52,   128,    14,    55,   128,
     135,   567,   128,   128,  1114,  1057,   128,    64,    65,   130,
     128,   128,   128,    70,   236,   498,   128,   237,   230,   134,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,
      51,    52,   129,   130,    55,    -1,    -1,   134,    -1,   136,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    50,    51,    52,   129,   130,
      55,    -1,    -1,   134,    -1,   136,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    -1,    -1,    -1,   129,     3,     4,     5,     6,   134,
      -1,   136,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    -1,
      -1,    -1,    60,    61,    -1,    -1,    64,    65,    -1,    -1,
      -1,    -1,    70,    -1,    -1,    -1,    74,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,
     128,   129,     3,     4,     5,     6,   134,   135,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    -1,    -1,    -1,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    -1,    -1,    -1,    60,
      61,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    -1,    -1,   128,   129,     3,
       4,     5,     6,   134,   135,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    -1,    -1,    -1,    -1,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    60,    61,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      74,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    50,    51,    52,   128,   129,    55,    -1,    -1,    -1,
     134,   135,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    50,    51,    52,   128,
     129,    55,    -1,    -1,    -1,   134,   135,    -1,    -1,    -1,
      64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,
      -1,    50,    51,    52,   128,   129,    55,    -1,    -1,    -1,
     134,   135,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,   128,
     129,     3,     4,     5,     6,   134,   135,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    60,    61,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    74,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    50,    51,    52,   128,   129,    55,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,
      52,   128,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    50,    51,    52,   128,   129,    55,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,
      52,   128,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,    -1,
     122,    -1,    -1,    -1,    50,    51,    52,   129,   130,    55,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    64,    65,
      -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,
      -1,    -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,
      50,    51,    52,   129,   130,    55,    -1,    -1,   134,    -1,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,   123,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    42,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    50,    51,    52,   129,   130,    55,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,    -1,    -1,    -1,   134,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      42,    -1,    -1,    -1,    -1,   122,    -1,    -1,    50,    51,
      52,    -1,   129,    55,    -1,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,
      -1,    73,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    99,   100,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   115,    -1,    42,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    50,    51,    52,    -1,   129,    55,    -1,
      -1,    -1,   134,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,
      51,    52,   129,   130,    55,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    50,    51,    52,   129,   130,
      55,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,    42,    -1,    -1,    -1,   122,    -1,    -1,
      -1,    50,    51,    52,   129,   130,    55,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      99,   100,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    42,
      -1,    -1,    -1,   122,    -1,    -1,    -1,    50,    51,    52,
     129,   130,    55,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      -1,    64,    65,    -1,    -1,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    99,   100,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,    42,    -1,    -1,    -1,   122,
      -1,    -1,    -1,    50,    51,    52,   129,   130,    55,    -1,
      -1,   134,    -1,    -1,    -1,    -1,    -1,    64,    65,    -1,
      -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    99,   100,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    42,    -1,    -1,    -1,   122,    -1,    -1,    -1,    50,
      51,    52,   129,   130,    55,    -1,    -1,   134,    -1,    -1,
      -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    99,   100,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    42,    -1,    -1,
      -1,   122,    -1,    -1,    -1,    50,    51,    52,   129,   130,
      55,    -1,    -1,   134,    -1,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    42,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    50,    51,    52,    -1,   129,
      55,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,    64,
      65,    -1,    -1,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    42,    -1,    -1,    -1,    -1,   122,    -1,    -1,
      50,    51,    52,    -1,   129,    55,    -1,    -1,    -1,   134,
      -1,    -1,    -1,    -1,    64,    65,    -1,    -1,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    99,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,   134
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   138,   139,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    60,
      61,    64,    65,    70,    74,    99,   100,   115,   122,   128,
     129,   134,   140,   141,   143,   144,   145,   146,   147,   149,
     150,   152,   153,   156,   157,   160,   165,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     182,   183,   185,    97,    99,   100,   101,   102,   103,   104,
     105,   114,   127,   129,   114,   132,   116,   134,   114,    11,
     127,   129,    10,    11,    12,    13,    14,    31,   129,   165,
     178,    14,    10,    11,   128,   129,   167,   184,    10,    11,
      12,    14,    31,    36,   129,   129,   167,   129,   167,   129,
     167,   129,   167,   167,   174,    14,   165,   165,   167,   128,
     167,   128,   167,    10,    14,   167,    12,    12,   129,   167,
     129,   167,   165,   129,   165,    26,    44,   128,    26,    44,
     128,   134,   166,   166,   134,   166,   167,   165,   165,   167,
      14,    14,    14,    31,   178,    14,    10,    10,   178,   178,
      10,   130,   167,   180,    73,   139,    26,    29,    30,    37,
      40,    41,    44,    45,   128,    26,    29,    30,    37,    40,
      41,    44,    45,   128,    82,    83,    84,    85,    92,    93,
      94,   116,   117,    86,    87,    88,    89,    90,    91,    98,
     110,   118,   119,   106,    79,    80,   120,   111,   121,   122,
     107,   108,   109,   123,   124,   125,   113,   127,   127,    14,
     167,   167,   167,   167,   167,   167,    14,   130,   184,   129,
     167,   123,   167,    14,   167,   167,   128,    14,    17,    59,
      14,   130,   136,   162,   184,   132,   116,   134,   128,   101,
     102,   103,   104,   105,   114,   114,   130,   167,   184,   128,
     131,   114,   128,   114,   128,   114,   128,    10,    11,    12,
      14,    10,    14,    10,   180,   167,    26,    29,    30,    37,
      40,    41,    44,   128,   131,   167,    26,    44,   128,   131,
     167,   165,   167,   165,    81,   131,   165,    79,    80,   129,
     134,   154,   128,    26,    44,   128,   128,   114,   114,   134,
     116,   134,   116,   134,   167,   165,   167,   165,    29,    45,
      23,   167,   167,   167,   167,   167,   167,   167,   167,   167,
      54,   158,   134,   129,    14,    63,   134,   130,   131,   130,
     131,   135,    10,    11,    12,    13,    24,    25,    33,    34,
      48,    49,   135,   167,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   167,   167,
     170,   170,   170,   170,   170,   170,   168,   170,   170,   170,
     170,   170,   170,   170,   170,     7,     8,     9,   171,   173,
     173,   173,   174,   174,   175,   176,   177,   177,   178,   178,
     178,   178,   178,   178,   178,    14,    22,    24,    25,    33,
      34,    42,    43,    50,    51,    52,   181,    17,   181,   128,
     129,   128,   128,   128,   128,   128,   128,   114,   129,   130,
     167,   112,   128,   131,   121,   122,   131,   133,   117,   135,
     128,   129,    14,   129,    78,    14,   130,   131,   130,   131,
     167,    14,   167,   167,   167,   167,   167,   167,   167,   167,
     128,   130,   167,   167,   129,   167,     5,    14,   129,   163,
     167,   114,   128,   114,   128,   114,   128,   129,   154,   114,
     114,   130,   131,   167,   167,   167,   167,   167,   167,   167,
     184,   131,   167,   167,   184,   130,    27,    28,   142,   130,
      10,   184,   174,   174,    10,    11,    14,    66,    67,    68,
     130,   164,   139,   167,   167,   167,   167,   151,    14,   167,
      14,   167,   130,    27,   130,   167,   167,    10,   128,   128,
     128,   128,   128,   135,   165,    14,    22,   134,   159,   130,
     164,   129,   129,   161,   130,   184,   114,    10,   127,   132,
     134,   114,   167,   167,   167,   135,   167,    26,    44,   135,
      26,    44,   135,    26,    29,    30,    37,    40,    41,    44,
      45,   135,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,    95,   129,
      14,   129,   130,   184,   167,   130,   184,   131,   167,   184,
     167,   167,   184,   114,   114,   114,   130,   184,   130,   162,
     184,   167,   129,    14,   136,   162,   133,   117,   135,   128,
     128,   167,   112,   128,   131,    78,    78,   163,   128,   131,
     128,   167,   129,   167,   167,   130,   164,   167,   167,   114,
     184,   128,   128,   128,   128,   128,   128,   128,   128,   184,
     128,   128,   128,   165,   141,   165,   129,   167,   165,   165,
      81,   165,    81,    81,   114,    10,    71,    72,   154,   130,
     131,    24,    25,    34,   135,   167,   128,   128,   128,   128,
      38,    39,   135,   117,   135,   117,   135,   165,   165,   165,
     128,   128,   114,   167,    14,    14,   159,    57,    58,    61,
     135,   154,   130,   130,   164,   164,    75,    76,    77,   135,
     130,   167,    14,   167,   167,   167,    26,    30,    37,    40,
      41,    44,   135,   135,   135,   135,   167,   167,   167,   167,
     167,   167,   167,   167,   167,   167,   167,   167,   168,   130,
     184,   130,   162,   184,   128,   130,   128,   130,   184,   128,
     128,   133,   133,   133,   167,   167,   167,   130,   130,   130,
     167,    78,    14,   130,   131,   167,   184,   167,   167,   130,
       5,    14,   128,   167,   128,   131,   128,   154,   130,   128,
     128,   167,   130,   130,    27,   142,   167,   165,    10,    10,
      10,   167,   114,    10,    10,   154,    10,    11,    14,    66,
      67,    68,   167,   167,   135,   167,    26,    29,    30,    37,
      40,    41,    44,    45,   135,   167,   165,   128,   128,    27,
     167,   128,   135,    59,   181,    10,    13,    14,    15,    16,
      57,   154,   154,   130,   130,    14,    14,    14,   128,   114,
     129,   133,   135,   135,   167,   167,   167,   167,   167,   167,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   130,   130,   130,   128,   130,   128,   128,   128,
     130,   167,   129,   184,   128,   128,   128,    78,    78,   131,
     184,   154,   128,   131,   128,   128,   141,   165,   130,    27,
     142,   165,   165,   165,   167,   114,    10,    71,    72,    26,
      30,    44,   135,   135,   135,   167,   167,   167,   167,   167,
     167,   167,   167,   165,   135,   165,   128,   148,   167,   129,
     134,   155,   129,   155,    14,    22,    69,   114,   128,    14,
      22,    69,   114,   128,    10,    13,    15,    16,   128,   128,
     181,   154,   154,     7,     7,     7,   167,   130,   184,   114,
     114,   135,   135,   135,   135,   135,   135,   128,   167,   130,
     167,   167,   184,   128,   184,   165,   165,   167,   114,    10,
      10,   167,   167,   167,   135,   135,   135,   135,   135,   135,
     135,   135,   167,   130,   130,   164,    73,   139,   130,   164,
      14,    14,   128,   167,    14,    14,   128,   167,    14,    22,
      69,   114,   128,    14,    22,    69,   114,   128,   128,   128,
     129,   155,   135,   135,   130,   167,   167,   130,   128,   130,
     128,    27,   142,   167,   135,   135,   135,   128,   165,   155,
     130,   135,    24,    25,    34,   135,   167,   155,   130,   128,
     128,   128,   128,   128,   128,    14,    14,   128,   167,    14,
      14,   128,   167,   130,   164,   135,   135,   135,   128,   165,
     148,   155,   167,   167,   135,   167,    26,    29,    30,    37,
      40,    41,    44,    45,   135,   155,   128,   128,   128,   128,
     128,   128,   155,   130,   130,    26,    30,    44,   135,   135,
     135,   167,   167,   167,   167,   167,   167,   167,   167,   155,
     165,   167,   167,   167,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   137,   138,   139,   139,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     142,   142,   142,   142,   143,   143,   144,   144,   144,   144,
     145,   145,   146,   146,   146,   147,   147,   147,   148,   149,
     149,   149,   149,   149,   149,   150,   150,   151,   151,   152,
     152,   152,   152,   152,   152,   152,   152,   152,   152,   152,
     153,   153,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   156,   157,   158,
     158,   158,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   160,   161,   161,   161,
     161,   162,   162,   162,   162,   163,   163,   163,   163,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   166,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   169,   169,   169,   169,   169,
     169,   169,   169,   169,   169,   169,   169,   169,   169,   169,
     169,   169,   169,   170,   170,   171,   171,   171,   172,   172,
     173,   173,   173,   174,   174,   175,   175,   176,   176,   176,
     177,   177,   177,   177,   177,   177,   177,   178,   178,   178,
     178,   178,   179,   179,   180,   180,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   182,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   183,   183,   183,
     183,   183,   183,   183,   183,   183,   184,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     6,
       3,     3,     5,     7,     7,     9,     4,     6,     6,     8,
       5,     5,     7,     6,     6,     8,    10,     6,     4,     4,
       4,     3,     5,     5,     6,     6,     2,     5,     4,     3,
       3,     3,     5,     7,     3,     5,     7,     3,     3,     2,
       5,     5,     3,     2,     4,     7,     6,     4,     6,     4,
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
       4,     3,     5,     5,     7,     3,     3,     5,     5,     1,
       1,     3,     2,     3,     3,     3,     4,     5,     5,     3,
       5,     4,     6,     1,     3,     1,     3,     1,     3,     3,
       3,     4,     6,     6,     6,     6,     6,     6,     6,     6,
       5,     7,     7,     7,     7,     7,     7,     5,     9,     8,
       8,     6,     9,     9,     5,     5,     4,     4,     4,     6,
       6,     6,     6,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     2,     2,     2,
       2,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     3,
       4,     6,     4,     3,     6,     6,     5,     3,     3,     6,
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
#line 396 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 3004 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 406 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3010 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 407 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3016 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 411 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3022 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 413 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3028 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 415 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3034 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 417 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3042 "raku.tab.c"
    break;

  case 9: /* stmt: '(' scalar_list ')' '=' expr ';'  */
#line 421 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3048 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 423 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3054 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 425 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3060 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 427 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3066 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 429 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3074 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 433 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3081 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 436 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3089 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 440 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3095 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 442 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3103 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 446 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3110 "raku.tab.c"
    break;

  case 19: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 449 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3118 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 453 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3124 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 455 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3130 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 457 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3136 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 459 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3142 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 461 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3148 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 463 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3156 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 467 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3164 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 471 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3170 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 473 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3176 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 475 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3182 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 477 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3188 "raku.tab.c"
    break;

  case 31: /* stmt: KW_USE IDENT ';'  */
#line 479 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3194 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3200 "raku.tab.c"
    break;

  case 33: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3206 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3212 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 487 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3218 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 489 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3224 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 491 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3230 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 493 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3236 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 495 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3242 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 497 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); free((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 3248 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 499 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3254 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 501 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3261 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 504 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3268 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 507 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3274 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 509 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3281 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 512 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3288 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 515 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3294 "raku.tab.c"
    break;

  case 48: /* stmt: KW_RETURN expr ';'  */
#line 517 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3300 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN ';'  */
#line 519 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3306 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 521 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3313 "raku.tab.c"
    break;

  case 51: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 524 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3320 "raku.tab.c"
    break;

  case 52: /* stmt: KW_EXIT expr ';'  */
#line 527 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3326 "raku.tab.c"
    break;

  case 53: /* stmt: KW_EXIT ';'  */
#line 529 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 3333 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 532 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3339 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 534 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3348 "raku.tab.c"
    break;

  case 56: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 539 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3356 "raku.tab.c"
    break;

  case 57: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 543 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3364 "raku.tab.c"
    break;

  case 58: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 547 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3373 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 552 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3381 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall ';'  */
#line 555 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3387 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 557 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3393 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 559 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3399 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 561 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3405 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 563 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3411 "raku.tab.c"
    break;

  case 65: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 565 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3418 "raku.tab.c"
    break;

  case 66: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 568 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3424 "raku.tab.c"
    break;

  case 67: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 570 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3430 "raku.tab.c"
    break;

  case 68: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 572 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3436 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 574 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3443 "raku.tab.c"
    break;

  case 70: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 577 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3450 "raku.tab.c"
    break;

  case 71: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 580 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3457 "raku.tab.c"
    break;

  case 72: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 583 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3464 "raku.tab.c"
    break;

  case 73: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 586 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3471 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_IF expr ';'  */
#line 589 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3477 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_UNLESS expr ';'  */
#line 591 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3483 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WHILE expr ';'  */
#line 593 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3489 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_UNTIL expr ';'  */
#line 595 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3495 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_FOR expr ';'  */
#line 597 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3502 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_WITH expr ';'  */
#line 600 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3508 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WITHOUT expr ';'  */
#line 602 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3514 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_GIVEN expr ';'  */
#line 604 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3520 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 606 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3527 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 609 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3534 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 612 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3542 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 616 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3549 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 619 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3555 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 621 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3561 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 623 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3567 "raku.tab.c"
    break;

  case 89: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 625 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3574 "raku.tab.c"
    break;

  case 90: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 628 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3581 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 631 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3588 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 634 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3595 "raku.tab.c"
    break;

  case 93: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 637 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3602 "raku.tab.c"
    break;

  case 94: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 640 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3609 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 643 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3616 "raku.tab.c"
    break;

  case 96: /* stmt: expr ';'  */
#line 645 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3622 "raku.tab.c"
    break;

  case 97: /* stmt: ';'  */
#line 646 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3628 "raku.tab.c"
    break;

  case 98: /* stmt: if_stmt  */
#line 647 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3634 "raku.tab.c"
    break;

  case 99: /* stmt: while_stmt  */
#line 648 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3640 "raku.tab.c"
    break;

  case 100: /* stmt: for_stmt  */
#line 649 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3646 "raku.tab.c"
    break;

  case 101: /* stmt: given_stmt  */
#line 650 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3652 "raku.tab.c"
    break;

  case 102: /* stmt: KW_TRY block  */
#line 652 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3658 "raku.tab.c"
    break;

  case 103: /* stmt: KW_TRY block KW_CATCH block  */
#line 654 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3664 "raku.tab.c"
    break;

  case 104: /* stmt: KW_CATCH block  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3670 "raku.tab.c"
    break;

  case 105: /* stmt: block  */
#line 658 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3676 "raku.tab.c"
    break;

  case 106: /* stmt: unless_stmt  */
#line 659 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3682 "raku.tab.c"
    break;

  case 107: /* stmt: until_stmt  */
#line 660 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3688 "raku.tab.c"
    break;

  case 108: /* stmt: repeat_stmt  */
#line 661 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3694 "raku.tab.c"
    break;

  case 109: /* stmt: loop_stmt  */
#line 662 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3700 "raku.tab.c"
    break;

  case 110: /* stmt: KW_LAST ';'  */
#line 663 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3706 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT ';'  */
#line 664 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3712 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST KW_IF expr ';'  */
#line 666 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3718 "raku.tab.c"
    break;

  case 113: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 668 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3724 "raku.tab.c"
    break;

  case 114: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3730 "raku.tab.c"
    break;

  case 115: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3736 "raku.tab.c"
    break;

  case 116: /* stmt: sub_decl  */
#line 673 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3742 "raku.tab.c"
    break;

  case 117: /* stmt: class_decl  */
#line 674 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3748 "raku.tab.c"
    break;

  case 118: /* stmt: role_decl  */
#line 675 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3754 "raku.tab.c"
    break;

  case 119: /* stmt: grammar_decl  */
#line 676 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3760 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF '(' expr ')' block  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3766 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3772 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 684 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3778 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF expr block  */
#line 686 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3784 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3790 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3796 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block elsif_tail  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3802 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 694 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3808 "raku.tab.c"
    break;

  case 128: /* elsif_tail: KW_ELSIF expr block  */
#line 698 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3814 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 700 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3820 "raku.tab.c"
    break;

  case 130: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 702 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3826 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 704 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3832 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 706 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3838 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3844 "raku.tab.c"
    break;

  case 134: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 712 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3850 "raku.tab.c"
    break;

  case 135: /* while_stmt: KW_WHILE expr block  */
#line 714 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3856 "raku.tab.c"
    break;

  case 136: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3862 "raku.tab.c"
    break;

  case 137: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3868 "raku.tab.c"
    break;

  case 138: /* unless_stmt: KW_UNLESS expr block  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3874 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 140: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3886 "raku.tab.c"
    break;

  case 141: /* until_stmt: KW_UNTIL expr block  */
#line 730 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3892 "raku.tab.c"
    break;

  case 142: /* repeat_stmt: KW_REPEAT block  */
#line 734 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3898 "raku.tab.c"
    break;

  case 143: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 736 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 144: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 738 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3910 "raku.tab.c"
    break;

  case 145: /* loop_stmt: KW_LOOP block  */
#line 742 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3916 "raku.tab.c"
    break;

  case 146: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 744 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3922 "raku.tab.c"
    break;

  case 147: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 746 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3928 "raku.tab.c"
    break;

  case 148: /* loop_incr: expr  */
#line 749 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3934 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 753 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3944 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 759 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3954 "raku.tab.c"
    break;

  case 151: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 765 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3963 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 770 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3973 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 776 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3982 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr block  */
#line 781 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3989 "raku.tab.c"
    break;

  case 155: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 786 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4001 "raku.tab.c"
    break;

  case 156: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 794 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4014 "raku.tab.c"
    break;

  case 157: /* when_list: %empty  */
#line 804 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4020 "raku.tab.c"
    break;

  case 158: /* when_list: when_list KW_WHEN expr block  */
#line 806 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4028 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 812 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4040 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 820 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4050 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_SUB IDENT sub_body  */
#line 826 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4060 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 832 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4072 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 840 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4082 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 846 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 4092 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 852 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4105 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 861 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4116 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 868 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4129 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 877 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4142 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 886 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4153 "raku.tab.c"
    break;

  case 170: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 895 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4164 "raku.tab.c"
    break;

  case 171: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 902 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4173 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list '}'  */
#line 908 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4179 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr '}'  */
#line 910 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4186 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 913 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4193 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 916 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4199 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 918 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4205 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 920 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4211 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 922 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4217 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 924 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4223 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 926 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4229 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 928 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4235 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 930 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4242 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 933 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4248 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 935 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4254 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 937 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4260 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 939 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4267 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 942 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4274 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 945 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4282 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list '}'  */
#line 950 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4288 "raku.tab.c"
    break;

  case 190: /* method_body: '{' YADA '}'  */
#line 951 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4294 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr '}'  */
#line 953 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4301 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 956 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4308 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 959 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4314 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 961 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4320 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 963 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4326 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 965 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4332 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 967 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4338 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 969 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4344 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 971 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4350 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 973 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4357 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 976 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4363 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 978 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4369 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 980 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4375 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 982 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4382 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 985 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4389 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 988 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4397 "raku.tab.c"
    break;

  case 207: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 994 "raku.y"
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
#line 4415 "raku.tab.c"
    break;

  case 208: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1010 "raku.y"
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
#line 4432 "raku.tab.c"
    break;

  case 209: /* is_clauses: %empty  */
#line 1024 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4438 "raku.tab.c"
    break;

  case 210: /* is_clauses: is_clauses IDENT IDENT  */
#line 1026 "raku.y"
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
#line 4454 "raku.tab.c"
    break;

  case 211: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1038 "raku.y"
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
#line 4470 "raku.tab.c"
    break;

  case 212: /* class_body_list: %empty  */
#line 1051 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4476 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1053 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4483 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1056 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4490 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1059 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4497 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1062 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4504 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1065 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4511 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1068 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4518 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1071 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4525 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1074 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4532 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1077 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4539 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1080 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4546 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1083 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4553 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1086 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4560 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1089 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4571 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1096 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4582 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1103 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4593 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1110 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4604 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1117 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4612 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1121 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4620 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1125 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4628 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1129 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4636 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1133 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4647 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1140 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4658 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1147 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4669 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1154 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4680 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1161 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4694 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1171 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4706 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1179 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4718 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1187 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4731 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1196 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4742 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1203 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4753 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1210 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4767 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1220 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4779 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1228 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4791 "raku.tab.c"
    break;

  case 246: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1238 "raku.y"
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
#line 4808 "raku.tab.c"
    break;

  case 247: /* grammar_body_list: %empty  */
#line 1252 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4814 "raku.tab.c"
    break;

  case 248: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1254 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4823 "raku.tab.c"
    break;

  case 249: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1259 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4832 "raku.tab.c"
    break;

  case 250: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1264 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4841 "raku.tab.c"
    break;

  case 251: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1271 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4849 "raku.tab.c"
    break;

  case 252: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1275 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 4857 "raku.tab.c"
    break;

  case 253: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1279 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4865 "raku.tab.c"
    break;

  case 254: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1283 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 4873 "raku.tab.c"
    break;

  case 255: /* pair_list: IDENT OP_FATARROW expr  */
#line 1289 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4879 "raku.tab.c"
    break;

  case 256: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1291 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4885 "raku.tab.c"
    break;

  case 257: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1293 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4891 "raku.tab.c"
    break;

  case 258: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1295 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4897 "raku.tab.c"
    break;

  case 259: /* param_list: VAR_SCALAR  */
#line 1298 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4903 "raku.tab.c"
    break;

  case 260: /* param_list: VAR_ARRAY  */
#line 1299 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4909 "raku.tab.c"
    break;

  case 261: /* param_list: param_list ',' VAR_ARRAY  */
#line 1300 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4915 "raku.tab.c"
    break;

  case 262: /* param_list: IDENT VAR_SCALAR  */
#line 1301 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4921 "raku.tab.c"
    break;

  case 263: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1302 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4927 "raku.tab.c"
    break;

  case 264: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1303 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4933 "raku.tab.c"
    break;

  case 265: /* param_list: param_list ',' VAR_SCALAR  */
#line 1304 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4939 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1305 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4945 "raku.tab.c"
    break;

  case 267: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1306 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4951 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1307 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4957 "raku.tab.c"
    break;

  case 269: /* param_list: VAR_SCALAR '=' expr  */
#line 1308 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4963 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1309 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4969 "raku.tab.c"
    break;

  case 271: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1310 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4975 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1311 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4981 "raku.tab.c"
    break;

  case 273: /* param_list: SLURPY_POS  */
#line 1312 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4987 "raku.tab.c"
    break;

  case 274: /* param_list: param_list ',' SLURPY_POS  */
#line 1313 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4993 "raku.tab.c"
    break;

  case 275: /* param_list: SLURPY_LOL  */
#line 1314 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4999 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' SLURPY_LOL  */
#line 1315 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 5005 "raku.tab.c"
    break;

  case 277: /* param_list: SLURPY_NAMED  */
#line 1316 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5011 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1317 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 5017 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list '}'  */
#line 1320 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 5023 "raku.tab.c"
    break;

  case 280: /* block: '{' YADA '}'  */
#line 1321 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5029 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr '}'  */
#line 1323 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 5035 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1325 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5041 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1327 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5047 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1329 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5053 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1331 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5059 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1333 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5066 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1336 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5072 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1338 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5078 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1340 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5084 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1342 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5090 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1344 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5097 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1347 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5104 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1350 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5112 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1354 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5119 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1357 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5126 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1360 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5133 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1363 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5139 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1365 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5147 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1369 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5154 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1372 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5161 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1375 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5168 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1378 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5175 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1381 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5182 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1384 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5188 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1386 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5194 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1388 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5200 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list KW_LAST '}'  */
#line 1390 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5206 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1392 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5212 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1394 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5218 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5224 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1398 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5230 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1400 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5236 "raku.tab.c"
    break;

  case 313: /* closure: '{' expr '}'  */
#line 1403 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5242 "raku.tab.c"
    break;

  case 314: /* expr: VAR_SCALAR '=' expr  */
#line 1406 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5248 "raku.tab.c"
    break;

  case 315: /* expr: VAR_ARRAY '=' expr  */
#line 1407 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5254 "raku.tab.c"
    break;

  case 316: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1408 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5260 "raku.tab.c"
    break;

  case 317: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1409 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5266 "raku.tab.c"
    break;

  case 318: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1410 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5272 "raku.tab.c"
    break;

  case 319: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1411 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5278 "raku.tab.c"
    break;

  case 320: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1412 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5284 "raku.tab.c"
    break;

  case 321: /* expr: KW_GATHER block  */
#line 1413 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5294 "raku.tab.c"
    break;

  case 322: /* expr: tern_expr  */
#line 1418 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5300 "raku.tab.c"
    break;

  case 323: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1422 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5306 "raku.tab.c"
    break;

  case 324: /* tern_expr: cmp_expr  */
#line 1423 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5312 "raku.tab.c"
    break;

  case 325: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1426 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5318 "raku.tab.c"
    break;

  case 326: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1427 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5324 "raku.tab.c"
    break;

  case 327: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1428 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5330 "raku.tab.c"
    break;

  case 328: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1429 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5336 "raku.tab.c"
    break;

  case 329: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1430 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5342 "raku.tab.c"
    break;

  case 330: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1431 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5348 "raku.tab.c"
    break;

  case 331: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1432 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5354 "raku.tab.c"
    break;

  case 332: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1433 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5360 "raku.tab.c"
    break;

  case 333: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1434 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5366 "raku.tab.c"
    break;

  case 334: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1435 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5372 "raku.tab.c"
    break;

  case 335: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1436 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5378 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1437 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5384 "raku.tab.c"
    break;

  case 337: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1438 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5390 "raku.tab.c"
    break;

  case 338: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1439 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5396 "raku.tab.c"
    break;

  case 339: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1441 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5406 "raku.tab.c"
    break;

  case 340: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1447 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5416 "raku.tab.c"
    break;

  case 341: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1453 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5426 "raku.tab.c"
    break;

  case 342: /* cmp_expr: divis_expr  */
#line 1458 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5432 "raku.tab.c"
    break;

  case 343: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1461 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5438 "raku.tab.c"
    break;

  case 344: /* divis_expr: jct_expr  */
#line 1462 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5444 "raku.tab.c"
    break;

  case 345: /* jct_expr: jct_expr '|' range_expr  */
#line 1465 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5450 "raku.tab.c"
    break;

  case 346: /* jct_expr: jct_expr '&' range_expr  */
#line 1466 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5456 "raku.tab.c"
    break;

  case 347: /* jct_expr: dor_expr  */
#line 1467 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5462 "raku.tab.c"
    break;

  case 348: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1471 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5468 "raku.tab.c"
    break;

  case 349: /* dor_expr: range_expr  */
#line 1472 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5474 "raku.tab.c"
    break;

  case 350: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1475 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5480 "raku.tab.c"
    break;

  case 351: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1476 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5486 "raku.tab.c"
    break;

  case 352: /* range_expr: add_expr  */
#line 1477 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5492 "raku.tab.c"
    break;

  case 353: /* add_expr: add_expr '~' repl_expr  */
#line 1480 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5498 "raku.tab.c"
    break;

  case 354: /* add_expr: repl_expr  */
#line 1481 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5504 "raku.tab.c"
    break;

  case 355: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1484 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5510 "raku.tab.c"
    break;

  case 356: /* repl_expr: addsub_expr  */
#line 1485 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5516 "raku.tab.c"
    break;

  case 357: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1488 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5522 "raku.tab.c"
    break;

  case 358: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1489 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5528 "raku.tab.c"
    break;

  case 359: /* addsub_expr: mul_expr  */
#line 1490 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5534 "raku.tab.c"
    break;

  case 360: /* mul_expr: mul_expr '*' unary_expr  */
#line 1493 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5540 "raku.tab.c"
    break;

  case 361: /* mul_expr: mul_expr '/' unary_expr  */
#line 1494 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5546 "raku.tab.c"
    break;

  case 362: /* mul_expr: mul_expr '%' unary_expr  */
#line 1495 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5552 "raku.tab.c"
    break;

  case 363: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1496 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5558 "raku.tab.c"
    break;

  case 364: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1498 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5564 "raku.tab.c"
    break;

  case 365: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1500 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5570 "raku.tab.c"
    break;

  case 366: /* mul_expr: unary_expr  */
#line 1501 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5576 "raku.tab.c"
    break;

  case 367: /* unary_expr: '-' unary_expr  */
#line 1504 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5582 "raku.tab.c"
    break;

  case 368: /* unary_expr: '!' unary_expr  */
#line 1505 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5588 "raku.tab.c"
    break;

  case 369: /* unary_expr: CARET unary_expr  */
#line 1506 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5594 "raku.tab.c"
    break;

  case 370: /* unary_expr: OP_REDUCE unary_expr  */
#line 1508 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5603 "raku.tab.c"
    break;

  case 371: /* unary_expr: pow_expr  */
#line 1512 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5609 "raku.tab.c"
    break;

  case 372: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1515 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5615 "raku.tab.c"
    break;

  case 373: /* pow_expr: postfix_expr  */
#line 1516 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5621 "raku.tab.c"
    break;

  case 374: /* scalar_list: VAR_SCALAR  */
#line 1519 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5627 "raku.tab.c"
    break;

  case 375: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1520 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5633 "raku.tab.c"
    break;

  case 376: /* meth_name: IDENT  */
#line 1523 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5639 "raku.tab.c"
    break;

  case 377: /* meth_name: KW_SORT  */
#line 1524 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5645 "raku.tab.c"
    break;

  case 378: /* meth_name: KW_MAP  */
#line 1525 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5651 "raku.tab.c"
    break;

  case 379: /* meth_name: KW_GREP  */
#line 1526 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5657 "raku.tab.c"
    break;

  case 380: /* meth_name: KW_SAY  */
#line 1527 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5663 "raku.tab.c"
    break;

  case 381: /* meth_name: KW_PRINT  */
#line 1528 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5669 "raku.tab.c"
    break;

  case 382: /* meth_name: KW_TAKE  */
#line 1529 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5675 "raku.tab.c"
    break;

  case 383: /* meth_name: KW_RETURN  */
#line 1530 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5681 "raku.tab.c"
    break;

  case 384: /* meth_name: KW_EXISTS  */
#line 1531 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5687 "raku.tab.c"
    break;

  case 385: /* meth_name: KW_DELETE  */
#line 1532 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5693 "raku.tab.c"
    break;

  case 386: /* meth_name: TESTOP  */
#line 1533 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5699 "raku.tab.c"
    break;

  case 387: /* postfix_expr: call_expr  */
#line 1535 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5705 "raku.tab.c"
    break;

  case 388: /* call_expr: IDENT '(' arg_list ')'  */
#line 1538 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5714 "raku.tab.c"
    break;

  case 389: /* call_expr: IDENT '(' ')'  */
#line 1542 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5720 "raku.tab.c"
    break;

  case 390: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1544 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5726 "raku.tab.c"
    break;

  case 391: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1546 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5732 "raku.tab.c"
    break;

  case 392: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1548 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5741 "raku.tab.c"
    break;

  case 393: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1553 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5747 "raku.tab.c"
    break;

  case 394: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1555 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5757 "raku.tab.c"
    break;

  case 395: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1561 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5767 "raku.tab.c"
    break;

  case 396: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1567 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5775 "raku.tab.c"
    break;

  case 397: /* call_expr: IDENT '.' KW_NEW  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5783 "raku.tab.c"
    break;

  case 398: /* call_expr: IDENT '.' IDENT  */
#line 1575 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5792 "raku.tab.c"
    break;

  case 399: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1580 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5803 "raku.tab.c"
    break;

  case 400: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1587 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5812 "raku.tab.c"
    break;

  case 401: /* call_expr: IDENT '.' CARET IDENT  */
#line 1592 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5822 "raku.tab.c"
    break;

  case 402: /* call_expr: atom '.' CARET IDENT  */
#line 1598 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5832 "raku.tab.c"
    break;

  case 403: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1604 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5843 "raku.tab.c"
    break;

  case 404: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1611 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5854 "raku.tab.c"
    break;

  case 405: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1618 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5863 "raku.tab.c"
    break;

  case 406: /* call_expr: atom '.' meth_name  */
#line 1623 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5872 "raku.tab.c"
    break;

  case 407: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1628 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5883 "raku.tab.c"
    break;

  case 408: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1635 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5892 "raku.tab.c"
    break;

  case 409: /* call_expr: call_expr '.' meth_name  */
#line 1640 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5901 "raku.tab.c"
    break;

  case 410: /* call_expr: KW_DIE expr  */
#line 1645 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5907 "raku.tab.c"
    break;

  case 411: /* call_expr: KW_MAP closure expr  */
#line 1647 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5913 "raku.tab.c"
    break;

  case 412: /* call_expr: KW_GREP closure expr  */
#line 1649 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5919 "raku.tab.c"
    break;

  case 413: /* call_expr: KW_SORT expr  */
#line 1651 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5925 "raku.tab.c"
    break;

  case 414: /* call_expr: KW_SORT closure expr  */
#line 1653 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5931 "raku.tab.c"
    break;

  case 415: /* call_expr: atom  */
#line 1654 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5937 "raku.tab.c"
    break;

  case 416: /* arg_list: expr  */
#line 1657 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5943 "raku.tab.c"
    break;

  case 417: /* arg_list: arg_list ',' expr  */
#line 1658 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5949 "raku.tab.c"
    break;

  case 418: /* arg_list: arg_list ','  */
#line 1659 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5955 "raku.tab.c"
    break;

  case 419: /* atom: LIT_INT  */
#line 1662 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5961 "raku.tab.c"
    break;

  case 420: /* atom: LIT_FLOAT  */
#line 1663 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5967 "raku.tab.c"
    break;

  case 421: /* atom: LIT_STR  */
#line 1664 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5973 "raku.tab.c"
    break;

  case 422: /* atom: WORDLIST  */
#line 1666 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5986 "raku.tab.c"
    break;

  case 423: /* atom: LIT_INTERP_STR  */
#line 1674 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5992 "raku.tab.c"
    break;

  case 424: /* atom: VAR_SCALAR  */
#line 1675 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5998 "raku.tab.c"
    break;

  case 425: /* atom: OP_INC VAR_SCALAR  */
#line 1676 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 6004 "raku.tab.c"
    break;

  case 426: /* atom: OP_DEC VAR_SCALAR  */
#line 1677 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 6010 "raku.tab.c"
    break;

  case 427: /* atom: VAR_SCALAR OP_INC  */
#line 1678 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 6016 "raku.tab.c"
    break;

  case 428: /* atom: VAR_SCALAR OP_DEC  */
#line 1679 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 6022 "raku.tab.c"
    break;

  case 429: /* atom: VAR_ARRAY  */
#line 1680 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6028 "raku.tab.c"
    break;

  case 430: /* atom: VAR_HASH  */
#line 1681 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6034 "raku.tab.c"
    break;

  case 431: /* atom: VAR_CAPTURE  */
#line 1683 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6042 "raku.tab.c"
    break;

  case 432: /* atom: VAR_FH  */
#line 1687 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 6050 "raku.tab.c"
    break;

  case 433: /* atom: VAR_NAMED_CAPTURE  */
#line 1691 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 6057 "raku.tab.c"
    break;

  case 434: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1694 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 6063 "raku.tab.c"
    break;

  case 435: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1696 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 6069 "raku.tab.c"
    break;

  case 436: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1698 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 6075 "raku.tab.c"
    break;

  case 437: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1700 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 6081 "raku.tab.c"
    break;

  case 438: /* atom: ARR_ALL_SLICE  */
#line 1702 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 6087 "raku.tab.c"
    break;

  case 439: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1704 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6093 "raku.tab.c"
    break;

  case 440: /* atom: VAR_HASH '{' expr '}'  */
#line 1706 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6099 "raku.tab.c"
    break;

  case 441: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1708 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 6105 "raku.tab.c"
    break;

  case 442: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1710 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 6111 "raku.tab.c"
    break;

  case 443: /* atom: IDENT  */
#line 1711 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 6117 "raku.tab.c"
    break;

  case 444: /* atom: VAR_TWIGIL  */
#line 1713 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6125 "raku.tab.c"
    break;

  case 445: /* atom: VAR_ARRAY_TWIGIL  */
#line 1717 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6133 "raku.tab.c"
    break;

  case 446: /* atom: VAR_HASH_TWIGIL  */
#line 1721 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 6141 "raku.tab.c"
    break;

  case 447: /* atom: '(' ')'  */
#line 1724 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 6147 "raku.tab.c"
    break;

  case 448: /* atom: '(' expr ')'  */
#line 1725 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 6153 "raku.tab.c"
    break;

  case 449: /* atom: '(' expr ',' ')'  */
#line 1727 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 6159 "raku.tab.c"
    break;

  case 450: /* atom: '(' expr ',' arg_list ')'  */
#line 1729 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6166 "raku.tab.c"
    break;

  case 451: /* atom: block  */
#line 1731 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6172 "raku.tab.c"
    break;

  case 452: /* atom: KW_SUB block  */
#line 1732 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6178 "raku.tab.c"
    break;


#line 6182 "raku.tab.c"

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

#line 1734 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
