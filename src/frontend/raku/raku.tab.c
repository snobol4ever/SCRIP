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
#define YYLAST   4195

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  133
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  444
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1117

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
     495,   498,   501,   503,   506,   509,   511,   513,   515,   518,
     521,   523,   528,   532,   536,   541,   545,   546,   548,   550,
     552,   554,   557,   559,   561,   563,   566,   569,   572,   575,
     578,   580,   582,   584,   586,   589,   591,   593,   595,   598,
     601,   605,   608,   610,   612,   614,   617,   620,   623,   626,
     629,   632,   635,   636,   637,   638,   639,   640,   641,   643,
     645,   647,   649,   650,   651,   652,   653,   654,   655,   657,
     659,   661,   663,   664,   665,   666,   669,   671,   673,   675,
     677,   679,   681,   683,   687,   689,   691,   693,   695,   697,
     701,   703,   707,   709,   711,   713,   717,   719,   723,   725,
     727,   731,   733,   735,   739,   742,   748,   754,   759,   765,
     770,   775,   783,   794,   795,   801,   809,   815,   821,   829,
     835,   841,   850,   857,   866,   875,   884,   891,   898,   899,
     902,   905,   907,   909,   911,   913,   915,   917,   919,   922,
     924,   926,   928,   931,   934,   940,   941,   942,   945,   948,
     950,   952,   954,   956,   958,   960,   962,   965,   967,   969,
     971,   974,   977,   983,   999,  1014,  1015,  1027,  1041,  1042,
    1045,  1048,  1051,  1054,  1057,  1060,  1063,  1066,  1069,  1072,
    1075,  1078,  1085,  1092,  1099,  1106,  1110,  1114,  1118,  1122,
    1129,  1136,  1143,  1150,  1160,  1168,  1176,  1185,  1192,  1199,
    1209,  1217,  1227,  1242,  1243,  1248,  1253,  1260,  1264,  1270,
    1272,  1274,  1276,  1280,  1281,  1282,  1283,  1284,  1285,  1286,
    1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1302,  1303,  1304,  1306,  1308,  1310,  1312,
    1314,  1317,  1319,  1321,  1323,  1325,  1328,  1331,  1335,  1338,
    1341,  1344,  1346,  1350,  1353,  1356,  1359,  1362,  1365,  1367,
    1369,  1371,  1373,  1375,  1377,  1379,  1381,  1385,  1388,  1389,
    1390,  1391,  1392,  1393,  1394,  1395,  1400,  1403,  1405,  1408,
    1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,  1418,
    1419,  1420,  1421,  1422,  1428,  1434,  1440,  1443,  1444,  1447,
    1448,  1449,  1452,  1454,  1457,  1458,  1459,  1462,  1463,  1466,
    1467,  1470,  1471,  1472,  1475,  1476,  1477,  1478,  1479,  1482,
    1483,  1484,  1485,  1490,  1493,  1494,  1497,  1498,  1501,  1502,
    1503,  1504,  1505,  1506,  1507,  1508,  1509,  1510,  1511,  1513,
    1515,  1520,  1521,  1523,  1525,  1530,  1532,  1538,  1544,  1548,
    1552,  1557,  1564,  1569,  1575,  1581,  1588,  1595,  1600,  1605,
    1612,  1617,  1622,  1624,  1626,  1628,  1630,  1632,  1635,  1636,
    1637,  1640,  1641,  1642,  1643,  1652,  1653,  1654,  1655,  1656,
    1657,  1658,  1659,  1660,  1664,  1668,  1671,  1673,  1675,  1677,
    1679,  1681,  1683,  1685,  1687,  1689,  1690,  1694,  1698,  1702,
    1703,  1704,  1706,  1709,  1710
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

#define YYPACT_NINF (-769)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-444)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -769,    61,  1716,  -769,  -769,  -769,  -769,  -769,   785,   124,
     -13,   -99,   185,  -769,  -769,  4064,  -769,  -769,  -769,    69,
    1789,   304,  3042,  3115,  3188,  3261,  3334,    -3,    63,  3334,
    1862,   137,  3334,   122,   202,  3407,  3480,    63,   -78,    28,
      46,    93,    93,  3553,    63,    63,  3334,   217,   299,    60,
    4064,  -769,  -769,   330,   263,   275,  4064,  4064,  -769,   739,
     170,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,   558,  -769,  -769,  -769,  1022,   615,  -769,   524,   875,
      68,   298,  -769,    32,   307,   467,   381,  -769,  -769,   319,
     328,   338,   419,  -769,  -769,  3334,  3334,  3334,  3334,  3334,
    3334,   429,  1935,  3626,  2009,   459,  3334,  3334,   192,  2082,
     253,   380,   144,  -769,    63,  -769,  -769,   386,   721,   172,
    -769,  2156,  -769,   123,   -47,   -26,   221,   523,   507,   193,
     534,   739,   525,   739,   265,   739,    63,   739,    63,   -61,
     139,   181,  -769,  -769,   423,  -769,    47,   442,   446,   440,
     211,   245,   739,    63,   739,    63,   396,  2230,  -769,  3334,
    3334,  -769,  3334,  3334,  -769,  3334,  3334,  3334,  2303,  3334,
    -769,   528,  -769,  -769,  -769,   464,   473,    54,  -769,   493,
    -769,  -769,  -769,  -769,  -769,   482,   458,  1152,  3334,  3334,
    3334,  3334,  3334,  3334,  3334,  3334,  -769,  3334,  3334,  3334,
    3334,  3334,  3334,  3334,  3334,  -769,  4064,  4064,  4064,  4064,
    4064,  4064,  4064,  4064,  4064,  4064,  4064,  4064,  4064,  4064,
    4064,   821,  4064,  4064,  4064,  4064,  4064,  4064,  4064,  4064,
    4064,  4064,  4064,  4064,  4064,  4064,  4064,   750,   688,   561,
     479,   527,   536,   570,   578,   606,   133,  -769,   521,   739,
     228,   600,   309,   633,   655,   640,   672,   792,   705,   296,
    -769,   649,   668,  2009,   819,  3334,  -769,  3334,  3334,  3334,
    3334,  3334,  3334,  3334,   731,   482,   745,  -769,  3334,  3334,
    -769,  3699,  -769,  3772,  -769,   266,   288,   301,   244,   748,
     771,  -769,   749,   777,  3334,  3334,  3334,  3334,  3334,  3334,
    3334,  -769,  3334,   780,  3334,  3334,  -769,  3334,   826,   754,
     839,  -769,   880,  3334,  -769,  4064,  4064,    15,  -769,  -769,
    -769,  3334,  3334,  -769,  3334,  3334,  -769,   943,  3334,   965,
    3334,   843,   909,   846,  -769,  3334,  3334,   940,   850,   862,
     864,   884,   899,   863,  -769,  -769,  -769,    63,    -5,  -769,
      21,   891,   911,  -769,  -769,  2376,  -769,   912,   271,   254,
     890,  3042,  3115,  3334,   927,   130,   206,  -769,   366,   910,
     926,   929,   932,   935,   959,   977,  1016,  1020,  1025,  1029,
    1034,  1035,  1036,  1054,  1055,   958,   958,   958,   958,   958,
     958,  1037,   958,   958,   958,   958,   958,   958,   958,   958,
    -769,  -769,  -769,    68,  -769,  -769,  -769,  1072,  1072,   307,
     467,   381,   381,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  1064,
    1177,  1066,  -769,  2450,  -769,  -769,  -769,  -769,  -769,  -769,
    3334,  2524,  -769,   865,  3334,  -769,  3334,  3334,  3334,  3334,
    1097,  1098,  1099,  -769,  2598,  -769,  2672,  3334,  -769,  1199,
    -769,  3845,   356,  1100,  1085,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  1093,  -769,  1101,   739,   259,  1145,   297,
    2746,   220,  1102,  3334,  -769,  3918,  -769,  3334,  -769,    41,
    -769,  3334,  3334,  1112,  1214,  2376,  1103,  1104,  1106,  1107,
    1108,  1109,  1110,   255,  2376,  1111,  1113,   291,    63,    12,
    3991,  -769,    63,    63,    82,   -38,   -33,  1119,  -769,     6,
    -769,  -769,  -769,  1114,   870,  1322,  1115,  1116,  1117,  1118,
       8,  1123,  1120,  1125,  1121,    63,    63,    63,  1122,  1124,
    1133,  3334,  -769,  -769,  -769,  -769,  -769,  -769,  1232,  1234,
    -769,   -20,  1114,   872,    48,   439,   146,  -769,   876,  1240,
    2009,  3334,  3334,   368,    13,   335,  -769,   215,  3334,  3334,
    -769,  3334,  3334,  -769,  3334,  3334,  3334,  3334,  3334,  3334,
    3334,  3334,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  4064,
    2820,  -769,  2894,  1130,   878,  1131,  -769,   892,  2376,  1132,
     369,  1128,  1129,   492,  3334,  3334,  3334,  -769,   894,  -769,
     901,   903,  -769,  1183,   913,  -769,  -769,  -769,  -769,  -769,
     915,  3334,  -769,  3334,  3334,  3334,   917,  -769,   125,  -769,
    1136,   739,   535,  1137,  1114,   920,  1138,  1140,  3334,  -769,
     922,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,   928,
    -769,  -769,  -769,   867,  -769,  -769,   739,    63,  -769,  -769,
    1256,  -769,  1257,  1258,  3334,  1158,  1260,  1262,  -769,  1114,
     526,  3042,  3115,  1519,  -769,   388,  -769,  -769,  -769,  -769,
    3334,    63,  -769,  -769,  -769,  1148,  1149,  1248,  -769,  -769,
    -769,  -769,  3334,  1151,  -769,  -769,   148,   643,   970,  1223,
    -769,  -769,  1114,  1114,   936,   947,  1266,  1267,  1268,  -769,
    -769,   325,   618,  1153,   363,  3334,  3334,  3334,  3334,  3334,
    3334,  -769,  -769,  -769,  -769,   367,   385,   424,   457,   471,
     494,   496,   510,   511,   515,   537,   541,  -769,  -769,   949,
    -769,   951,   953,  -769,  1161,  -769,  -769,   955,  -769,  -769,
    -769,  -769,  -769,  1162,  1163,  1164,  -769,  -769,  -769,  3334,
    -769,  2376,  1165,   592,  -769,  -769,  1166,  1215,  1216,  -769,
     961,  -769,  3334,  -769,  -769,  1114,  -769,  -769,   607,  1169,
    1170,    12,  -769,   963,  1065,    63,    63,    63,  -769,  3334,
    -769,  -769,  -769,  1185,  -769,    99,  -769,  -769,  -769,   418,
      49,  -769,   256,  3334,  3334,  3334,  3334,  3334,  3334,  3334,
    3334,  -769,    63,  1167,  -769,  -769,    63,  1172,  3334,  -769,
     345,   393,    -8,   216,   794,  1173,  1175,   750,  -769,  -769,
    1114,  1114,  1294,  1295,  1296,  3334,  2968,  1193,  1194,  -769,
     546,   550,   564,   565,   566,   574,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  1181,  -769,  -769,  -769,  -769,   967,  -769,  -769,
    -769,  3334,  3334,  2376,   608,  -769,  -769,  3334,  -769,  -769,
    -769,  -769,    63,    63,  -769,  -769,  -769,  -769,  -769,  3334,
    1196,  1298,  1299,  3334,  3334,  3334,  -769,  -769,  -769,   579,
     582,   583,   584,   609,   636,   637,   641,  -769,  -769,  -769,
    3334,  1184,  -769,    55,  1238,  -769,   113,  -769,  1300,  1301,
    1187,  3334,  -769,  1302,  1304,  1195,  3334,  -769,   232,   238,
    1197,  1198,  -769,  -769,   421,  -769,  -769,  -769,  -769,  -769,
     653,  1189,   969,  3334,  3334,  -769,  -769,  -769,  -769,  -769,
    -769,  -769,  1204,  -769,  -769,   971,  -769,   635,  1089,  -769,
    -769,  3334,  -769,  -769,   654,   702,   717,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  1205,    63,  1188,   993,  1192,
    1446,  1188,   998,  1206,  1224,  -769,  1235,  1236,  1237,  -769,
    1253,  1345,  1365,  1255,  3334,  -769,  1369,  1370,  1263,  3334,
    -769,  -769,  -769,   223,  -769,  -769,  -769,  1261,   723,   725,
    -769,  1264,  -769,    63,  -769,  -769,  -769,  -769,  -769,  3334,
    -769,  -769,  1188,  -769,  3042,  3115,  1592,  -769,   486,  -769,
    1188,  -769,  -769,  -769,  -769,  -769,  -769,  1265,  1269,  -769,
    1271,  1272,  1273,  -769,  1274,  1188,  1006,  -769,  -769,  -769,
    -769,  -769,  1275,  -769,   502,   111,  -769,   343,  3334,  3334,
    3334,  3334,  3334,  3334,  3334,  3334,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  1188,    63,  3334,  3334,  3334,
    -769,  -769,  -769,   728,   729,   742,   743,   746,   766,   767,
     768,  -769,  -769,   803,   823,   824,  -769,  -769,  -769,  -769,
    -769,  -769,  -769,  -769,  -769,  -769,  -769
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   411,   412,   413,   415,   416,   421,
     422,   436,   435,   437,   438,     0,   423,   424,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,   414,     0,     0,     0,     0,     0,    93,     0,
       3,     4,    94,    95,   102,   103,   104,   105,    96,    97,
     112,     0,   113,   114,   115,   101,     0,   316,   318,   336,
     338,   341,   343,   346,   348,   350,   353,   358,   363,   365,
     379,   407,     0,   419,   420,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     416,   421,   422,   436,     0,   443,   361,     0,   416,   421,
      35,     0,   408,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,     0,   444,   315,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,     0,   141,     0,
       0,   106,     0,     0,   107,     0,     0,     0,     3,     0,
     405,    98,   100,   402,   205,     0,     0,     0,   362,     0,
     417,   418,   360,   359,   439,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     310,   311,   312,   313,   314,   308,     0,   385,     0,     0,
     309,     0,     0,     0,     0,     0,   390,     0,   389,   435,
     381,     0,     0,     0,     0,     0,    30,     0,     0,     0,
       0,     0,     0,     0,   439,     0,     0,    38,   410,     0,
       6,     0,     9,     0,    10,     0,     0,     0,     0,     0,
       0,   366,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,    42,     0,     0,   119,
       0,   131,     0,     0,   150,     0,     0,     0,     3,   157,
      45,     0,     0,    46,     0,     0,   153,     0,     0,     0,
       0,     0,   134,     0,   137,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,   404,   406,     0,     0,   208,
       0,     0,     0,   243,   440,     0,   274,   416,   421,   422,
     436,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   321,   322,   325,   326,   319,
     320,     0,   323,   324,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   337,   339,   340,   342,   344,   345,   347,
     349,   351,   352,   357,   354,   355,   356,   364,   368,   378,
     372,   373,   374,   375,   376,   377,   370,   371,   369,   401,
       0,   398,    53,     0,    87,    88,    89,    90,    91,    50,
       0,     0,   384,     0,     0,    15,     0,     0,     0,     0,
     426,   431,   432,    55,     0,   393,     0,     0,   382,     0,
     380,   410,     0,     0,     0,   310,   311,   312,   313,   314,
     308,   309,    37,     0,   409,     0,     0,     0,   413,   435,
       0,     0,     0,     0,    27,     0,    28,     0,    29,     0,
     160,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   440,     0,
       0,   122,   440,     0,     0,   344,   345,   253,   254,     0,
     267,   269,   271,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   440,     0,   440,     0,     0,
       0,     0,   108,   109,   110,   111,   307,    99,     0,     0,
     208,     0,     0,     0,     0,     0,     0,   441,     0,     0,
       0,     0,     0,     0,     0,     0,   300,     0,     0,     0,
     301,     0,     0,   302,     0,     0,     0,     0,     0,     0,
       0,     0,   275,    57,    59,    61,    64,    62,    63,    58,
      60,    70,    72,    74,    77,    75,    76,    71,    73,     0,
       0,   394,     0,     0,     0,     0,   167,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   392,     0,   388,
       0,     0,   247,     0,     0,   426,   431,   432,    36,     5,
       0,     0,    11,     0,     0,     0,     0,    20,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   367,
       0,    78,    81,    80,    84,    82,    83,    79,    40,     0,
      85,    86,    43,   116,   121,   120,     0,     0,   130,   147,
       0,   149,     0,     0,     0,   256,     0,     0,   156,     0,
       0,     0,     0,     0,   168,     0,    48,    49,    32,    31,
       0,     0,   151,   433,   434,     0,     0,   132,   135,   136,
     139,   140,     0,     0,   206,   207,     0,     0,     0,     0,
     204,   165,     0,     0,     0,     0,     0,     0,     0,   242,
     442,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   284,   291,   298,   299,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,   400,     0,
     397,     0,     0,    52,     0,    54,   166,     0,    17,    16,
     429,   428,   427,     0,     0,     0,   391,   386,   387,     0,
     383,     0,     0,     0,   250,   249,     0,     0,     0,    22,
       0,    23,     0,    26,   159,     0,    34,    33,     0,   442,
     442,     0,   123,     0,   124,     0,     0,     0,   263,     0,
     257,   258,   155,   259,   255,     0,   268,   270,   272,     0,
       0,   171,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   169,     0,     0,    68,    69,     0,     0,     0,   203,
       0,     0,     0,     0,     0,     0,     0,     0,   164,   162,
       0,     0,     0,     0,     0,     0,     0,   426,   432,   295,
       0,     0,     0,     0,     0,     0,   303,   304,   305,   306,
     276,   278,   280,   283,   281,   282,   277,   279,   399,   396,
     395,    51,   442,    65,    66,    67,   248,     0,    13,    12,
      21,     0,     0,     0,     0,   158,     7,     0,    41,    44,
     118,   117,   440,     0,   128,   148,   145,   146,   265,     0,
     260,     0,     0,     0,     0,     0,   172,   173,   170,     0,
       0,     0,     0,     0,     0,     0,     0,   154,   152,   133,
       0,     0,   144,     0,     3,   238,     0,   235,     0,     0,
       0,     0,   214,     0,     0,     0,     0,   209,     0,     0,
       0,     0,   210,   211,     0,   161,   163,   244,   245,   246,
       0,   167,     0,     0,     0,   285,   287,   290,   288,   289,
     286,    18,   442,   252,   251,     0,    24,     0,   125,   126,
     264,     0,   261,   262,     0,     0,     0,   174,   176,   178,
     181,   179,   180,   175,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   225,     0,     0,     0,   226,
       0,     0,     0,     0,     0,   216,     0,     0,     0,     0,
     215,   212,   213,     0,   241,   294,   293,   166,     0,     0,
      14,   442,     8,     0,   129,   266,   182,   184,   183,     0,
     143,   237,     0,   186,     0,     0,     0,   185,     0,   234,
       0,   223,   224,   218,   221,   222,   217,     0,     0,   227,
       0,     0,     0,   228,     0,     0,     0,   292,   296,   297,
      25,   127,     0,   236,     0,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   187,   233,   231,   232,
     220,   229,   230,   219,   240,     0,     0,     0,     0,     0,
     190,   191,   188,     0,     0,     0,     0,     0,     0,     0,
       0,   239,   142,     0,     0,     0,   192,   194,   196,   199,
     197,   198,   193,   195,   200,   202,   201
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -769,  -769,     3,  -769,  -505,  -658,  -769,  -769,  -769,  -769,
    -769,   358,  -769,  -769,  -769,  -769,  -769,  -278,  -768,  -769,
    -769,  -769,   842,  -769,  -769,  -443,   923,  -343,   327,  1105,
      -2,  -210,  -769,   706,  1178,  -769,   621,   -11,  1176,  1179,
     921,    -7,  -769,  -769,  -237,  -769,  -769,    23,  -769
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   187,    61,    62,   511,    63,    64,    65,    66,
      67,   921,    68,    69,   530,    70,    71,   319,   925,    72,
      73,   348,   551,    74,   556,   261,   481,   524,   115,   166,
     122,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   292,   429,    89,    90,   123,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,   431,   391,     2,   664,   792,   928,   553,   116,   548,
     490,   141,   107,   620,   929,   140,   675,   549,   624,   312,
     132,   134,   136,   138,   139,   517,   518,   144,   146,   519,
     149,   517,   518,   153,   155,   519,   707,   708,    24,   304,
     709,   170,   672,   178,   173,   690,   691,   673,   157,   182,
     183,   517,   518,    60,   159,   519,   305,   185,   517,   518,
     930,     3,   519,   927,   279,   517,   518,   313,   351,   519,
      60,   160,   162,   321,   176,   304,   676,   677,   280,   228,
     520,   521,   522,   117,   228,   281,   520,   521,   522,   163,
     322,   177,   305,   240,   241,   242,   243,   244,   245,   282,
     105,   250,   252,   931,   254,   255,   520,   521,   522,   900,
     226,   227,   710,   520,   521,   522,   352,   932,   106,   275,
     520,   521,   522,   517,   518,   248,   550,   519,    60,   293,
     777,   303,   262,   308,   150,   310,   894,   304,   306,   778,
     692,   307,   523,    60,   276,   732,   645,   147,   552,   228,
     331,   148,   333,   161,   305,   338,   568,   339,   340,   751,
     341,   342,   670,   343,   344,   345,   343,   346,   644,   901,
     902,   164,   323,   569,   306,   713,  1014,   307,   520,   521,
     522,   907,   987,   223,   224,   368,   369,   370,   371,   372,
     373,   374,   375,   376,    60,   377,   378,   379,   380,   381,
     382,   383,   384,   289,   707,   708,   256,   290,   709,   257,
     278,   714,   715,    60,   151,   407,   408,   315,   316,  1031,
     716,   717,   718,  1039,   165,   413,   414,   415,   416,   417,
     933,   174,   571,   517,   518,   103,   306,   519,   934,   307,
     991,   321,   186,  1091,   440,   678,  1001,   443,   277,   572,
     258,   278,  1006,   104,  1002,   161,   228,   264,   322,   441,
    1007,   462,   570,   464,  1063,   465,   466,   467,   468,   469,
     470,   471,  1077,   180,   711,   265,   474,   475,   719,   477,
     829,   482,   321,   273,   935,   181,   890,  1084,   520,   521,
     522,   304,   496,   497,   498,   499,   500,   501,   502,   322,
    1003,   263,   505,   506,   515,   516,  1008,   317,   305,   108,
    1024,   109,   318,   175,   124,   125,   126,  1101,   127,   526,
     527,   525,   528,   529,   327,   503,   532,   936,   534,    75,
     507,   164,   283,   538,   539,   128,   514,   444,   573,   129,
     323,   937,   328,  1004,   179,   637,   284,   734,   638,  1009,
    1055,    93,    94,   445,   142,   143,   446,  1005,   329,   563,
     564,   565,   567,  1010,   156,   158,   784,   105,   631,   321,
     489,   171,   172,   457,   635,   318,   330,   483,   558,   102,
     658,   323,   103,   278,   632,   561,   322,   633,   908,   747,
     306,   484,   574,   307,   725,   575,   576,   295,   726,   485,
     560,   802,   577,   225,   727,   578,   579,   728,   729,   580,
     581,   730,   487,   486,   813,   229,   662,   814,   815,   278,
     108,   108,   109,   109,   816,   335,   488,   817,   818,   236,
     130,   819,   820,   239,   838,   839,   845,   449,   605,   450,
     336,   142,   609,   246,   903,   611,   612,   295,   904,   517,
     518,   846,   237,   519,   297,   622,   604,   298,   299,   474,
     320,   905,   238,   309,   607,   311,   314,   733,   323,   610,
     831,   923,   613,   253,   630,  1092,   924,   618,   185,   621,
     332,   640,   334,   642,   449,   643,   625,   232,   453,   646,
     647,   205,   542,   301,   759,   849,   302,   278,   582,   856,
     731,   233,   234,   235,   520,   521,   522,   885,   667,   263,
     543,   266,  1068,   205,    75,  1069,  1070,   857,   650,   926,
     821,   288,  1071,   685,   924,  1072,  1073,   659,  1087,  1074,
    1075,   295,  1088,   285,   286,   287,   803,   804,   297,   703,
     805,   298,   299,   301,   291,  1089,   302,  1013,   320,   544,
     906,   294,   924,   324,   295,   296,   858,   325,   722,   723,
     724,   297,   945,   946,   298,   299,   735,   736,   300,   737,
     738,   326,   739,   740,   741,   742,   743,   744,   745,   746,
     988,   347,   545,   992,   188,   230,   231,   189,   190,   859,
     356,   806,   807,   808,   191,   349,   591,   192,   193,   350,
     944,   194,   195,   860,   434,   206,   207,   208,   209,   354,
     355,   205,   763,   764,   765,   210,   211,   212,  1076,   592,
     278,   593,   762,   749,   353,   752,   861,   301,   862,   772,
     302,   757,   774,   775,  1090,   594,   595,   213,   214,   780,
     596,   197,   863,   864,   198,   199,   788,   865,   442,   278,
     301,   200,   435,   302,   201,   202,   773,   418,   203,   204,
     781,   436,   597,   782,   793,   419,   598,   420,   421,   866,
    1056,   651,   798,   867,   547,   653,   422,   423,   955,   809,
     810,   812,   956,   196,   424,   425,   432,   433,   822,   654,
     655,   656,   426,   427,   428,   437,   957,   958,   959,   657,
     827,   830,   418,   438,   591,   430,   960,   592,   593,   594,
     419,   977,   420,   421,   978,   979,   980,   879,   447,   448,
     278,   422,   423,   850,   851,   852,   853,   854,   855,   424,
     425,   439,   886,   966,   595,   887,   278,   426,   427,   428,
     205,   981,     4,     5,     6,     7,   449,   451,   847,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
    1022,   596,   597,   278,   418,   453,   598,   876,   982,   983,
     114,    28,   419,   984,   420,   421,   458,   459,   755,   651,
      33,   509,   510,   422,   423,  1015,  1026,   452,    41,    42,
      43,   424,   425,    46,   877,   460,   461,   898,   454,   426,
     427,   428,    50,    51,   938,   884,   455,   939,    52,   940,
     941,   909,   910,   911,   912,   913,   914,   915,   916,    93,
      94,   267,   268,   269,   270,   271,   922,   653,   400,   401,
     402,   456,   272,   463,  1027,   663,   665,    54,    55,   668,
     669,   671,   657,   950,   404,   405,   406,   102,   873,  1028,
     875,    56,    75,   591,   592,  1058,   472,  1059,    57,   491,
    1106,  1107,   697,   698,   699,    59,   184,   593,   594,   952,
      60,   595,   473,   278,  1108,  1109,   493,   494,  1110,   963,
     964,    92,   492,    93,    94,    95,    96,    97,    98,    99,
     513,   596,   597,   598,   791,   510,   100,   970,  1111,  1112,
    1113,   974,   975,   976,   354,   495,   965,   354,   504,   101,
     967,   102,   385,   386,   387,   388,   389,   390,   985,   392,
     393,   394,   395,   396,   397,   398,   399,   990,   651,   996,
       4,     5,     6,     7,  1000,  1114,   536,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,   653,   657,
     540,  1018,  1019,   508,   355,  1115,  1116,   531,   114,    28,
     215,   216,   217,   218,   219,   220,   512,   355,    33,  1025,
     535,   355,   221,   537,   355,   541,    41,    42,    43,   533,
     832,    46,   222,   833,   834,   835,   836,   542,  1038,   543,
      50,    51,   354,   608,   794,   546,    52,   679,   680,   712,
     680,   562,  1050,   720,   278,   754,   278,  1054,    92,   544,
      93,    94,    95,    96,    97,    98,    99,   554,   823,   756,
     278,   766,   278,   100,   545,    54,    55,   922,   767,   459,
     768,   278,  1064,  1065,  1067,   583,   559,   555,   102,    56,
     770,   459,   354,   771,   776,   638,    57,   785,   680,   789,
     278,   584,   145,    59,   585,   790,   278,   586,    60,   566,
     587,  -443,  -443,   840,   680,   222,  1093,  1094,  1095,  1096,
    1097,  1098,  1099,  1100,   841,   680,   868,   278,   869,   459,
     870,   278,   872,   278,   588,  1103,  1104,  1105,   354,   883,
     892,   355,   893,   510,   962,   278,  1017,   278,  1021,   278,
    -443,  -443,   589,  -443,  -443,  -443,  -443,  -443,  -443,  -443,
    -443,  -443,  -443,  -443,  -443,  -443,  1023,   510,   891,  -443,
    1032,   680,   895,   896,   897,  1040,   680,  -443,  -443,  -443,
    -443,   599,  -443,  1085,   680,  -443,  -443,  -443,  -443,  -443,
    -443,   590,  -443,  -443,  -443,   591,  -443,   167,   169,   917,
     592,   411,   412,   919,   593,     4,     5,     6,     7,   594,
     595,   596,   357,   358,   359,   360,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   361,   362,    24,   597,
     598,    25,    26,    27,    28,   363,   364,    31,    32,   228,
     600,   601,   602,    33,    34,    35,    36,    37,    38,   365,
     366,    41,    42,    43,    44,    45,    46,    47,   614,   615,
     616,    48,    49,   623,   626,    50,    51,   627,   628,   968,
     969,    52,   634,   648,   649,    53,   629,   639,   651,   652,
     674,   653,   654,   655,   656,   657,   660,   693,   661,   695,
     686,   687,   688,   689,   702,   318,   704,   700,   705,   701,
      54,    55,   694,   696,   721,   753,   755,   758,   760,   761,
     769,   779,   783,   786,    56,   787,   795,   796,   797,   799,
     800,    57,   801,   824,   825,   826,   828,    58,    59,   837,
     842,   843,   844,    60,   367,   848,   871,   873,   874,   875,
     878,   880,   881,   882,   888,   889,   899,   920,   942,   918,
     943,   947,   948,   949,   953,   954,   961,   971,   972,   973,
     989,   986,   995,  1030,   993,   994,   997,    75,   998,   924,
     999,  1016,  1011,  1012,  1033,     4,     5,     6,     7,  1020,
    1029,  1041,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   681,   682,    24,  1042,
    1061,    25,    26,    27,    28,    29,   683,    31,    32,  1047,
    1043,  1044,  1045,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,  1046,  1048,
    1049,    48,    49,  1051,  1052,    50,    51,  1062,  1053,  1060,
    1078,    52,   706,  1057,  1079,    53,  1080,  1081,  1082,  1083,
     403,     0,  1086,   636,   409,     0,     0,     0,   410,     0,
       0,     0,     0,  1102,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,    58,    59,     4,
       5,     6,     7,    60,   684,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
    1034,  1035,    24,     0,     0,    25,    26,    27,    28,    29,
    1036,    31,    32,     0,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,    48,    49,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,    53,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,  1037,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,   145,    59,    46,     0,     0,     0,
      60,   811,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    56,     0,     0,     0,     0,     0,
       0,    57,     0,     0,     0,     0,     0,   145,    59,     4,
       5,     6,     7,    60,  1066,     0,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,     0,     0,    25,    26,    27,    28,    29,
      30,    31,    32,     0,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,     0,     0,     0,    48,    49,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,    53,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,   120,   121,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   145,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   247,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,   259,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,   251,
       0,    41,    42,    43,     0,    59,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   260,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,   337,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   274,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,    59,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,   186,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,    59,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   557,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   603,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   606,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,   259,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   617,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,   478,     7,     0,     0,     0,   118,   119,   112,   113,
     479,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   619,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   184,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,   259,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   748,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   750,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   119,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   951,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,   131,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   133,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   135,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   119,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   137,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   119,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
      59,    46,     0,     0,     0,    60,     0,     0,     0,     0,
      50,    51,     0,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   119,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   152,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   119,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   154,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   119,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,    59,
      46,     0,     0,     0,   168,     0,     0,     0,     0,    50,
      51,     0,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     119,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   249,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,   478,     7,     0,
       0,     0,   118,   119,   112,   113,   479,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   476,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   119,   112,   113,   259,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,   480,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   119,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,    59,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   119,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   641,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   110,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,     0,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   666,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
      59,     0,     0,     0,     0,    60
};

static const yytype_int16 yycheck[] =
{
       2,   238,   212,     0,   509,   663,    14,   350,    15,    14,
     288,    14,   111,   456,    22,    26,    10,    22,   461,    80,
      22,    23,    24,    25,    26,    10,    11,    29,    30,    14,
      32,    10,    11,    35,    36,    14,    56,    57,    26,    26,
      60,    43,    80,    50,    46,    37,    38,    80,   126,    56,
      57,    10,    11,   131,    26,    14,    43,    59,    10,    11,
      68,     0,    14,   831,   111,    10,    11,   128,    14,    14,
     131,    43,    26,    26,    14,    26,    70,    71,   125,   117,
      65,    66,    67,    14,   117,   111,    65,    66,    67,    43,
      43,    31,    43,    95,    96,    97,    98,    99,   100,   125,
     113,   103,   104,   111,   106,   107,    65,    66,    67,    10,
      78,    79,   132,    65,    66,    67,    62,   125,   131,   121,
      65,    66,    67,    10,    11,   102,   131,    14,   131,   131,
       5,   133,   109,   135,    12,   137,   794,    26,   125,    14,
     132,   128,   127,   131,   121,   132,   489,    10,   127,   117,
     152,    14,   154,   125,    43,   157,    26,   159,   160,   602,
     162,   163,    80,   165,   166,   167,   168,   169,   127,    70,
      71,   125,   125,    43,   125,   127,   944,   128,    65,    66,
      67,   132,   127,   115,   116,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   131,   197,   198,   199,   200,   201,
     202,   203,   204,    10,    56,    57,    14,    14,    60,    17,
     128,   554,   555,   131,    12,   226,   227,    78,    79,   987,
      74,    75,    76,   991,   131,   232,   233,   234,   235,   236,
      14,    14,    26,    10,    11,   111,   125,    14,    22,   128,
     127,    26,    72,   132,   111,   523,    14,   249,   125,    43,
      58,   128,    14,   129,    22,   125,   117,   113,    43,   126,
      22,   263,   132,   265,  1032,   267,   268,   269,   270,   271,
     272,   273,  1040,    10,   552,   131,   278,   279,   132,   281,
     132,   283,    26,   111,    68,    10,   791,  1055,    65,    66,
      67,    26,   294,   295,   296,   297,   298,   299,   300,    43,
      68,   129,   304,   305,   315,   316,    68,   126,    43,   124,
     968,   126,   131,    14,    10,    11,    12,  1085,    14,   321,
     322,   318,   324,   325,   113,   302,   328,   111,   330,     2,
     307,   125,   111,   335,   336,    31,   313,   109,   132,    35,
     125,   125,   131,   111,    14,   125,   125,   132,   128,   111,
     127,    98,    99,   125,    27,    28,   128,   125,   113,   361,
     362,   363,   364,   125,    37,    38,   644,   113,   109,    26,
     126,    44,    45,    77,    77,   131,   131,   111,   355,   126,
     125,   125,   111,   128,   125,   131,    43,   128,   132,   599,
     125,   125,    26,   128,    26,    29,    30,    29,    30,   111,
     129,   679,    36,   105,    36,    39,    40,    39,    40,    43,
      44,    43,   111,   125,    26,   108,   125,    29,    30,   128,
     124,   124,   126,   126,    36,    29,   125,    39,    40,   110,
     126,    43,    44,    14,   712,   713,   111,   128,   440,   130,
      44,   114,   444,    14,    26,   447,   448,    29,    30,    10,
      11,   126,   124,    14,    36,   457,   433,    39,    40,   461,
     125,    43,   124,   136,   441,   138,   139,   132,   125,   446,
     707,   126,   449,    14,   476,   132,   131,   454,   480,   456,
     153,   483,   155,   485,   128,   487,   130,   106,   125,   491,
     492,   125,   125,   125,   125,   132,   128,   128,   132,   132,
     132,   120,   121,   122,    65,    66,    67,   785,   510,   129,
     125,   125,    26,   125,   187,    29,    30,   132,   495,   126,
     132,    14,    36,   525,   131,    39,    40,   504,    26,    43,
      44,    29,    30,    10,    11,    12,    10,    11,    36,   541,
      14,    39,    40,   125,    10,    43,   128,   126,   125,   125,
     132,    26,   131,   111,    29,    30,   132,   111,   560,   561,
     562,    36,   840,   841,    39,    40,   568,   569,    43,   571,
     572,   131,   574,   575,   576,   577,   578,   579,   580,   581,
     923,    53,   125,   926,    26,   118,   119,    29,    30,   132,
     132,    65,    66,    67,    36,   131,   125,    39,    40,   126,
     837,    43,    44,   132,   125,    81,    82,    83,    84,   127,
     128,   125,   614,   615,   616,    91,    92,    93,   132,   125,
     128,   125,   130,   600,   131,   602,   132,   125,   132,   631,
     128,   608,   634,   635,   132,   125,   125,   113,   114,   641,
     125,    26,   132,   132,    29,    30,   648,   132,   127,   128,
     125,    36,   125,   128,    39,    40,   633,    14,    43,    44,
     125,   125,   125,   128,   666,    22,   125,    24,    25,   132,
    1013,   125,   674,   132,   347,   125,    33,    34,   132,   681,
     682,   683,   132,   125,    41,    42,   125,   126,   690,   125,
     125,   125,    49,    50,    51,   125,   132,   132,   132,   125,
     702,    58,    14,   125,   125,    17,   132,   125,   125,   125,
      22,   132,    24,    25,   132,   132,   132,   125,   118,   119,
     128,    33,    34,   725,   726,   727,   728,   729,   730,    41,
      42,   125,   125,   125,   125,   128,   128,    49,    50,    51,
     125,   132,     3,     4,     5,     6,   128,   114,   130,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
     125,   125,   125,   128,    14,   125,   125,   769,   132,   132,
      31,    32,    22,   132,    24,    25,   127,   128,   125,   125,
      41,    27,    28,    33,    34,   132,   132,   132,    49,    50,
      51,    41,    42,    54,   771,   127,   128,   799,   126,    49,
      50,    51,    63,    64,    10,   782,    14,    13,    69,    15,
      16,   813,   814,   815,   816,   817,   818,   819,   820,    98,
      99,   100,   101,   102,   103,   104,   828,   125,     7,     8,
       9,   126,   111,    14,   132,   508,   509,    98,    99,   512,
     513,   514,   125,   845,   223,   224,   225,   126,   125,   132,
     125,   112,   525,   125,   125,   132,   125,   132,   119,   111,
     132,   132,   535,   536,   537,   126,   127,   125,   125,   846,
     131,   125,   127,   128,   132,   132,   127,   128,   132,   881,
     882,    96,   111,    98,    99,   100,   101,   102,   103,   104,
      10,   125,   125,   125,    27,    28,   111,   899,   132,   132,
     132,   903,   904,   905,   127,   128,   883,   127,   128,   124,
     887,   126,   206,   207,   208,   209,   210,   211,   920,   213,
     214,   215,   216,   217,   218,   219,   220,   924,   125,   931,
       3,     4,     5,     6,   936,   132,    27,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,   125,   125,
      10,   953,   954,   127,   128,   132,   132,    14,    31,    32,
      85,    86,    87,    88,    89,    90,   127,   128,    41,   971,
     127,   128,    97,   127,   128,   125,    49,    50,    51,    14,
      10,    54,   107,    13,    14,    15,    16,   125,   990,   125,
      63,    64,   127,   128,   667,   132,    69,   127,   128,   127,
     128,   111,  1004,   127,   128,   127,   128,  1009,    96,   125,
      98,    99,   100,   101,   102,   103,   104,   126,   691,   127,
     128,   127,   128,   111,   125,    98,    99,  1029,   127,   128,
     127,   128,  1034,  1035,  1036,   125,   124,   126,   126,   112,
     127,   128,   127,   128,   127,   128,   119,   127,   128,   127,
     128,   125,   125,   126,   125,   127,   128,   125,   131,   132,
     125,    39,    40,   127,   128,   107,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,   127,   128,   127,   128,   127,   128,
     127,   128,   127,   128,   125,  1087,  1088,  1089,   127,   128,
     127,   128,    27,    28,   127,   128,   127,   128,   127,   128,
      78,    79,   125,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    27,    28,   791,    97,
     127,   128,   795,   796,   797,   127,   128,   105,   106,   107,
     108,    94,   110,   127,   128,   113,   114,   115,   116,   117,
     118,   125,   120,   121,   122,   125,   124,    42,    43,   822,
     125,   230,   231,   826,   125,     3,     4,     5,     6,   125,
     125,   125,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   125,
     125,    29,    30,    31,    32,    33,    34,    35,    36,   117,
     126,    14,   126,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   111,   111,
     111,    59,    60,    14,   114,    63,    64,   132,   125,   892,
     893,    69,    77,   111,    10,    73,   125,   125,   125,   125,
     111,   125,   125,   125,   125,   125,   125,   114,   125,   114,
     125,   125,   125,   125,   111,   131,    14,   125,    14,   125,
      98,    99,   132,   132,    14,   125,   125,   125,   130,   130,
      77,   125,   125,   125,   112,   125,    10,    10,    10,   111,
      10,   119,    10,   125,   125,    27,   125,   125,   126,    56,
      14,    14,    14,   131,   132,   132,   125,   125,   125,   125,
     125,   125,    77,    77,   125,   125,   111,   125,   125,   132,
     125,     7,     7,     7,   111,   111,   125,   111,    10,    10,
      72,   127,   125,   986,    14,    14,    14,   990,    14,   131,
     125,   132,   125,   125,   132,     3,     4,     5,     6,   125,
     125,   125,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   125,
    1023,    29,    30,    31,    32,    33,    34,    35,    36,    14,
     125,   125,   125,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   125,    14,
     125,    59,    60,    14,    14,    63,    64,  1029,   125,   125,
     125,    69,   550,   132,   125,    73,   125,   125,   125,   125,
     222,    -1,   127,   480,   228,    -1,    -1,    -1,   229,    -1,
      -1,    -1,    -1,  1086,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,   125,   126,     3,
       4,     5,     6,   131,   132,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    73,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,   125,   126,    54,    -1,    -1,    -1,   131,   132,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,   125,   126,    54,    -1,    -1,    -1,
     131,   132,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,    -1,    -1,   125,   126,     3,
       4,     5,     6,   131,   132,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    73,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,
      41,    -1,    -1,    -1,    -1,   119,    -1,    -1,    49,    50,
      51,   125,   126,    54,    -1,    -1,    -1,   131,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,    -1,
      -1,    49,    50,    51,   125,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    49,    50,    51,   125,   126,    54,
      -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      41,    -1,    -1,    -1,   119,    -1,    -1,    -1,    49,    50,
      51,   126,   127,    54,    -1,    -1,   131,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    98,    99,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   112,    -1,    41,    -1,    -1,    -1,    -1,   119,   120,
      -1,    49,    50,    51,    -1,   126,    54,    -1,    -1,    -1,
     131,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
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
      20,    -1,    -1,    23,    98,    99,    -1,    -1,    -1,    -1,
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
      -1,    -1,    69,    -1,    -1,    72,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    98,    99,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   112,    -1,    41,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    49,    50,    51,    -1,   126,
      54,    -1,    -1,    -1,   131,    -1,    -1,    -1,    -1,    63,
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
      99,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   112,    -1,    41,    -1,    -1,    -1,    -1,
     119,    -1,    -1,    49,    50,    51,    -1,   126,    54,    -1,
      -1,    -1,   131,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   112,    -1,    -1,    -1,
      -1,    -1,    -1,   119,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,   131
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
     127,   158,   180,   129,   113,   131,   125,   100,   101,   102,
     103,   104,   111,   111,   127,   163,   180,   125,   128,   111,
     125,   111,   125,   111,   125,    10,    11,    12,    14,    10,
      14,    10,   176,   163,    26,    29,    30,    36,    39,    40,
      43,   125,   128,   163,    26,    43,   125,   128,   163,   161,
     163,   161,    80,   128,   161,    78,    79,   126,   131,   150,
     125,    26,    43,   125,   111,   111,   131,   113,   131,   113,
     131,   163,   161,   163,   161,    29,    44,    23,   163,   163,
     163,   163,   163,   163,   163,   163,   163,    53,   154,   131,
     126,    14,    62,   131,   127,   128,   132,    10,    11,    12,
      13,    24,    25,    33,    34,    47,    48,   132,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   166,   166,   166,   166,   166,
     166,   164,   166,   166,   166,   166,   166,   166,   166,   166,
       7,     8,     9,   167,   169,   169,   169,   170,   170,   171,
     172,   173,   173,   174,   174,   174,   174,   174,    14,    22,
      24,    25,    33,    34,    41,    42,    49,    50,    51,   177,
      17,   177,   125,   126,   125,   125,   125,   125,   125,   125,
     111,   126,   127,   163,   109,   125,   128,   118,   119,   128,
     130,   114,   132,   125,   126,    14,   126,    77,   127,   128,
     127,   128,   163,    14,   163,   163,   163,   163,   163,   163,
     163,   163,   125,   127,   163,   163,   126,   163,     5,    14,
     126,   159,   163,   111,   125,   111,   125,   111,   125,   126,
     150,   111,   111,   127,   128,   128,   163,   163,   163,   163,
     163,   163,   163,   180,   128,   163,   163,   180,   127,    27,
      28,   138,   127,    10,   180,   170,   170,    10,    11,    14,
      65,    66,    67,   127,   160,   135,   163,   163,   163,   163,
     147,    14,   163,    14,   163,   127,    27,   127,   163,   163,
      10,   125,   125,   125,   125,   125,   132,   161,    14,    22,
     131,   155,   127,   160,   126,   126,   157,   127,   180,   124,
     129,   131,   111,   163,   163,   163,   132,   163,    26,    43,
     132,    26,    43,   132,    26,    29,    30,    36,    39,    40,
      43,    44,   132,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,    94,
     126,    14,   126,   127,   180,   163,   127,   180,   128,   163,
     180,   163,   163,   180,   111,   111,   111,   127,   180,   127,
     158,   180,   163,    14,   158,   130,   114,   132,   125,   125,
     163,   109,   125,   128,    77,    77,   159,   125,   128,   125,
     163,   126,   163,   163,   127,   160,   163,   163,   111,    10,
     180,   125,   125,   125,   125,   125,   125,   125,   125,   180,
     125,   125,   125,   161,   137,   161,   126,   163,   161,   161,
      80,   161,    80,    80,   111,    10,    70,    71,   150,   127,
     128,    24,    25,    34,   132,   163,   125,   125,   125,   125,
      37,    38,   132,   114,   132,   114,   132,   161,   161,   161,
     125,   125,   111,   163,    14,    14,   155,    56,    57,    60,
     132,   150,   127,   127,   160,   160,    74,    75,    76,   132,
     127,    14,   163,   163,   163,    26,    30,    36,    39,    40,
      43,   132,   132,   132,   132,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   164,   127,   180,
     127,   158,   180,   125,   127,   125,   127,   180,   125,   125,
     130,   130,   130,   163,   163,   163,   127,   127,   127,    77,
     127,   128,   163,   180,   163,   163,   127,     5,    14,   125,
     163,   125,   128,   125,   150,   127,   125,   125,   163,   127,
     127,    27,   138,   163,   161,    10,    10,    10,   163,   111,
      10,    10,   150,    10,    11,    14,    65,    66,    67,   163,
     163,   132,   163,    26,    29,    30,    36,    39,    40,    43,
      44,   132,   163,   161,   125,   125,    27,   163,   125,   132,
      58,   177,    10,    13,    14,    15,    16,    56,   150,   150,
     127,   127,    14,    14,    14,   111,   126,   130,   132,   132,
     163,   163,   163,   163,   163,   163,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132,   132,   127,   127,
     127,   125,   127,   125,   125,   125,   163,   180,   125,   125,
     125,    77,    77,   128,   180,   150,   125,   128,   125,   125,
     137,   161,   127,    27,   138,   161,   161,   161,   163,   111,
      10,    70,    71,    26,    30,    43,   132,   132,   132,   163,
     163,   163,   163,   163,   163,   163,   163,   161,   132,   161,
     125,   144,   163,   126,   131,   151,   126,   151,    14,    22,
      68,   111,   125,    14,    22,    68,   111,   125,    10,    13,
      15,    16,   125,   125,   177,   150,   150,     7,     7,     7,
     163,   127,   180,   111,   111,   132,   132,   132,   132,   132,
     132,   125,   127,   163,   163,   180,   125,   180,   161,   161,
     163,   111,    10,    10,   163,   163,   163,   132,   132,   132,
     132,   132,   132,   132,   132,   163,   127,   127,   160,    72,
     135,   127,   160,    14,    14,   125,   163,    14,    14,   125,
     163,    14,    22,    68,   111,   125,    14,    22,    68,   111,
     125,   125,   125,   126,   151,   132,   132,   127,   163,   163,
     125,   127,   125,    27,   138,   163,   132,   132,   132,   125,
     161,   151,   127,   132,    24,    25,    34,   132,   163,   151,
     127,   125,   125,   125,   125,   125,   125,    14,    14,   125,
     163,    14,    14,   125,   163,   127,   160,   132,   132,   132,
     125,   161,   144,   151,   163,   163,   132,   163,    26,    29,
      30,    36,    39,    40,    43,    44,   132,   151,   125,   125,
     125,   125,   125,   125,   151,   127,   127,    26,    30,    43,
     132,   132,   132,   163,   163,   163,   163,   163,   163,   163,
     163,   151,   161,   163,   163,   163,   132,   132,   132,   132,
     132,   132,   132,   132,   132,   132,   132
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
     136,   136,   136,   136,   136,   136,   137,   137,   137,   137,
     137,   137,   137,   137,   138,   138,   138,   138,   138,   138,
     139,   139,   140,   140,   140,   140,   141,   141,   142,   142,
     142,   143,   143,   143,   144,   145,   145,   145,   145,   145,
     145,   146,   146,   147,   147,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   149,   149,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   151,   151,   151,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   151,   151,   152,   153,   154,   154,   154,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   156,   157,   157,   157,   157,   158,   158,   159,
     159,   159,   159,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   162,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   164,   164,   165,
     165,   165,   165,   165,   165,   165,   165,   165,   165,   165,
     165,   165,   165,   165,   165,   165,   165,   166,   166,   167,
     167,   167,   168,   168,   169,   169,   169,   170,   170,   171,
     171,   172,   172,   172,   173,   173,   173,   173,   173,   174,
     174,   174,   174,   174,   175,   175,   176,   176,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   178,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   180,   180,
     180,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   181,   181,   181,   181
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
       7,     5,     5,     0,     4,     4,     4,     3,     5,     3,
       3,     5,     5,     1,     1,     3,     2,     3,     3,     3,
       4,     5,     5,     3,     5,     4,     6,     1,     3,     1,
       3,     1,     3,     3,     3,     4,     6,     6,     6,     6,
       6,     6,     6,     6,     5,     7,     7,     7,     7,     7,
       7,     5,     9,     8,     8,     6,     9,     9,     5,     5,
       4,     4,     4,     6,     6,     6,     6,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     5,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     2,
       2,     2,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     4,     6,     4,     3,     6,     6,     5,     3,
       3,     6,     5,     4,     4,     6,     6,     5,     3,     6,
       5,     3,     2,     3,     3,     2,     3,     1,     1,     3,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     4,     6,     6,     6,
       1,     4,     4,     5,     5,     1,     1,     1,     1,     2,
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

  case 48: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 516 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 3190 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 519 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 3197 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 522 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3203 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 524 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3212 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 529 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3220 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 533 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3228 "raku.tab.c"
    break;

  case 54: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 537 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3237 "raku.tab.c"
    break;

  case 55: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 542 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3245 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall ';'  */
#line 545 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3251 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 547 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3257 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 549 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3263 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 551 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3269 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 553 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3275 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 555 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3282 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 558 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3288 "raku.tab.c"
    break;

  case 63: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 560 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3294 "raku.tab.c"
    break;

  case 64: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 562 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3300 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 564 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3307 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3314 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 570 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3321 "raku.tab.c"
    break;

  case 68: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 573 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3328 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 576 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3335 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_IF expr ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3341 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNLESS expr ';'  */
#line 581 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3347 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_WHILE expr ';'  */
#line 583 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3353 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_UNTIL expr ';'  */
#line 585 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3359 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_FOR expr ';'  */
#line 587 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3366 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_WITH expr ';'  */
#line 590 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3372 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITHOUT expr ';'  */
#line 592 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3378 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_GIVEN expr ';'  */
#line 594 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3384 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 596 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3391 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 599 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3398 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 602 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3406 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 606 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3413 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 609 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3419 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 611 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3425 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 613 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3431 "raku.tab.c"
    break;

  case 85: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 615 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3438 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 618 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3445 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 621 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3452 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 624 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3459 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 627 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3466 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 630 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3473 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 633 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3480 "raku.tab.c"
    break;

  case 92: /* stmt: expr ';'  */
#line 635 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3486 "raku.tab.c"
    break;

  case 93: /* stmt: ';'  */
#line 636 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3492 "raku.tab.c"
    break;

  case 94: /* stmt: if_stmt  */
#line 637 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3498 "raku.tab.c"
    break;

  case 95: /* stmt: while_stmt  */
#line 638 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3504 "raku.tab.c"
    break;

  case 96: /* stmt: for_stmt  */
#line 639 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3510 "raku.tab.c"
    break;

  case 97: /* stmt: given_stmt  */
#line 640 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3516 "raku.tab.c"
    break;

  case 98: /* stmt: KW_TRY block  */
#line 642 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3522 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block KW_CATCH block  */
#line 644 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3528 "raku.tab.c"
    break;

  case 100: /* stmt: KW_CATCH block  */
#line 646 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3534 "raku.tab.c"
    break;

  case 101: /* stmt: block  */
#line 648 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3540 "raku.tab.c"
    break;

  case 102: /* stmt: unless_stmt  */
#line 649 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3546 "raku.tab.c"
    break;

  case 103: /* stmt: until_stmt  */
#line 650 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3552 "raku.tab.c"
    break;

  case 104: /* stmt: repeat_stmt  */
#line 651 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3558 "raku.tab.c"
    break;

  case 105: /* stmt: loop_stmt  */
#line 652 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3564 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST ';'  */
#line 653 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3570 "raku.tab.c"
    break;

  case 107: /* stmt: KW_NEXT ';'  */
#line 654 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3576 "raku.tab.c"
    break;

  case 108: /* stmt: KW_LAST KW_IF expr ';'  */
#line 656 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3582 "raku.tab.c"
    break;

  case 109: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 658 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3588 "raku.tab.c"
    break;

  case 110: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 660 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3594 "raku.tab.c"
    break;

  case 111: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 662 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3600 "raku.tab.c"
    break;

  case 112: /* stmt: sub_decl  */
#line 663 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3606 "raku.tab.c"
    break;

  case 113: /* stmt: class_decl  */
#line 664 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3612 "raku.tab.c"
    break;

  case 114: /* stmt: role_decl  */
#line 665 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3618 "raku.tab.c"
    break;

  case 115: /* stmt: grammar_decl  */
#line 666 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3624 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block  */
#line 670 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3630 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3636 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3642 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3648 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3654 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3660 "raku.tab.c"
    break;

  case 122: /* if_stmt: KW_IF expr block elsif_tail  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3666 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 684 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3672 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3678 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3684 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3690 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 694 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3696 "raku.tab.c"
    break;

  case 128: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 696 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3702 "raku.tab.c"
    break;

  case 129: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 698 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3708 "raku.tab.c"
    break;

  case 130: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 702 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3714 "raku.tab.c"
    break;

  case 131: /* while_stmt: KW_WHILE expr block  */
#line 704 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3720 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3726 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3732 "raku.tab.c"
    break;

  case 134: /* unless_stmt: KW_UNLESS expr block  */
#line 712 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3738 "raku.tab.c"
    break;

  case 135: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3744 "raku.tab.c"
    break;

  case 136: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3750 "raku.tab.c"
    break;

  case 137: /* until_stmt: KW_UNTIL expr block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3756 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3762 "raku.tab.c"
    break;

  case 139: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 726 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3768 "raku.tab.c"
    break;

  case 140: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3774 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP block  */
#line 732 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3780 "raku.tab.c"
    break;

  case 142: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 734 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3786 "raku.tab.c"
    break;

  case 143: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 736 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3792 "raku.tab.c"
    break;

  case 144: /* loop_incr: expr  */
#line 739 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3798 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 743 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3808 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 749 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3818 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 755 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3827 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 760 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3837 "raku.tab.c"
    break;

  case 149: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 766 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3846 "raku.tab.c"
    break;

  case 150: /* for_stmt: KW_FOR expr block  */
#line 771 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3853 "raku.tab.c"
    break;

  case 151: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 776 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3865 "raku.tab.c"
    break;

  case 152: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 784 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3878 "raku.tab.c"
    break;

  case 153: /* when_list: %empty  */
#line 794 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3884 "raku.tab.c"
    break;

  case 154: /* when_list: when_list KW_WHEN expr block  */
#line 796 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3892 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 802 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3904 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 810 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3914 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_SUB IDENT sub_body  */
#line 816 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3924 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 822 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3936 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 830 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3946 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 836 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3956 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 842 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3969 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 851 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3980 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 858 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3993 "raku.tab.c"
    break;

  case 164: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 867 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 4006 "raku.tab.c"
    break;

  case 165: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 876 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4017 "raku.tab.c"
    break;

  case 166: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 885 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4028 "raku.tab.c"
    break;

  case 167: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 892 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4037 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list '}'  */
#line 898 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4043 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list expr '}'  */
#line 900 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4050 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 903 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4057 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 906 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4063 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 908 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4069 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 910 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4075 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 912 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4081 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 914 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4087 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 916 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4093 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 918 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4099 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 920 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4106 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 923 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4112 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 925 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4118 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 927 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4124 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 929 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4131 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 932 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4138 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 935 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4146 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list '}'  */
#line 940 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4152 "raku.tab.c"
    break;

  case 186: /* method_body: '{' YADA '}'  */
#line 941 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4158 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list expr '}'  */
#line 943 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4165 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 946 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4172 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 949 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4178 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 951 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4184 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 953 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4190 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 955 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4196 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 957 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4202 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 959 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4208 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 961 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4214 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 963 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4221 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 966 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4227 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 968 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4233 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 970 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4239 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 972 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4246 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 975 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4253 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 978 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4261 "raku.tab.c"
    break;

  case 203: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 984 "raku.y"
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
#line 4279 "raku.tab.c"
    break;

  case 204: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1000 "raku.y"
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
#line 4296 "raku.tab.c"
    break;

  case 205: /* is_clauses: %empty  */
#line 1014 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4302 "raku.tab.c"
    break;

  case 206: /* is_clauses: is_clauses IDENT IDENT  */
#line 1016 "raku.y"
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
#line 4318 "raku.tab.c"
    break;

  case 207: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1028 "raku.y"
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
#line 4334 "raku.tab.c"
    break;

  case 208: /* class_body_list: %empty  */
#line 1041 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4340 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1043 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4347 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1046 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4354 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1049 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4361 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1052 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4368 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1055 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4375 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1058 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4382 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1061 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4389 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1064 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4396 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1067 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4403 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1070 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4410 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1073 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4417 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1076 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4424 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1079 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4435 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1086 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4446 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1093 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4457 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1100 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4468 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1107 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4476 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1111 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4484 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1115 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4492 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1119 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4500 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1123 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4511 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1130 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4522 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1137 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4533 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1144 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4544 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1151 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4558 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1161 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4570 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1169 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4582 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1177 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4595 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1186 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4606 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1193 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4617 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1200 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4631 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1210 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4643 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1218 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4655 "raku.tab.c"
    break;

  case 242: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1228 "raku.y"
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
#line 4672 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: %empty  */
#line 1242 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4678 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1244 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4687 "raku.tab.c"
    break;

  case 245: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1249 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4696 "raku.tab.c"
    break;

  case 246: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1254 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4705 "raku.tab.c"
    break;

  case 247: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1261 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4713 "raku.tab.c"
    break;

  case 248: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1265 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4721 "raku.tab.c"
    break;

  case 249: /* pair_list: IDENT OP_FATARROW expr  */
#line 1271 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4727 "raku.tab.c"
    break;

  case 250: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1273 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4733 "raku.tab.c"
    break;

  case 251: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1275 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4739 "raku.tab.c"
    break;

  case 252: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1277 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4745 "raku.tab.c"
    break;

  case 253: /* param_list: VAR_SCALAR  */
#line 1280 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4751 "raku.tab.c"
    break;

  case 254: /* param_list: VAR_ARRAY  */
#line 1281 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4757 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_ARRAY  */
#line 1282 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4763 "raku.tab.c"
    break;

  case 256: /* param_list: IDENT VAR_SCALAR  */
#line 1283 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4769 "raku.tab.c"
    break;

  case 257: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1284 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4775 "raku.tab.c"
    break;

  case 258: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1285 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4781 "raku.tab.c"
    break;

  case 259: /* param_list: param_list ',' VAR_SCALAR  */
#line 1286 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4787 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1287 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4793 "raku.tab.c"
    break;

  case 261: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1288 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4799 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1289 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4805 "raku.tab.c"
    break;

  case 263: /* param_list: VAR_SCALAR '=' expr  */
#line 1290 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4811 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1291 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4817 "raku.tab.c"
    break;

  case 265: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1292 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4823 "raku.tab.c"
    break;

  case 266: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1293 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); free((yyvsp[-3].sval)); }
#line 4829 "raku.tab.c"
    break;

  case 267: /* param_list: SLURPY_POS  */
#line 1294 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4835 "raku.tab.c"
    break;

  case 268: /* param_list: param_list ',' SLURPY_POS  */
#line 1295 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4841 "raku.tab.c"
    break;

  case 269: /* param_list: SLURPY_LOL  */
#line 1296 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4847 "raku.tab.c"
    break;

  case 270: /* param_list: param_list ',' SLURPY_LOL  */
#line 1297 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4853 "raku.tab.c"
    break;

  case 271: /* param_list: SLURPY_NAMED  */
#line 1298 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4859 "raku.tab.c"
    break;

  case 272: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1299 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 4865 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list '}'  */
#line 1302 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4871 "raku.tab.c"
    break;

  case 274: /* block: '{' YADA '}'  */
#line 1303 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4877 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list expr '}'  */
#line 1305 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4883 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1307 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4889 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1309 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4895 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1311 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4901 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1313 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4907 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1315 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4914 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1318 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4920 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1320 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4926 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1322 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4932 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1324 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4938 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1326 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4945 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1329 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4952 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1332 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4960 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1336 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4967 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1339 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4974 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1342 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4981 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1345 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4987 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1347 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4995 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1351 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5002 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1354 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5009 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1357 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 5016 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1360 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5023 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1363 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 5030 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1366 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 5036 "raku.tab.c"
    break;

  case 299: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1368 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5042 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1370 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5048 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list KW_LAST '}'  */
#line 1372 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5054 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1374 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5060 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1376 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5066 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1378 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5072 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1380 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5078 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1382 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5084 "raku.tab.c"
    break;

  case 307: /* closure: '{' expr '}'  */
#line 1385 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5090 "raku.tab.c"
    break;

  case 308: /* expr: VAR_SCALAR '=' expr  */
#line 1388 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5096 "raku.tab.c"
    break;

  case 309: /* expr: VAR_ARRAY '=' expr  */
#line 1389 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 5102 "raku.tab.c"
    break;

  case 310: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1390 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 5108 "raku.tab.c"
    break;

  case 311: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1391 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 5114 "raku.tab.c"
    break;

  case 312: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1392 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 5120 "raku.tab.c"
    break;

  case 313: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1393 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 5126 "raku.tab.c"
    break;

  case 314: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1394 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 5132 "raku.tab.c"
    break;

  case 315: /* expr: KW_GATHER block  */
#line 1395 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5142 "raku.tab.c"
    break;

  case 316: /* expr: tern_expr  */
#line 1400 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5148 "raku.tab.c"
    break;

  case 317: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1404 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5154 "raku.tab.c"
    break;

  case 318: /* tern_expr: cmp_expr  */
#line 1405 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5160 "raku.tab.c"
    break;

  case 319: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1408 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5166 "raku.tab.c"
    break;

  case 320: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1409 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5172 "raku.tab.c"
    break;

  case 321: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1410 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5178 "raku.tab.c"
    break;

  case 322: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1411 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5184 "raku.tab.c"
    break;

  case 323: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1412 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5190 "raku.tab.c"
    break;

  case 324: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1413 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5196 "raku.tab.c"
    break;

  case 325: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1414 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5202 "raku.tab.c"
    break;

  case 326: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1415 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 327: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1416 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5214 "raku.tab.c"
    break;

  case 328: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1417 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5220 "raku.tab.c"
    break;

  case 329: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1418 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5226 "raku.tab.c"
    break;

  case 330: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1419 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5232 "raku.tab.c"
    break;

  case 331: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1420 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5238 "raku.tab.c"
    break;

  case 332: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1421 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5244 "raku.tab.c"
    break;

  case 333: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1423 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5254 "raku.tab.c"
    break;

  case 334: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1429 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5264 "raku.tab.c"
    break;

  case 335: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1435 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5274 "raku.tab.c"
    break;

  case 336: /* cmp_expr: divis_expr  */
#line 1440 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5280 "raku.tab.c"
    break;

  case 337: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1443 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5286 "raku.tab.c"
    break;

  case 338: /* divis_expr: jct_expr  */
#line 1444 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5292 "raku.tab.c"
    break;

  case 339: /* jct_expr: jct_expr '|' range_expr  */
#line 1447 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5298 "raku.tab.c"
    break;

  case 340: /* jct_expr: jct_expr '&' range_expr  */
#line 1448 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5304 "raku.tab.c"
    break;

  case 341: /* jct_expr: dor_expr  */
#line 1449 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5310 "raku.tab.c"
    break;

  case 342: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1453 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5316 "raku.tab.c"
    break;

  case 343: /* dor_expr: range_expr  */
#line 1454 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5322 "raku.tab.c"
    break;

  case 344: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1457 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5328 "raku.tab.c"
    break;

  case 345: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1458 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5334 "raku.tab.c"
    break;

  case 346: /* range_expr: add_expr  */
#line 1459 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5340 "raku.tab.c"
    break;

  case 347: /* add_expr: add_expr '~' repl_expr  */
#line 1462 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5346 "raku.tab.c"
    break;

  case 348: /* add_expr: repl_expr  */
#line 1463 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5352 "raku.tab.c"
    break;

  case 349: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1466 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5358 "raku.tab.c"
    break;

  case 350: /* repl_expr: addsub_expr  */
#line 1467 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5364 "raku.tab.c"
    break;

  case 351: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1470 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5370 "raku.tab.c"
    break;

  case 352: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1471 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5376 "raku.tab.c"
    break;

  case 353: /* addsub_expr: mul_expr  */
#line 1472 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5382 "raku.tab.c"
    break;

  case 354: /* mul_expr: mul_expr '*' unary_expr  */
#line 1475 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5388 "raku.tab.c"
    break;

  case 355: /* mul_expr: mul_expr '/' unary_expr  */
#line 1476 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5394 "raku.tab.c"
    break;

  case 356: /* mul_expr: mul_expr '%' unary_expr  */
#line 1477 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5400 "raku.tab.c"
    break;

  case 357: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1478 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5406 "raku.tab.c"
    break;

  case 358: /* mul_expr: unary_expr  */
#line 1479 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5412 "raku.tab.c"
    break;

  case 359: /* unary_expr: '-' unary_expr  */
#line 1482 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5418 "raku.tab.c"
    break;

  case 360: /* unary_expr: '!' unary_expr  */
#line 1483 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5424 "raku.tab.c"
    break;

  case 361: /* unary_expr: CARET unary_expr  */
#line 1484 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5430 "raku.tab.c"
    break;

  case 362: /* unary_expr: OP_REDUCE unary_expr  */
#line 1486 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5439 "raku.tab.c"
    break;

  case 363: /* unary_expr: pow_expr  */
#line 1490 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5445 "raku.tab.c"
    break;

  case 364: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1493 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5451 "raku.tab.c"
    break;

  case 365: /* pow_expr: postfix_expr  */
#line 1494 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5457 "raku.tab.c"
    break;

  case 366: /* scalar_list: VAR_SCALAR  */
#line 1497 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5463 "raku.tab.c"
    break;

  case 367: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1498 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5469 "raku.tab.c"
    break;

  case 368: /* meth_name: IDENT  */
#line 1501 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5475 "raku.tab.c"
    break;

  case 369: /* meth_name: KW_SORT  */
#line 1502 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5481 "raku.tab.c"
    break;

  case 370: /* meth_name: KW_MAP  */
#line 1503 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5487 "raku.tab.c"
    break;

  case 371: /* meth_name: KW_GREP  */
#line 1504 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5493 "raku.tab.c"
    break;

  case 372: /* meth_name: KW_SAY  */
#line 1505 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5499 "raku.tab.c"
    break;

  case 373: /* meth_name: KW_PRINT  */
#line 1506 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5505 "raku.tab.c"
    break;

  case 374: /* meth_name: KW_TAKE  */
#line 1507 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5511 "raku.tab.c"
    break;

  case 375: /* meth_name: KW_RETURN  */
#line 1508 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5517 "raku.tab.c"
    break;

  case 376: /* meth_name: KW_EXISTS  */
#line 1509 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5523 "raku.tab.c"
    break;

  case 377: /* meth_name: KW_DELETE  */
#line 1510 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5529 "raku.tab.c"
    break;

  case 378: /* meth_name: TESTOP  */
#line 1511 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5535 "raku.tab.c"
    break;

  case 379: /* postfix_expr: call_expr  */
#line 1513 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5541 "raku.tab.c"
    break;

  case 380: /* call_expr: IDENT '(' arg_list ')'  */
#line 1516 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5550 "raku.tab.c"
    break;

  case 381: /* call_expr: IDENT '(' ')'  */
#line 1520 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5556 "raku.tab.c"
    break;

  case 382: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1522 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5562 "raku.tab.c"
    break;

  case 383: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1524 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5568 "raku.tab.c"
    break;

  case 384: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1526 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5577 "raku.tab.c"
    break;

  case 385: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1531 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5583 "raku.tab.c"
    break;

  case 386: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1533 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5593 "raku.tab.c"
    break;

  case 387: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1539 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5603 "raku.tab.c"
    break;

  case 388: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1545 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5611 "raku.tab.c"
    break;

  case 389: /* call_expr: IDENT '.' KW_NEW  */
#line 1549 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5619 "raku.tab.c"
    break;

  case 390: /* call_expr: IDENT '.' IDENT  */
#line 1553 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5628 "raku.tab.c"
    break;

  case 391: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1558 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5639 "raku.tab.c"
    break;

  case 392: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1565 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5648 "raku.tab.c"
    break;

  case 393: /* call_expr: IDENT '.' CARET IDENT  */
#line 1570 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5658 "raku.tab.c"
    break;

  case 394: /* call_expr: atom '.' CARET IDENT  */
#line 1576 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5668 "raku.tab.c"
    break;

  case 395: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1582 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5679 "raku.tab.c"
    break;

  case 396: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1589 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5690 "raku.tab.c"
    break;

  case 397: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1596 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5699 "raku.tab.c"
    break;

  case 398: /* call_expr: atom '.' meth_name  */
#line 1601 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5708 "raku.tab.c"
    break;

  case 399: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1606 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5719 "raku.tab.c"
    break;

  case 400: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1613 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5728 "raku.tab.c"
    break;

  case 401: /* call_expr: call_expr '.' meth_name  */
#line 1618 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5737 "raku.tab.c"
    break;

  case 402: /* call_expr: KW_DIE expr  */
#line 1623 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5743 "raku.tab.c"
    break;

  case 403: /* call_expr: KW_MAP closure expr  */
#line 1625 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5749 "raku.tab.c"
    break;

  case 404: /* call_expr: KW_GREP closure expr  */
#line 1627 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5755 "raku.tab.c"
    break;

  case 405: /* call_expr: KW_SORT expr  */
#line 1629 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5761 "raku.tab.c"
    break;

  case 406: /* call_expr: KW_SORT closure expr  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5767 "raku.tab.c"
    break;

  case 407: /* call_expr: atom  */
#line 1632 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5773 "raku.tab.c"
    break;

  case 408: /* arg_list: expr  */
#line 1635 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5779 "raku.tab.c"
    break;

  case 409: /* arg_list: arg_list ',' expr  */
#line 1636 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5785 "raku.tab.c"
    break;

  case 410: /* arg_list: arg_list ','  */
#line 1637 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5791 "raku.tab.c"
    break;

  case 411: /* atom: LIT_INT  */
#line 1640 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5797 "raku.tab.c"
    break;

  case 412: /* atom: LIT_FLOAT  */
#line 1641 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5803 "raku.tab.c"
    break;

  case 413: /* atom: LIT_STR  */
#line 1642 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5809 "raku.tab.c"
    break;

  case 414: /* atom: WORDLIST  */
#line 1644 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5822 "raku.tab.c"
    break;

  case 415: /* atom: LIT_INTERP_STR  */
#line 1652 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5828 "raku.tab.c"
    break;

  case 416: /* atom: VAR_SCALAR  */
#line 1653 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5834 "raku.tab.c"
    break;

  case 417: /* atom: OP_INC VAR_SCALAR  */
#line 1654 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5840 "raku.tab.c"
    break;

  case 418: /* atom: OP_DEC VAR_SCALAR  */
#line 1655 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5846 "raku.tab.c"
    break;

  case 419: /* atom: VAR_SCALAR OP_INC  */
#line 1656 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5852 "raku.tab.c"
    break;

  case 420: /* atom: VAR_SCALAR OP_DEC  */
#line 1657 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5858 "raku.tab.c"
    break;

  case 421: /* atom: VAR_ARRAY  */
#line 1658 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5864 "raku.tab.c"
    break;

  case 422: /* atom: VAR_HASH  */
#line 1659 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5870 "raku.tab.c"
    break;

  case 423: /* atom: VAR_CAPTURE  */
#line 1661 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5878 "raku.tab.c"
    break;

  case 424: /* atom: VAR_FH  */
#line 1665 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5886 "raku.tab.c"
    break;

  case 425: /* atom: VAR_NAMED_CAPTURE  */
#line 1669 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5893 "raku.tab.c"
    break;

  case 426: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1672 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5899 "raku.tab.c"
    break;

  case 427: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1674 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5905 "raku.tab.c"
    break;

  case 428: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1676 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5911 "raku.tab.c"
    break;

  case 429: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1678 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5917 "raku.tab.c"
    break;

  case 430: /* atom: ARR_ALL_SLICE  */
#line 1680 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5923 "raku.tab.c"
    break;

  case 431: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1682 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5929 "raku.tab.c"
    break;

  case 432: /* atom: VAR_HASH '{' expr '}'  */
#line 1684 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5935 "raku.tab.c"
    break;

  case 433: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1686 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5941 "raku.tab.c"
    break;

  case 434: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1688 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5947 "raku.tab.c"
    break;

  case 435: /* atom: IDENT  */
#line 1689 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5953 "raku.tab.c"
    break;

  case 436: /* atom: VAR_TWIGIL  */
#line 1691 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5961 "raku.tab.c"
    break;

  case 437: /* atom: VAR_ARRAY_TWIGIL  */
#line 1695 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5969 "raku.tab.c"
    break;

  case 438: /* atom: VAR_HASH_TWIGIL  */
#line 1699 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5977 "raku.tab.c"
    break;

  case 439: /* atom: '(' ')'  */
#line 1702 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5983 "raku.tab.c"
    break;

  case 440: /* atom: '(' expr ')'  */
#line 1703 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5989 "raku.tab.c"
    break;

  case 441: /* atom: '(' expr ',' ')'  */
#line 1705 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5995 "raku.tab.c"
    break;

  case 442: /* atom: '(' expr ',' arg_list ')'  */
#line 1707 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 6002 "raku.tab.c"
    break;

  case 443: /* atom: block  */
#line 1709 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6008 "raku.tab.c"
    break;

  case 444: /* atom: KW_SUB block  */
#line 1710 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 6014 "raku.tab.c"
    break;


#line 6018 "raku.tab.c"

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

#line 1712 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
