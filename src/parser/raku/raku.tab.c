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

#line 406 "raku.tab.c"

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
#define YYLAST   4101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  130
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  426
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1086

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
       0,   420,   420,   431,   432,   435,   437,   439,   441,   445,
     447,   449,   451,   455,   458,   462,   464,   468,   471,   475,
     477,   479,   481,   483,   485,   489,   493,   495,   497,   499,
     501,   503,   505,   507,   509,   511,   513,   515,   517,   519,
     521,   524,   527,   529,   532,   535,   537,   539,   541,   543,
     548,   552,   556,   561,   565,   566,   568,   570,   572,   574,
     577,   579,   581,   583,   586,   589,   592,   595,   598,   600,
     602,   604,   606,   609,   611,   613,   615,   618,   621,   625,
     628,   630,   632,   634,   637,   640,   643,   646,   649,   652,
     655,   656,   657,   658,   659,   660,   661,   663,   665,   667,
     669,   670,   671,   672,   673,   674,   675,   677,   679,   681,
     683,   684,   685,   686,   689,   691,   693,   695,   697,   699,
     701,   703,   707,   709,   711,   713,   715,   717,   721,   723,
     727,   729,   731,   733,   737,   739,   743,   745,   747,   751,
     753,   755,   759,   762,   768,   774,   779,   785,   790,   795,
     803,   814,   815,   821,   829,   835,   841,   849,   855,   861,
     870,   877,   886,   895,   904,   911,   918,   919,   922,   925,
     927,   929,   931,   933,   935,   937,   939,   942,   944,   946,
     948,   951,   954,   960,   961,   962,   965,   968,   970,   972,
     974,   976,   978,   980,   982,   985,   987,   989,   991,   994,
     997,  1003,  1019,  1034,  1035,  1047,  1061,  1062,  1065,  1068,
    1071,  1074,  1077,  1080,  1083,  1086,  1089,  1092,  1095,  1098,
    1105,  1112,  1119,  1126,  1130,  1134,  1138,  1142,  1149,  1156,
    1163,  1170,  1180,  1188,  1196,  1205,  1212,  1219,  1229,  1237,
    1247,  1262,  1263,  1268,  1273,  1280,  1284,  1290,  1292,  1294,
    1296,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1308,
    1309,  1312,  1313,  1314,  1316,  1318,  1320,  1322,  1324,  1327,
    1329,  1331,  1333,  1335,  1338,  1341,  1345,  1348,  1351,  1354,
    1356,  1360,  1363,  1366,  1369,  1372,  1375,  1377,  1379,  1381,
    1383,  1385,  1387,  1389,  1391,  1395,  1398,  1399,  1404,  1407,
    1409,  1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,
    1421,  1422,  1423,  1424,  1425,  1426,  1432,  1438,  1444,  1447,
    1448,  1451,  1452,  1453,  1456,  1458,  1461,  1462,  1463,  1466,
    1467,  1470,  1471,  1474,  1475,  1476,  1479,  1480,  1481,  1482,
    1483,  1486,  1487,  1488,  1489,  1494,  1497,  1498,  1501,  1502,
    1505,  1506,  1507,  1508,  1509,  1510,  1511,  1512,  1513,  1514,
    1515,  1517,  1519,  1524,  1525,  1527,  1529,  1534,  1536,  1542,
    1548,  1552,  1556,  1561,  1568,  1573,  1579,  1585,  1592,  1599,
    1604,  1609,  1616,  1621,  1626,  1628,  1630,  1632,  1634,  1636,
    1639,  1640,  1641,  1644,  1645,  1646,  1647,  1655,  1656,  1657,
    1658,  1659,  1660,  1661,  1662,  1663,  1667,  1671,  1674,  1676,
    1678,  1680,  1682,  1684,  1686,  1688,  1690,  1692,  1693,  1697,
    1701,  1705,  1706,  1707,  1709,  1712,  1713
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

#define YYPACT_NINF (-790)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-426)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -790,    16,  1650,  -790,  -790,  -790,  -790,  -790,   632,   -68,
     -38,   -82,   -11,  -790,  -790,  3973,  -790,  -790,  -790,   108,
    1720,    38,  2993,  3063,  3133,  3203,  3273,    -7,    89,  3273,
    1790,   167,  3273,   324,   360,  3343,  3413,    89,   -12,   112,
     179,   119,   119,  3483,    89,    89,  3273,   270,   400,   117,
    3973,  -790,  -790,   412,   373,   444,  3973,  3973,  -790,  1860,
     363,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,
    -790,   443,  -790,  -790,  -790,   903,   477,  -790,   604,   446,
     292,   345,  -790,   240,   405,   409,   325,  -790,  -790,   388,
     452,   500,   620,  -790,  -790,  3273,  3273,  3273,  3273,  3273,
    3273,   626,  1931,  3553,  2002,   630,  3273,  3273,    48,  2072,
     187,   434,   140,  -790,    89,  -790,  -790,   478,   238,  -790,
    2143,  -790,    58,   -62,   116,   198,   737,   634,   235,   646,
    1860,   422,  1860,    57,  1860,    89,  1860,    89,   129,   251,
     209,  -790,  -790,   548,  -790,   569,   605,   642,   567,   190,
     234,  1860,    89,  1860,    89,    47,  2214,  -790,  3273,  3273,
    -790,  3273,  3273,  -790,  3273,  3273,  3273,  2284,  3273,  -790,
     654,  -790,  -790,  -790,   624,   619,     4,  -790,   628,  -790,
    -790,  -790,  -790,  -790,   417,   635,  1099,  3273,  3273,  3273,
    3273,  3273,  3273,  3273,  3273,  -790,  3273,  3273,  3273,  3273,
    3273,  3273,  3273,  3273,  -790,  3973,  3973,  3973,  3973,  3973,
    3973,  3973,  3973,  3973,  3973,  3973,  3973,  3973,  3973,  3973,
     754,  3973,  3973,  3973,  3973,  3973,  3973,  3973,  3973,  3973,
    3973,  3973,  3973,  3973,  3973,  3973,   647,   613,   600,   673,
     677,   686,   688,   692,   710,   135,  -790,   667,  1860,   -45,
     689,   351,   749,   733,   761,   763,   853,   765,   -37,  -790,
     682,   696,  2002,   863,  3273,  -790,  3273,   775,   417,   698,
    -790,  3273,  3273,  -790,  3623,  -790,  3693,  -790,   201,   221,
     241,   299,   791,   813,  -790,   716,   718,  3273,  3273,  3273,
    3273,  3273,  3273,  3273,  -790,  3273,   720,  3273,  3273,  -790,
    3273,   723,   822,   729,  -790,   902,  3273,  -790,  3973,  3973,
       5,  -790,  -790,  -790,  -790,  3273,  3273,  -790,   924,  3273,
     927,  3273,   731,   919,   739,  -790,  3273,  3273,   970,   834,
     873,   882,   897,   901,   880,  -790,  -790,  -790,    89,    -5,
    -790,    15,   906,   907,  -790,  -790,  2354,  -790,   738,   250,
     259,   923,  2993,  3063,  3273,   762,    94,   199,  -790,   237,
     910,   911,   912,   921,   922,   925,   926,   928,   929,   930,
     931,   935,   937,   938,   939,   940,   941,   941,   941,   941,
     941,   941,   955,   941,   941,   941,   941,   941,   941,   941,
     941,  -790,  -790,  -790,   292,  -790,  -790,  -790,   944,   944,
     405,   409,   325,   325,  -790,  -790,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,
     942,  1035,   943,  -790,  2425,  -790,  -790,  -790,  -790,  -790,
    -790,  3273,  2496,  -790,   741,  3273,  -790,  3273,  3273,  3273,
    3273,   956,   959,   960,  -790,  2567,  -790,  2638,  3273,  -790,
    1049,  -790,  3763,   458,   958,   945,  -790,  -790,   948,  -790,
     950,  1860,   130,   999,    -6,  2709,   248,   957,  3273,  -790,
    3833,  -790,  3273,  -790,    21,  -790,  3273,  3273,   972,  1068,
    2354,   961,   962,   963,   964,   965,   966,   967,   331,  2354,
     968,   969,   355,    89,    19,  3903,  -790,    89,    89,   174,
      10,    26,   973,    41,   954,   745,  1268,   971,   974,    44,
     981,   977,   984,   978,    89,    89,    89,   976,   986,   991,
    3273,  -790,  -790,  -790,  -790,  -790,  -790,  1080,  1086,  -790,
       7,   954,   751,    22,   382,   284,  -790,   755,  1112,  2002,
    3273,  3273,    49,   115,    86,  -790,    92,  3273,  3273,  -790,
    3273,  3273,  -790,  3273,  3273,  3273,  3273,  3273,  3273,  3273,
    3273,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  3973,  2780,
    -790,  2851,  1005,   757,  1014,  -790,   768,  2354,  1015,   389,
    1011,  1012,   551,  3273,  3273,  3273,  -790,   770,  -790,   777,
     779,  -790,  1081,   800,  -790,  -790,  -790,  -790,  -790,   802,
    3273,  -790,  3273,  3273,  3273,   804,  -790,   197,  -790,  1034,
    1860,   401,  1038,   954,   806,  1039,  1042,  3273,  -790,   808,
    -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,   810,  -790,
    -790,  -790,   909,  -790,  -790,  1860,    89,  -790,  -790,  1147,
    -790,  1156,  1160,  3273,  -790,  1161,  1162,  -790,   954,   486,
    2993,  3063,  1459,  -790,   338,  -790,  -790,  3273,    89,  -790,
    -790,  -790,  1051,  1052,  1148,  -790,  -790,  -790,  -790,  3273,
    1054,  -790,  -790,    14,   556,   744,  1121,  -790,  -790,   954,
     954,   815,   820,  1164,  1165,  1166,  -790,  -790,   252,   610,
    1053,   149,  3273,  3273,  3273,  3273,  3273,  3273,  -790,  -790,
    -790,  -790,   257,   262,   272,   278,   286,   302,   342,   368,
     369,   370,   372,   386,  -790,  -790,   823,  -790,   826,   828,
    -790,  1059,  -790,  -790,   830,  -790,  -790,  -790,  -790,  -790,
    1061,  1062,  1063,  -790,  -790,  -790,  3273,  -790,  2354,  1064,
     479,  -790,  -790,  1065,  1114,  1115,  -790,   835,  -790,  3273,
    -790,  -790,   954,  -790,  -790,   565,  1069,  1070,    19,  -790,
     837,   936,    89,    89,    89,  -790,  -790,  -790,  -790,  1082,
      46,   268,   213,  -790,   390,  3273,  3273,  3273,  3273,  3273,
    3273,  3273,  3273,  -790,    89,  1067,  -790,  -790,    89,  1071,
    3273,  -790,   307,   399,    85,   105,   814,  1075,  1076,   647,
    -790,  -790,   954,   954,  1192,  1193,  1194,  3273,  2922,  1094,
    1095,  -790,   414,   437,   439,   440,   453,   465,  -790,  -790,
    -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  1083,  -790,  -790,  -790,  -790,   841,
    -790,  -790,  -790,  3273,  3273,  2354,   586,  -790,  -790,  3273,
    -790,  -790,  -790,  -790,    89,    89,  -790,  -790,  -790,  -790,
    3273,  -790,  1196,  1197,  3273,  3273,  3273,  -790,  -790,  -790,
     466,   474,   480,   488,   491,   493,   494,   502,  -790,  -790,
    -790,  3273,  1085,  -790,    28,  1135,  -790,    29,  -790,  1198,
    1199,  1088,  3273,  -790,  1200,  1202,  1089,  3273,  -790,   111,
     153,  1096,  1097,  -790,  -790,   449,  -790,  -790,  -790,  -790,
    -790,   504,  1091,   843,  3273,  3273,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  1101,  -790,  -790,   845,  -790,   599,   946,
    -790,  -790,  -790,  -790,   507,   510,   520,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  -790,  1102,    89,  1098,   851,  1100,
    1389,  1098,   875,  1103,  1108,  -790,  1109,  1110,  1111,  -790,
    1113,  1203,  1220,  1116,  3273,  -790,  1222,  1223,  1117,  3273,
    -790,  -790,  -790,    45,  -790,  -790,  -790,  1118,   523,   528,
    -790,  1119,  -790,    89,  -790,  -790,  -790,  -790,  3273,  -790,
    -790,  1098,  -790,  2993,  3063,  1529,  -790,   359,  -790,  1098,
    -790,  -790,  -790,  -790,  -790,  -790,  1120,  1122,  -790,  1123,
    1124,  1126,  -790,  1127,  1098,   887,  -790,  -790,  -790,  -790,
    -790,  1128,  -790,   380,   296,  -790,   544,  3273,  3273,  3273,
    3273,  3273,  3273,  3273,  3273,  -790,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  1098,    89,  3273,  3273,  3273,  -790,
    -790,  -790,   545,   546,   557,   577,   587,   588,   590,   612,
    -790,  -790,   614,   616,   617,  -790,  -790,  -790,  -790,  -790,
    -790,  -790,  -790,  -790,  -790,  -790
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   393,   394,   395,   397,   398,   403,
     404,   418,   417,   419,   420,     0,   405,   406,   407,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   412,   396,     0,     0,     0,     0,     0,    91,     0,
       3,     4,    92,    93,   100,   101,   102,   103,    94,    95,
     110,     0,   111,   112,   113,    99,     0,   298,   300,   318,
     320,   323,   325,   328,   330,   332,   335,   340,   345,   347,
     361,   389,     0,   401,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,   403,   404,   418,     0,   425,   343,     0,   398,    35,
       0,   390,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   328,
       0,   426,   297,     0,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   136,     0,   139,     0,     0,
     104,     0,     0,   105,     0,     0,     0,     3,     0,   387,
      96,    98,   384,   203,     0,     0,     0,   344,     0,   399,
     400,   342,   341,   421,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    54,     0,     0,     0,     0,
       0,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   296,     0,   367,     0,     0,     0,
       0,     0,     0,     0,     0,   372,     0,   371,   417,   363,
       0,     0,     0,     0,     0,    30,     0,   421,     0,     0,
      38,   392,     0,     6,     0,     9,     0,    10,     0,     0,
       0,     0,     0,     0,   348,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,    42,
       0,     0,   117,     0,   129,     0,     0,   148,     0,     0,
       0,     3,   155,    45,    46,     0,     0,   151,     0,     0,
       0,     0,     0,   132,     0,   135,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   385,   386,   388,     0,     0,
     206,     0,     0,     0,   241,   422,     0,   262,   398,   403,
     404,   418,     0,     0,     0,     0,     0,     0,   261,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   303,   304,   307,   308,
     301,   302,     0,   305,   306,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   319,   321,   322,   324,   326,   327,
     329,   331,   333,   334,   339,   336,   337,   338,   346,   350,
     360,   354,   355,   356,   357,   358,   359,   352,   353,   351,
     383,     0,   380,    51,     0,    85,    86,    87,    88,    89,
      48,     0,     0,   366,     0,     0,    15,     0,     0,     0,
       0,   408,   413,   414,    53,     0,   375,     0,     0,   364,
       0,   362,   392,     0,     0,     0,   296,    37,     0,   391,
       0,     0,     0,   395,   417,     0,     0,     0,     0,    27,
       0,    28,     0,    29,     0,   158,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   422,     0,     0,   120,   422,     0,     0,
     326,   327,   251,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   422,     0,   422,     0,     0,     0,
       0,   106,   107,   108,   109,   295,    97,     0,     0,   206,
       0,     0,     0,     0,     0,     0,   423,     0,     0,     0,
       0,     0,     0,     0,     0,   288,     0,     0,     0,   289,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
       0,   263,    55,    57,    59,    62,    60,    61,    56,    58,
      68,    70,    72,    75,    73,    74,    69,    71,     0,     0,
     376,     0,     0,     0,     0,   165,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   374,     0,   370,     0,
       0,   245,     0,     0,   408,   413,   414,    36,     5,     0,
       0,    11,     0,     0,     0,     0,    20,     0,    19,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
      76,    79,    78,    82,    80,    81,    77,    40,     0,    83,
      84,    43,   114,   119,   118,     0,     0,   128,   145,     0,
     147,     0,     0,     0,   252,     0,     0,   154,     0,     0,
       0,     0,     0,   166,     0,    32,    31,     0,     0,   149,
     415,   416,     0,     0,   130,   133,   134,   137,   138,     0,
       0,   204,   205,     0,     0,     0,     0,   202,   163,     0,
       0,     0,     0,     0,     0,     0,   240,   424,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   272,   279,
     286,   287,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   299,   382,     0,   379,     0,     0,
      50,     0,    52,   164,     0,    17,    16,   411,   410,   409,
       0,     0,     0,   373,   368,   369,     0,   365,     0,     0,
       0,   248,   247,     0,     0,     0,    22,     0,    23,     0,
      26,   157,     0,    34,    33,     0,   424,   424,     0,   121,
       0,   122,     0,     0,     0,   259,   253,   254,   153,   255,
       0,     0,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   167,     0,     0,    66,    67,     0,     0,
       0,   201,     0,     0,     0,     0,     0,     0,     0,     0,
     162,   160,     0,     0,     0,     0,     0,     0,     0,   408,
     414,   283,     0,     0,     0,     0,     0,     0,   291,   292,
     293,   294,   264,   266,   268,   271,   269,   270,   265,   267,
     381,   378,   377,    49,   424,    63,    64,    65,   246,     0,
      13,    12,    21,     0,     0,     0,     0,   156,     7,     0,
      41,    44,   116,   115,   422,     0,   126,   146,   143,   144,
       0,   256,     0,     0,     0,     0,     0,   170,   171,   168,
       0,     0,     0,     0,     0,     0,     0,     0,   152,   150,
     131,     0,     0,   142,     0,     3,   236,     0,   233,     0,
       0,     0,     0,   212,     0,     0,     0,     0,   207,     0,
       0,     0,     0,   208,   209,     0,   159,   161,   242,   243,
     244,     0,   165,     0,     0,     0,   273,   275,   278,   276,
     277,   274,    18,   424,   250,   249,     0,    24,     0,   123,
     124,   260,   257,   258,     0,     0,     0,   172,   174,   176,
     179,   177,   178,   173,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,   224,
       0,     0,     0,     0,     0,   214,     0,     0,     0,     0,
     213,   210,   211,     0,   239,   282,   281,   164,     0,     0,
      14,   424,     8,     0,   127,   180,   182,   181,     0,   141,
     235,     0,   184,     0,     0,     0,   183,     0,   232,     0,
     221,   222,   216,   219,   220,   215,     0,     0,   225,     0,
       0,     0,   226,     0,     0,     0,   280,   284,   285,    25,
     125,     0,   234,     0,     0,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   185,   231,   229,   230,   218,
     227,   228,   217,   238,     0,     0,     0,     0,     0,   188,
     189,   186,     0,     0,     0,     0,     0,     0,     0,     0,
     237,   140,     0,     0,     0,   190,   192,   194,   197,   195,
     196,   191,   193,   198,   200,   199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -790,  -790,     1,  -790,  -489,  -636,  -790,  -790,  -790,  -790,
    -790,   242,  -790,  -790,  -790,  -790,  -790,  -278,  -789,  -790,
    -790,  -790,   714,  -790,  -790,  -439,   785,  -330,   303,   983,
      -2,  -207,  -790,   701,  1030,  -790,   547,   -16,  1026,  1027,
     798,    -3,  -790,  -790,  -235,  -790,  -790,   139,  -790
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   186,    61,    62,   496,    63,    64,    65,    66,
      67,   892,    68,    69,   509,    70,    71,   312,   896,    72,
      73,   339,   530,    74,   535,   260,   466,   505,   115,   165,
     121,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,   285,   420,    89,    90,   122,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      76,     2,   422,   475,   382,   643,   769,   140,   599,   527,
     139,   532,   116,   603,   898,   502,     3,   528,   342,   503,
     131,   133,   135,   137,   138,   502,   107,   143,   145,   503,
     148,   502,   502,   152,   154,   503,   503,   448,   502,   502,
     103,   169,   503,   503,   172,    24,   272,   177,   123,   124,
     125,   654,   126,   181,   182,   502,   871,   184,   104,   503,
     273,   435,   255,   684,   685,   256,   343,   686,   614,   127,
     684,   685,   105,   128,   686,   702,   326,   436,   288,   703,
     437,   667,   668,   297,   108,   704,   109,   651,   705,   706,
     106,   327,   707,   239,   240,   241,   242,   243,   244,   899,
     298,   249,   251,   652,   253,   254,   257,   900,   655,   656,
     108,   156,   109,   872,   873,   108,    60,   109,   268,   904,
     547,    60,   117,   529,   227,   971,   984,   905,   286,   504,
     296,   175,   301,   972,   303,   866,   687,   548,   158,   531,
     227,   297,   728,   801,   624,   623,   690,    60,   176,   322,
     901,   324,   957,   961,   329,   159,   330,   331,   298,   332,
     333,   129,   334,   335,   336,   334,   337,   976,  1000,  1024,
     906,   294,  1008,   669,   295,   977,   973,   146,   708,   299,
     270,   147,   300,   271,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   902,   368,   369,   370,   371,   372,   373,
     374,   375,   754,   691,   692,   161,   305,   903,   313,   398,
     399,   755,  1032,   907,   314,   710,   160,    60,   978,   974,
    1046,   711,   162,   549,   274,   550,   657,   908,   404,   405,
     406,   407,   408,   975,   160,  1053,   610,   299,   275,   297,
     300,   247,   551,   431,   709,   282,   434,   164,   261,   283,
     263,   649,   611,   688,   306,   612,   298,    60,   432,   269,
     453,   979,   455,   553,   456,  1070,   554,   555,   264,   459,
     460,   444,   462,   556,   467,   980,   557,   558,   821,   862,
     559,   560,    93,    94,   173,   481,   482,   483,   484,   485,
     486,   487,   500,   501,   874,   490,   491,   288,   875,   271,
     318,   163,    60,   994,   290,    75,   276,   291,   292,   468,
     102,   876,   506,   507,   508,   225,   226,   511,   319,   513,
     277,   163,   297,   469,   517,   518,   308,   309,   552,   470,
     141,   142,   310,    93,    94,   299,   149,   311,   300,   298,
     155,   157,   878,   471,   320,   761,   266,   170,   171,   472,
     542,   543,   544,   546,   227,   693,   694,   695,   103,   204,
     817,   102,   321,   473,   785,   227,   561,   786,   787,   105,
     616,   724,   150,   617,   788,   818,   539,   789,   790,   521,
     778,   791,   792,   179,   522,  1037,   828,   540,  1038,  1039,
     294,   829,   502,   295,   523,  1040,   503,   877,  1041,  1042,
     524,   830,  1043,  1044,   222,   223,  1056,   831,   570,   288,
    1057,   810,   811,   696,   174,   832,   290,   141,   299,   291,
     292,   300,   474,  1058,   571,  1060,   178,   311,   231,   584,
     894,   833,   185,   588,   488,   895,   590,   591,   302,   492,
     304,   307,   232,   233,   234,   499,   601,   224,   287,   803,
     459,   288,   289,   637,   180,   323,   271,   325,   290,   609,
     204,   291,   292,   184,   572,   293,   619,   793,   621,   187,
     622,   834,   188,   189,   625,   626,   440,   641,   441,   190,
     271,   204,   191,   192,   857,   537,   193,   194,  1045,    75,
     573,   574,   575,   646,   576,   235,   779,   835,   836,   837,
     780,   838,   294,   196,   664,   295,   197,   198,   577,  1059,
     228,   736,   314,   199,   271,   839,   200,   201,   680,   879,
     202,   203,   897,   758,   229,   230,   759,   895,   214,   215,
     216,   217,   218,   219,   916,   917,   630,   699,   700,   701,
     220,   345,   346,   926,   294,   712,   713,   295,   714,   715,
     221,   716,   717,   718,   719,   720,   721,   722,   723,   632,
     262,   633,   634,   583,   958,   195,   927,   962,   928,   929,
     409,   586,   983,   236,   915,   635,   589,   895,   410,   592,
     411,   412,   930,   440,   597,   604,   600,   636,   570,   413,
     414,   740,   741,   742,   931,   947,   571,   415,   416,   204,
     265,   851,   572,   948,   271,   417,   418,   419,   749,   949,
     573,   751,   752,   574,   802,   575,   576,   950,   757,   629,
     951,   237,   952,   953,   577,   765,   732,   409,   638,   630,
     421,   954,   632,   985,   238,   410,   995,   411,   412,   996,
     245,   526,   636,   770,   252,   845,   413,   414,   281,   997,
     847,   775,  1027,  1025,   415,   416,   284,  1028,   781,   782,
     784,   409,   417,   418,   419,   794,   314,   570,   571,   410,
     313,   411,   412,  1061,  1075,  1076,   271,   799,   739,   572,
     413,   414,   205,   206,   207,   208,  1077,   858,   415,   416,
     859,   314,   209,   210,   211,   317,   417,   418,   419,   573,
     822,   823,   824,   825,   826,   827,  1078,   338,   937,   574,
     575,   271,   576,   315,   212,   213,  1079,  1080,   726,  1081,
     729,   992,   423,   424,   271,    92,   734,    93,    94,    95,
      96,    97,    98,    99,   577,   440,   630,   819,   632,   636,
     100,  1082,   341,  1083,   848,  1084,  1085,   278,   279,   280,
     316,   750,   340,   101,   804,   102,   344,   805,   806,   807,
     808,   391,   392,   393,   347,     4,     5,     6,     7,   395,
     396,   397,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,   880,   881,   882,   883,   884,   885,   886,
     887,   433,   271,   114,    28,   425,   642,   644,   893,   426,
     647,   648,   650,    33,   438,   439,   449,   450,   427,    75,
     428,    41,    42,    43,   429,   921,    46,   674,   675,   676,
     451,   452,   458,   271,   909,    50,    51,   910,    52,   911,
     912,    92,   430,    93,    94,    95,    96,    97,    98,    99,
     478,   479,   345,   480,   345,   489,   100,   493,   346,   494,
     495,   934,   935,   497,   346,   514,   346,    54,    55,   538,
     442,   102,   443,   516,   346,   345,   587,   446,   941,   658,
     659,    56,   944,   945,   946,   689,   659,   454,    57,   697,
     271,   731,   271,   444,   144,    59,   445,   849,   447,   955,
      60,   545,   733,   271,   743,   271,   960,   457,   856,   476,
     966,   744,   450,   745,   271,   970,   376,   377,   378,   379,
     380,   381,   498,   383,   384,   385,   386,   387,   388,   389,
     390,   477,   988,   989,   747,   450,   345,   748,   753,   617,
     762,   659,   766,   271,   767,   271,   768,   495,   510,   812,
     659,   512,  -425,  -425,   813,   659,   515,   840,   271,   771,
     841,   450,   842,   271,   844,   271,   520,   923,  1007,   345,
     855,   864,   346,   865,   495,   933,   271,   987,   271,   991,
     271,   795,  1019,   993,   495,  1001,   659,  1023,  -425,  -425,
     519,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,  -425,
    -425,  -425,  -425,  -425,   936,   521,   893,  -425,   938,  1009,
     659,  1033,  1034,  1036,   522,  -425,  -425,  -425,  -425,   525,
    -425,  1054,   659,  -425,  -425,  -425,  -425,  -425,  -425,   523,
    -425,  -425,  -425,   524,  -425,   166,   168,   402,   403,   533,
     534,   541,   562,   563,   564,  1062,  1063,  1064,  1065,  1066,
    1067,  1068,  1069,   565,   566,   221,   578,   567,   568,   580,
     569,   570,   571,   572,  1072,  1073,  1074,   573,   227,   574,
     575,   576,   577,   602,   593,   579,   581,   594,   595,   605,
     607,   863,   608,   613,   606,   867,   868,   869,   628,   618,
     627,   653,   311,   630,   631,   632,   633,   634,   635,   636,
     639,   640,   670,   665,   681,   672,   666,   888,   677,   679,
     682,   890,     4,     5,     6,     7,   671,   673,   678,   348,
     349,   350,   351,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,   352,   353,    24,   698,   730,    25,    26,
      27,    28,   354,   355,    31,    32,   732,   735,   737,   738,
      33,    34,    35,    36,    37,    38,   356,   357,    41,    42,
      43,    44,    45,    46,    47,   746,   756,   772,    48,    49,
     760,   763,    50,    51,   764,    52,   773,   939,   940,    53,
     774,   776,   777,   796,   797,   798,   800,   809,   814,   815,
     816,   843,   820,   845,   846,   847,   850,   852,   853,   854,
     870,   860,   861,   891,    54,    55,   889,   913,   914,   918,
     919,   920,   924,   925,   959,   932,   942,   943,    56,   956,
     965,   969,   963,   964,   967,    57,   968,  1016,   981,   982,
     986,    58,    59,   990,   998,  1010,   895,    60,   358,  1002,
    1011,  1012,  1013,  1014,  1017,  1015,  1020,  1021,  1018,  1022,
    1031,  1029,  1047,   683,  1048,  1049,  1050,  1026,  1051,  1052,
     615,   394,  1055,   400,     0,   401,     0,     0,     0,   999,
       0,     0,     0,    75,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   660,   661,    24,     0,  1030,    25,    26,    27,
      28,    29,   662,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     0,     0,     0,    48,    49,     0,
       0,    50,    51,     0,    52,     0,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1071,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,     0,     0,
       0,     0,     0,     0,    57,     0,     0,     0,     0,     0,
      58,    59,     4,     5,     6,     7,    60,   663,     0,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,  1003,  1004,    24,     0,     0,    25,    26,
      27,    28,    29,  1005,    31,    32,     0,     0,     0,     0,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     0,     0,     0,    48,    49,
       0,     0,    50,    51,     0,    52,     0,     0,     0,    53,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,    58,    59,    46,     0,     0,     0,    60,  1006,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     0,     0,   118,
     111,   112,   113,    12,    13,    14,    15,    16,    17,    18,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,     0,    56,     0,
      33,     0,     0,     0,     0,    57,     0,     0,    41,    42,
      43,   144,    59,    46,     0,     0,     0,    60,   783,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,    57,     0,     0,     0,     0,
       0,   144,    59,     4,     5,     6,     7,    60,  1035,     0,
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
       0,     0,   118,   111,   112,   113,   258,    13,    14,    15,
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
       0,     0,    41,    42,    43,    59,   259,    46,     0,     0,
      60,     0,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     0,     0,     0,   118,   111,   112,   113,    12,    13,
      14,    15,    16,    17,    18,     0,     0,   328,    54,    55,
       0,     0,     0,     0,     0,   114,    28,     0,     0,     0,
       0,     0,    56,     0,     0,    33,     0,     0,     0,    57,
       0,     0,     0,    41,    42,    43,    59,   267,    46,     0,
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
      57,     0,     0,     0,    41,    42,    43,    59,   536,    46,
       0,     0,    60,     0,     0,     0,     0,     0,    50,    51,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     0,     0,   118,   111,   112,   113,
      12,    13,    14,    15,    16,    17,    18,     0,     0,     0,
      54,    55,     0,     0,     0,     0,     0,   114,    28,     0,
       0,     0,     0,     0,    56,     0,     0,    33,     0,     0,
       0,    57,     0,     0,     0,    41,    42,    43,    59,   582,
      46,     0,     0,    60,     0,     0,     0,     0,     0,    50,
      51,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     0,     0,     0,   118,   111,   112,
     113,    12,    13,    14,    15,    16,    17,    18,     0,     0,
       0,    54,    55,     0,     0,     0,     0,     0,   114,    28,
       0,     0,     0,     0,     0,    56,     0,     0,    33,     0,
       0,     0,    57,     0,     0,     0,    41,    42,    43,    59,
     585,    46,     0,     0,    60,     0,     0,     0,     0,     0,
      50,    51,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     0,     0,     0,   118,   111,
     112,   113,   258,    13,    14,    15,    16,    17,    18,     0,
       0,     0,    54,    55,     0,     0,     0,     0,     0,   114,
      28,     0,     0,     0,     0,     0,    56,     0,     0,    33,
       0,     0,     0,    57,     0,     0,     0,    41,    42,    43,
      59,   596,    46,     0,     0,    60,     0,     0,     0,     0,
       0,    50,    51,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     4,     5,   463,     7,     0,     0,     0,   118,
     111,   112,   113,   464,    13,    14,    15,    16,    17,    18,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
     114,    28,     0,     0,     0,     0,     0,    56,     0,     0,
      33,     0,     0,     0,    57,     0,     0,     0,    41,    42,
      43,    59,   598,    46,     0,     0,    60,     0,     0,     0,
       0,     0,    50,    51,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     0,     0,     0,
     118,   111,   112,   113,    12,    13,    14,    15,    16,    17,
      18,     0,     0,     0,    54,    55,     0,     0,     0,     0,
       0,   114,    28,     0,     0,     0,     0,     0,    56,     0,
       0,    33,     0,     0,     0,    57,     0,     0,     0,    41,
      42,    43,    59,   183,    46,     0,     0,    60,     0,     0,
       0,     0,     0,    50,    51,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     0,     0,
       0,   118,   111,   112,   113,   258,    13,    14,    15,    16,
      17,    18,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,   114,    28,     0,     0,     0,     0,     0,    56,
       0,     0,    33,     0,     0,     0,    57,     0,     0,     0,
      41,    42,    43,    59,   725,    46,     0,     0,    60,     0,
       0,     0,     0,     0,    50,    51,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     0,
       0,     0,   118,   111,   112,   113,    12,    13,    14,    15,
      16,    17,    18,     0,     0,     0,    54,    55,     0,     0,
       0,     0,     0,   114,    28,     0,     0,     0,     0,     0,
      56,     0,     0,    33,     0,     0,     0,    57,     0,     0,
       0,    41,    42,    43,    59,   727,    46,     0,     0,    60,
       0,     0,     0,     0,     0,    50,    51,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,    54,    55,     0,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,    56,     0,     0,    33,     0,     0,     0,    57,     0,
       0,     0,    41,    42,    43,    59,   922,    46,     0,     0,
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
       0,     0,     0,     0,     0,     0,     4,     5,   463,     7,
       0,     0,     0,   118,   111,   112,   113,   464,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   461,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   118,   111,   112,   113,   258,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,    28,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   465,    46,     0,     0,
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
       0,     0,    41,    42,    43,     0,   620,    46,     0,     0,
       0,    60,     0,     0,     0,     0,    50,    51,     0,    52,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       0,     0,     0,   110,   111,   112,   113,    12,    13,    14,
      15,    16,    17,    18,     0,     0,     0,     0,    54,    55,
       0,     0,     0,     0,   114,     0,     0,     0,     0,     0,
       0,     0,    56,     0,    33,     0,     0,     0,     0,    57,
       0,     0,    41,    42,    43,     0,   645,    46,     0,     0,
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
       2,     0,   237,   281,   211,   494,   642,    14,   447,    14,
      26,   341,    15,   452,   803,    10,     0,    22,    14,    14,
      22,    23,    24,    25,    26,    10,   108,    29,    30,    14,
      32,    10,    10,    35,    36,    14,    14,    74,    10,    10,
     108,    43,    14,    14,    46,    26,   108,    50,    10,    11,
      12,    10,    14,    56,    57,    10,    10,    59,   126,    14,
     122,   106,    14,    56,    57,    17,    62,    60,    74,    31,
      56,    57,   110,    35,    60,    26,    29,   122,    29,    30,
     125,    37,    38,    26,   121,    36,   123,    77,    39,    40,
     128,    44,    43,    95,    96,    97,    98,    99,   100,    14,
      43,   103,   104,    77,   106,   107,    58,    22,    67,    68,
     121,   123,   123,    67,    68,   121,   128,   123,   120,    14,
      26,   128,    14,   128,   114,    14,   915,    22,   130,   124,
     132,    14,   134,    22,   136,   771,   129,    43,    26,   124,
     114,    26,   581,   129,   474,   124,   124,   128,    31,   151,
      65,   153,   124,   124,   156,    43,   158,   159,    43,   161,
     162,   123,   164,   165,   166,   167,   168,    14,   957,   124,
      65,   122,   961,   129,   125,    22,    65,    10,   129,   122,
     122,    14,   125,   125,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   108,   196,   197,   198,   199,   200,   201,
     202,   203,     5,   533,   534,    26,    77,   122,   122,   225,
     226,    14,  1001,   108,   122,   129,   122,   128,    65,   108,
    1009,   129,    43,   129,   108,    26,   504,   122,   231,   232,
     233,   234,   235,   122,   122,  1024,   106,   122,   122,    26,
     125,   102,    43,   108,   129,    10,   248,   128,   109,    14,
     110,    77,   122,   531,   125,   125,    43,   128,   123,   120,
     262,   108,   264,    26,   266,  1054,    29,    30,   128,   271,
     272,   122,   274,    36,   276,   122,    39,    40,   129,   768,
      43,    44,    95,    96,    14,   287,   288,   289,   290,   291,
     292,   293,   308,   309,    26,   297,   298,    29,    30,   125,
     110,   122,   128,   939,    36,     2,   108,    39,    40,   108,
     123,    43,   311,   315,   316,    75,    76,   319,   128,   321,
     122,   122,    26,   122,   326,   327,    75,    76,   129,   108,
      27,    28,   123,    95,    96,   122,    12,   128,   125,    43,
      37,    38,   129,   122,   110,   623,   108,    44,    45,   108,
     352,   353,   354,   355,   114,    71,    72,    73,   108,   122,
     108,   123,   128,   122,    26,   114,   129,    29,    30,   110,
     122,   578,    12,   125,    36,   123,   126,    39,    40,   122,
     658,    43,    44,    10,   122,    26,   129,   128,    29,    30,
     122,   129,    10,   125,   122,    36,    14,   129,    39,    40,
     122,   129,    43,    44,   112,   113,    26,   129,   122,    29,
      30,   689,   690,   129,    14,   129,    36,   114,   122,    39,
      40,   125,   123,    43,   122,   129,    14,   128,   103,   431,
     123,   129,    69,   435,   295,   128,   438,   439,   135,   300,
     137,   138,   117,   118,   119,   306,   448,   102,    26,   684,
     452,    29,    30,   122,    10,   152,   125,   154,    36,   461,
     122,    39,    40,   465,   122,    43,   468,   129,   470,    26,
     472,   129,    29,    30,   476,   477,   125,   122,   127,    36,
     125,   122,    39,    40,   762,   346,    43,    44,   129,   186,
     122,   122,   122,   495,   122,   107,    10,   129,   129,   129,
      14,   129,   122,    26,   506,   125,    29,    30,   122,   129,
     105,   122,   122,    36,   125,   129,    39,    40,   520,   129,
      43,    44,   123,   122,   115,   116,   125,   128,    82,    83,
      84,    85,    86,    87,   812,   813,   122,   539,   540,   541,
      94,   124,   125,   129,   122,   547,   548,   125,   550,   551,
     104,   553,   554,   555,   556,   557,   558,   559,   560,   122,
     126,   122,   122,   424,   894,   122,   129,   897,   129,   129,
      14,   432,   123,   121,   809,   122,   437,   128,    22,   440,
      24,    25,   129,   125,   445,   127,   447,   122,   122,    33,
      34,   593,   594,   595,   129,   129,   122,    41,    42,   122,
     122,   122,   122,   129,   125,    49,    50,    51,   610,   129,
     122,   613,   614,   122,    58,   122,   122,   129,   620,   480,
     129,   121,   129,   129,   122,   627,   122,    14,   489,   122,
      17,   129,   122,   129,    14,    22,   129,    24,    25,   129,
      14,   338,   122,   645,    14,   122,    33,    34,    14,   129,
     122,   653,   129,   983,    41,    42,    10,   129,   660,   661,
     662,    14,    49,    50,    51,   667,   122,   122,   122,    22,
     122,    24,    25,   129,   129,   129,   125,   679,   127,   122,
      33,    34,    78,    79,    80,    81,   129,   122,    41,    42,
     125,   122,    88,    89,    90,   128,    49,    50,    51,   122,
     702,   703,   704,   705,   706,   707,   129,    53,   122,   122,
     122,   125,   122,   108,   110,   111,   129,   129,   579,   129,
     581,   122,   122,   123,   125,    93,   587,    95,    96,    97,
      98,    99,   100,   101,   122,   125,   122,   127,   122,   122,
     108,   129,   123,   129,   746,   129,   129,    10,    11,    12,
     108,   612,   128,   121,    10,   123,   128,    13,    14,    15,
      16,     7,     8,     9,   129,     3,     4,     5,     6,   222,
     223,   224,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,   785,   786,   787,   788,   789,   790,   791,
     792,   124,   125,    31,    32,   122,   493,   494,   800,   122,
     497,   498,   499,    41,   115,   116,   124,   125,   122,   506,
     122,    49,    50,    51,   122,   817,    54,   514,   515,   516,
     124,   125,   124,   125,    10,    63,    64,    13,    66,    15,
      16,    93,   122,    95,    96,    97,    98,    99,   100,   101,
     124,   125,   124,   125,   124,   125,   108,   124,   125,    27,
      28,   853,   854,   124,   125,   124,   125,    95,    96,   121,
     111,   123,   129,   124,   125,   124,   125,    14,   870,   124,
     125,   109,   874,   875,   876,   124,   125,    14,   116,   124,
     125,   124,   125,   122,   122,   123,   123,   748,   123,   891,
     128,   129,   124,   125,   124,   125,   895,   122,   759,   108,
     902,   124,   125,   124,   125,   907,   205,   206,   207,   208,
     209,   210,    10,   212,   213,   214,   215,   216,   217,   218,
     219,   108,   924,   925,   124,   125,   124,   125,   124,   125,
     124,   125,   124,   125,   124,   125,    27,    28,    14,   124,
     125,    14,    39,    40,   124,   125,    27,   124,   125,   646,
     124,   125,   124,   125,   124,   125,   122,   818,   960,   124,
     125,   124,   125,    27,    28,   124,   125,   124,   125,   124,
     125,   668,   974,    27,    28,   124,   125,   979,    75,    76,
      10,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,   855,   122,   998,    94,   859,   124,
     125,  1003,  1004,  1005,   122,   102,   103,   104,   105,   129,
     107,   124,   125,   110,   111,   112,   113,   114,   115,   122,
     117,   118,   119,   122,   121,    42,    43,   229,   230,   123,
     123,   108,   122,   122,   122,  1037,  1038,  1039,  1040,  1041,
    1042,  1043,  1044,   122,   122,   104,    91,   122,   122,    14,
     122,   122,   122,   122,  1056,  1057,  1058,   122,   114,   122,
     122,   122,   122,    14,   108,   123,   123,   108,   108,   111,
     122,   768,   122,    74,   129,   772,   773,   774,    10,   122,
     108,   108,   128,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   111,   122,    14,   111,   122,   794,   122,   108,
      14,   798,     3,     4,     5,     6,   129,   129,   122,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    14,   122,    29,    30,
      31,    32,    33,    34,    35,    36,   122,   122,   127,   127,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    74,   122,    10,    59,    60,
     122,   122,    63,    64,   122,    66,    10,   864,   865,    70,
      10,    10,    10,   122,   122,    27,   122,    56,    14,    14,
      14,   122,   129,   122,   122,   122,   122,   122,    74,    74,
     108,   122,   122,   122,    95,    96,   129,   122,   122,     7,
       7,     7,   108,   108,    69,   122,    10,    10,   109,   124,
     122,   122,    14,    14,    14,   116,    14,    14,   122,   122,
     129,   122,   123,   122,   122,   122,   128,   128,   129,   129,
     122,   122,   122,   122,    14,   122,    14,    14,   122,   122,
     998,   122,   122,   529,   122,   122,   122,   129,   122,   122,
     465,   221,   124,   227,    -1,   228,    -1,    -1,    -1,   956,
      -1,    -1,    -1,   960,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,   993,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    -1,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    -1,    -1,    -1,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1055,    -1,
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
     117,   160,    14,   160,   160,    14,    17,    58,    14,   124,
     155,   177,   126,   110,   128,   122,   108,   124,   160,   177,
     122,   125,   108,   122,   108,   122,   108,   122,    10,    11,
      12,    14,    10,    14,    10,   173,   160,    26,    29,    30,
      36,    39,    40,    43,   122,   125,   160,    26,    43,   122,
     125,   160,   158,   160,   158,    77,   125,   158,    75,    76,
     123,   128,   147,   122,   122,   108,   108,   128,   110,   128,
     110,   128,   160,   158,   160,   158,    29,    44,    23,   160,
     160,   160,   160,   160,   160,   160,   160,   160,    53,   151,
     128,   123,    14,    62,   128,   124,   125,   129,    10,    11,
      12,    13,    24,    25,    33,    34,    47,    48,   129,   160,
     160,   160,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   160,   160,   163,   163,   163,   163,
     163,   163,   161,   163,   163,   163,   163,   163,   163,   163,
     163,     7,     8,     9,   164,   166,   166,   166,   167,   167,
     168,   169,   170,   170,   171,   171,   171,   171,   171,    14,
      22,    24,    25,    33,    34,    41,    42,    49,    50,    51,
     174,    17,   174,   122,   123,   122,   122,   122,   122,   122,
     122,   108,   123,   124,   160,   106,   122,   125,   115,   116,
     125,   127,   111,   129,   122,   123,    14,   123,    74,   124,
     125,   124,   125,   160,    14,   160,   160,   122,   124,   160,
     160,   123,   160,     5,    14,   123,   156,   160,   108,   122,
     108,   122,   108,   122,   123,   147,   108,   108,   124,   125,
     125,   160,   160,   160,   160,   160,   160,   160,   177,   125,
     160,   160,   177,   124,    27,    28,   135,   124,    10,   177,
     167,   167,    10,    14,   124,   157,   132,   160,   160,   144,
      14,   160,    14,   160,   124,    27,   124,   160,   160,    10,
     122,   122,   122,   122,   122,   129,   158,    14,    22,   128,
     152,   124,   157,   123,   123,   154,   124,   177,   121,   126,
     128,   108,   160,   160,   160,   129,   160,    26,    43,   129,
      26,    43,   129,    26,    29,    30,    36,    39,    40,    43,
      44,   129,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,    91,   123,
      14,   123,   124,   177,   160,   124,   177,   125,   160,   177,
     160,   160,   177,   108,   108,   108,   124,   177,   124,   155,
     177,   160,    14,   155,   127,   111,   129,   122,   122,   160,
     106,   122,   125,    74,    74,   156,   122,   125,   122,   160,
     123,   160,   160,   124,   157,   160,   160,   108,    10,   177,
     122,   122,   122,   122,   122,   122,   122,   122,   177,   122,
     122,   122,   158,   134,   158,   123,   160,   158,   158,    77,
     158,    77,    77,   108,    10,    67,    68,   147,   124,   125,
      24,    25,    34,   129,   160,   122,   122,    37,    38,   129,
     111,   129,   111,   129,   158,   158,   158,   122,   122,   108,
     160,    14,    14,   152,    56,    57,    60,   129,   147,   124,
     124,   157,   157,    71,    72,    73,   129,   124,    14,   160,
     160,   160,    26,    30,    36,    39,    40,    43,   129,   129,
     129,   129,   160,   160,   160,   160,   160,   160,   160,   160,
     160,   160,   160,   160,   161,   124,   177,   124,   155,   177,
     122,   124,   122,   124,   177,   122,   122,   127,   127,   127,
     160,   160,   160,   124,   124,   124,    74,   124,   125,   160,
     177,   160,   160,   124,     5,    14,   122,   160,   122,   125,
     122,   147,   124,   122,   122,   160,   124,   124,    27,   135,
     160,   158,    10,    10,    10,   160,    10,    10,   147,    10,
      14,   160,   160,   129,   160,    26,    29,    30,    36,    39,
      40,    43,    44,   129,   160,   158,   122,   122,    27,   160,
     122,   129,    58,   174,    10,    13,    14,    15,    16,    56,
     147,   147,   124,   124,    14,    14,    14,   108,   123,   127,
     129,   129,   160,   160,   160,   160,   160,   160,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     124,   124,   124,   122,   124,   122,   122,   122,   160,   177,
     122,   122,   122,    74,    74,   125,   177,   147,   122,   125,
     122,   122,   134,   158,   124,    27,   135,   158,   158,   158,
     108,    10,    67,    68,    26,    30,    43,   129,   129,   129,
     160,   160,   160,   160,   160,   160,   160,   160,   158,   129,
     158,   122,   141,   160,   123,   128,   148,   123,   148,    14,
      22,    65,   108,   122,    14,    22,    65,   108,   122,    10,
      13,    15,    16,   122,   122,   174,   147,   147,     7,     7,
       7,   160,   124,   177,   108,   108,   129,   129,   129,   129,
     129,   129,   122,   124,   160,   160,   177,   122,   177,   158,
     158,   160,    10,    10,   160,   160,   160,   129,   129,   129,
     129,   129,   129,   129,   129,   160,   124,   124,   157,    69,
     132,   124,   157,    14,    14,   122,   160,    14,    14,   122,
     160,    14,    22,    65,   108,   122,    14,    22,    65,   108,
     122,   122,   122,   123,   148,   129,   129,   124,   160,   160,
     122,   124,   122,    27,   135,   129,   129,   129,   122,   158,
     148,   124,   129,    24,    25,    34,   129,   160,   148,   124,
     122,   122,   122,   122,   122,   122,    14,    14,   122,   160,
      14,    14,   122,   160,   124,   157,   129,   129,   129,   122,
     158,   141,   148,   160,   160,   129,   160,    26,    29,    30,
      36,    39,    40,    43,    44,   129,   148,   122,   122,   122,
     122,   122,   122,   148,   124,   124,    26,    30,    43,   129,
     129,   129,   160,   160,   160,   160,   160,   160,   160,   160,
     148,   158,   160,   160,   160,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129
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
     156,   157,   157,   157,   157,   157,   157,   157,   157,   157,
     157,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,   158,   159,   160,   160,   160,   161,
     161,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   162,   162,   163,
     163,   164,   164,   164,   165,   165,   166,   166,   166,   167,
     167,   168,   168,   169,   169,   169,   170,   170,   170,   170,
     170,   171,   171,   171,   171,   171,   172,   172,   173,   173,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     174,   175,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   176,   176,   176,
     177,   177,   177,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178
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
       5,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       9,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     2,     1,     5,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     2,     2,     2,     2,     1,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     4,     6,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     2,     3,     3,     2,     3,     1,
       1,     3,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     4,     6,
       6,     6,     1,     4,     4,     5,     5,     1,     1,     1,
       1,     2,     3,     4,     5,     1,     2
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
#line 421 "raku.y"
        {
            ExprList *all = (yyvsp[0].list);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) add_proc(all->items[i]);
                exprlist_free(all);
            }
        }
#line 2883 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 431 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 2889 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 432 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 2895 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 436 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 2901 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 438 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 2907 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 440 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 2913 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 442 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 2921 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 446 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2927 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_HASH ';'  */
#line 448 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 2933 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 450 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2939 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 452 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2947 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 456 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2954 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 459 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2962 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 463 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 2968 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 465 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2976 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' expr OP_REP_XX expr ';'  */
#line 469 "raku.y"
        { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-3].node)); expr_add_child(call,(yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 2983 "raku.tab.c"
    break;

  case 18: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 472 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 2991 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 476 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 2997 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 478 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 3003 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 480 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 3009 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 482 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3015 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 484 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 3021 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 486 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); free((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3029 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 490 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); free((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 3037 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 494 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); free((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 3043 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 496 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3049 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 498 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3055 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 500 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 3061 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT ';'  */
#line 502 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); free((yyvsp[-1].sval)); (yyval.node)=u; }
#line 3067 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 504 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3073 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 506 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3079 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 508 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3085 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 510 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); free((yyvsp[-3].sval)); }
#line 3091 "raku.tab.c"
    break;

  case 35: /* stmt: TESTOP ';'  */
#line 512 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); free((yyvsp[-1].sval)); }
#line 3097 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 514 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-4].sval))); free((yyvsp[-4].sval)); ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3103 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' ')' ';'  */
#line 516 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); free((yyvsp[-3].sval)); }
#line 3109 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP arg_list ';'  */
#line 518 "raku.y"
        { tree_t *c=make_call(testop_rt((yyvsp[-2].sval))); free((yyvsp[-2].sval)); ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3115 "raku.tab.c"
    break;

  case 39: /* stmt: KW_SAY expr ';'  */
#line 520 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3121 "raku.tab.c"
    break;

  case 40: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 522 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3128 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 525 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3135 "raku.tab.c"
    break;

  case 42: /* stmt: KW_PRINT expr ';'  */
#line 528 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3141 "raku.tab.c"
    break;

  case 43: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 530 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3148 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 533 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 3155 "raku.tab.c"
    break;

  case 45: /* stmt: KW_TAKE expr ';'  */
#line 536 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 3161 "raku.tab.c"
    break;

  case 46: /* stmt: KW_RETURN expr ';'  */
#line 538 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 3167 "raku.tab.c"
    break;

  case 47: /* stmt: KW_RETURN ';'  */
#line 540 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 3173 "raku.tab.c"
    break;

  case 48: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 542 "raku.y"
        { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 3179 "raku.tab.c"
    break;

  case 49: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 544 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 3188 "raku.tab.c"
    break;

  case 50: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 549 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 3196 "raku.tab.c"
    break;

  case 51: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 553 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 3204 "raku.tab.c"
    break;

  case 52: /* stmt: VAR_SCALAR '.' IDENT '=' expr ';'  */
#line 557 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD);
          fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval));
          expr_add_child(fe,var_node((yyvsp[-5].sval)));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3213 "raku.tab.c"
    break;

  case 53: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 562 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 3221 "raku.tab.c"
    break;

  case 54: /* stmt: scalar_methcall ';'  */
#line 565 "raku.y"
                                                 { (yyval.node)=(yyvsp[-1].node); }
#line 3227 "raku.tab.c"
    break;

  case 55: /* stmt: scalar_methcall KW_IF expr ';'  */
#line 567 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3233 "raku.tab.c"
    break;

  case 56: /* stmt: scalar_methcall KW_UNLESS expr ';'  */
#line 569 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3239 "raku.tab.c"
    break;

  case 57: /* stmt: scalar_methcall KW_WHILE expr ';'  */
#line 571 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3245 "raku.tab.c"
    break;

  case 58: /* stmt: scalar_methcall KW_UNTIL expr ';'  */
#line 573 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3251 "raku.tab.c"
    break;

  case 59: /* stmt: scalar_methcall KW_FOR expr ';'  */
#line 575 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3258 "raku.tab.c"
    break;

  case 60: /* stmt: scalar_methcall KW_WITH expr ';'  */
#line 578 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3264 "raku.tab.c"
    break;

  case 61: /* stmt: scalar_methcall KW_WITHOUT expr ';'  */
#line 580 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3270 "raku.tab.c"
    break;

  case 62: /* stmt: scalar_methcall KW_GIVEN expr ';'  */
#line 582 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3276 "raku.tab.c"
    break;

  case 63: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 584 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3283 "raku.tab.c"
    break;

  case 64: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 587 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3290 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 590 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 3297 "raku.tab.c"
    break;

  case 66: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 593 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 3304 "raku.tab.c"
    break;

  case 67: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 596 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 3311 "raku.tab.c"
    break;

  case 68: /* stmt: expr KW_IF expr ';'  */
#line 599 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3317 "raku.tab.c"
    break;

  case 69: /* stmt: expr KW_UNLESS expr ';'  */
#line 601 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3323 "raku.tab.c"
    break;

  case 70: /* stmt: expr KW_WHILE expr ';'  */
#line 603 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 3329 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_UNTIL expr ';'  */
#line 605 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 3335 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_FOR expr ';'  */
#line 607 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 3342 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WITH expr ';'  */
#line 610 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 3348 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_WITHOUT expr ';'  */
#line 612 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 3354 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_GIVEN expr ';'  */
#line 614 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 3360 "raku.tab.c"
    break;

  case 76: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 616 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 3367 "raku.tab.c"
    break;

  case 77: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 619 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 3374 "raku.tab.c"
    break;

  case 78: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 622 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 3382 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 626 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 3389 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 629 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 3395 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 631 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 3401 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 633 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 3407 "raku.tab.c"
    break;

  case 83: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 635 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 3414 "raku.tab.c"
    break;

  case 84: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 638 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 3421 "raku.tab.c"
    break;

  case 85: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 641 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 3428 "raku.tab.c"
    break;

  case 86: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 644 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 3435 "raku.tab.c"
    break;

  case 87: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 647 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 3442 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 650 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 3449 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 653 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 3456 "raku.tab.c"
    break;

  case 90: /* stmt: expr ';'  */
#line 655 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 3462 "raku.tab.c"
    break;

  case 91: /* stmt: ';'  */
#line 656 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 3468 "raku.tab.c"
    break;

  case 92: /* stmt: if_stmt  */
#line 657 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3474 "raku.tab.c"
    break;

  case 93: /* stmt: while_stmt  */
#line 658 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3480 "raku.tab.c"
    break;

  case 94: /* stmt: for_stmt  */
#line 659 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3486 "raku.tab.c"
    break;

  case 95: /* stmt: given_stmt  */
#line 660 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3492 "raku.tab.c"
    break;

  case 96: /* stmt: KW_TRY block  */
#line 662 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3498 "raku.tab.c"
    break;

  case 97: /* stmt: KW_TRY block KW_CATCH block  */
#line 664 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3504 "raku.tab.c"
    break;

  case 98: /* stmt: KW_CATCH block  */
#line 666 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3510 "raku.tab.c"
    break;

  case 99: /* stmt: block  */
#line 668 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 3516 "raku.tab.c"
    break;

  case 100: /* stmt: unless_stmt  */
#line 669 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3522 "raku.tab.c"
    break;

  case 101: /* stmt: until_stmt  */
#line 670 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3528 "raku.tab.c"
    break;

  case 102: /* stmt: repeat_stmt  */
#line 671 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3534 "raku.tab.c"
    break;

  case 103: /* stmt: loop_stmt  */
#line 672 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3540 "raku.tab.c"
    break;

  case 104: /* stmt: KW_LAST ';'  */
#line 673 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 3546 "raku.tab.c"
    break;

  case 105: /* stmt: KW_NEXT ';'  */
#line 674 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 3552 "raku.tab.c"
    break;

  case 106: /* stmt: KW_LAST KW_IF expr ';'  */
#line 676 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3558 "raku.tab.c"
    break;

  case 107: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 678 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 3564 "raku.tab.c"
    break;

  case 108: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 680 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3570 "raku.tab.c"
    break;

  case 109: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 682 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 3576 "raku.tab.c"
    break;

  case 110: /* stmt: sub_decl  */
#line 683 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3582 "raku.tab.c"
    break;

  case 111: /* stmt: class_decl  */
#line 684 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3588 "raku.tab.c"
    break;

  case 112: /* stmt: role_decl  */
#line 685 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3594 "raku.tab.c"
    break;

  case 113: /* stmt: grammar_decl  */
#line 686 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 3600 "raku.tab.c"
    break;

  case 114: /* if_stmt: KW_IF '(' expr ')' block  */
#line 690 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3606 "raku.tab.c"
    break;

  case 115: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 692 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3612 "raku.tab.c"
    break;

  case 116: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 694 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3618 "raku.tab.c"
    break;

  case 117: /* if_stmt: KW_IF expr block  */
#line 696 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3624 "raku.tab.c"
    break;

  case 118: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 698 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3630 "raku.tab.c"
    break;

  case 119: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 700 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3636 "raku.tab.c"
    break;

  case 120: /* if_stmt: KW_IF expr block elsif_tail  */
#line 702 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3642 "raku.tab.c"
    break;

  case 121: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 704 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3648 "raku.tab.c"
    break;

  case 122: /* elsif_tail: KW_ELSIF expr block  */
#line 708 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3654 "raku.tab.c"
    break;

  case 123: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 710 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3660 "raku.tab.c"
    break;

  case 124: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 712 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3666 "raku.tab.c"
    break;

  case 125: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 714 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3672 "raku.tab.c"
    break;

  case 126: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 716 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3678 "raku.tab.c"
    break;

  case 127: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 718 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3684 "raku.tab.c"
    break;

  case 128: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 722 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3690 "raku.tab.c"
    break;

  case 129: /* while_stmt: KW_WHILE expr block  */
#line 724 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)); }
#line 3696 "raku.tab.c"
    break;

  case 130: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 728 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3702 "raku.tab.c"
    break;

  case 131: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 730 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3708 "raku.tab.c"
    break;

  case 132: /* unless_stmt: KW_UNLESS expr block  */
#line 732 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3714 "raku.tab.c"
    break;

  case 133: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 734 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3720 "raku.tab.c"
    break;

  case 134: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 738 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3726 "raku.tab.c"
    break;

  case 135: /* until_stmt: KW_UNTIL expr block  */
#line 740 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 3732 "raku.tab.c"
    break;

  case 136: /* repeat_stmt: KW_REPEAT block  */
#line 744 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=e; }
#line 3738 "raku.tab.c"
    break;

  case 137: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 746 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=e; }
#line 3744 "raku.tab.c"
    break;

  case 138: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 748 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=e; }
#line 3750 "raku.tab.c"
    break;

  case 139: /* loop_stmt: KW_LOOP block  */
#line 752 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=expr_binary(TT_WHILE,one,(yyvsp[0].node)); }
#line 3756 "raku.tab.c"
    break;

  case 140: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 754 "raku.y"
        { (yyval.node)=rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3762 "raku.tab.c"
    break;

  case 141: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 756 "raku.y"
        { (yyval.node)=rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)); }
#line 3768 "raku.tab.c"
    break;

  case 142: /* loop_incr: expr  */
#line 759 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 3774 "raku.tab.c"
    break;

  case 143: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 763 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3784 "raku.tab.c"
    break;

  case 144: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 769 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = r; }
#line 3794 "raku.tab.c"
    break;

  case 145: /* for_stmt: KW_FOR expr OP_ARROW VAR_SCALAR block  */
#line 775 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
          gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3803 "raku.tab.c"
    break;

  case 146: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 780 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3813 "raku.tab.c"
    break;

  case 147: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 786 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3822 "raku.tab.c"
    break;

  case 148: /* for_stmt: KW_FOR expr block  */
#line 791 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = expr_binary(TT_EVERY, gen, (yyvsp[0].node)); }
#line 3829 "raku.tab.c"
    break;

  case 149: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 796 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 3841 "raku.tab.c"
    break;

  case 150: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 804 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 3854 "raku.tab.c"
    break;

  case 151: /* when_list: %empty  */
#line 814 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 3860 "raku.tab.c"
    break;

  case 152: /* when_list: when_list KW_WHEN expr block  */
#line 816 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 3868 "raku.tab.c"
    break;

  case 153: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 822 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3880 "raku.tab.c"
    break;

  case 154: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 830 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3890 "raku.tab.c"
    break;

  case 155: /* sub_decl: KW_SUB IDENT sub_body  */
#line 836 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3900 "raku.tab.c"
    break;

  case 156: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 842 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3912 "raku.tab.c"
    break;

  case 157: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 850 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3922 "raku.tab.c"
    break;

  case 158: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 856 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 3932 "raku.tab.c"
    break;

  case 159: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 862 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-4].sval)); (yyval.node)=e; }
#line 3945 "raku.tab.c"
    break;

  case 160: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 871 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3956 "raku.tab.c"
    break;

  case 161: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 878 "raku.y"
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

  case 162: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 887 "raku.y"
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

  case 163: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 896 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          free((yyvsp[-3].sval)); (yyval.node)=e; }
#line 3993 "raku.tab.c"
    break;

  case 164: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' arg_list ')'  */
#line 905 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 4004 "raku.tab.c"
    break;

  case 165: /* scalar_methcall: VAR_SCALAR '.' IDENT '(' ')'  */
#line 912 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval)));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 4013 "raku.tab.c"
    break;

  case 166: /* sub_body: '{' stmt_list '}'  */
#line 918 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4019 "raku.tab.c"
    break;

  case 167: /* sub_body: '{' stmt_list expr '}'  */
#line 920 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4026 "raku.tab.c"
    break;

  case 168: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 923 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4033 "raku.tab.c"
    break;

  case 169: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 926 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4039 "raku.tab.c"
    break;

  case 170: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 928 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4045 "raku.tab.c"
    break;

  case 171: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 930 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4051 "raku.tab.c"
    break;

  case 172: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 932 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4057 "raku.tab.c"
    break;

  case 173: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 934 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4063 "raku.tab.c"
    break;

  case 174: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 936 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4069 "raku.tab.c"
    break;

  case 175: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 938 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4075 "raku.tab.c"
    break;

  case 176: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 940 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4082 "raku.tab.c"
    break;

  case 177: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 943 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4088 "raku.tab.c"
    break;

  case 178: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 945 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4094 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 947 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4100 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 949 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4107 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 952 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4114 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 955 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4122 "raku.tab.c"
    break;

  case 183: /* method_body: '{' stmt_list '}'  */
#line 960 "raku.y"
                                 { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4128 "raku.tab.c"
    break;

  case 184: /* method_body: '{' YADA '}'  */
#line 961 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4134 "raku.tab.c"
    break;

  case 185: /* method_body: '{' stmt_list expr '}'  */
#line 963 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4141 "raku.tab.c"
    break;

  case 186: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 966 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4148 "raku.tab.c"
    break;

  case 187: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 969 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4154 "raku.tab.c"
    break;

  case 188: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 971 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4160 "raku.tab.c"
    break;

  case 189: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 973 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4166 "raku.tab.c"
    break;

  case 190: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 975 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4172 "raku.tab.c"
    break;

  case 191: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 977 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4178 "raku.tab.c"
    break;

  case 192: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 979 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4184 "raku.tab.c"
    break;

  case 193: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 981 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4190 "raku.tab.c"
    break;

  case 194: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 983 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4197 "raku.tab.c"
    break;

  case 195: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 986 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4203 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 988 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4209 "raku.tab.c"
    break;

  case 197: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 990 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4215 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 992 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4222 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 995 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4229 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 998 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4237 "raku.tab.c"
    break;

  case 201: /* class_decl: KW_CLASS IDENT is_clauses '{' class_body_list '}'  */
#line 1004 "raku.y"
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
#line 4255 "raku.tab.c"
    break;

  case 202: /* role_decl: KW_ROLE IDENT '{' class_body_list '}'  */
#line 1020 "raku.y"
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
#line 4272 "raku.tab.c"
    break;

  case 203: /* is_clauses: %empty  */
#line 1034 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 4278 "raku.tab.c"
    break;

  case 204: /* is_clauses: is_clauses IDENT IDENT  */
#line 1036 "raku.y"
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
#line 4294 "raku.tab.c"
    break;

  case 205: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1048 "raku.y"
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
#line 4310 "raku.tab.c"
    break;

  case 206: /* class_body_list: %empty  */
#line 1061 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4316 "raku.tab.c"
    break;

  case 207: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1063 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4323 "raku.tab.c"
    break;

  case 208: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1066 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4330 "raku.tab.c"
    break;

  case 209: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1069 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4337 "raku.tab.c"
    break;

  case 210: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1072 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4344 "raku.tab.c"
    break;

  case 211: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1075 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4351 "raku.tab.c"
    break;

  case 212: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1078 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 4358 "raku.tab.c"
    break;

  case 213: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1081 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4365 "raku.tab.c"
    break;

  case 214: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1084 "raku.y"
        { free((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4372 "raku.tab.c"
    break;

  case 215: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1087 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4379 "raku.tab.c"
    break;

  case 216: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1090 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4386 "raku.tab.c"
    break;

  case 217: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1093 "raku.y"
        { free((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4393 "raku.tab.c"
    break;

  case 218: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1096 "raku.y"
        { free((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); free((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4400 "raku.tab.c"
    break;

  case 219: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1099 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4411 "raku.tab.c"
    break;

  case 220: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1106 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4422 "raku.tab.c"
    break;

  case 221: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1113 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4433 "raku.tab.c"
    break;

  case 222: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1120 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4444 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1127 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4452 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1131 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 4460 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1135 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4468 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1139 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 4476 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1143 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4487 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1150 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4498 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1157 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4509 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1164 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          free((yyvsp[-4].sval)); free((yyvsp[-3].sval)); free((yyvsp[-2].sval)); free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 4520 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1171 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4534 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1181 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4546 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1189 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4558 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1197 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4571 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1206 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 4582 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1213 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 4593 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1220 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 4607 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1230 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 4619 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1238 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          free((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 4631 "raku.tab.c"
    break;

  case 240: /* grammar_decl: KW_GRAMMAR IDENT '{' grammar_body_list '}'  */
#line 1248 "raku.y"
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
#line 4648 "raku.tab.c"
    break;

  case 241: /* grammar_body_list: %empty  */
#line 1262 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 4654 "raku.tab.c"
    break;

  case 242: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1264 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4663 "raku.tab.c"
    break;

  case 243: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1269 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4672 "raku.tab.c"
    break;

  case 244: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1274 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); free((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 4681 "raku.tab.c"
    break;

  case 245: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1281 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 4689 "raku.tab.c"
    break;

  case 246: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1285 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 4697 "raku.tab.c"
    break;

  case 247: /* pair_list: IDENT OP_FATARROW expr  */
#line 1291 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4703 "raku.tab.c"
    break;

  case 248: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1293 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 4709 "raku.tab.c"
    break;

  case 249: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1295 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); free((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4715 "raku.tab.c"
    break;

  case 250: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1297 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 4721 "raku.tab.c"
    break;

  case 251: /* param_list: VAR_SCALAR  */
#line 1300 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 4727 "raku.tab.c"
    break;

  case 252: /* param_list: IDENT VAR_SCALAR  */
#line 1301 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4733 "raku.tab.c"
    break;

  case 253: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1302 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4739 "raku.tab.c"
    break;

  case 254: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1303 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4745 "raku.tab.c"
    break;

  case 255: /* param_list: param_list ',' VAR_SCALAR  */
#line 1304 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 4751 "raku.tab.c"
    break;

  case 256: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1305 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); free((yyvsp[-1].sval)); }
#line 4757 "raku.tab.c"
    break;

  case 257: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1306 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4763 "raku.tab.c"
    break;

  case 258: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1307 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); free((yyvsp[-2].sval)); }
#line 4769 "raku.tab.c"
    break;

  case 259: /* param_list: VAR_SCALAR '=' expr  */
#line 1308 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4775 "raku.tab.c"
    break;

  case 260: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1309 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 4781 "raku.tab.c"
    break;

  case 261: /* block: '{' stmt_list '}'  */
#line 1312 "raku.y"
                         { (yyval.node)=make_seq((yyvsp[-1].list)); }
#line 4787 "raku.tab.c"
    break;

  case 262: /* block: '{' YADA '}'  */
#line 1313 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 4793 "raku.tab.c"
    break;

  case 263: /* block: '{' stmt_list expr '}'  */
#line 1315 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 4799 "raku.tab.c"
    break;

  case 264: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1317 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4805 "raku.tab.c"
    break;

  case 265: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1319 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4811 "raku.tab.c"
    break;

  case 266: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1321 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4817 "raku.tab.c"
    break;

  case 267: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1323 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4823 "raku.tab.c"
    break;

  case 268: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1325 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4830 "raku.tab.c"
    break;

  case 269: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1328 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4836 "raku.tab.c"
    break;

  case 270: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1330 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4842 "raku.tab.c"
    break;

  case 271: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1332 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4848 "raku.tab.c"
    break;

  case 272: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1334 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 4854 "raku.tab.c"
    break;

  case 273: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1336 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4861 "raku.tab.c"
    break;

  case 274: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1339 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4868 "raku.tab.c"
    break;

  case 275: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1342 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4876 "raku.tab.c"
    break;

  case 276: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1346 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4883 "raku.tab.c"
    break;

  case 277: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1349 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4890 "raku.tab.c"
    break;

  case 278: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1352 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=(yyvsp[-5].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4897 "raku.tab.c"
    break;

  case 279: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1355 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 4903 "raku.tab.c"
    break;

  case 280: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' arg_list ')' '}'  */
#line 1357 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(c,args->items[i]); exprlist_free(args); }
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4911 "raku.tab.c"
    break;

  case 281: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '(' ')' '}'  */
#line 1361 "raku.y"
        { tree_t *c=ast_node_new(TT_METHCALL); ast_push(c,var_node((yyvsp[-5].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *l=(yyvsp[-6].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4918 "raku.tab.c"
    break;

  case 282: /* block: '{' stmt_list VAR_SCALAR '.' IDENT '=' expr '}'  */
#line 1364 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); free((yyvsp[-3].sval)); expr_add_child(fe,var_node((yyvsp[-5].sval)));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-6].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4925 "raku.tab.c"
    break;

  case 283: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1367 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=(yyvsp[-4].list); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 4932 "raku.tab.c"
    break;

  case 284: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1370 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4939 "raku.tab.c"
    break;

  case 285: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1373 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=(yyvsp[-7].list); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 4946 "raku.tab.c"
    break;

  case 286: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1376 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 4952 "raku.tab.c"
    break;

  case 287: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1378 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=(yyvsp[-3].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4958 "raku.tab.c"
    break;

  case 288: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1380 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=(yyvsp[-2].list); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 4964 "raku.tab.c"
    break;

  case 289: /* block: '{' stmt_list KW_LAST '}'  */
#line 1382 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 4970 "raku.tab.c"
    break;

  case 290: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1384 "raku.y"
        { ExprList *l=(yyvsp[-2].list); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 4976 "raku.tab.c"
    break;

  case 291: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1386 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4982 "raku.tab.c"
    break;

  case 292: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1388 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4988 "raku.tab.c"
    break;

  case 293: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1390 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 4994 "raku.tab.c"
    break;

  case 294: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1392 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=(yyvsp[-4].list); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5000 "raku.tab.c"
    break;

  case 295: /* closure: '{' expr '}'  */
#line 1395 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 5006 "raku.tab.c"
    break;

  case 296: /* expr: VAR_SCALAR '=' expr  */
#line 1398 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 5012 "raku.tab.c"
    break;

  case 297: /* expr: KW_GATHER block  */
#line 1399 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 5022 "raku.tab.c"
    break;

  case 298: /* expr: tern_expr  */
#line 1404 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5028 "raku.tab.c"
    break;

  case 299: /* tern_expr: cmp_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1408 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5034 "raku.tab.c"
    break;

  case 300: /* tern_expr: cmp_expr  */
#line 1409 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 5040 "raku.tab.c"
    break;

  case 301: /* cmp_expr: cmp_expr OP_AND divis_expr  */
#line 1412 "raku.y"
                                  { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5046 "raku.tab.c"
    break;

  case 302: /* cmp_expr: cmp_expr OP_OR divis_expr  */
#line 1413 "raku.y"
                                  { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5052 "raku.tab.c"
    break;

  case 303: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1414 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 5058 "raku.tab.c"
    break;

  case 304: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1415 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 5064 "raku.tab.c"
    break;

  case 305: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1416 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 5070 "raku.tab.c"
    break;

  case 306: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1417 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 5076 "raku.tab.c"
    break;

  case 307: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1418 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 5082 "raku.tab.c"
    break;

  case 308: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1419 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 5088 "raku.tab.c"
    break;

  case 309: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1420 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5094 "raku.tab.c"
    break;

  case 310: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1421 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5100 "raku.tab.c"
    break;

  case 311: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1422 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5106 "raku.tab.c"
    break;

  case 312: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1423 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5112 "raku.tab.c"
    break;

  case 313: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1424 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5118 "raku.tab.c"
    break;

  case 314: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1425 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5124 "raku.tab.c"
    break;

  case 315: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1427 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 5134 "raku.tab.c"
    break;

  case 316: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1433 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 5144 "raku.tab.c"
    break;

  case 317: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1439 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 5154 "raku.tab.c"
    break;

  case 318: /* cmp_expr: divis_expr  */
#line 1444 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 5160 "raku.tab.c"
    break;

  case 319: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1447 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5166 "raku.tab.c"
    break;

  case 320: /* divis_expr: jct_expr  */
#line 1448 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5172 "raku.tab.c"
    break;

  case 321: /* jct_expr: jct_expr '|' range_expr  */
#line 1451 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5178 "raku.tab.c"
    break;

  case 322: /* jct_expr: jct_expr '&' range_expr  */
#line 1452 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5184 "raku.tab.c"
    break;

  case 323: /* jct_expr: dor_expr  */
#line 1453 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5190 "raku.tab.c"
    break;

  case 324: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1457 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5196 "raku.tab.c"
    break;

  case 325: /* dor_expr: range_expr  */
#line 1458 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 5202 "raku.tab.c"
    break;

  case 326: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1461 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5208 "raku.tab.c"
    break;

  case 327: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1462 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 5214 "raku.tab.c"
    break;

  case 328: /* range_expr: add_expr  */
#line 1463 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 5220 "raku.tab.c"
    break;

  case 329: /* add_expr: add_expr '~' repl_expr  */
#line 1466 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5226 "raku.tab.c"
    break;

  case 330: /* add_expr: repl_expr  */
#line 1467 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 5232 "raku.tab.c"
    break;

  case 331: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1470 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5238 "raku.tab.c"
    break;

  case 332: /* repl_expr: addsub_expr  */
#line 1471 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5244 "raku.tab.c"
    break;

  case 333: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1474 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5250 "raku.tab.c"
    break;

  case 334: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1475 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5256 "raku.tab.c"
    break;

  case 335: /* addsub_expr: mul_expr  */
#line 1476 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 5262 "raku.tab.c"
    break;

  case 336: /* mul_expr: mul_expr '*' unary_expr  */
#line 1479 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5268 "raku.tab.c"
    break;

  case 337: /* mul_expr: mul_expr '/' unary_expr  */
#line 1480 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5274 "raku.tab.c"
    break;

  case 338: /* mul_expr: mul_expr '%' unary_expr  */
#line 1481 "raku.y"
                                  { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5280 "raku.tab.c"
    break;

  case 339: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1482 "raku.y"
                                  { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5286 "raku.tab.c"
    break;

  case 340: /* mul_expr: unary_expr  */
#line 1483 "raku.y"
                                  { (yyval.node)=(yyvsp[0].node); }
#line 5292 "raku.tab.c"
    break;

  case 341: /* unary_expr: '-' unary_expr  */
#line 1486 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 5298 "raku.tab.c"
    break;

  case 342: /* unary_expr: '!' unary_expr  */
#line 1487 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 5304 "raku.tab.c"
    break;

  case 343: /* unary_expr: CARET unary_expr  */
#line 1488 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 5310 "raku.tab.c"
    break;

  case 344: /* unary_expr: OP_REDUCE unary_expr  */
#line 1490 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); free((yyvsp[-1].sval)); (yyval.node)=e; }
#line 5319 "raku.tab.c"
    break;

  case 345: /* unary_expr: pow_expr  */
#line 1494 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 5325 "raku.tab.c"
    break;

  case 346: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1497 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 5331 "raku.tab.c"
    break;

  case 347: /* pow_expr: postfix_expr  */
#line 1498 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 5337 "raku.tab.c"
    break;

  case 348: /* scalar_list: VAR_SCALAR  */
#line 1501 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5343 "raku.tab.c"
    break;

  case 349: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1502 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); free((yyvsp[0].sval)); }
#line 5349 "raku.tab.c"
    break;

  case 350: /* meth_name: IDENT  */
#line 1505 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5355 "raku.tab.c"
    break;

  case 351: /* meth_name: KW_SORT  */
#line 1506 "raku.y"
                 { (yyval.sval)=strdup("sort"); }
#line 5361 "raku.tab.c"
    break;

  case 352: /* meth_name: KW_MAP  */
#line 1507 "raku.y"
                 { (yyval.sval)=strdup("map"); }
#line 5367 "raku.tab.c"
    break;

  case 353: /* meth_name: KW_GREP  */
#line 1508 "raku.y"
                 { (yyval.sval)=strdup("grep"); }
#line 5373 "raku.tab.c"
    break;

  case 354: /* meth_name: KW_SAY  */
#line 1509 "raku.y"
                 { (yyval.sval)=strdup("say"); }
#line 5379 "raku.tab.c"
    break;

  case 355: /* meth_name: KW_PRINT  */
#line 1510 "raku.y"
                 { (yyval.sval)=strdup("print"); }
#line 5385 "raku.tab.c"
    break;

  case 356: /* meth_name: KW_TAKE  */
#line 1511 "raku.y"
                 { (yyval.sval)=strdup("take"); }
#line 5391 "raku.tab.c"
    break;

  case 357: /* meth_name: KW_RETURN  */
#line 1512 "raku.y"
                 { (yyval.sval)=strdup("return"); }
#line 5397 "raku.tab.c"
    break;

  case 358: /* meth_name: KW_EXISTS  */
#line 1513 "raku.y"
                 { (yyval.sval)=strdup("exists"); }
#line 5403 "raku.tab.c"
    break;

  case 359: /* meth_name: KW_DELETE  */
#line 1514 "raku.y"
                 { (yyval.sval)=strdup("delete"); }
#line 5409 "raku.tab.c"
    break;

  case 360: /* meth_name: TESTOP  */
#line 1515 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 5415 "raku.tab.c"
    break;

  case 361: /* postfix_expr: call_expr  */
#line 1517 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 5421 "raku.tab.c"
    break;

  case 362: /* call_expr: IDENT '(' arg_list ')'  */
#line 1520 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5430 "raku.tab.c"
    break;

  case 363: /* call_expr: IDENT '(' ')'  */
#line 1524 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 5436 "raku.tab.c"
    break;

  case 364: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1526 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); free((yyvsp[-3].sval)); }
#line 5442 "raku.tab.c"
    break;

  case 365: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 1528 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); free((yyvsp[-5].sval)); }
#line 5448 "raku.tab.c"
    break;

  case 366: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 1530 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 5457 "raku.tab.c"
    break;

  case 367: /* call_expr: VAR_SCALAR '(' ')'  */
#line 1535 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 5463 "raku.tab.c"
    break;

  case 368: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 1537 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5473 "raku.tab.c"
    break;

  case 369: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 1543 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5483 "raku.tab.c"
    break;

  case 370: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 1549 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); free((yyvsp[-4].sval));
          (yyval.node) = c; }
#line 5491 "raku.tab.c"
    break;

  case 371: /* call_expr: IDENT '.' KW_NEW  */
#line 1553 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5499 "raku.tab.c"
    break;

  case 372: /* call_expr: IDENT '.' IDENT  */
#line 1557 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); free((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5508 "raku.tab.c"
    break;

  case 373: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 1562 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); free((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5519 "raku.tab.c"
    break;

  case 374: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 1569 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); free((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5528 "raku.tab.c"
    break;

  case 375: /* call_expr: IDENT '.' CARET IDENT  */
#line 1574 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); free((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5538 "raku.tab.c"
    break;

  case 376: /* call_expr: atom '.' CARET IDENT  */
#line 1580 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)malloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); free(_m); }
          free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5548 "raku.tab.c"
    break;

  case 377: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 1586 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5559 "raku.tab.c"
    break;

  case 378: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 1593 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 5570 "raku.tab.c"
    break;

  case 379: /* call_expr: atom '.' meth_name '(' ')'  */
#line 1600 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5579 "raku.tab.c"
    break;

  case 380: /* call_expr: atom '.' meth_name  */
#line 1605 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5588 "raku.tab.c"
    break;

  case 381: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 1610 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); free((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 5599 "raku.tab.c"
    break;

  case 382: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 1617 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); free((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 5608 "raku.tab.c"
    break;

  case 383: /* call_expr: call_expr '.' meth_name  */
#line 1622 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = c; }
#line 5617 "raku.tab.c"
    break;

  case 384: /* call_expr: KW_DIE expr  */
#line 1627 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 5623 "raku.tab.c"
    break;

  case 385: /* call_expr: KW_MAP closure expr  */
#line 1629 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5629 "raku.tab.c"
    break;

  case 386: /* call_expr: KW_GREP closure expr  */
#line 1631 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5635 "raku.tab.c"
    break;

  case 387: /* call_expr: KW_SORT expr  */
#line 1633 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5641 "raku.tab.c"
    break;

  case 388: /* call_expr: KW_SORT closure expr  */
#line 1635 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 5647 "raku.tab.c"
    break;

  case 389: /* call_expr: atom  */
#line 1636 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 5653 "raku.tab.c"
    break;

  case 390: /* arg_list: expr  */
#line 1639 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 5659 "raku.tab.c"
    break;

  case 391: /* arg_list: arg_list ',' expr  */
#line 1640 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 5665 "raku.tab.c"
    break;

  case 392: /* arg_list: arg_list ','  */
#line 1641 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 5671 "raku.tab.c"
    break;

  case 393: /* atom: LIT_INT  */
#line 1644 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 5677 "raku.tab.c"
    break;

  case 394: /* atom: LIT_FLOAT  */
#line 1645 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 5683 "raku.tab.c"
    break;

  case 395: /* atom: LIT_STR  */
#line 1646 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 5689 "raku.tab.c"
    break;

  case 396: /* atom: WORDLIST  */
#line 1648 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)malloc(L+1);
            memcpy(tok,w,L); tok[L]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); free(tok); wc++; }
          free((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 5701 "raku.tab.c"
    break;

  case 397: /* atom: LIT_INTERP_STR  */
#line 1655 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 5707 "raku.tab.c"
    break;

  case 398: /* atom: VAR_SCALAR  */
#line 1656 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5713 "raku.tab.c"
    break;

  case 399: /* atom: OP_INC VAR_SCALAR  */
#line 1657 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 5719 "raku.tab.c"
    break;

  case 400: /* atom: OP_DEC VAR_SCALAR  */
#line 1658 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 5725 "raku.tab.c"
    break;

  case 401: /* atom: VAR_SCALAR OP_INC  */
#line 1659 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 5731 "raku.tab.c"
    break;

  case 402: /* atom: VAR_SCALAR OP_DEC  */
#line 1660 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 5737 "raku.tab.c"
    break;

  case 403: /* atom: VAR_ARRAY  */
#line 1661 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5743 "raku.tab.c"
    break;

  case 404: /* atom: VAR_HASH  */
#line 1662 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5749 "raku.tab.c"
    break;

  case 405: /* atom: VAR_CAPTURE  */
#line 1664 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5757 "raku.tab.c"
    break;

  case 406: /* atom: VAR_FH  */
#line 1668 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 5765 "raku.tab.c"
    break;

  case 407: /* atom: VAR_NAMED_CAPTURE  */
#line 1672 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 5772 "raku.tab.c"
    break;

  case 408: /* atom: VAR_ARRAY '[' expr ']'  */
#line 1675 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 5778 "raku.tab.c"
    break;

  case 409: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 1677 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 5784 "raku.tab.c"
    break;

  case 410: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 1679 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 5790 "raku.tab.c"
    break;

  case 411: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 1681 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 5796 "raku.tab.c"
    break;

  case 412: /* atom: ARR_ALL_SLICE  */
#line 1683 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 5802 "raku.tab.c"
    break;

  case 413: /* atom: VAR_HASH '<' IDENT '>'  */
#line 1685 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5808 "raku.tab.c"
    break;

  case 414: /* atom: VAR_HASH '{' expr '}'  */
#line 1687 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5814 "raku.tab.c"
    break;

  case 415: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 1689 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 5820 "raku.tab.c"
    break;

  case 416: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 1691 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 5826 "raku.tab.c"
    break;

  case 417: /* atom: IDENT  */
#line 1692 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 5832 "raku.tab.c"
    break;

  case 418: /* atom: VAR_TWIGIL  */
#line 1694 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5840 "raku.tab.c"
    break;

  case 419: /* atom: VAR_ARRAY_TWIGIL  */
#line 1698 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5848 "raku.tab.c"
    break;

  case 420: /* atom: VAR_HASH_TWIGIL  */
#line 1702 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); free((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 5856 "raku.tab.c"
    break;

  case 421: /* atom: '(' ')'  */
#line 1705 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 5862 "raku.tab.c"
    break;

  case 422: /* atom: '(' expr ')'  */
#line 1706 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 5868 "raku.tab.c"
    break;

  case 423: /* atom: '(' expr ',' ')'  */
#line 1708 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 5874 "raku.tab.c"
    break;

  case 424: /* atom: '(' expr ',' arg_list ')'  */
#line 1710 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 5881 "raku.tab.c"
    break;

  case 425: /* atom: block  */
#line 1712 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5887 "raku.tab.c"
    break;

  case 426: /* atom: KW_SUB block  */
#line 1713 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 5893 "raku.tab.c"
    break;


#line 5897 "raku.tab.c"

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

#line 1715 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
