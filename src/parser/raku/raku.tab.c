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

#line 410 "raku.tab.c"

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
  YYSYMBOL_KW_HANDLES = 66,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 67,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 68,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 69,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 70,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 71,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 72,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 73,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 74,                  /* KW_REGEX  */
  YYSYMBOL_OP_FATARROW = 75,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 76,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 77,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 78,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 79,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 80,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 81,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 82,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 83,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 84,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 85,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 86,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 87,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 88,                    /* OP_SGE  */
  YYSYMBOL_OP_AND = 89,                    /* OP_AND  */
  YYSYMBOL_OP_OR = 90,                     /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 91,               /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 92,               /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 93,                   /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 94,                  /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 95,                 /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 96,                    /* OP_INC  */
  YYSYMBOL_OP_DEC = 97,                    /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 98,                 /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 99,                 /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 100,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 101,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 102,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 103,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 104,                   /* OP_DIV  */
  YYSYMBOL_OP_DIVIS = 105,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 106,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 107,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 108,                   /* OP_POW  */
  YYSYMBOL_109_ = 109,                     /* '='  */
  YYSYMBOL_110_ = 110,                     /* '!'  */
  YYSYMBOL_111_ = 111,                     /* '<'  */
  YYSYMBOL_112_ = 112,                     /* '>'  */
  YYSYMBOL_113_ = 113,                     /* '|'  */
  YYSYMBOL_114_ = 114,                     /* '&'  */
  YYSYMBOL_115_ = 115,                     /* '~'  */
  YYSYMBOL_116_ = 116,                     /* '+'  */
  YYSYMBOL_117_ = 117,                     /* '-'  */
  YYSYMBOL_118_ = 118,                     /* '*'  */
  YYSYMBOL_119_ = 119,                     /* '/'  */
  YYSYMBOL_120_ = 120,                     /* '%'  */
  YYSYMBOL_UMINUS = 121,                   /* UMINUS  */
  YYSYMBOL_122_ = 122,                     /* '.'  */
  YYSYMBOL_123_ = 123,                     /* ';'  */
  YYSYMBOL_124_ = 124,                     /* '('  */
  YYSYMBOL_125_ = 125,                     /* ')'  */
  YYSYMBOL_126_ = 126,                     /* ','  */
  YYSYMBOL_127_ = 127,                     /* '['  */
  YYSYMBOL_128_ = 128,                     /* ']'  */
  YYSYMBOL_129_ = 129,                     /* '{'  */
  YYSYMBOL_130_ = 130,                     /* '}'  */
  YYSYMBOL_YYACCEPT = 131,                 /* $accept  */
  YYSYMBOL_program = 132,                  /* program  */
  YYSYMBOL_stmt_list = 133,                /* stmt_list  */
  YYSYMBOL_stmt = 134,                     /* stmt  */
  YYSYMBOL_if_stmt = 135,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 136,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 137,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 138,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 139,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 140,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 141,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 142,                /* loop_incr  */
  YYSYMBOL_for_stmt = 143,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 144,               /* given_stmt  */
  YYSYMBOL_when_list = 145,                /* when_list  */
  YYSYMBOL_sub_decl = 146,                 /* sub_decl  */
  YYSYMBOL_scalar_methcall = 147,          /* scalar_methcall  */
  YYSYMBOL_sub_body = 148,                 /* sub_body  */
  YYSYMBOL_method_body = 149,              /* method_body  */
  YYSYMBOL_class_decl = 150,               /* class_decl  */
  YYSYMBOL_role_decl = 151,                /* role_decl  */
  YYSYMBOL_is_clauses = 152,               /* is_clauses  */
  YYSYMBOL_class_body_list = 153,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 154,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 155,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 156,           /* named_arg_list  */
  YYSYMBOL_pair_list = 157,                /* pair_list  */
  YYSYMBOL_param_list = 158,               /* param_list  */
  YYSYMBOL_block = 159,                    /* block  */
  YYSYMBOL_closure = 160,                  /* closure  */
  YYSYMBOL_expr = 161,                     /* expr  */
  YYSYMBOL_tern_expr = 162,                /* tern_expr  */
  YYSYMBOL_cmp_expr = 163,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 164,               /* divis_expr  */
  YYSYMBOL_jct_expr = 165,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 166,                 /* dor_expr  */
  YYSYMBOL_range_expr = 167,               /* range_expr  */
  YYSYMBOL_add_expr = 168,                 /* add_expr  */
  YYSYMBOL_repl_expr = 169,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 170,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 171,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 172,               /* unary_expr  */
  YYSYMBOL_pow_expr = 173,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 174,              /* scalar_list  */
  YYSYMBOL_meth_name = 175,                /* meth_name  */
  YYSYMBOL_postfix_expr = 176,             /* postfix_expr  */
  YYSYMBOL_call_expr = 177,                /* call_expr  */
  YYSYMBOL_arg_list = 178,                 /* arg_list  */
  YYSYMBOL_atom = 179                      /* atom  */
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
#define YYLAST   4142

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  131
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  428
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1088

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   364


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
       2,     2,     2,   110,     2,     2,     2,   120,   114,     2,
     124,   125,   118,   116,   126,   117,   122,   119,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,   123,
     111,   109,   112,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   127,     2,   128,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   129,   113,   130,   115,     2,     2,     2,
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
     105,   106,   107,   108,   121
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   425,   425,   436,   437,   440,   442,   444,   446,   450,
     452,   454,   456,   460,   463,   467,   469,   473,   476,   480,
     482,   484,   486,   488,   490,   494,   498,   500,   502,   504,
     506,   508,   510,   512,   514,   516,   518,   520,   522,   524,
     526,   529,   532,   534,   537,   540,   542,   544,   546,   548,
     553,   557,   561,   566,   570,   571,   573,   575,   577,   579,
     582,   584,   586,   588,   591,   594,   597,   600,   603,   605,
     607,   609,   611,   614,   616,   618,   620,   623,   626,   630,
     633,   635,   637,   639,   642,   645,   648,   651,   654,   657,
     660,   661,   662,   663,   664,   665,   666,   668,   670,   672,
     674,   675,   676,   677,   678,   679,   680,   682,   684,   686,
     688,   689,   690,   691,   694,   696,   698,   700,   702,   704,
     706,   708,   712,   714,   716,   718,   720,   722,   726,   728,
     732,   734,   736,   738,   742,   744,   748,   750,   752,   756,
     758,   760,   764,   767,   773,   779,   784,   790,   795,   800,
     808,   819,   820,   826,   834,   840,   846,   854,   860,   866,
     875,   882,   891,   900,   909,   916,   923,   924,   927,   930,
     932,   934,   936,   938,   940,   942,   944,   947,   949,   951,
     953,   956,   959,   965,   966,   967,   970,   973,   975,   977,
     979,   981,   983,   985,   987,   990,   992,   994,   996,   999,
    1002,  1008,  1024,  1039,  1040,  1052,  1066,  1067,  1070,  1073,
    1076,  1079,  1082,  1085,  1088,  1091,  1094,  1097,  1100,  1103,
    1110,  1117,  1124,  1131,  1135,  1139,  1143,  1147,  1154,  1161,
    1168,  1175,  1185,  1193,  1201,  1210,  1217,  1224,  1234,  1242,
    1252,  1267,  1268,  1273,  1278,  1285,  1289,  1295,  1297,  1299,
    1301,  1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1313,
    1314,  1315,  1316,  1319,  1320,  1321,  1323,  1325,  1327,  1329,
    1331,  1334,  1336,  1338,  1340,  1342,  1345,  1348,  1352,  1355,
    1358,  1361,  1363,  1367,  1370,  1373,  1376,  1379,  1382,  1384,
    1386,  1388,  1390,  1392,  1394,  1396,  1398,  1402,  1405,  1406,
    1411,  1414,  1416,  1419,  1420,  1421,  1422,  1423,  1424,  1425,
    1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,  1439,  1445,
    1451,  1454,  1455,  1458,  1459,  1460,  1463,  1465,  1468,  1469,
    1470,  1473,  1474,  1477,  1478,  1481,  1482,  1483,  1486,  1487,
    1488,  1489,  1490,  1493,  1494,  1495,  1496,  1501,  1504,  1505,
    1508,  1509,  1512,  1513,  1514,  1515,  1516,  1517,  1518,  1519,
    1520,  1521,  1522,  1524,  1526,  1531,  1532,  1534,  1536,  1541,
    1543,  1549,  1555,  1559,  1563,  1568,  1575,  1580,  1586,  1592,
    1599,  1606,  1611,  1616,  1623,  1628,  1633,  1635,  1637,  1639,
    1641,  1643,  1646,  1647,  1648,  1651,  1652,  1653,  1654,  1662,
    1663,  1664,  1665,  1666,  1667,  1668,  1669,  1670,  1674,  1678,
    1681,  1683,  1685,  1687,  1689,  1691,  1693,  1695,  1697,  1699,
    1700,  1704,  1708,  1712,  1713,  1714,  1716,  1719,  1720
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
  "SLURPY_POS", "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U",
  "YADA", "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "OP_FATARROW",
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

#define YYPACT_NINF (-792)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-428)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -792,     7,  1657,  -792,  -792,  -792,  -792,  -792,   786,   -38,
      -8,   -59,   -45,  -792,  -792,  4013,  -792,  -792,  -792,   138,
    1728,    37,  3019,  3090,  3161,  3232,  3303,    15,    10,  3303,
    1799,   433,  3303,   162,   165,  3374,  3445,    10,   147,    82,
     180,   239,   239,  3516,    10,    10,  3303,   219,   374,   297,
    4013,  -792,  -792,   393,   420,   477,  4013,  4013,  -792,  1870,
     383,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,
    -792,   539,  -792,  -792,  -792,   850,   585,  -792,   604,   501,
     195,   398,  -792,    38,   437,   205,   282,  -792,  -792,   388,
     450,   458,   581,  -792,  -792,  3303,  3303,  3303,  3303,  3303,
    3303,   608,  1942,  3587,  2014,   616,  3303,  3303,   199,  2085,
     155,   474,   111,  -792,    10,  -792,  -792,   495,   281,  -792,
    2157,  -792,   184,   175,   264,   294,   550,   621,   435,   640,
    1870,   494,  1870,    57,  1870,    10,  1870,    10,   -53,   166,
     303,  -792,  -792,   544,  -792,   548,   579,   600,   649,   144,
     172,  1870,    10,  1870,    10,    62,  2229,  -792,  3303,  3303,
    -792,  3303,  3303,  -792,  3303,  3303,  3303,  2300,  3303,  -792,
     729,  -792,  -792,  -792,   655,   572,    55,  -792,   683,  -792,
    -792,  -792,  -792,  -792,   208,   698,   708,  3303,  3303,  3303,
    3303,  3303,  3303,  3303,  3303,  -792,  3303,  3303,  3303,  3303,
    3303,  3303,  3303,  3303,  -792,  4013,  4013,  4013,  4013,  4013,
    4013,  4013,  4013,  4013,  4013,  4013,  4013,  4013,  4013,  4013,
     739,  4013,  4013,  4013,  4013,  4013,  4013,  4013,  4013,  4013,
    4013,  4013,  4013,  4013,  4013,  4013,   822,   313,   404,   718,
     720,   742,   746,   788,   805,   125,  -792,   515,  1870,   171,
     538,   149,   740,   764,   827,   777,   895,   820,   -58,  -792,
     610,   639,  2014,   943,  3303,  -792,  3303,   829,   208,   669,
    -792,  3303,  3303,  -792,  3658,  -792,  3729,  -792,   299,   305,
     317,   348,   858,   898,  -792,   709,   733,  3303,  3303,  3303,
    3303,  3303,  3303,  3303,  -792,  3303,   736,  3303,  3303,  -792,
    3303,   753,   865,   771,  -792,   949,  3303,  -792,  4013,  4013,
       2,  -792,  -792,  -792,  -792,  3303,  3303,  -792,   957,  3303,
     961,  3303,   774,   988,   778,  -792,  3303,  3303,  1045,   913,
     946,   947,   948,   951,   942,  -792,  -792,  -792,    10,     4,
    -792,    21,   952,   956,  -792,  -792,  2371,  -792,   884,   191,
     194,   966,  3019,  3090,  3303,  1393,    81,    94,  -792,   331,
     958,   959,   960,   962,   963,   964,   965,   967,   968,   970,
     971,   972,   973,   974,   975,   976,   979,   979,   979,   979,
     979,   979,   997,   979,   979,   979,   979,   979,   979,   979,
     979,  -792,  -792,  -792,   195,  -792,  -792,  -792,   977,   977,
     437,   205,   282,   282,  -792,  -792,  -792,  -792,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,
     978,  1087,   985,  -792,  2443,  -792,  -792,  -792,  -792,  -792,
    -792,  3303,  2515,  -792,   780,  3303,  -792,  3303,  3303,  3303,
    3303,   994,  1001,  1002,  -792,  2587,  -792,  2659,  3303,  -792,
    1115,  -792,  3800,   287,  1027,  1000,  -792,  -792,  1017,  -792,
    1019,  1870,   233,  1066,   139,  2731,   390,  1035,  3303,  -792,
    3871,  -792,  3303,  -792,    22,  -792,  3303,  3303,  1050,  1150,
    2371,  1040,  1041,  1044,  1049,  1051,  1052,  1053,   391,  2371,
    1054,  1055,   419,    10,    16,  3942,  -792,    10,    10,   -41,
     104,   143,  1059,   168,  -792,  1056,   787,  1102,  1057,  1058,
      25,  1067,  1060,  1070,  1061,    10,    10,    10,  1063,  1064,
    1074,  3303,  -792,  -792,  -792,  -792,  -792,  -792,  1170,  1174,
    -792,   -17,  1056,   789,    44,     5,     8,  -792,   791,  1175,
    2014,  3303,  3303,   353,   115,   -85,  -792,   -11,  3303,  3303,
    -792,  3303,  3303,  -792,  3303,  3303,  3303,  3303,  3303,  3303,
    3303,  3303,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  4013,
    2803,  -792,  2875,  1069,   793,  1071,  -792,   795,  2371,  1072,
     523,  1065,  1068,   479,  3303,  3303,  3303,  -792,   797,  -792,
     817,   821,  -792,  1122,   823,  -792,  -792,  -792,  -792,  -792,
     851,  3303,  -792,  3303,  3303,  3303,   862,  -792,    85,  -792,
    1077,  1870,   670,  1078,  1056,   864,  1079,  1080,  3303,  -792,
     866,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,   869,
    -792,  -792,  -792,   969,  -792,  -792,  1870,    10,  -792,  -792,
    1194,  -792,  1195,  1196,  3303,  -792,  1197,  1198,  -792,  1056,
     234,  3019,  3090,  1464,  -792,   355,  -792,  -792,  3303,    10,
    -792,  -792,  -792,  1086,  1088,  1183,  -792,  -792,  -792,  -792,
    3303,  1090,  -792,  -792,    18,   623,   835,  1158,  -792,  -792,
    1056,  1056,   874,   876,  1201,  1202,  1203,  -792,  -792,   220,
     551,  1091,   235,  3303,  3303,  3303,  3303,  3303,  3303,  -792,
    -792,  -792,  -792,   246,   274,   325,   339,   341,   350,   358,
     359,   369,   372,   392,   418,  -792,  -792,   899,  -792,   901,
     903,  -792,  1095,  -792,  -792,   905,  -792,  -792,  -792,  -792,
    -792,  1097,  1099,  1100,  -792,  -792,  -792,  3303,  -792,  2371,
    1101,   687,  -792,  -792,  1104,  1153,  1154,  -792,   907,  -792,
    3303,  -792,  -792,  1056,  -792,  -792,   701,  1107,  1110,    16,
    -792,   909,  1018,    10,    10,    10,  -792,  -792,  -792,  -792,
    1125,   198,  -792,   380,   124,  -792,   421,  3303,  3303,  3303,
    3303,  3303,  3303,  3303,  3303,  -792,    10,  1105,  -792,  -792,
      10,  1113,  3303,  -792,   362,   447,    70,   109,   516,  1114,
    1116,   822,  -792,  -792,  1056,  1056,  1231,  1233,  1234,  3303,
    2947,  1133,  1134,  -792,   440,   443,   451,   467,   480,   493,
    -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  1121,  -792,  -792,  -792,
    -792,   922,  -792,  -792,  -792,  3303,  3303,  2371,   716,  -792,
    -792,  3303,  -792,  -792,  -792,  -792,    10,    10,  -792,  -792,
    -792,  -792,  3303,  -792,  1235,  1236,  3303,  3303,  3303,  -792,
    -792,  -792,   504,   508,   509,   513,   528,   540,   545,   546,
    -792,  -792,  -792,  3303,  1123,  -792,    45,  1177,  -792,    46,
    -792,  1237,  1238,  1126,  3303,  -792,  1239,  1240,  1127,  3303,
    -792,   159,   193,  1132,  1135,  -792,  -792,   475,  -792,  -792,
    -792,  -792,  -792,   557,  1129,   924,  3303,  3303,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  1137,  -792,  -792,   926,  -792,
     734,  1026,  -792,  -792,  -792,  -792,   559,   567,   568,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  -792,  1138,    10,  1128,
     934,  1139,  1271,  1128,   936,  1140,  1141,  -792,  1142,  1144,
    1145,  -792,  1147,  1242,  1257,  1149,  3303,  -792,  1259,  1264,
    1156,  3303,  -792,  -792,  -792,    51,  -792,  -792,  -792,  1168,
     569,   577,  -792,  1157,  -792,    10,  -792,  -792,  -792,  -792,
    3303,  -792,  -792,  1128,  -792,  3019,  3090,  1535,  -792,   468,
    -792,  1128,  -792,  -792,  -792,  -792,  -792,  -792,  1185,  1186,
    -792,  1187,  1188,  1204,  -792,  1205,  1128,   938,  -792,  -792,
    -792,  -792,  -792,  1207,  -792,   395,   213,  -792,   587,  3303,
    3303,  3303,  3303,  3303,  3303,  3303,  3303,  -792,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  1128,    10,  3303,  3303,
    3303,  -792,  -792,  -792,   643,   647,   651,   653,   676,   684,
     685,   686,  -792,  -792,   699,   700,   703,  -792,  -792,  -792,
    -792,  -792,  -792,  -792,  -792,  -792,  -792,  -792
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   395,   396,   397,   399,   400,   405,
     406,   420,   419,   421,   422,     0,   407,   408,   409,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   414,   398,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   300,   302,   320,
     322,   325,   327,   330,   332,   334,   337,   342,   347,   349,
     363,   391,     0,   403,   404,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,   405,   406,   420,     0,   427,   345,     0,   400,    35,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   330,
       0,   428,   299,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   139,     0,     0,
     104,     0,     0,   105,     0,     0,     0,     3,     0,   389,
      96,    98,   386,   203,     0,     0,     0,   346,     0,   401,
     402,   344,   343,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   298,     0,   369,     0,     0,     0,
       0,     0,     0,     0,     0,   374,     0,   373,   419,   365,
       0,     0,     0,     0,     0,    30,     0,   423,     0,     0,
      38,   394,     0,     6,     0,     9,     0,    10,     0,     0,
       0,     0,     0,     0,   350,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    42,
       0,     0,   117,     0,   129,     0,     0,   148,     0,     0,
       0,     3,   155,    45,    46,     0,     0,   151,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   387,   388,   390,     0,     0,
     206,     0,     0,     0,   241,   424,     0,   264,   400,   405,
     406,   420,     0,     0,     0,     0,     0,     0,   263,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   305,   306,   309,   310,
     303,   304,     0,   307,   308,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   321,   323,   324,   326,   328,   329,
     331,   333,   335,   336,   341,   338,   339,   340,   348,   352,
     362,   356,   357,   358,   359,   360,   361,   354,   355,   353,
     385,     0,   382,    51,     0,    85,    86,    87,    88,    89,
      48,     0,     0,   368,     0,     0,    15,     0,     0,     0,
       0,   410,   415,   416,    53,     0,   377,     0,     0,   366,
       0,   364,   394,     0,     0,     0,   298,    37,     0,   393,
       0,     0,     0,   397,   419,     0,     0,     0,     0,    27,
       0,    28,     0,    29,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,     0,   120,   424,     0,     0,
     328,   329,   251,     0,   261,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   424,     0,   424,     0,     0,
       0,     0,   106,   107,   108,   109,   297,    97,     0,     0,
     206,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
     291,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,     0,   265,    55,    57,    59,    62,    60,    61,    56,
      58,    68,    70,    72,    75,    73,    74,    69,    71,     0,
       0,   378,     0,     0,     0,     0,   165,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,   372,
       0,     0,   245,     0,     0,   410,   415,   416,    36,     5,
       0,     0,    11,     0,     0,     0,     0,    20,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
       0,    76,    79,    78,    82,    80,    81,    77,    40,     0,
      83,    84,    43,   114,   119,   118,     0,     0,   128,   145,
       0,   147,     0,     0,     0,   252,     0,     0,   154,     0,
       0,     0,     0,     0,   166,     0,    32,    31,     0,     0,
     149,   417,   418,     0,     0,   130,   133,   134,   137,   138,
       0,     0,   204,   205,     0,     0,     0,     0,   202,   163,
       0,     0,     0,     0,     0,     0,     0,   240,   426,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   274,
     281,   288,   289,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   301,   384,     0,   381,     0,
       0,    50,     0,    52,   164,     0,    17,    16,   413,   412,
     411,     0,     0,     0,   375,   370,   371,     0,   367,     0,
       0,     0,   248,   247,     0,     0,     0,    22,     0,    23,
       0,    26,   157,     0,    34,    33,     0,   426,   426,     0,
     121,     0,   122,     0,     0,     0,   259,   253,   254,   153,
     255,     0,   262,     0,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   167,     0,     0,    66,    67,
       0,     0,     0,   201,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   160,     0,     0,     0,     0,     0,     0,
       0,   410,   416,   285,     0,     0,     0,     0,     0,     0,
     293,   294,   295,   296,   266,   268,   270,   273,   271,   272,
     267,   269,   383,   380,   379,    49,   426,    63,    64,    65,
     246,     0,    13,    12,    21,     0,     0,     0,     0,   156,
       7,     0,    41,    44,   116,   115,   424,     0,   126,   146,
     143,   144,     0,   256,     0,     0,     0,     0,     0,   170,
     171,   168,     0,     0,     0,     0,     0,     0,     0,     0,
     152,   150,   131,     0,     0,   142,     0,     3,   236,     0,
     233,     0,     0,     0,     0,   212,     0,     0,     0,     0,
     207,     0,     0,     0,     0,   208,   209,     0,   159,   161,
     242,   243,   244,     0,   165,     0,     0,     0,   275,   277,
     280,   278,   279,   276,    18,   426,   250,   249,     0,    24,
       0,   123,   124,   260,   257,   258,     0,     0,     0,   172,
     174,   176,   179,   177,   178,   173,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,   224,     0,     0,     0,     0,     0,   214,     0,     0,
       0,     0,   213,   210,   211,     0,   239,   284,   283,   164,
       0,     0,    14,   426,     8,     0,   127,   180,   182,   181,
       0,   141,   235,     0,   184,     0,     0,     0,   183,     0,
     232,     0,   221,   222,   216,   219,   220,   215,     0,     0,
     225,     0,     0,     0,   226,     0,     0,     0,   282,   286,
     287,    25,   125,     0,   234,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,   231,   229,
     230,   218,   227,   228,   217,   238,     0,     0,     0,     0,
       0,   188,   189,   186,     0,     0,     0,     0,     0,     0,
       0,     0,   237,   140,     0,     0,     0,   190,   192,   194,
     197,   195,   196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -792,  -792,     1,  -792,  -489,  -637,  -792,  -792,  -792,  -792,
    -792,   329,  -792,  -792,  -792,  -792,  -792,  -279,  -791,  -792,
    -792,  -792,   803,  -792,  -792,  -439,   871,  -332,   304,  1023,
      -2,  -207,  -792,   804,  1118,  -792,   644,   -16,  1117,  1109,
     838,    -4,  -792,  -792,  -234,  -792,  -792,    20,  -792
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   186,    61,    62,   496,    63,    64,    65,    66,
      67,   894,    68,    69,   510,    70,    71,   312,   898,    72,
      73,   339,   531,    74,   536,   260,   466,   506,   115,   165,
     121,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   285,   420,    89,    90,   122,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,     2,   475,   422,   382,   644,   770,     3,   600,   533,
     139,   116,   502,   604,   900,   502,   503,   448,   528,   503,
     131,   133,   135,   137,   138,   305,   529,   143,   145,   140,
     148,   502,   502,   152,   154,   503,   503,   650,   313,   685,
     686,   169,    24,   687,   172,   711,   177,   123,   124,   125,
     107,   126,   181,   182,   502,   502,   502,   184,   503,   503,
     503,   502,   668,   669,   108,   503,   109,   504,   127,   342,
     504,   103,   128,   306,   685,   686,    60,   108,   687,   109,
     694,   695,   696,   297,   901,   271,   504,   504,    60,   104,
     755,   326,   902,   239,   240,   241,   242,   243,   244,   756,
     298,   249,   251,   105,   253,   254,   327,   548,   158,   504,
     504,   504,   314,   688,   225,   226,   504,   343,   268,   712,
     551,   106,   247,   906,   549,   159,   986,   505,   286,   261,
     296,   907,   301,   530,   303,   868,   903,   552,   697,    60,
     269,   297,   625,   729,    60,    60,   532,   624,   803,   322,
     297,   324,   117,   227,   329,   670,   330,   331,   298,   332,
     333,   129,   334,   335,   336,   334,   337,   298,  1002,   691,
     959,   963,  1010,   973,   149,   908,  1026,   150,   655,   904,
     299,   974,   652,   300,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   905,   368,   369,   370,   371,   372,   373,
     374,   375,   692,   693,   160,   160,   161,   978,   873,   398,
     399,   550,  1034,   255,   615,   979,   256,   163,   909,   227,
    1048,   653,   263,   162,   553,   975,   658,   404,   405,   406,
     407,   408,   910,   173,   431,  1055,   656,   657,   299,   297,
     264,   300,   308,   309,   780,   710,   434,   299,   781,   432,
     300,    93,    94,   689,   880,   318,   298,   257,   227,   980,
     453,   108,   455,   109,   456,  1072,   874,   875,   976,   459,
     460,   156,   462,   319,   467,   440,    60,   441,   435,   102,
     864,   227,   977,   320,   272,   481,   482,   483,   484,   485,
     486,   487,   500,   501,   436,   490,   491,   437,   273,   782,
     103,   321,   981,   163,   996,   105,    75,   270,   222,   223,
     271,   175,   507,   508,   509,   488,   982,   512,   540,   514,
     492,   229,   230,   541,   518,   519,   499,   409,   176,   819,
     421,   141,   142,   345,   346,   410,   299,   411,   412,   300,
     611,   155,   157,  1062,   820,   762,   413,   414,   170,   171,
     543,   544,   545,   547,   415,   416,   612,   554,   444,   613,
     555,   556,   417,   418,   419,   823,   538,   557,   164,   522,
     558,   559,   725,   274,   560,   561,   830,    93,    94,   703,
     779,   787,   288,   704,   788,   789,   231,   275,   174,   705,
     266,   790,   706,   707,   791,   792,   708,   523,   793,   794,
     232,   233,   234,   276,   831,   102,   876,   178,   468,   288,
     877,   812,   813,   440,   470,   605,   290,   277,   141,   291,
     292,  1058,   469,   878,   288,  1059,   472,   310,   471,   585,
     179,   290,   311,   589,   291,   292,   591,   592,  1060,   302,
     473,   304,   307,   146,   584,   282,   602,   147,   524,   283,
     459,   805,   587,   185,   204,   832,   323,   590,   325,   610,
     593,   562,   525,   184,   571,   598,   620,   601,   622,   833,
     623,   834,   474,   572,   626,   627,   294,   311,   204,   295,
     835,   573,   574,   709,   859,   795,   896,   180,   836,   837,
      75,   897,   575,   647,  1039,   576,   235,  1040,  1041,   838,
     630,   224,   839,   294,  1042,   665,   295,  1043,  1044,   639,
     879,  1045,  1046,   617,   638,   577,   618,   271,   294,   681,
     287,   295,   840,   288,   289,  1061,   911,   423,   424,   912,
     290,   913,   914,   291,   292,   918,   919,   293,   700,   701,
     702,   578,   642,   228,   314,   271,   713,   714,   841,   715,
     716,   881,   717,   718,   719,   720,   721,   722,   723,   724,
     278,   279,   280,   631,   960,   187,   633,   964,   188,   189,
     928,   899,   236,   929,   634,   190,   897,   917,   191,   192,
     237,   930,   193,   194,   214,   215,   216,   217,   218,   219,
     635,   204,   741,   742,   743,   238,   220,   931,  1047,   985,
     727,   262,   730,   636,   897,   271,   221,   740,   735,   750,
     932,   196,   752,   753,   197,   198,   637,   294,   265,   758,
     295,   199,   245,   933,   200,   201,   766,   571,   202,   203,
     252,   572,   573,   751,   949,   281,   574,   409,   950,   951,
     433,   271,   527,   952,   771,   410,   737,   411,   412,   271,
     284,   575,   776,  1027,   438,   439,   413,   414,   953,   783,
     784,   786,   195,   576,   415,   416,   796,   313,   577,   578,
     954,   314,   417,   418,   419,   955,   956,   440,   801,   821,
     733,   804,   631,   205,   206,   207,   208,   987,   315,   997,
     633,   637,   847,   209,   210,   211,   341,   998,   999,  1029,
     849,   824,   825,   826,   827,   828,   829,  1030,   204,   316,
     314,     4,     5,     6,     7,   212,   213,  1063,   348,   349,
     350,   351,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   352,   353,    24,   449,   450,    25,    26,    27,
      28,   354,   355,    31,    32,   850,   391,   392,   393,    33,
      34,    35,    36,    37,    38,   356,   357,    41,    42,    43,
      44,    45,    46,    47,   451,   452,   571,    48,    49,   851,
     572,    50,    51,  1077,   573,    52,   574,  1078,   317,    53,
     858,  1079,   338,  1080,   340,   882,   883,   884,   885,   886,
     887,   888,   889,   759,   458,   271,   760,   643,   645,   575,
     895,   648,   649,   651,    54,    55,  1081,   576,   577,   578,
     853,    75,   344,   271,  1082,  1083,  1084,   923,    56,   675,
     676,   677,   631,   633,   860,    57,   637,   861,   347,  1085,
    1086,    58,    59,  1087,   478,   479,   409,    60,   358,   939,
     925,   425,   271,   426,   410,   806,   411,   412,   807,   808,
     809,   810,   442,   936,   937,   413,   414,   994,   345,   480,
     271,   345,   489,   415,   416,   427,   395,   396,   397,   428,
     943,   417,   418,   419,   946,   947,   948,   938,   493,   346,
      92,   940,    93,    94,    95,    96,    97,    98,    99,  -427,
    -427,   957,   494,   495,   443,   100,   497,   346,   962,   515,
     346,   445,   968,   517,   346,   345,   588,   972,   101,   446,
     102,   429,   659,   660,   690,   660,   698,   271,   732,   271,
     734,   271,   744,   271,   990,   991,  -427,  -427,   430,  -427,
    -427,  -427,  -427,  -427,  -427,  -427,  -427,  -427,  -427,  -427,
    -427,  -427,   745,   450,   447,  -427,   746,   271,   748,   450,
     444,   772,   457,  -427,  -427,  -427,  -427,   454,  -427,   498,
    1009,  -427,  -427,  -427,  -427,  -427,  -427,   476,  -427,  -427,
    -427,   511,  -427,   797,  1021,   513,   345,   749,    92,  1025,
      93,    94,    95,    96,    97,    98,    99,   754,   618,   763,
     660,   767,   271,   100,   768,   271,   769,   495,   895,   814,
     660,   815,   660,  1035,  1036,  1038,   539,   477,   102,   376,
     377,   378,   379,   380,   381,   516,   383,   384,   385,   386,
     387,   388,   389,   390,   842,   271,   843,   450,   844,   271,
     846,   271,   345,   857,   866,   346,   521,  1064,  1065,  1066,
    1067,  1068,  1069,  1070,  1071,   867,   495,   935,   271,   989,
     271,   993,   271,   995,   495,   520,  1074,  1075,  1076,  1003,
     660,  1011,   660,  1056,   660,   166,   168,   402,   403,   522,
     523,   524,   526,   865,   525,   542,   534,   869,   870,   871,
     535,   563,   564,   565,   221,   566,   567,   568,   569,   579,
     570,   571,   227,   572,   573,   574,   575,   576,   577,   578,
     890,   581,   580,   594,   892,     4,     5,     6,     7,   582,
     595,   596,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   661,   662,    24,   603,
     607,    25,    26,    27,    28,    29,   663,    31,    32,   606,
     608,   614,   609,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,   619,   628,
     629,    48,    49,   631,   632,    50,    51,   633,   654,    52,
     941,   942,   634,    53,   635,   636,   637,   640,   641,   671,
     666,   667,   673,   680,   682,   311,   678,   679,   683,   699,
     672,   674,   731,   738,   733,   736,   739,   747,    54,    55,
     757,   761,   764,   765,   773,   774,   775,   777,   778,   798,
     800,   799,    56,   802,   811,   816,   817,   818,   845,    57,
     847,   822,   848,   849,   852,    58,    59,   854,   855,   856,
     862,    60,   664,   863,   872,   891,   893,   915,   920,   916,
     921,   922,   926,   927,   934,   944,   945,   961,   958,   967,
     971,   965,   966,   969,   970,   983,  1018,   897,   984,   988,
     992,  1000,  1001,  1012,  1013,  1014,    75,  1015,  1016,  1004,
    1017,  1019,  1020,  1022,     4,     5,     6,     7,  1023,  1024,
    1031,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,  1005,  1006,    24,  1028,  1032,
      25,    26,    27,    28,    29,  1007,    31,    32,  1049,  1050,
    1051,  1052,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,  1053,  1054,  1033,
      48,    49,  1057,   684,    50,    51,   616,   401,    52,   394,
       0,     0,    53,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1073,     0,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    56,     0,     0,     0,     0,     0,     0,    57,     0,
       0,     0,     0,     0,    58,    59,     4,     5,     6,     7,
      60,  1008,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,     0,     0,    33,     0,     0,     0,     0,     0,
       0,     0,    41,    42,    43,     0,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,    50,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,   144,    59,    46,     0,
       0,     0,    60,   546,     0,     0,     0,    50,    51,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,   144,    59,    46,
       0,     0,     0,    60,   785,     0,     0,     0,    50,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    56,     0,     0,     0,     0,
       0,     0,    57,     0,     0,     0,     0,     0,   144,    59,
       4,     5,     6,     7,    60,  1037,     0,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     0,     0,    25,    26,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,     0,     0,     0,    48,    49,     0,     0,
      50,    51,     0,     0,    52,     0,     0,     0,    53,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
      58,    59,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,   119,   120,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,   144,    59,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   183,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   246,    46,     0,
       0,    60,     0,     0,     0,     0,     0,    50,    51,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,   258,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,   250,     0,    41,    42,    43,     0,    59,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     259,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,   328,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   267,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,    59,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,    52,     0,     0,
     185,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   537,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   583,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     586,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,   258,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   597,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     4,     5,   463,     7,     0,     0,
       0,   118,   111,   112,   113,   464,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   599,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   183,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,   258,
      13,    14,    15,    16,    17,    18,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,    56,     0,     0,    33,     0,     0,     0,
      57,     0,     0,     0,    41,    42,    43,    59,   726,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     728,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   924,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   130,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,   132,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   134,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   136,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,     0,    56,     0,    33,     0,     0,     0,     0,
      57,     0,     0,    41,    42,    43,     0,    59,    46,     0,
       0,     0,    60,     0,     0,     0,     0,    50,    51,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     0,     0,     0,   118,   111,   112,   113,    12,
      13,    14,    15,    16,    17,    18,     0,     0,     0,     0,
      54,    55,     0,     0,     0,     0,   114,    28,     0,     0,
       0,     0,     0,     0,    56,     0,    33,     0,     0,     0,
       0,    57,     0,     0,    41,    42,    43,     0,   151,    46,
       0,     0,     0,    60,     0,     0,     0,     0,    50,    51,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
       0,    54,    55,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,     0,    56,     0,    33,     0,     0,
       0,     0,    57,     0,     0,    41,    42,    43,     0,   153,
      46,     0,     0,     0,    60,     0,     0,     0,     0,    50,
      51,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,     0,    56,     0,    33,     0,
       0,     0,     0,    57,     0,     0,    41,    42,    43,     0,
      59,    46,     0,     0,     0,   167,     0,     0,     0,     0,
      50,    51,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,    12,    13,    14,    15,    16,    17,    18,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,     0,    56,     0,    33,
       0,     0,     0,     0,    57,     0,     0,    41,    42,    43,
       0,   248,    46,     0,     0,     0,    60,     0,     0,     0,
       0,    50,    51,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,   463,     7,     0,     0,     0,   118,
     111,   112,   113,   464,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,     0,   461,    46,     0,     0,     0,    60,     0,     0,
       0,     0,    50,    51,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,   258,    13,    14,    15,    16,    17,
      18,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,     0,    56,
       0,    33,     0,     0,     0,     0,    57,     0,     0,    41,
      42,    43,     0,   465,    46,     0,     0,     0,    60,     0,
       0,     0,     0,    50,    51,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,    12,    13,    14,    15,    16,
      17,    18,     0,     0,     0,     0,    54,    55,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,     0,
      56,     0,    33,     0,     0,     0,     0,    57,     0,     0,
      41,    42,    43,     0,    59,    46,     0,     0,     0,    60,
       0,     0,     0,     0,    50,    51,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,     0,    54,    55,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
       0,    56,     0,    33,     0,     0,     0,     0,    57,     0,
       0,    41,    42,    43,     0,   621,    46,     0,     0,     0,
      60,     0,     0,     0,     0,    50,    51,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   110,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   646,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    54,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
      57,     0,     0,     0,     0,     0,     0,    59,     0,     0,
       0,     0,    60
};

static const yytype_int16 yycheck[] =
{
       2,     0,   281,   237,   211,   494,   643,     0,   447,   341,
      26,    15,    10,   452,   805,    10,    14,    75,    14,    14,
      22,    23,    24,    25,    26,    78,    22,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    78,   123,    56,
      57,    43,    26,    60,    46,   130,    50,    10,    11,    12,
     109,    14,    56,    57,    10,    10,    10,    59,    14,    14,
      14,    10,    37,    38,   122,    14,   124,    65,    31,    14,
      65,   109,    35,   126,    56,    57,   129,   122,    60,   124,
      72,    73,    74,    26,    14,   126,    65,    65,   129,   127,
       5,    29,    22,    95,    96,    97,    98,    99,   100,    14,
      43,   103,   104,   111,   106,   107,    44,    26,    26,    65,
      65,    65,   123,   130,    76,    77,    65,    62,   120,   130,
      26,   129,   102,    14,    43,    43,   917,   125,   130,   109,
     132,    22,   134,   129,   136,   772,    66,    43,   130,   129,
     120,    26,   474,   582,   129,   129,   125,   125,   130,   151,
      26,   153,    14,   115,   156,   130,   158,   159,    43,   161,
     162,   124,   164,   165,   166,   167,   168,    43,   959,   125,
     125,   125,   963,    14,    12,    66,   125,    12,    10,   109,
     123,    22,    78,   126,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   123,   196,   197,   198,   199,   200,   201,
     202,   203,   534,   535,   123,   123,    26,    14,    10,   225,
     226,   130,  1003,    14,    75,    22,    17,   123,   109,   115,
    1011,    78,   111,    43,   130,    66,   505,   231,   232,   233,
     234,   235,   123,    14,   109,  1026,    68,    69,   123,    26,
     129,   126,    76,    77,    10,   130,   248,   123,    14,   124,
     126,    96,    97,   532,   130,   111,    43,    58,   115,    66,
     262,   122,   264,   124,   266,  1056,    68,    69,   109,   271,
     272,   124,   274,   129,   276,   126,   129,   128,   107,   124,
     769,   115,   123,   111,   109,   287,   288,   289,   290,   291,
     292,   293,   308,   309,   123,   297,   298,   126,   123,    65,
     109,   129,   109,   123,   941,   111,     2,   123,   113,   114,
     126,    14,   311,   315,   316,   295,   123,   319,   127,   321,
     300,   116,   117,   129,   326,   327,   306,    14,    31,   109,
      17,    27,    28,   125,   126,    22,   123,    24,    25,   126,
     107,    37,    38,   130,   124,   624,    33,    34,    44,    45,
     352,   353,   354,   355,    41,    42,   123,    26,   123,   126,
      29,    30,    49,    50,    51,   130,   346,    36,   129,   123,
      39,    40,   579,   109,    43,    44,   130,    96,    97,    26,
     659,    26,    29,    30,    29,    30,   104,   123,    14,    36,
     109,    36,    39,    40,    39,    40,    43,   123,    43,    44,
     118,   119,   120,   109,   130,   124,    26,    14,   109,    29,
      30,   690,   691,   126,   109,   128,    36,   123,   114,    39,
      40,    26,   123,    43,    29,    30,   109,   124,   123,   431,
      10,    36,   129,   435,    39,    40,   438,   439,    43,   135,
     123,   137,   138,    10,   424,    10,   448,    14,   123,    14,
     452,   685,   432,    70,   123,   130,   152,   437,   154,   461,
     440,   130,   123,   465,   123,   445,   468,   447,   470,   130,
     472,   130,   124,   123,   476,   477,   123,   129,   123,   126,
     130,   123,   123,   130,   763,   130,   124,    10,   130,   130,
     186,   129,   123,   495,    26,   123,   108,    29,    30,   130,
     480,   103,   130,   123,    36,   507,   126,    39,    40,   489,
     130,    43,    44,   123,   123,   123,   126,   126,   123,   521,
      26,   126,   130,    29,    30,   130,    10,   123,   124,    13,
      36,    15,    16,    39,    40,   814,   815,    43,   540,   541,
     542,   123,   123,   106,   123,   126,   548,   549,   130,   551,
     552,   130,   554,   555,   556,   557,   558,   559,   560,   561,
      10,    11,    12,   123,   896,    26,   123,   899,    29,    30,
     130,   124,   122,   130,   123,    36,   129,   811,    39,    40,
     122,   130,    43,    44,    83,    84,    85,    86,    87,    88,
     123,   123,   594,   595,   596,    14,    95,   130,   130,   124,
     580,   127,   582,   123,   129,   126,   105,   128,   588,   611,
     130,    26,   614,   615,    29,    30,   123,   123,   123,   621,
     126,    36,    14,   130,    39,    40,   628,   123,    43,    44,
      14,   123,   123,   613,   130,    14,   123,    14,   130,   130,
     125,   126,   338,   130,   646,    22,   123,    24,    25,   126,
      10,   123,   654,   985,   116,   117,    33,    34,   130,   661,
     662,   663,   123,   123,    41,    42,   668,   123,   123,   123,
     130,   123,    49,    50,    51,   130,   130,   126,   680,   128,
     123,    58,   123,    79,    80,    81,    82,   130,   109,   130,
     123,   123,   123,    89,    90,    91,   124,   130,   130,   130,
     123,   703,   704,   705,   706,   707,   708,   130,   123,   109,
     123,     3,     4,     5,     6,   111,   112,   130,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   125,   126,    29,    30,    31,
      32,    33,    34,    35,    36,   747,     7,     8,     9,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,   125,   126,   123,    59,    60,   749,
     123,    63,    64,   130,   123,    67,   123,   130,   129,    71,
     760,   130,    53,   130,   129,   787,   788,   789,   790,   791,
     792,   793,   794,   123,   125,   126,   126,   493,   494,   123,
     802,   497,   498,   499,    96,    97,   130,   123,   123,   123,
     123,   507,   129,   126,   130,   130,   130,   819,   110,   515,
     516,   517,   123,   123,   123,   117,   123,   126,   130,   130,
     130,   123,   124,   130,   125,   126,    14,   129,   130,   123,
     820,   123,   126,   123,    22,    10,    24,    25,    13,    14,
      15,    16,   112,   855,   856,    33,    34,   123,   125,   126,
     126,   125,   126,    41,    42,   123,   222,   223,   224,   123,
     872,    49,    50,    51,   876,   877,   878,   857,   125,   126,
      94,   861,    96,    97,    98,    99,   100,   101,   102,    39,
      40,   893,    27,    28,   130,   109,   125,   126,   897,   125,
     126,   124,   904,   125,   126,   125,   126,   909,   122,    14,
     124,   123,   125,   126,   125,   126,   125,   126,   125,   126,
     125,   126,   125,   126,   926,   927,    76,    77,   123,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,   125,   126,   124,    95,   125,   126,   125,   126,
     123,   647,   123,   103,   104,   105,   106,    14,   108,    10,
     962,   111,   112,   113,   114,   115,   116,   109,   118,   119,
     120,    14,   122,   669,   976,    14,   125,   126,    94,   981,
      96,    97,    98,    99,   100,   101,   102,   125,   126,   125,
     126,   125,   126,   109,   125,   126,    27,    28,  1000,   125,
     126,   125,   126,  1005,  1006,  1007,   122,   109,   124,   205,
     206,   207,   208,   209,   210,    27,   212,   213,   214,   215,
     216,   217,   218,   219,   125,   126,   125,   126,   125,   126,
     125,   126,   125,   126,   125,   126,   123,  1039,  1040,  1041,
    1042,  1043,  1044,  1045,  1046,    27,    28,   125,   126,   125,
     126,   125,   126,    27,    28,    10,  1058,  1059,  1060,   125,
     126,   125,   126,   125,   126,    42,    43,   229,   230,   123,
     123,   123,   130,   769,   123,   109,   124,   773,   774,   775,
     124,   123,   123,   123,   105,   123,   123,   123,   123,    92,
     123,   123,   115,   123,   123,   123,   123,   123,   123,   123,
     796,    14,   124,   109,   800,     3,     4,     5,     6,   124,
     109,   109,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    14,
     130,    29,    30,    31,    32,    33,    34,    35,    36,   112,
     123,    75,   123,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,   123,   109,
      10,    59,    60,   123,   123,    63,    64,   123,   109,    67,
     866,   867,   123,    71,   123,   123,   123,   123,   123,   112,
     123,   123,   112,   109,    14,   129,   123,   123,    14,    14,
     130,   130,   123,   128,   123,   123,   128,    75,    96,    97,
     123,   123,   123,   123,    10,    10,    10,    10,    10,   123,
      27,   123,   110,   123,    56,    14,    14,    14,   123,   117,
     123,   130,   123,   123,   123,   123,   124,   123,    75,    75,
     123,   129,   130,   123,   109,   130,   123,   123,     7,   123,
       7,     7,   109,   109,   123,    10,    10,    70,   125,   123,
     123,    14,    14,    14,    14,   123,    14,   129,   123,   130,
     123,   123,   958,   123,   123,   123,   962,   123,   123,   130,
     123,    14,   123,    14,     3,     4,     5,     6,    14,   123,
     123,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   130,   995,
      29,    30,    31,    32,    33,    34,    35,    36,   123,   123,
     123,   123,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,   123,   123,  1000,
      59,    60,   125,   530,    63,    64,   465,   228,    67,   221,
      -1,    -1,    71,    -1,   227,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1057,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,   124,     3,     4,     5,     6,
     129,   130,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    41,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    49,    50,    51,   123,   124,    54,    -1,
      -1,    -1,   129,   130,    -1,    -1,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    41,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    49,    50,    51,   123,   124,    54,
      -1,    -1,    -1,   129,   130,    -1,    -1,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,
       3,     4,     5,     6,   129,   130,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    -1,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    71,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    41,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    49,    50,    51,
     123,   124,    54,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,    50,
      51,   123,   124,    54,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,
      50,    51,   123,   124,    54,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    41,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    49,    50,    51,   124,   125,    54,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    41,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    49,    50,    51,   124,   125,    54,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    41,    -1,    -1,    -1,
      -1,   117,   118,    -1,    49,    50,    51,    -1,   124,    54,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    41,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    49,    50,    51,   124,
     125,    54,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    23,    96,    97,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      41,    -1,    -1,    -1,   117,    -1,    -1,    -1,    49,    50,
      51,   124,   125,    54,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,
      50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      70,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      49,    50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    41,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    49,    50,    51,   124,   125,    54,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    41,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    49,    50,    51,   124,   125,    54,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    41,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    49,    50,    51,   124,
     125,    54,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      41,    -1,    -1,    -1,   117,    -1,    -1,    -1,    49,    50,
      51,   124,   125,    54,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    41,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      49,    50,    51,   124,   125,    54,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    41,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    49,    50,    51,   124,   125,    54,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    41,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    49,    50,    51,   124,   125,    54,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    41,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    49,    50,    51,   124,
     125,    54,    -1,    -1,   129,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      41,    -1,    -1,    -1,   117,    -1,    -1,    -1,    49,    50,
      51,   124,   125,    54,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,
      50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      49,    50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    41,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    49,    50,    51,    -1,   124,    54,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    41,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    49,    50,    51,    -1,   124,    54,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    41,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    49,    50,    51,    -1,   124,    54,    -1,
      -1,    -1,   129,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      96,    97,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    41,    -1,    -1,    -1,
      -1,   117,    -1,    -1,    49,    50,    51,    -1,   124,    54,
      -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    96,    97,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    41,    -1,    -1,
      -1,    -1,   117,    -1,    -1,    49,    50,    51,    -1,   124,
      54,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    41,    -1,
      -1,    -1,    -1,   117,    -1,    -1,    49,    50,    51,    -1,
     124,    54,    -1,    -1,    -1,   129,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    41,
      -1,    -1,    -1,    -1,   117,    -1,    -1,    49,    50,    51,
      -1,   124,    54,    -1,    -1,    -1,   129,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,    50,
      51,    -1,   124,    54,    -1,    -1,    -1,   129,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,    49,
      50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    96,    97,    -1,    -1,
      -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    41,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      49,    50,    51,    -1,   124,    54,    -1,    -1,    -1,   129,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    96,    97,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    41,    -1,    -1,    -1,    -1,   117,    -1,
      -1,    49,    50,    51,    -1,   124,    54,    -1,    -1,    -1,
     129,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    67,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    96,    97,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    41,    -1,    -1,    -1,    -1,   117,
      -1,    -1,    49,    50,    51,    -1,   124,    54,    -1,    -1,
      -1,   129,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,   129
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   132,   133,     0,     3,     4,     5,     6,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    29,    30,    31,    32,    33,
      34,    35,    36,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    59,    60,
      63,    64,    67,    71,    96,    97,   110,   117,   123,   124,
     129,   134,   135,   137,   138,   139,   140,   141,   143,   144,
     146,   147,   150,   151,   154,   159,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   176,
     177,   179,    94,    96,    97,    98,    99,   100,   101,   102,
     109,   122,   124,   109,   127,   111,   129,   109,   122,   124,
      10,    11,    12,    13,    31,   159,   172,    14,    10,   123,
     124,   161,   178,    10,    11,    12,    14,    31,    35,   124,
     124,   161,   124,   161,   124,   161,   124,   161,   161,   168,
      14,   159,   159,   161,   123,   161,    10,    14,   161,    12,
      12,   124,   161,   124,   161,   159,   124,   159,    26,    43,
     123,    26,    43,   123,   129,   160,   160,   129,   160,   161,
     159,   159,   161,    14,    14,    14,    31,   172,    14,    10,
      10,   172,   172,   125,   161,    70,   133,    26,    29,    30,
      36,    39,    40,    43,    44,   123,    26,    29,    30,    36,
      39,    40,    43,    44,   123,    79,    80,    81,    82,    89,
      90,    91,   111,   112,    83,    84,    85,    86,    87,    88,
      95,   105,   113,   114,   103,    76,    77,   115,   106,   116,
     117,   104,   118,   119,   120,   108,   122,   122,    14,   161,
     161,   161,   161,   161,   161,    14,   125,   178,   124,   161,
     118,   161,    14,   161,   161,    14,    17,    58,    14,   125,
     156,   178,   127,   111,   129,   123,   109,   125,   161,   178,
     123,   126,   109,   123,   109,   123,   109,   123,    10,    11,
      12,    14,    10,    14,    10,   174,   161,    26,    29,    30,
      36,    39,    40,    43,   123,   126,   161,    26,    43,   123,
     126,   161,   159,   161,   159,    78,   126,   159,    76,    77,
     124,   129,   148,   123,   123,   109,   109,   129,   111,   129,
     111,   129,   161,   159,   161,   159,    29,    44,    23,   161,
     161,   161,   161,   161,   161,   161,   161,   161,    53,   152,
     129,   124,    14,    62,   129,   125,   126,   130,    10,    11,
      12,    13,    24,    25,    33,    34,    47,    48,   130,   161,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   164,   164,   164,   164,
     164,   164,   162,   164,   164,   164,   164,   164,   164,   164,
     164,     7,     8,     9,   165,   167,   167,   167,   168,   168,
     169,   170,   171,   171,   172,   172,   172,   172,   172,    14,
      22,    24,    25,    33,    34,    41,    42,    49,    50,    51,
     175,    17,   175,   123,   124,   123,   123,   123,   123,   123,
     123,   109,   124,   125,   161,   107,   123,   126,   116,   117,
     126,   128,   112,   130,   123,   124,    14,   124,    75,   125,
     126,   125,   126,   161,    14,   161,   161,   123,   125,   161,
     161,   124,   161,     5,    14,   124,   157,   161,   109,   123,
     109,   123,   109,   123,   124,   148,   109,   109,   125,   126,
     126,   161,   161,   161,   161,   161,   161,   161,   178,   126,
     161,   161,   178,   125,    27,    28,   136,   125,    10,   178,
     168,   168,    10,    14,    65,   125,   158,   133,   161,   161,
     145,    14,   161,    14,   161,   125,    27,   125,   161,   161,
      10,   123,   123,   123,   123,   123,   130,   159,    14,    22,
     129,   153,   125,   158,   124,   124,   155,   125,   178,   122,
     127,   129,   109,   161,   161,   161,   130,   161,    26,    43,
     130,    26,    43,   130,    26,    29,    30,    36,    39,    40,
      43,    44,   130,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,    92,
     124,    14,   124,   125,   178,   161,   125,   178,   126,   161,
     178,   161,   161,   178,   109,   109,   109,   125,   178,   125,
     156,   178,   161,    14,   156,   128,   112,   130,   123,   123,
     161,   107,   123,   126,    75,    75,   157,   123,   126,   123,
     161,   124,   161,   161,   125,   158,   161,   161,   109,    10,
     178,   123,   123,   123,   123,   123,   123,   123,   123,   178,
     123,   123,   123,   159,   135,   159,   124,   161,   159,   159,
      78,   159,    78,    78,   109,    10,    68,    69,   148,   125,
     126,    24,    25,    34,   130,   161,   123,   123,    37,    38,
     130,   112,   130,   112,   130,   159,   159,   159,   123,   123,
     109,   161,    14,    14,   153,    56,    57,    60,   130,   148,
     125,   125,   158,   158,    72,    73,    74,   130,   125,    14,
     161,   161,   161,    26,    30,    36,    39,    40,    43,   130,
     130,   130,   130,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   162,   125,   178,   125,   156,
     178,   123,   125,   123,   125,   178,   123,   123,   128,   128,
     128,   161,   161,   161,   125,   125,   125,    75,   125,   126,
     161,   178,   161,   161,   125,     5,    14,   123,   161,   123,
     126,   123,   148,   125,   123,   123,   161,   125,   125,    27,
     136,   161,   159,    10,    10,    10,   161,    10,    10,   148,
      10,    14,    65,   161,   161,   130,   161,    26,    29,    30,
      36,    39,    40,    43,    44,   130,   161,   159,   123,   123,
      27,   161,   123,   130,    58,   175,    10,    13,    14,    15,
      16,    56,   148,   148,   125,   125,    14,    14,    14,   109,
     124,   128,   130,   130,   161,   161,   161,   161,   161,   161,
     130,   130,   130,   130,   130,   130,   130,   130,   130,   130,
     130,   130,   125,   125,   125,   123,   125,   123,   123,   123,
     161,   178,   123,   123,   123,    75,    75,   126,   178,   148,
     123,   126,   123,   123,   135,   159,   125,    27,   136,   159,
     159,   159,   109,    10,    68,    69,    26,    30,    43,   130,
     130,   130,   161,   161,   161,   161,   161,   161,   161,   161,
     159,   130,   159,   123,   142,   161,   124,   129,   149,   124,
     149,    14,    22,    66,   109,   123,    14,    22,    66,   109,
     123,    10,    13,    15,    16,   123,   123,   175,   148,   148,
       7,     7,     7,   161,   125,   178,   109,   109,   130,   130,
     130,   130,   130,   130,   123,   125,   161,   161,   178,   123,
     178,   159,   159,   161,    10,    10,   161,   161,   161,   130,
     130,   130,   130,   130,   130,   130,   130,   161,   125,   125,
     158,    70,   133,   125,   158,    14,    14,   123,   161,    14,
      14,   123,   161,    14,    22,    66,   109,   123,    14,    22,
      66,   109,   123,   123,   123,   124,   149,   130,   130,   125,
     161,   161,   123,   125,   123,    27,   136,   130,   130,   130,
     123,   159,   149,   125,   130,    24,    25,    34,   130,   161,
     149,   125,   123,   123,   123,   123,   123,   123,    14,    14,
     123,   161,    14,    14,   123,   161,   125,   158,   130,   130,
     130,   123,   159,   142,   149,   161,   161,   130,   161,    26,
      29,    30,    36,    39,    40,    43,    44,   130,   149,   123,
     123,   123,   123,   123,   123,   149,   125,   125,    26,    30,
      43,   130,   130,   130,   161,   161,   161,   161,   161,   161,
     161,   161,   149,   159,   161,   161,   161,   130,   130,   130,
     130,   130,   130,   130,   130,   130,   130,   130
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   131,   132,   133,   133,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   135,   135,   135,   135,   135,   135,
     135,   135,   136,   136,   136,   136,   136,   136,   137,   137,
     138,   138,   138,   138,   139,   139,   140,   140,   140,   141,
     141,   141,   142,   143,   143,   143,   143,   143,   143,   144,
     144,   145,   145,   146,   146,   146,   146,   146,   146,   146,
     146,   146,   146,   146,   147,   147,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   150,   151,   152,   152,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   153,   153,
     154,   155,   155,   155,   155,   156,   156,   157,   157,   157,
     157,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   161,   161,
     161,   162,   162,   163,   163,   163,   163,   163,   163,   163,
     163,   163,   163,   163,   163,   163,   163,   163,   163,   163,
     163,   164,   164,   165,   165,   165,   166,   166,   167,   167,
     167,   168,   168,   169,   169,   170,   170,   170,   171,   171,
     171,   171,   171,   172,   172,   172,   172,   172,   173,   173,
     174,   174,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   175,   175,   176,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   177,   177,   177,
     177,   177,   178,   178,   178,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179
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
       5,     1,     3,     3,     3,     4,     6,     6,     6,     6,
       6,     6,     6,     6,     5,     7,     7,     7,     7,     7,
       7,     5,     9,     8,     8,     6,     9,     9,     5,     5,
       4,     4,     4,     6,     6,     6,     6,     3,     3,     2,
       1,     5,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     3,     3,     1,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
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
#line 426 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2896 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 436 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2902 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 437 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2908 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 441 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2914 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 443 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2920 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 445 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2926 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 447 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2934 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 451 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2940 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 453 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2946 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 455 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2952 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 457 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2960 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 461 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2967 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 464 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2975 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 468 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2981 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 470 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2989 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 474 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2996 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 477 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 3004 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 481 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3010 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 483 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3016 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 485 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3022 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 487 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3028 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 489 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3034 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 491 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3042 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 495 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3050 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 499 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3056 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 501 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3062 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 503 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3068 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 505 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3074 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 507 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3080 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 509 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3086 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 511 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3092 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 513 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3098 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 515 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3104 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 517 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3110 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 519 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3116 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 521 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3122 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 523 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3128 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 525 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3134 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 527 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3141 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 530 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3148 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 533 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3154 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 535 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3161 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 538 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3168 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 541 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3174 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 543 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3180 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 545 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3186 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 547 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3192 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 549 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3201 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 554 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3209 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 558 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3217 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 562 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3226 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 567 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3234 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 570 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3240 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 572 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3246 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 574 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3252 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 576 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3258 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 578 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3264 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 580 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3271 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 583 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3277 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 585 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3283 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 587 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3289 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 589 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3296 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 592 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3303 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 595 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3310 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 598 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3317 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 601 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3324 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 604 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3330 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 606 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3336 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 608 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3342 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 610 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3348 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 612 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3355 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 615 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3361 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 617 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3367 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 619 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3373 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 621 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3380 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 624 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3387 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 627 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3395 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 631 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3402 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 634 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3408 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 636 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3414 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 638 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3420 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 640 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3427 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 643 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3434 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 646 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3441 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 649 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3448 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 652 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3455 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 655 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3462 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 658 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3469 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 660 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3475 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 661 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3481 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 662 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3487 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 663 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3493 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 664 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3499 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 665 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3505 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 667 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3511 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 669 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3517 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 671 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3523 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 673 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3529 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 674 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3535 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 675 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3541 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 676 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3547 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 677 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3553 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 678 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3559 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 679 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3565 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 681 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3571 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 683 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3577 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 685 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3583 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 687 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3589 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 688 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3595 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 689 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3601 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 690 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3607 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 691 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3613 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 695 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3619 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 697 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3625 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 699 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3631 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 701 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3637 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 703 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3643 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 705 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3649 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 707 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3655 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 709 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3661 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 713 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3667 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 715 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3673 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 717 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3679 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 719 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3685 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 721 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3691 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 723 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3697 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 727 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3703 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 729 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3709 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 733 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3715 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 735 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3721 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 737 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3727 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 739 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3733 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 743 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3739 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 745 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3745 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 749 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3751 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 751 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3757 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 753 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3763 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 757 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3769 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 759 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3775 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 761 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3781 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 764 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3787 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 768 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3797 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 774 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3807 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 780 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3816 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 785 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3826 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 791 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3835 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 796 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3842 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 801 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3854 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 809 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3867 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 819 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3873 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 821 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3881 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 827 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3893 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 835 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3903 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 841 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3913 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 847 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3925 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 855 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3935 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 861 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3945 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 867 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3958 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 876 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3969 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 883 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3982 "raku.tab.c"
    break;

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 892 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3995 "raku.tab.c"
    break;

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 901 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 4006 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 910 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4017 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 917 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4026 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 923 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4032 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 925 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4039 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 928 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4046 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 931 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4052 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 933 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4058 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 935 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4064 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 937 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4070 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 939 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4076 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 941 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4082 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 943 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4088 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 945 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4095 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 948 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4101 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 950 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4107 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 952 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4113 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 954 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4120 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 957 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4127 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 960 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4135 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 965 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4141 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 966 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4147 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 968 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4154 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 971 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4161 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 974 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4167 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 976 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4173 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 978 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4179 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 980 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4185 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 982 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4191 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 984 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4197 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 986 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4203 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 988 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4210 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 991 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4216 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 993 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4222 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 995 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4228 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 997 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4235 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1000 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4242 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1003 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4250 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1009 "raku.y"
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
#line 4268 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1025 "raku.y"
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
#line 4285 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 1039 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4291 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 1041 "raku.y"
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
#line 4307 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1053 "raku.y"
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
#line 4323 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1066 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4329 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1068 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4336 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1071 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4343 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1074 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4350 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1077 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4357 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1080 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4364 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1083 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4371 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1086 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4378 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1089 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4385 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1092 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4392 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1095 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4399 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1098 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4406 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1101 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4413 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1104 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4424 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1111 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4435 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1118 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4446 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1125 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4457 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1132 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4465 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1136 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4473 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1140 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4481 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1144 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4489 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1148 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4500 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1155 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4511 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1162 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4522 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1169 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4533 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1176 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4547 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1186 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4559 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1194 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4571 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1202 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4584 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1211 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4595 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1218 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4606 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1225 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4620 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1235 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4632 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1243 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4644 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1253 "raku.y"
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
#line 4661 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1267 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4667 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1269 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4676 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1274 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4685 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1279 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4694 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1286 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4702 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1290 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4710 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1296 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4716 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1298 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4722 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1300 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4728 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1302 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4734 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1305 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4740 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1306 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4746 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1307 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4752 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1308 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4758 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1309 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4764 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1310 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4770 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1311 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4776 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1312 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4782 "raku.tab.c"
    break;

  case 259: /* param_list: VAR_SCALAR '=' expr  */
#line 1313 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4788 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1314 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4794 "raku.tab.c"
    break;

  case 261: /* param_list: SLURPY_POS  */
#line 1315 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 4800 "raku.tab.c"
    break;

  case 262: /* param_list: param_list ',' SLURPY_POS  */
#line 1316 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 4806 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list '}'  */
#line 1319 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4812 "raku.tab.c"
    break;

  case 264: /* block: '{' YADA '}'  */
#line 1320 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4818 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr '}'  */
#line 1322 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4824 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1324 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4830 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4836 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1328 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4842 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1330 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4848 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1332 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4855 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1335 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4861 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1337 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4867 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1339 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4873 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1341 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4879 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1343 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4886 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1346 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4893 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1349 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4901 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1353 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4908 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1356 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4915 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1359 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4922 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1362 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4928 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1364 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4936 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1368 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4943 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1371 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4950 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1374 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4957 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1377 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4964 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1380 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4971 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1383 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4977 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1385 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4983 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1387 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4989 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_LAST '}'  */
#line 1389 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4995 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1391 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 5001 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1393 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5007 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1395 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5013 "raku.tab.c"
    break;

  case 295: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1397 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5019 "raku.tab.c"
    break;

  case 296: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1399 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5025 "raku.tab.c"
    break;

  case 297: /* closure: '{' expr '}'  */
#line 1402 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5031 "raku.tab.c"
    break;

  case 298: /* expr: VAR_SCALAR '=' expr  */
#line 1405 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5037 "raku.tab.c"
    break;

  case 299: /* expr: KW_GATHER block  */
#line 1406 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5047 "raku.tab.c"
    break;

  case 300: /* expr: tern_expr  */
#line 1411 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5053 "raku.tab.c"
    break;

  case 301: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1415 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5059 "raku.tab.c"
    break;

  case 302: /* tern_expr: cmp_expr  */
#line 1416 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5065 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1419 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5071 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1420 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5077 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1421 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5083 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1422 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5089 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1423 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5095 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1424 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5101 "raku.tab.c"
    break;

  case 309: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1425 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5107 "raku.tab.c"
    break;

  case 310: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1426 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5113 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1427 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5119 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1428 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5125 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1429 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5131 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1430 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5137 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1431 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5143 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1432 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5149 "raku.tab.c"
    break;

  case 317: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1434 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5159 "raku.tab.c"
    break;

  case 318: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1440 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5169 "raku.tab.c"
    break;

  case 319: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1446 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5179 "raku.tab.c"
    break;

  case 320: /* cmp_expr: divis_expr  */
#line 1451 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5185 "raku.tab.c"
    break;

  case 321: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1454 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5191 "raku.tab.c"
    break;

  case 322: /* divis_expr: jct_expr  */
#line 1455 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5197 "raku.tab.c"
    break;

  case 323: /* jct_expr: jct_expr '|' range_expr  */
#line 1458 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5203 "raku.tab.c"
    break;

  case 324: /* jct_expr: jct_expr '&' range_expr  */
#line 1459 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5209 "raku.tab.c"
    break;

  case 325: /* jct_expr: dor_expr  */
#line 1460 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5215 "raku.tab.c"
    break;

  case 326: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1464 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5221 "raku.tab.c"
    break;

  case 327: /* dor_expr: range_expr  */
#line 1465 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5227 "raku.tab.c"
    break;

  case 328: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1468 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5233 "raku.tab.c"
    break;

  case 329: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1469 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5239 "raku.tab.c"
    break;

  case 330: /* range_expr: add_expr  */
#line 1470 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5245 "raku.tab.c"
    break;

  case 331: /* add_expr: add_expr '~' repl_expr  */
#line 1473 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5251 "raku.tab.c"
    break;

  case 332: /* add_expr: repl_expr  */
#line 1474 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5257 "raku.tab.c"
    break;

  case 333: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1477 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5263 "raku.tab.c"
    break;

  case 334: /* repl_expr: addsub_expr  */
#line 1478 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5269 "raku.tab.c"
    break;

  case 335: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1481 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5275 "raku.tab.c"
    break;

  case 336: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1482 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5281 "raku.tab.c"
    break;

  case 337: /* addsub_expr: mul_expr  */
#line 1483 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5287 "raku.tab.c"
    break;

  case 338: /* mul_expr: mul_expr '*' unary_expr  */
#line 1486 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5293 "raku.tab.c"
    break;

  case 339: /* mul_expr: mul_expr '/' unary_expr  */
#line 1487 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5299 "raku.tab.c"
    break;

  case 340: /* mul_expr: mul_expr '%' unary_expr  */
#line 1488 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5305 "raku.tab.c"
    break;

  case 341: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1489 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5311 "raku.tab.c"
    break;

  case 342: /* mul_expr: unary_expr  */
#line 1490 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5317 "raku.tab.c"
    break;

  case 343: /* unary_expr: '-' unary_expr  */
#line 1493 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5323 "raku.tab.c"
    break;

  case 344: /* unary_expr: '!' unary_expr  */
#line 1494 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5329 "raku.tab.c"
    break;

  case 345: /* unary_expr: CARET unary_expr  */
#line 1495 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5335 "raku.tab.c"
    break;

  case 346: /* unary_expr: OP_REDUCE unary_expr  */
#line 1497 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5344 "raku.tab.c"
    break;

  case 347: /* unary_expr: pow_expr  */
#line 1501 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5350 "raku.tab.c"
    break;

  case 348: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1504 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5356 "raku.tab.c"
    break;

  case 349: /* pow_expr: postfix_expr  */
#line 1505 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5362 "raku.tab.c"
    break;

  case 350: /* scalar_list: VAR_SCALAR  */
#line 1508 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5368 "raku.tab.c"
    break;

  case 351: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1509 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5374 "raku.tab.c"
    break;

  case 352: /* meth_name: IDENT  */
#line 1512 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5380 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_SORT  */
#line 1513 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5386 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_MAP  */
#line 1514 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5392 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_GREP  */
#line 1515 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5398 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_SAY  */
#line 1516 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5404 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_PRINT  */
#line 1517 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5410 "raku.tab.c"
    break;

  case 358: /* meth_name: KW_TAKE  */
#line 1518 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5416 "raku.tab.c"
    break;

  case 359: /* meth_name: KW_RETURN  */
#line 1519 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5422 "raku.tab.c"
    break;

  case 360: /* meth_name: KW_EXISTS  */
#line 1520 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5428 "raku.tab.c"
    break;

  case 361: /* meth_name: KW_DELETE  */
#line 1521 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5434 "raku.tab.c"
    break;

  case 362: /* meth_name: TESTOP  */
#line 1522 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5440 "raku.tab.c"
    break;

  case 363: /* postfix_expr: call_expr  */
#line 1524 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5446 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '(' arg_list ')'  */
#line 1527 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5455 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '(' ')'  */
#line 1531 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5461 "raku.tab.c"
    break;

  case 366: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1533 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5467 "raku.tab.c"
    break;

  case 367: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1535 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5473 "raku.tab.c"
    break;

  case 368: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1537 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5482 "raku.tab.c"
    break;

  case 369: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1542 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5488 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1544 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5498 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1550 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5508 "raku.tab.c"
    break;

  case 372: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1556 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5516 "raku.tab.c"
    break;

  case 373: /* call_expr: IDENT '.' KW_NEW  */
#line 1560 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5524 "raku.tab.c"
    break;

  case 374: /* call_expr: IDENT '.' IDENT  */
#line 1564 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5533 "raku.tab.c"
    break;

  case 375: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1569 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5544 "raku.tab.c"
    break;

  case 376: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1576 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5553 "raku.tab.c"
    break;

  case 377: /* call_expr: IDENT '.' CARET IDENT  */
#line 1581 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5563 "raku.tab.c"
    break;

  case 378: /* call_expr: atom '.' CARET IDENT  */
#line 1587 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5573 "raku.tab.c"
    break;

  case 379: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1593 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5584 "raku.tab.c"
    break;

  case 380: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1600 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5595 "raku.tab.c"
    break;

  case 381: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1607 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5604 "raku.tab.c"
    break;

  case 382: /* call_expr: atom '.' meth_name  */
#line 1612 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5613 "raku.tab.c"
    break;

  case 383: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1617 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5624 "raku.tab.c"
    break;

  case 384: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1624 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5633 "raku.tab.c"
    break;

  case 385: /* call_expr: call_expr '.' meth_name  */
#line 1629 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5642 "raku.tab.c"
    break;

  case 386: /* call_expr: KW_DIE expr  */
#line 1634 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5648 "raku.tab.c"
    break;

  case 387: /* call_expr: KW_MAP closure expr  */
#line 1636 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5654 "raku.tab.c"
    break;

  case 388: /* call_expr: KW_GREP closure expr  */
#line 1638 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5660 "raku.tab.c"
    break;

  case 389: /* call_expr: KW_SORT expr  */
#line 1640 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5666 "raku.tab.c"
    break;

  case 390: /* call_expr: KW_SORT closure expr  */
#line 1642 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5672 "raku.tab.c"
    break;

  case 391: /* call_expr: atom  */
#line 1643 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5678 "raku.tab.c"
    break;

  case 392: /* arg_list: expr  */
#line 1646 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5684 "raku.tab.c"
    break;

  case 393: /* arg_list: arg_list ',' expr  */
#line 1647 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5690 "raku.tab.c"
    break;

  case 394: /* arg_list: arg_list ','  */
#line 1648 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5696 "raku.tab.c"
    break;

  case 395: /* atom: LIT_INT  */
#line 1651 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5702 "raku.tab.c"
    break;

  case 396: /* atom: LIT_FLOAT  */
#line 1652 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5708 "raku.tab.c"
    break;

  case 397: /* atom: LIT_STR  */
#line 1653 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5714 "raku.tab.c"
    break;

  case 398: /* atom: WORDLIST  */
#line 1655 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5726 "raku.tab.c"
    break;

  case 399: /* atom: LIT_INTERP_STR  */
#line 1662 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5732 "raku.tab.c"
    break;

  case 400: /* atom: VAR_SCALAR  */
#line 1663 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5738 "raku.tab.c"
    break;

  case 401: /* atom: OP_INC VAR_SCALAR  */
#line 1664 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5744 "raku.tab.c"
    break;

  case 402: /* atom: OP_DEC VAR_SCALAR  */
#line 1665 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5750 "raku.tab.c"
    break;

  case 403: /* atom: VAR_SCALAR OP_INC  */
#line 1666 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5756 "raku.tab.c"
    break;

  case 404: /* atom: VAR_SCALAR OP_DEC  */
#line 1667 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5762 "raku.tab.c"
    break;

  case 405: /* atom: VAR_ARRAY  */
#line 1668 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5768 "raku.tab.c"
    break;

  case 406: /* atom: VAR_HASH  */
#line 1669 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5774 "raku.tab.c"
    break;

  case 407: /* atom: VAR_CAPTURE  */
#line 1671 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5782 "raku.tab.c"
    break;

  case 408: /* atom: VAR_FH  */
#line 1675 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5790 "raku.tab.c"
    break;

  case 409: /* atom: VAR_NAMED_CAPTURE  */
#line 1679 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5797 "raku.tab.c"
    break;

  case 410: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1682 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5803 "raku.tab.c"
    break;

  case 411: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1684 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5809 "raku.tab.c"
    break;

  case 412: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1686 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5815 "raku.tab.c"
    break;

  case 413: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1688 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5821 "raku.tab.c"
    break;

  case 414: /* atom: ARR_ALL_SLICE  */
#line 1690 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5827 "raku.tab.c"
    break;

  case 415: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1692 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5833 "raku.tab.c"
    break;

  case 416: /* atom: VAR_HASH '{' expr '}'  */
#line 1694 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5839 "raku.tab.c"
    break;

  case 417: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1696 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5845 "raku.tab.c"
    break;

  case 418: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1698 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5851 "raku.tab.c"
    break;

  case 419: /* atom: IDENT  */
#line 1699 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5857 "raku.tab.c"
    break;

  case 420: /* atom: VAR_TWIGIL  */
#line 1701 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5865 "raku.tab.c"
    break;

  case 421: /* atom: VAR_ARRAY_TWIGIL  */
#line 1705 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5873 "raku.tab.c"
    break;

  case 422: /* atom: VAR_HASH_TWIGIL  */
#line 1709 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5881 "raku.tab.c"
    break;

  case 423: /* atom: '(' ')'  */
#line 1712 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5887 "raku.tab.c"
    break;

  case 424: /* atom: '(' expr ')'  */
#line 1713 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5893 "raku.tab.c"
    break;

  case 425: /* atom: '(' expr ',' ')'  */
#line 1715 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5899 "raku.tab.c"
    break;

  case 426: /* atom: '(' expr ',' arg_list ')'  */
#line 1717 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5906 "raku.tab.c"
    break;

  case 427: /* atom: block  */
#line 1719 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5912 "raku.tab.c"
    break;

  case 428: /* atom: KW_SUB block  */
#line 1720 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5918 "raku.tab.c"
    break;


#line 5922 "raku.tab.c"

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

#line 1722 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
