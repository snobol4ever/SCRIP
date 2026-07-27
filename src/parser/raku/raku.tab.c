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
static tree_t *rk_slurpy_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("*@"))); return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_slurpy_lol_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, intern("**@"))); return p;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_param_default(tree_t *p, tree_t *dflt) {
    return expr_binary(TT_ASSIGN, p, dflt);
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_scalar_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_XREP || rhs->n < 2) return rhs;
    tree_t *c = make_call("__rk_rep"); expr_add_child(c, rhs->c[0]); expr_add_child(c, rhs->c[1]); return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_named_call(const char *fname, ExprList *pos, ExprList *named) {
    tree_t *c = make_call("__rk_named_call");
    expr_add_child(c, leaf_sval(TT_QLIT, fname));
    tree_t *n = ast_node_new(TT_ILIT); n->v.ival = pos ? pos->count : 0;
    expr_add_child(c, n);
    if (pos) { for (int i = 0; i < pos->count; i++) expr_add_child(c, pos->items[i]); exprlist_free(pos); }
    if (named) { for (int i = 0; i < named->count; i++) expr_add_child(c, named->items[i]); exprlist_free(named); }
    return c;
}
/*--------------------------------------------------------------------------------------------------------------------*/
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

#line 414 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 67,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 68,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 69,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 70,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 71,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 72,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 73,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 74,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 75,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 76,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 77,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 78,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 79,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 80,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 81,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 82,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 83,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 84,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 85,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 86,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 87,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 88,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 89,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 90,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 91,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 92,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 93,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 94,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 95,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 96,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 97,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 98,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 99,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 100,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 101,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 102,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 103,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 104,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 105,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 106,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 107,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 108,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 109,                   /* OP_POW  */
  YYSYMBOL_110_ = 110,                     /* '='  */
  YYSYMBOL_111_ = 111,                     /* '!'  */
  YYSYMBOL_112_ = 112,                     /* '<'  */
  YYSYMBOL_113_ = 113,                     /* '>'  */
  YYSYMBOL_114_ = 114,                     /* '|'  */
  YYSYMBOL_115_ = 115,                     /* '&'  */
  YYSYMBOL_116_ = 116,                     /* '~'  */
  YYSYMBOL_117_ = 117,                     /* '+'  */
  YYSYMBOL_118_ = 118,                     /* '-'  */
  YYSYMBOL_119_ = 119,                     /* '*'  */
  YYSYMBOL_120_ = 120,                     /* '/'  */
  YYSYMBOL_121_ = 121,                     /* '%'  */
  YYSYMBOL_UMINUS = 122,                   /* UMINUS  */
  YYSYMBOL_123_ = 123,                     /* '.'  */
  YYSYMBOL_124_ = 124,                     /* ';'  */
  YYSYMBOL_125_ = 125,                     /* '('  */
  YYSYMBOL_126_ = 126,                     /* ')'  */
  YYSYMBOL_127_ = 127,                     /* ','  */
  YYSYMBOL_128_ = 128,                     /* '['  */
  YYSYMBOL_129_ = 129,                     /* ']'  */
  YYSYMBOL_130_ = 130,                     /* '{'  */
  YYSYMBOL_131_ = 131,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 132,                 /* $accept  */
  YYSYMBOL_program = 133,                  /* program  */
  YYSYMBOL_stmt_list = 134,                /* stmt_list  */
  YYSYMBOL_stmt = 135,                     /* stmt  */
  YYSYMBOL_if_stmt = 136,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 137,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 138,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 139,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 140,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 141,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 142,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 143,                /* loop_incr  */
  YYSYMBOL_for_stmt = 144,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 145,               /* given_stmt  */
  YYSYMBOL_when_list = 146,                /* when_list  */
  YYSYMBOL_sub_decl = 147,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 148,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 149,                 /* sub_body  */
  YYSYMBOL_method_body = 150,              /* method_body  */
  YYSYMBOL_class_decl = 151,               /* class_decl  */
  YYSYMBOL_role_decl = 152,                /* role_decl  */
  YYSYMBOL_is_clauses = 153,               /* is_clauses  */
  YYSYMBOL_class_body_list = 154,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 155,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 156,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 157,           /* named_arg_list  */
  YYSYMBOL_pair_list = 158,                /* pair_list  */
  YYSYMBOL_param_list = 159,               /* param_list  */
  YYSYMBOL_block = 160,                    /* block  */
  YYSYMBOL_closure = 161,                  /* closure  */
  YYSYMBOL_expr = 162,                     /* expr  */
  YYSYMBOL_tern_expr = 163,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 164,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 165,               /* divis_expr  */
  YYSYMBOL_jct_expr = 166,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 167,                 /* dor_expr  */
  YYSYMBOL_range_expr = 168,               /* range_expr  */
  YYSYMBOL_add_expr = 169,                 /* add_expr  */
  YYSYMBOL_repl_expr = 170,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 171,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 172,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 173,               /* unary_expr  */
  YYSYMBOL_pow_expr = 174,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 175,              /* scalar_list  */
  YYSYMBOL_meth_name = 176,                /* meth_name  */
  YYSYMBOL_postfix_expr = 177,             /* postfix_expr  */
  YYSYMBOL_call_expr = 178,                /* call_expr  */
  YYSYMBOL_arg_list = 179,                 /* arg_list  */
  YYSYMBOL_atom = 180                      /* atom  */
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
#define YYLAST   4116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  132
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  430
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1090

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   365


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
       2,     2,     2,   111,     2,     2,     2,   121,   115,     2,
     125,   126,   119,   117,   127,   118,   123,   120,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   124,
     112,   110,   113,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   128,     2,   129,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   130,   114,   131,   116,     2,     2,     2,
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
     105,   106,   107,   108,   109,   122
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   430,   430,   441,   442,   445,   447,   449,   451,   455,
     457,   459,   461,   465,   468,   472,   474,   478,   481,   485,
     487,   489,   491,   493,   495,   499,   503,   505,   507,   509,
     511,   513,   515,   517,   519,   521,   523,   525,   527,   529,
     531,   534,   537,   539,   542,   545,   547,   549,   551,   553,
     558,   562,   566,   571,   575,   576,   578,   580,   582,   584,
     587,   589,   591,   593,   596,   599,   602,   605,   608,   610,
     612,   614,   616,   619,   621,   623,   625,   628,   631,   635,
     638,   640,   642,   644,   647,   650,   653,   656,   659,   662,
     665,   666,   667,   668,   669,   670,   671,   673,   675,   677,
     679,   680,   681,   682,   683,   684,   685,   687,   689,   691,
     693,   694,   695,   696,   699,   701,   703,   705,   707,   709,
     711,   713,   717,   719,   721,   723,   725,   727,   731,   733,
     737,   739,   741,   743,   747,   749,   753,   755,   757,   761,
     763,   765,   769,   772,   778,   784,   789,   795,   800,   805,
     813,   824,   825,   831,   839,   845,   851,   859,   865,   871,
     880,   887,   896,   905,   914,   921,   928,   929,   932,   935,
     937,   939,   941,   943,   945,   947,   949,   952,   954,   956,
     958,   961,   964,   970,   971,   972,   975,   978,   980,   982,
     984,   986,   988,   990,   992,   995,   997,   999,  1001,  1004,
    1007,  1013,  1029,  1044,  1045,  1057,  1071,  1072,  1075,  1078,
    1081,  1084,  1087,  1090,  1093,  1096,  1099,  1102,  1105,  1108,
    1115,  1122,  1129,  1136,  1140,  1144,  1148,  1152,  1159,  1166,
    1173,  1180,  1190,  1198,  1206,  1215,  1222,  1229,  1239,  1247,
    1257,  1272,  1273,  1278,  1283,  1290,  1294,  1300,  1302,  1304,
    1306,  1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1318,
    1319,  1320,  1321,  1322,  1323,  1326,  1327,  1328,  1330,  1332,
    1334,  1336,  1338,  1341,  1343,  1345,  1347,  1349,  1352,  1355,
    1359,  1362,  1365,  1368,  1370,  1374,  1377,  1380,  1383,  1386,
    1389,  1391,  1393,  1395,  1397,  1399,  1401,  1403,  1405,  1409,
    1412,  1413,  1418,  1421,  1423,  1426,  1427,  1428,  1429,  1430,
    1431,  1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,
    1446,  1452,  1458,  1461,  1462,  1465,  1466,  1467,  1470,  1472,
    1475,  1476,  1477,  1480,  1481,  1484,  1485,  1488,  1489,  1490,
    1493,  1494,  1495,  1496,  1497,  1500,  1501,  1502,  1503,  1508,
    1511,  1512,  1515,  1516,  1519,  1520,  1521,  1522,  1523,  1524,
    1525,  1526,  1527,  1528,  1529,  1531,  1533,  1538,  1539,  1541,
    1543,  1548,  1550,  1556,  1562,  1566,  1570,  1575,  1582,  1587,
    1593,  1599,  1606,  1613,  1618,  1623,  1630,  1635,  1640,  1642,
    1644,  1646,  1648,  1650,  1653,  1654,  1655,  1658,  1659,  1660,
    1661,  1669,  1670,  1671,  1672,  1673,  1674,  1675,  1676,  1677,
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
  "SLURPY_POS", "SLURPY_LOL", "KW_HANDLES", "WORDLIST", "OP_COLON_D",
  "OP_COLON_U", "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT",
  "OP_SGE", "OP_AND", "OP_OR", "OP_TERNARY1", "OP_TERNARY2", "OP_BIND",
  "OP_DOTEQ", "OP_SMATCH", "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ",
  "OP_MUL_EQ", "OP_DIV_EQ", "OP_CAT_EQ", "OP_DOR", "OP_DIV", "OP_DIVIS",
  "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'", "'<'", "'>'", "'|'",
  "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS", "'.'", "';'",
  "'('", "')'", "','", "'['", "']'", "'{'", "'}'", "$accept", "program",
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

#define YYPACT_NINF (-794)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-430)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -794,    84,  1597,  -794,  -794,  -794,  -794,  -794,  1150,   -74,
     -69,   -72,   372,  -794,  -794,  3986,  -794,  -794,  -794,    54,
    1669,    68,  2978,  3050,  3122,  3194,  3266,    12,   -11,  3266,
    1741,   103,  3266,   168,   195,  3338,  3410,   -11,   279,    34,
      57,    83,    83,  3482,   -11,   -11,  3266,   242,   259,   203,
    3986,  -794,  -794,   285,   316,   381,  3986,  3986,  -794,  1813,
     257,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
    -794,   586,  -794,  -794,  -794,   986,   651,  -794,   340,   550,
      31,   263,  -794,   105,   288,   514,   320,  -794,  -794,   264,
     290,   311,   430,  -794,  -794,  3266,  3266,  3266,  3266,  3266,
    3266,   463,  1886,  3554,  1959,   530,  3266,  3266,    95,  2031,
     224,   406,   145,  -794,   -11,  -794,  -794,   439,   271,  -794,
    2104,  -794,   281,     0,    26,    28,   445,   592,   209,   609,
    1813,   546,  1813,    47,  1813,   -11,  1813,   -11,   -61,   255,
     313,  -794,  -794,   499,  -794,   517,   537,   564,   579,   146,
     229,  1813,   -11,  1813,   -11,   219,  2177,  -794,  3266,  3266,
    -794,  3266,  3266,  -794,  3266,  3266,  3266,  2249,  3266,  -794,
     683,  -794,  -794,  -794,   650,   649,    67,  -794,   670,  -794,
    -794,  -794,  -794,  -794,   557,   655,  1112,  3266,  3266,  3266,
    3266,  3266,  3266,  3266,  3266,  -794,  3266,  3266,  3266,  3266,
    3266,  3266,  3266,  3266,  -794,  3986,  3986,  3986,  3986,  3986,
    3986,  3986,  3986,  3986,  3986,  3986,  3986,  3986,  3986,  3986,
     734,  3986,  3986,  3986,  3986,  3986,  3986,  3986,  3986,  3986,
    3986,  3986,  3986,  3986,  3986,  3986,  1163,   960,   597,   701,
     712,   715,   718,   733,   751,   173,  -794,   622,  1813,   -79,
     648,   378,   758,   761,   772,   785,   908,   800,   206,  -794,
     642,   644,  1959,   930,  3266,  -794,  3266,   823,   557,   646,
    -794,  3266,  3266,  -794,  3626,  -794,  3698,  -794,   253,   266,
     276,   361,   853,   854,  -794,   652,   656,  3266,  3266,  3266,
    3266,  3266,  3266,  3266,  -794,  3266,   658,  3266,  3266,  -794,
    3266,   669,   771,   679,  -794,   955,  3266,  -794,  3986,  3986,
      -1,  -794,  -794,  -794,  -794,  3266,  3266,  -794,   953,  3266,
     954,  3266,   686,   942,   688,  -794,  3266,  3266,   961,   846,
     851,   859,   862,   863,   857,  -794,  -794,  -794,   -11,     3,
    -794,     5,   864,   865,  -794,  -794,  2321,  -794,  1255,   246,
     249,   881,  2978,  3050,  3266,   713,    24,   199,  -794,   211,
     868,   871,   872,   873,   874,   875,   879,   880,   884,   888,
     889,   890,   891,   892,   893,   894,   914,   914,   914,   914,
     914,   914,   926,   914,   914,   914,   914,   914,   914,   914,
     914,  -794,  -794,  -794,    31,  -794,  -794,  -794,   905,   905,
     288,   514,   320,   320,  -794,  -794,  -794,  -794,  -794,  -794,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
     897,  1009,   902,  -794,  2394,  -794,  -794,  -794,  -794,  -794,
    -794,  3266,  2467,  -794,   690,  3266,  -794,  3266,  3266,  3266,
    3266,   918,   919,   920,  -794,  2540,  -794,  2613,  3266,  -794,
    1010,  -794,  3770,   628,   921,   900,  -794,  -794,   909,  -794,
     912,  1813,   144,   956,   221,  2686,   419,   923,  3266,  -794,
    3842,  -794,  3266,  -794,    21,  -794,  3266,  3266,   925,  1027,
    2321,   924,   927,   928,   929,   931,   932,   933,   437,  2321,
     937,   938,   464,   -11,   -10,  3914,  -794,   -11,   -11,   -42,
      -5,    60,   939,    22,  -794,  -794,   935,   694,  1279,   957,
     959,    38,   941,   907,   966,   958,   -11,   -11,   -11,   963,
     964,   940,  3266,  -794,  -794,  -794,  -794,  -794,  -794,  1080,
    1082,  -794,   251,   935,   696,    41,   235,   205,  -794,   703,
    1083,  1959,  3266,  3266,   358,    13,   -82,  -794,   112,  3266,
    3266,  -794,  3266,  3266,  -794,  3266,  3266,  3266,  3266,  3266,
    3266,  3266,  3266,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
    3986,  2759,  -794,  2832,   980,   706,   987,  -794,   708,  2321,
     989,   531,   981,   985,   629,  3266,  3266,  3266,  -794,   714,
    -794,   719,   721,  -794,  1043,   723,  -794,  -794,  -794,  -794,
    -794,   743,  3266,  -794,  3266,  3266,  3266,   747,  -794,   352,
    -794,   996,  1813,   539,   997,   935,   755,  1015,  1016,  3266,
    -794,   757,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
     759,  -794,  -794,  -794,   860,  -794,  -794,  1813,   -11,  -794,
    -794,  1139,  -794,  1140,  1141,  3266,  -794,  1142,  1158,  -794,
     935,   350,  2978,  3050,   848,  -794,   363,  -794,  -794,  3266,
     -11,  -794,  -794,  -794,  1045,  1046,  1146,  -794,  -794,  -794,
    -794,  3266,  1050,  -794,  -794,   283,   899,   698,  1125,  -794,
    -794,   935,   935,   764,   768,  1168,  1169,  1172,  -794,  -794,
     184,   632,  1058,   234,  3266,  3266,  3266,  3266,  3266,  3266,
    -794,  -794,  -794,  -794,   286,   295,   304,   318,   330,   334,
     336,   345,   347,   348,   349,   357,  -794,  -794,   774,  -794,
     779,   781,  -794,  1066,  -794,  -794,   788,  -794,  -794,  -794,
    -794,  -794,  1067,  1068,  1069,  -794,  -794,  -794,  3266,  -794,
    2321,  1070,   573,  -794,  -794,  1071,  1122,  1123,  -794,   791,
    -794,  3266,  -794,  -794,   935,  -794,  -794,   574,  1076,  1077,
     -10,  -794,   793,   901,   -11,   -11,   -11,  -794,  -794,  -794,
    -794,  1092,    52,  -794,  -794,   472,    46,  -794,   359,  3266,
    3266,  3266,  3266,  3266,  3266,  3266,  3266,  -794,   -11,  1072,
    -794,  -794,   -11,  1084,  3266,  -794,   449,   462,    94,   192,
     724,  1087,  1091,  1163,  -794,  -794,   935,   935,  1199,  1200,
    1209,  3266,  2905,  1107,  1108,  -794,   368,   385,   398,   399,
     400,   401,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  1095,  -794,
    -794,  -794,  -794,   804,  -794,  -794,  -794,  3266,  3266,  2321,
     611,  -794,  -794,  3266,  -794,  -794,  -794,  -794,   -11,   -11,
    -794,  -794,  -794,  -794,  3266,  -794,  1210,  1211,  3266,  3266,
    3266,  -794,  -794,  -794,   402,   404,   414,   418,   438,   442,
     459,   473,  -794,  -794,  -794,  3266,  1096,  -794,    49,  1153,
    -794,   113,  -794,  1212,  1213,  1101,  3266,  -794,  1214,  1215,
    1109,  3266,  -794,   194,   210,  1110,  1111,  -794,  -794,   590,
    -794,  -794,  -794,  -794,  -794,   476,  1100,   808,  3266,  3266,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  1114,  -794,  -794,
     810,  -794,   623,   911,  -794,  -794,  -794,  -794,   477,   478,
     487,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  1115,
     -11,  1102,   816,  1113,  1402,  1102,   825,  1116,  1117,  -794,
    1145,  1147,  1148,  -794,  1152,  1232,  1263,  1154,  3266,  -794,
    1265,  1266,  1157,  3266,  -794,  -794,  -794,   201,  -794,  -794,
    -794,  1155,   493,   497,  -794,  1164,  -794,   -11,  -794,  -794,
    -794,  -794,  3266,  -794,  -794,  1102,  -794,  2978,  3050,  1474,
    -794,   474,  -794,  1102,  -794,  -794,  -794,  -794,  -794,  -794,
    1182,  1192,  -794,  1193,  1194,  1195,  -794,  1216,  1102,   827,
    -794,  -794,  -794,  -794,  -794,  1161,  -794,   541,   179,  -794,
     509,  3266,  3266,  3266,  3266,  3266,  3266,  3266,  3266,  -794,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  1102,   -11,
    3266,  3266,  3266,  -794,  -794,  -794,   518,   519,   520,   528,
     533,   545,   547,   551,  -794,  -794,   561,   562,   565,  -794,
    -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794,  -794
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   397,   398,   399,   401,   402,   407,
     408,   422,   421,   423,   424,     0,   409,   410,   411,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   416,   400,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   302,   304,   322,
     324,   327,   329,   332,   334,   336,   339,   344,   349,   351,
     365,   393,     0,   405,   406,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   407,   408,   422,     0,   429,   347,     0,   402,    35,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   332,
       0,   430,   301,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   139,     0,     0,
     104,     0,     0,   105,     0,     0,     0,     3,     0,   391,
      96,    98,   388,   203,     0,     0,     0,   348,     0,   403,
     404,   346,   345,   425,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   300,     0,   371,     0,     0,     0,
       0,     0,     0,     0,     0,   376,     0,   375,   421,   367,
       0,     0,     0,     0,     0,    30,     0,   425,     0,     0,
      38,   396,     0,     6,     0,     9,     0,    10,     0,     0,
       0,     0,     0,     0,   352,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    42,
       0,     0,   117,     0,   129,     0,     0,   148,     0,     0,
       0,     3,   155,    45,    46,     0,     0,   151,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   389,   390,   392,     0,     0,
     206,     0,     0,     0,   241,   426,     0,   266,   402,   407,
     408,   422,     0,     0,     0,     0,     0,     0,   265,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   308,   311,   312,
     305,   306,     0,   309,   310,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   323,   325,   326,   328,   330,   331,
     333,   335,   337,   338,   343,   340,   341,   342,   350,   354,
     364,   358,   359,   360,   361,   362,   363,   356,   357,   355,
     387,     0,   384,    51,     0,    85,    86,    87,    88,    89,
      48,     0,     0,   370,     0,     0,    15,     0,     0,     0,
       0,   412,   417,   418,    53,     0,   379,     0,     0,   368,
       0,   366,   396,     0,     0,     0,   300,    37,     0,   395,
       0,     0,     0,   399,   421,     0,     0,     0,     0,    27,
       0,    28,     0,    29,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   426,     0,     0,   120,   426,     0,     0,
     330,   331,   251,     0,   261,   263,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,   426,     0,
       0,     0,     0,   106,   107,   108,   109,   299,    97,     0,
       0,   206,     0,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,     0,     0,   292,     0,     0,
       0,   293,     0,     0,   294,     0,     0,     0,     0,     0,
       0,     0,     0,   267,    55,    57,    59,    62,    60,    61,
      56,    58,    68,    70,    72,    75,    73,    74,    69,    71,
       0,     0,   380,     0,     0,     0,     0,   165,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   378,     0,
     374,     0,     0,   245,     0,     0,   412,   417,   418,    36,
       5,     0,     0,    11,     0,     0,     0,     0,    20,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     353,     0,    76,    79,    78,    82,    80,    81,    77,    40,
       0,    83,    84,    43,   114,   119,   118,     0,     0,   128,
     145,     0,   147,     0,     0,     0,   252,     0,     0,   154,
       0,     0,     0,     0,     0,   166,     0,    32,    31,     0,
       0,   149,   419,   420,     0,     0,   130,   133,   134,   137,
     138,     0,     0,   204,   205,     0,     0,     0,     0,   202,
     163,     0,     0,     0,     0,     0,     0,     0,   240,   428,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     276,   283,   290,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   386,     0,   383,
       0,     0,    50,     0,    52,   164,     0,    17,    16,   415,
     414,   413,     0,     0,     0,   377,   372,   373,     0,   369,
       0,     0,     0,   248,   247,     0,     0,     0,    22,     0,
      23,     0,    26,   157,     0,    34,    33,     0,   428,   428,
       0,   121,     0,   122,     0,     0,     0,   259,   253,   254,
     153,   255,     0,   262,   264,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,     0,
      66,    67,     0,     0,     0,   201,     0,     0,     0,     0,
       0,     0,     0,     0,   162,   160,     0,     0,     0,     0,
       0,     0,     0,   412,   418,   287,     0,     0,     0,     0,
       0,     0,   295,   296,   297,   298,   268,   270,   272,   275,
     273,   274,   269,   271,   385,   382,   381,    49,   428,    63,
      64,    65,   246,     0,    13,    12,    21,     0,     0,     0,
       0,   156,     7,     0,    41,    44,   116,   115,   426,     0,
     126,   146,   143,   144,     0,   256,     0,     0,     0,     0,
       0,   170,   171,   168,     0,     0,     0,     0,     0,     0,
       0,     0,   152,   150,   131,     0,     0,   142,     0,     3,
     236,     0,   233,     0,     0,     0,     0,   212,     0,     0,
       0,     0,   207,     0,     0,     0,     0,   208,   209,     0,
     159,   161,   242,   243,   244,     0,   165,     0,     0,     0,
     277,   279,   282,   280,   281,   278,    18,   428,   250,   249,
       0,    24,     0,   123,   124,   260,   257,   258,     0,     0,
       0,   172,   174,   176,   179,   177,   178,   173,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,   224,     0,     0,     0,     0,     0,   214,
       0,     0,     0,     0,   213,   210,   211,     0,   239,   286,
     285,   164,     0,     0,    14,   428,     8,     0,   127,   180,
     182,   181,     0,   141,   235,     0,   184,     0,     0,     0,
     183,     0,   232,     0,   221,   222,   216,   219,   220,   215,
       0,     0,   225,     0,     0,     0,   226,     0,     0,     0,
     284,   288,   289,    25,   125,     0,   234,     0,     0,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
     231,   229,   230,   218,   227,   228,   217,   238,     0,     0,
       0,     0,     0,   188,   189,   186,     0,     0,     0,     0,
       0,     0,     0,     0,   237,   140,     0,     0,     0,   190,
     192,   194,   197,   195,   196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -794,  -794,     4,  -794,  -489,  -638,  -794,  -794,  -794,  -794,
    -794,   333,  -794,  -794,  -794,  -794,  -794,  -280,  -793,  -794,
    -794,  -794,   805,  -794,  -794,  -440,   876,  -333,   310,   913,
      -2,  -208,  -794,  1049,  1124,  -794,   529,   -16,  1119,  1120,
     731,    -4,  -794,  -794,  -235,  -794,  -794,   -62,  -794
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   186,    61,    62,   496,    63,    64,    65,    66,
      67,   896,    68,    69,   511,    70,    71,   312,   900,    72,
      73,   339,   532,    74,   537,   260,   466,   507,   115,   165,
     121,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   285,   420,    89,    90,   122,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,   475,   422,   382,     2,   645,   771,   601,   534,   502,
     139,   116,   605,   503,   902,   502,    24,   529,   305,   503,
     131,   133,   135,   137,   138,   530,   140,   143,   145,   435,
     148,   502,   656,   152,   154,   503,   103,   651,   107,   297,
     247,   169,   313,   105,   172,   436,   177,   261,   437,   712,
     549,   502,   181,   182,   104,   503,   298,   184,   269,   502,
     158,   106,   875,   503,   504,   505,   306,   550,   117,    60,
     504,   505,   297,   297,   653,   669,   670,   159,   123,   124,
     125,   342,   126,   161,     3,   271,   504,   505,    60,   298,
     298,   657,   658,   239,   240,   241,   242,   243,   244,   127,
     162,   249,   251,   128,   253,   254,   504,   505,   903,   255,
     272,   227,   256,   146,   504,   505,   904,   147,   268,    60,
      60,   876,   877,   502,   273,   506,   988,   503,   286,   343,
     296,   533,   301,   531,   303,   870,   274,   299,   276,   654,
     300,   626,    60,   730,   711,   222,   223,   625,   160,   322,
     275,   324,   277,   257,   329,   551,   330,   331,   160,   332,
     333,   905,   334,   335,   336,   334,   337,   692,  1004,   671,
     299,   299,  1012,   300,   300,   961,   227,   882,   504,   505,
     149,   163,   225,   226,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   129,   368,   369,   370,   371,   372,   373,
     374,   375,   693,   694,   906,   297,   908,   150,   975,   398,
     399,   502,  1036,   164,   909,   503,   976,   175,   907,   282,
    1050,   227,   298,   283,   980,   552,   659,   404,   405,   406,
     407,   408,   981,   488,   176,  1057,   314,   555,   492,   965,
     556,   557,   553,   713,   499,   502,   434,   558,   326,   503,
     559,   560,   612,   690,   561,   562,   173,   263,   318,   910,
     453,   977,   455,   327,   456,  1074,   504,   505,   613,   459,
     460,   614,   462,   174,   467,   264,   319,   982,   695,   696,
     697,   866,   448,   431,   539,   481,   482,   483,   484,   485,
     486,   487,   500,   501,   821,   490,   491,   616,   432,   178,
     504,   505,   911,   299,   978,   998,   300,   686,   687,   822,
    1064,   688,    75,   509,   510,   508,   912,   513,   979,   515,
     983,    93,    94,   163,   519,   520,   179,  1028,   185,   108,
     554,   109,   308,   309,   984,   204,   698,   141,   142,   686,
     687,   320,   563,   688,   108,   763,   109,   155,   157,   102,
     544,   545,   546,   548,   170,   171,   103,   756,   444,   321,
     781,   105,   585,   468,   782,   825,   757,   224,    93,    94,
     588,   227,   726,   235,   541,   591,   470,   469,   594,   542,
     780,   266,   689,   599,   704,   602,   472,   288,   705,   789,
     471,   180,   790,   791,   706,   228,   102,   707,   708,   792,
     473,   709,   793,   794,   156,   270,   795,   796,   271,    60,
     523,   814,   815,   236,   805,   783,   784,   832,   631,   524,
     205,   206,   207,   208,   141,   231,   833,   640,   525,   586,
     209,   210,   211,   590,   237,   834,   592,   593,   310,   232,
     233,   234,   526,   311,   238,   302,   603,   304,   307,   835,
     459,   807,   212,   213,   572,   278,   279,   280,   573,   611,
     574,   836,   323,   184,   325,   837,   621,   838,   623,   575,
     624,   576,   577,   578,   627,   628,   839,   245,   840,   841,
     842,   579,   294,   314,   861,   295,   474,   204,   843,   710,
     883,   311,   632,   648,   797,   108,    75,   109,   878,   930,
    1041,   288,   879,  1042,  1043,   440,   666,   441,   290,   634,
    1044,   291,   292,  1045,  1046,   880,   931,  1047,  1048,   728,
     682,   731,   635,   636,   637,   638,   572,   736,   573,   932,
     933,   934,   935,   951,   262,   952,   920,   921,   574,   701,
     702,   703,   575,   618,   252,   953,   619,   714,   715,   954,
     716,   717,   752,   718,   719,   720,   721,   722,   723,   724,
     725,   639,   576,   265,   271,   962,   577,  1060,   966,   955,
     288,  1061,   287,   956,   898,   288,   289,   290,   919,   899,
     291,   292,   290,   578,  1062,   291,   292,   901,   643,   293,
     957,   271,   899,   742,   743,   744,   294,   579,   204,   295,
     734,   632,   634,   881,   958,  1049,   281,   989,   999,  1000,
     751,   638,   187,   753,   754,   188,   189,   849,  1001,   284,
     759,   851,   190,   313,  1031,   191,   192,   767,  1032,   193,
     194,   229,   230,   314,   214,   215,   216,   217,   218,   219,
    1065,   314,   572,   573,   574,   772,   220,   315,   528,  1079,
    1080,  1081,   575,   777,  1029,   738,   221,   576,   271,  1082,
     785,   786,   788,   760,  1083,   294,   761,   798,   295,   577,
     294,   578,  1063,   295,   316,   579,  1084,   196,  1085,   803,
     197,   198,  1086,   345,   346,   632,   634,   199,   853,   638,
     200,   201,  1087,  1088,   202,   203,  1089,   855,   862,   860,
     271,   863,   826,   827,   828,   829,   830,   831,   808,   317,
     195,   809,   810,   811,   812,   987,     4,     5,     6,     7,
     899,   423,   424,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,   913,   941,   338,   914,   271,   915,
     916,   391,   392,   393,   114,    28,   852,   996,   433,   271,
     271,   395,   396,   397,    33,   440,   271,   606,   741,   440,
     927,   823,    41,    42,    43,   438,   439,    46,   449,   450,
     451,   452,   458,   271,   341,   204,    50,    51,   478,   479,
     340,    52,   345,   480,   345,   489,   347,   884,   885,   886,
     887,   888,   889,   890,   891,   493,   346,   940,   494,   495,
     344,   942,   897,   644,   646,   497,   346,   649,   650,   652,
      54,    55,   516,   346,   518,   346,   345,   589,    75,   925,
     660,   661,   691,   661,    56,   425,   676,   677,   678,   699,
     271,    57,   733,   271,   735,   271,   426,   144,    59,   427,
     745,   271,   428,    60,   547,   746,   450,   747,   271,   749,
     450,     4,     5,     6,     7,   938,   939,   429,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,   345,
     750,   442,   945,   755,   619,   430,   948,   949,   950,   114,
      28,   764,   661,   768,   271,   769,   271,   770,   495,    33,
     816,   661,   443,   959,   817,   661,   444,    41,    42,    43,
     844,   271,    46,   964,   970,   845,   450,   846,   271,   974,
     445,    50,    51,   409,   848,   271,    52,   345,   859,   868,
     346,   410,   446,   411,   412,   447,   992,   993,   869,   495,
     937,   271,   413,   414,   991,   271,   995,   271,   997,   495,
     415,   416,  1005,   661,   454,    54,    55,   457,   417,   418,
     419,  1013,   661,  1058,   661,   166,   168,   806,   773,    56,
     402,   403,  1011,   476,   477,   498,    57,   512,   514,   517,
     522,   521,   144,    59,   409,   523,  1023,   421,    60,   787,
     799,  1027,   410,   524,   411,   412,   525,   526,   527,   535,
     536,   543,   564,   413,   414,   565,   566,   567,   568,   569,
     897,   415,   416,   570,   571,  1037,  1038,  1040,   572,   417,
     418,   419,   573,   574,   575,   576,   577,   578,   579,   580,
     221,   227,   581,   582,   604,  -429,  -429,   583,   595,   596,
     597,   608,   615,   609,   607,   629,   610,   630,   673,  1066,
    1067,  1068,  1069,  1070,  1071,  1072,  1073,   620,   632,   655,
     681,   633,   634,   635,   672,   636,   637,   638,  1076,  1077,
    1078,   641,   642,  -429,  -429,   311,  -429,  -429,  -429,  -429,
    -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,  -429,   674,
     867,   667,  -429,   668,   871,   872,   873,   679,   680,   675,
    -429,  -429,  -429,  -429,   683,  -429,   684,   700,  -429,  -429,
    -429,  -429,  -429,  -429,   732,  -429,  -429,  -429,   892,  -429,
     739,   734,   894,   737,   740,     4,     5,     6,     7,   748,
     758,   762,   348,   349,   350,   351,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   352,   353,    24,   765,
     766,    25,    26,    27,    28,   354,   355,    31,    32,   774,
     775,   776,   778,    33,    34,    35,    36,    37,    38,   356,
     357,    41,    42,    43,    44,    45,    46,    47,   779,   800,
     801,    48,    49,   802,   804,    50,    51,   409,   943,   944,
      52,   813,   818,   819,    53,   410,   820,   411,   412,   824,
     847,   849,   850,   851,   854,   856,   413,   414,   857,   858,
     864,   865,   874,   893,   415,   416,   922,   923,   895,    54,
      55,   917,   417,   418,   419,   918,   924,   928,   929,   936,
     946,   947,   960,    56,   963,   969,   967,   968,   971,   972,
      57,   990,   899,   973,   985,   986,    58,    59,   994,  1002,
    1014,  1015,    60,   358,  1006,    92,  1020,    93,    94,    95,
      96,    97,    98,    99,   376,   377,   378,   379,   380,   381,
     100,   383,   384,   385,   386,   387,   388,   389,   390,  1016,
    1003,  1017,  1018,   101,    75,   102,  1019,  1021,  1022,  1024,
    1025,  1026,     4,     5,     6,     7,  1030,  1059,  1033,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   662,   663,    24,  1051,  1034,    25,    26,
      27,    28,    29,   664,    31,    32,  1052,  1053,  1054,  1055,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,  1035,   685,     0,    48,    49,
    1056,   617,    50,    51,     0,   394,   400,    52,   401,     0,
      92,    53,    93,    94,    95,    96,    97,    98,    99,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,  1075,
       0,     0,     0,     0,     0,     0,    54,    55,   540,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,    58,    59,     4,     5,     6,     7,    60,
     665,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,  1007,  1008,    24,     0,
       0,    25,    26,    27,    28,    29,  1009,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,     0,     0,
       0,    48,    49,     0,     0,    50,    51,     0,     0,     0,
      52,     0,     0,     0,    53,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,    58,    59,    46,     0,
       0,     0,    60,  1010,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,   144,    59,
       4,     5,     6,     7,    60,  1039,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,    26,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
      50,    51,     0,     0,     0,    52,     0,     0,     0,    53,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,   119,   120,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,   144,    59,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   183,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   246,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,   258,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,   250,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   259,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
     328,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     267,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,    59,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,     0,     0,
     185,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   538,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     584,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   587,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,   258,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   598,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,   463,     7,     0,     0,     0,   118,   111,   112,   113,
     464,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   600,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   183,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,   258,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   727,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   729,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   926,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   130,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   132,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   134,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   136,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,    59,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   151,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   153,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,   167,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   248,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,   463,     7,     0,     0,     0,   118,   111,
     112,   113,   464,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   461,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,   258,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   465,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,    59,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,   622,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   110,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,     0,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   647,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
       0,    59,     0,     0,     0,     0,    60
};

static const yytype_int16 yycheck[] =
{
       2,   281,   237,   211,     0,   494,   644,   447,   341,    10,
      26,    15,   452,    14,   807,    10,    26,    14,    79,    14,
      22,    23,    24,    25,    26,    22,    14,    29,    30,   108,
      32,    10,    10,    35,    36,    14,   110,    79,   110,    26,
     102,    43,   124,   112,    46,   124,    50,   109,   127,   131,
      26,    10,    56,    57,   128,    14,    43,    59,   120,    10,
      26,   130,    10,    14,    65,    66,   127,    43,    14,   130,
      65,    66,    26,    26,    79,    37,    38,    43,    10,    11,
      12,    14,    14,    26,     0,   127,    65,    66,   130,    43,
      43,    69,    70,    95,    96,    97,    98,    99,   100,    31,
      43,   103,   104,    35,   106,   107,    65,    66,    14,    14,
     110,   116,    17,    10,    65,    66,    22,    14,   120,   130,
     130,    69,    70,    10,   124,   126,   919,    14,   130,    62,
     132,   126,   134,   130,   136,   773,   110,   124,   110,    79,
     127,   474,   130,   583,   131,   114,   115,   126,   124,   151,
     124,   153,   124,    58,   156,   131,   158,   159,   124,   161,
     162,    67,   164,   165,   166,   167,   168,   126,   961,   131,
     124,   124,   965,   127,   127,   126,   116,   131,    65,    66,
      12,   124,    77,    78,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   125,   196,   197,   198,   199,   200,   201,
     202,   203,   535,   536,   110,    26,    14,    12,    14,   225,
     226,    10,  1005,   130,    22,    14,    22,    14,   124,    10,
    1013,   116,    43,    14,    14,    26,   506,   231,   232,   233,
     234,   235,    22,   295,    31,  1028,   124,    26,   300,   126,
      29,    30,    43,   131,   306,    10,   248,    36,    29,    14,
      39,    40,   108,   533,    43,    44,    14,   112,   112,    67,
     262,    67,   264,    44,   266,  1058,    65,    66,   124,   271,
     272,   127,   274,    14,   276,   130,   130,    67,    73,    74,
      75,   770,    76,   110,   346,   287,   288,   289,   290,   291,
     292,   293,   308,   309,   110,   297,   298,    76,   125,    14,
      65,    66,   110,   124,   110,   943,   127,    56,    57,   125,
     131,    60,     2,   315,   316,   311,   124,   319,   124,   321,
     110,    97,    98,   124,   326,   327,    10,   126,    71,   123,
     131,   125,    77,    78,   124,   124,   131,    27,    28,    56,
      57,   112,   131,    60,   123,   625,   125,    37,    38,   125,
     352,   353,   354,   355,    44,    45,   110,     5,   124,   130,
      10,   112,   424,   110,    14,   131,    14,   104,    97,    98,
     432,   116,   580,   109,   128,   437,   110,   124,   440,   130,
     660,   110,   131,   445,    26,   447,   110,    29,    30,    26,
     124,    10,    29,    30,    36,   107,   125,    39,    40,    36,
     124,    43,    39,    40,   125,   124,    43,    44,   127,   130,
     124,   691,   692,   123,   131,    65,    66,   131,   480,   124,
      80,    81,    82,    83,   114,   105,   131,   489,   124,   431,
      90,    91,    92,   435,   123,   131,   438,   439,   125,   119,
     120,   121,   124,   130,    14,   135,   448,   137,   138,   131,
     452,   686,   112,   113,   124,    10,    11,    12,   124,   461,
     124,   131,   152,   465,   154,   131,   468,   131,   470,   124,
     472,   124,   124,   124,   476,   477,   131,    14,   131,   131,
     131,   124,   124,   124,   764,   127,   125,   124,   131,   131,
     131,   130,   124,   495,   131,   123,   186,   125,    26,   131,
      26,    29,    30,    29,    30,   127,   508,   129,    36,   124,
      36,    39,    40,    39,    40,    43,   131,    43,    44,   581,
     522,   583,   124,   124,   124,   124,   124,   589,   124,   131,
     131,   131,   131,   131,   128,   131,   816,   817,   124,   541,
     542,   543,   124,   124,    14,   131,   127,   549,   550,   131,
     552,   553,   614,   555,   556,   557,   558,   559,   560,   561,
     562,   124,   124,   124,   127,   898,   124,    26,   901,   131,
      29,    30,    26,   131,   125,    29,    30,    36,   813,   130,
      39,    40,    36,   124,    43,    39,    40,   125,   124,    43,
     131,   127,   130,   595,   596,   597,   124,   124,   124,   127,
     124,   124,   124,   131,   131,   131,    14,   131,   131,   131,
     612,   124,    26,   615,   616,    29,    30,   124,   131,    10,
     622,   124,    36,   124,   131,    39,    40,   629,   131,    43,
      44,   117,   118,   124,    84,    85,    86,    87,    88,    89,
     131,   124,   124,   124,   124,   647,    96,   110,   338,   131,
     131,   131,   124,   655,   987,   124,   106,   124,   127,   131,
     662,   663,   664,   124,   131,   124,   127,   669,   127,   124,
     124,   124,   131,   127,   110,   124,   131,    26,   131,   681,
      29,    30,   131,   126,   127,   124,   124,    36,   750,   124,
      39,    40,   131,   131,    43,    44,   131,   124,   124,   761,
     127,   127,   704,   705,   706,   707,   708,   709,    10,   130,
     124,    13,    14,    15,    16,   125,     3,     4,     5,     6,
     130,   124,   125,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    10,   124,    53,    13,   127,    15,
      16,     7,     8,     9,    31,    32,   748,   124,   126,   127,
     127,   222,   223,   224,    41,   127,   127,   129,   129,   127,
     822,   129,    49,    50,    51,   117,   118,    54,   126,   127,
     126,   127,   126,   127,   125,   124,    63,    64,   126,   127,
     130,    68,   126,   127,   126,   127,   131,   789,   790,   791,
     792,   793,   794,   795,   796,   126,   127,   859,    27,    28,
     130,   863,   804,   493,   494,   126,   127,   497,   498,   499,
      97,    98,   126,   127,   126,   127,   126,   127,   508,   821,
     126,   127,   126,   127,   111,   124,   516,   517,   518,   126,
     127,   118,   126,   127,   126,   127,   124,   124,   125,   124,
     126,   127,   124,   130,   131,   126,   127,   126,   127,   126,
     127,     3,     4,     5,     6,   857,   858,   124,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,   126,
     127,   113,   874,   126,   127,   124,   878,   879,   880,    31,
      32,   126,   127,   126,   127,   126,   127,    27,    28,    41,
     126,   127,   131,   895,   126,   127,   124,    49,    50,    51,
     126,   127,    54,   899,   906,   126,   127,   126,   127,   911,
     125,    63,    64,    14,   126,   127,    68,   126,   127,   126,
     127,    22,    14,    24,    25,   125,   928,   929,    27,    28,
     126,   127,    33,    34,   126,   127,   126,   127,    27,    28,
      41,    42,   126,   127,    14,    97,    98,   124,    49,    50,
      51,   126,   127,   126,   127,    42,    43,    58,   648,   111,
     229,   230,   964,   110,   110,    10,   118,    14,    14,    27,
     124,    10,   124,   125,    14,   124,   978,    17,   130,   131,
     670,   983,    22,   124,    24,    25,   124,   124,   131,   125,
     125,   110,   124,    33,    34,   124,   124,   124,   124,   124,
    1002,    41,    42,   124,   124,  1007,  1008,  1009,   124,    49,
      50,    51,   124,   124,   124,   124,   124,   124,   124,    93,
     106,   116,   125,    14,    14,    39,    40,   125,   110,   110,
     110,   131,    76,   124,   113,   110,   124,    10,   131,  1041,
    1042,  1043,  1044,  1045,  1046,  1047,  1048,   124,   124,   110,
     110,   124,   124,   124,   113,   124,   124,   124,  1060,  1061,
    1062,   124,   124,    77,    78,   130,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,   113,
     770,   124,    96,   124,   774,   775,   776,   124,   124,   131,
     104,   105,   106,   107,    14,   109,    14,    14,   112,   113,
     114,   115,   116,   117,   124,   119,   120,   121,   798,   123,
     129,   124,   802,   124,   129,     3,     4,     5,     6,    76,
     124,   124,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   124,
     124,    29,    30,    31,    32,    33,    34,    35,    36,    10,
      10,    10,    10,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    10,   124,
     124,    59,    60,    27,   124,    63,    64,    14,   868,   869,
      68,    56,    14,    14,    72,    22,    14,    24,    25,   131,
     124,   124,   124,   124,   124,   124,    33,    34,    76,    76,
     124,   124,   110,   131,    41,    42,     7,     7,   124,    97,
      98,   124,    49,    50,    51,   124,     7,   110,   110,   124,
      10,    10,   126,   111,    71,   124,    14,    14,    14,    14,
     118,   131,   130,   124,   124,   124,   124,   125,   124,   124,
     124,   124,   130,   131,   131,    95,    14,    97,    98,    99,
     100,   101,   102,   103,   205,   206,   207,   208,   209,   210,
     110,   212,   213,   214,   215,   216,   217,   218,   219,   124,
     960,   124,   124,   123,   964,   125,   124,    14,   124,    14,
      14,   124,     3,     4,     5,     6,   131,   126,   124,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   124,   997,    29,    30,
      31,    32,    33,    34,    35,    36,   124,   124,   124,   124,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,  1002,   531,    -1,    59,    60,
     124,   465,    63,    64,    -1,   221,   227,    68,   228,    -1,
      95,    72,    97,    98,    99,   100,   101,   102,   103,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,  1059,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,   123,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,   124,   125,     3,     4,     5,     6,   130,
     131,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    -1,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    72,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    41,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    49,    50,    51,   124,   125,    54,    -1,
      -1,    -1,   130,   131,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,   124,   125,
       3,     4,     5,     6,   130,   131,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    72,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      41,    -1,    -1,    -1,    -1,   118,    -1,    -1,    49,    50,
      51,   124,   125,    54,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    41,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      49,    50,    51,   124,   125,    54,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    41,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    49,    50,    51,   124,   125,    54,    -1,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    41,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    49,    50,    51,   125,   126,
      54,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      41,    -1,    -1,    -1,   118,    -1,    -1,    -1,    49,    50,
      51,   125,   126,    54,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    41,    -1,    -1,    -1,    -1,   118,   119,    -1,
      49,    50,    51,    -1,   125,    54,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    41,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    49,    50,    51,   125,   126,    54,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      23,    97,    98,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    41,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    49,    50,    51,   125,
     126,    54,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      41,    -1,    -1,    -1,    -1,   118,    -1,    -1,    49,    50,
      51,    -1,   125,    54,    -1,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      71,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    41,    -1,    -1,    -1,    -1,   118,    -1,    -1,
      49,    50,    51,    -1,   125,    54,    -1,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    97,    98,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   111,    -1,    -1,    41,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    49,    50,    51,   125,   126,    54,    -1,
      -1,   130,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    41,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    49,    50,    51,   125,
     126,    54,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   111,    -1,
      -1,    41,    -1,    -1,    -1,   118,    -1,    -1,    -1,    49,
      50,    51,   125,   126,    54,    -1,    -1,   130,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    -1,    41,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    49,    50,    51,   125,   126,    54,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      97,    98,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    41,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    49,    50,    51,   125,   126,
      54,    -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      41,    -1,    -1,    -1,   118,    -1,    -1,    -1,    49,    50,
      51,   125,   126,    54,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    97,    98,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     111,    -1,    -1,    41,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    49,    50,    51,   125,   126,    54,    -1,    -1,   130,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    -1,    41,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    49,    50,    51,   125,   126,    54,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    41,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    49,    50,    51,
     125,   126,    54,    -1,    -1,   130,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    41,    -1,    -1,    -1,    -1,   118,    -1,    -1,    49,
      50,    51,    -1,   125,    54,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    41,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    49,    50,    51,    -1,   125,    54,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    41,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    49,    50,    51,    -1,   125,    54,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    41,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    49,    50,    51,    -1,   125,
      54,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    41,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    49,    50,    51,
      -1,   125,    54,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    41,    -1,    -1,    -1,    -1,   118,    -1,    -1,    49,
      50,    51,    -1,   125,    54,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    41,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    49,    50,    51,    -1,   125,    54,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    41,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    49,    50,    51,    -1,   125,    54,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    41,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    49,    50,    51,    -1,   125,
      54,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    41,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    49,    50,    51,
      -1,   125,    54,    -1,    -1,    -1,   130,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    97,    98,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   111,
      -1,    41,    -1,    -1,    -1,    -1,   118,    -1,    -1,    49,
      50,    51,    -1,   125,    54,    -1,    -1,    -1,   130,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    68,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    97,    98,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   111,    -1,    41,    -1,    -1,    -1,    -1,   118,    -1,
      -1,    49,    50,    51,    -1,   125,    54,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    97,
      98,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   111,    -1,    41,    -1,    -1,    -1,    -1,
     118,    -1,    -1,    49,    50,    51,    -1,   125,    54,    -1,
      -1,    -1,   130,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    97,    98,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   111,    -1,    41,    -1,    -1,
      -1,    -1,   118,    -1,    -1,    49,    50,    51,    -1,   125,
      54,    -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,
      -1,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,   130
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   133,   134,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    68,    72,    97,    98,   111,   118,   124,   125,
     130,   135,   136,   138,   139,   140,   141,   142,   144,   145,
     147,   148,   151,   152,   155,   160,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   177,
     178,   180,    95,    97,    98,    99,   100,   101,   102,   103,
     110,   123,   125,   110,   128,   112,   130,   110,   123,   125,
      10,    11,    12,    13,    31,   160,   173,    14,    10,   124,
     125,   162,   179,    10,    11,    12,    14,    31,    35,   125,
     125,   162,   125,   162,   125,   162,   125,   162,   162,   169,
      14,   160,   160,   162,   124,   162,    10,    14,   162,    12,
      12,   125,   162,   125,   162,   160,   125,   160,    26,    43,
     124,    26,    43,   124,   130,   161,   161,   130,   161,   162,
     160,   160,   162,    14,    14,    14,    31,   173,    14,    10,
      10,   173,   173,   126,   162,    71,   134,    26,    29,    30,
      36,    39,    40,    43,    44,   124,    26,    29,    30,    36,
      39,    40,    43,    44,   124,    80,    81,    82,    83,    90,
      91,    92,   112,   113,    84,    85,    86,    87,    88,    89,
      96,   106,   114,   115,   104,    77,    78,   116,   107,   117,
     118,   105,   119,   120,   121,   109,   123,   123,    14,   162,
     162,   162,   162,   162,   162,    14,   126,   179,   125,   162,
     119,   162,    14,   162,   162,    14,    17,    58,    14,   126,
     157,   179,   128,   112,   130,   124,   110,   126,   162,   179,
     124,   127,   110,   124,   110,   124,   110,   124,    10,    11,
      12,    14,    10,    14,    10,   175,   162,    26,    29,    30,
      36,    39,    40,    43,   124,   127,   162,    26,    43,   124,
     127,   162,   160,   162,   160,    79,   127,   160,    77,    78,
     125,   130,   149,   124,   124,   110,   110,   130,   112,   130,
     112,   130,   162,   160,   162,   160,    29,    44,    23,   162,
     162,   162,   162,   162,   162,   162,   162,   162,    53,   153,
     130,   125,    14,    62,   130,   126,   127,   131,    10,    11,
      12,    13,    24,    25,    33,    34,    47,    48,   131,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   165,   165,   165,   165,
     165,   165,   163,   165,   165,   165,   165,   165,   165,   165,
     165,     7,     8,     9,   166,   168,   168,   168,   169,   169,
     170,   171,   172,   172,   173,   173,   173,   173,   173,    14,
      22,    24,    25,    33,    34,    41,    42,    49,    50,    51,
     176,    17,   176,   124,   125,   124,   124,   124,   124,   124,
     124,   110,   125,   126,   162,   108,   124,   127,   117,   118,
     127,   129,   113,   131,   124,   125,    14,   125,    76,   126,
     127,   126,   127,   162,    14,   162,   162,   124,   126,   162,
     162,   125,   162,     5,    14,   125,   158,   162,   110,   124,
     110,   124,   110,   124,   125,   149,   110,   110,   126,   127,
     127,   162,   162,   162,   162,   162,   162,   162,   179,   127,
     162,   162,   179,   126,    27,    28,   137,   126,    10,   179,
     169,   169,    10,    14,    65,    66,   126,   159,   134,   162,
     162,   146,    14,   162,    14,   162,   126,    27,   126,   162,
     162,    10,   124,   124,   124,   124,   124,   131,   160,    14,
      22,   130,   154,   126,   159,   125,   125,   156,   126,   179,
     123,   128,   130,   110,   162,   162,   162,   131,   162,    26,
      43,   131,    26,    43,   131,    26,    29,    30,    36,    39,
      40,    43,    44,   131,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
      93,   125,    14,   125,   126,   179,   162,   126,   179,   127,
     162,   179,   162,   162,   179,   110,   110,   110,   126,   179,
     126,   157,   179,   162,    14,   157,   129,   113,   131,   124,
     124,   162,   108,   124,   127,    76,    76,   158,   124,   127,
     124,   162,   125,   162,   162,   126,   159,   162,   162,   110,
      10,   179,   124,   124,   124,   124,   124,   124,   124,   124,
     179,   124,   124,   124,   160,   136,   160,   125,   162,   160,
     160,    79,   160,    79,    79,   110,    10,    69,    70,   149,
     126,   127,    24,    25,    34,   131,   162,   124,   124,    37,
      38,   131,   113,   131,   113,   131,   160,   160,   160,   124,
     124,   110,   162,    14,    14,   154,    56,    57,    60,   131,
     149,   126,   126,   159,   159,    73,    74,    75,   131,   126,
      14,   162,   162,   162,    26,    30,    36,    39,    40,    43,
     131,   131,   131,   131,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   163,   126,   179,   126,
     157,   179,   124,   126,   124,   126,   179,   124,   124,   129,
     129,   129,   162,   162,   162,   126,   126,   126,    76,   126,
     127,   162,   179,   162,   162,   126,     5,    14,   124,   162,
     124,   127,   124,   149,   126,   124,   124,   162,   126,   126,
      27,   137,   162,   160,    10,    10,    10,   162,    10,    10,
     149,    10,    14,    65,    66,   162,   162,   131,   162,    26,
      29,    30,    36,    39,    40,    43,    44,   131,   162,   160,
     124,   124,    27,   162,   124,   131,    58,   176,    10,    13,
      14,    15,    16,    56,   149,   149,   126,   126,    14,    14,
      14,   110,   125,   129,   131,   131,   162,   162,   162,   162,
     162,   162,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   126,   126,   126,   124,   126,   124,
     124,   124,   162,   179,   124,   124,   124,    76,    76,   127,
     179,   149,   124,   127,   124,   124,   136,   160,   126,    27,
     137,   160,   160,   160,   110,    10,    69,    70,    26,    30,
      43,   131,   131,   131,   162,   162,   162,   162,   162,   162,
     162,   162,   160,   131,   160,   124,   143,   162,   125,   130,
     150,   125,   150,    14,    22,    67,   110,   124,    14,    22,
      67,   110,   124,    10,    13,    15,    16,   124,   124,   176,
     149,   149,     7,     7,     7,   162,   126,   179,   110,   110,
     131,   131,   131,   131,   131,   131,   124,   126,   162,   162,
     179,   124,   179,   160,   160,   162,    10,    10,   162,   162,
     162,   131,   131,   131,   131,   131,   131,   131,   131,   162,
     126,   126,   159,    71,   134,   126,   159,    14,    14,   124,
     162,    14,    14,   124,   162,    14,    22,    67,   110,   124,
      14,    22,    67,   110,   124,   124,   124,   125,   150,   131,
     131,   126,   162,   162,   124,   126,   124,    27,   137,   131,
     131,   131,   124,   160,   150,   126,   131,    24,    25,    34,
     131,   162,   150,   126,   124,   124,   124,   124,   124,   124,
      14,    14,   124,   162,    14,    14,   124,   162,   126,   159,
     131,   131,   131,   124,   160,   143,   150,   162,   162,   131,
     162,    26,    29,    30,    36,    39,    40,    43,    44,   131,
     150,   124,   124,   124,   124,   124,   124,   150,   126,   126,
      26,    30,    43,   131,   131,   131,   162,   162,   162,   162,
     162,   162,   162,   162,   150,   160,   162,   162,   162,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   132,   133,   134,   134,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     135,   135,   135,   135,   136,   136,   136,   136,   136,   136,
     136,   136,   137,   137,   137,   137,   137,   137,   138,   138,
     139,   139,   139,   139,   140,   140,   141,   141,   141,   142,
     142,   142,   143,   144,   144,   144,   144,   144,   144,   145,
     145,   146,   146,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   148,   148,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   151,   152,   153,   153,   153,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     155,   156,   156,   156,   156,   157,   157,   158,   158,   158,
     158,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   161,
     162,   162,   162,   163,   163,   164,   164,   164,   164,   164,
     164,   164,   164,   164,   164,   164,   164,   164,   164,   164,
     164,   164,   164,   165,   165,   166,   166,   166,   167,   167,
     168,   168,   168,   169,   169,   170,   170,   171,   171,   171,
     172,   172,   172,   172,   172,   173,   173,   173,   173,   173,
     174,   174,   175,   175,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   177,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   179,   179,   179,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180
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
       5,     1,     3,     1,     3,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     9,     8,     8,     6,     9,     9,
       5,     5,     4,     4,     4,     6,     6,     6,     6,     3,
       3,     2,     1,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     1,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     3,     1,
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
#line 431 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2898 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 441 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2904 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 442 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2910 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 446 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2916 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 448 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2922 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 450 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2928 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 452 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2936 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 456 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2942 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 458 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2948 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 460 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2954 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 462 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2962 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 466 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2969 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 469 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2977 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 473 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2983 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 475 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2991 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 479 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2998 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 482 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3006 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 486 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3012 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 488 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3018 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 490 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3024 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 492 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3030 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 494 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3036 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 496 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3044 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 500 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3052 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 504 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3058 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 506 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3064 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 508 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3070 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 510 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3076 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 512 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3082 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 514 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3088 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 516 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3094 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 518 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3100 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 520 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3106 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 522 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3112 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 524 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3118 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 526 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3124 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 528 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3130 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 530 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3136 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 532 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3143 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 535 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3150 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 538 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3156 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 540 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3163 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 543 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3170 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 546 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3176 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 548 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3182 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 550 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3188 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 552 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3194 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 554 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3203 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 559 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3211 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 563 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3219 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3228 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 572 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3236 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 575 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3242 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 577 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3248 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 579 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3254 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 581 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3260 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 583 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3266 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 585 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3273 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 588 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3279 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 590 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3285 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 592 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3291 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 594 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3298 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 597 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3305 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 600 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3312 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 603 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3319 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 606 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3326 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 609 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3332 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 611 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3338 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 613 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3344 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 615 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3350 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 617 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3357 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 620 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3363 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 622 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3369 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 624 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3375 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 626 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3382 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 629 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3389 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 632 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3397 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 636 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3404 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 639 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3410 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 641 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3416 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 643 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3422 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 645 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3429 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 648 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3436 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 651 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3443 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 654 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3450 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 657 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3457 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 660 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3464 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 663 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3471 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 665 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3477 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 666 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3483 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 667 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3489 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 668 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3495 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 669 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3501 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 670 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3507 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 672 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3513 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 674 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3519 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3525 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 678 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3531 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 679 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3537 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 680 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3543 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 681 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3549 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 682 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3555 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 683 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3561 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 684 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3567 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 686 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3573 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 688 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3579 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3585 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3591 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 693 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3597 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 694 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3603 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 695 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3609 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 696 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3615 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 700 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3621 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 702 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3627 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 704 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3633 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 706 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3639 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3645 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3651 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 712 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3657 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3663 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3669 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 720 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3675 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 722 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3681 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 724 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3687 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 726 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3693 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3699 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 732 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3705 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 734 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3711 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 738 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3717 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 740 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3723 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 742 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3729 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 744 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3735 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3741 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 750 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3747 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 754 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3753 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 756 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3759 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 758 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3765 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 762 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3771 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 764 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3777 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 766 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3783 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 769 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3789 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 773 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3799 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 779 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3809 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 785 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3818 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 790 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3828 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 796 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3837 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 801 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3844 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 806 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3856 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 814 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3869 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 824 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3875 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 826 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3883 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 832 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3895 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 840 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3905 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 846 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3915 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 852 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3927 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 860 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3937 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 866 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3947 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 872 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3960 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 881 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3971 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 888 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3984 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 897 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3997 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 906 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4008 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 915 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4019 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 922 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4028 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 928 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4034 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 930 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4041 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 933 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4048 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 936 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4054 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 938 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4060 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 940 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4066 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 942 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4072 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 944 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4078 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 946 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4084 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 948 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4090 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 950 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4097 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 953 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4103 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 955 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4109 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 957 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4115 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 959 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4122 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 962 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4129 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 965 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4137 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 970 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4143 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 971 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4149 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 973 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4156 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 976 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4163 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 979 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4169 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 981 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4175 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 983 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4181 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 985 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4187 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 987 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4193 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 989 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4199 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 991 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4205 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 993 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4212 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 996 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4218 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 998 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4224 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1000 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4230 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1002 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4237 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1005 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4244 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1008 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4252 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1014 "raku.y"
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
#line 4270 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1030 "raku.y"
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
#line 4287 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 1044 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4293 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 1046 "raku.y"
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
#line 4309 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1058 "raku.y"
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
#line 4325 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1071 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4331 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1073 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4338 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1076 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4345 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1079 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4352 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1082 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4359 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1085 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4366 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1088 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4373 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1091 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4380 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1094 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4387 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1097 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4394 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1100 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4401 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1103 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4408 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1106 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4415 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1109 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4426 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1116 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4437 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1123 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4448 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1130 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4459 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1137 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4467 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1141 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4475 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1145 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4483 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1149 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4491 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1153 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4502 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1160 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4513 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1167 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4524 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1174 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4535 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1181 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4549 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1191 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4561 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1199 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4573 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1207 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4586 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1216 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4597 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1223 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4608 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1230 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4622 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1240 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4634 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1248 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4646 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1258 "raku.y"
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
#line 4663 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1272 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4669 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1274 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4678 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1279 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4687 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1284 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4696 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1291 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4704 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1295 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4712 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1301 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4718 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1303 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4724 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1305 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4730 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1307 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4736 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1310 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4742 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1311 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4748 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1312 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4754 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1313 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4760 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1314 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4766 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1315 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4772 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1316 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4778 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1317 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4784 "raku.tab.c"
    break;

  case 259: /* param_list: VAR_SCALAR '=' expr  */
#line 1318 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4790 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1319 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4796 "raku.tab.c"
    break;

  case 261: /* param_list: SLURPY_POS  */
#line 1320 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4802 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' SLURPY_POS  */
#line 1321 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4808 "raku.tab.c"
    break;

  case 263: /* param_list: SLURPY_LOL  */
#line 1322 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4814 "raku.tab.c"
    break;

  case 264: /* param_list: param_list ',' SLURPY_LOL  */
#line 1323 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 4820 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list '}'  */
#line 1326 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4826 "raku.tab.c"
    break;

  case 266: /* block: '{' YADA '}'  */
#line 1327 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4832 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr '}'  */
#line 1329 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4838 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1331 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4844 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1333 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4850 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1335 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4856 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1337 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4862 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1339 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4869 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1342 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4875 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1344 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4881 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1346 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4887 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1348 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4893 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1350 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4900 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1353 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4907 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1356 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4915 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1360 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4922 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1363 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4929 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1366 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4936 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1369 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4942 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1371 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4950 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1375 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4957 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1378 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4964 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1381 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4971 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1384 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4978 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1387 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4985 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1390 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4991 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1392 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4997 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1394 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5003 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_LAST '}'  */
#line 1396 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 5009 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1398 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5015 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1400 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5021 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1402 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5027 "raku.tab.c"
    break;

  case 297: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1404 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5033 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1406 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5039 "raku.tab.c"
    break;

  case 299: /* closure: '{' expr '}'  */
#line 1409 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5045 "raku.tab.c"
    break;

  case 300: /* expr: VAR_SCALAR '=' expr  */
#line 1412 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5051 "raku.tab.c"
    break;

  case 301: /* expr: KW_GATHER block  */
#line 1413 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5061 "raku.tab.c"
    break;

  case 302: /* expr: tern_expr  */
#line 1418 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5067 "raku.tab.c"
    break;

  case 303: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1422 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5073 "raku.tab.c"
    break;

  case 304: /* tern_expr: cmp_expr  */
#line 1423 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5079 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1426 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5085 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1427 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5091 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1428 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5097 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1429 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5103 "raku.tab.c"
    break;

  case 309: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1430 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5109 "raku.tab.c"
    break;

  case 310: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1431 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5115 "raku.tab.c"
    break;

  case 311: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1432 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5121 "raku.tab.c"
    break;

  case 312: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1433 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5127 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1434 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5133 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1435 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5139 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1436 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5145 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1437 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5151 "raku.tab.c"
    break;

  case 317: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1438 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5157 "raku.tab.c"
    break;

  case 318: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1439 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5163 "raku.tab.c"
    break;

  case 319: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1441 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5173 "raku.tab.c"
    break;

  case 320: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1447 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5183 "raku.tab.c"
    break;

  case 321: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1453 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5193 "raku.tab.c"
    break;

  case 322: /* cmp_expr: divis_expr  */
#line 1458 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5199 "raku.tab.c"
    break;

  case 323: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1461 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5205 "raku.tab.c"
    break;

  case 324: /* divis_expr: jct_expr  */
#line 1462 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5211 "raku.tab.c"
    break;

  case 325: /* jct_expr: jct_expr '|' range_expr  */
#line 1465 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5217 "raku.tab.c"
    break;

  case 326: /* jct_expr: jct_expr '&' range_expr  */
#line 1466 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5223 "raku.tab.c"
    break;

  case 327: /* jct_expr: dor_expr  */
#line 1467 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5229 "raku.tab.c"
    break;

  case 328: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1471 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5235 "raku.tab.c"
    break;

  case 329: /* dor_expr: range_expr  */
#line 1472 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5241 "raku.tab.c"
    break;

  case 330: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1475 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5247 "raku.tab.c"
    break;

  case 331: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1476 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5253 "raku.tab.c"
    break;

  case 332: /* range_expr: add_expr  */
#line 1477 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5259 "raku.tab.c"
    break;

  case 333: /* add_expr: add_expr '~' repl_expr  */
#line 1480 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5265 "raku.tab.c"
    break;

  case 334: /* add_expr: repl_expr  */
#line 1481 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5271 "raku.tab.c"
    break;

  case 335: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1484 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5277 "raku.tab.c"
    break;

  case 336: /* repl_expr: addsub_expr  */
#line 1485 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5283 "raku.tab.c"
    break;

  case 337: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1488 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5289 "raku.tab.c"
    break;

  case 338: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1489 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5295 "raku.tab.c"
    break;

  case 339: /* addsub_expr: mul_expr  */
#line 1490 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5301 "raku.tab.c"
    break;

  case 340: /* mul_expr: mul_expr '*' unary_expr  */
#line 1493 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5307 "raku.tab.c"
    break;

  case 341: /* mul_expr: mul_expr '/' unary_expr  */
#line 1494 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5313 "raku.tab.c"
    break;

  case 342: /* mul_expr: mul_expr '%' unary_expr  */
#line 1495 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5319 "raku.tab.c"
    break;

  case 343: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1496 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5325 "raku.tab.c"
    break;

  case 344: /* mul_expr: unary_expr  */
#line 1497 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5331 "raku.tab.c"
    break;

  case 345: /* unary_expr: '-' unary_expr  */
#line 1500 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5337 "raku.tab.c"
    break;

  case 346: /* unary_expr: '!' unary_expr  */
#line 1501 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5343 "raku.tab.c"
    break;

  case 347: /* unary_expr: CARET unary_expr  */
#line 1502 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5349 "raku.tab.c"
    break;

  case 348: /* unary_expr: OP_REDUCE unary_expr  */
#line 1504 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5358 "raku.tab.c"
    break;

  case 349: /* unary_expr: pow_expr  */
#line 1508 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5364 "raku.tab.c"
    break;

  case 350: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1511 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5370 "raku.tab.c"
    break;

  case 351: /* pow_expr: postfix_expr  */
#line 1512 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5376 "raku.tab.c"
    break;

  case 352: /* scalar_list: VAR_SCALAR  */
#line 1515 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5382 "raku.tab.c"
    break;

  case 353: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1516 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5388 "raku.tab.c"
    break;

  case 354: /* meth_name: IDENT  */
#line 1519 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5394 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_SORT  */
#line 1520 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5400 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_MAP  */
#line 1521 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5406 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_GREP  */
#line 1522 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5412 "raku.tab.c"
    break;

  case 358: /* meth_name: KW_SAY  */
#line 1523 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5418 "raku.tab.c"
    break;

  case 359: /* meth_name: KW_PRINT  */
#line 1524 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5424 "raku.tab.c"
    break;

  case 360: /* meth_name: KW_TAKE  */
#line 1525 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5430 "raku.tab.c"
    break;

  case 361: /* meth_name: KW_RETURN  */
#line 1526 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5436 "raku.tab.c"
    break;

  case 362: /* meth_name: KW_EXISTS  */
#line 1527 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5442 "raku.tab.c"
    break;

  case 363: /* meth_name: KW_DELETE  */
#line 1528 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5448 "raku.tab.c"
    break;

  case 364: /* meth_name: TESTOP  */
#line 1529 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5454 "raku.tab.c"
    break;

  case 365: /* postfix_expr: call_expr  */
#line 1531 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5460 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '(' arg_list ')'  */
#line 1534 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5469 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '(' ')'  */
#line 1538 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5475 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1540 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5481 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1542 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5487 "raku.tab.c"
    break;

  case 370: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1544 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5496 "raku.tab.c"
    break;

  case 371: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1549 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5502 "raku.tab.c"
    break;

  case 372: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1551 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5512 "raku.tab.c"
    break;

  case 373: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1557 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5522 "raku.tab.c"
    break;

  case 374: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1563 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5530 "raku.tab.c"
    break;

  case 375: /* call_expr: IDENT '.' KW_NEW  */
#line 1567 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5538 "raku.tab.c"
    break;

  case 376: /* call_expr: IDENT '.' IDENT  */
#line 1571 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5547 "raku.tab.c"
    break;

  case 377: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1576 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5558 "raku.tab.c"
    break;

  case 378: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1583 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5567 "raku.tab.c"
    break;

  case 379: /* call_expr: IDENT '.' CARET IDENT  */
#line 1588 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5577 "raku.tab.c"
    break;

  case 380: /* call_expr: atom '.' CARET IDENT  */
#line 1594 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5587 "raku.tab.c"
    break;

  case 381: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1600 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5598 "raku.tab.c"
    break;

  case 382: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1607 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5609 "raku.tab.c"
    break;

  case 383: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1614 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5618 "raku.tab.c"
    break;

  case 384: /* call_expr: atom '.' meth_name  */
#line 1619 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5627 "raku.tab.c"
    break;

  case 385: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1624 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5638 "raku.tab.c"
    break;

  case 386: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5647 "raku.tab.c"
    break;

  case 387: /* call_expr: call_expr '.' meth_name  */
#line 1636 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5656 "raku.tab.c"
    break;

  case 388: /* call_expr: KW_DIE expr  */
#line 1641 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5662 "raku.tab.c"
    break;

  case 389: /* call_expr: KW_MAP closure expr  */
#line 1643 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5668 "raku.tab.c"
    break;

  case 390: /* call_expr: KW_GREP closure expr  */
#line 1645 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5674 "raku.tab.c"
    break;

  case 391: /* call_expr: KW_SORT expr  */
#line 1647 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5680 "raku.tab.c"
    break;

  case 392: /* call_expr: KW_SORT closure expr  */
#line 1649 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5686 "raku.tab.c"
    break;

  case 393: /* call_expr: atom  */
#line 1650 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5692 "raku.tab.c"
    break;

  case 394: /* arg_list: expr  */
#line 1653 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5698 "raku.tab.c"
    break;

  case 395: /* arg_list: arg_list ',' expr  */
#line 1654 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5704 "raku.tab.c"
    break;

  case 396: /* arg_list: arg_list ','  */
#line 1655 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5710 "raku.tab.c"
    break;

  case 397: /* atom: LIT_INT  */
#line 1658 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5716 "raku.tab.c"
    break;

  case 398: /* atom: LIT_FLOAT  */
#line 1659 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5722 "raku.tab.c"
    break;

  case 399: /* atom: LIT_STR  */
#line 1660 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5728 "raku.tab.c"
    break;

  case 400: /* atom: WORDLIST  */
#line 1662 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5740 "raku.tab.c"
    break;

  case 401: /* atom: LIT_INTERP_STR  */
#line 1669 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5746 "raku.tab.c"
    break;

  case 402: /* atom: VAR_SCALAR  */
#line 1670 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5752 "raku.tab.c"
    break;

  case 403: /* atom: OP_INC VAR_SCALAR  */
#line 1671 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5758 "raku.tab.c"
    break;

  case 404: /* atom: OP_DEC VAR_SCALAR  */
#line 1672 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5764 "raku.tab.c"
    break;

  case 405: /* atom: VAR_SCALAR OP_INC  */
#line 1673 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5770 "raku.tab.c"
    break;

  case 406: /* atom: VAR_SCALAR OP_DEC  */
#line 1674 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5776 "raku.tab.c"
    break;

  case 407: /* atom: VAR_ARRAY  */
#line 1675 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5782 "raku.tab.c"
    break;

  case 408: /* atom: VAR_HASH  */
#line 1676 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5788 "raku.tab.c"
    break;

  case 409: /* atom: VAR_CAPTURE  */
#line 1678 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5796 "raku.tab.c"
    break;

  case 410: /* atom: VAR_FH  */
#line 1682 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5804 "raku.tab.c"
    break;

  case 411: /* atom: VAR_NAMED_CAPTURE  */
#line 1686 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5811 "raku.tab.c"
    break;

  case 412: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1689 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5817 "raku.tab.c"
    break;

  case 413: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1691 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5823 "raku.tab.c"
    break;

  case 414: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1693 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5829 "raku.tab.c"
    break;

  case 415: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1695 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5835 "raku.tab.c"
    break;

  case 416: /* atom: ARR_ALL_SLICE  */
#line 1697 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5841 "raku.tab.c"
    break;

  case 417: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1699 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5847 "raku.tab.c"
    break;

  case 418: /* atom: VAR_HASH '{' expr '}'  */
#line 1701 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5853 "raku.tab.c"
    break;

  case 419: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1703 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5859 "raku.tab.c"
    break;

  case 420: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1705 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5865 "raku.tab.c"
    break;

  case 421: /* atom: IDENT  */
#line 1706 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5871 "raku.tab.c"
    break;

  case 422: /* atom: VAR_TWIGIL  */
#line 1708 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5879 "raku.tab.c"
    break;

  case 423: /* atom: VAR_ARRAY_TWIGIL  */
#line 1712 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5887 "raku.tab.c"
    break;

  case 424: /* atom: VAR_HASH_TWIGIL  */
#line 1716 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5895 "raku.tab.c"
    break;

  case 425: /* atom: '(' ')'  */
#line 1719 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5901 "raku.tab.c"
    break;

  case 426: /* atom: '(' expr ')'  */
#line 1720 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5907 "raku.tab.c"
    break;

  case 427: /* atom: '(' expr ',' ')'  */
#line 1722 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5913 "raku.tab.c"
    break;

  case 428: /* atom: '(' expr ',' arg_list ')'  */
#line 1724 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5920 "raku.tab.c"
    break;

  case 429: /* atom: block  */
#line 1726 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5926 "raku.tab.c"
    break;

  case 430: /* atom: KW_SUB block  */
#line 1727 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5932 "raku.tab.c"
    break;


#line 5936 "raku.tab.c"

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
