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

#include "ct_arena.h"
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
    ExprList *l = ct_zalloc(1, sizeof *l);
    if (!l) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    return l;
}
static ExprList *exprlist_append(ExprList *l, tree_t *e) {
    if (l->count >= l->cap) {
        l->cap = l->cap ? l->cap * 2 : 8;
        l->items = ct_grow(l->items, l->cap * sizeof(tree_t *));
        if (!l->items) { fprintf(stderr, "raku: OOM\n"); exit(1); }
    }
    l->items[l->count++] = e;
    return l;
}
static void exprlist_free(ExprList *l) { if (l) { ct_drop(l->items); ct_drop(l); } }
static const char *strip_sigil(const char *s) {
    if (s && (s[0]=='$'||s[0]=='@'||s[0]=='%')) return s+1;
    return s;
}
static int rk_tw_priv(const char *s) { return (s && s[0]=='!') ? 1 : 0; }
static const char *rk_tw_bare(const char *s) { return (s && (s[0]=='.'||s[0]=='!')) ? s+1 : s; }
static tree_t *leaf_sval(tree_e k, const char *s) {
    tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e;
}
#define RK_ARRNAME_MAX 256
static const char *rk_array_names[RK_ARRNAME_MAX];
static int rk_array_names_n = 0;
static void rk_mark_array_name(const char *bare) {
    if (!bare) return;
    for (int i = 0; i < rk_array_names_n; i++) if (!strcmp(rk_array_names[i], bare)) return;
    if (rk_array_names_n < RK_ARRNAME_MAX) rk_array_names[rk_array_names_n++] = intern(bare);
}
int rk_is_array_name(const char *bare) {
    if (!bare) return 0;
    for (int i = 0; i < rk_array_names_n; i++) if (!strcmp(rk_array_names[i], bare)) return 1;
    return 0;
}
const char *rk_arrlit_scalars[RK_ARRNAME_MAX];
int rk_arrlit_scalars_n = 0;
int rk_is_arrlit_scalar(const char *bare) {
    if (!bare) return 0;
    for (int i = 0; i < rk_arrlit_scalars_n; i++) if (!strcmp(rk_arrlit_scalars[i], bare)) return 1;
    return 0;
}
static void rk_mark_arrlit_scalar(const char *bare, const tree_t *rhs) {
    if (!bare || !rhs || rhs->t != TT_FNC || !rhs->v.sval || strcmp(rhs->v.sval, "__rk_arr_lit")) return;
    if (rk_is_arrlit_scalar(bare) || rk_arrlit_scalars_n >= RK_ARRNAME_MAX) return;
    rk_arrlit_scalars[rk_arrlit_scalars_n++] = intern(bare);
}
static const char *rk_var_ident(const char *s) {
    if (s && (s[0] == '@' || s[0] == '%')) return s;
    return strip_sigil(s);
}
static tree_t *var_node(const char *name) {
    const char *id = rk_var_ident(name);
    if (name && name[0] == '@') rk_mark_array_name(id);
    return leaf_sval(TT_VAR, id);
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
    if (!strcmp(s, "subtest")) return "__rk_test_subtest";
    if (!strcmp(s, "is-deeply")) return "__rk_test_is_deeply";
    if (!strcmp(s, "isa-ok")) return "__rk_test_isa_ok";
    if (!strcmp(s, "does-ok")) return "__rk_test_does_ok";
    if (!strcmp(s, "cmp-ok")) return "__rk_test_cmp_ok";
    if (!strcmp(s, "lives-ok")) return "__rk_test_lives_ok";
    if (!strcmp(s, "dies-ok")) return "__rk_test_dies_ok";
    if (!strcmp(s, "throws-like")) return "__rk_test_throws_like";
    if (!strcmp(s, "eval-lives-ok")) return "__rk_test_eval_lives_ok";
    if (!strcmp(s, "eval-dies-ok")) return "__rk_test_eval_dies_ok";
    return "__rk_test_ok";
}
static tree_t *rk_testop_call(const char *name, ExprList *a) {
    tree_t *c = leaf_sval(TT_FNC, name);
    tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name);
    expr_add_child(c, n);
    if (!a) return c;
    if (a->count == 1 && a->items[0] && a->items[0]->t == TT_FNC && a->items[0]->v.sval && !strcmp(a->items[0]->v.sval, "__rk_arr")) {
        tree_t *lst = a->items[0];
        for (int i = 1; i < lst->n; i++) expr_add_child(c, lst->c[i]);
        return c;
    }
    if (a->count == 1 && a->items[0] && a->items[0]->t == TT_FNC && a->items[0]->v.sval && !strcmp(a->items[0]->v.sval, "__rk_pair")
        && name && !strcmp(name, "__rk_test_subtest") && a->items[0]->n >= 3) {
        tree_t *pr = a->items[0];
        for (int i = 1; i < pr->n; i++) expr_add_child(c, pr->c[i]);
        return c;
    }
    for (int i = 0; i < a->count; i++) expr_add_child(c, a->items[i]);
    return c;
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
static tree_t *rk_byref_param(const char *name) {
    tree_t *p = var_node(name); expr_add_child(p, leaf_sval(TT_QLIT, "@")); return p;
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
#define RK_PH_BODY 4
#define RK_PH_DROP (-1)
#define RK_PH_LOOP (-2)
static tree_t *rk_ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
static tree_t *rk_phaser_mark(const char *name, tree_t *body) {
    char buf[64]; snprintf(buf, sizeof buf, "__rk_phaser_%s", name ? name : "");
    tree_t *m = make_call(buf); expr_add_child(m, body ? body : ast_node_new(TT_SEQ_EXPR)); return m;
}
static const char *rk_phaser_of(tree_t *n) {
    if (!n || n->t != TT_FNC || !n->v.sval || strncmp(n->v.sval, "__rk_phaser_", 12) != 0) return NULL;
    return n->v.sval + 12;
}
static tree_t *rk_phaser_body(tree_t *n) { return (n && n->n >= 2) ? n->c[n->n - 1] : ast_node_new(TT_SEQ_EXPR); }
static int rk_phaser_is_loop(const char *p) { return !strcmp(p, "FIRST") || !strcmp(p, "LAST") || !strcmp(p, "NEXT") || !strcmp(p, "once"); }
static int rk_phaser_rank(const char *p, int mainline) {
    if (!strcmp(p, "BEGIN")) return 0;
    if (!strcmp(p, "CHECK")) return 1;
    if (!strcmp(p, "INIT")) return 2;
    if (!strcmp(p, "ENTER") || !strcmp(p, "PRE")) return 3;
    if (!strcmp(p, "LEAVE") || !strcmp(p, "POST")) return 5;
    if (!strcmp(p, "KEEP")) return mainline ? RK_PH_DROP : 6;
    if (!strcmp(p, "UNDO")) return mainline ? 6 : RK_PH_DROP;
    if (!strcmp(p, "END")) return 7;
    if (!strcmp(p, "TEMP")) return RK_PH_DROP;
    if (rk_phaser_is_loop(p)) return RK_PH_LOOP;
    return RK_PH_BODY;
}
static void rk_phaser_unplaced(const char *p) {
    extern void rt_script_die_surface(const char *msg);
    char m[192]; snprintf(m, sizeof m, "%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p);
    rt_script_die_surface(m);
}
static void rk_phaser_inline_tree(tree_t *n) {
    if (!n) return;
    for (int i = 0; i < n->n; i++) {
        const char *p = rk_phaser_of(n->c[i]);
        if (p) { if (rk_phaser_is_loop(p)) rk_phaser_unplaced(p); n->c[i] = rk_phaser_body(n->c[i]); }
        rk_phaser_inline_tree(n->c[i]);
    }
}
static ExprList *rk_phasers_place(ExprList *l, int mainline) {
    int any = 0;
    tree_t *tail_ret = NULL;
    if (!l) return l;
    for (int i = 0; i < l->count; i++) if (rk_phaser_of(l->items[i])) { any = 1; break; }
    if (!any) return l;
    if (!mainline && l->count > 0 && l->items[l->count - 1] && l->items[l->count - 1]->t == TT_RETURN) tail_ret = l->items[--l->count];
    ExprList *out = exprlist_new();
    for (int rank = 0; rank <= 7; rank++) {
        for (int i = 0; i < l->count; i++) {
            tree_t *it = l->items[i];
            const char *p = rk_phaser_of(it);
            int r = p ? rk_phaser_rank(p, mainline) : RK_PH_BODY;
            if (r == RK_PH_DROP) continue;
            if (r == RK_PH_LOOP && !mainline) { if (rank == RK_PH_BODY) exprlist_append(out, it); continue; }
            if (r == RK_PH_LOOP) r = RK_PH_BODY;
            if (r != rank) continue;
            exprlist_append(out, p ? rk_phaser_body(it) : it);
        }
    }
    if (tail_ret) exprlist_append(out, tail_ret);
    exprlist_free(l);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tail_is_statement(int t) {
    switch (t) {
    case TT_RETURN: case TT_NRETURN: case TT_PROC_FAIL: case TT_SAY: case TT_SAY_FH: case TT_PRINT: case TT_PRINT_FH:
    case TT_IF: case TT_UNLESS: case TT_WHILE: case TT_UNTIL: case TT_REPEAT: case TT_FOR: case TT_DO_WHILE: case TT_CLOOP:
    case TT_EVERY: case TT_CASE: case TT_LOOP_BREAK: case TT_LOOP_NEXT: case TT_SUB_DECL: case TT_PROC_DECL:
    case TT_CLASS_DECL: case TT_RECORD_DECL: case TT_DIE: case TT_TRY: case TT_CATCH: case TT_YADA: case TT_SEQ:
    case TT_LABEL_DEF: case TT_STMT: case TT_PROGRAM: case TT_END: case TT_GATHER: case TT_GOTO_S: case TT_GOTO_F:
    case TT_GOTO_U: case TT_GOTO_DIRECT: case TT_GLOBAL: case TT_LOCAL: case TT_STATIC_DECL: case TT_INITIAL:
        return 1;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_rhs_is_aliasable(tree_t *r) {
    if (!r) return 0;
    switch (r->t) {
    case TT_VAR: case TT_ARR_GET: case TT_HASH_GET: case TT_FIELD: case TT_TWIGIL_FIELD: case TT_INDIRECT: return 1;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_bind(tree_t *target, tree_t *rhs) {
    if (rk_rhs_is_aliasable(rhs)) {
        raku_yyerror("bind ':=' whose right side is an lvalue is not implemented: an aggregate here is carried BY VALUE, so there is no container to alias and lowering this to '=' would copy where the program expects a shared container");
        return (tree_t *)0;
    }
    return expr_binary(TT_ASSIGN, target, rhs);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_bind_container(void) {
    raku_yyerror("bind ':=' to an @array or %hash is not implemented: ':=' binds the right side itself, so rakudo prints (1 2 3) for a bound List where '=' prints [1 2 3] for a copied Array -- lowering this to '=' would print a wrong answer where we presently refuse");
    return (tree_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ExprList *rk_tail_value(ExprList *l) {
    if (!l || l->count <= 0) return l;
    { tree_t *last = l->items[l->count - 1];
      if (!last || rk_tail_is_statement(last->t)) return l;
      { tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, last); l->items[l->count - 1] = r; } }
    return l;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_phaser_join(tree_t *a, tree_t *b) {
    if (!a) return b;
    ExprList *l = exprlist_new(); exprlist_append(l, a); exprlist_append(l, b); return make_seq(l);
}
static tree_t *rk_loop_phasers(tree_t *loop, tree_t *body) {
    tree_t *first = NULL, *nxt = NULL, *last = NULL;
    int keep = 0;
    if (!body || body->t != TT_SEQ_EXPR) return loop;
    for (int i = 0; i < body->n; i++) {
        const char *p = rk_phaser_of(body->c[i]);
        if (p && (!strcmp(p, "FIRST") || !strcmp(p, "once"))) first = rk_phaser_join(first, rk_phaser_body(body->c[i]));
        else if (p && !strcmp(p, "NEXT")) nxt = rk_phaser_join(nxt, rk_phaser_body(body->c[i]));
        else if (p && !strcmp(p, "LAST")) last = rk_phaser_join(last, rk_phaser_body(body->c[i]));
        else body->c[keep++] = body->c[i];
    }
    if (keep == body->n) return loop;
    char gn[48]; snprintf(gn, sizeof gn, "__rk_ph_g%d", raku_get_lineno());
    const char *g = intern(gn);
    ExprList *inner = exprlist_new();
    if (first) {
        tree_t *t = ast_node_new(TT_SEQ_EXPR); expr_add_child(t, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(0))); expr_add_child(t, first);
        tree_t *iff = ast_node_new(TT_IF); expr_add_child(iff, leaf_sval(TT_VAR, g)); expr_add_child(iff, t); exprlist_append(inner, iff);
    } else if (last) exprlist_append(inner, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(0)));
    for (int i = 0; i < keep; i++) exprlist_append(inner, body->c[i]);
    if (nxt) exprlist_append(inner, nxt);
    body->n = 0;
    for (int i = 0; i < inner->count; i++) ast_push(body, inner->items[i]);
    exprlist_free(inner);
    if (!first && !last) return loop;
    ExprList *outer = exprlist_new();
    exprlist_append(outer, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(1)));
    exprlist_append(outer, loop);
    if (last) { tree_t *u = ast_node_new(TT_UNLESS); ast_push(u, leaf_sval(TT_VAR, g)); ast_push(u, seq1(last)); exprlist_append(outer, u); }
    return make_seq(outer);
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
static tree_t *rk_tw_field(const char *name) {
    tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *)intern(rk_tw_bare(name)); return fe;
}
static tree_t *rk_tw_post_incdec(const char *var, int add) {
    static int __twpost_uid = 0; char tmp[32]; snprintf(tmp, sizeof tmp, "__twpost_%d", __twpost_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rk_tw_field(var)));
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tw_field(var), expr_binary(add ? TT_ADD : TT_SUB, rk_tw_field(var), one)));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
static ExprList *rk_group_targets(tree_t *grp) {
    ExprList *t = exprlist_new();
    if (grp && grp->t == TT_FNC && grp->v.sval && strcmp(grp->v.sval, "__rk_arr") == 0) { for (int i = 1; i < grp->n; i++) exprlist_append(t, grp->c[i]); }
    else if (grp) { exprlist_append(t, grp); }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
static tree_t *rk_for_multi(ExprList *vars, tree_t *list, tree_t *body) {
    static int __fm_uid = 0; char av[32], iv[32];
    snprintf(av, sizeof av, "__fm_a_%d", __fm_uid); snprintf(iv, sizeof iv, "__fm_i_%d", __fm_uid); __fm_uid++;
    int k = vars ? vars->count : 0;
    tree_t *hoist = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(av)), list);
    tree_t *z = ast_node_new(TT_ILIT); z->v.ival = 0;
    tree_t *init = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), z);
    tree_t *el = make_call("elems"); expr_add_child(el, leaf_sval(TT_VAR, intern(av)));
    tree_t *cond = expr_binary(TT_LT, leaf_sval(TT_VAR, intern(iv)), el);
    tree_t *kk = ast_node_new(TT_ILIT); kk->v.ival = k;
    tree_t *incr = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), expr_binary(TT_ADD, leaf_sval(TT_VAR, intern(iv)), kk));
    tree_t *seq = ast_node_new(TT_SEQ);
    for (int i = 0; i < k; i++) {
        tree_t *idx = leaf_sval(TT_VAR, intern(iv));
        if (i) { tree_t *off = ast_node_new(TT_ILIT); off->v.ival = i; idx = expr_binary(TT_ADD, idx, off); }
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, intern(av))); expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, vars->items[i], get));
    }
    expr_add_child(seq, body);
    if (vars) exprlist_free(vars);
    tree_t *outer = ast_node_new(TT_SEQ);
    expr_add_child(outer, hoist); expr_add_child(outer, rk_cstyle_loop(init, cond, incr, seq));
    return outer;
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
    if (hi && hi->t == TT_VAR && rk_is_array_name(hi->v.sval)) {
        tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, hi); ast_push(el, leaf_sval(TT_QLIT, "elems"));
        hi = el;
    }
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, one));
}
static tree_t *rk_numeric_ctx(tree_t *e) {
    if (e && e->t == TT_VAR && rk_is_array_name(e->v.sval)) {
        tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, e); ast_push(el, leaf_sval(TT_QLIT, "elems"));
        return el;
    }
    return e;
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
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_adhoc_name(const char *nm) { return nm && !strcmp(nm, "X::AdHoc"); }
static tree_t *rk_adhoc_new(const char *cname, ExprList *named, ExprList *pos) {
    if (!rk_is_adhoc_name(cname)) return NULL;
    tree_t *val = NULL;
    if (named) { for (int i = 0; i + 1 < named->count; i += 2) {
            tree_t *k = named->items[i]; const char *ks = k ? k->v.sval : NULL;
            if (ks && (!strcmp(ks, "payload") || !strcmp(ks, "message"))) { val = named->items[i + 1]; break; } } }
    if (!val && pos && pos->count > 0) val = pos->items[0];
    if (!val) { val = ast_node_new(TT_QLIT); val->v.sval = (char *)intern(""); }
    return val;
}
static tree_t *rk_catch_when_cond(tree_t *e) {
    if (e && e->t == TT_VAR && rk_is_adhoc_name(e->v.sval)) return ast_node_new(TT_NUL);
    return e;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_order_call(const char *fn, tree_t *left, tree_t *right) {
    tree_t *c = ast_node_new(TT_FNC); c->v.sval = (char *)intern(fn);
    ast_push(c, leaf_sval(TT_VAR, fn)); ast_push(c, left); ast_push(c, right); return c;
}
static tree_t *rk_interp_primary(const char *s, int *ip, int len) {
    int i = *ip;
    while (i<len && s[i]==' ') i++;
    if (i<len && (s[i]=='$'||s[i]=='@')) {
        i++;
        char nm[256]; int nl=0;
        while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
            { if(nl<255) nm[nl++]=s[i]; i++; }
        nm[nl]='\0'; *ip=i;
        return leaf_sval(TT_VAR,nm);
    }
    if (i<len && s[i]>='0' && s[i]<='9') {
        long v=0;
        while (i<len && s[i]>='0' && s[i]<='9') { v=v*10+(s[i]-'0'); i++; }
        *ip=i;
        tree_t *lit=ast_node_new(TT_ILIT); lit->v.ival=v; return lit;
    }
    *ip=i; return NULL;
}
static tree_t *rk_interp_subexpr(const char *s, int *ip, int len) {
    tree_t *left = rk_interp_primary(s,ip,len);
    if (!left) return NULL;
    int i = *ip;
    while (i<len && s[i]==' ') i++;
    if (i<len && (s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='%')) {
        char op = s[i]; i++; *ip=i;
        tree_t *right = rk_interp_primary(s,ip,len);
        if (!right) return left;
        tree_e k = op=='+'?TT_ADD:op=='-'?TT_SUB:op=='*'?TT_MUL:op=='/'?TT_DIV:TT_MOD;
        return expr_binary(k,left,right);
    }
    *ip=i; return left;
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
        } else if (s[i]=='@' && i+1<len &&
            (s[i+1]=='_'||(s[i+1]>='A'&&s[i+1]<='Z')||(s[i+1]>='a'&&s[i+1]<='z'))) {
            if (litpos>0) { litbuf[litpos]='\0';
                tree_t *lit=leaf_sval(TT_QLIT,litbuf);
                result=result?expr_binary(TT_CAT,result,lit):lit; litpos=0; }
            char vname[256]; int vlen=0; vname[vlen++]=s[i]; i++;
            while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
                { if(vlen<255) vname[vlen++]=s[i]; i++; }
            vname[vlen]='\0';
            tree_t *arrpart;
            if (i<len && s[i]=='[') {
                i++;
                tree_t *idx = rk_interp_subexpr(s,&i,len);
                while (i<len && s[i]!=']') i++;
                if (i<len && s[i]==']') i++;
                arrpart = idx ? rk_arr_index(vname,idx) : leaf_sval(TT_VAR,vname);
            } else { arrpart = leaf_sval(TT_VAR,vname); }
            result=result?expr_binary(TT_CAT,result,arrpart):arrpart;
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

#line 712 "raku.tab.c"

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
  YYSYMBOL_LIT_BOOL = 4,                   /* LIT_BOOL  */
  YYSYMBOL_LIT_FLOAT = 5,                  /* LIT_FLOAT  */
  YYSYMBOL_LIT_STR = 6,                    /* LIT_STR  */
  YYSYMBOL_LIT_INTERP_STR = 7,             /* LIT_INTERP_STR  */
  YYSYMBOL_LIT_REGEX = 8,                  /* LIT_REGEX  */
  YYSYMBOL_LIT_MATCH_GLOBAL = 9,           /* LIT_MATCH_GLOBAL  */
  YYSYMBOL_LIT_SUBST = 10,                 /* LIT_SUBST  */
  YYSYMBOL_VAR_SCALAR = 11,                /* VAR_SCALAR  */
  YYSYMBOL_VAR_ARRAY = 12,                 /* VAR_ARRAY  */
  YYSYMBOL_VAR_HASH = 13,                  /* VAR_HASH  */
  YYSYMBOL_VAR_TWIGIL = 14,                /* VAR_TWIGIL  */
  YYSYMBOL_IDENT = 15,                     /* IDENT  */
  YYSYMBOL_VAR_ARRAY_TWIGIL = 16,          /* VAR_ARRAY_TWIGIL  */
  YYSYMBOL_VAR_HASH_TWIGIL = 17,           /* VAR_HASH_TWIGIL  */
  YYSYMBOL_CARET = 18,                     /* CARET  */
  YYSYMBOL_DOLLAR_LBRACKET = 19,           /* DOLLAR_LBRACKET  */
  YYSYMBOL_VAR_CAPTURE = 20,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 21,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 22,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 23,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 24,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 25,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 26,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 27,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 28,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 29,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 30,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 31,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 32,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 33,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 34,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 35,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 36,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 37,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 38,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 39,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 40,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 41,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 42,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 43,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 44,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 45,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 46,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 47,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 48,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 49,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 50,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 51,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 52,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 53,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 54,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 55,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 56,                   /* KW_SORT  */
  YYSYMBOL_KW_REVERSE = 57,                /* KW_REVERSE  */
  YYSYMBOL_KW_TRY = 58,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 59,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 60,                    /* KW_DIE  */
  YYSYMBOL_KW_FAIL = 61,                   /* KW_FAIL  */
  YYSYMBOL_KW_CLASS = 62,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 63,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 64,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 65,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 66,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 67,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 68,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 69,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 70,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 71,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 72,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 73,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 74,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 75,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 76,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 77,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 78,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 79,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 80,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 81,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 82,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 83,                  /* KW_REGEX  */
  YYSYMBOL_KW_MODULE = 84,                 /* KW_MODULE  */
  YYSYMBOL_PHASER = 85,                    /* PHASER  */
  YYSYMBOL_OP_FATARROW = 86,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 87,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 88,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 89,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 90,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 91,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 92,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 93,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 94,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 95,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 96,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 97,                    /* OP_SLE  */
  YYSYMBOL_OP_SGT = 98,                    /* OP_SGT  */
  YYSYMBOL_OP_SGE = 99,                    /* OP_SGE  */
  YYSYMBOL_OP_CMP3 = 100,                  /* OP_CMP3  */
  YYSYMBOL_OP_CMPG = 101,                  /* OP_CMPG  */
  YYSYMBOL_OP_LEG = 102,                   /* OP_LEG  */
  YYSYMBOL_OP_AND = 103,                   /* OP_AND  */
  YYSYMBOL_OP_OR = 104,                    /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 105,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 106,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 107,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 108,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 109,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 110,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 111,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 112,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 113,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 114,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 115,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 116,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 117,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 118,                   /* OP_DIV  */
  YYSYMBOL_ADV_EXISTS = 119,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 120,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 121,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 122,                   /* OP_SHL  */
  YYSYMBOL_OP_GCD = 123,                   /* OP_GCD  */
  YYSYMBOL_OP_LCM = 124,                   /* OP_LCM  */
  YYSYMBOL_OP_MODW = 125,                  /* OP_MODW  */
  YYSYMBOL_OP_NBAND = 126,                 /* OP_NBAND  */
  YYSYMBOL_OP_UMUL = 127,                  /* OP_UMUL  */
  YYSYMBOL_OP_UDIV = 128,                  /* OP_UDIV  */
  YYSYMBOL_OP_BORT = 129,                  /* OP_BORT  */
  YYSYMBOL_OP_NBOR = 130,                  /* OP_NBOR  */
  YYSYMBOL_OP_QBOR = 131,                  /* OP_QBOR  */
  YYSYMBOL_OP_QBXOR = 132,                 /* OP_QBXOR  */
  YYSYMBOL_OP_UMINUS_I = 133,              /* OP_UMINUS_I  */
  YYSYMBOL_OP_COMPOSE = 134,               /* OP_COMPOSE  */
  YYSYMBOL_OP_COMPOSEU = 135,              /* OP_COMPOSEU  */
  YYSYMBOL_OP_SETINT = 136,                /* OP_SETINT  */
  YYSYMBOL_OP_SETMUL = 137,                /* OP_SETMUL  */
  YYSYMBOL_OP_SETUNI = 138,                /* OP_SETUNI  */
  YYSYMBOL_OP_SETSUM = 139,                /* OP_SETSUM  */
  YYSYMBOL_OP_SETDIF = 140,                /* OP_SETDIF  */
  YYSYMBOL_OP_SETSYM = 141,                /* OP_SETSYM  */
  YYSYMBOL_OP_XORJ = 142,                  /* OP_XORJ  */
  YYSYMBOL_OP_RANGE_XL = 143,              /* OP_RANGE_XL  */
  YYSYMBOL_OP_RANGE_XB = 144,              /* OP_RANGE_XB  */
  YYSYMBOL_OP_BUT = 145,                   /* OP_BUT  */
  YYSYMBOL_OP_DOESW = 146,                 /* OP_DOESW  */
  YYSYMBOL_OP_COLL = 147,                  /* OP_COLL  */
  YYSYMBOL_OP_UNICMP = 148,                /* OP_UNICMP  */
  YYSYMBOL_OP_IDENT3 = 149,                /* OP_IDENT3  */
  YYSYMBOL_OP_EQV = 150,                   /* OP_EQV  */
  YYSYMBOL_OP_BEFORE = 151,                /* OP_BEFORE  */
  YYSYMBOL_OP_AFTER = 152,                 /* OP_AFTER  */
  YYSYMBOL_OP_SETCONT = 153,               /* OP_SETCONT  */
  YYSYMBOL_OP_SETELEM = 154,               /* OP_SETELEM  */
  YYSYMBOL_OP_APPROX = 155,                /* OP_APPROX  */
  YYSYMBOL_OP_SMARTM = 156,                /* OP_SMARTM  */
  YYSYMBOL_OP_NSMARTM = 157,               /* OP_NSMARTM  */
  YYSYMBOL_OP_MINOP = 158,                 /* OP_MINOP  */
  YYSYMBOL_OP_MAXOP = 159,                 /* OP_MAXOP  */
  YYSYMBOL_OP_XOROP = 160,                 /* OP_XOROP  */
  YYSYMBOL_OP_DIVIS = 161,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 162,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 163,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 164,                   /* OP_POW  */
  YYSYMBOL_165_ = 165,                     /* '='  */
  YYSYMBOL_166_ = 166,                     /* '!'  */
  YYSYMBOL_167_ = 167,                     /* '<'  */
  YYSYMBOL_168_ = 168,                     /* '>'  */
  YYSYMBOL_169_ = 169,                     /* '|'  */
  YYSYMBOL_170_ = 170,                     /* '&'  */
  YYSYMBOL_171_ = 171,                     /* '~'  */
  YYSYMBOL_172_ = 172,                     /* '+'  */
  YYSYMBOL_173_ = 173,                     /* '-'  */
  YYSYMBOL_174_ = 174,                     /* '*'  */
  YYSYMBOL_175_ = 175,                     /* '/'  */
  YYSYMBOL_176_ = 176,                     /* '%'  */
  YYSYMBOL_UMINUS = 177,                   /* UMINUS  */
  YYSYMBOL_178_ = 178,                     /* '.'  */
  YYSYMBOL_179_ = 179,                     /* ';'  */
  YYSYMBOL_180_ = 180,                     /* '('  */
  YYSYMBOL_181_ = 181,                     /* ')'  */
  YYSYMBOL_182_ = 182,                     /* ','  */
  YYSYMBOL_183_ = 183,                     /* '['  */
  YYSYMBOL_184_ = 184,                     /* ']'  */
  YYSYMBOL_185_ = 185,                     /* '{'  */
  YYSYMBOL_186_ = 186,                     /* '}'  */
  YYSYMBOL_187_ = 187,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 188,                 /* $accept  */
  YYSYMBOL_program = 189,                  /* program  */
  YYSYMBOL_stmt_list = 190,                /* stmt_list  */
  YYSYMBOL_stmt = 191,                     /* stmt  */
  YYSYMBOL_if_stmt = 192,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 193,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 194,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 195,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 196,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 197,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 198,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 199,                /* loop_incr  */
  YYSYMBOL_for_stmt = 200,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 201,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 202,          /* catch_when_list  */
  YYSYMBOL_when_list = 203,                /* when_list  */
  YYSYMBOL_sub_trait_list = 204,           /* sub_trait_list  */
  YYSYMBOL_sub_decl = 205,                 /* sub_decl  */
  YYSYMBOL_sub_body = 206,                 /* sub_body  */
  YYSYMBOL_method_body = 207,              /* method_body  */
  YYSYMBOL_pkg_name = 208,                 /* pkg_name  */
  YYSYMBOL_class_decl = 209,               /* class_decl  */
  YYSYMBOL_role_decl = 210,                /* role_decl  */
  YYSYMBOL_module_decl = 211,              /* module_decl  */
  YYSYMBOL_is_clauses = 212,               /* is_clauses  */
  YYSYMBOL_class_body_list = 213,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 214,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 215,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 216,           /* named_arg_list  */
  YYSYMBOL_pair_list = 217,                /* pair_list  */
  YYSYMBOL_param_list = 218,               /* param_list  */
  YYSYMBOL_block = 219,                    /* block  */
  YYSYMBOL_closure = 220,                  /* closure  */
  YYSYMBOL_expr = 221,                     /* expr  */
  YYSYMBOL_tern_expr = 222,                /* tern_expr  */
  YYSYMBOL_or_expr = 223,                  /* or_expr  */
  YYSYMBOL_and_expr = 224,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 225,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 226,               /* divis_expr  */
  YYSYMBOL_jct_expr = 227,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 228,                 /* dor_expr  */
  YYSYMBOL_range_expr = 229,               /* range_expr  */
  YYSYMBOL_add_expr = 230,                 /* add_expr  */
  YYSYMBOL_repl_expr = 231,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 232,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 233,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 234,               /* unary_expr  */
  YYSYMBOL_pow_expr = 235,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 236,              /* scalar_list  */
  YYSYMBOL_meth_name = 237,                /* meth_name  */
  YYSYMBOL_postfix_expr = 238,             /* postfix_expr  */
  YYSYMBOL_call_expr = 239,                /* call_expr  */
  YYSYMBOL_arg_list = 240,                 /* arg_list  */
  YYSYMBOL_paren_group = 241,              /* paren_group  */
  YYSYMBOL_atom = 242                      /* atom  */
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

/* The parser invokes alloca or the compile-time arena; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include \"ct_arena.h\"
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
       && ! ((defined YYMALLOC) \
             && (defined YYFREE)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC ct_alloc
#   if 0
void *ct_alloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE ct_drop
#   if 0
void ct_drop(void *); /* INFRINGES ON USER NAME SPACE */
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
#define YYLAST   6978

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  549
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1346

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   420


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
       2,     2,     2,   166,     2,     2,     2,   176,   170,     2,
     180,   181,   174,   172,   182,   173,   178,   175,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   187,   179,
     167,   165,   168,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   183,     2,   184,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   185,   169,   186,   171,     2,     2,     2,
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
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     177
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   737,   737,   748,   749,   752,   754,   756,   758,   760,
     762,   765,   767,   769,   771,   773,   777,   779,   781,   783,
     785,   789,   793,   795,   799,   803,   805,   807,   809,   811,
     813,   817,   821,   823,   825,   827,   829,   831,   833,   835,
     837,   839,   841,   849,   851,   853,   855,   857,   859,   861,
     864,   867,   869,   872,   875,   877,   881,   883,   885,   887,
     889,   892,   895,   897,   900,   903,   906,   908,   913,   917,
     921,   924,   927,   931,   934,   937,   940,   943,   946,   948,
     950,   952,   954,   957,   959,   961,   963,   966,   969,   973,
     976,   978,   980,   982,   985,   988,   991,   994,   997,  1000,
    1003,  1004,  1005,  1006,  1007,  1008,  1009,  1011,  1013,  1017,
    1022,  1026,  1028,  1030,  1032,  1034,  1035,  1036,  1037,  1038,
    1039,  1040,  1042,  1044,  1046,  1048,  1049,  1050,  1051,  1052,
    1055,  1057,  1059,  1061,  1063,  1065,  1067,  1069,  1073,  1075,
    1077,  1079,  1081,  1083,  1087,  1089,  1093,  1095,  1097,  1099,
    1103,  1105,  1109,  1111,  1113,  1117,  1119,  1121,  1125,  1128,
    1134,  1140,  1148,  1154,  1159,  1164,  1172,  1183,  1185,  1189,
    1190,  1196,  1197,  1200,  1208,  1214,  1220,  1228,  1234,  1242,
    1248,  1254,  1263,  1270,  1279,  1288,  1297,  1298,  1301,  1304,
    1306,  1308,  1310,  1312,  1314,  1316,  1318,  1321,  1323,  1325,
    1327,  1330,  1333,  1339,  1340,  1341,  1344,  1347,  1350,  1352,
    1354,  1356,  1358,  1360,  1362,  1364,  1367,  1369,  1371,  1373,
    1376,  1379,  1385,  1388,  1404,  1419,  1434,  1435,  1447,  1461,
    1462,  1465,  1468,  1471,  1474,  1477,  1480,  1483,  1486,  1489,
    1492,  1495,  1498,  1505,  1511,  1517,  1524,  1531,  1538,  1542,
    1546,  1550,  1554,  1561,  1568,  1575,  1582,  1592,  1600,  1608,
    1617,  1624,  1631,  1641,  1649,  1657,  1666,  1675,  1690,  1691,
    1696,  1701,  1708,  1712,  1716,  1723,  1727,  1733,  1735,  1737,
    1739,  1743,  1744,  1745,  1746,  1747,  1748,  1749,  1750,  1751,
    1752,  1753,  1754,  1755,  1756,  1757,  1758,  1759,  1760,  1761,
    1762,  1763,  1764,  1765,  1766,  1769,  1770,  1771,  1773,  1775,
    1777,  1779,  1781,  1784,  1786,  1788,  1790,  1792,  1795,  1798,
    1802,  1805,  1808,  1811,  1813,  1816,  1819,  1822,  1825,  1828,
    1830,  1832,  1834,  1836,  1838,  1840,  1842,  1844,  1848,  1851,
    1852,  1853,  1854,  1855,  1856,  1857,  1858,  1863,  1868,  1870,
    1873,  1875,  1878,  1879,  1880,  1881,  1882,  1885,  1886,  1889,
    1890,  1891,  1892,  1893,  1894,  1895,  1896,  1897,  1898,  1899,
    1900,  1901,  1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,
    1910,  1911,  1912,  1918,  1924,  1930,  1936,  1939,  1940,  1943,
    1944,  1945,  1946,  1947,  1948,  1949,  1950,  1951,  1954,  1956,
    1959,  1960,  1961,  1962,  1963,  1964,  1965,  1968,  1969,  1970,
    1971,  1974,  1975,  1976,  1979,  1980,  1981,  1982,  1983,  1984,
    1985,  1986,  1989,  1990,  1991,  1992,  1993,  1994,  1995,  1996,
    1997,  1999,  2001,  2003,  2005,  2008,  2009,  2010,  2011,  2012,
    2017,  2020,  2021,  2024,  2025,  2028,  2029,  2030,  2031,  2032,
    2033,  2034,  2035,  2036,  2037,  2038,  2039,  2040,  2042,  2044,
    2050,  2055,  2056,  2058,  2060,  2065,  2067,  2076,  2085,  2092,
    2096,  2101,  2108,  2113,  2119,  2125,  2132,  2139,  2144,  2149,
    2156,  2161,  2166,  2173,  2180,  2187,  2192,  2197,  2199,  2201,
    2203,  2205,  2207,  2209,  2211,  2214,  2215,  2216,  2219,  2220,
    2221,  2223,  2228,  2229,  2233,  2234,  2235,  2244,  2245,  2246,
    2247,  2248,  2249,  2250,  2251,  2252,  2253,  2254,  2258,  2262,
    2265,  2267,  2269,  2271,  2273,  2275,  2277,  2279,  2281,  2283,
    2285,  2287,  2289,  2291,  2292,  2296,  2300,  2304,  2305,  2307,
    2309,  2312,  2313,  2315,  2317,  2320,  2321,  2322,  2323,  2326
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
  "\"end of file\"", "error", "\"invalid token\"", "LIT_INT", "LIT_BOOL",
  "LIT_FLOAT", "LIT_STR", "LIT_INTERP_STR", "LIT_REGEX",
  "LIT_MATCH_GLOBAL", "LIT_SUBST", "VAR_SCALAR", "VAR_ARRAY", "VAR_HASH",
  "VAR_TWIGIL", "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL", "CARET",
  "DOLLAR_LBRACKET", "VAR_CAPTURE", "VAR_FH", "VAR_NAMED_CAPTURE",
  "KW_USE", "TESTOP", "KW_MY", "KW_SAY", "KW_PRINT", "KW_IF", "KW_ELSE",
  "KW_ELSIF", "KW_WHILE", "KW_FOR", "KW_SUB", "KW_GATHER", "KW_TAKE",
  "KW_RETURN", "KW_EXIT", "KW_CONSTANT", "KW_ENUM", "KW_JOIN", "KW_GIVEN",
  "KW_WHEN", "KW_DEFAULT", "KW_WITH", "KW_WITHOUT", "KW_EXISTS",
  "KW_DELETE", "KW_UNLESS", "KW_UNTIL", "KW_REPEAT", "KW_LOOP", "KW_LAST",
  "KW_NEXT", "KW_MAP", "KW_GREP", "KW_SORT", "KW_REVERSE", "KW_TRY",
  "KW_CATCH", "KW_DIE", "KW_FAIL", "KW_CLASS", "KW_METHOD", "KW_HAS",
  "KW_NEW", "KW_ROLE", "KW_MULTI", "KW_PROTO", "OP_NAME", "OP_REDUCE",
  "ARR_ALL_SLICE", "SLURPY_POS", "SLURPY_LOL", "SLURPY_NAMED",
  "KW_HANDLES", "WORDLIST", "OP_COLON_D", "OP_COLON_U", "YADA",
  "KW_GRAMMAR", "KW_TOKEN", "KW_RULE", "KW_REGEX", "KW_MODULE", "PHASER",
  "OP_FATARROW", "OP_RANGE", "OP_RANGE_EX", "OP_ARROW", "OP_EQ", "OP_NE",
  "OP_LE", "OP_GE", "OP_SEQ", "OP_SNE", "OP_SLT", "OP_SLE", "OP_SGT",
  "OP_SGE", "OP_CMP3", "OP_CMPG", "OP_LEG", "OP_AND", "OP_OR",
  "OP_TERNARY1", "OP_TERNARY2", "OP_BIND", "OP_DOTEQ", "OP_SMATCH",
  "OP_INC", "OP_DEC", "OP_ADD_EQ", "OP_SUB_EQ", "OP_MUL_EQ", "OP_DIV_EQ",
  "OP_CAT_EQ", "OP_DOR", "OP_DIV", "ADV_EXISTS", "ADV_DELETE", "OP_BAND",
  "OP_SHL", "OP_GCD", "OP_LCM", "OP_MODW", "OP_NBAND", "OP_UMUL",
  "OP_UDIV", "OP_BORT", "OP_NBOR", "OP_QBOR", "OP_QBXOR", "OP_UMINUS_I",
  "OP_COMPOSE", "OP_COMPOSEU", "OP_SETINT", "OP_SETMUL", "OP_SETUNI",
  "OP_SETSUM", "OP_SETDIF", "OP_SETSYM", "OP_XORJ", "OP_RANGE_XL",
  "OP_RANGE_XB", "OP_BUT", "OP_DOESW", "OP_COLL", "OP_UNICMP", "OP_IDENT3",
  "OP_EQV", "OP_BEFORE", "OP_AFTER", "OP_SETCONT", "OP_SETELEM",
  "OP_APPROX", "OP_SMARTM", "OP_NSMARTM", "OP_MINOP", "OP_MAXOP",
  "OP_XOROP", "OP_DIVIS", "OP_REP_X", "OP_REP_XX", "OP_POW", "'='", "'!'",
  "'<'", "'>'", "'|'", "'&'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "'.'", "';'", "'('", "')'", "','", "'['", "']'", "'{'", "'}'",
  "':'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
  "elsif_tail", "while_stmt", "unless_stmt", "until_stmt", "repeat_stmt",
  "loop_stmt", "loop_incr", "for_stmt", "given_stmt", "catch_when_list",
  "when_list", "sub_trait_list", "sub_decl", "sub_body", "method_body",
  "pkg_name", "class_decl", "role_decl", "module_decl", "is_clauses",
  "class_body_list", "grammar_decl", "grammar_body_list", "named_arg_list",
  "pair_list", "param_list", "block", "closure", "expr", "tern_expr",
  "or_expr", "and_expr", "cmp_expr", "divis_expr", "jct_expr", "dor_expr",
  "range_expr", "add_expr", "repl_expr", "addsub_expr", "mul_expr",
  "unary_expr", "pow_expr", "scalar_list", "meth_name", "postfix_expr",
  "call_expr", "arg_list", "paren_group", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-990)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-547)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -990,    18,  2838,  -990,  -990,  -990,  -990,  -990,  -990,   653,
     206,    -8,   -23,    25,  -990,  -990,  6702,  2925,  -990,  -990,
    -990,    42,  3060,    51,  5283,  5418,  5497,  5632,  5711,    -6,
     -22,  5711,  3139,  3274,    30,   161,  5711,  5711,   171,   194,
    5846,  5925,   -30,   387,   193,   231,    76,    76,  6060,  5711,
     -30,   102,  5711,  3353,   295,   295,   248,  6702,  -990,  -990,
     295,   295,   -30,   282,   324,   345,  6702,  6702,  6702,   952,
    -990,  3488,  3567,   297,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  6802,   342,
     274,   349,   289,   628,   754,   736,   287,   899,   377,   428,
     696,   795,  -990,  -990,   249,   241,   304,   269,  5711,   410,
    -990,  -990,  5711,  5711,  5711,  5711,  5711,  5711,  3702,  5711,
    6139,  3781,  5711,   523,  5711,  -990,  -990,  5711,   392,    43,
    1164,   158,   418,   118,   505,   -35,   432,  -990,  -990,   397,
    -990,   485,   724,   151,  -990,   443,  3916,  -990,  3995,  -990,
     -58,   223,   336,   430,   665,   630,   211,   282,  3488,   548,
    3488,   176,  3488,   -30,  3488,   -30,   242,   385,   448,   611,
    -990,  -990,  -990,   162,  -990,   278,  -990,   281,   585,   597,
     626,   532,   647,   140,   292,  3488,   -30,  3488,   -30,   461,
    4131,  -990,  5711,  5711,  -990,  5711,  5711,  -990,  5711,  4210,
    5711,  4345,  5711,  -990,  -990,   784,   424,  -990,  -990,  -990,
     641,  -990,  -990,   660,   678,    40,  -990,   685,   697,   708,
    -990,   320,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,
     723,  -990,   550,  -990,   457,   709,  1801,  5711,  5711,  5711,
    5711,  5711,  5711,  5711,  5711,  -990,  5711,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,   686,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,
    6702,  6702,  6702,  6702,  6702,  6702,  6702,  6702,   952,  5711,
    1521,   758,   731,   769,   773,   783,   793,   806,   812,  -990,
     799,   821,  3488,   531,   770,   504,   877,   837,   878,   907,
    -990,   911,  1099,   920,   -42,  -990,  1108,   829,   840,  3781,
    1122,  5711,   952,  1521,  5711,  5711,  5711,  5711,  5711,  5711,
    5711,  4424,  -990,  -990,   995,  1034,   550,   863,  -990,  5711,
    5711,  5711,  -990,  5711,  6274,  -990,  5711,  6353,  -990,   442,
      16,   168,   617,   979,  1044,   869,   889,  5711,  5711,  5711,
    5711,  5711,  5711,  5711,  -990,  5711,   897,  5711,  5711,  -990,
    5711,   951,  1110,   961,  -990,   282,  5711,  -990,  6702,  6702,
       5,  -990,  -990,    99,  -990,   271,  -990,  -990,  -990,   965,
    -990,  5711,  5711,  5711,  -990,  5711,  5711,  -990,  5711,  5711,
    1054,  5711,  -990,  1208,  5711,  1212,  5711,   967,  1218,   970,
    -990,  5711,  5711,  1230,  1106,  1117,  1118,  1119,  1120,  1084,
    5711,  -990,  -990,  -990,   -30,  5711,   -30,    48,  -990,     4,
    -990,    21,  1079,  1121,  -990,  -990,  -990,  1289,  -990,  4559,
    -990,  4638,  4774,  -990,  -990,   296,   208,   429,  5283,  5418,
    5711,  2440,    32,    53,  -990,   450,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1134,  1135,  1136,  -990,   289,  1198,   289,
     289,   289,   628,  1155,  1155,  1155,  1155,  1155,  1155,  1155,
    1155,  1155,  1155,  1155,  1155,  1155,  1155,  1155,  1155,  1155,
    1155,  1155,  1155,  1155,  1155,  1155,  -990,  -990,  -990,  -990,
     736,   899,   899,   899,   899,   899,   899,   899,   899,   899,
     449,   449,   449,   449,   449,   449,   428,   428,   428,   696,
     696,   795,   795,   795,   795,   795,   795,   795,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,   -82,  1138,  1303,   229,  -990,  -990,  4853,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,   972,  -990,  5711,  5711,
    5711,  5711,  1154,  -990,   484,   486,  -990,  4989,  -990,  1350,
    5711,  1140,  -990,    -7,  -990,  1616,   606,  1153,  1137,   -95,
      73,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,   715,
    -990,  -990,  1143,  -990,  1145,  1146,  1147,  3488,   565,  1148,
    1242,   141,  5068,   591,  1150,  5711,  5711,  -990,  6488,  -990,
    5711,  -990,    56,  -990,  5711,  5711,  1166,  4638,  1156,  1159,
    1160,  1161,  1162,  1167,  1169,   640,  4638,  1171,  1173,   730,
     -30,    58,  6567,  -990,   -30,   320,   250,   280,   382,   -10,
     974,  1985,  1317,  5711,   107,  1322,  1323,   -30,   792,   778,
    1179,  1180,  1181,  1194,  1195,  1199,  -990,  1193,    84,  1175,
    1191,  1211,  1200,   -30,   -30,   -30,  1206,  1209,  1226,  5711,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,   -30,  1201,  5711,
     -30,  -990,  1377,  1379,  -990,    33,  1210,   976,    61,   611,
     270,  2169,  -990,  -990,   978,  -990,   980,  -990,   937,  3781,
    5711,  5711,   452,    54,   368,  -990,   104,  5711,  5711,  -990,
    5711,  5711,  -990,  5711,  5711,  5711,  5711,  5711,  5711,  5711,
    5711,  -990,   952,  1521,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  6702,  5711,  5204,  5711,  -990,  -990,  5711,  1431,
    5711,  1219,   982,  4638,   841,  1213,  1215,   938,  5711,  -990,
    -990,  5711,  -990,  -990,  5711,  -990,   991,  -990,  1006,  1008,
    -990,  5711,  1314,  1386,  1010,  -990,  1074,  1076,  -990,  -990,
    -990,  -990,  -990,  1018,  -990,  5711,  -990,  5711,  5711,  1020,
    -990,   134,  -990,  1223,  1224,  3488,   876,  1232,  1210,  1024,
    1234,  1235,  5711,  1026,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  1030,  -990,  -990,  -990,  1196,  -990,  -990,  3488,
     -30,  -990,  -990,  1398,  -990,  1404,  1405,  1402,    -3,  -990,
     -10,  5283,  5418,  2575,  -990,   514,  -990,  -990,  1403,  5711,
    -990,  -990,  -990,   113,  -990,   474,  -990,  -990,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,  5711,   -30,  -990,  -990,
    -990,  1240,  1243,  1394,  -990,  -990,  -990,  -990,  5711,  1245,
    -990,  -990,   -30,  1239,  -990,  -990,   106,   909,  1046,  1364,
    -990,  -990,  1210,  1210,  1048,  1050,  1413,  1414,  1415,  -990,
    -990,  -990,  -990,  -990,   954,  1246,   116,  5711,  5711,  5711,
    5711,  5711,  5711,  -990,  -990,  -990,  -990,   175,   327,   451,
     471,   483,   487,   488,   503,   518,   520,   526,   547,   467,
     500,  -990,  1252,  -990,  1055,  1193,  1254,  -990,  1057,  1062,
    1193,  -990,  1261,  1064,  -990,  -990,  -990,  -990,  1262,  1275,
    1276,  -990,  -990,  -990,  1277,  5711,  1279,  -990,  4638,   887,
    -990,  -990,  1278,  1370,  1376,  -990,  -990,  1075,  -990,  5711,
    -990,  -990,  1210,  -990,  -990,   898,  1284,  1287,    58,  -990,
    1080,  1222,   -30,   -30,   -30,  -990,  1452,  -990,    -3,  -990,
     529,   164,  -990,   260,  5711,  5711,  5711,  5711,  5711,  5711,
    5711,  5711,  -990,  -990,  -990,  1453,  5711,   115,  1458,  1459,
     -30,  1286,  -990,  -990,   -30,  1294,  5711,  -990,  -990,  -990,
     661,   677,    41,   126,  1073,  1295,  1296,  1070,  -990,  -990,
    1210,  1210,  1468,  1470,  1471,  1315,   491,  -990,   549,   557,
     605,   608,   614,   619,  -990,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990,  5711,  5711,  -990,  -990,
    -990,  -990,  -990,  -990,  1302,  -990,  -990,  -990,  -990,  -990,
    5711,  1082,  -990,  -990,  5711,  5711,  4638,   925,  -990,  -990,
    5711,  -990,  -990,  -990,  -990,   -30,   -30,  -990,  -990,  -990,
    -990,  -990,  -990,  5711,  5711,  5711,  -990,  -990,  -990,   622,
     631,   635,   645,   692,   700,   702,   704,  -990,  -990,  1467,
    5711,  -990,  -990,  -990,  -990,  -990,  5711,  1308,  -990,    92,
    1411,  -990,   136,  -990,  1477,  1478,  1316,  5711,  -990,  1479,
    1481,  1318,  5711,  -990,   163,   247,  1319,  1320,  -990,  -990,
    1324,   713,  -990,  -990,  -990,  -990,  -990,  5711,  5711,  -990,
    -990,  -990,  -990,  -990,  -990,   706,   722,  -990,  1325,  1321,
    -990,  -990,  1085,  -990,   936,  1225,  -990,   728,   752,   753,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,  -990,
    1326,   -30,  1327,  1087,  1328,  2353,  1327,  1091,  1329,  1330,
    -990,  1331,   325,   435,  -990,  1332,  1488,  1498,  1336,  5711,
    -990,  1502,  1503,  1340,  5711,  -990,  -990,  -990,   142,   156,
    -990,   755,   761,  -990,  -990,  -990,  -990,  1342,  -990,   -30,
    -990,  -990,  -990,  -990,  5711,  -990,  -990,  1327,  -990,   477,
    5283,  5418,  2654,  -990,   537,  -990,  1327,  -990,  -990,  -990,
    5711,  -990,  5711,  -990,  -990,  1345,  1346,  -990,  1347,  1348,
    1353,  -990,  1355,  1327,  1095,  1327,  1097,  -990,  -990,  -990,
    -990,  1357,  -990,  5711,   627,   266,  -990,   276,  5711,  5711,
    5711,  5711,  5711,  5711,  5711,  5711,  -990,  -990,  1361,  1365,
    -990,  -990,  -990,  -990,  -990,  -990,  -990,  1327,  -990,  1327,
     -30,   767,  5711,  5711,  5711,  -990,  -990,  -990,   772,   775,
     789,   798,   803,   804,   807,   815,  -990,  -990,  -990,  -990,
    -990,  -990,   816,   817,   818,  -990,  -990,  -990,  -990,  -990,
    -990,  -990,  -990,  -990,  -990,  -990
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   502,   503,   504,   505,   507,   508,
     515,   516,   534,   533,   535,   536,     0,     0,   517,   518,
     519,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   524,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     3,     4,   102,   103,   115,   116,   117,
     118,   104,   105,   125,   126,   127,   129,   128,   112,     0,
     349,   351,   356,   358,   386,   388,   397,   399,   406,   410,
     413,   421,   434,   440,   442,   458,   545,   494,     0,     0,
     511,   512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   513,   514,     0,     0,     0,
       0,   508,   515,   516,   534,   533,     0,   546,   438,   458,
     545,   494,   508,   515,   541,     0,     0,    43,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,     0,     0,
     547,   347,   346,     0,    57,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   152,
       0,   155,     0,     0,   119,     0,     0,   120,     0,     0,
       0,     3,     0,   491,   493,   106,     3,   111,   487,    59,
       0,   222,   226,     0,     0,     0,   439,     0,     0,   113,
     443,     0,   509,   510,   437,   436,   435,   445,   457,   450,
     451,   452,   453,   456,   454,   455,   448,   449,   446,   447,
     486,   498,     0,   537,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   341,   342,   343,   344,   345,   339,   465,
       0,     0,     0,   340,     0,     0,     0,     0,     0,     0,
      47,   470,     0,   469,   533,   461,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   542,    36,     0,   498,     0,     0,    46,   497,
       0,     0,     6,     0,     0,    17,     0,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    48,     0,     0,     0,     0,    51,
       0,     0,   133,     0,   145,     0,     0,   164,     0,     0,
       0,     3,   175,   281,   286,     0,   299,   301,   303,     0,
      54,     0,     0,     0,    56,     0,     0,    62,     0,     0,
       0,     0,   169,     0,     0,     0,     0,     0,   148,     0,
     151,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   488,   490,   492,     0,     0,     0,     0,    58,     0,
     229,     0,     0,     0,   268,     3,   114,     0,   549,     0,
     499,     0,     0,   538,   306,   515,   516,   534,     0,     0,
       0,     0,     0,     0,   305,     0,   458,   494,     0,     0,
       0,     0,     0,     0,     0,     0,   348,   352,     0,   355,
     354,   353,   357,   359,   368,   371,   372,   367,   366,   365,
     364,   363,   362,   361,   360,   369,   370,   373,   377,   378,
     379,   380,   381,   374,   375,   376,   382,   383,   384,   385,
     387,   395,   394,   393,   392,   391,   390,   389,   396,   398,
     404,   403,   402,   401,   400,   405,   409,   408,   407,   411,
     412,   419,   418,   417,   416,   415,   414,   420,   430,   432,
     433,   431,   427,   426,   425,   424,   423,   422,   428,   429,
     441,   481,     0,     0,   478,    11,    69,     0,    95,    96,
      97,    98,    99,    66,   464,    12,     0,    22,     0,     0,
       0,     0,   520,    13,   525,   526,    72,     0,   473,     0,
       0,   274,   462,     0,   460,   497,     0,     0,     0,   481,
     478,   341,   342,   343,   344,   345,   339,   340,   543,     0,
      37,    45,     0,   496,     0,     0,     0,     0,     0,     0,
     505,   533,     0,     0,     0,     0,     0,    33,     0,    34,
       0,    35,     0,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     499,     0,     0,   136,   499,     0,     0,   400,   405,     0,
       0,     0,     0,     0,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,   459,     0,     0,
       0,     0,     0,   499,     0,   499,     0,     0,     0,     0,
     121,   122,   123,   124,   338,   489,   107,     0,     0,     0,
       0,   108,     0,     0,   229,     0,     0,     0,     0,     0,
       0,     0,   444,   485,     0,   500,     0,   539,     0,     0,
       0,     0,     0,     0,     0,   331,     0,     0,     0,   332,
       0,     0,   333,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,     0,    78,    80,    82,    85,    83,    84,
      79,    81,     0,     0,     0,     0,    16,   474,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   530,
     532,     0,   529,   531,     0,   472,     0,   468,     0,     0,
     272,     0,     0,     0,     0,   520,   525,   526,   544,    44,
       7,     5,     8,     0,    19,     0,     9,     0,     0,     0,
      26,     0,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    89,    88,    92,    90,    91,
      87,    49,     0,    93,    94,    52,   130,   135,   134,     0,
       0,   144,   161,     0,   163,     0,     0,     0,     0,   174,
       0,     0,     0,     0,   186,     0,   282,   295,     0,     0,
     289,   290,   548,   291,   287,     0,   300,   302,   304,    55,
      60,    61,    64,    65,    39,    38,     0,     0,   165,   527,
     528,     0,     0,   146,   149,   150,   153,   154,     0,     0,
     167,   110,     0,     0,   227,   228,     0,     0,     0,     0,
     224,   185,     0,     0,     0,     0,     0,     0,     0,   267,
     225,   484,   501,   540,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   316,   323,   329,   330,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
     478,   350,     0,   480,     0,   483,     0,   477,     0,     0,
     482,    68,     0,     0,    23,   523,   522,   521,     0,     0,
       0,   471,   466,   467,     0,     0,     0,   463,     0,     0,
     278,   277,     0,     0,     0,    10,    28,     0,    29,     0,
      32,   179,     0,    41,    40,     0,   501,   501,     0,   137,
       0,   138,     0,     0,     0,   171,     0,   177,     0,   173,
       0,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   187,   284,   297,     0,     0,   292,     0,     0,
       0,     0,    76,    77,     0,     0,     0,   168,   109,   223,
       0,     0,     0,     0,     0,     0,     0,     0,   184,   182,
       0,     0,     0,     0,     0,   520,   526,   326,     0,     0,
       0,     0,     0,     0,   334,   335,   336,   337,   308,   310,
     312,   315,   313,   314,   309,   311,     0,     0,    70,   479,
      71,   476,   475,    67,   501,    73,    74,    75,   273,   275,
       0,     0,    20,    27,     0,     0,     0,     0,   178,    14,
       0,    50,    53,   132,   131,   499,     0,   142,   162,   159,
     160,   172,   176,     0,     0,     0,   190,   191,   188,     0,
       0,     0,     0,     0,     0,     0,     0,   283,   296,     0,
       0,   293,   294,   170,   166,   147,     0,     0,   158,     0,
       3,   261,     0,   258,     0,     0,     0,     0,   235,     0,
       0,     0,     0,   230,     0,     0,     0,     0,   231,   232,
       0,     0,   181,   183,   269,   270,   271,     0,     0,   317,
     319,   322,   320,   321,   318,     0,     0,    24,     0,   501,
     280,   279,     0,    30,     0,   139,   140,     0,     0,     0,
     192,   194,   196,   199,   197,   198,   193,   195,   285,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     248,     0,     0,     0,   249,     0,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   236,   233,   234,     0,     0,
     264,     0,     0,   324,   325,   276,    21,   501,    15,     0,
     143,   200,   202,   201,     0,   157,   260,     0,   204,   534,
       0,     0,     0,   203,     0,   257,     0,   246,   247,   239,
       0,   242,     0,   245,   238,     0,     0,   250,     0,     0,
       0,   251,     0,     0,     0,     0,     0,   327,   328,    31,
     141,     0,   259,     0,     0,     0,   208,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   206,   256,     0,     0,
     254,   255,   241,   252,   253,   240,   266,     0,   263,     0,
       0,     0,     0,     0,     0,   209,   210,   207,     0,     0,
       0,     0,     0,     0,     0,     0,   243,   244,   265,   262,
     156,   205,     0,     0,     0,   211,   213,   215,   218,   216,
     217,   212,   214,   219,   221,   220
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -990,  -990,     1,  -990,  -666,  -840,  -990,  -990,  -990,  -990,
    -990,   285,  1513,  -990,  -990,  -990,   689,  -990,  -390,  -989,
     -12,  -990,  -990,  -990,  -990,   822,  -990,  -990,  -556,   908,
    -417,   378,  1236,    -2,  -254,  -990,   842,  1290,  1465,  1264,
    -990,   737,    -1,   826,   988,   727,   209,  -990,  -150,  -288,
    -990,     9,   293,    10,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   246,    74,    75,   673,    76,    77,    78,    79,
      80,  1137,    81,    82,   467,   698,   858,    83,   422,  1141,
     212,    84,    85,    86,   469,   725,    87,   730,   357,   643,
     429,   137,   199,   149,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   221,   240,
     104,   139,   150,   140,   141
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,     2,   653,   680,   508,   847,   999,   395,   802,   168,
      28,   105,   106,   107,   857,   145,   423,   424,     3,   722,
     425,  1006,   159,   161,   163,   165,   166,   167,   723,   173,
     175,   177,   423,   424,   181,   182,   425,   128,   186,   188,
     581,   178,   584,   213,   610,   179,   203,   204,   217,   218,
     208,   210,  1143,   798,   727,   472,  1144,   146,   351,   804,
     747,   352,   151,   152,   153,  1145,   154,   423,   424,   242,
     244,   425,   423,   424,   619,   620,   425,   426,   427,   428,
     748,   750,   407,   773,   155,   774,    26,   125,   126,   156,
     719,   720,   775,   426,   427,   428,   907,   908,   774,   122,
     909,   751,   408,   423,   424,   775,   331,   425,   353,   473,
     333,   334,   335,   336,   337,   338,  1146,   341,   343,   345,
     346,   378,   348,   682,   379,   349,   886,   887,   426,   427,
     428,   868,   432,   426,   427,   428,   129,  1025,   130,  1129,
     983,  1149,   127,   129,   374,   130,   376,   423,   424,   984,
    1150,   425,   433,   423,   424,    73,   396,   425,   406,   123,
     411,  1107,   413,    73,   426,   427,   428,   423,   424,   907,
     908,   425,  1230,   909,   169,   421,   180,   124,  1216,    73,
     803,   648,   421,   447,   183,   449,   679,  1217,   454,   724,
     455,   456,   407,   457,   458,   649,   459,   461,   462,   459,
     463,  1151,   726,   129,   407,   130,  1147,   184,   426,   427,
     428,   194,   408,  1246,   426,   427,   428,  1255,   749,   910,
    1148,   192,   393,   958,   408,   138,   394,   818,   426,   427,
     428,   157,   197,   409,   721,   829,   410,   828,  1218,   752,
     934,   193,   913,    73,   495,   498,   499,   500,   501,   502,
     503,   504,   505,   779,   506,   496,   106,   497,  1282,   195,
     780,   198,  1221,   214,   683,   675,   216,  1297,   110,   111,
     888,  1222,   869,  1202,   539,   224,   225,   226,  1026,   196,
    1130,   215,   684,   434,  1306,   360,  1308,   206,   432,   859,
     936,  1152,  1039,   220,   407,   606,   550,   551,   552,   553,
     554,   555,  1057,   361,   432,  1153,   432,   443,   433,   435,
     211,   914,   915,   119,   408,   122,   370,  1206,  1328,   129,
    1329,   130,  1223,  1273,   433,   444,   433,   582,  1219,   436,
     380,   415,  1103,   650,   359,   222,   911,  1275,   118,   853,
     596,   430,  1220,   409,   431,  1240,   410,   651,   685,   686,
    1117,   916,   917,   918,   710,   409,   223,   616,   410,   618,
     256,  1064,   621,   622,   623,   624,   625,   626,   627,   855,
     247,   120,   194,   248,   249,   123,   245,   633,   634,   635,
      88,   636,   638,   250,   639,   644,   251,   252,   381,   121,
     253,   254,   262,   740,   778,   658,   659,   660,   661,   662,
     663,   664,   382,   119,   296,   667,   668,   170,   172,   779,
     197,   340,  1224,   327,   303,   304,   780,   677,   678,   328,
     189,   191,   681,   358,   416,   332,  1225,    73,   205,   207,
     690,   691,   379,   692,   693,    73,   694,   695,   991,   434,
     219,   377,   700,   383,   702,   409,  1118,   330,   410,   706,
     707,   305,  1316,   257,   258,   434,   919,   434,   715,   445,
     437,   120,  1317,   717,   301,   302,   465,   466,  1007,   329,
    1009,   856,   418,   419,   949,   950,   731,   446,   753,   739,
     927,   754,   755,   398,   928,  1027,   742,   743,   744,   746,
    1260,   756,   451,   929,   757,   758,   930,   931,   759,   760,
     932,   384,   477,   245,  1261,    73,   711,   259,   260,   261,
     452,   303,   304,  1065,   170,   385,   303,   304,   951,   303,
     304,   255,  1048,  1049,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   386,   347,   125,
     126,   412,  1014,   414,   417,  1015,  1016,   430,   305,   645,
     431,  1028,  1029,   305,   935,  1017,   305,  1113,  1018,  1019,
     398,  1114,  1020,  1021,   448,  1288,   450,   190,  1289,  1290,
     400,   350,    73,   401,   402,   362,   397,  1115,  1291,   398,
     399,  1292,  1293,   303,   304,  1294,  1295,   125,   126,   400,
     306,   307,   401,   402,   741,   387,   403,   785,   786,   478,
    1262,   359,  1098,   789,   790,   792,   793,   646,   800,   388,
     792,   793,   169,   633,  1263,   125,   126,    73,  1112,  1041,
     305,   647,   423,   424,    88,   371,   425,   372,   420,   255,
     712,   404,  1076,   421,   405,   813,   761,  1066,   933,   482,
     242,   483,  1283,   823,   824,   392,   826,   774,   827,   791,
     713,   794,   830,   831,   775,  1312,  1168,  1067,   398,  1313,
    1162,  1163,   764,   363,   629,  1077,   765,   766,   400,  1068,
     850,   401,   402,  1069,  1070,  1314,   389,   390,   391,   865,
     779,   867,   767,   426,   427,   428,   601,   780,   602,  1071,
     105,   106,   107,   255,   536,   537,   538,   768,   665,   769,
    1022,   211,   440,   669,  1072,   770,  1073,   899,   404,   676,
     597,   405,  1074,   598,   441,  1116,   255,   902,   263,   264,
     265,   266,  1203,  1296,   689,  1207,   771,   404,   834,    89,
     405,   480,   481,  1075,   697,  1169,   836,   924,   925,   926,
     105,   106,   107,  1170,   814,   937,   938,   815,   939,   940,
     438,   941,   942,   943,   944,   945,   946,   947,   948,  1161,
     108,   109,   439,   110,   111,   112,   113,   114,   115,   116,
     820,   952,   734,   821,   736,   738,   956,   267,   268,   269,
     270,   271,   272,   273,   837,   274,   968,   838,   601,   969,
     805,  1171,   970,   839,  1172,   275,   276,   652,   840,   974,
    1173,   764,   421,   873,   874,  1174,   404,   875,  1190,   405,
     765,  1274,  1276,  1315,   766,   980,   981,  1191,   117,   841,
     468,  1192,   379,   987,   767,   308,   309,   310,   311,   312,
     995,  1193,   442,   118,   110,   111,   364,   365,   366,   367,
     368,  1139,   716,   464,   718,   470,  1140,  1000,   277,   278,
     279,   280,   281,   282,   283,   284,   285,  1142,   471,  1010,
    1011,  1013,  1140,   286,   876,   877,   878,  1024,   313,   314,
     474,   768,   288,   289,   290,   291,   292,   293,  1194,   769,
     782,   770,   475,   771,  1030,  1078,  1195,   476,  1196,   369,
    1197,   784,  1233,  1229,   787,   484,  1035,   379,  1140,   808,
     796,  1080,   799,   479,   118,   294,   295,   834,  1234,   845,
     586,   587,   379,   315,  1241,   287,   316,   317,   318,   319,
     320,   321,   322,   323,   227,  1058,  1059,  1060,  1061,  1062,
    1063,   836,   840,   228,  1085,   229,   230,   585,  1242,  1243,
    1087,  1277,   599,   600,   231,   232,   606,  1278,   588,   233,
     833,   764,   589,  1331,   765,   234,   235,   879,  1335,   842,
     379,  1336,   590,   236,   237,   238,   239,   227,   766,   324,
     325,   326,   591,  1089,  1040,  1337,   228,   767,   229,   230,
     594,   379,   768,   769,  1338,   592,   770,   231,   232,  1339,
    1340,   593,   233,  1341,   771,   834,   836,   840,   234,   235,
     595,  1342,  1343,  1344,  1345,   604,   236,   237,   238,   239,
     612,   613,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,
     964,   614,   615,   379,  1128,   541,   542,   543,   544,   545,
     546,   547,   548,   549,  1138,   561,   562,   563,   564,   565,
     566,   567,   297,   298,   632,   379,   299,   300,   846,   848,
     656,   477,   851,   852,   854,   988,   603,  1042,   989,    88,
    1043,  1044,  1045,  1046,   605,   872,  1092,   954,   955,   379,
     480,   657,   959,   960,  1175,  1176,   963,  1099,   480,   666,
    1100,   893,   894,   895,  1154,   227,   606,  1155,  1178,  1156,
    1157,   607,  1180,  1181,   228,   900,   229,   230,   903,   507,
     609,   509,   510,   511,  1183,   231,   232,   379,   979,    88,
     233,  1187,  1188,  1189,   608,  1238,   234,   235,   379,   379,
     379,   923,   967,   611,   236,   237,   238,   239,  1199,   556,
     557,   558,   670,   481,  1200,  1160,   601,   617,  1055,   671,
     672,  1205,   674,   481,   654,  1211,   687,   688,   703,   481,
    1215,   705,   481,   480,   783,   860,   688,   912,   688,   921,
     379,   922,   379,   962,   379,  1231,  1232,     4,     5,     6,
       7,     8,   971,   379,   630,   142,   143,   133,   134,   354,
      14,    15,    16,    17,    18,    19,    20,   972,   613,   973,
     379,   977,   613,   789,   790,   792,   793,   136,    30,   480,
     978,   982,   821,  1254,    36,   992,   688,   996,   379,   655,
      38,   997,   379,   631,   105,   106,   107,  1268,    46,    47,
      48,    49,  1272,   699,    52,   998,   672,   701,  1001,  1050,
     688,  1051,   688,   696,    57,    58,  1079,   379,  1081,   613,
      59,   708,  1138,  1082,   379,  1084,   379,   704,  1284,  1285,
    1287,  1106,   672,    63,  1239,   672,   480,  1096,  1298,   728,
    1299,  1105,   481,  1179,   379,  1031,  1237,   379,  1247,   688,
     714,  1091,  1256,   688,    64,    65,  1307,   688,  1309,   688,
    1037,  1311,  1097,   200,   202,   709,  1318,  1319,  1320,  1321,
    1322,  1323,  1324,  1325,   559,   560,   710,   711,   712,   713,
     732,   729,   762,   763,   772,   764,   765,   766,   767,   768,
    1332,  1333,  1334,   769,   770,   771,   287,   776,   777,   788,
     801,   806,   809,   807,   810,   811,   812,   816,   817,   822,
      66,   832,   866,   870,   871,   834,    67,    68,   835,   836,
     837,   838,    69,   889,    71,   355,   839,    72,   840,    73,
     843,   356,   844,     4,     5,     6,     7,     8,   880,   881,
     882,   142,   143,   133,   134,   354,    14,    15,    16,    17,
      18,    19,    20,   883,   884,   379,  1104,   890,   885,   891,
    1108,  1109,  1110,   136,    30,   896,   892,   901,   897,  1182,
      36,   898,   904,  1184,   905,   421,    38,   965,   961,   966,
     975,   976,   985,   986,    46,    47,    48,    49,  1133,  1002,
      52,   990,  1135,   993,   994,  1003,  1004,  1005,  1023,  1032,
      57,    58,  1033,  1034,  1036,  1038,    59,  1047,  1052,  1053,
    1054,  1078,  1056,  1080,     4,     5,     6,     7,     8,    63,
    1083,  1085,   142,   143,   133,   134,   354,    14,    15,    16,
      17,    18,    19,    20,  1086,  1087,  1094,  1093,  1088,  1090,
      64,    65,  1095,  1101,   136,    30,  1102,  1111,  1127,  1131,
    1132,    36,  1134,  1136,  1158,  1159,  1164,    38,  1165,  1166,
    1167,  1177,  1198,  1185,  1186,    46,    47,    48,    49,  1201,
    1204,    52,  1208,  1209,  1212,  1210,  1213,  1214,  1226,  1227,
    1236,    57,    58,  1265,  1228,  1244,  1235,    59,  1257,  1258,
    1259,  1264,  1140,  1266,  1248,  1267,    66,  1269,  1270,  1271,
      63,  1279,    67,    68,  1300,  1301,  1302,  1303,    69,  1281,
      71,   797,  1304,    72,  1305,    73,   227,   356,  1310,   583,
    1326,    64,    65,   171,  1327,   228,   906,   229,   230,  1008,
     819,   540,   512,     0,     0,     0,   231,   232,     0,     0,
       0,   233,     0,     0,     0,     0,     0,   234,   235,     0,
       0,     0,     0,     0,     0,   236,   237,   238,   239,  1245,
       0,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,   957,     0,    72,     0,    73,  1280,   356,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   354,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   136,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,  1330,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   513,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,     0,    71,     0,     0,    72,
       0,    73,     0,   356,     4,     5,     6,     7,     8,     0,
       0,     0,     9,   485,   486,   487,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   488,   489,    26,
       0,     0,    27,    28,    29,    30,   490,   491,    33,    34,
      35,    36,    37,     0,     0,     0,     0,    38,    39,    40,
      41,    42,    43,   492,   493,    46,    47,    48,    49,    50,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,    60,     0,     0,     0,    61,    62,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
      70,    71,     0,     0,    72,     0,    73,   494,     4,     5,
       6,     7,     8,     0,     0,     0,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,   861,   862,    26,     0,     0,    27,    28,    29,    30,
      31,   863,    33,    34,    35,    36,    37,     0,     0,     0,
       0,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,    60,     0,     0,     0,    61,
      62,     0,     0,     0,    63,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,    70,    71,     0,     0,    72,     0,
      73,   864,     4,     5,     6,     7,     8,     0,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,     0,     0,     0,     0,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,     0,     0,     0,    55,    56,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,    60,
       0,     0,     0,    61,    62,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,    70,    71,
       0,     0,    72,     0,    73,   920,     4,     5,     6,     7,
       8,     0,     0,     0,     9,    10,    11,  1249,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,  1250,
    1251,    26,     0,     0,    27,    28,    29,    30,    31,  1252,
      33,    34,    35,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,    60,     0,     0,     0,    61,    62,     0,
       0,     0,    63,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,   136,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,    66,
       0,     0,     0,     0,     0,    67,    68,     0,     0,    63,
       0,    69,    70,    71,     0,     0,    72,     0,    73,  1253,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   136,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,   174,
      71,    38,     0,    72,     0,    73,   745,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   136,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,   174,    71,     0,     0,    72,     0,
      73,  1012,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,   174,    71,     0,     0,    72,     0,    73,
    1286,     4,     5,     6,     7,     8,     0,     0,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,     0,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       0,     0,     0,     0,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,     0,     0,     0,    55,    56,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,    60,     0,
       0,     0,    61,    62,     0,     0,     0,    63,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,   136,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,    66,     0,     0,     0,     0,     0,
      67,    68,     0,     0,    63,     0,    69,    70,    71,     0,
       0,    72,     0,    73,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,     0,    71,    38,     0,    72,   144,
      73,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    63,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,   136,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    63,     0,
       0,     0,    67,    68,     0,     0,     0,     0,    69,   147,
     148,     0,     0,    72,     0,    73,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,   174,    71,
      38,     0,    72,     0,    73,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    63,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    63,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,   176,    71,     0,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,   209,    71,    38,     0,    72,     0,    73,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     136,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,     0,    71,   241,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,    71,    38,     0,
      72,   243,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   136,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,     0,    71,   339,     0,    72,     0,    73,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,   136,
      30,     0,     0,    67,    68,   344,    36,     0,     0,    69,
       0,    71,    38,     0,    72,     0,    73,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    63,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   136,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,   373,    71,     0,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,   453,     0,     0,     0,
       0,    66,     0,     0,   136,    30,     0,    67,    68,     0,
       0,    36,     0,    69,     0,    71,   375,    38,    72,     0,
      73,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      63,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   136,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,     0,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   136,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
      71,    38,   460,    72,     0,    73,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,   245,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   136,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,     0,    71,     0,     0,    72,     0,
      73,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   136,    30,     0,     0,    67,    68,     0,    36,
       0,     0,    69,     0,    71,    38,     0,    72,   628,    73,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    63,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,   136,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    63,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,    71,
     733,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,   136,    30,     0,
      67,    68,     0,     0,    36,     0,    69,     0,    71,   735,
      38,    72,     0,    73,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    63,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    63,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,     0,    71,     0,     0,    72,   737,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,   136,    30,     0,    67,    68,     0,     0,    36,
       0,    69,     0,    71,   781,    38,    72,     0,    73,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     4,     5,     6,   640,     8,     0,     0,    63,   142,
     143,   133,   134,   641,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,   136,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    63,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,    71,
     795,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,   136,    30,     0,
      67,    68,     0,     0,    36,     0,    69,     0,    71,   241,
      38,    72,     0,    73,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    63,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    63,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,     0,    71,   953,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,     0,   158,    38,     0,    72,     0,    73,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     136,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,     0,   160,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,   162,    38,     0,
      72,     0,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   136,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,     0,   164,     0,     0,    72,     0,    73,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,   136,
      30,     0,     0,    67,    68,     0,    36,     0,     0,    69,
       0,    71,    38,     0,    72,     0,    73,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    63,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   136,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,     0,   185,     0,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,     0,   187,    38,     0,    72,     0,
      73,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    63,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,   136,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    63,     0,
       0,     0,    67,    68,     0,     0,     0,     0,    69,     0,
      71,     0,     0,    72,     0,   201,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,     0,   342,
      38,     0,    72,     0,    73,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     4,     5,     6,   640,
       8,     0,     0,    63,   142,   143,   133,   134,   641,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    63,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,     0,   637,     0,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,     0,   642,    38,     0,    72,     0,    73,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     136,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,     0,   825,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   131,   132,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,     0,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,   849,    38,     0,
      72,     0,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -546,  -546,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,     0,    71,     0,     0,    72,     0,    73,  -546,  -546,
    -546,     0,  -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,
    -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,     0,     0,
       0,  -546,     0,     0,     0,     0,     0,     0,     0,  -546,
    -546,     0,     0,  -546,  -546,  -546,  -546,  -546,  -546,  -546,
    -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,
    -546,  -546,  -546,  -546,     0,  -546,  -546,     0,     0,  -546,
    -546,  -546,  -546,  -546,  -546,  -546,  -546,  -546,     0,  -546,
    -546,  -546,  -546,  -546,  -546,  -546,  -546,     0,     0,  -546,
    -546,  -546,  -546,  -546,     0,     0,  -546,  -546,  -546
};

static const yytype_int16 yycheck[] =
{
       2,     0,   392,   420,   258,   671,   846,   157,    15,    15,
      32,     2,     2,     2,    24,    17,    11,    12,     0,    15,
      15,    24,    24,    25,    26,    27,    28,    28,    24,    31,
      32,    33,    11,    12,    36,    37,    15,    12,    40,    41,
     328,    11,   330,    55,    86,    15,    48,    49,    60,    61,
      52,    53,  1041,   609,   471,    15,    15,    15,    15,   615,
      28,    18,    11,    12,    13,    24,    15,    11,    12,    71,
      72,    15,    11,    12,   362,   363,    15,    72,    73,    74,
      48,    28,    28,   165,    33,   180,    28,   110,   111,    38,
      42,    43,   187,    72,    73,    74,    63,    64,   180,   107,
      67,    48,    48,    11,    12,   187,   108,    15,    65,    69,
     112,   113,   114,   115,   116,   117,    75,   119,   120,   121,
     122,   179,   124,    24,   182,   127,    42,    43,    72,    73,
      74,    24,    28,    72,    73,    74,   178,    24,   180,    24,
       6,    15,   165,   178,   146,   180,   148,    11,    12,    15,
      24,    15,    48,    11,    12,   185,   158,    15,   160,   167,
     162,  1001,   164,   185,    72,    73,    74,    11,    12,    63,
      64,    15,  1161,    67,   180,   185,    15,   185,    15,   185,
     187,   165,   185,   185,    13,   187,   181,    24,   190,   185,
     192,   193,    28,   195,   196,   179,   198,   199,   200,   201,
     202,    75,   181,   178,    28,   180,   165,    13,    72,    73,
      74,   179,    48,  1202,    72,    73,    74,  1206,   186,   186,
     179,    28,    11,   779,    48,    16,    15,    86,    72,    73,
      74,   180,   179,   179,   186,   652,   182,   181,    75,   186,
     186,    48,   181,   185,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   180,   256,   246,   246,   246,  1247,    28,
     187,   185,    15,    15,   165,   415,    57,  1256,   110,   111,
     186,    24,   165,   181,   286,    66,    67,    68,   165,    48,
     165,    33,    11,   179,  1273,   167,  1275,   185,    28,   679,
     186,   165,   186,    11,    28,   179,   297,   298,   299,   300,
     301,   302,   186,   185,    28,   179,    28,   167,    48,    28,
      15,   728,   729,   107,    48,   107,   165,   181,  1307,   178,
    1309,   180,    75,   181,    48,   185,    48,   329,   165,    48,
     107,    89,   998,   165,   183,    11,   726,   181,   180,    89,
     342,   179,   179,   179,   182,  1185,   182,   179,    77,    78,
     186,    81,    82,    83,   179,   179,    11,   359,   182,   361,
      86,   186,   364,   365,   366,   367,   368,   369,   370,    89,
      28,   165,   179,    31,    32,   167,    79,   379,   380,   381,
       2,   383,   384,    41,   386,   387,    44,    45,   165,   183,
      48,    49,   103,   185,   165,   397,   398,   399,   400,   401,
     402,   403,   179,   107,   117,   407,   408,    29,    30,   180,
     179,   118,   165,   164,   134,   135,   187,   418,   419,   178,
      42,    43,   421,   130,   182,    15,   179,   185,    50,    51,
     432,   433,   182,   435,   436,   185,   438,   439,   828,   179,
      62,   148,   444,   107,   446,   179,   186,   178,   182,   451,
     452,   171,   186,   104,   105,   179,   186,   179,   460,   167,
     179,   165,   186,   465,    87,    88,    42,    43,   858,   165,
     860,    89,    87,    88,   762,   763,   475,   185,    28,   183,
      28,    31,    32,    31,    32,    11,   488,   489,   490,   491,
     165,    41,    31,    41,    44,    45,    44,    45,    48,    49,
      48,   165,   182,    79,   179,   185,   179,   158,   159,   160,
      49,   134,   135,   186,   136,   179,   134,   135,   772,   134,
     135,   179,   912,   913,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   107,    15,   110,
     111,   163,    28,   165,   166,    31,    32,   179,   171,   107,
     182,    77,    78,   171,   186,    41,   171,    28,    44,    45,
      31,    32,    48,    49,   186,    28,   188,   180,    31,    32,
      41,   179,   185,    44,    45,   178,    28,    48,    41,    31,
      32,    44,    45,   134,   135,    48,    49,   110,   111,    41,
     162,   163,    44,    45,   165,   165,    48,   599,   600,   221,
     165,   183,   992,   119,   120,   119,   120,   165,   610,   179,
     119,   120,   180,   615,   179,   110,   111,   185,  1008,   907,
     171,   179,    11,    12,   246,   182,    15,   184,   180,   179,
     179,   179,   165,   185,   182,   637,   186,   186,   186,   182,
     642,   184,   165,   645,   646,    15,   648,   180,   650,   165,
     179,   165,   654,   655,   187,    28,   165,   186,    31,    32,
    1050,  1051,   179,   178,   371,   165,   179,   179,    41,   186,
     672,    44,    45,   186,   186,    48,    11,    12,    13,   681,
     180,   683,   179,    72,    73,    74,   182,   187,   184,   186,
     681,   681,   681,   179,     8,     9,    10,   179,   405,   179,
     186,    15,    76,   410,   186,   179,   186,   709,   179,   416,
     179,   182,   186,   182,   182,   186,   179,   719,    90,    91,
      92,    93,  1139,   186,   431,  1142,   179,   179,   179,   731,
     182,   181,   182,   186,   441,   186,   179,   739,   740,   741,
     731,   731,   731,   186,   179,   747,   748,   182,   750,   751,
     165,   753,   754,   755,   756,   757,   758,   759,   760,  1047,
     107,   108,   165,   110,   111,   112,   113,   114,   115,   116,
     179,   773,   479,   182,   481,   482,   778,   149,   150,   151,
     152,   153,   154,   155,   179,   157,   788,   179,   182,   791,
     184,   186,   794,   179,   186,   167,   168,   180,   179,   801,
     186,   179,   185,    11,    12,   186,   179,    15,   186,   182,
     179,  1228,  1229,   186,   179,   817,   818,   186,   165,   179,
     179,   186,   182,   825,   179,   129,   130,   131,   132,   133,
     832,   186,   185,   180,   110,   111,   112,   113,   114,   115,
     116,   180,   464,    59,   466,   185,   185,   849,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   180,   180,   861,
     862,   863,   185,   109,    72,    73,    74,   869,   172,   173,
     185,   179,   136,   137,   138,   139,   140,   141,   186,   179,
     587,   179,   185,   179,   886,   179,   186,   179,   186,   165,
     186,   598,   186,   180,   601,   186,   898,   182,   185,   184,
     607,   179,   609,   180,   180,   169,   170,   179,   186,   179,
     179,   180,   182,   118,   186,   161,   121,   122,   123,   124,
     125,   126,   127,   128,    15,   927,   928,   929,   930,   931,
     932,   179,   179,    24,   179,    26,    27,   179,   186,   186,
     179,   186,   172,   173,    35,    36,   179,   186,   179,    40,
     657,   179,   179,   186,   179,    46,    47,   179,   186,   666,
     182,   186,   179,    54,    55,    56,    57,    15,   179,   174,
     175,   176,   179,   975,    65,   186,    24,   179,    26,    27,
     181,   182,   179,   179,   186,   179,   179,    35,    36,   186,
     186,   179,    40,   186,   179,   179,   179,   179,    46,    47,
     179,   186,   186,   186,   186,   168,    54,    55,    56,    57,
     181,   182,  1014,  1015,  1016,  1017,  1018,  1019,  1020,  1021,
     179,   181,   182,   182,  1026,   288,   289,   290,   291,   292,
     293,   294,   295,   296,  1036,   308,   309,   310,   311,   312,
     313,   314,   143,   144,   181,   182,   147,   148,   670,   671,
     181,   182,   674,   675,   676,   179,   179,    11,   182,   681,
      14,    15,    16,    17,   186,   687,   179,   774,   775,   182,
     181,   182,   779,   780,  1076,  1077,   783,   179,   181,   182,
     182,   703,   704,   705,    11,    15,   179,    14,  1090,    16,
      17,   180,  1094,  1095,    24,   717,    26,    27,   720,   257,
     180,   259,   260,   261,   179,    35,    36,   182,   815,   731,
      40,  1113,  1114,  1115,    15,   179,    46,    47,   182,   182,
     182,   184,   184,    15,    54,    55,    56,    57,  1130,   303,
     304,   305,   181,   182,  1136,    65,   182,    15,   184,    29,
      30,  1140,   181,   182,   165,  1147,   181,   182,   181,   182,
    1152,   181,   182,   181,   182,   181,   182,   181,   182,   181,
     182,   181,   182,   181,   182,  1167,  1168,     3,     4,     5,
       6,     7,   181,   182,   179,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   181,   182,   181,
     182,   181,   182,   119,   120,   119,   120,    33,    34,   181,
     182,   181,   182,  1205,    40,   181,   182,   181,   182,   165,
      46,   181,   182,   179,  1205,  1205,  1205,  1219,    54,    55,
      56,    57,  1224,    15,    60,    29,    30,    15,   850,   181,
     182,   181,   182,   179,    70,    71,   181,   182,   181,   182,
      76,    11,  1244,   181,   182,   181,   182,    29,  1250,  1251,
    1252,    29,    30,    89,    29,    30,   181,   182,  1260,   180,
    1262,   181,   182,   181,   182,   887,   181,   182,   181,   182,
     186,   978,   181,   182,   110,   111,   181,   182,   181,   182,
     902,  1283,   989,    47,    48,   179,  1288,  1289,  1290,  1291,
    1292,  1293,  1294,  1295,   306,   307,   179,   179,   179,   179,
      11,   180,   178,   178,   106,   179,   179,   179,   179,   179,
    1312,  1313,  1314,   179,   179,   179,   161,   179,    15,   165,
     180,   168,   179,   186,   179,   179,   179,   179,    86,   179,
     166,   165,    15,    11,    11,   179,   172,   173,   179,   179,
     179,   179,   178,   168,   180,   181,   179,   183,   179,   185,
     179,   187,   179,     3,     4,     5,     6,     7,   179,   179,
     179,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   179,   179,   182,   998,   186,   179,   168,
    1002,  1003,  1004,    33,    34,   179,   186,   186,   179,  1096,
      40,   165,    15,  1100,    15,   185,    46,   184,   179,   184,
      86,    15,   179,   179,    54,    55,    56,    57,  1030,    11,
      60,   179,  1034,   179,   179,    11,    11,    15,    15,   179,
      70,    71,   179,    29,   179,   186,    76,    63,    15,    15,
      15,   179,   186,   179,     3,     4,     5,     6,     7,    89,
     179,   179,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   179,   179,    86,   179,   181,   180,
     110,   111,    86,   179,    33,    34,   179,    15,    15,    11,
      11,    40,   186,   179,   179,   179,     8,    46,     8,     8,
     165,   179,    15,  1105,  1106,    54,    55,    56,    57,   181,
      79,    60,    15,    15,    15,   179,    15,   179,   179,   179,
     179,    70,    71,    15,   180,   179,   181,    76,   179,   179,
     179,   179,   185,    15,   186,   179,   166,    15,    15,   179,
      89,   179,   172,   173,   179,   179,   179,   179,   178,  1244,
     180,   181,   179,   183,   179,   185,    15,   187,   181,    18,
     179,   110,   111,    30,   179,    24,   724,    26,    27,   860,
     642,   287,   262,    -1,    -1,    -1,    35,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,  1201,
      -1,    -1,    -1,  1205,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,
      -1,   180,   181,    -1,   183,    -1,   185,  1239,   187,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,  1310,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,   173,
      -1,    -1,    -1,    -1,   178,    -1,   180,    -1,    -1,   183,
      -1,   185,    -1,   187,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,
     179,   180,    -1,    -1,   183,    -1,   185,   186,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    -1,    -1,
      -1,    66,    67,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,
      85,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,   172,   173,    -1,
      -1,    -1,    -1,   178,   179,   180,    -1,    -1,   183,    -1,
     185,   186,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   172,   173,    -1,    -1,    -1,    -1,   178,   179,   180,
      -1,    -1,   183,    -1,   185,   186,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    85,    -1,
      -1,    -1,    89,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,    89,
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,
      -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,   179,
     180,    46,    -1,   183,    -1,   185,   186,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    89,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,
      -1,    -1,    -1,   178,   179,   180,    -1,    -1,   183,    -1,
     185,   186,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,
     186,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    89,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
     172,   173,    -1,    -1,    89,    -1,   178,   179,   180,    -1,
      -1,   183,    -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,
      40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,   184,
     185,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    89,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    89,    -1,
      -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,   179,
     180,    -1,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,
      -1,   172,   173,    -1,    40,    -1,    -1,   178,   179,   180,
      46,    -1,   183,    -1,   185,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    89,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,
      -1,   178,   179,   180,    46,    -1,   183,    -1,   185,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    89,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,
     172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,
      -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,
     183,   184,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    89,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,    -1,   180,   181,    -1,   183,    -1,   185,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,
      34,    -1,    -1,   172,   173,   174,    40,    -1,    -1,   178,
      -1,   180,    46,    -1,   183,    -1,   185,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    89,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    89,    -1,    -1,    -1,   172,   173,
      -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,   183,
      -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    33,    34,    -1,   172,   173,    -1,
      -1,    40,    -1,   178,    -1,   180,   181,    46,   183,    -1,
     185,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      89,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    89,
      -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,
      -1,   180,    -1,    -1,   183,    -1,   185,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,
      -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,    -1,
     180,    46,   182,   183,    -1,   185,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    79,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    89,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,
      -1,    -1,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,
     185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,
      -1,    -1,   178,    -1,   180,    46,    -1,   183,   184,   185,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    89,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,
      -1,   172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    33,    34,    -1,
     172,   173,    -1,    -1,    40,    -1,   178,    -1,   180,   181,
      46,   183,    -1,   185,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    89,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,    -1,   180,    -1,    -1,   183,   184,   185,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    33,    34,    -1,   172,   173,    -1,    -1,    40,
      -1,   178,    -1,   180,   181,    46,   183,    -1,   185,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    89,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,
      -1,   172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,
     181,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    33,    34,    -1,
     172,   173,    -1,    -1,    40,    -1,   178,    -1,   180,   181,
      46,   183,    -1,   185,    -1,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    89,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,    -1,   180,   181,    -1,   183,    -1,   185,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,
      -1,   178,    -1,   180,    46,    -1,   183,    -1,   185,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    89,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,
     172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,    -1,
      -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,
     183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    89,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,    -1,   180,    -1,    -1,   183,    -1,   185,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,
      34,    -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,
      -1,   180,    46,    -1,   183,    -1,   185,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    89,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    89,    -1,    -1,    -1,   172,   173,
      -1,    -1,    -1,    -1,   178,    -1,   180,    -1,    -1,   183,
      -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,
      40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,    -1,
     185,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    89,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    89,    -1,
      -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,    -1,
     180,    -1,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,
      -1,   172,   173,    -1,    40,    -1,    -1,   178,    -1,   180,
      46,    -1,   183,    -1,   185,    -1,    -1,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    89,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,
      -1,   178,    -1,   180,    46,    -1,   183,    -1,   185,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    89,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,
     172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,    -1,
      -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    33,    -1,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,
     183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,    -1,   180,    -1,    -1,   183,    -1,   185,    86,    87,
      88,    -1,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,
     118,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,    -1,   143,   144,    -1,    -1,   147,
     148,   149,   150,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,   170,   171,    -1,    -1,   174,   175,   176
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   189,   190,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    66,    67,    70,    71,    76,
      80,    84,    85,    89,   110,   111,   166,   172,   173,   178,
     179,   180,   183,   185,   191,   192,   194,   195,   196,   197,
     198,   200,   201,   205,   209,   210,   211,   214,   219,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   238,   239,   241,   242,   107,   108,
     110,   111,   112,   113,   114,   115,   116,   165,   180,   107,
     165,   183,   107,   167,   185,   110,   111,   165,    12,   178,
     180,    11,    12,    13,    14,    15,    33,   219,   234,   239,
     241,   242,    11,    12,   184,   221,    15,   179,   180,   221,
     240,    11,    12,    13,    15,    33,    38,   180,   180,   221,
     180,   221,   180,   221,   180,   221,   221,   230,    15,   180,
     219,   200,   219,   221,   179,   221,   179,   221,    11,    15,
      15,   221,   221,    13,    13,   180,   221,   180,   221,   219,
     180,   219,    28,    48,   179,    28,    48,   179,   185,   220,
     220,   185,   220,   221,   221,   219,   185,   219,   221,   179,
     221,    15,   208,   208,    15,    33,   234,   208,   208,   219,
      11,   236,    11,    11,   234,   234,   234,    15,    24,    26,
      27,    35,    36,    40,    46,    47,    54,    55,    56,    57,
     237,   181,   221,   184,   221,    79,   190,    28,    31,    32,
      41,    44,    45,    48,    49,   179,    86,   104,   105,   158,
     159,   160,   103,    90,    91,    92,    93,   149,   150,   151,
     152,   153,   154,   155,   157,   167,   168,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   109,   161,   136,   137,
     138,   139,   140,   141,   169,   170,   117,   143,   144,   147,
     148,    87,    88,   134,   135,   171,   162,   163,   129,   130,
     131,   132,   133,   172,   173,   118,   121,   122,   123,   124,
     125,   126,   127,   128,   174,   175,   176,   164,   178,   165,
     178,   221,    15,   221,   221,   221,   221,   221,   221,   181,
     240,   221,   180,   221,   174,   221,   221,    15,   221,   221,
     179,    15,    18,    65,    15,   181,   187,   216,   240,   183,
     167,   185,   178,   178,   112,   113,   114,   115,   116,   165,
     165,   182,   184,   179,   221,   181,   221,   240,   179,   182,
     107,   165,   179,   107,   165,   179,   107,   165,   179,    11,
      12,    13,    15,    11,    15,   236,   221,    28,    31,    32,
      41,    44,    45,    48,   179,   182,   221,    28,    48,   179,
     182,   221,   219,   221,   219,    89,   182,   219,    87,    88,
     180,   185,   206,    11,    12,    15,    72,    73,    74,   218,
     179,   182,    28,    48,   179,    28,    48,   179,   165,   165,
      76,   182,   185,   167,   185,   167,   185,   221,   219,   221,
     219,    31,    49,    25,   221,   221,   221,   221,   221,   221,
     182,   221,   221,   221,    59,    42,    43,   202,   179,   212,
     185,   180,    15,    69,   185,   185,   179,   182,   219,   180,
     181,   182,   182,   184,   186,    12,    13,    14,    26,    27,
      35,    36,    52,    53,   186,   221,   239,   242,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   224,   222,   224,
     224,   224,   225,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,     8,     9,    10,   208,
     227,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     230,   230,   230,   230,   230,   230,   231,   231,   231,   232,
     232,   233,   233,   233,   233,   233,   233,   233,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   237,   221,    18,   237,   179,   179,   180,   179,   179,
     179,   179,   179,   179,   181,   179,   221,   179,   182,   172,
     173,   182,   184,   179,   168,   186,   179,   180,    15,   180,
      86,    15,   181,   182,   181,   182,   221,    15,   221,   237,
     237,   221,   221,   221,   221,   221,   221,   221,   184,   240,
     179,   179,   181,   221,   221,   221,   221,   180,   221,   221,
       6,    15,   180,   217,   221,   107,   165,   179,   165,   179,
     165,   179,   180,   206,   165,   165,   181,   182,   221,   221,
     221,   221,   221,   221,   221,   240,   182,   221,   221,   240,
     181,    29,    30,   193,   181,   236,   240,   230,   230,   181,
     218,   190,    24,   165,    11,    77,    78,   181,   182,   240,
     221,   221,   221,   221,   221,   221,   179,   240,   203,    15,
     221,    15,   221,   181,    29,   181,   221,   221,    11,   179,
     179,   179,   179,   179,   186,   221,   219,   221,   219,    42,
      43,   186,    15,    24,   185,   213,   181,   218,   180,   180,
     215,   190,    11,   181,   240,   181,   240,   184,   240,   183,
     185,   165,   221,   221,   221,   186,   221,    28,    48,   186,
      28,    48,   186,    28,    31,    32,    41,    44,    45,    48,
      49,   186,   178,   178,   179,   179,   179,   179,   179,   179,
     179,   179,   106,   165,   180,   187,   179,    15,   165,   180,
     187,   181,   240,   182,   240,   221,   221,   240,   165,   119,
     120,   165,   119,   120,   165,   181,   240,   181,   216,   240,
     221,   180,    15,   187,   216,   184,   168,   186,   184,   179,
     179,   179,   179,   221,   179,   182,   179,    86,    86,   217,
     179,   182,   179,   221,   221,   180,   221,   221,   181,   218,
     221,   221,   165,   240,   179,   179,   179,   179,   179,   179,
     179,   179,   240,   179,   179,   179,   219,   192,   219,   180,
     221,   219,   219,    89,   219,    89,    89,    24,   204,   206,
     181,    26,    27,    36,   186,   221,    15,   221,    24,   165,
      11,    11,   219,    11,    12,    15,    72,    73,    74,   179,
     179,   179,   179,   179,   179,   179,    42,    43,   186,   168,
     186,   168,   186,   219,   219,   219,   179,   179,   165,   221,
     219,   186,   221,   219,    15,    15,   213,    63,    64,    67,
     186,   206,   181,   181,   218,   218,    81,    82,    83,   186,
     186,   181,   181,   184,   221,   221,   221,    28,    32,    41,
      44,    45,    48,   186,   186,   186,   186,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   237,
     237,   222,   221,   181,   240,   240,   221,   181,   216,   240,
     240,   179,   181,   240,   179,   184,   184,   184,   221,   221,
     221,   181,   181,   181,   221,    86,    15,   181,   182,   240,
     221,   221,   181,     6,    15,   179,   179,   221,   179,   182,
     179,   206,   181,   179,   179,   221,   181,   181,    29,   193,
     221,   219,    11,    11,    11,    15,    24,   206,   204,   206,
     221,   221,   186,   221,    28,    31,    32,    41,    44,    45,
      48,    49,   186,    15,   221,    24,   165,    11,    77,    78,
     221,   219,   179,   179,    29,   221,   179,   219,   186,   186,
      65,   237,    11,    14,    15,    16,    17,    63,   206,   206,
     181,   181,    15,    15,    15,   184,   186,   186,   221,   221,
     221,   221,   221,   221,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   165,   165,   179,   181,
     179,   181,   181,   179,   181,   179,   179,   179,   181,   221,
     180,   240,   179,   179,    86,    86,   182,   240,   206,   179,
     182,   179,   179,   192,   219,   181,    29,   193,   219,   219,
     219,    15,   206,    28,    32,    48,   186,   186,   186,   221,
     221,   221,   221,   221,   221,   221,   221,    15,   221,    24,
     165,    11,    11,   219,   186,   219,   179,   199,   221,   180,
     185,   207,   180,   207,    15,    24,    75,   165,   179,    15,
      24,    75,   165,   179,    11,    14,    16,    17,   179,   179,
      65,   237,   206,   206,     8,     8,     8,   165,   165,   186,
     186,   186,   186,   186,   186,   221,   221,   179,   221,   181,
     221,   221,   240,   179,   240,   219,   219,   221,   221,   221,
     186,   186,   186,   186,   186,   186,   186,   186,    15,   221,
     221,   181,   181,   218,    79,   190,   181,   218,    15,    15,
     179,   221,    15,    15,   179,   221,    15,    24,    75,   165,
     179,    15,    24,    75,   165,   179,   179,   179,   180,   180,
     207,   221,   221,   186,   186,   181,   179,   181,   179,    29,
     193,   186,   186,   186,   179,   219,   207,   181,   186,    14,
      26,    27,    36,   186,   221,   207,   181,   179,   179,   179,
     165,   179,   165,   179,   179,    15,    15,   179,   221,    15,
      15,   179,   221,   181,   218,   181,   218,   186,   186,   179,
     219,   199,   207,   165,   221,   221,   186,   221,    28,    31,
      32,    41,    44,    45,    48,    49,   186,   207,   221,   221,
     179,   179,   179,   179,   179,   179,   207,   181,   207,   181,
     181,   221,    28,    32,    48,   186,   186,   186,   221,   221,
     221,   221,   221,   221,   221,   221,   179,   179,   207,   207,
     219,   186,   221,   221,   221,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   188,   189,   190,   190,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     191,   191,   191,   191,   191,   191,   191,   191,   191,   191,
     192,   192,   192,   192,   192,   192,   192,   192,   193,   193,
     193,   193,   193,   193,   194,   194,   195,   195,   195,   195,
     196,   196,   197,   197,   197,   198,   198,   198,   199,   200,
     200,   200,   200,   200,   200,   201,   201,   202,   202,   203,
     203,   204,   204,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   208,   209,   210,   211,   212,   212,   212,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   214,   215,   215,
     215,   215,   216,   216,   216,   216,   216,   217,   217,   217,
     217,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   220,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     222,   222,   223,   223,   223,   223,   223,   224,   224,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   226,   226,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   228,   228,
     229,   229,   229,   229,   229,   229,   229,   230,   230,   230,
     230,   231,   231,   231,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   234,
     234,   235,   235,   236,   236,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   240,   240,   240,   241,   241,
     241,   241,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     5,     5,     5,
       6,     4,     4,     4,     7,     9,     4,     3,     3,     5,
       7,     9,     4,     6,     8,     5,     5,     7,     6,     6,
       8,    10,     6,     4,     4,     4,     3,     4,     5,     5,
       6,     6,     4,     2,     5,     4,     3,     3,     3,     5,
       7,     3,     5,     7,     3,     5,     3,     2,     3,     2,
       5,     5,     3,     2,     5,     5,     4,     7,     6,     4,
       6,     6,     4,     7,     7,     7,     6,     6,     4,     4,
       4,     4,     4,     4,     4,     4,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     4,     4,     4,     4,     4,
       2,     1,     1,     1,     1,     1,     2,     4,     4,     6,
       5,     2,     1,     2,     3,     1,     1,     1,     1,     2,
       2,     4,     4,     4,     4,     1,     1,     1,     1,     1,
       5,     7,     7,     3,     5,     5,     4,     6,     3,     5,
       5,     7,     4,     6,     5,     3,     5,     7,     3,     5,
       5,     3,     2,     5,     5,     2,    12,     9,     1,     7,
       7,     5,     7,     5,     3,     5,     7,     3,     4,     0,
       4,     2,     3,     6,     5,     3,     7,     6,     7,     6,
       4,     7,     6,     7,     6,     5,     3,     4,     5,     4,
       5,     5,     6,     6,     6,     6,     6,     6,     6,     6,
       7,     7,     7,     3,     3,     6,     4,     5,     4,     5,
       5,     6,     6,     6,     6,     6,     6,     6,     6,     7,
       7,     7,     1,     6,     5,     5,     0,     3,     3,     0,
       4,     4,     4,     5,     5,     4,     5,     5,     6,     6,
       7,     7,     6,     8,     8,     6,     6,     6,     5,     5,
       6,     6,     7,     7,     7,     7,     7,     6,     4,     7,
       6,     4,     8,     7,     5,     8,     7,     5,     0,     4,
       4,     4,     3,     5,     2,     5,     7,     3,     3,     5,
       5,     1,     3,     5,     4,     6,     1,     3,     2,     3,
       3,     3,     4,     5,     5,     3,     5,     4,     6,     1,
       3,     1,     3,     1,     3,     3,     3,     4,     6,     6,
       6,     6,     6,     6,     6,     6,     5,     7,     7,     7,
       7,     7,     7,     5,     8,     8,     6,     9,     9,     5,
       5,     4,     4,     4,     6,     6,     6,     6,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     1,
       5,     1,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     2,     2,     2,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     4,
       4,     3,     4,     6,     4,     3,     6,     6,     5,     3,
       3,     6,     5,     4,     4,     6,     6,     5,     3,     6,
       5,     3,     5,     5,     5,     4,     2,     2,     3,     4,
       3,     2,     3,     2,     1,     1,     3,     2,     2,     3,
       4,     5,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     2,     2,     1,     1,     1,     1,     1,
       4,     6,     6,     6,     1,     4,     4,     5,     5,     5,
       5,     5,     5,     1,     1,     1,     1,     2,     3,     4,
       5,     2,     3,     4,     5,     1,     1,     2,     5,     3
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
#line 738 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 3960 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 748 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3966 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 749 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3972 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 753 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3978 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 755 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3984 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 757 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 3990 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 759 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 3996 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 761 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4002 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 763 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4009 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 766 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4015 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 768 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4021 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 770 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4027 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 772 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4033 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 774 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4041 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 778 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4047 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 780 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4053 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 782 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4059 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 784 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4065 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 786 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4073 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 790 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4081 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 794 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4087 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 796 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4095 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 800 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4103 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 804 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4109 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 806 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4115 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 808 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4121 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 810 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4127 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4133 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 814 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4141 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 818 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4149 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 822 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4155 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4161 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4167 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4173 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 830 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4179 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 832 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4185 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 834 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4191 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 836 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4197 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 838 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4203 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 840 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4209 "raku.tab.c"
    break;

  case 42: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 842 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4221 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 850 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4227 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 852 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4233 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 854 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4239 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 856 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4245 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 858 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4251 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 860 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4257 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 862 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4264 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 865 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4271 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 868 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4277 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 870 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4284 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 873 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4291 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 876 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4297 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 878 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4305 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 882 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4311 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 884 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4317 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 886 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4323 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 888 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4329 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 890 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4336 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 893 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4343 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 896 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4349 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 898 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4356 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 901 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4363 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 904 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4370 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 907 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4376 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 909 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4385 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 914 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4393 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 918 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4401 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 922 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4408 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 925 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4415 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 928 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4423 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 932 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4430 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 935 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4437 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 938 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4444 "raku.tab.c"
    break;

  case 76: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 941 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4451 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 944 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4458 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_IF expr ';'  */
#line 947 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4464 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNLESS expr ';'  */
#line 949 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4470 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WHILE expr ';'  */
#line 951 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4476 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_UNTIL expr ';'  */
#line 953 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4482 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_FOR expr ';'  */
#line 955 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4489 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WITH expr ';'  */
#line 958 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4495 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITHOUT expr ';'  */
#line 960 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4501 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_GIVEN expr ';'  */
#line 962 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4507 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 964 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4514 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 967 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4521 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 970 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4529 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 974 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4536 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 977 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4542 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 979 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4548 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 981 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4554 "raku.tab.c"
    break;

  case 93: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 983 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4561 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 986 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4568 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 989 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4575 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 992 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4582 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 995 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4589 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 998 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4596 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1001 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4603 "raku.tab.c"
    break;

  case 100: /* stmt: expr ';'  */
#line 1003 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4609 "raku.tab.c"
    break;

  case 101: /* stmt: ';'  */
#line 1004 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4615 "raku.tab.c"
    break;

  case 102: /* stmt: if_stmt  */
#line 1005 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4621 "raku.tab.c"
    break;

  case 103: /* stmt: while_stmt  */
#line 1006 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4627 "raku.tab.c"
    break;

  case 104: /* stmt: for_stmt  */
#line 1007 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4633 "raku.tab.c"
    break;

  case 105: /* stmt: given_stmt  */
#line 1008 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4639 "raku.tab.c"
    break;

  case 106: /* stmt: KW_TRY block  */
#line 1010 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4645 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block KW_CATCH block  */
#line 1012 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4651 "raku.tab.c"
    break;

  case 108: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1014 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4659 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1018 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4668 "raku.tab.c"
    break;

  case 110: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1023 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4676 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH block  */
#line 1027 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4682 "raku.tab.c"
    break;

  case 112: /* stmt: block  */
#line 1029 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4688 "raku.tab.c"
    break;

  case 113: /* stmt: PHASER block  */
#line 1031 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 4694 "raku.tab.c"
    break;

  case 114: /* stmt: PHASER block ';'  */
#line 1033 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 4700 "raku.tab.c"
    break;

  case 115: /* stmt: unless_stmt  */
#line 1034 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4706 "raku.tab.c"
    break;

  case 116: /* stmt: until_stmt  */
#line 1035 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4712 "raku.tab.c"
    break;

  case 117: /* stmt: repeat_stmt  */
#line 1036 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4718 "raku.tab.c"
    break;

  case 118: /* stmt: loop_stmt  */
#line 1037 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4724 "raku.tab.c"
    break;

  case 119: /* stmt: KW_LAST ';'  */
#line 1038 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4730 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT ';'  */
#line 1039 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4736 "raku.tab.c"
    break;

  case 121: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1041 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4742 "raku.tab.c"
    break;

  case 122: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1043 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4748 "raku.tab.c"
    break;

  case 123: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1045 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4754 "raku.tab.c"
    break;

  case 124: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1047 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4760 "raku.tab.c"
    break;

  case 125: /* stmt: sub_decl  */
#line 1048 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4766 "raku.tab.c"
    break;

  case 126: /* stmt: class_decl  */
#line 1049 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4772 "raku.tab.c"
    break;

  case 127: /* stmt: role_decl  */
#line 1050 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4778 "raku.tab.c"
    break;

  case 128: /* stmt: grammar_decl  */
#line 1051 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4784 "raku.tab.c"
    break;

  case 129: /* stmt: module_decl  */
#line 1052 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4790 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1056 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4796 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1058 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4802 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1060 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4808 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF expr block  */
#line 1062 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4814 "raku.tab.c"
    break;

  case 134: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1064 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4820 "raku.tab.c"
    break;

  case 135: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1066 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4826 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1068 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4832 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1070 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4838 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block  */
#line 1074 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4844 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1076 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4850 "raku.tab.c"
    break;

  case 140: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1078 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4856 "raku.tab.c"
    break;

  case 141: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4862 "raku.tab.c"
    break;

  case 142: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4868 "raku.tab.c"
    break;

  case 143: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1084 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4874 "raku.tab.c"
    break;

  case 144: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1088 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4880 "raku.tab.c"
    break;

  case 145: /* while_stmt: KW_WHILE expr block  */
#line 1090 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4886 "raku.tab.c"
    break;

  case 146: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1094 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4892 "raku.tab.c"
    break;

  case 147: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1096 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4898 "raku.tab.c"
    break;

  case 148: /* unless_stmt: KW_UNLESS expr block  */
#line 1098 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4904 "raku.tab.c"
    break;

  case 149: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1100 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4910 "raku.tab.c"
    break;

  case 150: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1104 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4916 "raku.tab.c"
    break;

  case 151: /* until_stmt: KW_UNTIL expr block  */
#line 1106 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4922 "raku.tab.c"
    break;

  case 152: /* repeat_stmt: KW_REPEAT block  */
#line 1110 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4928 "raku.tab.c"
    break;

  case 153: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1112 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4934 "raku.tab.c"
    break;

  case 154: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1114 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4940 "raku.tab.c"
    break;

  case 155: /* loop_stmt: KW_LOOP block  */
#line 1118 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4946 "raku.tab.c"
    break;

  case 156: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1120 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4952 "raku.tab.c"
    break;

  case 157: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1122 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4958 "raku.tab.c"
    break;

  case 158: /* loop_incr: expr  */
#line 1125 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4964 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1129 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 4974 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1135 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 4984 "raku.tab.c"
    break;

  case 161: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1141 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 4996 "raku.tab.c"
    break;

  case 162: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1149 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5006 "raku.tab.c"
    break;

  case 163: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1155 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5015 "raku.tab.c"
    break;

  case 164: /* for_stmt: KW_FOR expr block  */
#line 1160 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5022 "raku.tab.c"
    break;

  case 165: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1165 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 5034 "raku.tab.c"
    break;

  case 166: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1173 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 5047 "raku.tab.c"
    break;

  case 167: /* catch_when_list: KW_WHEN expr block  */
#line 1184 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5053 "raku.tab.c"
    break;

  case 168: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1186 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5059 "raku.tab.c"
    break;

  case 169: /* when_list: %empty  */
#line 1189 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5065 "raku.tab.c"
    break;

  case 170: /* when_list: when_list KW_WHEN expr block  */
#line 1191 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5073 "raku.tab.c"
    break;

  case 171: /* sub_trait_list: TESTOP IDENT  */
#line 1196 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5079 "raku.tab.c"
    break;

  case 172: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1197 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5085 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1201 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5097 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1209 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5107 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1215 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5117 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1221 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5129 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1229 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5139 "raku.tab.c"
    break;

  case 178: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1235 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5151 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1243 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5161 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1249 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5171 "raku.tab.c"
    break;

  case 181: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1255 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5184 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1264 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5195 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1271 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5208 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1280 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5221 "raku.tab.c"
    break;

  case 185: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1289 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5232 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list '}'  */
#line 1297 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5238 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr '}'  */
#line 1299 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5245 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1302 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5252 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1305 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5258 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1307 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5264 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1309 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5270 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1311 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5276 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1313 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5282 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1315 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5288 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1317 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5294 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1319 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5301 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1322 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5307 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1324 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5313 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5319 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1328 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5326 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1331 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5333 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1334 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5341 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list '}'  */
#line 1339 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5347 "raku.tab.c"
    break;

  case 204: /* method_body: '{' YADA '}'  */
#line 1340 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5353 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1342 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5360 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr '}'  */
#line 1345 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5367 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1348 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5374 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1351 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5380 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1353 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5386 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1355 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5392 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1357 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5398 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1359 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5404 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1361 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5410 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1363 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5416 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1365 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5423 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1368 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5429 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1370 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5435 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1372 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5441 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1374 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5448 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1377 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5455 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1380 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5463 "raku.tab.c"
    break;

  case 222: /* pkg_name: IDENT  */
#line 1385 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5469 "raku.tab.c"
    break;

  case 223: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1389 "raku.y"
        {
            const char *cname = intern((yyvsp[-4].sval)); ct_drop((yyvsp[-4].sval));
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
#line 5487 "raku.tab.c"
    break;

  case 224: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1405 "raku.y"
        {
            const char *rname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
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
#line 5504 "raku.tab.c"
    break;

  case 225: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1420 "raku.y"
        {
            const char *mname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
            ExprList *body = (yyvsp[-1].list);
            tree_t *md = ast_node_new(TT_MODULE_DECL);
            ast_push(md, leaf_sval(TT_VAR, mname));
            if (body) {
                for (int i = 0; i < body->count; i++)
                    if (body->items[i]) ast_push(md, body->items[i]);
                exprlist_free(body);
            }
            (yyval.node) = md;
        }
#line 5521 "raku.tab.c"
    break;

  case 226: /* is_clauses: %empty  */
#line 1434 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5527 "raku.tab.c"
    break;

  case 227: /* is_clauses: is_clauses IDENT IDENT  */
#line 1436 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)ct_alloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)ct_alloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); ct_drop((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval));
        }
#line 5543 "raku.tab.c"
    break;

  case 228: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1448 "raku.y"
        {
            char tag = 0;
            if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "is")) tag = 'i';
            else if ((yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "does")) tag = 'd';
            if (tag && (yyvsp[0].sval)) {
                size_t l2 = strlen((yyvsp[0].sval));
                if (!(yyvsp[-2].sval)) { char *m = (char *)ct_alloc(l2 + 2); m[0] = tag; memcpy(m + 1, (yyvsp[0].sval), l2 + 1); (yyval.sval) = m; }
                else { size_t l1 = strlen((yyvsp[-2].sval)); char *m = (char *)ct_alloc(l1 + l2 + 3); memcpy(m, (yyvsp[-2].sval), l1); m[l1] = '\x01'; m[l1 + 1] = tag; memcpy(m + l1 + 2, (yyvsp[0].sval), l2 + 1); ct_drop((yyvsp[-2].sval)); (yyval.sval) = m; }
            } else { (yyval.sval) = (yyvsp[-2].sval); }
            ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval));
        }
#line 5559 "raku.tab.c"
    break;

  case 229: /* class_body_list: %empty  */
#line 1461 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5565 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1463 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5572 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1466 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5579 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1469 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5586 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1472 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5593 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1475 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5600 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1478 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5607 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1481 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5614 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1484 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5621 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1487 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5628 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1490 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5635 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1493 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5642 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1496 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5649 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1499 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5660 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1506 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5670 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1512 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5680 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1518 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5691 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1525 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5702 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1532 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5713 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1539 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5721 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1543 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5729 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1547 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5737 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1551 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5745 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1555 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5756 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1562 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5767 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1569 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5778 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1576 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5789 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1583 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5803 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1593 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5815 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1601 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5827 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1609 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5840 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1618 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5851 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1625 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5862 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1632 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5876 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1642 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5888 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1650 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5900 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1658 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5913 "raku.tab.c"
    break;

  case 266: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1667 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5924 "raku.tab.c"
    break;

  case 267: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1676 "raku.y"
        {
            const char *gname = intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
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
#line 5941 "raku.tab.c"
    break;

  case 268: /* grammar_body_list: %empty  */
#line 1690 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5947 "raku.tab.c"
    break;

  case 269: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1692 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5956 "raku.tab.c"
    break;

  case 270: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1697 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5965 "raku.tab.c"
    break;

  case 271: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1702 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5974 "raku.tab.c"
    break;

  case 272: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1709 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5982 "raku.tab.c"
    break;

  case 273: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1713 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5990 "raku.tab.c"
    break;

  case 274: /* named_arg_list: ':' IDENT  */
#line 1717 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6001 "raku.tab.c"
    break;

  case 275: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1724 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6009 "raku.tab.c"
    break;

  case 276: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1728 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6017 "raku.tab.c"
    break;

  case 277: /* pair_list: IDENT OP_FATARROW expr  */
#line 1734 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6023 "raku.tab.c"
    break;

  case 278: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1736 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6029 "raku.tab.c"
    break;

  case 279: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1738 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6035 "raku.tab.c"
    break;

  case 280: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1740 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6041 "raku.tab.c"
    break;

  case 281: /* param_list: VAR_SCALAR  */
#line 1743 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6047 "raku.tab.c"
    break;

  case 282: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1744 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6053 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1745 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6059 "raku.tab.c"
    break;

  case 284: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1746 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6065 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1747 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6071 "raku.tab.c"
    break;

  case 286: /* param_list: VAR_ARRAY  */
#line 1748 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6077 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' VAR_ARRAY  */
#line 1749 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6083 "raku.tab.c"
    break;

  case 288: /* param_list: IDENT VAR_SCALAR  */
#line 1750 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6089 "raku.tab.c"
    break;

  case 289: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1751 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6095 "raku.tab.c"
    break;

  case 290: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1752 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6101 "raku.tab.c"
    break;

  case 291: /* param_list: param_list ',' VAR_SCALAR  */
#line 1753 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6107 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1754 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6113 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1755 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6119 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1756 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6125 "raku.tab.c"
    break;

  case 295: /* param_list: VAR_SCALAR '=' expr  */
#line 1757 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6131 "raku.tab.c"
    break;

  case 296: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1758 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6137 "raku.tab.c"
    break;

  case 297: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1759 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6143 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1760 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6149 "raku.tab.c"
    break;

  case 299: /* param_list: SLURPY_POS  */
#line 1761 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6155 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' SLURPY_POS  */
#line 1762 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6161 "raku.tab.c"
    break;

  case 301: /* param_list: SLURPY_LOL  */
#line 1763 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6167 "raku.tab.c"
    break;

  case 302: /* param_list: param_list ',' SLURPY_LOL  */
#line 1764 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6173 "raku.tab.c"
    break;

  case 303: /* param_list: SLURPY_NAMED  */
#line 1765 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6179 "raku.tab.c"
    break;

  case 304: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1766 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6185 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list '}'  */
#line 1769 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6191 "raku.tab.c"
    break;

  case 306: /* block: '{' YADA '}'  */
#line 1770 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6197 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list expr '}'  */
#line 1772 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6203 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1774 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6209 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1776 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6215 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1778 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6221 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1780 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6227 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1782 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6234 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1785 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6240 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1787 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6246 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1789 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6252 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1791 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6258 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1793 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6265 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1796 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6272 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1799 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6280 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1803 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6287 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1806 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6294 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1809 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6301 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1812 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6307 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1814 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6314 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1817 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6321 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1820 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6328 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1823 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6335 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1826 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6342 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1829 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6348 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1831 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6354 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1833 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6360 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_LAST '}'  */
#line 1835 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6366 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1837 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6372 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1839 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6378 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1841 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6384 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1843 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6390 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1845 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6396 "raku.tab.c"
    break;

  case 338: /* closure: '{' expr '}'  */
#line 1848 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6402 "raku.tab.c"
    break;

  case 339: /* expr: VAR_SCALAR '=' expr  */
#line 1851 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6408 "raku.tab.c"
    break;

  case 340: /* expr: VAR_ARRAY '=' expr  */
#line 1852 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6414 "raku.tab.c"
    break;

  case 341: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1853 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6420 "raku.tab.c"
    break;

  case 342: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1854 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6426 "raku.tab.c"
    break;

  case 343: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1855 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6432 "raku.tab.c"
    break;

  case 344: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1856 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6438 "raku.tab.c"
    break;

  case 345: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1857 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6444 "raku.tab.c"
    break;

  case 346: /* expr: KW_GATHER block  */
#line 1858 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6454 "raku.tab.c"
    break;

  case 347: /* expr: KW_GATHER for_stmt  */
#line 1863 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6464 "raku.tab.c"
    break;

  case 348: /* expr: tern_expr OP_FATARROW expr  */
#line 1869 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6470 "raku.tab.c"
    break;

  case 349: /* expr: tern_expr  */
#line 1870 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6476 "raku.tab.c"
    break;

  case 350: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1874 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6482 "raku.tab.c"
    break;

  case 351: /* tern_expr: or_expr  */
#line 1875 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6488 "raku.tab.c"
    break;

  case 352: /* or_expr: or_expr OP_OR and_expr  */
#line 1878 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6494 "raku.tab.c"
    break;

  case 353: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1879 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6500 "raku.tab.c"
    break;

  case 354: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1880 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6506 "raku.tab.c"
    break;

  case 355: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1881 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6512 "raku.tab.c"
    break;

  case 356: /* or_expr: and_expr  */
#line 1882 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6518 "raku.tab.c"
    break;

  case 357: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1885 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6524 "raku.tab.c"
    break;

  case 358: /* and_expr: cmp_expr  */
#line 1886 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6530 "raku.tab.c"
    break;

  case 359: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1889 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6536 "raku.tab.c"
    break;

  case 360: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1890 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6542 "raku.tab.c"
    break;

  case 361: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1891 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6548 "raku.tab.c"
    break;

  case 362: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1892 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6554 "raku.tab.c"
    break;

  case 363: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1893 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6560 "raku.tab.c"
    break;

  case 364: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1894 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6566 "raku.tab.c"
    break;

  case 365: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1895 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6572 "raku.tab.c"
    break;

  case 366: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1896 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6578 "raku.tab.c"
    break;

  case 367: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1897 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6584 "raku.tab.c"
    break;

  case 368: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1898 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6590 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1899 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6596 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1900 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6602 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1901 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6608 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1902 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6614 "raku.tab.c"
    break;

  case 373: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1903 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6620 "raku.tab.c"
    break;

  case 374: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1904 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6626 "raku.tab.c"
    break;

  case 375: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1905 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6632 "raku.tab.c"
    break;

  case 376: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1906 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6638 "raku.tab.c"
    break;

  case 377: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1907 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6644 "raku.tab.c"
    break;

  case 378: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1908 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6650 "raku.tab.c"
    break;

  case 379: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1909 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6656 "raku.tab.c"
    break;

  case 380: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1910 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6662 "raku.tab.c"
    break;

  case 381: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1911 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6668 "raku.tab.c"
    break;

  case 382: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1913 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6678 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1919 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6688 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1925 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6698 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1931 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6708 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr  */
#line 1936 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6714 "raku.tab.c"
    break;

  case 387: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1939 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6720 "raku.tab.c"
    break;

  case 388: /* divis_expr: jct_expr  */
#line 1940 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6726 "raku.tab.c"
    break;

  case 389: /* jct_expr: jct_expr '|' range_expr  */
#line 1943 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6732 "raku.tab.c"
    break;

  case 390: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1944 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6738 "raku.tab.c"
    break;

  case 391: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1945 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6744 "raku.tab.c"
    break;

  case 392: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1946 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6750 "raku.tab.c"
    break;

  case 393: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1947 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6756 "raku.tab.c"
    break;

  case 394: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1948 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6762 "raku.tab.c"
    break;

  case 395: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1949 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6768 "raku.tab.c"
    break;

  case 396: /* jct_expr: jct_expr '&' range_expr  */
#line 1950 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6774 "raku.tab.c"
    break;

  case 397: /* jct_expr: dor_expr  */
#line 1951 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6780 "raku.tab.c"
    break;

  case 398: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1955 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6786 "raku.tab.c"
    break;

  case 399: /* dor_expr: range_expr  */
#line 1956 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6792 "raku.tab.c"
    break;

  case 400: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1959 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6798 "raku.tab.c"
    break;

  case 401: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1960 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6804 "raku.tab.c"
    break;

  case 402: /* range_expr: range_expr OP_COLL add_expr  */
#line 1961 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6810 "raku.tab.c"
    break;

  case 403: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1962 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6816 "raku.tab.c"
    break;

  case 404: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1963 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6822 "raku.tab.c"
    break;

  case 405: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1964 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6828 "raku.tab.c"
    break;

  case 406: /* range_expr: add_expr  */
#line 1965 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6834 "raku.tab.c"
    break;

  case 407: /* add_expr: add_expr '~' repl_expr  */
#line 1968 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6840 "raku.tab.c"
    break;

  case 408: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 1969 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6846 "raku.tab.c"
    break;

  case 409: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 1970 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6852 "raku.tab.c"
    break;

  case 410: /* add_expr: repl_expr  */
#line 1971 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6858 "raku.tab.c"
    break;

  case 411: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1974 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6864 "raku.tab.c"
    break;

  case 412: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1975 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6870 "raku.tab.c"
    break;

  case 413: /* repl_expr: addsub_expr  */
#line 1976 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6876 "raku.tab.c"
    break;

  case 414: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1979 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6882 "raku.tab.c"
    break;

  case 415: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 1980 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6888 "raku.tab.c"
    break;

  case 416: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 1981 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6894 "raku.tab.c"
    break;

  case 417: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 1982 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6900 "raku.tab.c"
    break;

  case 418: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 1983 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6906 "raku.tab.c"
    break;

  case 419: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 1984 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6912 "raku.tab.c"
    break;

  case 420: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1985 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6918 "raku.tab.c"
    break;

  case 421: /* addsub_expr: mul_expr  */
#line 1986 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6924 "raku.tab.c"
    break;

  case 422: /* mul_expr: mul_expr '*' unary_expr  */
#line 1989 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6930 "raku.tab.c"
    break;

  case 423: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 1990 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6936 "raku.tab.c"
    break;

  case 424: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 1991 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6942 "raku.tab.c"
    break;

  case 425: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 1992 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6948 "raku.tab.c"
    break;

  case 426: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 1993 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6954 "raku.tab.c"
    break;

  case 427: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 1994 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6960 "raku.tab.c"
    break;

  case 428: /* mul_expr: mul_expr '/' unary_expr  */
#line 1995 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6966 "raku.tab.c"
    break;

  case 429: /* mul_expr: mul_expr '%' unary_expr  */
#line 1996 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6972 "raku.tab.c"
    break;

  case 430: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1998 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6978 "raku.tab.c"
    break;

  case 431: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2000 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6984 "raku.tab.c"
    break;

  case 432: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2002 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6990 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2004 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6996 "raku.tab.c"
    break;

  case 434: /* mul_expr: unary_expr  */
#line 2005 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7002 "raku.tab.c"
    break;

  case 435: /* unary_expr: '-' unary_expr  */
#line 2008 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 7008 "raku.tab.c"
    break;

  case 436: /* unary_expr: '+' unary_expr  */
#line 2009 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 7014 "raku.tab.c"
    break;

  case 437: /* unary_expr: '!' unary_expr  */
#line 2010 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7020 "raku.tab.c"
    break;

  case 438: /* unary_expr: CARET unary_expr  */
#line 2011 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7026 "raku.tab.c"
    break;

  case 439: /* unary_expr: OP_REDUCE unary_expr  */
#line 2013 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7035 "raku.tab.c"
    break;

  case 440: /* unary_expr: pow_expr  */
#line 2017 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7041 "raku.tab.c"
    break;

  case 441: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2020 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7047 "raku.tab.c"
    break;

  case 442: /* pow_expr: postfix_expr  */
#line 2021 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7053 "raku.tab.c"
    break;

  case 443: /* scalar_list: VAR_SCALAR  */
#line 2024 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7059 "raku.tab.c"
    break;

  case 444: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2025 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7065 "raku.tab.c"
    break;

  case 445: /* meth_name: IDENT  */
#line 2028 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7071 "raku.tab.c"
    break;

  case 446: /* meth_name: KW_SORT  */
#line 2029 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7077 "raku.tab.c"
    break;

  case 447: /* meth_name: KW_REVERSE  */
#line 2030 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7083 "raku.tab.c"
    break;

  case 448: /* meth_name: KW_MAP  */
#line 2031 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7089 "raku.tab.c"
    break;

  case 449: /* meth_name: KW_GREP  */
#line 2032 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7095 "raku.tab.c"
    break;

  case 450: /* meth_name: KW_SAY  */
#line 2033 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7101 "raku.tab.c"
    break;

  case 451: /* meth_name: KW_PRINT  */
#line 2034 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7107 "raku.tab.c"
    break;

  case 452: /* meth_name: KW_TAKE  */
#line 2035 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7113 "raku.tab.c"
    break;

  case 453: /* meth_name: KW_RETURN  */
#line 2036 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7119 "raku.tab.c"
    break;

  case 454: /* meth_name: KW_EXISTS  */
#line 2037 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7125 "raku.tab.c"
    break;

  case 455: /* meth_name: KW_DELETE  */
#line 2038 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7131 "raku.tab.c"
    break;

  case 456: /* meth_name: KW_JOIN  */
#line 2039 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7137 "raku.tab.c"
    break;

  case 457: /* meth_name: TESTOP  */
#line 2040 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7143 "raku.tab.c"
    break;

  case 458: /* postfix_expr: call_expr  */
#line 2042 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7149 "raku.tab.c"
    break;

  case 459: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2045 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7159 "raku.tab.c"
    break;

  case 460: /* call_expr: IDENT '(' arg_list ')'  */
#line 2051 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7168 "raku.tab.c"
    break;

  case 461: /* call_expr: IDENT '(' ')'  */
#line 2055 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7174 "raku.tab.c"
    break;

  case 462: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2057 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7180 "raku.tab.c"
    break;

  case 463: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2059 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7186 "raku.tab.c"
    break;

  case 464: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2061 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7195 "raku.tab.c"
    break;

  case 465: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2066 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7201 "raku.tab.c"
    break;

  case 466: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2068 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7214 "raku.tab.c"
    break;

  case 467: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2077 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7227 "raku.tab.c"
    break;

  case 468: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2086 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7238 "raku.tab.c"
    break;

  case 469: /* call_expr: IDENT '.' KW_NEW  */
#line 2093 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7246 "raku.tab.c"
    break;

  case 470: /* call_expr: IDENT '.' IDENT  */
#line 2097 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7255 "raku.tab.c"
    break;

  case 471: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2102 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7266 "raku.tab.c"
    break;

  case 472: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2109 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7275 "raku.tab.c"
    break;

  case 473: /* call_expr: IDENT '.' CARET IDENT  */
#line 2114 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7285 "raku.tab.c"
    break;

  case 474: /* call_expr: atom '.' CARET IDENT  */
#line 2120 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7295 "raku.tab.c"
    break;

  case 475: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2126 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7306 "raku.tab.c"
    break;

  case 476: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2133 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7317 "raku.tab.c"
    break;

  case 477: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2140 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7326 "raku.tab.c"
    break;

  case 478: /* call_expr: atom '.' meth_name  */
#line 2145 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7335 "raku.tab.c"
    break;

  case 479: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2150 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7346 "raku.tab.c"
    break;

  case 480: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2157 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7355 "raku.tab.c"
    break;

  case 481: /* call_expr: call_expr '.' meth_name  */
#line 2162 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7364 "raku.tab.c"
    break;

  case 482: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2167 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7375 "raku.tab.c"
    break;

  case 483: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2174 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7386 "raku.tab.c"
    break;

  case 484: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2181 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7397 "raku.tab.c"
    break;

  case 485: /* call_expr: '.' meth_name '(' ')'  */
#line 2188 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7406 "raku.tab.c"
    break;

  case 486: /* call_expr: '.' meth_name  */
#line 2193 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7415 "raku.tab.c"
    break;

  case 487: /* call_expr: KW_DIE expr  */
#line 2198 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7421 "raku.tab.c"
    break;

  case 488: /* call_expr: KW_MAP closure expr  */
#line 2200 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7427 "raku.tab.c"
    break;

  case 489: /* call_expr: KW_MAP closure ',' expr  */
#line 2202 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7433 "raku.tab.c"
    break;

  case 490: /* call_expr: KW_GREP closure expr  */
#line 2204 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7439 "raku.tab.c"
    break;

  case 491: /* call_expr: KW_SORT expr  */
#line 2206 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7445 "raku.tab.c"
    break;

  case 492: /* call_expr: KW_SORT closure expr  */
#line 2208 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7451 "raku.tab.c"
    break;

  case 493: /* call_expr: KW_REVERSE expr  */
#line 2210 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7457 "raku.tab.c"
    break;

  case 494: /* call_expr: atom  */
#line 2211 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7463 "raku.tab.c"
    break;

  case 495: /* arg_list: expr  */
#line 2214 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7469 "raku.tab.c"
    break;

  case 496: /* arg_list: arg_list ',' expr  */
#line 2215 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7475 "raku.tab.c"
    break;

  case 497: /* arg_list: arg_list ','  */
#line 2216 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7481 "raku.tab.c"
    break;

  case 498: /* paren_group: '(' ')'  */
#line 2219 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7487 "raku.tab.c"
    break;

  case 499: /* paren_group: '(' expr ')'  */
#line 2220 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7493 "raku.tab.c"
    break;

  case 500: /* paren_group: '(' expr ',' ')'  */
#line 2222 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7499 "raku.tab.c"
    break;

  case 501: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2224 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7506 "raku.tab.c"
    break;

  case 502: /* atom: LIT_INT  */
#line 2228 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7512 "raku.tab.c"
    break;

  case 503: /* atom: LIT_BOOL  */
#line 2230 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7520 "raku.tab.c"
    break;

  case 504: /* atom: LIT_FLOAT  */
#line 2233 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7526 "raku.tab.c"
    break;

  case 505: /* atom: LIT_STR  */
#line 2234 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7532 "raku.tab.c"
    break;

  case 506: /* atom: WORDLIST  */
#line 2236 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7545 "raku.tab.c"
    break;

  case 507: /* atom: LIT_INTERP_STR  */
#line 2244 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7551 "raku.tab.c"
    break;

  case 508: /* atom: VAR_SCALAR  */
#line 2245 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7557 "raku.tab.c"
    break;

  case 509: /* atom: OP_INC VAR_SCALAR  */
#line 2246 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7563 "raku.tab.c"
    break;

  case 510: /* atom: OP_DEC VAR_SCALAR  */
#line 2247 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7569 "raku.tab.c"
    break;

  case 511: /* atom: VAR_SCALAR OP_INC  */
#line 2248 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7575 "raku.tab.c"
    break;

  case 512: /* atom: VAR_SCALAR OP_DEC  */
#line 2249 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7581 "raku.tab.c"
    break;

  case 513: /* atom: VAR_TWIGIL OP_INC  */
#line 2250 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 7587 "raku.tab.c"
    break;

  case 514: /* atom: VAR_TWIGIL OP_DEC  */
#line 2251 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 7593 "raku.tab.c"
    break;

  case 515: /* atom: VAR_ARRAY  */
#line 2252 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7599 "raku.tab.c"
    break;

  case 516: /* atom: VAR_HASH  */
#line 2253 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7605 "raku.tab.c"
    break;

  case 517: /* atom: VAR_CAPTURE  */
#line 2255 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7613 "raku.tab.c"
    break;

  case 518: /* atom: VAR_FH  */
#line 2259 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7621 "raku.tab.c"
    break;

  case 519: /* atom: VAR_NAMED_CAPTURE  */
#line 2263 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7628 "raku.tab.c"
    break;

  case 520: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2266 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7634 "raku.tab.c"
    break;

  case 521: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2268 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7640 "raku.tab.c"
    break;

  case 522: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2270 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7646 "raku.tab.c"
    break;

  case 523: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2272 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7652 "raku.tab.c"
    break;

  case 524: /* atom: ARR_ALL_SLICE  */
#line 2274 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7658 "raku.tab.c"
    break;

  case 525: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2276 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7664 "raku.tab.c"
    break;

  case 526: /* atom: VAR_HASH '{' expr '}'  */
#line 2278 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7670 "raku.tab.c"
    break;

  case 527: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2280 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7676 "raku.tab.c"
    break;

  case 528: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2282 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7682 "raku.tab.c"
    break;

  case 529: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2284 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7688 "raku.tab.c"
    break;

  case 530: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2286 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7694 "raku.tab.c"
    break;

  case 531: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2288 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7700 "raku.tab.c"
    break;

  case 532: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2290 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7706 "raku.tab.c"
    break;

  case 533: /* atom: IDENT  */
#line 2291 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7712 "raku.tab.c"
    break;

  case 534: /* atom: VAR_TWIGIL  */
#line 2293 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7720 "raku.tab.c"
    break;

  case 535: /* atom: VAR_ARRAY_TWIGIL  */
#line 2297 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7728 "raku.tab.c"
    break;

  case 536: /* atom: VAR_HASH_TWIGIL  */
#line 2301 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7736 "raku.tab.c"
    break;

  case 537: /* atom: '[' ']'  */
#line 2304 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7742 "raku.tab.c"
    break;

  case 538: /* atom: '[' expr ']'  */
#line 2306 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7748 "raku.tab.c"
    break;

  case 539: /* atom: '[' expr ',' ']'  */
#line 2308 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7754 "raku.tab.c"
    break;

  case 540: /* atom: '[' expr ',' arg_list ']'  */
#line 2310 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7761 "raku.tab.c"
    break;

  case 541: /* atom: DOLLAR_LBRACKET ']'  */
#line 2312 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7767 "raku.tab.c"
    break;

  case 542: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2314 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7773 "raku.tab.c"
    break;

  case 543: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2316 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7779 "raku.tab.c"
    break;

  case 544: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2318 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7786 "raku.tab.c"
    break;

  case 545: /* atom: paren_group  */
#line 2320 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 7792 "raku.tab.c"
    break;

  case 546: /* atom: block  */
#line 2321 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7798 "raku.tab.c"
    break;

  case 547: /* atom: KW_SUB block  */
#line 2322 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7804 "raku.tab.c"
    break;

  case 548: /* atom: KW_SUB '(' param_list ')' block  */
#line 2324 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 7811 "raku.tab.c"
    break;

  case 549: /* atom: OP_ARROW scalar_list block  */
#line 2327 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 7818 "raku.tab.c"
    break;


#line 7822 "raku.tab.c"

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

#line 2330 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
