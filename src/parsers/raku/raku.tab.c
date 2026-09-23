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
    tree_t *e = leaf_sval(TT_VAR, id);
    if (name && name[0] != '$' && name[0] != '@' && name[0] != '%') e->slen = 1;
    return e;
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

#line 714 "raku.tab.c"

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
#define YYLAST   7122

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  550
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1348

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
       0,   739,   739,   750,   751,   754,   756,   758,   760,   762,
     764,   767,   769,   771,   773,   775,   779,   781,   783,   785,
     787,   791,   795,   797,   801,   805,   807,   809,   811,   813,
     815,   819,   823,   825,   827,   829,   831,   833,   835,   837,
     839,   841,   843,   851,   853,   855,   857,   859,   861,   863,
     866,   869,   871,   874,   877,   879,   883,   885,   887,   889,
     891,   894,   897,   899,   902,   905,   908,   910,   915,   919,
     923,   926,   929,   933,   936,   939,   942,   945,   948,   950,
     952,   954,   956,   959,   961,   963,   965,   968,   971,   975,
     978,   980,   982,   984,   987,   990,   993,   996,   999,  1002,
    1005,  1006,  1007,  1008,  1009,  1010,  1011,  1013,  1015,  1019,
    1024,  1028,  1030,  1032,  1034,  1036,  1037,  1038,  1039,  1040,
    1041,  1042,  1044,  1046,  1048,  1050,  1051,  1052,  1053,  1054,
    1057,  1059,  1061,  1063,  1065,  1067,  1069,  1071,  1075,  1077,
    1079,  1081,  1083,  1085,  1089,  1091,  1095,  1097,  1099,  1101,
    1105,  1107,  1111,  1113,  1115,  1119,  1121,  1123,  1127,  1130,
    1136,  1142,  1150,  1156,  1161,  1166,  1174,  1185,  1187,  1191,
    1192,  1198,  1199,  1202,  1210,  1216,  1222,  1230,  1236,  1244,
    1250,  1256,  1265,  1272,  1281,  1290,  1299,  1300,  1303,  1306,
    1308,  1310,  1312,  1314,  1316,  1318,  1320,  1323,  1325,  1327,
    1329,  1332,  1335,  1341,  1342,  1343,  1346,  1349,  1352,  1354,
    1356,  1358,  1360,  1362,  1364,  1366,  1369,  1371,  1373,  1375,
    1378,  1381,  1387,  1390,  1406,  1421,  1436,  1437,  1449,  1463,
    1464,  1467,  1470,  1473,  1476,  1479,  1482,  1485,  1488,  1491,
    1494,  1497,  1500,  1507,  1513,  1519,  1526,  1533,  1540,  1544,
    1548,  1552,  1556,  1563,  1570,  1577,  1584,  1594,  1602,  1610,
    1619,  1626,  1633,  1643,  1651,  1659,  1668,  1677,  1692,  1693,
    1698,  1703,  1710,  1714,  1718,  1725,  1729,  1735,  1737,  1739,
    1741,  1745,  1746,  1747,  1748,  1749,  1750,  1751,  1752,  1753,
    1754,  1755,  1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,
    1764,  1765,  1766,  1767,  1768,  1771,  1772,  1773,  1775,  1777,
    1779,  1781,  1783,  1786,  1788,  1790,  1792,  1794,  1797,  1800,
    1804,  1807,  1810,  1813,  1815,  1818,  1821,  1824,  1827,  1830,
    1832,  1834,  1836,  1838,  1840,  1842,  1844,  1846,  1850,  1853,
    1854,  1855,  1856,  1857,  1858,  1859,  1860,  1865,  1870,  1872,
    1875,  1877,  1880,  1881,  1882,  1883,  1884,  1887,  1888,  1891,
    1892,  1893,  1894,  1895,  1896,  1897,  1898,  1899,  1900,  1901,
    1902,  1903,  1904,  1905,  1906,  1907,  1908,  1909,  1910,  1911,
    1912,  1913,  1914,  1920,  1926,  1932,  1938,  1941,  1942,  1945,
    1946,  1947,  1948,  1949,  1950,  1951,  1952,  1953,  1956,  1958,
    1961,  1962,  1963,  1964,  1965,  1966,  1967,  1968,  1971,  1972,
    1973,  1974,  1977,  1978,  1979,  1982,  1983,  1984,  1985,  1986,
    1987,  1988,  1989,  1992,  1993,  1994,  1995,  1996,  1997,  1998,
    1999,  2000,  2002,  2004,  2006,  2008,  2011,  2012,  2013,  2014,
    2015,  2020,  2023,  2024,  2027,  2028,  2031,  2032,  2033,  2034,
    2035,  2036,  2037,  2038,  2039,  2040,  2041,  2042,  2043,  2045,
    2047,  2053,  2058,  2059,  2061,  2063,  2068,  2070,  2079,  2088,
    2095,  2099,  2104,  2111,  2116,  2122,  2128,  2135,  2142,  2147,
    2152,  2159,  2164,  2169,  2176,  2183,  2190,  2195,  2200,  2202,
    2204,  2206,  2208,  2210,  2212,  2214,  2217,  2218,  2219,  2222,
    2223,  2224,  2226,  2231,  2232,  2236,  2237,  2238,  2247,  2248,
    2249,  2250,  2251,  2252,  2253,  2254,  2255,  2256,  2257,  2261,
    2265,  2268,  2270,  2272,  2274,  2276,  2278,  2280,  2282,  2284,
    2286,  2288,  2290,  2292,  2294,  2295,  2299,  2303,  2307,  2308,
    2310,  2312,  2315,  2316,  2318,  2320,  2323,  2324,  2325,  2326,
    2329
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

#define YYPACT_NINF (-1037)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-113)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1037,    98,  3073, -1037, -1037, -1037, -1037, -1037, -1037,   654,
     320,   170,   396,    32, -1037, -1037,  6937,  3160, -1037, -1037,
   -1037,   108,  3295,    44,  5518,  5653,  5732,  5867,  5946,    -1,
     -14,  5946,  3374,  3509,   578,   140,  5946,  5946,   151,   163,
    6081,  6160,    38,   -59,    30,    59,    51,    51,  6295,  5946,
      38,    81,  5946,  3588,   278,   278,    76,  6937, -1037, -1037,
     278,   278,    38,   308,   350,   363,  6937,  6937,  6937,  1085,
   -1037,  3723,  3802,   273, -1037, -1037, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,  1413,   629,
     298,   466,   287,   423,   758,   659,   296,   918,   341,   446,
     715,   778, -1037, -1037,   258,   285,   283,   293,  5946,   471,
   -1037, -1037,  5946,  5946,  5946,  5946,  5946,  5946,  3937,  5946,
    6374,  4016,  5946,   477,  5946, -1037, -1037,  5946,   338,   603,
    1500,   173,   352,  -119,   601,  -126,   130, -1037, -1037,   355,
   -1037,   391,   671,   -34, -1037,   156,  4151, -1037,  4230, -1037,
     323,   281,   427,   437,   882,   564,   713,   308,  3723,   553,
    3723,   180,  3723,    38,  3723,    38,   -40,   478,   171,   703,
   -1037, -1037, -1037,   567, -1037,    88, -1037,   212,   479,   498,
     541,   485,   542,   128,   133,  3723,    38,  3723,    38,   300,
    4366, -1037,  5946,  5946, -1037,  5946,  5946, -1037,  5946,  4445,
    5946,  4580,  5946, -1037, -1037,   625,    22, -1037, -1037, -1037,
     573, -1037, -1037,   595,   583,    79, -1037,   641,   697,   631,
   -1037,   589, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,
     640, -1037,   518, -1037,   270,   690,  2036,  5946,  5946,  5946,
    5946,  5946,  5946,  5946,  5946, -1037,  5946,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,   643,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,
    6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  6937,  1085,
    5946,  1013,   710,   626,   747,   763,   788,   815,   829,   845,
   -1037,   740,   854,  3723,   610,   765,   658,   885,   904,   892,
     912, -1037,   917,  1091,   930,   125, -1037,  1107,   780,   790,
    4016,  1118,  5946,  1085,  1013,  5946,  5946,  5946,  5946,  5946,
    5946,  5946,  4659, -1037, -1037,  1015,  1024,   518,   800, -1037,
    5946,  5946,  5946, -1037,  5946,  6509, -1037,  5946,  6588, -1037,
     449,   -80,   -18,   225,  1049,  1060,   862,   936,  5946,  5946,
    5946,  5946,  5946,  5946,  5946, -1037,  5946,   947,  5946,  5946,
   -1037,  5946,   976,  1130,   980, -1037,   308,  5946, -1037,  6937,
    6937,    56, -1037, -1037,    21, -1037,    25, -1037, -1037, -1037,
     982, -1037,  5946,  5946,  5946, -1037,  5946,  5946, -1037,  5946,
    5946,  1070,  5946, -1037,  1228,  5946,  1244,  5946,   984,  1232,
    1011, -1037,  5946,  5946,  1255,  1098,  1123,  1124,  1125,  1126,
    1120,  5946, -1037, -1037, -1037,    38,  5946,    38,    18, -1037,
       4, -1037,    61,  1127,  1128, -1037, -1037, -1037,  1298, -1037,
    4794, -1037,  4873,  5009, -1037, -1037,   440,   351,   438,  5518,
    5653,  5946,  2675,    47,   115, -1037,   276,  1132,  1133,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1144, -1037,   287,  1218,
     287,   287,   287,   423,  1164,  1164,  1164,  1164,  1164,  1164,
    1164,  1164,  1164,  1164,  1164,  1164,  1164,  1164,  1164,  1164,
    1164,  1164,  1164,  1164,  1164,  1164,  1164, -1037, -1037, -1037,
   -1037,   659,   918,   918,   918,   918,   918,   918,   918,   918,
     918,   448,   448,   448,   448,   448,   448,   448,   446,   446,
     446,   715,   715,   778,   778,   778,   778,   778,   778,   778,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037,   246,  1148,  1313,   482, -1037, -1037,  5088,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,  1014, -1037,
    5946,  5946,  5946,  5946,  1165, -1037,   228,   468, -1037,  5224,
   -1037,  1636,  5946,  1149, -1037,    -5, -1037,  1851,   872,  1163,
    1146,     0,    89, -1037, -1037, -1037, -1037, -1037, -1037, -1037,
   -1037,   966, -1037, -1037,  1154, -1037,  1155,  1156,  1157,  3723,
     611,  1158,  1252,   241,  5303,   738,  1160,  5946,  5946, -1037,
    6723, -1037,  5946, -1037,    78, -1037,  5946,  5946,  1175,  4873,
    1162,  1166,  1167,  1168,  1169,  1170,  1171,   876,  4873,  1172,
    1173,   910,    38,    -7,  6802, -1037,    38,   589,   233,     3,
     164,    -8,  1016,  2220,  1327,  5946,    62,  1332,  1333,    38,
     792,   920,  1174,  1176,  1177,  1178,  1179,  1180, -1037,  1181,
      20,  1186,  1182,  1192,  1183,    38,    38,    38,  1185,  1187,
    1196,  5946, -1037, -1037, -1037, -1037, -1037, -1037, -1037,    38,
    1184,  5946,    38, -1037,  1347,  1350, -1037,    17,  1188,  1018,
      93,   703,   263,  2404, -1037, -1037,  1020, -1037,  1025, -1037,
     967,  4016,  5946,  5946,   425,   147,   509, -1037,   288,  5946,
    5946, -1037,  5946,  5946, -1037,  5946,  5946,  5946,  5946,  5946,
    5946,  5946,  5946, -1037,  1085,  1013, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037,  6937,  5946,  5439,  5946, -1037, -1037,
    5946,  1715,  5946,  1193,  1027,  4873,   926,  1191,  1197,   971,
    5946, -1037, -1037,  5946, -1037, -1037,  5946, -1037,  1029, -1037,
    1031,  1039, -1037,  5946,  1281,  1356,  1041, -1037,  1108,  1110,
   -1037, -1037, -1037, -1037, -1037,  1050, -1037,  5946, -1037,  5946,
    5946,  1052, -1037,    68, -1037,  1198,  1203,  3723,   937,  1204,
    1188,  1054,  1205,  1206,  5946,  1056, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037,  1058, -1037, -1037, -1037,  1212, -1037,
   -1037,  3723,    38, -1037, -1037,  1365, -1037,  1375,  1376,  1373,
      -4, -1037,    -8,  5518,  5653,  2810, -1037,   450, -1037, -1037,
    1374,  5946, -1037, -1037, -1037,   100, -1037,   362, -1037, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,  5946,    38,
   -1037, -1037, -1037,  1211,  1213,  1362, -1037, -1037, -1037, -1037,
    5946,  1214, -1037, -1037,    38,  1208, -1037, -1037,    33,   909,
     996,  1334, -1037, -1037,  1188,  1188,  1064,  1066,  1380,  1381,
    1383, -1037, -1037, -1037, -1037, -1037,   972,  1215,   123,  5946,
    5946,  5946,  5946,  5946,  5946, -1037, -1037, -1037, -1037,   367,
     496,   511,   519,   524,   537,   550,   559,   593,   623,   635,
     636,   499,   500, -1037,  1220, -1037,  1072,  1181,  1221, -1037,
    1074,  1076,  1181, -1037,  1223,  1083, -1037, -1037, -1037, -1037,
    1224,  1225,  1226, -1037, -1037, -1037,  1227,  5946,  1229, -1037,
    4873,   944, -1037, -1037,  1233,  1321,  1325, -1037, -1037,  1086,
   -1037,  5946, -1037, -1037,  1188, -1037, -1037,   945,  1235,  1236,
      -7, -1037,  1088,  1242,    38,    38,    38, -1037,  1406, -1037,
      -4, -1037,   452,   286, -1037,   364,  5946,  5946,  5946,  5946,
    5946,  5946,  5946,  5946, -1037, -1037, -1037,  1407,  5946,   116,
    1412,  1431,    38,  1257, -1037, -1037,    38,  1276,  5946, -1037,
   -1037, -1037,   447,   491,    64,   117,  1087,  1277,  1278,   933,
   -1037, -1037,  1188,  1188,  1450,  1468,  1469,  1320,   520, -1037,
     637,   651,   652,   664,   691,   693, -1037, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,  5946,  5946,
   -1037, -1037, -1037, -1037, -1037, -1037,  1299, -1037, -1037, -1037,
   -1037, -1037,  5946,  1092, -1037, -1037,  5946,  5946,  4873,   955,
   -1037, -1037,  5946, -1037, -1037, -1037, -1037,    38,    38, -1037,
   -1037, -1037, -1037, -1037, -1037,  5946,  5946,  5946, -1037, -1037,
   -1037,   694,   695,   699,   705,   711,   737,   739,   755, -1037,
   -1037,  1471,  5946, -1037, -1037, -1037, -1037, -1037,  5946,  1306,
   -1037,   142,  1409, -1037,   158, -1037,  1475,  1476,  1315,  5946,
   -1037,  1477,  1480,  1317,  5946, -1037,   210,   282,  1322,  1329,
   -1037, -1037,  1319,   528, -1037, -1037, -1037, -1037, -1037,  5946,
    5946, -1037, -1037, -1037, -1037, -1037, -1037,   760,   761, -1037,
    1328,  1331, -1037, -1037,  1094, -1037,   956,  1250, -1037,   764,
     772,   791, -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037,
   -1037, -1037,  1346,    38,  1341,  1100,  1314,  2588,  1341,  1103,
    1348,  1349, -1037,  1351,   216,   265, -1037,  1352,  1514,  1517,
    1357,  5946, -1037,  1520,  1522,  1359,  5946, -1037, -1037, -1037,
     190,   206, -1037,   797,   799, -1037, -1037, -1037, -1037,  1360,
   -1037,    38, -1037, -1037, -1037, -1037,  5946, -1037, -1037,  1341,
   -1037,   457,  5518,  5653,  2889, -1037,   510, -1037,  1341, -1037,
   -1037, -1037,  5946, -1037,  5946, -1037, -1037,  1363,  1364, -1037,
    1366,  1368,  1369, -1037,  1370,  1341,  1105,  1341,  1115, -1037,
   -1037, -1037, -1037,  1371, -1037,  5946,   689,   322, -1037,   366,
    5946,  5946,  5946,  5946,  5946,  5946,  5946,  5946, -1037, -1037,
    1372,  1379, -1037, -1037, -1037, -1037, -1037, -1037, -1037,  1341,
   -1037,  1341,    38,   805,  5946,  5946,  5946, -1037, -1037, -1037,
     807,   813,   816,   817,   818,   843,   844,   846, -1037, -1037,
   -1037, -1037, -1037, -1037,   848,   856,   859, -1037, -1037, -1037,
   -1037, -1037, -1037, -1037, -1037, -1037, -1037, -1037
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   503,   504,   505,   506,   508,   509,
     516,   517,   535,   534,   536,   537,     0,     0,   518,   519,
     520,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   525,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     3,     4,   102,   103,   115,   116,   117,
     118,   104,   105,   125,   126,   127,   129,   128,   547,     0,
     349,   351,   356,   358,   386,   388,   397,   399,   407,   411,
     414,   422,   435,   441,   443,   459,   546,   495,     0,     0,
     512,   513,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,   515,     0,     0,     0,
       0,   509,   516,   517,   535,   534,     0,   547,   439,   459,
     546,   495,   509,   516,   542,     0,     0,    43,     0,   496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   407,     0,     0,
     548,   347,   346,     0,    57,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   152,
       0,   155,     0,     0,   119,     0,     0,   120,     0,     0,
       0,     3,     0,   492,   494,   106,     3,   111,   488,    59,
       0,   222,   226,     0,     0,     0,   440,     0,     0,   113,
     444,     0,   510,   511,   438,   437,   436,   446,   458,   451,
     452,   453,   454,   457,   455,   456,   449,   450,   447,   448,
     487,   499,     0,   538,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   341,   342,   343,   344,   345,   339,
     466,     0,     0,     0,   340,     0,     0,     0,     0,     0,
       0,    47,   471,     0,   470,   534,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   543,    36,     0,   499,     0,     0,    46,
     498,     0,     0,     6,     0,     0,    17,     0,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,     0,     0,     0,
      51,     0,     0,   133,     0,   145,     0,     0,   164,     0,
       0,     0,     3,   175,   281,   286,     0,   299,   301,   303,
       0,    54,     0,     0,     0,    56,     0,     0,    62,     0,
       0,     0,     0,   169,     0,     0,     0,     0,     0,   148,
       0,   151,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   489,   491,   493,     0,     0,     0,     0,    58,
       0,   229,     0,     0,     0,   268,     3,   114,     0,   550,
       0,   500,     0,     0,   539,   306,   516,   517,   535,     0,
       0,     0,     0,     0,     0,   305,     0,   459,   495,     0,
       0,     0,     0,     0,     0,     0,     0,   348,   352,     0,
     355,   354,   353,   357,   359,   368,   371,   372,   367,   366,
     365,   364,   363,   362,   361,   360,   369,   370,   373,   377,
     378,   379,   380,   381,   374,   375,   376,   382,   383,   384,
     385,   387,   395,   394,   393,   392,   391,   390,   389,   396,
     398,   405,   404,   403,   402,   401,   400,   406,   410,   409,
     408,   412,   413,   420,   419,   418,   417,   416,   415,   421,
     431,   433,   434,   432,   428,   427,   426,   425,   424,   423,
     429,   430,   442,   482,     0,     0,   479,    11,    69,     0,
      95,    96,    97,    98,    99,    66,   465,    12,     0,    22,
       0,     0,     0,     0,   521,    13,   526,   527,    72,     0,
     474,     0,     0,   274,   463,     0,   461,   498,     0,     0,
       0,   482,   479,   341,   342,   343,   344,   345,   339,   340,
     544,     0,    37,    45,     0,   497,     0,     0,     0,     0,
       0,     0,   506,   534,     0,     0,     0,     0,     0,    33,
       0,    34,     0,    35,     0,   180,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   500,     0,     0,   136,   500,     0,     0,   400,
     406,     0,     0,     0,     0,     0,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,   460,
       0,     0,     0,     0,     0,   500,     0,   500,     0,     0,
       0,     0,   121,   122,   123,   124,   338,   490,   107,     0,
       0,     0,     0,   108,     0,     0,   229,     0,     0,     0,
       0,     0,     0,     0,   445,   486,     0,   501,     0,   540,
       0,     0,     0,     0,     0,     0,     0,   331,     0,     0,
       0,   332,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,     0,    78,    80,    82,    85,
      83,    84,    79,    81,     0,     0,     0,     0,    16,   475,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   531,   533,     0,   530,   532,     0,   473,     0,   469,
       0,     0,   272,     0,     0,     0,     0,   521,   526,   527,
     545,    44,     7,     5,     8,     0,    19,     0,     9,     0,
       0,     0,    26,     0,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    86,    89,    88,    92,
      90,    91,    87,    49,     0,    93,    94,    52,   130,   135,
     134,     0,     0,   144,   161,     0,   163,     0,     0,     0,
       0,   174,     0,     0,     0,     0,   186,     0,   282,   295,
       0,     0,   289,   290,   549,   291,   287,     0,   300,   302,
     304,    55,    60,    61,    64,    65,    39,    38,     0,     0,
     165,   528,   529,     0,     0,   146,   149,   150,   153,   154,
       0,     0,   167,   110,     0,     0,   227,   228,     0,     0,
       0,     0,   224,   185,     0,     0,     0,     0,     0,     0,
       0,   267,   225,   485,   502,   541,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   316,   323,   329,   330,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   482,   479,   350,     0,   481,     0,   484,     0,   478,
       0,     0,   483,    68,     0,     0,    23,   524,   523,   522,
       0,     0,     0,   472,   467,   468,     0,     0,     0,   464,
       0,     0,   278,   277,     0,     0,     0,    10,    28,     0,
      29,     0,    32,   179,     0,    41,    40,     0,   502,   502,
       0,   137,     0,   138,     0,     0,     0,   171,     0,   177,
       0,   173,     0,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   284,   297,     0,     0,   292,
       0,     0,     0,     0,    76,    77,     0,     0,     0,   168,
     109,   223,     0,     0,     0,     0,     0,     0,     0,     0,
     184,   182,     0,     0,     0,     0,     0,   521,   527,   326,
       0,     0,     0,     0,     0,     0,   334,   335,   336,   337,
     308,   310,   312,   315,   313,   314,   309,   311,     0,     0,
      70,   480,    71,   477,   476,    67,   502,    73,    74,    75,
     273,   275,     0,     0,    20,    27,     0,     0,     0,     0,
     178,    14,     0,    50,    53,   132,   131,   500,     0,   142,
     162,   159,   160,   172,   176,     0,     0,     0,   190,   191,
     188,     0,     0,     0,     0,     0,     0,     0,     0,   283,
     296,     0,     0,   293,   294,   170,   166,   147,     0,     0,
     158,     0,     3,   261,     0,   258,     0,     0,     0,     0,
     235,     0,     0,     0,     0,   230,     0,     0,     0,     0,
     231,   232,     0,     0,   181,   183,   269,   270,   271,     0,
       0,   317,   319,   322,   320,   321,   318,     0,     0,    24,
       0,   502,   280,   279,     0,    30,     0,   139,   140,     0,
       0,     0,   192,   194,   196,   199,   197,   198,   193,   195,
     285,   298,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   248,     0,     0,     0,   249,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,   236,   233,   234,
       0,     0,   264,     0,     0,   324,   325,   276,    21,   502,
      15,     0,   143,   200,   202,   201,     0,   157,   260,     0,
     204,   535,     0,     0,     0,   203,     0,   257,     0,   246,
     247,   239,     0,   242,     0,   245,   238,     0,     0,   250,
       0,     0,     0,   251,     0,     0,     0,     0,     0,   327,
     328,    31,   141,     0,   259,     0,     0,     0,   208,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   206,   256,
       0,     0,   254,   255,   241,   252,   253,   240,   266,     0,
     263,     0,     0,     0,     0,     0,     0,   209,   210,   207,
       0,     0,     0,     0,     0,     0,     0,     0,   243,   244,
     265,   262,   156,   205,     0,     0,     0,   211,   213,   215,
     218,   216,   217,   212,   214,   219,   221,   220
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1037, -1037,     1, -1037,  -668,  -844, -1037, -1037, -1037, -1037,
   -1037,   295,  1523, -1037, -1037, -1037,   682, -1037,  -351, -1036,
     -28, -1037, -1037, -1037, -1037,   824, -1037, -1037,  -574,   915,
    -419,   374,  1251,    -2,  -255, -1037,   814,  1300,   906,  1274,
   -1037,   619,   -11,   840,   993,   773,   204, -1037,  -148,  -323,
   -1037,     9,   224,    10,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   246,    74,    75,   675,    76,    77,    78,    79,
      80,  1139,    81,    82,   468,   700,   860,    83,   423,  1143,
     212,    84,    85,    86,   470,   727,    87,   732,   358,   645,
     430,   137,   199,   149,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   221,   240,
     104,   139,   150,   140,   141
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,     2,   682,   509,  1001,   849,   583,  1145,   586,   396,
     804,   105,   106,   107,   168,   145,   859,   167,    28,   724,
    1008,    26,   159,   161,   163,   165,   166,   213,   725,   173,
     175,   177,   217,   218,   181,   182,   686,   800,   186,   188,
     621,   622,   655,   806,   128,   684,   203,   204,   361,   416,
     208,   210,   129,   729,   130,   151,   152,   153,   192,   154,
     721,   722,   888,   889,   466,   467,   362,   424,   425,   242,
     244,   426,   424,   425,   985,   749,   426,   155,   193,  1146,
     909,   910,   156,   986,   911,   650,   870,   195,  1147,   424,
     425,   214,   857,   426,   473,   750,   909,   910,     3,   651,
     911,   245,   687,   688,   424,   425,   332,   196,   426,   215,
     334,   335,   336,   337,   338,   339,   433,   342,   344,   346,
     347,   190,   349,   146,  1027,   350,    73,  1232,   427,   428,
     429,   371,  1151,   427,   428,   429,   434,   304,   305,  1148,
    1131,  1152,   417,   752,   375,    73,   377,   652,   474,   360,
     427,   428,   429,   424,   425,   180,   397,   426,   407,  1109,
     412,   653,   414,   753,   183,   427,   428,   429,  1248,   424,
     425,    73,  1257,   426,   306,   408,   184,   422,    73,   169,
     776,   422,   805,   448,    73,   450,   685,   777,   455,   726,
     456,   457,  1153,   458,   459,   409,   460,   462,   463,   460,
     464,   424,   425,   912,   723,   426,   890,   960,   408,   194,
     129,   612,   130,  1284,   427,   428,   429,   424,   425,  1041,
     138,   426,  1299,    73,   157,  1218,   194,   871,   409,  1149,
     427,   428,   429,   751,  1219,   831,   198,   681,   197,  1308,
     436,  1310,   728,  1150,   496,   499,   500,   501,   502,   503,
     504,   505,   506,   858,   507,   497,   106,   498,   540,   830,
     437,   216,   427,   428,   429,  1028,   206,   435,   677,   781,
     224,   225,   226,  1330,   915,  1331,   782,   122,   427,   428,
     429,  1132,  1154,   110,   111,  1220,   551,   552,   553,   554,
     555,   556,   557,   211,   197,   444,  1155,  1223,   304,   305,
     446,   754,   608,   129,   755,   130,  1224,   756,   757,  1059,
     169,   916,   917,   445,   408,    73,   433,   758,   447,   220,
     759,   760,   855,  1204,   761,   762,   410,   820,   584,   411,
     861,   452,  1105,   936,   409,   306,   434,   123,   372,  1208,
     373,   598,   341,  1242,   918,   919,   920,   791,   792,   453,
     408,   421,   245,   118,   359,   124,   422,  1225,   618,   410,
     620,   222,   411,   623,   624,   625,   626,   627,   628,   629,
     409,  1275,   378,  1029,   223,  1221,    88,   913,   635,   636,
     637,  1262,   638,   640,   256,   641,   646,  1277,   381,  1222,
     262,   438,   433,   793,   433,  1263,   660,   661,   662,   663,
     664,   665,   666,   170,   172,   654,   669,   670,   679,   680,
     422,   775,   434,   296,   434,   380,   189,   191,    73,   129,
     301,   130,   328,   683,   205,   207,   776,   119,   302,   303,
    1264,   692,   693,   777,   694,   695,   219,   696,   697,  1030,
    1031,   951,   952,   702,  1265,   704,   382,  1226,   330,   921,
     708,   709,   483,   929,   484,   255,   399,   930,   122,   717,
     383,  1227,   763,   329,   719,   410,   931,   435,   411,   932,
     933,   331,  1119,   934,   938,   304,   305,   733,  1016,   993,
    1115,  1017,  1018,   399,  1116,   120,   333,   744,   745,   746,
     748,  1019,   348,   401,  1020,  1021,   402,   403,  1022,  1023,
    1117,   410,   379,   121,   411,   380,   125,   126,  1318,  1009,
     170,  1011,   306,   263,   264,   265,   266,   351,   123,   953,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   363,   384,   360,   742,   413,  1290,   415,
     418,  1291,  1292,   435,   387,   435,   712,   119,   125,   126,
    1120,  1293,  1319,  1066,  1294,  1295,   647,   301,  1296,  1297,
     449,   127,   451,  1050,  1051,   419,   420,   125,   126,   364,
     257,   258,   267,   268,   269,   270,   271,   272,   273,   393,
     274,   398,   304,   305,   399,   400,  1043,   794,   795,   178,
     275,   276,   385,   179,   401,   479,   631,   402,   403,   787,
     788,   404,   388,   743,   405,   120,   386,   406,   307,   308,
     802,   935,   304,   305,   648,   635,   389,   441,   352,   306,
      88,   353,  1285,   741,   259,   260,   261,  1141,   649,   255,
     667,   405,  1142,   796,   406,   671,  1024,   815,  1118,   794,
     795,   678,   242,  1100,   439,   825,   826,   780,   828,   306,
     829,   537,   538,   539,   832,   833,   691,   247,   211,  1114,
     248,   249,   781,   440,  1078,  1079,   699,   442,   354,   782,
     250,  1144,   852,   251,   252,   713,  1142,   253,   254,   776,
     781,   867,  1067,   869,   465,  1170,   777,   782,   431,   255,
     714,   432,   105,   106,   107,   937,  1298,  1068,   715,   481,
     482,  1164,  1165,   766,   736,  1069,   738,   740,  1231,   901,
    1070,   125,   126,  1142,   424,   425,   767,  1314,   426,   904,
     399,  1315,  1205,  1071,   394,  1209,  1163,   443,   395,   768,
     401,    89,   405,   402,   403,   406,  1072,  1316,   769,   926,
     927,   928,   105,   106,   107,  1073,   431,   939,   940,   432,
     941,   942,   469,   943,   944,   945,   946,   947,   948,   949,
     950,   108,   109,   472,   110,   111,   112,   113,   114,   115,
     116,   478,   770,   954,    73,   427,   428,   429,   958,  1074,
     471,   110,   111,   365,   366,   367,   368,   369,   970,   599,
     816,   971,   600,   817,   972,   288,   289,   290,   291,   292,
     293,   976,   771,   875,   876,   588,   589,   877,   255,  1075,
     477,  1276,  1278,   784,   772,   773,   836,   982,   983,   117,
     480,  1076,  1077,  1171,   786,   989,   475,   789,   294,   295,
     838,   839,   997,   798,   118,   801,   370,  1172,  1173,   718,
     603,   720,   604,   840,   309,   310,   311,   312,   313,  1002,
    1174,   118,   277,   278,   279,   280,   281,   282,   283,   284,
     285,  1012,  1013,  1015,   878,   879,   880,   286,   405,  1026,
     841,   406,   842,   766,   767,  1317,   485,  1175,   768,  1176,
    1192,  1193,   476,   835,   769,  1194,  1032,   314,   315,   587,
     770,  1195,   844,   390,   391,   392,   316,  1196,  1037,   317,
     318,   319,   320,   321,   322,   323,   324,   542,   543,   544,
     545,   546,   547,   548,   549,   550,   771,   822,   772,   287,
     823,   596,   380,  1197,   227,  1198,   590,  1060,  1061,  1062,
    1063,  1064,  1065,   228,   773,   229,   230,   601,   602,  1080,
    1082,  1199,   591,   836,   231,   232,  1235,  1236,   227,   233,
    1243,   838,   325,   326,   327,   234,   235,   228,  1244,   229,
     230,   614,   615,   236,   237,   238,   239,   592,   231,   232,
     842,   616,   617,   233,  1042,  1091,  1087,  1245,  1089,   234,
     235,   634,   380,  1279,   608,  1280,   766,   236,   237,   238,
     239,  1333,   767,  1337,   593,   768,   769,   770,  1162,  1338,
     956,   957,  1339,  1340,  1341,   961,   962,  1044,   594,   965,
    1045,  1046,  1047,  1048,  1121,  1122,  1123,  1124,  1125,  1126,
    1127,  1128,   771,   772,   595,   773,  1130,   836,   227,  1342,
    1343,   585,  1344,   597,  1345,   838,  1140,   228,   842,   229,
     230,   981,  1346,   658,   478,  1347,   848,   850,   231,   232,
     853,   854,   856,   233,   603,   843,   807,    88,   380,   234,
     235,   297,   298,   874,   605,   299,   300,   236,   237,   238,
     239,   508,   606,   510,   511,   512,  1177,  1178,   607,   895,
     896,   897,   563,   564,   565,   566,   567,   568,   569,   847,
    1180,   608,   380,   902,  1182,  1183,   905,   609,  1156,   881,
     227,  1157,   380,  1158,  1159,   966,   610,    88,   380,   228,
     611,   229,   230,  1189,  1190,  1191,   990,   481,   659,   991,
     231,   232,   613,  1094,  1101,   233,   380,  1102,   481,   668,
    1201,   234,   235,   619,  1185,  1240,  1202,   380,   380,   236,
     237,   238,   239,  1207,   558,   559,   560,  1213,   380,   380,
     810,   925,  1217,   380,   603,   969,  1057,   672,   482,   673,
     674,   676,   482,   689,   690,   705,   482,  1233,  1234,   514,
     515,   516,   517,   518,   519,   520,   521,   522,   523,   524,
     525,   526,   527,   528,   529,   530,   531,   532,   533,   534,
     535,   536,   707,   482,   632,   481,   785,   862,   690,   914,
     690,   923,   380,   633,  1093,  1256,   924,   380,   964,   380,
     973,   380,   974,   615,   656,  1099,   105,   106,   107,  1270,
     975,   380,   979,   615,  1274,   657,  1003,   791,   792,   794,
     795,   481,   980,   984,   823,   994,   690,   998,   380,   999,
     380,  1000,   674,   701,  1140,  1052,   690,  1053,   690,   698,
    1286,  1287,  1289,  1081,   380,  1083,   615,  1084,   380,   703,
    1300,   706,  1301,  1033,  1086,   380,   710,   481,  1098,  1107,
     482,  1108,   674,  1181,   380,  1239,   380,   711,  1039,  1241,
     674,  1249,   690,  1313,  1258,   690,  1309,   690,  1320,  1321,
    1322,  1323,  1324,  1325,  1326,  1327,  1311,   690,   200,   202,
     561,   562,   712,   713,   714,   715,   716,   730,   731,   734,
     764,   765,  1334,  1335,  1336,   766,   767,   768,   769,   770,
     771,   772,  1184,   773,   774,   287,  1186,   778,   779,   803,
     790,   808,   809,   811,   812,   813,   814,   818,   819,   824,
     834,   836,   868,   872,   873,   837,   838,   839,   840,   841,
     842,   845,   846,   882,   891,   883,   884,   885,   886,   887,
     893,   900,   906,   380,   898,   907,   899,   977,   892,   894,
     903,   978,   963,   422,  1106,   967,  1004,   987,  1110,  1111,
    1112,   968,   988,   992,   995,   996,  1005,  1006,  1007,  1025,
    1034,  1036,  1035,  1038,  1040,  1054,  1055,  1049,  1056,  1080,
    1082,  1058,  1085,  1087,  1088,  1089,  1135,  1096,  1090,  1092,
    1137,  1097,  1095,  -112,  1103,  1104,  -112,  -112,  -112,  -112,
    -112,  1113,  1129,  1133,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  1134,  1136,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  1138,  1160,  1161,  1166,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  1167,  1168,  1179,  -112,
    -112,  1187,  1188,  -112,  -112,  1169,  1200,  1203,  1206,  -112,
    1210,  1211,  1214,  -112,  1212,  1215,  1216,  -112,  -112,  1230,
    1250,  1228,  -112,     4,     5,     6,     7,     8,  1229,  1237,
    1238,   142,   143,   133,   134,   355,    14,    15,    16,    17,
      18,    19,    20,  -112,  -112,  1246,  1142,  1259,  1260,  1267,
    1261,  1266,  1268,   136,    30,  1271,  1269,  1272,  1273,  1281,
      36,  1283,  1302,  1303,  1010,  1304,    38,  1305,  1306,  1307,
     908,  1328,  1312,   171,    46,    47,    48,    49,  1329,   821,
      52,   541,   513,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,  1247,     0,  -112,
       0,    88,     0,     0,     0,  -112,  -112,     0,     0,    63,
       0,  -112,  -112,  -112,     0,     0,  -112,     0,  -112,  -112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,  1282,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   355,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,   136,
      30,     0,    67,    68,     0,     0,    36,     0,    69,     0,
      71,   356,    38,    72,     0,    73,  1332,   357,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    63,   142,   143,   133,   134,
     355,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   136,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,     0,    71,   799,     0,    72,
       0,    73,     0,   357,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   355,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,   136,    30,     0,    67,    68,     0,
       0,    36,     0,    69,     0,    71,   959,    38,    72,     0,
      73,     0,   357,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      63,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,     0,     0,    72,     0,    73,     0,   357,     4,
       5,     6,     7,     8,     0,     0,     0,     9,   486,   487,
     488,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,   489,   490,    26,     0,     0,    27,    28,    29,
      30,   491,   492,    33,    34,    35,    36,    37,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,   493,   494,
      46,    47,    48,    49,    50,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,    60,     0,     0,     0,
      61,    62,     0,     0,     0,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,    70,    71,     0,     0,    72,
       0,    73,   495,     4,     5,     6,     7,     8,     0,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,   863,   864,    26,     0,
       0,    27,    28,    29,    30,    31,   865,    33,    34,    35,
      36,    37,     0,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,     0,     0,     0,    55,    56,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
      60,     0,     0,     0,    61,    62,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,     0,    69,    70,
      71,     0,     0,    72,     0,    73,   866,     4,     5,     6,
       7,     8,     0,     0,     0,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     0,     0,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,     0,     0,     0,     0,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,     0,     0,     0,
      55,    56,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,    60,     0,     0,     0,    61,    62,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,    70,    71,     0,     0,    72,     0,    73,
     922,     4,     5,     6,     7,     8,     0,     0,     0,     9,
      10,    11,  1251,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,  1252,  1253,    26,     0,     0,    27,
      28,    29,    30,    31,  1254,    33,    34,    35,    36,    37,
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
       0,    72,     0,    73,  1255,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,   174,    71,    38,     0,    72,     0,
      73,   747,     0,     0,    46,    47,    48,    49,     0,     0,
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
       0,     0,    67,    68,     0,     0,     0,     0,    69,   174,
      71,     0,     0,    72,     0,    73,  1014,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,   174,    71,
       0,     0,    72,     0,    73,  1288,     4,     5,     6,     7,
       8,     0,     0,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,     0,     0,    27,    28,    29,    30,    31,    32,
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
       0,    69,    70,    71,     0,     0,    72,     0,    73,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   136,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
      71,    38,     0,    72,   144,    73,     0,     0,     0,    46,
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
       0,     0,     0,    69,   147,   148,     0,     0,    72,     0,
      73,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   136,    30,     0,     0,    67,    68,     0,    36,
       0,     0,    69,   174,    71,    38,     0,    72,     0,    73,
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
       0,    67,    68,     0,     0,     0,     0,    69,   176,    71,
       0,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   136,    30,     0,     0,
      67,    68,     0,    36,     0,     0,    69,   209,    71,    38,
       0,    72,     0,    73,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    63,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,   136,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    63,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,     0,    71,   241,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
     136,    30,     0,     0,    67,    68,     0,    36,     0,     0,
      69,     0,    71,    38,     0,    72,   243,    73,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    63,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,   136,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    63,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,     0,    71,   340,     0,
      72,     0,    73,     0,     0,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,   136,    30,     0,     0,    67,    68,
     345,    36,     0,     0,    69,     0,    71,    38,     0,    72,
       0,    73,     0,     0,     0,    46,    47,    48,    49,     0,
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
     374,    71,     0,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,   454,     0,     0,     0,     0,    66,     0,     0,   136,
      30,     0,    67,    68,     0,     0,    36,     0,    69,     0,
      71,   376,    38,    72,     0,    73,     0,     0,     0,     0,
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
       0,     0,     0,     0,    69,     0,    71,     0,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,     0,    71,    38,   461,    72,     0,
      73,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,   245,
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
      71,     0,     0,    72,     0,    73,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,     0,    71,
      38,     0,    72,   630,    73,     0,     0,     0,    46,    47,
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
       0,     0,    69,     0,    71,   735,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,   136,    30,     0,    67,    68,     0,     0,    36,
       0,    69,     0,    71,   737,    38,    72,     0,    73,     0,
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
       0,     0,    72,   739,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,   136,    30,     0,
      67,    68,     0,     0,    36,     0,    69,     0,    71,   783,
      38,    72,     0,    73,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     4,     5,     6,   642,
       8,     0,     0,    63,   142,   143,   133,   134,   643,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,    63,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,     0,    71,   797,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,   136,    30,     0,    67,    68,     0,     0,    36,
       0,    69,     0,    71,   241,    38,    72,     0,    73,     0,
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
     955,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   136,    30,     0,     0,
      67,    68,     0,    36,     0,     0,    69,     0,   158,    38,
       0,    72,     0,    73,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    63,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,   136,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    63,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,     0,   160,     0,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
     136,    30,     0,     0,    67,    68,     0,    36,     0,     0,
      69,     0,   162,    38,     0,    72,     0,    73,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    63,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,   136,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    63,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,     0,   164,     0,     0,
      72,     0,    73,     0,     0,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,   136,    30,     0,     0,    67,    68,
       0,    36,     0,     0,    69,     0,    71,    38,     0,    72,
       0,    73,     0,     0,     0,    46,    47,    48,    49,     0,
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
       0,   185,     0,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   136,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
     187,    38,     0,    72,     0,    73,     0,     0,     0,    46,
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
       0,     0,     0,    69,     0,    71,     0,     0,    72,     0,
     201,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     142,   143,   133,   134,   135,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   136,    30,     0,     0,    67,    68,     0,    36,
       0,     0,    69,     0,   343,    38,     0,    72,     0,    73,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     4,     5,     6,   642,     8,     0,     0,    63,   142,
     143,   133,   134,   643,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,   136,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    63,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,   639,
       0,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   136,    30,     0,     0,
      67,    68,     0,    36,     0,     0,    69,     0,   644,    38,
       0,    72,     0,    73,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    63,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,   136,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    63,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,     0,   827,     0,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   131,   132,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
     136,     0,     0,     0,    67,    68,     0,    36,     0,     0,
      69,     0,   851,    38,     0,    72,     0,    73,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,     0,    71,     0,     0,
      72,     0,    73
};

static const yytype_int16 yycheck[] =
{
       2,     0,   421,   258,   848,   673,   329,  1043,   331,   157,
      15,     2,     2,     2,    15,    17,    24,    28,    32,    15,
      24,    28,    24,    25,    26,    27,    28,    55,    24,    31,
      32,    33,    60,    61,    36,    37,    11,   611,    40,    41,
     363,   364,   393,   617,    12,    24,    48,    49,   167,    89,
      52,    53,   178,   472,   180,    11,    12,    13,    28,    15,
      42,    43,    42,    43,    42,    43,   185,    11,    12,    71,
      72,    15,    11,    12,     6,    28,    15,    33,    48,    15,
      63,    64,    38,    15,    67,   165,    24,    28,    24,    11,
      12,    15,    89,    15,    15,    48,    63,    64,     0,   179,
      67,    79,    77,    78,    11,    12,   108,    48,    15,    33,
     112,   113,   114,   115,   116,   117,    28,   119,   120,   121,
     122,   180,   124,    15,    24,   127,   185,  1163,    72,    73,
      74,   165,    15,    72,    73,    74,    48,   134,   135,    75,
      24,    24,   182,    28,   146,   185,   148,   165,    69,   183,
      72,    73,    74,    11,    12,    15,   158,    15,   160,  1003,
     162,   179,   164,    48,    13,    72,    73,    74,  1204,    11,
      12,   185,  1208,    15,   171,    28,    13,   185,   185,   180,
     180,   185,   187,   185,   185,   187,   165,   187,   190,   185,
     192,   193,    75,   195,   196,    48,   198,   199,   200,   201,
     202,    11,    12,   186,   186,    15,   186,   781,    28,   179,
     178,    86,   180,  1249,    72,    73,    74,    11,    12,   186,
      16,    15,  1258,   185,   180,    15,   179,   165,    48,   165,
      72,    73,    74,   186,    24,   654,   185,   181,   179,  1275,
      28,  1277,   181,   179,   246,   247,   248,   249,   250,   251,
     252,   253,   254,    89,   256,   246,   246,   246,   286,   181,
      48,    57,    72,    73,    74,   165,   185,   179,   416,   180,
      66,    67,    68,  1309,   181,  1311,   187,   107,    72,    73,
      74,   165,   165,   110,   111,    75,   297,   298,   299,   300,
     301,   302,   303,    15,   179,   167,   179,    15,   134,   135,
     167,   186,   179,   178,    28,   180,    24,    31,    32,   186,
     180,   730,   731,   185,    28,   185,    28,    41,   185,    11,
      44,    45,    89,   181,    48,    49,   179,    86,   330,   182,
     681,    31,  1000,   186,    48,   171,    48,   167,   182,   181,
     184,   343,   118,  1187,    81,    82,    83,   119,   120,    49,
      28,   180,    79,   180,   130,   185,   185,    75,   360,   179,
     362,    11,   182,   365,   366,   367,   368,   369,   370,   371,
      48,   181,   148,    11,    11,   165,     2,   728,   380,   381,
     382,   165,   384,   385,    86,   387,   388,   181,   107,   179,
     103,   179,    28,   165,    28,   179,   398,   399,   400,   401,
     402,   403,   404,    29,    30,   180,   408,   409,   419,   420,
     185,   165,    48,   117,    48,   182,    42,    43,   185,   178,
      79,   180,   164,   422,    50,    51,   180,   107,    87,    88,
     165,   433,   434,   187,   436,   437,    62,   439,   440,    77,
      78,   764,   765,   445,   179,   447,   165,   165,   165,   186,
     452,   453,   182,    28,   184,   179,    31,    32,   107,   461,
     179,   179,   186,   178,   466,   179,    41,   179,   182,    44,
      45,   178,   186,    48,   186,   134,   135,   476,    28,   830,
      28,    31,    32,    31,    32,   165,    15,   489,   490,   491,
     492,    41,    15,    41,    44,    45,    44,    45,    48,    49,
      48,   179,   179,   183,   182,   182,   110,   111,   186,   860,
     136,   862,   171,    90,    91,    92,    93,   179,   167,   774,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   178,   107,   183,   185,   163,    28,   165,
     166,    31,    32,   179,   107,   179,   179,   107,   110,   111,
     186,    41,   186,   186,    44,    45,   107,    79,    48,    49,
     186,   165,   188,   914,   915,    87,    88,   110,   111,   178,
     104,   105,   149,   150,   151,   152,   153,   154,   155,    15,
     157,    28,   134,   135,    31,    32,   909,   119,   120,    11,
     167,   168,   165,    15,    41,   221,   372,    44,    45,   601,
     602,    48,   165,   165,   179,   165,   179,   182,   162,   163,
     612,   186,   134,   135,   165,   617,   179,    76,    15,   171,
     246,    18,   165,   183,   158,   159,   160,   180,   179,   179,
     406,   179,   185,   165,   182,   411,   186,   639,   186,   119,
     120,   417,   644,   994,   165,   647,   648,   165,   650,   171,
     652,     8,     9,    10,   656,   657,   432,    28,    15,  1010,
      31,    32,   180,   165,   165,   165,   442,   182,    65,   187,
      41,   180,   674,    44,    45,   179,   185,    48,    49,   180,
     180,   683,   186,   685,    59,   165,   187,   187,   179,   179,
     179,   182,   683,   683,   683,   186,   186,   186,   179,   181,
     182,  1052,  1053,   179,   480,   186,   482,   483,   180,   711,
     186,   110,   111,   185,    11,    12,   179,    28,    15,   721,
      31,    32,  1141,   186,    11,  1144,  1049,   185,    15,   179,
      41,   733,   179,    44,    45,   182,   186,    48,   179,   741,
     742,   743,   733,   733,   733,   186,   179,   749,   750,   182,
     752,   753,   179,   755,   756,   757,   758,   759,   760,   761,
     762,   107,   108,   180,   110,   111,   112,   113,   114,   115,
     116,   182,   179,   775,   185,    72,    73,    74,   780,   186,
     185,   110,   111,   112,   113,   114,   115,   116,   790,   179,
     179,   793,   182,   182,   796,   136,   137,   138,   139,   140,
     141,   803,   179,    11,    12,   179,   180,    15,   179,   186,
     179,  1230,  1231,   589,   179,   179,   179,   819,   820,   165,
     180,   186,   186,   186,   600,   827,   185,   603,   169,   170,
     179,   179,   834,   609,   180,   611,   165,   186,   186,   465,
     182,   467,   184,   179,   129,   130,   131,   132,   133,   851,
     186,   180,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   863,   864,   865,    72,    73,    74,   109,   179,   871,
     179,   182,   179,   179,   179,   186,   186,   186,   179,   186,
     186,   186,   185,   659,   179,   186,   888,   172,   173,   179,
     179,   186,   668,    11,    12,    13,   118,   186,   900,   121,
     122,   123,   124,   125,   126,   127,   128,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   179,   179,   179,   161,
     182,   181,   182,   186,    15,   186,   179,   929,   930,   931,
     932,   933,   934,    24,   179,    26,    27,   172,   173,   179,
     179,   186,   179,   179,    35,    36,   186,   186,    15,    40,
     186,   179,   174,   175,   176,    46,    47,    24,   186,    26,
      27,   181,   182,    54,    55,    56,    57,   179,    35,    36,
     179,   181,   182,    40,    65,   977,   179,   186,   179,    46,
      47,   181,   182,   186,   179,   186,   179,    54,    55,    56,
      57,   186,   179,   186,   179,   179,   179,   179,    65,   186,
     776,   777,   186,   186,   186,   781,   782,    11,   179,   785,
      14,    15,    16,    17,  1016,  1017,  1018,  1019,  1020,  1021,
    1022,  1023,   179,   179,   179,   179,  1028,   179,    15,   186,
     186,    18,   186,   179,   186,   179,  1038,    24,   179,    26,
      27,   817,   186,   181,   182,   186,   672,   673,    35,    36,
     676,   677,   678,    40,   182,   179,   184,   683,   182,    46,
      47,   143,   144,   689,   179,   147,   148,    54,    55,    56,
      57,   257,   168,   259,   260,   261,  1078,  1079,   186,   705,
     706,   707,   309,   310,   311,   312,   313,   314,   315,   179,
    1092,   179,   182,   719,  1096,  1097,   722,   180,    11,   179,
      15,    14,   182,    16,    17,   179,    15,   733,   182,    24,
     180,    26,    27,  1115,  1116,  1117,   179,   181,   182,   182,
      35,    36,    15,   179,   179,    40,   182,   182,   181,   182,
    1132,    46,    47,    15,   179,   179,  1138,   182,   182,    54,
      55,    56,    57,  1142,   304,   305,   306,  1149,   182,   182,
     184,   184,  1154,   182,   182,   184,   184,   181,   182,    29,
      30,   181,   182,   181,   182,   181,   182,  1169,  1170,   263,
     264,   265,   266,   267,   268,   269,   270,   271,   272,   273,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   181,   182,   179,   181,   182,   181,   182,   181,
     182,   181,   182,   179,   980,  1207,   181,   182,   181,   182,
     181,   182,   181,   182,   165,   991,  1207,  1207,  1207,  1221,
     181,   182,   181,   182,  1226,   165,   852,   119,   120,   119,
     120,   181,   182,   181,   182,   181,   182,   181,   182,   181,
     182,    29,    30,    15,  1246,   181,   182,   181,   182,   179,
    1252,  1253,  1254,   181,   182,   181,   182,   181,   182,    15,
    1262,    29,  1264,   889,   181,   182,    11,   181,   182,   181,
     182,    29,    30,   181,   182,   181,   182,   179,   904,    29,
      30,   181,   182,  1285,   181,   182,   181,   182,  1290,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,   181,   182,    47,    48,
     307,   308,   179,   179,   179,   179,   186,   180,   180,    11,
     178,   178,  1314,  1315,  1316,   179,   179,   179,   179,   179,
     179,   179,  1098,   179,   106,   161,  1102,   179,    15,   180,
     165,   168,   186,   179,   179,   179,   179,   179,    86,   179,
     165,   179,    15,    11,    11,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   168,   179,   179,   179,   179,   179,
     168,   165,    15,   182,   179,    15,   179,    86,   186,   186,
     186,    15,   179,   185,  1000,   184,    11,   179,  1004,  1005,
    1006,   184,   179,   179,   179,   179,    11,    11,    15,    15,
     179,    29,   179,   179,   186,    15,    15,    63,    15,   179,
     179,   186,   179,   179,   179,   179,  1032,    86,   181,   180,
    1036,    86,   179,     0,   179,   179,     3,     4,     5,     6,
       7,    15,    15,    11,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    11,   186,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,   179,   179,   179,     8,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,     8,     8,   179,    66,
      67,  1107,  1108,    70,    71,   165,    15,   181,    79,    76,
      15,    15,    15,    80,   179,    15,   179,    84,    85,   180,
     186,   179,    89,     3,     4,     5,     6,     7,   179,   181,
     179,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   110,   111,   179,   185,   179,   179,    15,
     179,   179,    15,    33,    34,    15,   179,    15,   179,   179,
      40,  1246,   179,   179,   862,   179,    46,   179,   179,   179,
     726,   179,   181,    30,    54,    55,    56,    57,   179,   644,
      60,   287,   262,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,  1203,    -1,   166,
      -1,  1207,    -1,    -1,    -1,   172,   173,    -1,    -1,    89,
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,   186,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,  1241,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,
      34,    -1,   172,   173,    -1,    -1,    40,    -1,   178,    -1,
     180,   181,    46,   183,    -1,   185,  1312,   187,    -1,    -1,
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
      -1,    -1,    -1,    -1,   178,    -1,   180,   181,    -1,   183,
      -1,   185,    -1,   187,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    33,    34,    -1,   172,   173,    -1,
      -1,    40,    -1,   178,    -1,   180,   181,    46,   183,    -1,
     185,    -1,   187,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,
      -1,   180,    -1,    -1,   183,    -1,   185,    -1,   187,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,   173,
      -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,   183,
      -1,   185,   186,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,   179,
     180,    -1,    -1,   183,    -1,   185,   186,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    -1,    -1,    -1,    -1,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    -1,    -1,    -1,
      66,    67,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    80,    -1,    -1,    -1,    84,    85,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
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
      -1,   183,    -1,   185,   186,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,
      40,    -1,    -1,   178,   179,   180,    46,    -1,   183,    -1,
     185,   186,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
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
     180,    -1,    -1,   183,    -1,   185,   186,    -1,    -1,   110,
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
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,
      -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,    -1,
     180,    46,    -1,   183,   184,   185,    -1,    -1,    -1,    54,
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
     185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,
      -1,    -1,   178,   179,   180,    46,    -1,   183,    -1,   185,
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
      -1,   172,   173,    -1,    -1,    -1,    -1,   178,   179,   180,
      -1,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,
     172,   173,    -1,    40,    -1,    -1,   178,   179,   180,    46,
      -1,   183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    89,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,
      -1,   178,    -1,   180,   181,    -1,   183,    -1,   185,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,    -1,
     178,    -1,   180,    46,    -1,   183,   184,   185,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    89,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,   178,    -1,   180,   181,    -1,
     183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,
     174,    40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,
      -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
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
     179,   180,    -1,    -1,   183,    -1,   185,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,
      34,    -1,   172,   173,    -1,    -1,    40,    -1,   178,    -1,
     180,   181,    46,   183,    -1,   185,    -1,    -1,    -1,    -1,
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
      40,    -1,    -1,   178,    -1,   180,    46,   182,   183,    -1,
     185,    -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    79,
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
      46,    -1,   183,   184,   185,    -1,    -1,    -1,    54,    55,
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
      -1,    -1,   183,   184,   185,    -1,    -1,    -1,   110,   111,
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
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,
     172,   173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,
      -1,   183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    89,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,
      -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,    -1,
     178,    -1,   180,    46,    -1,   183,    -1,   185,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    89,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,   178,    -1,   180,    -1,    -1,
     183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,
      -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,
      -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
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
     180,    46,    -1,   183,    -1,   185,    -1,    -1,    -1,    54,
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
      -1,    -1,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,
     185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,
      -1,    -1,   178,    -1,   180,    46,    -1,   183,    -1,   185,
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
      -1,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,
     172,   173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,
      -1,   183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    89,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,
      -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      33,    -1,    -1,    -1,   172,   173,    -1,    40,    -1,    -1,
     178,    -1,   180,    46,    -1,   183,    -1,   185,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,   178,    -1,   180,    -1,    -1,
     183,    -1,   185
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
     148,    79,    87,    88,   134,   135,   171,   162,   163,   129,
     130,   131,   132,   133,   172,   173,   118,   121,   122,   123,
     124,   125,   126,   127,   128,   174,   175,   176,   164,   178,
     165,   178,   221,    15,   221,   221,   221,   221,   221,   221,
     181,   240,   221,   180,   221,   174,   221,   221,    15,   221,
     221,   179,    15,    18,    65,    15,   181,   187,   216,   240,
     183,   167,   185,   178,   178,   112,   113,   114,   115,   116,
     165,   165,   182,   184,   179,   221,   181,   221,   240,   179,
     182,   107,   165,   179,   107,   165,   179,   107,   165,   179,
      11,    12,    13,    15,    11,    15,   236,   221,    28,    31,
      32,    41,    44,    45,    48,   179,   182,   221,    28,    48,
     179,   182,   221,   219,   221,   219,    89,   182,   219,    87,
      88,   180,   185,   206,    11,    12,    15,    72,    73,    74,
     218,   179,   182,    28,    48,   179,    28,    48,   179,   165,
     165,    76,   182,   185,   167,   185,   167,   185,   221,   219,
     221,   219,    31,    49,    25,   221,   221,   221,   221,   221,
     221,   182,   221,   221,   221,    59,    42,    43,   202,   179,
     212,   185,   180,    15,    69,   185,   185,   179,   182,   219,
     180,   181,   182,   182,   184,   186,    12,    13,    14,    26,
      27,    35,    36,    52,    53,   186,   221,   239,   242,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   224,   222,
     224,   224,   224,   225,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,     8,     9,    10,
     208,   227,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     231,   232,   232,   233,   233,   233,   233,   233,   233,   233,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   237,   221,    18,   237,   179,   179,   180,
     179,   179,   179,   179,   179,   179,   181,   179,   221,   179,
     182,   172,   173,   182,   184,   179,   168,   186,   179,   180,
      15,   180,    86,    15,   181,   182,   181,   182,   221,    15,
     221,   237,   237,   221,   221,   221,   221,   221,   221,   221,
     184,   240,   179,   179,   181,   221,   221,   221,   221,   180,
     221,   221,     6,    15,   180,   217,   221,   107,   165,   179,
     165,   179,   165,   179,   180,   206,   165,   165,   181,   182,
     221,   221,   221,   221,   221,   221,   221,   240,   182,   221,
     221,   240,   181,    29,    30,   193,   181,   236,   240,   230,
     230,   181,   218,   190,    24,   165,    11,    77,    78,   181,
     182,   240,   221,   221,   221,   221,   221,   221,   179,   240,
     203,    15,   221,    15,   221,   181,    29,   181,   221,   221,
      11,   179,   179,   179,   179,   179,   186,   221,   219,   221,
     219,    42,    43,   186,    15,    24,   185,   213,   181,   218,
     180,   180,   215,   190,    11,   181,   240,   181,   240,   184,
     240,   183,   185,   165,   221,   221,   221,   186,   221,    28,
      48,   186,    28,    48,   186,    28,    31,    32,    41,    44,
      45,    48,    49,   186,   178,   178,   179,   179,   179,   179,
     179,   179,   179,   179,   106,   165,   180,   187,   179,    15,
     165,   180,   187,   181,   240,   182,   240,   221,   221,   240,
     165,   119,   120,   165,   119,   120,   165,   181,   240,   181,
     216,   240,   221,   180,    15,   187,   216,   184,   168,   186,
     184,   179,   179,   179,   179,   221,   179,   182,   179,    86,
      86,   217,   179,   182,   179,   221,   221,   180,   221,   221,
     181,   218,   221,   221,   165,   240,   179,   179,   179,   179,
     179,   179,   179,   179,   240,   179,   179,   179,   219,   192,
     219,   180,   221,   219,   219,    89,   219,    89,    89,    24,
     204,   206,   181,    26,    27,    36,   186,   221,    15,   221,
      24,   165,    11,    11,   219,    11,    12,    15,    72,    73,
      74,   179,   179,   179,   179,   179,   179,   179,    42,    43,
     186,   168,   186,   168,   186,   219,   219,   219,   179,   179,
     165,   221,   219,   186,   221,   219,    15,    15,   213,    63,
      64,    67,   186,   206,   181,   181,   218,   218,    81,    82,
      83,   186,   186,   181,   181,   184,   221,   221,   221,    28,
      32,    41,    44,    45,    48,   186,   186,   186,   186,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   237,   237,   222,   221,   181,   240,   240,   221,   181,
     216,   240,   240,   179,   181,   240,   179,   184,   184,   184,
     221,   221,   221,   181,   181,   181,   221,    86,    15,   181,
     182,   240,   221,   221,   181,     6,    15,   179,   179,   221,
     179,   182,   179,   206,   181,   179,   179,   221,   181,   181,
      29,   193,   221,   219,    11,    11,    11,    15,    24,   206,
     204,   206,   221,   221,   186,   221,    28,    31,    32,    41,
      44,    45,    48,    49,   186,    15,   221,    24,   165,    11,
      77,    78,   221,   219,   179,   179,    29,   221,   179,   219,
     186,   186,    65,   237,    11,    14,    15,    16,    17,    63,
     206,   206,   181,   181,    15,    15,    15,   184,   186,   186,
     221,   221,   221,   221,   221,   221,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   165,   165,
     179,   181,   179,   181,   181,   179,   181,   179,   179,   179,
     181,   221,   180,   240,   179,   179,    86,    86,   182,   240,
     206,   179,   182,   179,   179,   192,   219,   181,    29,   193,
     219,   219,   219,    15,   206,    28,    32,    48,   186,   186,
     186,   221,   221,   221,   221,   221,   221,   221,   221,    15,
     221,    24,   165,    11,    11,   219,   186,   219,   179,   199,
     221,   180,   185,   207,   180,   207,    15,    24,    75,   165,
     179,    15,    24,    75,   165,   179,    11,    14,    16,    17,
     179,   179,    65,   237,   206,   206,     8,     8,     8,   165,
     165,   186,   186,   186,   186,   186,   186,   221,   221,   179,
     221,   181,   221,   221,   240,   179,   240,   219,   219,   221,
     221,   221,   186,   186,   186,   186,   186,   186,   186,   186,
      15,   221,   221,   181,   181,   218,    79,   190,   181,   218,
      15,    15,   179,   221,    15,    15,   179,   221,    15,    24,
      75,   165,   179,    15,    24,    75,   165,   179,   179,   179,
     180,   180,   207,   221,   221,   186,   186,   181,   179,   181,
     179,    29,   193,   186,   186,   186,   179,   219,   207,   181,
     186,    14,    26,    27,    36,   186,   221,   207,   181,   179,
     179,   179,   165,   179,   165,   179,   179,    15,    15,   179,
     221,    15,    15,   179,   221,   181,   218,   181,   218,   186,
     186,   179,   219,   199,   207,   165,   221,   221,   186,   221,
      28,    31,    32,    41,    44,    45,    48,    49,   186,   207,
     221,   221,   179,   179,   179,   179,   179,   179,   207,   181,
     207,   181,   181,   221,    28,    32,    48,   186,   186,   186,
     221,   221,   221,   221,   221,   221,   221,   221,   179,   179,
     207,   207,   219,   186,   221,   221,   221,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186
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
     229,   229,   229,   229,   229,   229,   229,   229,   230,   230,
     230,   230,   231,   231,   231,   232,   232,   232,   232,   232,
     232,   232,   232,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   234,   234,   234,   234,
     234,   234,   235,   235,   236,   236,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   238,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   240,   241,
     241,   241,   241,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242
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
       3,     3,     3,     3,     3,     3,     3,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     2,     2,     2,
       2,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     3,     4,     6,     4,     3,     6,     6,     5,
       3,     3,     6,     5,     4,     4,     6,     6,     5,     3,
       6,     5,     3,     5,     5,     5,     4,     2,     2,     3,
       4,     3,     2,     3,     2,     1,     1,     3,     2,     2,
       3,     4,     5,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     1,     1,     1,     1,
       1,     4,     6,     6,     6,     1,     4,     4,     5,     5,
       5,     5,     5,     5,     1,     1,     1,     1,     2,     3,
       4,     5,     2,     3,     4,     5,     1,     1,     2,     5,
       3
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
#line 740 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 3995 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 750 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 4001 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 751 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 4007 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 755 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 4013 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 757 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 4019 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 759 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4025 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 761 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4031 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 763 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4037 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 765 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4044 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 768 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4050 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 770 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4056 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 772 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4062 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 774 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4068 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 776 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4076 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 780 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4082 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 782 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4088 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 784 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4094 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 786 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4100 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 788 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4108 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 792 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4116 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 796 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4122 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 798 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4130 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 802 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4138 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 806 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4144 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 808 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4150 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 810 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4156 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 812 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4162 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 814 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4168 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 816 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4176 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 820 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4184 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 824 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4190 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4196 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4202 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 830 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4208 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 832 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4214 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 834 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4220 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 836 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4226 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 838 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4232 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 840 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4238 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 842 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4244 "raku.tab.c"
    break;

  case 42: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 844 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4256 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 852 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4262 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 854 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4268 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 856 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4274 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 858 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4280 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 860 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4286 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 862 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4292 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 864 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4299 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 867 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4306 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 870 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4312 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 872 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4319 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 875 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4326 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 878 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4332 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 880 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4340 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 884 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4346 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 886 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4352 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 888 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4358 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 890 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4364 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 892 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4371 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 895 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4378 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 898 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4384 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 900 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4391 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 903 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4398 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 906 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4405 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 909 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4411 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 911 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4420 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 916 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4428 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 920 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4436 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 924 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4443 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 927 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4450 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 930 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4458 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 934 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4465 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 937 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4472 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 940 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4479 "raku.tab.c"
    break;

  case 76: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 943 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4486 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 946 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4493 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_IF expr ';'  */
#line 949 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4499 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNLESS expr ';'  */
#line 951 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4505 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WHILE expr ';'  */
#line 953 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4511 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_UNTIL expr ';'  */
#line 955 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4517 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_FOR expr ';'  */
#line 957 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4524 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WITH expr ';'  */
#line 960 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4530 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITHOUT expr ';'  */
#line 962 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4536 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_GIVEN expr ';'  */
#line 964 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4542 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 966 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4549 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 969 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4556 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 972 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4564 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 976 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4571 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 979 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4577 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 981 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4583 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 983 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4589 "raku.tab.c"
    break;

  case 93: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 985 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4596 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 988 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4603 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 991 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4610 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 994 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4617 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 997 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4624 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 1000 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4631 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1003 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4638 "raku.tab.c"
    break;

  case 100: /* stmt: expr ';'  */
#line 1005 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4644 "raku.tab.c"
    break;

  case 101: /* stmt: ';'  */
#line 1006 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4650 "raku.tab.c"
    break;

  case 102: /* stmt: if_stmt  */
#line 1007 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4656 "raku.tab.c"
    break;

  case 103: /* stmt: while_stmt  */
#line 1008 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4662 "raku.tab.c"
    break;

  case 104: /* stmt: for_stmt  */
#line 1009 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4668 "raku.tab.c"
    break;

  case 105: /* stmt: given_stmt  */
#line 1010 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4674 "raku.tab.c"
    break;

  case 106: /* stmt: KW_TRY block  */
#line 1012 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4680 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block KW_CATCH block  */
#line 1014 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4686 "raku.tab.c"
    break;

  case 108: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1016 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4694 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1020 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4703 "raku.tab.c"
    break;

  case 110: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1025 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4711 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH block  */
#line 1029 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4717 "raku.tab.c"
    break;

  case 112: /* stmt: block  */
#line 1031 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4723 "raku.tab.c"
    break;

  case 113: /* stmt: PHASER block  */
#line 1033 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 4729 "raku.tab.c"
    break;

  case 114: /* stmt: PHASER block ';'  */
#line 1035 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 4735 "raku.tab.c"
    break;

  case 115: /* stmt: unless_stmt  */
#line 1036 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4741 "raku.tab.c"
    break;

  case 116: /* stmt: until_stmt  */
#line 1037 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4747 "raku.tab.c"
    break;

  case 117: /* stmt: repeat_stmt  */
#line 1038 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4753 "raku.tab.c"
    break;

  case 118: /* stmt: loop_stmt  */
#line 1039 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4759 "raku.tab.c"
    break;

  case 119: /* stmt: KW_LAST ';'  */
#line 1040 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4765 "raku.tab.c"
    break;

  case 120: /* stmt: KW_NEXT ';'  */
#line 1041 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4771 "raku.tab.c"
    break;

  case 121: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1043 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4777 "raku.tab.c"
    break;

  case 122: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1045 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4783 "raku.tab.c"
    break;

  case 123: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1047 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4789 "raku.tab.c"
    break;

  case 124: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1049 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4795 "raku.tab.c"
    break;

  case 125: /* stmt: sub_decl  */
#line 1050 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4801 "raku.tab.c"
    break;

  case 126: /* stmt: class_decl  */
#line 1051 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4807 "raku.tab.c"
    break;

  case 127: /* stmt: role_decl  */
#line 1052 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4813 "raku.tab.c"
    break;

  case 128: /* stmt: grammar_decl  */
#line 1053 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4819 "raku.tab.c"
    break;

  case 129: /* stmt: module_decl  */
#line 1054 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4825 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1058 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4831 "raku.tab.c"
    break;

  case 131: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1060 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4837 "raku.tab.c"
    break;

  case 132: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1062 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4843 "raku.tab.c"
    break;

  case 133: /* if_stmt: KW_IF expr block  */
#line 1064 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4849 "raku.tab.c"
    break;

  case 134: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1066 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4855 "raku.tab.c"
    break;

  case 135: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1068 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4861 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1070 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4867 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1072 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4873 "raku.tab.c"
    break;

  case 138: /* elsif_tail: KW_ELSIF expr block  */
#line 1076 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4879 "raku.tab.c"
    break;

  case 139: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1078 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4885 "raku.tab.c"
    break;

  case 140: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4891 "raku.tab.c"
    break;

  case 141: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4897 "raku.tab.c"
    break;

  case 142: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1084 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4903 "raku.tab.c"
    break;

  case 143: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1086 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4909 "raku.tab.c"
    break;

  case 144: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1090 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4915 "raku.tab.c"
    break;

  case 145: /* while_stmt: KW_WHILE expr block  */
#line 1092 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4921 "raku.tab.c"
    break;

  case 146: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1096 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4927 "raku.tab.c"
    break;

  case 147: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1098 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4933 "raku.tab.c"
    break;

  case 148: /* unless_stmt: KW_UNLESS expr block  */
#line 1100 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4939 "raku.tab.c"
    break;

  case 149: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1102 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4945 "raku.tab.c"
    break;

  case 150: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1106 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4951 "raku.tab.c"
    break;

  case 151: /* until_stmt: KW_UNTIL expr block  */
#line 1108 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4957 "raku.tab.c"
    break;

  case 152: /* repeat_stmt: KW_REPEAT block  */
#line 1112 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4963 "raku.tab.c"
    break;

  case 153: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1114 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4969 "raku.tab.c"
    break;

  case 154: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1116 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4975 "raku.tab.c"
    break;

  case 155: /* loop_stmt: KW_LOOP block  */
#line 1120 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4981 "raku.tab.c"
    break;

  case 156: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1122 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4987 "raku.tab.c"
    break;

  case 157: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1124 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4993 "raku.tab.c"
    break;

  case 158: /* loop_incr: expr  */
#line 1127 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4999 "raku.tab.c"
    break;

  case 159: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1131 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5009 "raku.tab.c"
    break;

  case 160: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1137 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5019 "raku.tab.c"
    break;

  case 161: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1143 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5031 "raku.tab.c"
    break;

  case 162: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1151 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5041 "raku.tab.c"
    break;

  case 163: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1157 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5050 "raku.tab.c"
    break;

  case 164: /* for_stmt: KW_FOR expr block  */
#line 1162 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5057 "raku.tab.c"
    break;

  case 165: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1167 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 5069 "raku.tab.c"
    break;

  case 166: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1175 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 5082 "raku.tab.c"
    break;

  case 167: /* catch_when_list: KW_WHEN expr block  */
#line 1186 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5088 "raku.tab.c"
    break;

  case 168: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1188 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5094 "raku.tab.c"
    break;

  case 169: /* when_list: %empty  */
#line 1191 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5100 "raku.tab.c"
    break;

  case 170: /* when_list: when_list KW_WHEN expr block  */
#line 1193 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5108 "raku.tab.c"
    break;

  case 171: /* sub_trait_list: TESTOP IDENT  */
#line 1198 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5114 "raku.tab.c"
    break;

  case 172: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1199 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5120 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1203 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5132 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1211 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5142 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1217 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5152 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1223 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5164 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1231 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5174 "raku.tab.c"
    break;

  case 178: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1237 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5186 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1245 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5196 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1251 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5206 "raku.tab.c"
    break;

  case 181: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1257 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5219 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1266 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5230 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1273 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5243 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1282 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5256 "raku.tab.c"
    break;

  case 185: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1291 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5267 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list '}'  */
#line 1299 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5273 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr '}'  */
#line 1301 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5280 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1304 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5287 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1307 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5293 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1309 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5299 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1311 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5305 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1313 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5311 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1315 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5317 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1317 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5323 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1319 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5329 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1321 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5336 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1324 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1326 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5348 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1328 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5354 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1330 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5361 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1333 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5368 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1336 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5376 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list '}'  */
#line 1341 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5382 "raku.tab.c"
    break;

  case 204: /* method_body: '{' YADA '}'  */
#line 1342 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5388 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1344 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5395 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr '}'  */
#line 1347 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5402 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1350 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5409 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1353 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5415 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1355 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5421 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1357 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5427 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1359 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5433 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1361 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5439 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1363 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5445 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1365 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5451 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1367 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5458 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1370 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5464 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1372 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5470 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1374 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5476 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1376 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5483 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1379 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5490 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1382 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5498 "raku.tab.c"
    break;

  case 222: /* pkg_name: IDENT  */
#line 1387 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5504 "raku.tab.c"
    break;

  case 223: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1391 "raku.y"
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
#line 5522 "raku.tab.c"
    break;

  case 224: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1407 "raku.y"
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
#line 5539 "raku.tab.c"
    break;

  case 225: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1422 "raku.y"
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
#line 5556 "raku.tab.c"
    break;

  case 226: /* is_clauses: %empty  */
#line 1436 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5562 "raku.tab.c"
    break;

  case 227: /* is_clauses: is_clauses IDENT IDENT  */
#line 1438 "raku.y"
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
#line 5578 "raku.tab.c"
    break;

  case 228: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1450 "raku.y"
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
#line 5594 "raku.tab.c"
    break;

  case 229: /* class_body_list: %empty  */
#line 1463 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5600 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1465 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5607 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1468 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5614 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1471 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5621 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1474 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5628 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1477 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5635 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1480 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5642 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1483 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5649 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1486 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5656 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1489 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5663 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1492 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5670 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1495 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5677 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1498 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5684 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1501 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5695 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1508 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5705 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1514 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5715 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1520 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5726 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1527 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5737 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1534 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5748 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1541 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5756 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1545 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5764 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1549 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5772 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1553 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5780 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1557 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5791 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1564 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5802 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1571 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5813 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1578 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5824 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1585 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5838 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1595 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5850 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1603 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5862 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1611 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5875 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1620 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5886 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1627 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5897 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1634 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5911 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1644 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5923 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1652 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5935 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1660 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5948 "raku.tab.c"
    break;

  case 266: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1669 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5959 "raku.tab.c"
    break;

  case 267: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1678 "raku.y"
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
#line 5976 "raku.tab.c"
    break;

  case 268: /* grammar_body_list: %empty  */
#line 1692 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5982 "raku.tab.c"
    break;

  case 269: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1694 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5991 "raku.tab.c"
    break;

  case 270: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1699 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6000 "raku.tab.c"
    break;

  case 271: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1704 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6009 "raku.tab.c"
    break;

  case 272: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1711 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 6017 "raku.tab.c"
    break;

  case 273: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1715 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 6025 "raku.tab.c"
    break;

  case 274: /* named_arg_list: ':' IDENT  */
#line 1719 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6036 "raku.tab.c"
    break;

  case 275: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1726 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6044 "raku.tab.c"
    break;

  case 276: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1730 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6052 "raku.tab.c"
    break;

  case 277: /* pair_list: IDENT OP_FATARROW expr  */
#line 1736 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6058 "raku.tab.c"
    break;

  case 278: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1738 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6064 "raku.tab.c"
    break;

  case 279: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1740 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6070 "raku.tab.c"
    break;

  case 280: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1742 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6076 "raku.tab.c"
    break;

  case 281: /* param_list: VAR_SCALAR  */
#line 1745 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6082 "raku.tab.c"
    break;

  case 282: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1746 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6088 "raku.tab.c"
    break;

  case 283: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1747 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6094 "raku.tab.c"
    break;

  case 284: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1748 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6100 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1749 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6106 "raku.tab.c"
    break;

  case 286: /* param_list: VAR_ARRAY  */
#line 1750 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6112 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' VAR_ARRAY  */
#line 1751 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6118 "raku.tab.c"
    break;

  case 288: /* param_list: IDENT VAR_SCALAR  */
#line 1752 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6124 "raku.tab.c"
    break;

  case 289: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1753 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6130 "raku.tab.c"
    break;

  case 290: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1754 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6136 "raku.tab.c"
    break;

  case 291: /* param_list: param_list ',' VAR_SCALAR  */
#line 1755 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6142 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1756 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6148 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1757 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6154 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1758 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6160 "raku.tab.c"
    break;

  case 295: /* param_list: VAR_SCALAR '=' expr  */
#line 1759 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6166 "raku.tab.c"
    break;

  case 296: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1760 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6172 "raku.tab.c"
    break;

  case 297: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1761 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6178 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1762 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6184 "raku.tab.c"
    break;

  case 299: /* param_list: SLURPY_POS  */
#line 1763 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6190 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' SLURPY_POS  */
#line 1764 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6196 "raku.tab.c"
    break;

  case 301: /* param_list: SLURPY_LOL  */
#line 1765 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6202 "raku.tab.c"
    break;

  case 302: /* param_list: param_list ',' SLURPY_LOL  */
#line 1766 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6208 "raku.tab.c"
    break;

  case 303: /* param_list: SLURPY_NAMED  */
#line 1767 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6214 "raku.tab.c"
    break;

  case 304: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1768 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6220 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list '}'  */
#line 1771 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6226 "raku.tab.c"
    break;

  case 306: /* block: '{' YADA '}'  */
#line 1772 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6232 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list expr '}'  */
#line 1774 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6238 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1776 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6244 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1778 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6250 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1780 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6256 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1782 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6262 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1784 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6269 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1787 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6275 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1789 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6281 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1791 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6287 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1793 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6293 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1795 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6300 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1798 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6307 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1801 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6315 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1805 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6322 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1808 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6329 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1811 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6336 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1814 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6342 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1816 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6349 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1819 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6356 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1822 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6363 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1825 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6370 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1828 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6377 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1831 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6383 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1833 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6389 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1835 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6395 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_LAST '}'  */
#line 1837 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6401 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1839 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6407 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1841 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6413 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1843 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6419 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1845 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6425 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1847 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6431 "raku.tab.c"
    break;

  case 338: /* closure: '{' expr '}'  */
#line 1850 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6437 "raku.tab.c"
    break;

  case 339: /* expr: VAR_SCALAR '=' expr  */
#line 1853 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6443 "raku.tab.c"
    break;

  case 340: /* expr: VAR_ARRAY '=' expr  */
#line 1854 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6449 "raku.tab.c"
    break;

  case 341: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1855 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6455 "raku.tab.c"
    break;

  case 342: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1856 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6461 "raku.tab.c"
    break;

  case 343: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1857 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6467 "raku.tab.c"
    break;

  case 344: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1858 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6473 "raku.tab.c"
    break;

  case 345: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1859 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6479 "raku.tab.c"
    break;

  case 346: /* expr: KW_GATHER block  */
#line 1860 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6489 "raku.tab.c"
    break;

  case 347: /* expr: KW_GATHER for_stmt  */
#line 1865 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6499 "raku.tab.c"
    break;

  case 348: /* expr: tern_expr OP_FATARROW expr  */
#line 1871 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6505 "raku.tab.c"
    break;

  case 349: /* expr: tern_expr  */
#line 1872 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6511 "raku.tab.c"
    break;

  case 350: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1876 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6517 "raku.tab.c"
    break;

  case 351: /* tern_expr: or_expr  */
#line 1877 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6523 "raku.tab.c"
    break;

  case 352: /* or_expr: or_expr OP_OR and_expr  */
#line 1880 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6529 "raku.tab.c"
    break;

  case 353: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1881 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6535 "raku.tab.c"
    break;

  case 354: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1882 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6541 "raku.tab.c"
    break;

  case 355: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1883 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6547 "raku.tab.c"
    break;

  case 356: /* or_expr: and_expr  */
#line 1884 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6553 "raku.tab.c"
    break;

  case 357: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1887 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6559 "raku.tab.c"
    break;

  case 358: /* and_expr: cmp_expr  */
#line 1888 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6565 "raku.tab.c"
    break;

  case 359: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1891 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6571 "raku.tab.c"
    break;

  case 360: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1892 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6577 "raku.tab.c"
    break;

  case 361: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1893 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6583 "raku.tab.c"
    break;

  case 362: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1894 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6589 "raku.tab.c"
    break;

  case 363: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1895 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6595 "raku.tab.c"
    break;

  case 364: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1896 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6601 "raku.tab.c"
    break;

  case 365: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1897 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6607 "raku.tab.c"
    break;

  case 366: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1898 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6613 "raku.tab.c"
    break;

  case 367: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1899 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6619 "raku.tab.c"
    break;

  case 368: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1900 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6625 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1901 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6631 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1902 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6637 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1903 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6643 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1904 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6649 "raku.tab.c"
    break;

  case 373: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1905 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6655 "raku.tab.c"
    break;

  case 374: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1906 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6661 "raku.tab.c"
    break;

  case 375: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1907 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6667 "raku.tab.c"
    break;

  case 376: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1908 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6673 "raku.tab.c"
    break;

  case 377: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1909 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6679 "raku.tab.c"
    break;

  case 378: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1910 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6685 "raku.tab.c"
    break;

  case 379: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1911 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6691 "raku.tab.c"
    break;

  case 380: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1912 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6697 "raku.tab.c"
    break;

  case 381: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1913 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6703 "raku.tab.c"
    break;

  case 382: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1915 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6713 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1921 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6723 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1927 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6733 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1933 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6743 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr  */
#line 1938 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6749 "raku.tab.c"
    break;

  case 387: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1941 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6755 "raku.tab.c"
    break;

  case 388: /* divis_expr: jct_expr  */
#line 1942 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6761 "raku.tab.c"
    break;

  case 389: /* jct_expr: jct_expr '|' range_expr  */
#line 1945 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6767 "raku.tab.c"
    break;

  case 390: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1946 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6773 "raku.tab.c"
    break;

  case 391: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1947 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6779 "raku.tab.c"
    break;

  case 392: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1948 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6785 "raku.tab.c"
    break;

  case 393: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1949 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6791 "raku.tab.c"
    break;

  case 394: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1950 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6797 "raku.tab.c"
    break;

  case 395: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1951 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6803 "raku.tab.c"
    break;

  case 396: /* jct_expr: jct_expr '&' range_expr  */
#line 1952 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6809 "raku.tab.c"
    break;

  case 397: /* jct_expr: dor_expr  */
#line 1953 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6815 "raku.tab.c"
    break;

  case 398: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1957 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6821 "raku.tab.c"
    break;

  case 399: /* dor_expr: range_expr  */
#line 1958 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6827 "raku.tab.c"
    break;

  case 400: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1961 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6833 "raku.tab.c"
    break;

  case 401: /* range_expr: add_expr YADA add_expr  */
#line 1962 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6839 "raku.tab.c"
    break;

  case 402: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1963 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6845 "raku.tab.c"
    break;

  case 403: /* range_expr: range_expr OP_COLL add_expr  */
#line 1964 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6851 "raku.tab.c"
    break;

  case 404: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1965 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6857 "raku.tab.c"
    break;

  case 405: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1966 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6863 "raku.tab.c"
    break;

  case 406: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1967 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6869 "raku.tab.c"
    break;

  case 407: /* range_expr: add_expr  */
#line 1968 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6875 "raku.tab.c"
    break;

  case 408: /* add_expr: add_expr '~' repl_expr  */
#line 1971 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6881 "raku.tab.c"
    break;

  case 409: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 1972 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6887 "raku.tab.c"
    break;

  case 410: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 1973 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6893 "raku.tab.c"
    break;

  case 411: /* add_expr: repl_expr  */
#line 1974 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6899 "raku.tab.c"
    break;

  case 412: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1977 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6905 "raku.tab.c"
    break;

  case 413: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1978 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6911 "raku.tab.c"
    break;

  case 414: /* repl_expr: addsub_expr  */
#line 1979 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6917 "raku.tab.c"
    break;

  case 415: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1982 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6923 "raku.tab.c"
    break;

  case 416: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 1983 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6929 "raku.tab.c"
    break;

  case 417: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 1984 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6935 "raku.tab.c"
    break;

  case 418: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 1985 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6941 "raku.tab.c"
    break;

  case 419: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 1986 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6947 "raku.tab.c"
    break;

  case 420: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 1987 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6953 "raku.tab.c"
    break;

  case 421: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1988 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6959 "raku.tab.c"
    break;

  case 422: /* addsub_expr: mul_expr  */
#line 1989 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6965 "raku.tab.c"
    break;

  case 423: /* mul_expr: mul_expr '*' unary_expr  */
#line 1992 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6971 "raku.tab.c"
    break;

  case 424: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 1993 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6977 "raku.tab.c"
    break;

  case 425: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 1994 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6983 "raku.tab.c"
    break;

  case 426: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 1995 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6989 "raku.tab.c"
    break;

  case 427: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 1996 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6995 "raku.tab.c"
    break;

  case 428: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 1997 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7001 "raku.tab.c"
    break;

  case 429: /* mul_expr: mul_expr '/' unary_expr  */
#line 1998 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7007 "raku.tab.c"
    break;

  case 430: /* mul_expr: mul_expr '%' unary_expr  */
#line 1999 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7013 "raku.tab.c"
    break;

  case 431: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 2001 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7019 "raku.tab.c"
    break;

  case 432: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2003 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7025 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2005 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7031 "raku.tab.c"
    break;

  case 434: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2007 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7037 "raku.tab.c"
    break;

  case 435: /* mul_expr: unary_expr  */
#line 2008 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7043 "raku.tab.c"
    break;

  case 436: /* unary_expr: '-' unary_expr  */
#line 2011 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 7049 "raku.tab.c"
    break;

  case 437: /* unary_expr: '+' unary_expr  */
#line 2012 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 7055 "raku.tab.c"
    break;

  case 438: /* unary_expr: '!' unary_expr  */
#line 2013 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7061 "raku.tab.c"
    break;

  case 439: /* unary_expr: CARET unary_expr  */
#line 2014 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7067 "raku.tab.c"
    break;

  case 440: /* unary_expr: OP_REDUCE unary_expr  */
#line 2016 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7076 "raku.tab.c"
    break;

  case 441: /* unary_expr: pow_expr  */
#line 2020 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7082 "raku.tab.c"
    break;

  case 442: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2023 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7088 "raku.tab.c"
    break;

  case 443: /* pow_expr: postfix_expr  */
#line 2024 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7094 "raku.tab.c"
    break;

  case 444: /* scalar_list: VAR_SCALAR  */
#line 2027 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7100 "raku.tab.c"
    break;

  case 445: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2028 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7106 "raku.tab.c"
    break;

  case 446: /* meth_name: IDENT  */
#line 2031 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7112 "raku.tab.c"
    break;

  case 447: /* meth_name: KW_SORT  */
#line 2032 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7118 "raku.tab.c"
    break;

  case 448: /* meth_name: KW_REVERSE  */
#line 2033 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7124 "raku.tab.c"
    break;

  case 449: /* meth_name: KW_MAP  */
#line 2034 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7130 "raku.tab.c"
    break;

  case 450: /* meth_name: KW_GREP  */
#line 2035 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7136 "raku.tab.c"
    break;

  case 451: /* meth_name: KW_SAY  */
#line 2036 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7142 "raku.tab.c"
    break;

  case 452: /* meth_name: KW_PRINT  */
#line 2037 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7148 "raku.tab.c"
    break;

  case 453: /* meth_name: KW_TAKE  */
#line 2038 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7154 "raku.tab.c"
    break;

  case 454: /* meth_name: KW_RETURN  */
#line 2039 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7160 "raku.tab.c"
    break;

  case 455: /* meth_name: KW_EXISTS  */
#line 2040 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7166 "raku.tab.c"
    break;

  case 456: /* meth_name: KW_DELETE  */
#line 2041 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7172 "raku.tab.c"
    break;

  case 457: /* meth_name: KW_JOIN  */
#line 2042 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7178 "raku.tab.c"
    break;

  case 458: /* meth_name: TESTOP  */
#line 2043 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7184 "raku.tab.c"
    break;

  case 459: /* postfix_expr: call_expr  */
#line 2045 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7190 "raku.tab.c"
    break;

  case 460: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2048 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7200 "raku.tab.c"
    break;

  case 461: /* call_expr: IDENT '(' arg_list ')'  */
#line 2054 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7209 "raku.tab.c"
    break;

  case 462: /* call_expr: IDENT '(' ')'  */
#line 2058 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7215 "raku.tab.c"
    break;

  case 463: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2060 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7221 "raku.tab.c"
    break;

  case 464: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2062 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7227 "raku.tab.c"
    break;

  case 465: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2064 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7236 "raku.tab.c"
    break;

  case 466: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2069 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7242 "raku.tab.c"
    break;

  case 467: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2071 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7255 "raku.tab.c"
    break;

  case 468: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2080 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7268 "raku.tab.c"
    break;

  case 469: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2089 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7279 "raku.tab.c"
    break;

  case 470: /* call_expr: IDENT '.' KW_NEW  */
#line 2096 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7287 "raku.tab.c"
    break;

  case 471: /* call_expr: IDENT '.' IDENT  */
#line 2100 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7296 "raku.tab.c"
    break;

  case 472: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2105 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7307 "raku.tab.c"
    break;

  case 473: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2112 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7316 "raku.tab.c"
    break;

  case 474: /* call_expr: IDENT '.' CARET IDENT  */
#line 2117 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7326 "raku.tab.c"
    break;

  case 475: /* call_expr: atom '.' CARET IDENT  */
#line 2123 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7336 "raku.tab.c"
    break;

  case 476: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2129 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7347 "raku.tab.c"
    break;

  case 477: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2136 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7358 "raku.tab.c"
    break;

  case 478: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2143 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7367 "raku.tab.c"
    break;

  case 479: /* call_expr: atom '.' meth_name  */
#line 2148 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7376 "raku.tab.c"
    break;

  case 480: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2153 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7387 "raku.tab.c"
    break;

  case 481: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2160 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7396 "raku.tab.c"
    break;

  case 482: /* call_expr: call_expr '.' meth_name  */
#line 2165 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7405 "raku.tab.c"
    break;

  case 483: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2170 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7416 "raku.tab.c"
    break;

  case 484: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2177 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7427 "raku.tab.c"
    break;

  case 485: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2184 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7438 "raku.tab.c"
    break;

  case 486: /* call_expr: '.' meth_name '(' ')'  */
#line 2191 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7447 "raku.tab.c"
    break;

  case 487: /* call_expr: '.' meth_name  */
#line 2196 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7456 "raku.tab.c"
    break;

  case 488: /* call_expr: KW_DIE expr  */
#line 2201 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7462 "raku.tab.c"
    break;

  case 489: /* call_expr: KW_MAP closure expr  */
#line 2203 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7468 "raku.tab.c"
    break;

  case 490: /* call_expr: KW_MAP closure ',' expr  */
#line 2205 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7474 "raku.tab.c"
    break;

  case 491: /* call_expr: KW_GREP closure expr  */
#line 2207 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7480 "raku.tab.c"
    break;

  case 492: /* call_expr: KW_SORT expr  */
#line 2209 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7486 "raku.tab.c"
    break;

  case 493: /* call_expr: KW_SORT closure expr  */
#line 2211 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7492 "raku.tab.c"
    break;

  case 494: /* call_expr: KW_REVERSE expr  */
#line 2213 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7498 "raku.tab.c"
    break;

  case 495: /* call_expr: atom  */
#line 2214 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7504 "raku.tab.c"
    break;

  case 496: /* arg_list: expr  */
#line 2217 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7510 "raku.tab.c"
    break;

  case 497: /* arg_list: arg_list ',' expr  */
#line 2218 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7516 "raku.tab.c"
    break;

  case 498: /* arg_list: arg_list ','  */
#line 2219 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7522 "raku.tab.c"
    break;

  case 499: /* paren_group: '(' ')'  */
#line 2222 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7528 "raku.tab.c"
    break;

  case 500: /* paren_group: '(' expr ')'  */
#line 2223 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7534 "raku.tab.c"
    break;

  case 501: /* paren_group: '(' expr ',' ')'  */
#line 2225 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7540 "raku.tab.c"
    break;

  case 502: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2227 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7547 "raku.tab.c"
    break;

  case 503: /* atom: LIT_INT  */
#line 2231 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7553 "raku.tab.c"
    break;

  case 504: /* atom: LIT_BOOL  */
#line 2233 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7561 "raku.tab.c"
    break;

  case 505: /* atom: LIT_FLOAT  */
#line 2236 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7567 "raku.tab.c"
    break;

  case 506: /* atom: LIT_STR  */
#line 2237 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7573 "raku.tab.c"
    break;

  case 507: /* atom: WORDLIST  */
#line 2239 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7586 "raku.tab.c"
    break;

  case 508: /* atom: LIT_INTERP_STR  */
#line 2247 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7592 "raku.tab.c"
    break;

  case 509: /* atom: VAR_SCALAR  */
#line 2248 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7598 "raku.tab.c"
    break;

  case 510: /* atom: OP_INC VAR_SCALAR  */
#line 2249 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7604 "raku.tab.c"
    break;

  case 511: /* atom: OP_DEC VAR_SCALAR  */
#line 2250 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7610 "raku.tab.c"
    break;

  case 512: /* atom: VAR_SCALAR OP_INC  */
#line 2251 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7616 "raku.tab.c"
    break;

  case 513: /* atom: VAR_SCALAR OP_DEC  */
#line 2252 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7622 "raku.tab.c"
    break;

  case 514: /* atom: VAR_TWIGIL OP_INC  */
#line 2253 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 7628 "raku.tab.c"
    break;

  case 515: /* atom: VAR_TWIGIL OP_DEC  */
#line 2254 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 7634 "raku.tab.c"
    break;

  case 516: /* atom: VAR_ARRAY  */
#line 2255 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7640 "raku.tab.c"
    break;

  case 517: /* atom: VAR_HASH  */
#line 2256 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7646 "raku.tab.c"
    break;

  case 518: /* atom: VAR_CAPTURE  */
#line 2258 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7654 "raku.tab.c"
    break;

  case 519: /* atom: VAR_FH  */
#line 2262 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7662 "raku.tab.c"
    break;

  case 520: /* atom: VAR_NAMED_CAPTURE  */
#line 2266 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7669 "raku.tab.c"
    break;

  case 521: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2269 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7675 "raku.tab.c"
    break;

  case 522: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2271 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7681 "raku.tab.c"
    break;

  case 523: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2273 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7687 "raku.tab.c"
    break;

  case 524: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2275 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7693 "raku.tab.c"
    break;

  case 525: /* atom: ARR_ALL_SLICE  */
#line 2277 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7699 "raku.tab.c"
    break;

  case 526: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2279 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7705 "raku.tab.c"
    break;

  case 527: /* atom: VAR_HASH '{' expr '}'  */
#line 2281 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7711 "raku.tab.c"
    break;

  case 528: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2283 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7717 "raku.tab.c"
    break;

  case 529: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2285 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7723 "raku.tab.c"
    break;

  case 530: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2287 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7729 "raku.tab.c"
    break;

  case 531: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2289 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7735 "raku.tab.c"
    break;

  case 532: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2291 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7741 "raku.tab.c"
    break;

  case 533: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2293 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7747 "raku.tab.c"
    break;

  case 534: /* atom: IDENT  */
#line 2294 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7753 "raku.tab.c"
    break;

  case 535: /* atom: VAR_TWIGIL  */
#line 2296 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7761 "raku.tab.c"
    break;

  case 536: /* atom: VAR_ARRAY_TWIGIL  */
#line 2300 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7769 "raku.tab.c"
    break;

  case 537: /* atom: VAR_HASH_TWIGIL  */
#line 2304 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7777 "raku.tab.c"
    break;

  case 538: /* atom: '[' ']'  */
#line 2307 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7783 "raku.tab.c"
    break;

  case 539: /* atom: '[' expr ']'  */
#line 2309 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7789 "raku.tab.c"
    break;

  case 540: /* atom: '[' expr ',' ']'  */
#line 2311 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7795 "raku.tab.c"
    break;

  case 541: /* atom: '[' expr ',' arg_list ']'  */
#line 2313 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7802 "raku.tab.c"
    break;

  case 542: /* atom: DOLLAR_LBRACKET ']'  */
#line 2315 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7808 "raku.tab.c"
    break;

  case 543: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2317 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7814 "raku.tab.c"
    break;

  case 544: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2319 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7820 "raku.tab.c"
    break;

  case 545: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2321 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7827 "raku.tab.c"
    break;

  case 546: /* atom: paren_group  */
#line 2323 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 7833 "raku.tab.c"
    break;

  case 547: /* atom: block  */
#line 2324 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7839 "raku.tab.c"
    break;

  case 548: /* atom: KW_SUB block  */
#line 2325 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7845 "raku.tab.c"
    break;

  case 549: /* atom: KW_SUB '(' param_list ')' block  */
#line 2327 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 7852 "raku.tab.c"
    break;

  case 550: /* atom: OP_ARROW scalar_list block  */
#line 2330 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 7859 "raku.tab.c"
    break;


#line 7863 "raku.tab.c"

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

#line 2333 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
