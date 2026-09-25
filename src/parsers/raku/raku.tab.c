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
#define YYLTYPE         RAKU_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         raku_yyparse
#define yylex           raku_yylex
#define yyerror         raku_yyerror
#define yydebug         raku_yydebug
#define yynerrs         raku_yynerrs
#define yylval          raku_yylval
#define yychar          raku_yychar
#define yylloc          raku_yylloc

/* First part of user prologue.  */
#line 12 "raku.y"

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
static ExprList *exprlist_append_at(ExprList *l, int line, tree_t *n) { if (n && n->line == 0) n->line = line; return exprlist_append(l, n); }
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
static tree_t *rk_adverb_bool(int v) {
    tree_t *b = make_call("__rk_mkbool");
    tree_t *iv = ast_node_new(TT_ILIT); iv->v.ival = v;
    expr_add_child(b, iv);
    return b;
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
    tree_t *c = ast_node_new(e->t); c->v = e->v; c->line = e->line;
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
        char sig = s[i];
        i++;
        char nm[258]; int nl=0; nm[nl++]=sig;
        while (i<len&&(s[i]=='_'||(s[i]>='A'&&s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]>='0'&&s[i]<='9')))
            { if(nl<257) nm[nl++]=s[i]; i++; }
        nm[nl]='\0'; *ip=i;
        return var_node(nm);
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
        return expr_binary(k,rk_numeric_ctx(left),rk_numeric_ctx(right));
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

#line 724 "raku.tab.c"

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
         || (defined RAKU_YYLTYPE_IS_TRIVIAL && RAKU_YYLTYPE_IS_TRIVIAL \
             && defined RAKU_YYSTYPE_IS_TRIVIAL && RAKU_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYLAST   7690

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  566
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1398

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
       0,   748,   748,   759,   760,   763,   765,   767,   769,   771,
     773,   776,   778,   780,   782,   784,   788,   790,   792,   794,
     796,   800,   804,   806,   810,   814,   816,   818,   820,   822,
     824,   828,   832,   834,   836,   838,   840,   842,   844,   846,
     848,   850,   852,   860,   862,   864,   866,   868,   870,   872,
     875,   878,   880,   883,   886,   888,   892,   894,   896,   898,
     900,   903,   906,   908,   911,   914,   917,   919,   924,   928,
     932,   935,   938,   942,   945,   948,   951,   954,   957,   959,
     961,   963,   965,   968,   970,   972,   974,   977,   980,   984,
     987,   989,   991,   993,   996,   999,  1002,  1005,  1008,  1011,
    1014,  1015,  1016,  1017,  1018,  1019,  1020,  1022,  1024,  1028,
    1033,  1037,  1039,  1041,  1043,  1045,  1047,  1049,  1051,  1053,
    1056,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1066,  1068,
    1070,  1072,  1073,  1074,  1075,  1076,  1079,  1081,  1083,  1085,
    1087,  1089,  1091,  1093,  1097,  1099,  1101,  1103,  1105,  1107,
    1111,  1113,  1117,  1119,  1121,  1123,  1127,  1129,  1133,  1135,
    1137,  1141,  1143,  1145,  1149,  1152,  1158,  1164,  1172,  1178,
    1183,  1188,  1196,  1207,  1209,  1213,  1214,  1220,  1221,  1224,
    1232,  1238,  1244,  1252,  1258,  1266,  1272,  1278,  1287,  1294,
    1303,  1312,  1321,  1322,  1325,  1328,  1330,  1332,  1334,  1336,
    1338,  1340,  1342,  1345,  1347,  1349,  1351,  1354,  1357,  1363,
    1364,  1365,  1368,  1371,  1374,  1376,  1378,  1380,  1382,  1384,
    1386,  1388,  1391,  1393,  1395,  1397,  1400,  1403,  1409,  1412,
    1428,  1443,  1458,  1459,  1471,  1485,  1486,  1489,  1492,  1495,
    1498,  1501,  1504,  1507,  1510,  1513,  1516,  1519,  1522,  1529,
    1535,  1541,  1548,  1555,  1562,  1566,  1570,  1574,  1578,  1585,
    1592,  1599,  1606,  1616,  1624,  1632,  1641,  1648,  1655,  1665,
    1673,  1681,  1690,  1699,  1714,  1715,  1720,  1725,  1732,  1736,
    1740,  1747,  1751,  1757,  1759,  1761,  1763,  1767,  1768,  1769,
    1770,  1771,  1772,  1773,  1774,  1775,  1776,  1777,  1778,  1779,
    1780,  1781,  1782,  1783,  1784,  1785,  1786,  1787,  1788,  1789,
    1790,  1793,  1794,  1795,  1797,  1799,  1801,  1803,  1805,  1808,
    1810,  1812,  1814,  1816,  1819,  1822,  1826,  1829,  1832,  1835,
    1837,  1840,  1843,  1846,  1849,  1852,  1854,  1856,  1858,  1860,
    1862,  1864,  1866,  1868,  1872,  1875,  1876,  1877,  1878,  1879,
    1880,  1881,  1882,  1887,  1892,  1894,  1897,  1899,  1902,  1903,
    1904,  1905,  1906,  1909,  1910,  1913,  1914,  1915,  1916,  1917,
    1918,  1919,  1920,  1921,  1922,  1923,  1924,  1925,  1926,  1927,
    1928,  1929,  1930,  1931,  1932,  1933,  1934,  1935,  1936,  1942,
    1948,  1954,  1960,  1963,  1964,  1967,  1968,  1969,  1970,  1971,
    1972,  1973,  1974,  1975,  1978,  1980,  1983,  1984,  1985,  1986,
    1987,  1988,  1989,  1990,  1993,  1994,  1995,  1996,  1999,  2000,
    2001,  2004,  2005,  2006,  2007,  2008,  2009,  2010,  2011,  2014,
    2015,  2016,  2017,  2018,  2019,  2020,  2021,  2022,  2024,  2026,
    2028,  2030,  2033,  2034,  2035,  2036,  2037,  2042,  2045,  2046,
    2049,  2050,  2053,  2054,  2055,  2056,  2057,  2058,  2059,  2060,
    2061,  2062,  2063,  2064,  2065,  2067,  2069,  2075,  2080,  2081,
    2083,  2085,  2090,  2092,  2101,  2110,  2117,  2121,  2126,  2133,
    2138,  2144,  2150,  2157,  2164,  2169,  2174,  2181,  2186,  2191,
    2198,  2205,  2212,  2217,  2222,  2224,  2226,  2228,  2230,  2232,
    2234,  2236,  2239,  2240,  2241,  2242,  2243,  2244,  2245,  2246,
    2247,  2248,  2249,  2250,  2251,  2254,  2255,  2256,  2258,  2263,
    2264,  2268,  2269,  2270,  2279,  2280,  2281,  2282,  2283,  2284,
    2285,  2286,  2287,  2288,  2289,  2293,  2297,  2300,  2302,  2304,
    2306,  2308,  2310,  2312,  2314,  2316,  2318,  2320,  2322,  2324,
    2326,  2327,  2331,  2335,  2339,  2340,  2342,  2344,  2347,  2348,
    2350,  2352,  2355,  2356,  2357,  2358,  2361
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

#define YYPACT_NINF (-873)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-564)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -873,    44,  4326,  -873,  -873,  -873,  -873,  -873,  -873,   758,
     236,   194,   285,    29,  -873,  -873,  7411,  4413,  -873,  -873,
    -873,    63,  1466,    51,  6048,  6127,  6262,  6341,  6476,    30,
      20,  6476,  4548,  4627,   172,    71,  6476,  6476,   155,   164,
    6555,  6690,   -32,   237,     0,   241,    32,    32,  6769,  6476,
     -32,    86,  6476,  4762,   225,   225,    76,  7411,  -873,  -873,
     225,   225,  4841,   316,   341,   357,  7411,  7411,  7411,  1109,
    -873,  4976,  5055,   395,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  1379,   672,
     335,   682,   397,   676,   785,   780,   437,   766,   462,   -61,
     770,   808,  -873,  -873,   402,   420,   444,   439,  6476,   630,
    -873,  -873,  6476,  6476,  6476,  6476,  6476,  6476,  1601,  6476,
    6904,  5190,  6476,   656,  6476,  -873,  -873,  6476,   506,    38,
    1680,   187,   496,   109,    13,  -119,   385,  -873,  -873,   528,
    -873,   533,   527,   150,  -873,   755,  5269,  -873,  1816,     6,
    -873,   124,   -34,   233,   363,   864,   728,   729,   316,  4976,
     571,  4976,   128,  4976,   -32,  4976,   -32,   -35,   547,   443,
     650,  -873,  -873,  -873,   210,  -873,   265,  -873,   268,   626,
     638,   742,   654,   663,   263,   267,  4976,   -32,  4976,   -32,
     558,  5404,  -873,  6476,  6476,  -873,  6476,  6476,  -873,  6476,
    5483,  6476,  5618,  6476,  -873,  -873,   804,    50,  -873,  -873,
    -873,   671,  -873,  -873,   708,   731,    66,  -873,   723,   739,
     620,   505,   193,  6476,  7511,   749,  -873,   405,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,   807,  -873,    21,  -873,
     907,   786,  3289,  6476,  6476,  6476,  6476,  6476,  6476,  6476,
    6476,  -873,  6476,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,   847,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,  7411,
    7411,  7411,  7411,  7411,  7411,  1109,  6476,  1072,   832,   479,
     841,   857,   889,   930,   942,   961,  -873,   515,   975,  4976,
     468,   583,   950,   983,  1000,   840,   992,  -873,   996,  1164,
    1009,   202,  -873,    73,   763,   924,  5190,  1183,  6476,  1109,
    1072,  6476,  6476,  6476,  6476,  6476,  6476,  6476,  1895,  -873,
    -873,  1031,  1120,    21,   933,   -85,  1248,  -873,  2890,  6476,
    6476,  -873,  6476,  6983,  -873,  6476,  7118,  -873,   366,   183,
     276,   529,  1103,  1128,   979,  1025,  6476,  6476,  6476,  6476,
    6476,  6476,  6476,  -873,  2969,  1027,  6476,  6476,  -873,  2969,
    1056,  1050,  1058,  -873,   316,  2969,  -873,  7411,  7411,    25,
    -873,  -873,   -11,  -873,   346,  -873,  -873,  -873,  1060,  -873,
    2969,  6476,  6476,  -873,  6476,  6476,  -873,  6476,  6476,  1130,
    2969,  -873,  1296,  6476,  1302,  6476,  1062,  1293,  1064,  -873,
    6476,  6476,  1315,  1173,  1179,  1180,  1181,  1182,  1146,  6476,
    -873,  -873,  -873,   -32,  6476,   -32,    62,  -873,    34,  -873,
      60,  1168,  1185,  -873,  -873,  6476,  6476,  6476,  1351,  1188,
    -873,  -873,  1357,  -873,  2030,  -873,  2109,  2245,  -873,  -873,
     342,   198,   453,  6048,  6127,  6476,  3928,    37,    46,  -873,
     465,  1191,  1192,  1193,  1194,  1195,  1196,  1197,  1198,  1199,
    1201,  -873,   397,  1265,   397,   397,   397,   676,  1220,  1220,
    1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,
    1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,  1220,
    1220,  -873,  -873,  -873,  -873,   780,   766,   766,   766,   766,
     766,   766,   766,   766,   766,   459,   459,   459,   459,   459,
     459,   459,   -61,   -61,   -61,   770,   770,   808,   808,   808,
     808,   808,   808,   808,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,   -97,  1208,  1373,
     300,  -873,  -873,  2324,  -873,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  1066,  -873,  2969,  6476,  6476,  2969,  1243,  -873,
     219,   407,  -873,  2460,  -873,  2539,  6476,   258,  -873,    17,
    -873,  3104,   964,  1241,  1235,   166,   500,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  1013,  -873,  -873,  1244,  1407,
    5697,  -873,    81,  -873,  1245,  1263,  1264,  4976,   546,  1268,
    1358,   238,  5833,   575,  1269,  6476,  6476,  -873,  7197,  -873,
    6476,  -873,    65,  -873,  6476,  6476,  1286,  2109,  1273,  1274,
    1275,  1277,  1278,  1279,  1282,   593,  2109,  1283,  1287,   655,
     -32,     5,  7332,  -873,   -32,   405,   215,   347,   387,    -5,
    1070,  3473,  1450,  6476,    31,  1456,  1463,   -32,   736,   743,
    1297,  1312,  1313,  1314,  1316,  1317,  -873,  1319,   100,  1326,
    1289,  1329,  1318,   -32,   -32,   -32,  1323,  1324,  1333,  6476,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,   -32,  1321,  6476,
     -32,  -873,  1490,  1493,  -873,   278,  1325,  1073,   140,   650,
     249,  3657,  1330,  1332,  1334,  1346,  -873,  -873,  -873,  1075,
    -873,  1077,  -873,  1017,  5190,  6476,  6476,   467,    88,   559,
    -873,    59,  6476,  6476,  -873,  6476,  6476,  -873,  6476,  6476,
    6476,  6476,  6476,  6476,  6476,  6476,  -873,  1109,  1072,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  7411,  6476,  2675,
    2969,  -873,  -873,  6476,  2754,  2969,  1336,  1079,  2109,   827,
    1335,  1340,  1020,  6476,  -873,  -873,  6476,  -873,  -873,  6476,
    -873,  1084,  -873,  1089,  1092,  -873,  5697,  1430,  1502,   111,
    1094,  -873,  1158,  1160,  -873,  -873,  1350,  -873,  1344,   299,
    1512,  -873,  -873,  -873,  1100,  -873,  2969,  -873,  6476,  6476,
    1102,  -873,   115,  -873,  1349,  1352,  4976,   884,  1353,  1325,
    1104,  1354,  1355,  6476,  1106,  -873,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  1108,  -873,  -873,  -873,  1262,  -873,  -873,
    4976,   -32,  -873,  -873,  1518,  -873,  1519,  1524,  1523,     3,
    -873,    -5,  6048,  6127,  4063,  -873,   488,  -873,  -873,  1525,
    6476,  -873,  -873,  -873,    43,  -873,   446,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  6476,   -32,  -873,
    -873,  -873,  1360,  1362,  1514,  -873,  -873,  -873,  -873,  6476,
    1365,  -873,  -873,   -32,  1361,  -873,  -873,   292,   978,  1136,
    1483,  -873,  -873,  1325,  1325,  1114,  1116,  1533,  1534,  1535,
    -873,  -873,  -873,  -873,  -873,   537,  -873,  -873,  -873,  1021,
    1367,   526,  6476,  6476,  6476,  6476,  6476,  6476,  -873,  -873,
    -873,  -873,   540,   573,   611,   633,   653,   666,   674,   709,
     710,   718,   726,   761,   445,   489,  -873,  1375,  -873,  1122,
    1319,  1377,  -873,  1124,  1126,  1319,  -873,  1381,  1132,  -873,
    -873,  -873,  -873,  1382,  1384,  1387,  -873,  -873,  -873,  1386,
    6476,  1388,   350,  -873,  -873,  -873,  1554,  5912,  -873,  2109,
     931,  -873,  -873,  1391,  1485,  1486,  -873,  -873,  1134,  -873,
    2969,  -873,  -873,  1325,  -873,  -873,   941,  1394,  1395,     5,
    -873,  1137,  1291,   -32,   -32,   -32,  -873,  1560,  -873,     3,
    -873,   507,   247,  -873,   383,  6476,  6476,  6476,  6476,  6476,
    6476,  6476,  6476,  -873,  -873,  -873,  1563,  6476,    55,  1568,
    1569,   -32,  1396,  -873,  -873,   -32,  1402,  6476,  -873,  -873,
    -873,   622,   635,   143,   147,  1161,  1404,  1405,  1046,  -873,
    -873,  1325,  1325,  1577,  1578,  1579,  6476,  1423,   564,  -873,
     762,   787,   788,   789,   790,   791,  -873,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  6476,  6476,
    -873,  -873,  -873,  -873,  -873,  -873,  1410,  -873,  -873,  -873,
    -873,  -873,  6476,  5912,  1422,  -873,  1411,  1143,  -873,  -873,
    6476,  6476,  2109,   943,  -873,  -873,  2969,  -873,  -873,  -873,
    -873,   -32,   -32,  -873,  -873,  -873,  -873,  -873,  -873,  6476,
    6476,  6476,  -873,  -873,  -873,   792,   809,   811,   812,   813,
     815,   817,   821,  -873,  -873,  1576,  6476,  -873,  -873,  -873,
    -873,  -873,  6476,  1412,  -873,   154,  1515,  -873,   163,  -873,
    1580,  1581,  1418,  6476,  -873,  1583,  1584,  1421,  6476,  -873,
     149,   248,  1424,  1431,  -873,  -873,  1429,   679,  -873,  -873,
    -873,  -873,  -873,  1432,  6476,  6476,  -873,  -873,  -873,  -873,
    -873,  -873,   831,   833,  -873,  1420,  1443,  -873,  -873,  1446,
    -873,  -873,  1147,  -873,   988,  1301,  -873,   842,   843,   844,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873,
    1447,   -32,  1417,  1153,  1441,  3841,  1417,  1155,  1449,  1451,
    -873,  1452,   298,   326,  -873,  1454,  1614,  1621,  1458,  6476,
    -873,  1625,  1627,  1464,  6476,  -873,  -873,  -873,   170,   218,
    -873,  -873,   858,   859,  -873,  -873,  -873,  -873,  -873,  1469,
    -873,   -32,  -873,  -873,  -873,  -873,  6476,  -873,  -873,  1417,
    -873,   457,  6048,  6127,  4142,  -873,   512,  -873,  1417,  -873,
    -873,  -873,  6476,  -873,  6476,  -873,  -873,  1471,  1473,  -873,
    1475,  1480,  1481,  -873,  1484,  1417,  1165,  1417,  1169,  -873,
    -873,  -873,  -873,  1487,  -873,  6476,   560,   289,  -873,   441,
    6476,  6476,  6476,  6476,  6476,  6476,  6476,  6476,  -873,  -873,
    1488,  1491,  -873,  -873,  -873,  -873,  -873,  -873,  -873,  1417,
    -873,  1417,   -32,   860,  6476,  6476,  6476,  -873,  -873,  -873,
     861,   862,   863,   883,   885,   888,   897,   898,  -873,  -873,
    -873,  -873,  -873,  -873,   899,   909,   918,  -873,  -873,  -873,
    -873,  -873,  -873,  -873,  -873,  -873,  -873,  -873
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   519,   520,   521,   522,   524,   525,
     532,   533,   551,   550,   552,   553,     0,     0,   534,   535,
     536,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   541,   523,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     3,     4,   102,   103,   121,   122,   123,
     124,   104,   105,   131,   132,   133,   135,   134,   563,     0,
     355,   357,   362,   364,   392,   394,   403,   405,   413,   417,
     420,   428,   441,   447,   449,   465,   562,   501,     0,     0,
     528,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   530,   531,     0,     0,     0,
       0,   525,   532,   533,   551,   550,     0,   563,   445,   465,
     562,   501,   525,   532,   558,     0,     0,    43,     0,     0,
     502,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   413,     0,
       0,   564,   353,   352,     0,    57,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,     0,   161,     0,     0,   125,     0,     0,   126,     0,
       0,     0,     3,     0,   498,   500,   106,     3,   111,   494,
      59,     0,   228,   232,     0,     0,     0,   446,     0,     0,
     525,   532,   533,     0,   113,     0,   450,     0,   526,   527,
     444,   443,   442,   452,   464,   457,   458,   459,   460,   463,
     461,   462,   455,   456,   453,   454,   493,   515,     0,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     347,   348,   349,   350,   351,   345,   472,     0,     0,     0,
     346,     0,     0,     0,     0,     0,     0,    47,   477,     0,
     476,   550,   468,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   559,
      36,     0,   515,     0,     0,   505,     0,    46,   504,     0,
       0,     6,     0,     0,    17,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,     0,     0,    51,     0,
       0,   139,     0,   151,     0,     0,   170,     0,     0,     0,
       3,   181,   287,   292,     0,   305,   307,   309,     0,    54,
       0,     0,     0,    56,     0,     0,    62,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,   154,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     495,   497,   499,     0,     0,     0,     0,    58,     0,   235,
       0,     0,     0,   274,     3,     0,     0,     0,     0,     0,
     114,   115,     0,   566,     0,   516,     0,     0,   555,   312,
     532,   533,   551,     0,     0,     0,     0,     0,     0,   311,
       0,   465,   501,     0,     0,     0,     0,     0,     0,     0,
       0,   354,   358,     0,   361,   360,   359,   363,   365,   374,
     377,   378,   373,   372,   371,   370,   369,   368,   367,   366,
     375,   376,   379,   383,   384,   385,   386,   387,   380,   381,
     382,   388,   389,   390,   391,   393,   401,   400,   399,   398,
     397,   396,   395,   402,   404,   411,   410,   409,   408,   407,
     406,   412,   416,   415,   414,   418,   419,   426,   425,   424,
     423,   422,   421,   427,   437,   439,   440,   438,   434,   433,
     432,   431,   430,   429,   435,   436,   448,   488,     0,     0,
     485,    11,    69,     0,    95,    96,    97,    98,    99,    66,
     471,    12,     0,    22,     0,     0,     0,     0,   537,    13,
     542,   543,    72,     0,   480,     0,     0,   280,   469,     0,
     467,   504,     0,     0,     0,   488,   485,   347,   348,   349,
     350,   351,   345,   346,   560,     0,    37,    45,     0,     0,
       0,   506,     0,   503,     0,     0,     0,     0,     0,     0,
     522,   550,     0,     0,     0,     0,     0,    33,     0,    34,
       0,    35,     0,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,     0,     0,   142,   516,     0,     0,   406,   412,     0,
       0,     0,     0,     0,   294,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,   466,     0,     0,
       0,     0,     0,   516,     0,   516,     0,     0,     0,     0,
     127,   128,   129,   130,   344,   496,   107,     0,     0,     0,
       0,   108,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,   451,   492,     0,
     517,     0,   556,     0,     0,     0,     0,     0,     0,     0,
     337,     0,     0,     0,   338,     0,     0,   339,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,     0,    78,
      80,    82,    85,    83,    84,    79,    81,     0,     0,     0,
       0,    16,   481,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   547,   549,     0,   546,   548,     0,
     479,     0,   475,     0,     0,   278,     0,     0,     0,     0,
       0,   537,   542,   543,   561,    44,     0,   508,     0,   510,
       0,     7,     5,     8,     0,    19,     0,     9,     0,     0,
       0,    26,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    89,    88,    92,    90,
      91,    87,    49,     0,    93,    94,    52,   136,   141,   140,
       0,     0,   150,   167,     0,   169,     0,     0,     0,     0,
     180,     0,     0,     0,     0,   192,     0,   288,   301,     0,
       0,   295,   296,   565,   297,   293,     0,   306,   308,   310,
      55,    60,    61,    64,    65,    39,    38,     0,     0,   171,
     544,   545,     0,     0,   152,   155,   156,   159,   160,     0,
       0,   173,   110,     0,     0,   233,   234,     0,     0,     0,
       0,   230,   191,     0,     0,     0,     0,     0,     0,     0,
     273,   231,   116,   117,   118,   542,   491,   518,   557,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   322,   329,
     335,   336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   488,   485,   356,     0,   487,     0,
     490,     0,   484,     0,     0,   489,    68,     0,     0,    23,
     540,   539,   538,     0,     0,     0,   478,   473,   474,     0,
       0,     0,   280,   470,   509,   507,     0,     0,   511,     0,
       0,   284,   283,     0,     0,     0,    10,    28,     0,    29,
       0,    32,   185,     0,    41,    40,     0,   518,   518,     0,
     143,     0,   144,     0,     0,     0,   177,     0,   183,     0,
     179,     0,     0,   195,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,   290,   303,     0,     0,   298,     0,
       0,     0,     0,    76,    77,     0,     0,     0,   174,   109,
     229,     0,     0,     0,     0,     0,     0,     0,     0,   190,
     188,     0,     0,     0,     0,     0,     0,   537,   543,   332,
       0,     0,     0,     0,     0,     0,   340,   341,   342,   343,
     314,   316,   318,   321,   319,   320,   315,   317,     0,     0,
      70,   486,    71,   483,   482,    67,   518,    73,    74,    75,
     279,   281,     0,     0,     0,   513,     0,     0,    20,    27,
       0,     0,     0,     0,   184,    14,     0,    50,    53,   138,
     137,   516,     0,   148,   168,   165,   166,   178,   182,     0,
       0,     0,   196,   197,   194,     0,     0,     0,     0,     0,
       0,     0,     0,   289,   302,     0,     0,   299,   300,   176,
     172,   153,     0,     0,   164,     0,     3,   267,     0,   264,
       0,     0,     0,     0,   241,     0,     0,     0,     0,   236,
       0,     0,     0,     0,   237,   238,     0,     0,   187,   189,
     275,   276,   277,     0,     0,     0,   323,   325,   328,   326,
     327,   324,     0,     0,    24,     0,     0,   514,   512,   518,
     286,   285,     0,    30,     0,   145,   146,     0,     0,     0,
     198,   200,   202,   205,   203,   204,   199,   201,   291,   304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     254,     0,     0,     0,   255,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,   242,   239,   240,     0,     0,
     270,   119,     0,     0,   330,   331,   282,   279,    21,   518,
      15,     0,   149,   206,   208,   207,     0,   163,   266,     0,
     210,   551,     0,     0,     0,   209,     0,   263,     0,   252,
     253,   245,     0,   248,     0,   251,   244,     0,     0,   256,
       0,     0,     0,   257,     0,     0,     0,     0,     0,   333,
     334,    31,   147,     0,   265,     0,     0,     0,   214,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   262,
       0,     0,   260,   261,   247,   258,   259,   246,   272,     0,
     269,     0,     0,     0,     0,     0,     0,   215,   216,   213,
       0,     0,     0,     0,     0,     0,     0,     0,   249,   250,
     271,   268,   162,   211,     0,     0,     0,   217,   219,   221,
     224,   222,   223,   218,   220,   225,   227,   226
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -873,  -873,     2,  -873,  -685,  -872,  -873,  -873,  -873,  -873,
    -873,   368,  1632,  -873,  -873,  -873,   774,  -873,  -397,  -871,
     -43,  -873,  -873,  -873,  -873,   922,  -873,  -873,  -611,  1007,
    -372,    98,  1307,    -2,  -263,  -873,   927,  1406,   945,  1380,
    -873,   657,   -25,   740,  1043,   866,   252,  -873,  -151,  -327,
    -873,     7,   181,     9,    14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   252,    74,    75,   693,    76,    77,    78,    79,
      80,  1183,    81,    82,   476,   718,   889,    83,   431,  1187,
     213,    84,    85,    86,   478,   745,    87,   750,   364,   663,
     438,   137,   200,   150,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   227,   246,
     104,   139,   151,   140,   141
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,   523,     2,   168,   673,  1040,   878,   404,   597,   105,
     600,   106,   214,   702,   823,   145,   107,   218,   219,   888,
     830,   385,   160,   162,   164,   166,   167,  1047,   193,   174,
     176,   178,   827,    26,   182,   183,   432,   433,   187,   189,
     434,   128,   635,   636,     3,   169,   204,   205,   194,   742,
     209,   211,    28,   358,   424,   899,   359,   700,   743,   129,
     225,   130,   152,   153,   154,   772,   155,  1066,   798,   248,
     250,   432,   433,   389,   775,   434,   432,   433,   146,  1175,
     434,   481,   649,   799,   156,   773,   181,   441,   627,   157,
     800,   215,   474,   475,   776,   650,   839,   435,   436,   437,
      88,   313,   314,   360,   739,   740,   338,   442,   747,   216,
     340,   341,   342,   343,   344,   345,   416,   348,   350,   352,
     353,  1024,   355,   125,   126,   356,  1012,   171,   173,   251,
    1025,   390,   435,   436,   437,   482,   417,   435,   436,   437,
     190,   192,   917,   918,   381,   391,   383,   425,   206,   208,
      73,   432,   433,    73,   703,   434,   416,   405,  1190,   415,
     224,   420,  1195,   422,  1266,   432,   433,  1191,   184,   434,
    1153,  1196,   386,  1267,   432,   433,   417,   185,   434,   195,
     430,   432,   433,   179,   456,   434,   458,   180,   430,   463,
      73,   464,   465,   993,   466,   467,   900,   468,   470,   471,
     468,   472,   495,   496,   828,    73,   699,   129,  1067,   130,
     170,  1189,   435,   436,   437,    73,   195,   199,  1192,   744,
    1176,   489,  1197,   774,  1268,   198,   435,   436,   437,   432,
     433,   158,   777,   434,   171,   435,   436,   437,   443,   386,
     212,   746,   435,   436,   437,   971,   859,   840,   741,   554,
     510,   513,   514,   515,   516,   517,   518,   519,   520,   511,
     521,   106,   421,  1271,   423,   426,   512,   418,   138,   196,
     419,   207,  1272,   695,   969,   416,   367,   840,   565,   566,
     567,   568,   569,   570,   571,   457,   919,   459,   626,   197,
     435,   436,   437,   441,   368,   417,   444,   110,   111,   347,
     860,   122,   890,   387,   884,   122,   388,   418,  1193,   217,
     419,   365,  1198,   442,  1269,   377,   445,   416,   230,   231,
     232,   944,  1194,  1273,   849,   493,  1199,   226,  1270,   384,
     947,   948,   949,   366,   598,  1252,  1280,   417,   814,   815,
     392,   938,   939,   119,  1256,   940,   799,   612,   668,   942,
      88,  1325,   228,   800,  1149,   938,   939,   704,   487,   940,
     488,   123,   669,  1292,   632,   123,   634,   118,   229,   637,
     638,   639,   640,   641,   642,   643,   945,   946,   368,   124,
     129,  1298,   130,   765,   816,  1307,   653,   654,   655,   439,
     656,   658,   440,   659,   664,   125,   126,   388,   393,  1327,
      73,   120,   697,   698,   678,   679,   680,   681,   682,   683,
     684,   441,   394,  1274,   687,   688,   129,   191,   130,   121,
     198,   262,    73,   705,   706,   649,   418,  1275,  1334,   419,
     452,   442,   701,  1163,   454,   950,   886,  1349,   826,   710,
     711,   670,   712,   713,   443,   714,   715,   446,   453,   119,
     127,   720,   455,   722,  1358,   671,  1360,  1068,   726,   727,
     984,   985,  1032,  1312,   941,   803,  1016,   735,   418,   441,
     395,   419,   737,   665,   251,  1368,   887,  1313,  1080,  1017,
     804,   310,   311,   752,   753,   754,   751,   805,  1380,   442,
    1381,  1314,  1048,   778,  1050,   962,   779,   780,   407,   963,
     268,   767,   768,   769,   771,  1315,   781,   120,   964,   782,
     783,   965,   966,   784,   785,   967,  1055,  1016,   312,  1056,
    1057,   310,   311,  1069,  1070,   764,   817,   818,   396,  1058,
    1133,   666,  1059,  1060,   986,  1159,  1061,  1062,   407,  1160,
    1340,   307,   397,  1341,  1342,   667,  1089,  1090,   409,   308,
     309,   410,   411,  1343,   302,  1161,  1344,  1345,   312,   645,
    1346,  1347,   443,   125,   126,   170,   334,   125,   126,  1164,
      73,   736,   819,   738,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   492,  1364,   460,
      73,   407,  1365,   310,   311,   685,   310,   311,   335,   406,
     689,   409,   407,   408,   410,   411,   696,   461,  1366,   336,
    1118,  1082,   409,   810,   811,   410,   411,   337,   766,   412,
     443,   709,  1335,   429,   825,   799,   307,  1369,   430,   653,
     312,   717,   800,   312,   427,   428,  1144,   110,   111,   371,
     372,   373,   374,   375,   261,   339,   413,   613,   838,   414,
     614,   786,  1158,   968,  1119,   844,   814,   815,   602,   603,
     248,   432,   433,   854,   855,   434,   857,   261,   858,   804,
     486,   354,   861,   862,  1063,   759,   805,   761,   763,   366,
     804,   310,   311,   817,   818,   357,   413,   805,   366,   414,
     881,   261,   376,  1162,  1208,  1209,   610,   388,  1348,   896,
     253,   898,  1096,   254,   255,   622,   369,   118,   105,   672,
     106,   370,  1099,   256,   430,   107,   257,   258,   312,   730,
     259,   260,   435,   436,   437,   845,  1106,   930,   846,  1215,
     110,   111,   371,   372,   373,   374,   375,   933,   439,   413,
     402,   440,   414,   401,   403,   970,  1367,   904,   905,    89,
     413,   906,   731,   414,   851,   615,   616,   852,   105,  1107,
     106,  1207,   959,   960,   961,   107,   269,   270,   271,   272,
     972,   973,   872,   974,   975,   388,   976,   977,   978,   979,
     980,   981,   982,   983,   807,   485,   263,   264,   877,   879,
     732,   447,   882,   883,   885,   809,   987,  1108,   812,    88,
     118,   991,  1185,   448,   821,   903,   824,  1186,   907,   908,
     909,  1003,   733,  1253,  1004,  1188,  1257,  1005,   449,  1109,
    1186,   924,   925,   926,  1009,   273,   274,   275,   276,   277,
     278,   279,   789,   280,   876,   931,   450,   388,   934,  1110,
     265,   266,   267,   281,   282,   790,  1021,  1022,   451,    88,
     477,   261,  1111,   791,  1028,   551,   552,   553,   864,  1279,
    1112,  1036,   212,   473,  1186,   108,   109,   873,   110,   111,
     112,   113,   114,   115,   116,   398,   399,   400,  1041,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   792,   793,
    1051,  1052,  1054,   479,   292,  1113,  1114,   794,  1065,   315,
     316,   317,   318,   319,  1115,   795,  1326,  1328,   483,   303,
     304,   480,  1116,   305,   306,  1071,   294,   295,   296,   297,
     298,   299,   910,   117,   484,   388,   322,  1076,   491,   323,
     324,   325,   326,   327,   328,   329,   330,   378,   118,   379,
     796,   865,   320,   321,   628,   629,   293,  1117,  1216,   300,
     301,   556,   557,   558,   559,   560,   561,   562,   563,   564,
    1100,  1101,  1102,  1103,  1104,  1105,   867,   868,   869,   870,
     871,   789,   499,  1217,  1218,  1219,  1220,  1221,  1240,  1042,
     989,   990,   331,   332,   333,   994,   995,   494,   790,   998,
     791,   792,   793,   233,   794,  1241,   795,  1242,  1243,  1244,
     796,  1245,   234,  1246,   235,   236,   999,  1247,  1131,   388,
    1120,   601,  1122,   237,   238,  1136,  1072,  1284,   239,  1285,
     604,   865,   867,   871,   240,   241,   621,  1020,  1293,  1294,
    1295,  1078,   242,   243,   244,   245,   605,  1127,  1129,   622,
     789,   790,   791,  1081,  1329,  1330,  1383,  1387,  1388,  1389,
     572,   573,   574,  1165,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,   233,   792,  1029,   793,  1174,  1030,   794,   606,  1390,
     234,  1391,   235,   236,  1392,  1184,   795,   796,   865,   691,
     692,   237,   238,  1393,  1394,  1395,   239,   233,   867,   497,
     599,   498,   240,   241,  1213,  1396,   234,   871,   235,   236,
     242,   243,   244,   245,  1397,   630,   631,   237,   238,   607,
    1138,  1206,   239,   388,   648,   388,  1222,  1223,   240,   241,
    1145,   608,  1233,  1146,   233,   388,   242,   243,   244,   245,
    1225,  1226,   617,   234,   618,   235,   236,  1150,  1230,  1231,
     609,  1154,  1155,  1156,   237,   238,   617,  1083,   831,   239,
    1084,  1085,  1086,  1087,   611,   240,   241,  1237,  1238,  1239,
     676,   492,   619,   242,   243,   244,   245,  1290,   620,  1179,
     388,   622,  1200,  1181,  1249,  1201,   623,  1202,  1203,   624,
    1250,   577,   578,   579,   580,   581,   582,   583,  1255,   625,
     522,  1261,   524,   525,   526,   388,  1265,   834,   633,   388,
    1137,   958,   388,   617,  1002,  1097,   495,   677,   495,   686,
     646,  1143,  1282,  1283,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   690,   496,   694,
     496,   707,   708,   723,   496,   725,   496,   495,   808,  1235,
    1236,   891,   708,  1306,   943,   708,   956,   388,   957,   388,
     997,   388,   105,   651,   106,  1006,   388,  1320,   674,   107,
    1007,   629,  1324,  1008,   388,  1013,   629,   814,   815,   817,
     818,   495,  1019,  1023,   852,  1033,   708,  1037,   388,  1038,
     388,  1039,   692,   675,  1184,  1091,   708,  1092,   708,   647,
    1336,  1337,  1339,  1121,   388,  1123,   629,  1124,   388,   716,
    1350,   719,  1351,  1126,   388,   495,  1142,   721,  1151,   496,
    1152,   692,   724,  1232,  1229,   388,   728,  1234,  1289,   388,
    1291,   692,   734,  1363,  1299,   708,  1308,   708,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1359,   708,   748,  1297,
    1361,   708,   729,    88,   201,   203,   575,   576,   730,   731,
     732,   733,  1384,  1385,  1386,   749,   755,   756,   757,   787,
     788,   797,   789,   790,   791,   792,   793,   794,   795,  -112,
     796,   293,  -112,  -112,  -112,  -112,  -112,   801,   802,  1332,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,   813,   832,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,   833,   836,   835,   841,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,   842,   843,   848,  -112,  -112,   847,   853,  -112,
    -112,   863,   865,   866,   867,  -112,   868,   869,   870,  -112,
    1382,   871,   874,  -112,  -112,   897,   875,   901,  -112,     4,
       5,     6,     7,     8,   902,   921,   911,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,  -112,
    -112,   912,   913,   914,   920,   915,   916,   922,   929,   136,
      30,   388,   927,   928,   923,   935,    36,   932,   936,   952,
     430,   953,    38,   954,   955,   996,  1010,  1011,  1014,  1000,
      46,    47,    48,    49,  1001,  1015,    52,  1018,  1026,  1043,
    1044,  1027,  1031,  1034,  1035,  1045,    57,    58,  1046,  1073,
    1064,  1074,    59,  1075,  1077,  -112,  1088,  1079,  1093,  1094,
    1095,  -112,  -112,  1098,  1120,    63,  1122,  -112,  -112,  -112,
    1125,  1127,  -112,  1128,  -112,  -112,  1129,  1130,  1132,  1134,
    1139,  1140,  1141,  1147,  1148,  1157,    64,    65,  1173,  1177,
    1178,  1182,  1180,  1204,  1205,  1210,  1211,  1212,  1214,  1224,
    1227,  1248,  1228,  1251,  1254,  1258,  1259,  1260,  1262,  1263,
    1264,  1286,  1186,  1276,     4,     5,     6,     7,     8,  1278,
    1277,  1281,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,  1287,  1288,  1296,  1300,  1309,  1317,
    1310,  1311,    66,  1316,   136,    30,  1318,  1319,    67,    68,
    1321,    36,  1322,  1323,    69,   147,   148,    38,  1331,    72,
    1352,    73,  1353,   149,  1354,    46,    47,    48,    49,  1355,
    1356,    52,   172,  1357,  1333,  1049,   937,  1378,  1362,   850,
    1379,    57,    58,   555,   527,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      63,   142,   143,   133,   134,   361,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   136,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,   346,     0,    72,     0,    73,     0,   149,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,   136,
      30,     0,    67,    68,     0,     0,    36,     0,    69,     0,
      71,   362,    38,    72,     0,    73,     0,   363,     0,     0,
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
       0,     0,     0,     0,    69,     0,    71,   382,     0,    72,
       0,    73,     0,   149,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,     0,    71,    38,     0,    72,   644,
      73,     0,   149,     0,    46,    47,    48,    49,     0,     0,
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
      71,   758,     0,    72,     0,    73,     0,   149,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,   136,    30,
       0,    67,    68,     0,     0,    36,     0,    69,     0,    71,
     760,    38,    72,     0,    73,     0,   149,     0,     0,    46,
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
       0,     0,     0,    69,     0,    71,     0,     0,    72,   762,
      73,     0,   149,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,   136,    30,     0,    67,    68,     0,     0,
      36,     0,    69,     0,    71,   806,    38,    72,     0,    73,
       0,   149,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    63,
     142,   143,   133,   134,   361,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      64,    65,   136,    30,     0,     0,     0,     0,     0,    36,
       0,     0,     0,     0,     0,    38,     0,     0,     0,     0,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,    63,     0,
       0,     0,    67,    68,     0,     0,     0,     0,    69,     0,
      71,   820,     0,    72,     0,    73,     0,   149,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,   136,    30,
       0,    67,    68,     0,     0,    36,     0,    69,     0,    71,
     822,    38,    72,     0,    73,     0,   363,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   142,   143,   133,   134,   361,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   136,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,     0,    71,   988,     0,    72,     0,
      73,     0,   149,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,   136,    30,     0,    67,    68,     0,     0,
      36,     0,    69,     0,    71,   992,    38,    72,     0,    73,
       0,   363,     0,     0,    46,    47,    48,    49,     0,     0,
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
      71,     0,     0,    72,     0,    73,     0,   652,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   361,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,     0,    71,
      38,     0,    72,     0,    73,     0,   149,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,    69,     0,    71,     0,     0,    72,     0,    73,
       0,   829,     4,     5,     6,     7,     8,     0,     0,     0,
       9,   500,   501,   502,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,   503,   504,    26,     0,     0,
      27,    28,    29,    30,   505,   506,    33,    34,    35,    36,
      37,     0,     0,     0,     0,    38,    39,    40,    41,    42,
      43,   507,   508,    46,    47,    48,    49,    50,    51,    52,
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
       0,     0,    72,     0,    73,   509,     4,     5,     6,     7,
       8,     0,     0,     0,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,   892,
     893,    26,     0,     0,    27,    28,    29,    30,    31,   894,
      33,    34,    35,    36,    37,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,    55,
      56,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,    60,     0,     0,     0,    61,    62,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,    70,    71,     0,     0,    72,     0,    73,   895,
       4,     5,     6,     7,     8,     0,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,     0,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
       0,     0,     0,    55,    56,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,    60,     0,     0,
       0,    61,    62,     0,     0,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,    70,    71,     0,     0,
      72,     0,    73,   951,     4,     5,     6,     7,     8,     0,
       0,     0,     9,    10,    11,  1301,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,  1302,  1303,    26,
       0,     0,    27,    28,    29,    30,    31,  1304,    33,    34,
      35,    36,    37,     0,     0,     0,     0,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,     0,     0,     0,    55,    56,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,    60,     0,     0,     0,    61,    62,     0,     0,     0,
      63,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,   136,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,    63,     0,    69,
      70,    71,     0,     0,    72,     0,    73,  1305,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   136,    30,     0,     0,
      67,    68,     0,    36,     0,     0,    69,   175,    71,    38,
       0,    72,     0,    73,   770,     0,     0,    46,    47,    48,
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
       0,    69,   175,    71,     0,     0,    72,     0,    73,  1053,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,   175,    71,     0,     0,    72,     0,    73,  1338,     4,
       5,     6,     7,     8,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,     0,
       0,     0,    55,    56,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,    60,     0,     0,     0,
      61,    62,     0,     0,     0,    63,     4,     5,     6,     7,
       8,     0,     0,     0,   142,   143,   133,   134,   135,    14,
      15,    16,    17,    18,    19,    20,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,   136,    30,     0,     0,
       0,     0,     0,    36,     0,     0,     0,     0,     0,    38,
       0,     0,     0,     0,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,    66,     0,     0,     0,     0,     0,    67,    68,
       0,     0,    63,     0,    69,    70,    71,     0,     0,    72,
       0,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,     0,    71,    38,     0,    72,   144,    73,     0,
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
      67,    68,     0,     0,     0,     0,    69,   175,    71,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,   177,    71,    38,     0,
      72,     0,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   220,   221,   222,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,   223,     0,     0,
       0,     0,    64,    65,   136,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,   210,    71,     0,     0,    72,     0,    73,     0,     0,
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
       0,     0,     0,     0,    69,     0,    71,   247,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   142,   143,   133,   134,   135,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,   136,    30,     0,     0,    67,    68,     0,
      36,     0,     0,    69,     0,    71,    38,     0,    72,   249,
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
       0,     0,    67,    68,   351,     0,     0,     0,    69,     0,
      71,     0,     0,    72,     0,    73,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,   462,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,   380,    71,
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
       0,     0,    69,     0,    71,     0,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,     0,    71,    38,   469,    72,     0,    73,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,   251,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     136,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,     0,    71,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,   660,
       8,     0,     0,     0,   142,   143,   133,   134,   661,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,    66,     0,     0,   136,    30,     0,    67,
      68,     0,     0,    36,     0,    69,     0,    71,   837,    38,
      72,     0,    73,     0,     0,     0,     0,    46,    47,    48,
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
       0,    69,     0,    71,   247,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   142,
     143,   133,   134,   135,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,   136,    30,     0,    67,    68,     0,     0,    36,     0,
      69,     0,    71,  1135,    38,    72,     0,    73,     0,     0,
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
      67,    68,     0,     0,     0,     0,    69,     0,   159,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,   161,    38,     0,
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
      69,     0,   163,     0,     0,    72,     0,    73,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   135,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,   136,
      30,     0,     0,    67,    68,     0,    36,     0,     0,    69,
       0,   165,    38,     0,    72,     0,    73,     0,     0,     0,
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
      36,     0,     0,    69,     0,   186,    38,     0,    72,     0,
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
     188,     0,     0,    72,     0,    73,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   142,   143,   133,   134,   135,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,   136,    30,     0,
       0,    67,    68,     0,    36,     0,     0,    69,     0,    71,
      38,     0,    72,     0,   202,     0,     0,     0,    46,    47,
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
       0,     0,    69,     0,   349,     0,     0,    72,     0,    73,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,   660,     8,     0,     0,     0,   142,
     143,   133,   134,   661,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,   136,    30,     0,     0,    67,    68,     0,    36,     0,
       0,    69,     0,   657,    38,     0,    72,     0,    73,     0,
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
      67,    68,     0,     0,     0,     0,    69,     0,   662,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   136,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,   856,    38,     0,
      72,     0,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   131,   132,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   136,     0,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,     0,   880,     0,     0,    72,     0,    73,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
    -563,    71,     0,     0,    72,     0,    73,  -563,  -563,  -563,
       0,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,     0,     0,     0,
    -563,     0,     0,     0,     0,     0,     0,     0,  -563,  -563,
       0,     0,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,  -563,
    -563,  -563,  -563,     0,  -563,  -563,     0,     0,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,  -563,     0,  -563,  -563,
    -563,  -563,  -563,  -563,  -563,  -563,     0,     0,  -563,  -563,
    -563,  -563,  -563,     0,     0,  -563,  -563,  -563,     0,     0,
     490
};

static const yytype_int16 yycheck[] =
{
       2,   264,     0,    28,   401,   877,   691,   158,   335,     2,
     337,     2,    55,    24,   625,    17,     2,    60,    61,    24,
     631,    15,    24,    25,    26,    27,    28,    24,    28,    31,
      32,    33,    15,    28,    36,    37,    11,    12,    40,    41,
      15,    12,   369,   370,     0,    15,    48,    49,    48,    15,
      52,    53,    32,    15,    89,    24,    18,   429,    24,   178,
      62,   180,    11,    12,    13,    28,    15,    24,   165,    71,
      72,    11,    12,   107,    28,    15,    11,    12,    15,    24,
      15,    15,   167,   180,    33,    48,    15,    28,    15,    38,
     187,    15,    42,    43,    48,   180,    15,    72,    73,    74,
       2,   162,   163,    65,    42,    43,   108,    48,   480,    33,
     112,   113,   114,   115,   116,   117,    28,   119,   120,   121,
     122,     6,   124,   110,   111,   127,    15,    29,    30,    79,
      15,   165,    72,    73,    74,    69,    48,    72,    73,    74,
      42,    43,    42,    43,   146,   179,   148,   182,    50,    51,
     185,    11,    12,   185,   165,    15,    28,   159,    15,   161,
      62,   163,    15,   165,    15,    11,    12,    24,    13,    15,
    1042,    24,   166,    24,    11,    12,    48,    13,    15,   179,
     185,    11,    12,    11,   186,    15,   188,    15,   185,   191,
     185,   193,   194,   804,   196,   197,   165,   199,   200,   201,
     202,   203,   181,   182,   187,   185,   181,   178,   165,   180,
     180,  1082,    72,    73,    74,   185,   179,   185,    75,   185,
     165,   223,    75,   186,    75,   179,    72,    73,    74,    11,
      12,   180,   186,    15,   136,    72,    73,    74,   179,   166,
      15,   181,    72,    73,    74,   186,   181,   166,   186,   292,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   252,
     262,   252,   164,    15,   166,   167,   252,   179,    16,    28,
     182,   185,    24,   424,   186,    28,   167,   166,   303,   304,
     305,   306,   307,   308,   309,   187,   186,   189,    86,    48,
      72,    73,    74,    28,   185,    48,    28,   110,   111,   118,
     672,   107,   699,   179,    89,   107,   182,   179,   165,    57,
     182,   130,   165,    48,   165,   165,    48,    28,    66,    67,
      68,   181,   179,    75,    86,   227,   179,    11,   179,   148,
      81,    82,    83,   183,   336,   181,  1207,    48,   119,   120,
     107,    63,    64,   107,   181,    67,   180,   349,   165,   746,
     252,   181,    11,   187,  1039,    63,    64,    11,   165,    67,
     167,   167,   179,  1235,   366,   167,   368,   180,    11,   371,
     372,   373,   374,   375,   376,   377,   748,   749,   185,   185,
     178,  1252,   180,   185,   165,  1256,   388,   389,   390,   179,
     392,   393,   182,   395,   396,   110,   111,   182,   165,   181,
     185,   165,   427,   428,   406,   407,   408,   409,   410,   411,
     412,    28,   179,   165,   416,   417,   178,   180,   180,   183,
     179,    86,   185,    77,    78,   167,   179,   179,  1299,   182,
     167,    48,   430,   186,   167,   186,    89,  1308,   180,   441,
     442,   165,   444,   445,   179,   447,   448,   179,   185,   107,
     165,   453,   185,   455,  1325,   179,  1327,    11,   460,   461,
     787,   788,   859,   165,   186,   165,   167,   469,   179,    28,
     107,   182,   474,   107,    79,   186,    89,   179,   186,   180,
     180,   134,   135,   485,   486,   487,   484,   187,  1359,    48,
    1361,   165,   889,    28,   891,    28,    31,    32,    31,    32,
     103,   503,   504,   505,   506,   179,    41,   165,    41,    44,
      45,    44,    45,    48,    49,    48,    28,   167,   171,    31,
      32,   134,   135,    77,    78,   183,   119,   120,   165,    41,
     180,   165,    44,    45,   797,    28,    48,    49,    31,    32,
      28,    79,   179,    31,    32,   179,   943,   944,    41,    87,
      88,    44,    45,    41,   117,    48,    44,    45,   171,   378,
      48,    49,   179,   110,   111,   180,   164,   110,   111,   186,
     185,   473,   165,   475,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   182,    28,    31,
     185,    31,    32,   134,   135,   414,   134,   135,   178,    28,
     419,    41,    31,    32,    44,    45,   425,    49,    48,   165,
     165,   938,    41,   615,   616,    44,    45,   178,   165,    48,
     179,   440,   165,   180,   626,   180,    79,   186,   185,   631,
     171,   450,   187,   171,    87,    88,  1033,   110,   111,   112,
     113,   114,   115,   116,   179,    15,   179,   179,   650,   182,
     182,   186,  1049,   186,   165,   657,   119,   120,   179,   180,
     662,    11,    12,   665,   666,    15,   668,   179,   670,   180,
     165,    15,   674,   675,   186,   494,   187,   496,   497,   183,
     180,   134,   135,   119,   120,   179,   179,   187,   183,   182,
     692,   179,   165,   186,  1091,  1092,   181,   182,   186,   701,
      28,   703,   165,    31,    32,   179,   178,   180,   701,   180,
     701,   178,   186,    41,   185,   701,    44,    45,   171,   179,
      48,    49,    72,    73,    74,   179,   186,   729,   182,   165,
     110,   111,   112,   113,   114,   115,   116,   739,   179,   179,
      11,   182,   182,    15,    15,   186,   186,    11,    12,   751,
     179,    15,   179,   182,   179,   172,   173,   182,   751,   186,
     751,  1088,   764,   765,   766,   751,    90,    91,    92,    93,
     772,   773,   179,   775,   776,   182,   778,   779,   780,   781,
     782,   783,   784,   785,   603,   165,   104,   105,   690,   691,
     179,   165,   694,   695,   696,   614,   798,   186,   617,   701,
     180,   803,   180,   165,   623,   707,   625,   185,    72,    73,
      74,   813,   179,  1185,   816,   180,  1188,   819,    76,   186,
     185,   723,   724,   725,   826,   149,   150,   151,   152,   153,
     154,   155,   179,   157,   179,   737,   182,   182,   740,   186,
     158,   159,   160,   167,   168,   179,   848,   849,   185,   751,
     179,   179,   186,   179,   856,     8,     9,    10,   677,   180,
     186,   863,    15,    59,   185,   107,   108,   686,   110,   111,
     112,   113,   114,   115,   116,    11,    12,    13,   880,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   179,   179,
     892,   893,   894,   185,   109,   186,   186,   179,   900,   129,
     130,   131,   132,   133,   186,   179,  1278,  1279,   185,   143,
     144,   180,   186,   147,   148,   917,   136,   137,   138,   139,
     140,   141,   179,   165,   185,   182,   118,   929,   179,   121,
     122,   123,   124,   125,   126,   127,   128,   182,   180,   184,
     179,   179,   172,   173,   181,   182,   161,   186,   186,   169,
     170,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     962,   963,   964,   965,   966,   967,   179,   179,   179,   179,
     179,   179,   186,   186,   186,   186,   186,   186,   186,   881,
     799,   800,   174,   175,   176,   804,   805,   180,   179,   808,
     179,   179,   179,    15,   179,   186,   179,   186,   186,   186,
     179,   186,    24,   186,    26,    27,   179,   186,  1010,   182,
     179,   179,   179,    35,    36,  1017,   918,   186,    40,   186,
     179,   179,   179,   179,    46,    47,   186,   846,   186,   186,
     186,   933,    54,    55,    56,    57,   179,   179,   179,   179,
     179,   179,   179,    65,   186,   186,   186,   186,   186,   186,
     310,   311,   312,  1055,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,    15,   179,   179,   179,  1067,   182,   179,   179,   186,
      24,   186,    26,    27,   186,  1077,   179,   179,   179,    29,
      30,    35,    36,   186,   186,   186,    40,    15,   179,   182,
      18,   184,    46,    47,  1096,   186,    24,   179,    26,    27,
      54,    55,    56,    57,   186,   181,   182,    35,    36,   179,
     179,    65,    40,   182,   181,   182,  1118,  1119,    46,    47,
     179,   179,   179,   182,    15,   182,    54,    55,    56,    57,
    1132,  1133,   182,    24,   184,    26,    27,  1039,  1140,  1141,
     179,  1043,  1044,  1045,    35,    36,   182,    11,   184,    40,
      14,    15,    16,    17,   179,    46,    47,  1159,  1160,  1161,
     181,   182,   179,    54,    55,    56,    57,   179,   168,  1071,
     182,   179,    11,  1075,  1176,    14,   180,    16,    17,    15,
    1182,   315,   316,   317,   318,   319,   320,   321,  1186,   180,
     263,  1193,   265,   266,   267,   182,  1198,   184,    15,   182,
    1019,   184,   182,   182,   184,   184,   181,   182,   181,   182,
     179,  1030,  1214,  1215,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   181,   182,   181,
     182,   181,   182,   181,   182,   181,   182,   181,   182,  1151,
    1152,   181,   182,  1255,   181,   182,   181,   182,   181,   182,
     181,   182,  1255,    15,  1255,   181,   182,  1269,   165,  1255,
     181,   182,  1274,   181,   182,   181,   182,   119,   120,   119,
     120,   181,   182,   181,   182,   181,   182,   181,   182,   181,
     182,    29,    30,   165,  1296,   181,   182,   181,   182,   179,
    1302,  1303,  1304,   181,   182,   181,   182,   181,   182,   179,
    1312,    15,  1314,   181,   182,   181,   182,    15,   181,   182,
      29,    30,    29,  1142,   181,   182,    11,  1146,   181,   182,
      29,    30,   186,  1335,   181,   182,   181,   182,  1340,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,   181,   182,   180,  1251,
     181,   182,   179,  1255,    47,    48,   313,   314,   179,   179,
     179,   179,  1364,  1365,  1366,   180,    15,   179,    11,   178,
     178,   106,   179,   179,   179,   179,   179,   179,   179,     0,
     179,   161,     3,     4,     5,     6,     7,   179,    15,  1291,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,   165,   168,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,   186,    15,   179,   179,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,   179,   179,    86,    66,    67,   179,   179,    70,
      71,   165,   179,   179,   179,    76,   179,   179,   179,    80,
    1362,   179,   179,    84,    85,    15,   179,    11,    89,     3,
       4,     5,     6,     7,    11,   186,   179,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   110,
     111,   179,   179,   179,   168,   179,   179,   168,   165,    33,
      34,   182,   179,   179,   186,    15,    40,   186,    15,   179,
     185,   179,    46,   179,   168,   179,    86,    15,   168,   184,
      54,    55,    56,    57,   184,   181,    60,    15,   179,    11,
      11,   179,   179,   179,   179,    11,    70,    71,    15,   179,
      15,   179,    76,    29,   179,   166,    63,   186,    15,    15,
      15,   172,   173,   186,   179,    89,   179,   178,   179,   180,
     179,   179,   183,   179,   185,   186,   179,   181,   180,    15,
     179,    86,    86,   179,   179,    15,   110,   111,    15,    11,
      11,   179,   186,   179,   179,     8,     8,     8,   165,   179,
     168,    15,   181,   181,    79,    15,    15,   179,    15,    15,
     179,   181,   185,   179,     3,     4,     5,     6,     7,   180,
     179,   179,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   181,   179,   179,   186,   179,    15,
     179,   179,   166,   179,    33,    34,    15,   179,   172,   173,
      15,    40,    15,   179,   178,   179,   180,    46,   179,   183,
     179,   185,   179,   187,   179,    54,    55,    56,    57,   179,
     179,    60,    30,   179,  1296,   891,   744,   179,   181,   662,
     179,    70,    71,   293,   268,    -1,    -1,    76,    -1,    -1,
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
      -1,   180,   181,    -1,   183,    -1,   185,    -1,   187,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,
      34,    -1,   172,   173,    -1,    -1,    40,    -1,   178,    -1,
     180,   181,    46,   183,    -1,   185,    -1,   187,    -1,    -1,
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
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,    -1,
      40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,   184,
     185,    -1,   187,    -1,    54,    55,    56,    57,    -1,    -1,
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
     180,   181,    -1,   183,    -1,   185,    -1,   187,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,    34,
      -1,   172,   173,    -1,    -1,    40,    -1,   178,    -1,   180,
     181,    46,   183,    -1,   185,    -1,   187,    -1,    -1,    54,
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
      -1,    -1,    -1,   178,    -1,   180,    -1,    -1,   183,   184,
     185,    -1,   187,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    33,    34,    -1,   172,   173,    -1,    -1,
      40,    -1,   178,    -1,   180,   181,    46,   183,    -1,   185,
      -1,   187,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
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
     180,   181,    -1,   183,    -1,   185,    -1,   187,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,    34,
      -1,   172,   173,    -1,    -1,    40,    -1,   178,    -1,   180,
     181,    46,   183,    -1,   185,    -1,   187,    -1,    -1,    54,
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
      -1,    -1,    -1,   178,    -1,   180,   181,    -1,   183,    -1,
     185,    -1,   187,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    33,    34,    -1,   172,   173,    -1,    -1,
      40,    -1,   178,    -1,   180,   181,    46,   183,    -1,   185,
      -1,   187,    -1,    -1,    54,    55,    56,    57,    -1,    -1,
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
     180,    -1,    -1,   183,    -1,   185,    -1,   187,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,
      -1,   172,   173,    -1,    40,    -1,    -1,   178,    -1,   180,
      46,    -1,   183,    -1,   185,    -1,   187,    -1,    54,    55,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     166,    -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,
      -1,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,
      -1,   187,     3,     4,     5,     6,     7,    -1,    -1,    -1,
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
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,   186,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,
      -1,    84,    85,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,
     173,    -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,
     183,    -1,   185,   186,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      89,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   172,   173,    -1,    -1,    89,    -1,   178,
     179,   180,    -1,    -1,   183,    -1,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,
     172,   173,    -1,    40,    -1,    -1,   178,   179,   180,    46,
      -1,   183,    -1,   185,   186,    -1,    -1,    54,    55,    56,
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
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,   186,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,   179,   180,    -1,    -1,   183,    -1,   185,   186,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    -1,    -1,
      -1,    -1,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    76,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      84,    85,    -1,    -1,    -1,    89,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,   173,
      -1,    -1,    89,    -1,   178,   179,   180,    -1,    -1,   183,
      -1,   185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,
      -1,   178,    -1,   180,    46,    -1,   183,   184,   185,    -1,
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
     172,   173,    -1,    -1,    -1,    -1,   178,   179,   180,    -1,
      -1,   183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,   179,   180,    46,    -1,
     183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    89,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    26,    -1,    -1,
      -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,   179,   180,    -1,    -1,   183,    -1,   185,    -1,    -1,
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
      -1,    -1,    -1,    -1,   178,    -1,   180,   181,    -1,   183,
      -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
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
      -1,    -1,   172,   173,   174,    -1,    -1,    -1,   178,    -1,
     180,    -1,    -1,   183,    -1,   185,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    -1,    -1,    25,
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
      -1,    -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    33,    34,    -1,    -1,   172,   173,    -1,    40,    -1,
      -1,   178,    -1,   180,    46,   182,   183,    -1,   185,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    79,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    33,    34,    -1,   172,
     173,    -1,    -1,    40,    -1,   178,    -1,   180,   181,    46,
     183,    -1,   185,    -1,    -1,    -1,    -1,    54,    55,    56,
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
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      -1,    33,    34,    -1,   172,   173,    -1,    -1,    40,    -1,
     178,    -1,   180,   181,    46,   183,    -1,   185,    -1,    -1,
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
      -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,
     183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    89,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,
      89,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,    -1,   180,    -1,    -1,   183,    -1,   185,    -1,    -1,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      -1,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,   178,
      79,   180,    -1,    -1,   183,    -1,   185,    86,    87,    88,
      -1,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,    -1,    -1,    -1,
     109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   117,   118,
      -1,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,   143,   144,    -1,    -1,   147,   148,
     149,   150,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,   170,   171,    -1,    -1,   174,   175,   176,    -1,    -1,
     179
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
     241,   242,    11,    12,   184,   221,    15,   179,   180,   187,
     221,   240,    11,    12,    13,    15,    33,    38,   180,   180,
     221,   180,   221,   180,   221,   180,   221,   221,   230,    15,
     180,   219,   200,   219,   221,   179,   221,   179,   221,    11,
      15,    15,   221,   221,    13,    13,   180,   221,   180,   221,
     219,   180,   219,    28,    48,   179,    28,    48,   179,   185,
     220,   220,   185,   220,   221,   221,   219,   185,   219,   221,
     179,   221,    15,   208,   208,    15,    33,   234,   208,   208,
      11,    12,    13,    26,   219,   221,    11,   236,    11,    11,
     234,   234,   234,    15,    24,    26,    27,    35,    36,    40,
      46,    47,    54,    55,    56,    57,   237,   181,   221,   184,
     221,    79,   190,    28,    31,    32,    41,    44,    45,    48,
      49,   179,    86,   104,   105,   158,   159,   160,   103,    90,
      91,    92,    93,   149,   150,   151,   152,   153,   154,   155,
     157,   167,   168,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   109,   161,   136,   137,   138,   139,   140,   141,
     169,   170,   117,   143,   144,   147,   148,    79,    87,    88,
     134,   135,   171,   162,   163,   129,   130,   131,   132,   133,
     172,   173,   118,   121,   122,   123,   124,   125,   126,   127,
     128,   174,   175,   176,   164,   178,   165,   178,   221,    15,
     221,   221,   221,   221,   221,   221,   181,   240,   221,   180,
     221,   174,   221,   221,    15,   221,   221,   179,    15,    18,
      65,    15,   181,   187,   216,   240,   183,   167,   185,   178,
     178,   112,   113,   114,   115,   116,   165,   165,   182,   184,
     179,   221,   181,   221,   240,    15,   166,   179,   182,   107,
     165,   179,   107,   165,   179,   107,   165,   179,    11,    12,
      13,    15,    11,    15,   236,   221,    28,    31,    32,    41,
      44,    45,    48,   179,   182,   221,    28,    48,   179,   182,
     221,   219,   221,   219,    89,   182,   219,    87,    88,   180,
     185,   206,    11,    12,    15,    72,    73,    74,   218,   179,
     182,    28,    48,   179,    28,    48,   179,   165,   165,    76,
     182,   185,   167,   185,   167,   185,   221,   219,   221,   219,
      31,    49,    25,   221,   221,   221,   221,   221,   221,   182,
     221,   221,   221,    59,    42,    43,   202,   179,   212,   185,
     180,    15,    69,   185,   185,   165,   165,   165,   167,   221,
     179,   179,   182,   219,   180,   181,   182,   182,   184,   186,
      12,    13,    14,    26,    27,    35,    36,    52,    53,   186,
     221,   239,   242,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   224,   222,   224,   224,   224,   225,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,     8,     9,    10,   208,   227,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   230,   230,   230,   230,   230,
     230,   230,   231,   231,   231,   232,   232,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   237,   221,    18,
     237,   179,   179,   180,   179,   179,   179,   179,   179,   179,
     181,   179,   221,   179,   182,   172,   173,   182,   184,   179,
     168,   186,   179,   180,    15,   180,    86,    15,   181,   182,
     181,   182,   221,    15,   221,   237,   237,   221,   221,   221,
     221,   221,   221,   221,   184,   240,   179,   179,   181,   167,
     180,    15,   187,   221,   221,   221,   221,   180,   221,   221,
       6,    15,   180,   217,   221,   107,   165,   179,   165,   179,
     165,   179,   180,   206,   165,   165,   181,   182,   221,   221,
     221,   221,   221,   221,   221,   240,   182,   221,   221,   240,
     181,    29,    30,   193,   181,   236,   240,   230,   230,   181,
     218,   190,    24,   165,    11,    77,    78,   181,   182,   240,
     221,   221,   221,   221,   221,   221,   179,   240,   203,    15,
     221,    15,   221,   181,    29,   181,   221,   221,    11,   179,
     179,   179,   179,   179,   186,   221,   219,   221,   219,    42,
      43,   186,    15,    24,   185,   213,   181,   218,   180,   180,
     215,   190,   221,   221,   221,    15,   179,    11,   181,   240,
     181,   240,   184,   240,   183,   185,   165,   221,   221,   221,
     186,   221,    28,    48,   186,    28,    48,   186,    28,    31,
      32,    41,    44,    45,    48,    49,   186,   178,   178,   179,
     179,   179,   179,   179,   179,   179,   179,   106,   165,   180,
     187,   179,    15,   165,   180,   187,   181,   240,   182,   240,
     221,   221,   240,   165,   119,   120,   165,   119,   120,   165,
     181,   240,   181,   216,   240,   221,   180,    15,   187,   187,
     216,   184,   168,   186,   184,   179,    15,   181,   221,    15,
     166,   179,   179,   179,   221,   179,   182,   179,    86,    86,
     217,   179,   182,   179,   221,   221,   180,   221,   221,   181,
     218,   221,   221,   165,   240,   179,   179,   179,   179,   179,
     179,   179,   179,   240,   179,   179,   179,   219,   192,   219,
     180,   221,   219,   219,    89,   219,    89,    89,    24,   204,
     206,   181,    26,    27,    36,   186,   221,    15,   221,    24,
     165,    11,    11,   219,    11,    12,    15,    72,    73,    74,
     179,   179,   179,   179,   179,   179,   179,    42,    43,   186,
     168,   186,   168,   186,   219,   219,   219,   179,   179,   165,
     221,   219,   186,   221,   219,    15,    15,   213,    63,    64,
      67,   186,   206,   181,   181,   218,   218,    81,    82,    83,
     186,   186,   179,   179,   179,   168,   181,   181,   184,   221,
     221,   221,    28,    32,    41,    44,    45,    48,   186,   186,
     186,   186,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   237,   237,   222,   221,   181,   240,
     240,   221,   181,   216,   240,   240,   179,   181,   240,   179,
     184,   184,   184,   221,   221,   221,   181,   181,   181,   221,
      86,    15,    15,   181,   168,   181,   167,   180,    15,   182,
     240,   221,   221,   181,     6,    15,   179,   179,   221,   179,
     182,   179,   206,   181,   179,   179,   221,   181,   181,    29,
     193,   221,   219,    11,    11,    11,    15,    24,   206,   204,
     206,   221,   221,   186,   221,    28,    31,    32,    41,    44,
      45,    48,    49,   186,    15,   221,    24,   165,    11,    77,
      78,   221,   219,   179,   179,    29,   221,   179,   219,   186,
     186,    65,   237,    11,    14,    15,    16,    17,    63,   206,
     206,   181,   181,    15,    15,    15,   165,   184,   186,   186,
     221,   221,   221,   221,   221,   221,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   165,   165,
     179,   181,   179,   181,   181,   179,   181,   179,   179,   179,
     181,   221,   180,   180,    15,   181,   221,   240,   179,   179,
      86,    86,   182,   240,   206,   179,   182,   179,   179,   192,
     219,   181,    29,   193,   219,   219,   219,    15,   206,    28,
      32,    48,   186,   186,   186,   221,   221,   221,   221,   221,
     221,   221,   221,    15,   221,    24,   165,    11,    11,   219,
     186,   219,   179,   199,   221,   180,   185,   207,   180,   207,
      15,    24,    75,   165,   179,    15,    24,    75,   165,   179,
      11,    14,    16,    17,   179,   179,    65,   237,   206,   206,
       8,     8,     8,   221,   165,   165,   186,   186,   186,   186,
     186,   186,   221,   221,   179,   221,   221,   168,   181,   181,
     221,   221,   240,   179,   240,   219,   219,   221,   221,   221,
     186,   186,   186,   186,   186,   186,   186,   186,    15,   221,
     221,   181,   181,   218,    79,   190,   181,   218,    15,    15,
     179,   221,    15,    15,   179,   221,    15,    24,    75,   165,
     179,    15,    24,    75,   165,   179,   179,   179,   180,   180,
     207,   179,   221,   221,   186,   186,   181,   181,   179,   181,
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
     191,   191,   191,   191,   191,   191,   192,   192,   192,   192,
     192,   192,   192,   192,   193,   193,   193,   193,   193,   193,
     194,   194,   195,   195,   195,   195,   196,   196,   197,   197,
     197,   198,   198,   198,   199,   200,   200,   200,   200,   200,
     200,   201,   201,   202,   202,   203,   203,   204,   204,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   205,
     205,   205,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   208,   209,
     210,   211,   212,   212,   212,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   214,   215,   215,   215,   215,   216,   216,
     216,   216,   216,   217,   217,   217,   217,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   222,   222,   223,   223,
     223,   223,   223,   224,   224,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   226,   226,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   228,   228,   229,   229,   229,   229,
     229,   229,   229,   229,   230,   230,   230,   230,   231,   231,
     231,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   234,   234,   234,   234,   234,   234,   235,   235,
     236,   236,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   238,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   241,   241,   241,   241,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242
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
       5,     2,     1,     2,     3,     3,     5,     5,     5,     8,
       4,     1,     1,     1,     1,     2,     2,     4,     4,     4,
       4,     1,     1,     1,     1,     1,     5,     7,     7,     3,
       5,     5,     4,     6,     3,     5,     5,     7,     4,     6,
       5,     3,     5,     7,     3,     5,     5,     3,     2,     5,
       5,     2,    12,     9,     1,     7,     7,     5,     7,     5,
       3,     5,     7,     3,     4,     0,     4,     2,     3,     6,
       5,     3,     7,     6,     7,     6,     4,     7,     6,     7,
       6,     5,     3,     4,     5,     4,     5,     5,     6,     6,
       6,     6,     6,     6,     6,     6,     7,     7,     7,     3,
       3,     6,     4,     5,     4,     5,     5,     6,     6,     6,
       6,     6,     6,     6,     6,     7,     7,     7,     1,     6,
       5,     5,     0,     3,     3,     0,     4,     4,     4,     5,
       5,     4,     5,     5,     6,     6,     7,     7,     6,     8,
       8,     6,     6,     6,     5,     5,     6,     6,     7,     7,
       7,     7,     7,     6,     4,     7,     6,     4,     8,     7,
       5,     8,     7,     5,     0,     4,     4,     4,     3,     5,
       2,     5,     7,     3,     3,     5,     5,     1,     3,     5,
       4,     6,     1,     3,     2,     3,     3,     3,     4,     5,
       5,     3,     5,     4,     6,     1,     3,     1,     3,     1,
       3,     3,     3,     4,     6,     6,     6,     6,     6,     6,
       6,     6,     5,     7,     7,     7,     7,     7,     7,     5,
       8,     8,     6,     9,     9,     5,     5,     4,     4,     4,
       6,     6,     6,     6,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     1,     5,     1,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     2,     2,     2,     2,     2,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,     3,     4,
       6,     4,     3,     6,     6,     5,     3,     3,     6,     5,
       4,     4,     6,     6,     5,     3,     6,     5,     3,     5,
       5,     5,     4,     2,     2,     3,     4,     3,     2,     3,
       2,     1,     1,     3,     2,     2,     3,     5,     4,     5,
       4,     5,     7,     6,     7,     2,     3,     4,     5,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     4,     6,     6,
       6,     1,     4,     4,     5,     5,     5,     5,     5,     5,
       1,     1,     1,     1,     2,     3,     4,     5,     2,     3,
       4,     5,     1,     1,     2,     5,     3
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined RAKU_YYLTYPE_IS_TRIVIAL && RAKU_YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined RAKU_YYLTYPE_IS_TRIVIAL && RAKU_YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = RAKU_YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 749 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 4253 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 759 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 4259 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 760 "raku.y"
                     { if ((yyvsp[0].node) && (yyvsp[0].node)->line == 0) (yyvsp[0].node)->line = (yylsp[0]).first_line; (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 4265 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 764 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 4271 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 766 "raku.y"
        { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), nul); }
#line 4277 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 768 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4283 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 770 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4289 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 772 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4295 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 774 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4302 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 777 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4308 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 779 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4314 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 781 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4320 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 783 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4326 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 785 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4334 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 789 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4340 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 791 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4346 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 793 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4352 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 795 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4358 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 797 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4366 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 801 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4374 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 805 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4380 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 807 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4388 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 811 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4396 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 815 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4402 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 817 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4408 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 819 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4414 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 821 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4420 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 823 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4426 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 825 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4434 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 829 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4442 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 833 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4448 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 835 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4454 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 837 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4460 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4466 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 841 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4472 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 843 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4478 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 845 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4484 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 847 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4490 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 849 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4496 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 851 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4502 "raku.tab.c"
    break;

  case 42: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 853 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4514 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 861 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4520 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 863 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4526 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 865 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4532 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 867 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4538 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 869 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4544 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 871 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4550 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 873 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4557 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 876 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4564 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 879 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4570 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 881 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4577 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 884 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4584 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 887 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4590 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 889 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4598 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 893 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4604 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 895 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4610 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 897 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4616 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 899 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4622 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 901 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4629 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 904 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4636 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 907 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4642 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 909 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4649 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 912 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4656 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 915 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4663 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 918 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4669 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 920 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4678 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 925 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4686 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 929 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4694 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 933 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4701 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 936 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4708 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 939 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4716 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 943 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4723 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 946 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4730 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 949 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4737 "raku.tab.c"
    break;

  case 76: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 952 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4744 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 955 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4751 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_IF expr ';'  */
#line 958 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4757 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNLESS expr ';'  */
#line 960 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4763 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WHILE expr ';'  */
#line 962 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4769 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_UNTIL expr ';'  */
#line 964 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4775 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_FOR expr ';'  */
#line 966 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4782 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WITH expr ';'  */
#line 969 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4788 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITHOUT expr ';'  */
#line 971 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4794 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_GIVEN expr ';'  */
#line 973 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4800 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 975 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4807 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 978 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4814 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 981 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4822 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 985 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4829 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 988 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4835 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 990 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4841 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 992 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4847 "raku.tab.c"
    break;

  case 93: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 994 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4854 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 997 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4861 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 1000 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4868 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 1003 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4875 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 1006 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4882 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 1009 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4889 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1012 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4896 "raku.tab.c"
    break;

  case 100: /* stmt: expr ';'  */
#line 1014 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4902 "raku.tab.c"
    break;

  case 101: /* stmt: ';'  */
#line 1015 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4908 "raku.tab.c"
    break;

  case 102: /* stmt: if_stmt  */
#line 1016 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4914 "raku.tab.c"
    break;

  case 103: /* stmt: while_stmt  */
#line 1017 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4920 "raku.tab.c"
    break;

  case 104: /* stmt: for_stmt  */
#line 1018 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4926 "raku.tab.c"
    break;

  case 105: /* stmt: given_stmt  */
#line 1019 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4932 "raku.tab.c"
    break;

  case 106: /* stmt: KW_TRY block  */
#line 1021 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4938 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block KW_CATCH block  */
#line 1023 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4944 "raku.tab.c"
    break;

  case 108: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1025 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4952 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1029 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4961 "raku.tab.c"
    break;

  case 110: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1034 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4969 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH block  */
#line 1038 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4975 "raku.tab.c"
    break;

  case 112: /* stmt: block  */
#line 1040 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4981 "raku.tab.c"
    break;

  case 113: /* stmt: PHASER block  */
#line 1042 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 4987 "raku.tab.c"
    break;

  case 114: /* stmt: PHASER block ';'  */
#line 1044 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 4993 "raku.tab.c"
    break;

  case 115: /* stmt: PHASER expr ';'  */
#line 1046 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),seq1((yyvsp[-1].node))); ct_drop((yyvsp[-2].sval)); }
#line 4999 "raku.tab.c"
    break;

  case 116: /* stmt: PHASER VAR_SCALAR '=' expr ';'  */
#line 1048 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5005 "raku.tab.c"
    break;

  case 117: /* stmt: PHASER VAR_ARRAY '=' expr ';'  */
#line 1050 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_arr_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5011 "raku.tab.c"
    break;

  case 118: /* stmt: PHASER VAR_HASH '=' expr ';'  */
#line 1052 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)))); ct_drop((yyvsp[-4].sval)); }
#line 5017 "raku.tab.c"
    break;

  case 119: /* stmt: PHASER VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 1054 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node));
          (yyval.node)=rk_phaser_mark((yyvsp[-7].sval),seq1(c)); ct_drop((yyvsp[-7].sval)); }
#line 5024 "raku.tab.c"
    break;

  case 120: /* stmt: PHASER KW_SAY expr ';'  */
#line 1057 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); (yyval.node)=rk_phaser_mark((yyvsp[-3].sval),seq1(s)); ct_drop((yyvsp[-3].sval)); }
#line 5030 "raku.tab.c"
    break;

  case 121: /* stmt: unless_stmt  */
#line 1058 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5036 "raku.tab.c"
    break;

  case 122: /* stmt: until_stmt  */
#line 1059 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5042 "raku.tab.c"
    break;

  case 123: /* stmt: repeat_stmt  */
#line 1060 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5048 "raku.tab.c"
    break;

  case 124: /* stmt: loop_stmt  */
#line 1061 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5054 "raku.tab.c"
    break;

  case 125: /* stmt: KW_LAST ';'  */
#line 1062 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 5060 "raku.tab.c"
    break;

  case 126: /* stmt: KW_NEXT ';'  */
#line 1063 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 5066 "raku.tab.c"
    break;

  case 127: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1065 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5072 "raku.tab.c"
    break;

  case 128: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1067 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5078 "raku.tab.c"
    break;

  case 129: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1069 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5084 "raku.tab.c"
    break;

  case 130: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1071 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5090 "raku.tab.c"
    break;

  case 131: /* stmt: sub_decl  */
#line 1072 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5096 "raku.tab.c"
    break;

  case 132: /* stmt: class_decl  */
#line 1073 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5102 "raku.tab.c"
    break;

  case 133: /* stmt: role_decl  */
#line 1074 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5108 "raku.tab.c"
    break;

  case 134: /* stmt: grammar_decl  */
#line 1075 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5114 "raku.tab.c"
    break;

  case 135: /* stmt: module_decl  */
#line 1076 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5120 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5126 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5132 "raku.tab.c"
    break;

  case 138: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1084 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5138 "raku.tab.c"
    break;

  case 139: /* if_stmt: KW_IF expr block  */
#line 1086 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5144 "raku.tab.c"
    break;

  case 140: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1088 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5150 "raku.tab.c"
    break;

  case 141: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1090 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5156 "raku.tab.c"
    break;

  case 142: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1092 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5162 "raku.tab.c"
    break;

  case 143: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1094 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5168 "raku.tab.c"
    break;

  case 144: /* elsif_tail: KW_ELSIF expr block  */
#line 1098 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5174 "raku.tab.c"
    break;

  case 145: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1100 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5180 "raku.tab.c"
    break;

  case 146: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1102 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5186 "raku.tab.c"
    break;

  case 147: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1104 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5192 "raku.tab.c"
    break;

  case 148: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1106 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5198 "raku.tab.c"
    break;

  case 149: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1108 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5204 "raku.tab.c"
    break;

  case 150: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1112 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5210 "raku.tab.c"
    break;

  case 151: /* while_stmt: KW_WHILE expr block  */
#line 1114 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5216 "raku.tab.c"
    break;

  case 152: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1118 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5222 "raku.tab.c"
    break;

  case 153: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1120 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5228 "raku.tab.c"
    break;

  case 154: /* unless_stmt: KW_UNLESS expr block  */
#line 1122 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5234 "raku.tab.c"
    break;

  case 155: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1124 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5240 "raku.tab.c"
    break;

  case 156: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1128 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5246 "raku.tab.c"
    break;

  case 157: /* until_stmt: KW_UNTIL expr block  */
#line 1130 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5252 "raku.tab.c"
    break;

  case 158: /* repeat_stmt: KW_REPEAT block  */
#line 1134 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5258 "raku.tab.c"
    break;

  case 159: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1136 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5264 "raku.tab.c"
    break;

  case 160: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1138 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5270 "raku.tab.c"
    break;

  case 161: /* loop_stmt: KW_LOOP block  */
#line 1142 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5276 "raku.tab.c"
    break;

  case 162: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1144 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5282 "raku.tab.c"
    break;

  case 163: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1146 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5288 "raku.tab.c"
    break;

  case 164: /* loop_incr: expr  */
#line 1149 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 5294 "raku.tab.c"
    break;

  case 165: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1153 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5304 "raku.tab.c"
    break;

  case 166: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1159 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5314 "raku.tab.c"
    break;

  case 167: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1165 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5326 "raku.tab.c"
    break;

  case 168: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1173 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5336 "raku.tab.c"
    break;

  case 169: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1179 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5345 "raku.tab.c"
    break;

  case 170: /* for_stmt: KW_FOR expr block  */
#line 1184 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5352 "raku.tab.c"
    break;

  case 171: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1189 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 5364 "raku.tab.c"
    break;

  case 172: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1197 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 5377 "raku.tab.c"
    break;

  case 173: /* catch_when_list: KW_WHEN expr block  */
#line 1208 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5383 "raku.tab.c"
    break;

  case 174: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1210 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5389 "raku.tab.c"
    break;

  case 175: /* when_list: %empty  */
#line 1213 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5395 "raku.tab.c"
    break;

  case 176: /* when_list: when_list KW_WHEN expr block  */
#line 1215 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5403 "raku.tab.c"
    break;

  case 177: /* sub_trait_list: TESTOP IDENT  */
#line 1220 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5409 "raku.tab.c"
    break;

  case 178: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1221 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5415 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1225 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5427 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1233 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5437 "raku.tab.c"
    break;

  case 181: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1239 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5447 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1245 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5459 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1253 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5469 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1259 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5481 "raku.tab.c"
    break;

  case 185: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1267 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5491 "raku.tab.c"
    break;

  case 186: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1273 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5501 "raku.tab.c"
    break;

  case 187: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1279 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5514 "raku.tab.c"
    break;

  case 188: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1288 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5525 "raku.tab.c"
    break;

  case 189: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1295 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5538 "raku.tab.c"
    break;

  case 190: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1304 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5551 "raku.tab.c"
    break;

  case 191: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1313 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5562 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list '}'  */
#line 1321 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5568 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr '}'  */
#line 1323 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5575 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1326 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5582 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1329 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5588 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1331 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5594 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1333 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5600 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1335 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5606 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1337 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5612 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1339 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5618 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1341 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5624 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1343 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5631 "raku.tab.c"
    break;

  case 203: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1346 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5637 "raku.tab.c"
    break;

  case 204: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1348 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5643 "raku.tab.c"
    break;

  case 205: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1350 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5649 "raku.tab.c"
    break;

  case 206: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1352 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5656 "raku.tab.c"
    break;

  case 207: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1355 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5663 "raku.tab.c"
    break;

  case 208: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1358 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5671 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list '}'  */
#line 1363 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5677 "raku.tab.c"
    break;

  case 210: /* method_body: '{' YADA '}'  */
#line 1364 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5683 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1366 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5690 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr '}'  */
#line 1369 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5697 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1372 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5704 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1375 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5710 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1377 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5716 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1379 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5722 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1381 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5728 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1383 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5734 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1385 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5740 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1387 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5746 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1389 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5753 "raku.tab.c"
    break;

  case 222: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1392 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5759 "raku.tab.c"
    break;

  case 223: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1394 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5765 "raku.tab.c"
    break;

  case 224: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5771 "raku.tab.c"
    break;

  case 225: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1398 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5778 "raku.tab.c"
    break;

  case 226: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1401 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5785 "raku.tab.c"
    break;

  case 227: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1404 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5793 "raku.tab.c"
    break;

  case 228: /* pkg_name: IDENT  */
#line 1409 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5799 "raku.tab.c"
    break;

  case 229: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1413 "raku.y"
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
#line 5817 "raku.tab.c"
    break;

  case 230: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1429 "raku.y"
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
#line 5834 "raku.tab.c"
    break;

  case 231: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1444 "raku.y"
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
#line 5851 "raku.tab.c"
    break;

  case 232: /* is_clauses: %empty  */
#line 1458 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5857 "raku.tab.c"
    break;

  case 233: /* is_clauses: is_clauses IDENT IDENT  */
#line 1460 "raku.y"
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
#line 5873 "raku.tab.c"
    break;

  case 234: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1472 "raku.y"
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
#line 5889 "raku.tab.c"
    break;

  case 235: /* class_body_list: %empty  */
#line 1485 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5895 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1487 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5902 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1490 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5909 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1493 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5916 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1496 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5923 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1499 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5930 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1502 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5937 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1505 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5944 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1508 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5951 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1511 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5958 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1514 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5965 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1517 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5972 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1520 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5979 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1523 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5990 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1530 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6000 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1536 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6010 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1542 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6021 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1549 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6032 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1556 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6043 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1563 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6051 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1567 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6059 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1571 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6067 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1575 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6075 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1579 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6086 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1586 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6097 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1593 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6108 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1600 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6119 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1607 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6133 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1617 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6145 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1625 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6157 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1633 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6170 "raku.tab.c"
    break;

  case 266: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1642 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6181 "raku.tab.c"
    break;

  case 267: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1649 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6192 "raku.tab.c"
    break;

  case 268: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1656 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6206 "raku.tab.c"
    break;

  case 269: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1666 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6218 "raku.tab.c"
    break;

  case 270: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1674 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 6230 "raku.tab.c"
    break;

  case 271: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1682 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6243 "raku.tab.c"
    break;

  case 272: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1691 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6254 "raku.tab.c"
    break;

  case 273: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1700 "raku.y"
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
#line 6271 "raku.tab.c"
    break;

  case 274: /* grammar_body_list: %empty  */
#line 1714 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 6277 "raku.tab.c"
    break;

  case 275: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1716 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6286 "raku.tab.c"
    break;

  case 276: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1721 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6295 "raku.tab.c"
    break;

  case 277: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1726 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6304 "raku.tab.c"
    break;

  case 278: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1733 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 6312 "raku.tab.c"
    break;

  case 279: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1737 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 6320 "raku.tab.c"
    break;

  case 280: /* named_arg_list: ':' IDENT  */
#line 1741 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6331 "raku.tab.c"
    break;

  case 281: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1748 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6339 "raku.tab.c"
    break;

  case 282: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1752 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6347 "raku.tab.c"
    break;

  case 283: /* pair_list: IDENT OP_FATARROW expr  */
#line 1758 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6353 "raku.tab.c"
    break;

  case 284: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1760 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6359 "raku.tab.c"
    break;

  case 285: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1762 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6365 "raku.tab.c"
    break;

  case 286: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1764 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6371 "raku.tab.c"
    break;

  case 287: /* param_list: VAR_SCALAR  */
#line 1767 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6377 "raku.tab.c"
    break;

  case 288: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1768 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6383 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1769 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6389 "raku.tab.c"
    break;

  case 290: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1770 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6395 "raku.tab.c"
    break;

  case 291: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1771 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6401 "raku.tab.c"
    break;

  case 292: /* param_list: VAR_ARRAY  */
#line 1772 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6407 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' VAR_ARRAY  */
#line 1773 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6413 "raku.tab.c"
    break;

  case 294: /* param_list: IDENT VAR_SCALAR  */
#line 1774 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6419 "raku.tab.c"
    break;

  case 295: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1775 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6425 "raku.tab.c"
    break;

  case 296: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1776 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6431 "raku.tab.c"
    break;

  case 297: /* param_list: param_list ',' VAR_SCALAR  */
#line 1777 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6437 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1778 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6443 "raku.tab.c"
    break;

  case 299: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1779 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6449 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1780 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6455 "raku.tab.c"
    break;

  case 301: /* param_list: VAR_SCALAR '=' expr  */
#line 1781 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6461 "raku.tab.c"
    break;

  case 302: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1782 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6467 "raku.tab.c"
    break;

  case 303: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1783 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6473 "raku.tab.c"
    break;

  case 304: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1784 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6479 "raku.tab.c"
    break;

  case 305: /* param_list: SLURPY_POS  */
#line 1785 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6485 "raku.tab.c"
    break;

  case 306: /* param_list: param_list ',' SLURPY_POS  */
#line 1786 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6491 "raku.tab.c"
    break;

  case 307: /* param_list: SLURPY_LOL  */
#line 1787 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6497 "raku.tab.c"
    break;

  case 308: /* param_list: param_list ',' SLURPY_LOL  */
#line 1788 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6503 "raku.tab.c"
    break;

  case 309: /* param_list: SLURPY_NAMED  */
#line 1789 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6509 "raku.tab.c"
    break;

  case 310: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1790 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6515 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list '}'  */
#line 1793 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6521 "raku.tab.c"
    break;

  case 312: /* block: '{' YADA '}'  */
#line 1794 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6527 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list expr '}'  */
#line 1796 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6533 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1798 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6539 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1800 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6545 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1802 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6551 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1804 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6557 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1806 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6564 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1809 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6570 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1811 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6576 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1813 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6582 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1815 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 6588 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1817 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6595 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1820 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6602 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1823 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6610 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1827 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6617 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1830 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6624 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1833 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6631 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1836 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 6637 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1838 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6644 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1841 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6651 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1844 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,a); (yyval.node)=make_seq(l); }
#line 6658 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1847 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6665 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1850 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6672 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1853 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,t); (yyval.node)=make_seq(l); }
#line 6678 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1855 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 6684 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1857 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 6690 "raku.tab.c"
    break;

  case 338: /* block: '{' stmt_list KW_LAST '}'  */
#line 1859 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6696 "raku.tab.c"
    break;

  case 339: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1861 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6702 "raku.tab.c"
    break;

  case 340: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1863 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6708 "raku.tab.c"
    break;

  case 341: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1865 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6714 "raku.tab.c"
    break;

  case 342: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1867 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6720 "raku.tab.c"
    break;

  case 343: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1869 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6726 "raku.tab.c"
    break;

  case 344: /* closure: '{' expr '}'  */
#line 1872 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6732 "raku.tab.c"
    break;

  case 345: /* expr: VAR_SCALAR '=' expr  */
#line 1875 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6738 "raku.tab.c"
    break;

  case 346: /* expr: VAR_ARRAY '=' expr  */
#line 1876 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6744 "raku.tab.c"
    break;

  case 347: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1877 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6750 "raku.tab.c"
    break;

  case 348: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1878 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6756 "raku.tab.c"
    break;

  case 349: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1879 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6762 "raku.tab.c"
    break;

  case 350: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1880 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6768 "raku.tab.c"
    break;

  case 351: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1881 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6774 "raku.tab.c"
    break;

  case 352: /* expr: KW_GATHER block  */
#line 1882 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6784 "raku.tab.c"
    break;

  case 353: /* expr: KW_GATHER for_stmt  */
#line 1887 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6794 "raku.tab.c"
    break;

  case 354: /* expr: tern_expr OP_FATARROW expr  */
#line 1893 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6800 "raku.tab.c"
    break;

  case 355: /* expr: tern_expr  */
#line 1894 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6806 "raku.tab.c"
    break;

  case 356: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1898 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6812 "raku.tab.c"
    break;

  case 357: /* tern_expr: or_expr  */
#line 1899 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6818 "raku.tab.c"
    break;

  case 358: /* or_expr: or_expr OP_OR and_expr  */
#line 1902 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6824 "raku.tab.c"
    break;

  case 359: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1903 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6830 "raku.tab.c"
    break;

  case 360: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1904 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6836 "raku.tab.c"
    break;

  case 361: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1905 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6842 "raku.tab.c"
    break;

  case 362: /* or_expr: and_expr  */
#line 1906 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6848 "raku.tab.c"
    break;

  case 363: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1909 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6854 "raku.tab.c"
    break;

  case 364: /* and_expr: cmp_expr  */
#line 1910 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6860 "raku.tab.c"
    break;

  case 365: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1913 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_EQ,rk_numeric_ctx((yyvsp[0].node))); }
#line 6866 "raku.tab.c"
    break;

  case 366: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1914 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6872 "raku.tab.c"
    break;

  case 367: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1915 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6878 "raku.tab.c"
    break;

  case 368: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1916 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6884 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1917 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6890 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1918 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6896 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1919 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6902 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1920 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6908 "raku.tab.c"
    break;

  case 373: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1921 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6914 "raku.tab.c"
    break;

  case 374: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1922 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_NE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6920 "raku.tab.c"
    break;

  case 375: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1923 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6926 "raku.tab.c"
    break;

  case 376: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1924 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6932 "raku.tab.c"
    break;

  case 377: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1925 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6938 "raku.tab.c"
    break;

  case 378: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1926 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6944 "raku.tab.c"
    break;

  case 379: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1927 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6950 "raku.tab.c"
    break;

  case 380: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1928 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6956 "raku.tab.c"
    break;

  case 381: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1929 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6962 "raku.tab.c"
    break;

  case 382: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1930 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6968 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1931 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6974 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1932 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6980 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1933 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6986 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1934 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6992 "raku.tab.c"
    break;

  case 387: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1935 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6998 "raku.tab.c"
    break;

  case 388: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1937 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 7008 "raku.tab.c"
    break;

  case 389: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1943 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 7018 "raku.tab.c"
    break;

  case 390: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1949 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 7028 "raku.tab.c"
    break;

  case 391: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1955 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 7038 "raku.tab.c"
    break;

  case 392: /* cmp_expr: divis_expr  */
#line 1960 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 7044 "raku.tab.c"
    break;

  case 393: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1963 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7050 "raku.tab.c"
    break;

  case 394: /* divis_expr: jct_expr  */
#line 1964 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7056 "raku.tab.c"
    break;

  case 395: /* jct_expr: jct_expr '|' range_expr  */
#line 1967 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7062 "raku.tab.c"
    break;

  case 396: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1968 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7068 "raku.tab.c"
    break;

  case 397: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1969 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7074 "raku.tab.c"
    break;

  case 398: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1970 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7080 "raku.tab.c"
    break;

  case 399: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1971 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7086 "raku.tab.c"
    break;

  case 400: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1972 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7092 "raku.tab.c"
    break;

  case 401: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1973 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7098 "raku.tab.c"
    break;

  case 402: /* jct_expr: jct_expr '&' range_expr  */
#line 1974 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7104 "raku.tab.c"
    break;

  case 403: /* jct_expr: dor_expr  */
#line 1975 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7110 "raku.tab.c"
    break;

  case 404: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1979 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7116 "raku.tab.c"
    break;

  case 405: /* dor_expr: range_expr  */
#line 1980 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7122 "raku.tab.c"
    break;

  case 406: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1983 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7128 "raku.tab.c"
    break;

  case 407: /* range_expr: add_expr YADA add_expr  */
#line 1984 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7134 "raku.tab.c"
    break;

  case 408: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1985 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7140 "raku.tab.c"
    break;

  case 409: /* range_expr: range_expr OP_COLL add_expr  */
#line 1986 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7146 "raku.tab.c"
    break;

  case 410: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1987 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7152 "raku.tab.c"
    break;

  case 411: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1988 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7158 "raku.tab.c"
    break;

  case 412: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1989 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 7164 "raku.tab.c"
    break;

  case 413: /* range_expr: add_expr  */
#line 1990 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 7170 "raku.tab.c"
    break;

  case 414: /* add_expr: add_expr '~' repl_expr  */
#line 1993 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7176 "raku.tab.c"
    break;

  case 415: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 1994 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7182 "raku.tab.c"
    break;

  case 416: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 1995 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7188 "raku.tab.c"
    break;

  case 417: /* add_expr: repl_expr  */
#line 1996 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 7194 "raku.tab.c"
    break;

  case 418: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1999 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7200 "raku.tab.c"
    break;

  case 419: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 2000 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 7206 "raku.tab.c"
    break;

  case 420: /* repl_expr: addsub_expr  */
#line 2001 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7212 "raku.tab.c"
    break;

  case 421: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 2004 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7218 "raku.tab.c"
    break;

  case 422: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 2005 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7224 "raku.tab.c"
    break;

  case 423: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 2006 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7230 "raku.tab.c"
    break;

  case 424: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 2007 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7236 "raku.tab.c"
    break;

  case 425: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 2008 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7242 "raku.tab.c"
    break;

  case 426: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 2009 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7248 "raku.tab.c"
    break;

  case 427: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 2010 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7254 "raku.tab.c"
    break;

  case 428: /* addsub_expr: mul_expr  */
#line 2011 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 7260 "raku.tab.c"
    break;

  case 429: /* mul_expr: mul_expr '*' unary_expr  */
#line 2014 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7266 "raku.tab.c"
    break;

  case 430: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 2015 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7272 "raku.tab.c"
    break;

  case 431: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 2016 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7278 "raku.tab.c"
    break;

  case 432: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 2017 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7284 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 2018 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7290 "raku.tab.c"
    break;

  case 434: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 2019 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7296 "raku.tab.c"
    break;

  case 435: /* mul_expr: mul_expr '/' unary_expr  */
#line 2020 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7302 "raku.tab.c"
    break;

  case 436: /* mul_expr: mul_expr '%' unary_expr  */
#line 2021 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7308 "raku.tab.c"
    break;

  case 437: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 2023 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,rk_numeric_ctx((yyvsp[-2].node))); expr_add_child(c,rk_numeric_ctx((yyvsp[0].node))); (yyval.node)=c; }
#line 7314 "raku.tab.c"
    break;

  case 438: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2025 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7320 "raku.tab.c"
    break;

  case 439: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2027 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7326 "raku.tab.c"
    break;

  case 440: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2029 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7332 "raku.tab.c"
    break;

  case 441: /* mul_expr: unary_expr  */
#line 2030 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7338 "raku.tab.c"
    break;

  case 442: /* unary_expr: '-' unary_expr  */
#line 2033 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,rk_numeric_ctx((yyvsp[0].node))); }
#line 7344 "raku.tab.c"
    break;

  case 443: /* unary_expr: '+' unary_expr  */
#line 2034 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 7350 "raku.tab.c"
    break;

  case 444: /* unary_expr: '!' unary_expr  */
#line 2035 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7356 "raku.tab.c"
    break;

  case 445: /* unary_expr: CARET unary_expr  */
#line 2036 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7362 "raku.tab.c"
    break;

  case 446: /* unary_expr: OP_REDUCE unary_expr  */
#line 2038 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7371 "raku.tab.c"
    break;

  case 447: /* unary_expr: pow_expr  */
#line 2042 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7377 "raku.tab.c"
    break;

  case 448: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2045 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7383 "raku.tab.c"
    break;

  case 449: /* pow_expr: postfix_expr  */
#line 2046 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7389 "raku.tab.c"
    break;

  case 450: /* scalar_list: VAR_SCALAR  */
#line 2049 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7395 "raku.tab.c"
    break;

  case 451: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2050 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7401 "raku.tab.c"
    break;

  case 452: /* meth_name: IDENT  */
#line 2053 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7407 "raku.tab.c"
    break;

  case 453: /* meth_name: KW_SORT  */
#line 2054 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7413 "raku.tab.c"
    break;

  case 454: /* meth_name: KW_REVERSE  */
#line 2055 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7419 "raku.tab.c"
    break;

  case 455: /* meth_name: KW_MAP  */
#line 2056 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7425 "raku.tab.c"
    break;

  case 456: /* meth_name: KW_GREP  */
#line 2057 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7431 "raku.tab.c"
    break;

  case 457: /* meth_name: KW_SAY  */
#line 2058 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7437 "raku.tab.c"
    break;

  case 458: /* meth_name: KW_PRINT  */
#line 2059 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7443 "raku.tab.c"
    break;

  case 459: /* meth_name: KW_TAKE  */
#line 2060 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7449 "raku.tab.c"
    break;

  case 460: /* meth_name: KW_RETURN  */
#line 2061 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7455 "raku.tab.c"
    break;

  case 461: /* meth_name: KW_EXISTS  */
#line 2062 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7461 "raku.tab.c"
    break;

  case 462: /* meth_name: KW_DELETE  */
#line 2063 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7467 "raku.tab.c"
    break;

  case 463: /* meth_name: KW_JOIN  */
#line 2064 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7473 "raku.tab.c"
    break;

  case 464: /* meth_name: TESTOP  */
#line 2065 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7479 "raku.tab.c"
    break;

  case 465: /* postfix_expr: call_expr  */
#line 2067 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7485 "raku.tab.c"
    break;

  case 466: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2070 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7495 "raku.tab.c"
    break;

  case 467: /* call_expr: IDENT '(' arg_list ')'  */
#line 2076 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7504 "raku.tab.c"
    break;

  case 468: /* call_expr: IDENT '(' ')'  */
#line 2080 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7510 "raku.tab.c"
    break;

  case 469: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2082 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7516 "raku.tab.c"
    break;

  case 470: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2084 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7522 "raku.tab.c"
    break;

  case 471: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2086 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7531 "raku.tab.c"
    break;

  case 472: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2091 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7537 "raku.tab.c"
    break;

  case 473: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2093 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7550 "raku.tab.c"
    break;

  case 474: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2102 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7563 "raku.tab.c"
    break;

  case 475: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2111 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7574 "raku.tab.c"
    break;

  case 476: /* call_expr: IDENT '.' KW_NEW  */
#line 2118 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7582 "raku.tab.c"
    break;

  case 477: /* call_expr: IDENT '.' IDENT  */
#line 2122 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7591 "raku.tab.c"
    break;

  case 478: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2127 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7602 "raku.tab.c"
    break;

  case 479: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2134 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7611 "raku.tab.c"
    break;

  case 480: /* call_expr: IDENT '.' CARET IDENT  */
#line 2139 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7621 "raku.tab.c"
    break;

  case 481: /* call_expr: atom '.' CARET IDENT  */
#line 2145 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7631 "raku.tab.c"
    break;

  case 482: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2151 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7642 "raku.tab.c"
    break;

  case 483: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2158 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7653 "raku.tab.c"
    break;

  case 484: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2165 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7662 "raku.tab.c"
    break;

  case 485: /* call_expr: atom '.' meth_name  */
#line 2170 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7671 "raku.tab.c"
    break;

  case 486: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2175 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7682 "raku.tab.c"
    break;

  case 487: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2182 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7691 "raku.tab.c"
    break;

  case 488: /* call_expr: call_expr '.' meth_name  */
#line 2187 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7700 "raku.tab.c"
    break;

  case 489: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2192 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7711 "raku.tab.c"
    break;

  case 490: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2199 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7722 "raku.tab.c"
    break;

  case 491: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2206 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7733 "raku.tab.c"
    break;

  case 492: /* call_expr: '.' meth_name '(' ')'  */
#line 2213 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7742 "raku.tab.c"
    break;

  case 493: /* call_expr: '.' meth_name  */
#line 2218 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7751 "raku.tab.c"
    break;

  case 494: /* call_expr: KW_DIE expr  */
#line 2223 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7757 "raku.tab.c"
    break;

  case 495: /* call_expr: KW_MAP closure expr  */
#line 2225 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7763 "raku.tab.c"
    break;

  case 496: /* call_expr: KW_MAP closure ',' expr  */
#line 2227 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7769 "raku.tab.c"
    break;

  case 497: /* call_expr: KW_GREP closure expr  */
#line 2229 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7775 "raku.tab.c"
    break;

  case 498: /* call_expr: KW_SORT expr  */
#line 2231 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7781 "raku.tab.c"
    break;

  case 499: /* call_expr: KW_SORT closure expr  */
#line 2233 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7787 "raku.tab.c"
    break;

  case 500: /* call_expr: KW_REVERSE expr  */
#line 2235 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7793 "raku.tab.c"
    break;

  case 501: /* call_expr: atom  */
#line 2236 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7799 "raku.tab.c"
    break;

  case 502: /* arg_list: expr  */
#line 2239 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7805 "raku.tab.c"
    break;

  case 503: /* arg_list: arg_list ',' expr  */
#line 2240 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7811 "raku.tab.c"
    break;

  case 504: /* arg_list: arg_list ','  */
#line 2241 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7817 "raku.tab.c"
    break;

  case 505: /* arg_list: ':' IDENT  */
#line 2242 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7823 "raku.tab.c"
    break;

  case 506: /* arg_list: ':' '!' IDENT  */
#line 2243 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7829 "raku.tab.c"
    break;

  case 507: /* arg_list: ':' IDENT '(' expr ')'  */
#line 2244 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7835 "raku.tab.c"
    break;

  case 508: /* arg_list: ':' IDENT '(' ')'  */
#line 2245 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7841 "raku.tab.c"
    break;

  case 509: /* arg_list: ':' IDENT '<' IDENT '>'  */
#line 2246 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7847 "raku.tab.c"
    break;

  case 510: /* arg_list: arg_list ',' ':' IDENT  */
#line 2247 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7853 "raku.tab.c"
    break;

  case 511: /* arg_list: arg_list ',' ':' '!' IDENT  */
#line 2248 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7859 "raku.tab.c"
    break;

  case 512: /* arg_list: arg_list ',' ':' IDENT '(' expr ')'  */
#line 2249 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-6].list),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7865 "raku.tab.c"
    break;

  case 513: /* arg_list: arg_list ',' ':' IDENT '(' ')'  */
#line 2250 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-5].list),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7871 "raku.tab.c"
    break;

  case 514: /* arg_list: arg_list ',' ':' IDENT '<' IDENT '>'  */
#line 2251 "raku.y"
                                           { (yyval.list)=exprlist_append((yyvsp[-6].list),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7877 "raku.tab.c"
    break;

  case 515: /* paren_group: '(' ')'  */
#line 2254 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7883 "raku.tab.c"
    break;

  case 516: /* paren_group: '(' expr ')'  */
#line 2255 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7889 "raku.tab.c"
    break;

  case 517: /* paren_group: '(' expr ',' ')'  */
#line 2257 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7895 "raku.tab.c"
    break;

  case 518: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2259 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7902 "raku.tab.c"
    break;

  case 519: /* atom: LIT_INT  */
#line 2263 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7908 "raku.tab.c"
    break;

  case 520: /* atom: LIT_BOOL  */
#line 2265 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7916 "raku.tab.c"
    break;

  case 521: /* atom: LIT_FLOAT  */
#line 2268 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7922 "raku.tab.c"
    break;

  case 522: /* atom: LIT_STR  */
#line 2269 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7928 "raku.tab.c"
    break;

  case 523: /* atom: WORDLIST  */
#line 2271 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7941 "raku.tab.c"
    break;

  case 524: /* atom: LIT_INTERP_STR  */
#line 2279 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7947 "raku.tab.c"
    break;

  case 525: /* atom: VAR_SCALAR  */
#line 2280 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7953 "raku.tab.c"
    break;

  case 526: /* atom: OP_INC VAR_SCALAR  */
#line 2281 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7959 "raku.tab.c"
    break;

  case 527: /* atom: OP_DEC VAR_SCALAR  */
#line 2282 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7965 "raku.tab.c"
    break;

  case 528: /* atom: VAR_SCALAR OP_INC  */
#line 2283 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7971 "raku.tab.c"
    break;

  case 529: /* atom: VAR_SCALAR OP_DEC  */
#line 2284 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7977 "raku.tab.c"
    break;

  case 530: /* atom: VAR_TWIGIL OP_INC  */
#line 2285 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 7983 "raku.tab.c"
    break;

  case 531: /* atom: VAR_TWIGIL OP_DEC  */
#line 2286 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 7989 "raku.tab.c"
    break;

  case 532: /* atom: VAR_ARRAY  */
#line 2287 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7995 "raku.tab.c"
    break;

  case 533: /* atom: VAR_HASH  */
#line 2288 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8001 "raku.tab.c"
    break;

  case 534: /* atom: VAR_CAPTURE  */
#line 2290 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8009 "raku.tab.c"
    break;

  case 535: /* atom: VAR_FH  */
#line 2294 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8017 "raku.tab.c"
    break;

  case 536: /* atom: VAR_NAMED_CAPTURE  */
#line 2298 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 8024 "raku.tab.c"
    break;

  case 537: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2301 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8030 "raku.tab.c"
    break;

  case 538: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2303 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 8036 "raku.tab.c"
    break;

  case 539: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2305 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 8042 "raku.tab.c"
    break;

  case 540: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2307 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 8048 "raku.tab.c"
    break;

  case 541: /* atom: ARR_ALL_SLICE  */
#line 2309 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 8054 "raku.tab.c"
    break;

  case 542: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2311 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8060 "raku.tab.c"
    break;

  case 543: /* atom: VAR_HASH '{' expr '}'  */
#line 2313 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8066 "raku.tab.c"
    break;

  case 544: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2315 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8072 "raku.tab.c"
    break;

  case 545: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2317 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8078 "raku.tab.c"
    break;

  case 546: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2319 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8084 "raku.tab.c"
    break;

  case 547: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2321 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8090 "raku.tab.c"
    break;

  case 548: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2323 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8096 "raku.tab.c"
    break;

  case 549: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2325 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8102 "raku.tab.c"
    break;

  case 550: /* atom: IDENT  */
#line 2326 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8108 "raku.tab.c"
    break;

  case 551: /* atom: VAR_TWIGIL  */
#line 2328 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8116 "raku.tab.c"
    break;

  case 552: /* atom: VAR_ARRAY_TWIGIL  */
#line 2332 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8124 "raku.tab.c"
    break;

  case 553: /* atom: VAR_HASH_TWIGIL  */
#line 2336 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8132 "raku.tab.c"
    break;

  case 554: /* atom: '[' ']'  */
#line 2339 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 8138 "raku.tab.c"
    break;

  case 555: /* atom: '[' expr ']'  */
#line 2341 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8144 "raku.tab.c"
    break;

  case 556: /* atom: '[' expr ',' ']'  */
#line 2343 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8150 "raku.tab.c"
    break;

  case 557: /* atom: '[' expr ',' arg_list ']'  */
#line 2345 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8157 "raku.tab.c"
    break;

  case 558: /* atom: DOLLAR_LBRACKET ']'  */
#line 2347 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 8163 "raku.tab.c"
    break;

  case 559: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2349 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8169 "raku.tab.c"
    break;

  case 560: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2351 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8175 "raku.tab.c"
    break;

  case 561: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2353 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8182 "raku.tab.c"
    break;

  case 562: /* atom: paren_group  */
#line 2355 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 8188 "raku.tab.c"
    break;

  case 563: /* atom: block  */
#line 2356 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8194 "raku.tab.c"
    break;

  case 564: /* atom: KW_SUB block  */
#line 2357 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8200 "raku.tab.c"
    break;

  case 565: /* atom: KW_SUB '(' param_list ')' block  */
#line 2359 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 8207 "raku.tab.c"
    break;

  case 566: /* atom: OP_ARROW scalar_list block  */
#line 2362 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 8214 "raku.tab.c"
    break;


#line 8218 "raku.tab.c"

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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 2365 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
