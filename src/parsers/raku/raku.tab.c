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
    if (!strcmp(s, "is-approx")) return "__rk_test_is_approx";
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
static tree_t *rk_logical_and(tree_t *a, tree_t *b) { tree_t *s = expr_binary(TT_SEQ, a, b); s->v.ival = 1; return s; }
int rk_seq_is_logical_and(const tree_t *t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ExprList *rk_tail_value(ExprList *l) {
    if (!l || l->count <= 0) return l;
    { tree_t *last = l->items[l->count - 1];
      if (!last || (rk_tail_is_statement(last->t) && !rk_seq_is_logical_and(last))) return l;
      { tree_t *r = ast_node_new(TT_RETURN); r->line = last->line; expr_add_child(r, last); l->items[l->count - 1] = r; } }
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
static tree_t *rk_quiet_store(tree_t *target, tree_t *rhs) { tree_t *a = expr_binary(TT_ASSIGN, target, rhs); a->v.ival = 1; return a; }
static tree_t *rk_incdec(const char *var, int add) {
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    return rk_quiet_store(var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one));
}
static tree_t *rk_post_incdec(const char *var, int add) {
    static int __post_uid = 0; char tmp[32]; snprintf(tmp, sizeof tmp, "__post_%d", __post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), var_node(var)));
    tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1;
    expr_add_child(seq, rk_quiet_store(var_node(var), expr_binary(add ? TT_ADD : TT_SUB, var_node(var), one)));
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
    tree_t *bind = rk_quiet_store(leaf_sval(TT_VAR, tmp), rhs_arr); expr_add_child(seq, bind);
    int n = targets ? targets->count : 0;
    for (int i = 0; i < n; i++) {
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, tmp));
        tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = i; expr_add_child(get, idx);
        expr_add_child(seq, rk_quiet_store(targets->items[i], get));
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
    if (last) return rk_logical_and(left, expr_binary(op, rk_tree_clone(last), right));
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

#line 729 "raku.tab.c"

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
  YYSYMBOL_VAR_SCALAR_IDX = 18,            /* VAR_SCALAR_IDX  */
  YYSYMBOL_VAR_SCALAR_HASH = 19,           /* VAR_SCALAR_HASH  */
  YYSYMBOL_VAR_SCALAR_ANGLE = 20,          /* VAR_SCALAR_ANGLE  */
  YYSYMBOL_CARET = 21,                     /* CARET  */
  YYSYMBOL_DOLLAR_LBRACKET = 22,           /* DOLLAR_LBRACKET  */
  YYSYMBOL_VAR_CAPTURE = 23,               /* VAR_CAPTURE  */
  YYSYMBOL_VAR_FH = 24,                    /* VAR_FH  */
  YYSYMBOL_VAR_NAMED_CAPTURE = 25,         /* VAR_NAMED_CAPTURE  */
  YYSYMBOL_KW_USE = 26,                    /* KW_USE  */
  YYSYMBOL_TESTOP = 27,                    /* TESTOP  */
  YYSYMBOL_KW_MY = 28,                     /* KW_MY  */
  YYSYMBOL_KW_SAY = 29,                    /* KW_SAY  */
  YYSYMBOL_KW_PRINT = 30,                  /* KW_PRINT  */
  YYSYMBOL_KW_IF = 31,                     /* KW_IF  */
  YYSYMBOL_KW_ELSE = 32,                   /* KW_ELSE  */
  YYSYMBOL_KW_ELSIF = 33,                  /* KW_ELSIF  */
  YYSYMBOL_KW_WHILE = 34,                  /* KW_WHILE  */
  YYSYMBOL_KW_FOR = 35,                    /* KW_FOR  */
  YYSYMBOL_KW_SUB = 36,                    /* KW_SUB  */
  YYSYMBOL_KW_GATHER = 37,                 /* KW_GATHER  */
  YYSYMBOL_KW_TAKE = 38,                   /* KW_TAKE  */
  YYSYMBOL_KW_RETURN = 39,                 /* KW_RETURN  */
  YYSYMBOL_KW_EXIT = 40,                   /* KW_EXIT  */
  YYSYMBOL_KW_CONSTANT = 41,               /* KW_CONSTANT  */
  YYSYMBOL_KW_ENUM = 42,                   /* KW_ENUM  */
  YYSYMBOL_KW_JOIN = 43,                   /* KW_JOIN  */
  YYSYMBOL_KW_GIVEN = 44,                  /* KW_GIVEN  */
  YYSYMBOL_KW_WHEN = 45,                   /* KW_WHEN  */
  YYSYMBOL_KW_DEFAULT = 46,                /* KW_DEFAULT  */
  YYSYMBOL_KW_WITH = 47,                   /* KW_WITH  */
  YYSYMBOL_KW_WITHOUT = 48,                /* KW_WITHOUT  */
  YYSYMBOL_KW_EXISTS = 49,                 /* KW_EXISTS  */
  YYSYMBOL_KW_DELETE = 50,                 /* KW_DELETE  */
  YYSYMBOL_KW_UNLESS = 51,                 /* KW_UNLESS  */
  YYSYMBOL_KW_UNTIL = 52,                  /* KW_UNTIL  */
  YYSYMBOL_KW_REPEAT = 53,                 /* KW_REPEAT  */
  YYSYMBOL_KW_LOOP = 54,                   /* KW_LOOP  */
  YYSYMBOL_KW_LAST = 55,                   /* KW_LAST  */
  YYSYMBOL_KW_NEXT = 56,                   /* KW_NEXT  */
  YYSYMBOL_KW_MAP = 57,                    /* KW_MAP  */
  YYSYMBOL_KW_GREP = 58,                   /* KW_GREP  */
  YYSYMBOL_KW_SORT = 59,                   /* KW_SORT  */
  YYSYMBOL_KW_REVERSE = 60,                /* KW_REVERSE  */
  YYSYMBOL_KW_TRY = 61,                    /* KW_TRY  */
  YYSYMBOL_KW_CATCH = 62,                  /* KW_CATCH  */
  YYSYMBOL_KW_DIE = 63,                    /* KW_DIE  */
  YYSYMBOL_KW_FAIL = 64,                   /* KW_FAIL  */
  YYSYMBOL_KW_CLASS = 65,                  /* KW_CLASS  */
  YYSYMBOL_KW_METHOD = 66,                 /* KW_METHOD  */
  YYSYMBOL_KW_HAS = 67,                    /* KW_HAS  */
  YYSYMBOL_KW_NEW = 68,                    /* KW_NEW  */
  YYSYMBOL_KW_ROLE = 69,                   /* KW_ROLE  */
  YYSYMBOL_KW_MULTI = 70,                  /* KW_MULTI  */
  YYSYMBOL_KW_PROTO = 71,                  /* KW_PROTO  */
  YYSYMBOL_OP_NAME = 72,                   /* OP_NAME  */
  YYSYMBOL_OP_REDUCE = 73,                 /* OP_REDUCE  */
  YYSYMBOL_ARR_ALL_SLICE = 74,             /* ARR_ALL_SLICE  */
  YYSYMBOL_SLURPY_POS = 75,                /* SLURPY_POS  */
  YYSYMBOL_SLURPY_LOL = 76,                /* SLURPY_LOL  */
  YYSYMBOL_SLURPY_NAMED = 77,              /* SLURPY_NAMED  */
  YYSYMBOL_KW_HANDLES = 78,                /* KW_HANDLES  */
  YYSYMBOL_WORDLIST = 79,                  /* WORDLIST  */
  YYSYMBOL_OP_COLON_D = 80,                /* OP_COLON_D  */
  YYSYMBOL_OP_COLON_U = 81,                /* OP_COLON_U  */
  YYSYMBOL_YADA = 82,                      /* YADA  */
  YYSYMBOL_KW_GRAMMAR = 83,                /* KW_GRAMMAR  */
  YYSYMBOL_KW_TOKEN = 84,                  /* KW_TOKEN  */
  YYSYMBOL_KW_RULE = 85,                   /* KW_RULE  */
  YYSYMBOL_KW_REGEX = 86,                  /* KW_REGEX  */
  YYSYMBOL_KW_MODULE = 87,                 /* KW_MODULE  */
  YYSYMBOL_PHASER = 88,                    /* PHASER  */
  YYSYMBOL_OP_FATARROW = 89,               /* OP_FATARROW  */
  YYSYMBOL_OP_RANGE = 90,                  /* OP_RANGE  */
  YYSYMBOL_OP_RANGE_EX = 91,               /* OP_RANGE_EX  */
  YYSYMBOL_OP_ARROW = 92,                  /* OP_ARROW  */
  YYSYMBOL_OP_EQ = 93,                     /* OP_EQ  */
  YYSYMBOL_OP_NE = 94,                     /* OP_NE  */
  YYSYMBOL_OP_LE = 95,                     /* OP_LE  */
  YYSYMBOL_OP_GE = 96,                     /* OP_GE  */
  YYSYMBOL_OP_SEQ = 97,                    /* OP_SEQ  */
  YYSYMBOL_OP_SNE = 98,                    /* OP_SNE  */
  YYSYMBOL_OP_SLT = 99,                    /* OP_SLT  */
  YYSYMBOL_OP_SLE = 100,                   /* OP_SLE  */
  YYSYMBOL_OP_SGT = 101,                   /* OP_SGT  */
  YYSYMBOL_OP_SGE = 102,                   /* OP_SGE  */
  YYSYMBOL_OP_CMP3 = 103,                  /* OP_CMP3  */
  YYSYMBOL_OP_CMPG = 104,                  /* OP_CMPG  */
  YYSYMBOL_OP_LEG = 105,                   /* OP_LEG  */
  YYSYMBOL_OP_AND = 106,                   /* OP_AND  */
  YYSYMBOL_OP_OR = 107,                    /* OP_OR  */
  YYSYMBOL_OP_TERNARY1 = 108,              /* OP_TERNARY1  */
  YYSYMBOL_OP_TERNARY2 = 109,              /* OP_TERNARY2  */
  YYSYMBOL_OP_BIND = 110,                  /* OP_BIND  */
  YYSYMBOL_OP_DOTEQ = 111,                 /* OP_DOTEQ  */
  YYSYMBOL_OP_SMATCH = 112,                /* OP_SMATCH  */
  YYSYMBOL_OP_INC = 113,                   /* OP_INC  */
  YYSYMBOL_OP_DEC = 114,                   /* OP_DEC  */
  YYSYMBOL_OP_ADD_EQ = 115,                /* OP_ADD_EQ  */
  YYSYMBOL_OP_SUB_EQ = 116,                /* OP_SUB_EQ  */
  YYSYMBOL_OP_MUL_EQ = 117,                /* OP_MUL_EQ  */
  YYSYMBOL_OP_DIV_EQ = 118,                /* OP_DIV_EQ  */
  YYSYMBOL_OP_CAT_EQ = 119,                /* OP_CAT_EQ  */
  YYSYMBOL_OP_DOR = 120,                   /* OP_DOR  */
  YYSYMBOL_OP_DIV = 121,                   /* OP_DIV  */
  YYSYMBOL_ADV_EXISTS = 122,               /* ADV_EXISTS  */
  YYSYMBOL_ADV_DELETE = 123,               /* ADV_DELETE  */
  YYSYMBOL_OP_BAND = 124,                  /* OP_BAND  */
  YYSYMBOL_OP_SHL = 125,                   /* OP_SHL  */
  YYSYMBOL_OP_GCD = 126,                   /* OP_GCD  */
  YYSYMBOL_OP_LCM = 127,                   /* OP_LCM  */
  YYSYMBOL_OP_MODW = 128,                  /* OP_MODW  */
  YYSYMBOL_OP_NBAND = 129,                 /* OP_NBAND  */
  YYSYMBOL_OP_UMUL = 130,                  /* OP_UMUL  */
  YYSYMBOL_OP_UDIV = 131,                  /* OP_UDIV  */
  YYSYMBOL_OP_BORT = 132,                  /* OP_BORT  */
  YYSYMBOL_OP_NBOR = 133,                  /* OP_NBOR  */
  YYSYMBOL_OP_QBOR = 134,                  /* OP_QBOR  */
  YYSYMBOL_OP_QBXOR = 135,                 /* OP_QBXOR  */
  YYSYMBOL_OP_UMINUS_I = 136,              /* OP_UMINUS_I  */
  YYSYMBOL_OP_COMPOSE = 137,               /* OP_COMPOSE  */
  YYSYMBOL_OP_COMPOSEU = 138,              /* OP_COMPOSEU  */
  YYSYMBOL_OP_SETINT = 139,                /* OP_SETINT  */
  YYSYMBOL_OP_SETMUL = 140,                /* OP_SETMUL  */
  YYSYMBOL_OP_SETUNI = 141,                /* OP_SETUNI  */
  YYSYMBOL_OP_SETSUM = 142,                /* OP_SETSUM  */
  YYSYMBOL_OP_SETDIF = 143,                /* OP_SETDIF  */
  YYSYMBOL_OP_SETSYM = 144,                /* OP_SETSYM  */
  YYSYMBOL_OP_XORJ = 145,                  /* OP_XORJ  */
  YYSYMBOL_OP_RANGE_XL = 146,              /* OP_RANGE_XL  */
  YYSYMBOL_OP_RANGE_XB = 147,              /* OP_RANGE_XB  */
  YYSYMBOL_OP_BUT = 148,                   /* OP_BUT  */
  YYSYMBOL_OP_DOESW = 149,                 /* OP_DOESW  */
  YYSYMBOL_OP_COLL = 150,                  /* OP_COLL  */
  YYSYMBOL_OP_UNICMP = 151,                /* OP_UNICMP  */
  YYSYMBOL_OP_IDENT3 = 152,                /* OP_IDENT3  */
  YYSYMBOL_OP_EQV = 153,                   /* OP_EQV  */
  YYSYMBOL_OP_BEFORE = 154,                /* OP_BEFORE  */
  YYSYMBOL_OP_AFTER = 155,                 /* OP_AFTER  */
  YYSYMBOL_OP_SETCONT = 156,               /* OP_SETCONT  */
  YYSYMBOL_OP_SETELEM = 157,               /* OP_SETELEM  */
  YYSYMBOL_OP_APPROX = 158,                /* OP_APPROX  */
  YYSYMBOL_OP_SMARTM = 159,                /* OP_SMARTM  */
  YYSYMBOL_OP_NSMARTM = 160,               /* OP_NSMARTM  */
  YYSYMBOL_OP_MINOP = 161,                 /* OP_MINOP  */
  YYSYMBOL_OP_MAXOP = 162,                 /* OP_MAXOP  */
  YYSYMBOL_OP_XOROP = 163,                 /* OP_XOROP  */
  YYSYMBOL_OP_DIVIS = 164,                 /* OP_DIVIS  */
  YYSYMBOL_OP_REP_X = 165,                 /* OP_REP_X  */
  YYSYMBOL_OP_REP_XX = 166,                /* OP_REP_XX  */
  YYSYMBOL_OP_POW = 167,                   /* OP_POW  */
  YYSYMBOL_168_ = 168,                     /* '='  */
  YYSYMBOL_169_ = 169,                     /* '!'  */
  YYSYMBOL_170_ = 170,                     /* '<'  */
  YYSYMBOL_171_ = 171,                     /* '>'  */
  YYSYMBOL_172_ = 172,                     /* '|'  */
  YYSYMBOL_173_ = 173,                     /* '&'  */
  YYSYMBOL_174_ = 174,                     /* '~'  */
  YYSYMBOL_175_ = 175,                     /* '+'  */
  YYSYMBOL_176_ = 176,                     /* '-'  */
  YYSYMBOL_177_ = 177,                     /* '*'  */
  YYSYMBOL_178_ = 178,                     /* '/'  */
  YYSYMBOL_179_ = 179,                     /* '%'  */
  YYSYMBOL_UMINUS = 180,                   /* UMINUS  */
  YYSYMBOL_181_ = 181,                     /* '.'  */
  YYSYMBOL_182_ = 182,                     /* ';'  */
  YYSYMBOL_183_ = 183,                     /* '('  */
  YYSYMBOL_184_ = 184,                     /* ')'  */
  YYSYMBOL_185_ = 185,                     /* ','  */
  YYSYMBOL_186_ = 186,                     /* '['  */
  YYSYMBOL_187_ = 187,                     /* ']'  */
  YYSYMBOL_188_ = 188,                     /* '{'  */
  YYSYMBOL_189_ = 189,                     /* '}'  */
  YYSYMBOL_190_ = 190,                     /* ':'  */
  YYSYMBOL_YYACCEPT = 191,                 /* $accept  */
  YYSYMBOL_program = 192,                  /* program  */
  YYSYMBOL_stmt_list = 193,                /* stmt_list  */
  YYSYMBOL_stmt = 194,                     /* stmt  */
  YYSYMBOL_if_stmt = 195,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 196,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 197,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 198,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 199,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 200,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 201,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 202,                /* loop_incr  */
  YYSYMBOL_for_stmt = 203,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 204,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 205,          /* catch_when_list  */
  YYSYMBOL_when_list = 206,                /* when_list  */
  YYSYMBOL_sub_trait_list = 207,           /* sub_trait_list  */
  YYSYMBOL_sub_decl = 208,                 /* sub_decl  */
  YYSYMBOL_sub_body = 209,                 /* sub_body  */
  YYSYMBOL_method_body = 210,              /* method_body  */
  YYSYMBOL_pkg_name = 211,                 /* pkg_name  */
  YYSYMBOL_class_decl = 212,               /* class_decl  */
  YYSYMBOL_role_decl = 213,                /* role_decl  */
  YYSYMBOL_module_decl = 214,              /* module_decl  */
  YYSYMBOL_is_clauses = 215,               /* is_clauses  */
  YYSYMBOL_class_body_list = 216,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 217,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 218,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 219,           /* named_arg_list  */
  YYSYMBOL_pair_list = 220,                /* pair_list  */
  YYSYMBOL_param_list = 221,               /* param_list  */
  YYSYMBOL_block = 222,                    /* block  */
  YYSYMBOL_closure = 223,                  /* closure  */
  YYSYMBOL_expr = 224,                     /* expr  */
  YYSYMBOL_tern_expr = 225,                /* tern_expr  */
  YYSYMBOL_or_expr = 226,                  /* or_expr  */
  YYSYMBOL_and_expr = 227,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 228,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 229,               /* divis_expr  */
  YYSYMBOL_jct_expr = 230,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 231,                 /* dor_expr  */
  YYSYMBOL_range_expr = 232,               /* range_expr  */
  YYSYMBOL_add_expr = 233,                 /* add_expr  */
  YYSYMBOL_repl_expr = 234,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 235,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 236,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 237,               /* unary_expr  */
  YYSYMBOL_pow_expr = 238,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 239,              /* scalar_list  */
  YYSYMBOL_meth_name = 240,                /* meth_name  */
  YYSYMBOL_postfix_expr = 241,             /* postfix_expr  */
  YYSYMBOL_call_expr = 242,                /* call_expr  */
  YYSYMBOL_arg_list = 243,                 /* arg_list  */
  YYSYMBOL_paren_group = 244,              /* paren_group  */
  YYSYMBOL_atom = 245                      /* atom  */
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
#define YYLAST   7684

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  191
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  579
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1440

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   423


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
       2,     2,     2,   169,     2,     2,     2,   179,   173,     2,
     183,   184,   177,   175,   185,   176,   181,   178,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   190,   182,
     170,   168,   171,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   186,     2,   187,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   188,   172,   189,   174,     2,     2,     2,
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
     165,   166,   167,   180
};

#if RAKU_YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   754,   754,   765,   766,   769,   771,   773,   775,   777,
     779,   782,   784,   786,   788,   790,   794,   796,   798,   800,
     802,   806,   810,   812,   816,   820,   822,   824,   826,   828,
     830,   834,   838,   840,   842,   844,   846,   848,   850,   852,
     854,   856,   858,   866,   868,   870,   872,   874,   876,   878,
     881,   884,   886,   889,   892,   894,   898,   900,   902,   904,
     906,   909,   912,   914,   917,   920,   923,   925,   930,   934,
     938,   941,   944,   948,   951,   954,   957,   960,   963,   966,
     969,   972,   974,   976,   978,   980,   983,   985,   987,   989,
     992,   995,   999,  1002,  1004,  1006,  1008,  1011,  1014,  1017,
    1020,  1023,  1026,  1029,  1030,  1031,  1032,  1033,  1034,  1035,
    1037,  1039,  1043,  1048,  1052,  1054,  1056,  1058,  1060,  1062,
    1064,  1066,  1068,  1071,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1081,  1083,  1085,  1087,  1088,  1089,  1090,  1091,  1094,
    1096,  1098,  1100,  1102,  1104,  1106,  1108,  1112,  1114,  1116,
    1118,  1120,  1122,  1126,  1128,  1132,  1134,  1136,  1138,  1142,
    1144,  1148,  1150,  1152,  1156,  1158,  1160,  1164,  1167,  1173,
    1179,  1187,  1193,  1198,  1203,  1211,  1222,  1224,  1228,  1229,
    1235,  1236,  1239,  1247,  1253,  1259,  1267,  1273,  1281,  1287,
    1293,  1302,  1309,  1318,  1327,  1336,  1337,  1340,  1343,  1345,
    1347,  1349,  1351,  1353,  1355,  1357,  1360,  1362,  1364,  1366,
    1369,  1372,  1378,  1379,  1380,  1383,  1386,  1389,  1391,  1393,
    1395,  1397,  1399,  1401,  1403,  1406,  1408,  1410,  1412,  1415,
    1418,  1424,  1427,  1443,  1458,  1473,  1474,  1486,  1500,  1501,
    1502,  1505,  1508,  1511,  1514,  1517,  1520,  1523,  1526,  1529,
    1532,  1535,  1538,  1545,  1551,  1557,  1564,  1571,  1578,  1582,
    1586,  1590,  1594,  1601,  1608,  1615,  1622,  1632,  1640,  1648,
    1657,  1664,  1671,  1681,  1689,  1697,  1706,  1715,  1730,  1731,
    1736,  1741,  1748,  1752,  1756,  1763,  1767,  1773,  1775,  1777,
    1779,  1783,  1784,  1785,  1786,  1787,  1788,  1789,  1790,  1791,
    1792,  1793,  1794,  1795,  1796,  1797,  1798,  1799,  1800,  1801,
    1802,  1803,  1804,  1805,  1806,  1809,  1810,  1811,  1813,  1815,
    1817,  1819,  1821,  1824,  1826,  1828,  1830,  1832,  1835,  1838,
    1842,  1845,  1848,  1851,  1853,  1856,  1859,  1862,  1865,  1868,
    1870,  1872,  1874,  1876,  1878,  1880,  1882,  1884,  1888,  1891,
    1892,  1893,  1894,  1895,  1896,  1897,  1898,  1903,  1908,  1910,
    1913,  1915,  1918,  1919,  1920,  1921,  1922,  1925,  1926,  1929,
    1930,  1931,  1932,  1933,  1934,  1935,  1936,  1937,  1938,  1939,
    1940,  1941,  1942,  1943,  1944,  1945,  1946,  1947,  1948,  1949,
    1950,  1951,  1952,  1958,  1964,  1970,  1976,  1979,  1980,  1983,
    1984,  1985,  1986,  1987,  1988,  1989,  1990,  1991,  1994,  1996,
    1999,  2000,  2001,  2002,  2003,  2004,  2005,  2006,  2009,  2010,
    2011,  2012,  2015,  2016,  2017,  2020,  2021,  2022,  2023,  2024,
    2025,  2026,  2027,  2030,  2031,  2032,  2033,  2034,  2035,  2036,
    2037,  2038,  2040,  2042,  2044,  2046,  2049,  2050,  2051,  2052,
    2053,  2058,  2061,  2062,  2065,  2066,  2069,  2070,  2071,  2072,
    2073,  2074,  2075,  2076,  2077,  2078,  2079,  2080,  2081,  2083,
    2085,  2091,  2096,  2097,  2099,  2101,  2106,  2108,  2117,  2126,
    2133,  2137,  2142,  2149,  2154,  2160,  2166,  2173,  2180,  2185,
    2190,  2197,  2202,  2207,  2214,  2221,  2228,  2233,  2238,  2240,
    2242,  2244,  2246,  2248,  2250,  2252,  2254,  2256,  2259,  2260,
    2261,  2262,  2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,
    2271,  2274,  2275,  2276,  2278,  2283,  2284,  2288,  2289,  2290,
    2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,  2307,  2308,
    2309,  2313,  2317,  2320,  2322,  2324,  2326,  2328,  2330,  2332,
    2334,  2336,  2338,  2340,  2342,  2344,  2346,  2348,  2350,  2352,
    2354,  2356,  2358,  2360,  2361,  2365,  2369,  2373,  2374,  2376,
    2378,  2381,  2382,  2384,  2386,  2389,  2390,  2391,  2392,  2395
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
  "VAR_TWIGIL", "IDENT", "VAR_ARRAY_TWIGIL", "VAR_HASH_TWIGIL",
  "VAR_SCALAR_IDX", "VAR_SCALAR_HASH", "VAR_SCALAR_ANGLE", "CARET",
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

#define YYPACT_NINF (-1060)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-577)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1060,    31,  4337, -1060, -1060, -1060, -1060, -1060, -1060,   546,
      89,   248,   180,     3, -1060, -1060,  -139,   -45,    46,  7309,
    4442, -1060, -1060, -1060,   164,  1412,   133,  6082,  6187,  6269,
    6374,  6456,     8,    84,  6456,  4524,  4629,   860,   217,  6456,
    6456,   236,   368,  6561,  6643,    92,   189,     5,    55,   238,
     238,  6748,  6456,    92,   277,  6456,  4711,   259,   259,    72,
    7309, -1060, -1060,   259,   259,  4816,   479,   549,   572,  7309,
    7309,  7309,  1272, -1060,  4898,  5034,   328, -1060, -1060, -1060,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060,  7409,   571,   579,   434,   582,   586,   764,   801,   589,
     996,   426,   140,   759,   790, -1060, -1060,   560,   592,   581,
     601,  6456,   746, -1060, -1060,  6456,  6456,  6456,  6456,  6456,
    6456,  1632,  6456,  6830,  5116,  6456,   808,  6456, -1060, -1060,
    6456,   629,   462,  1826,  6456,  6456,   823,   467,   700,   109,
     391,   -81,   714,   656,   686,   254, -1060, -1060,   693, -1060,
     731,   650,   283, -1060,   253,  5221, -1060,  1908,     6, -1060,
     222,   439,   446,   452,  1026,   887,  1040,   479,  4898,   541,
    4898,   324,  4898,    92,  4898,    92,    32,   461,   626,   643,
   -1060, -1060, -1060,   285, -1060,    76, -1060,   343,   785,   807,
     885,   888,   901,   311,   423,  4898,    92,  4898,    92,   599,
    5303, -1060,  6456,  6456, -1060,  6456,  6456, -1060,  6456,  5408,
    5490,  5595,  5677, -1060, -1060,  1029,    65, -1060, -1060, -1060,
     933, -1060, -1060,   929,   948,    20, -1060,   938,   951,   662,
     498,   265,  6456,  7502,   963, -1060,   489, -1060, -1060, -1060,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060, -1060, -1060, -1060,   969, -1060,   918, -1060,   550,
     973,  3402,  6456,  6456,  6456,  6456,  6456,  6456,  6456,  6456,
   -1060,  6456,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,   330,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,  7309,
    7309,  7309,  7309,  7309,  1272,  6456,  1222,   989,   517,  1014,
    1024,  1039,  1054,  1057,  1062, -1060,   983,  1068,  4898,   431,
     997,   607,  1071,  1008,  1028,  1096, -1060,  1046,  1208,  1081,
     184, -1060,    62,   990,   992,  1107,  1109,  1129,  5116,  1288,
    6456,  6456,  6456,  1290,  1272,  1222,  6456,  6456,  6456,  6456,
    6456,  6456,  6456,  2044, -1060, -1060,  1126,  1138,   918,   998,
     470,  1313, -1060,  2997,  6456,  6456, -1060,  6456,  6935, -1060,
    6456,  7017, -1060,   456,   -56,   -31,   637,  1167,  1169,  1002,
    1004,  6456,  6456,  6456,  6456,  6456,  6456,  6456, -1060,  3132,
    1018,  6456,  6456, -1060,  3132,  1020,  1161,  1025, -1060,   479,
    3132, -1060,  7309,  7309,    28, -1060, -1060,    -7, -1060,   272,
   -1060, -1060, -1060,  1030, -1060,  3132,  6456,  6456, -1060,  6456,
    6456, -1060,  6456,  6456,  1163,  3132, -1060,  1338,  6456,  1373,
    6456,  1035,  1357,  1041, -1060,  6456,  6456,  1379,  1209,  1210,
    1211,  1212,  1213,  1207,  6456, -1060,  6456, -1060,  6456, -1060,
      92,  6456,    92,    44, -1060,    40, -1060,    54,  1214,  1215,
   -1060, -1060,  6456,  6456,  6456,  1384,  1218, -1060, -1060,  1390,
   -1060,  2126, -1060,  2262,  2344, -1060, -1060,   231,   424,   529,
    6082,  6187,  6456,  1009,   245,   282, -1060,    47,  1221,  1226,
    1227,  1229,  1230,  1231,  1232,  1238,  1239,  1240, -1060,   582,
    1299,   582,   582,   582,   586,  1274,  1274,  1274,  1274,  1274,
    1274,  1274,  1274,  1274,  1274,  1274,  1274,  1274,  1274,  1274,
    1274,  1274,  1274,  1274,  1274,  1274,  1274,  1274, -1060, -1060,
   -1060, -1060,   801,   996,   996,   996,   996,   996,   996,   996,
     996,   996,   209,   209,   209,   209,   209,   209,   209,   140,
     140,   140,   759,   759,   790,   790,   790,   790,   790,   790,
     790, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060, -1060, -1060,   -19,  1257,  1388,   378, -1060, -1060,
    2479, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,  1043,
   -1060,  3132,  6456,  6456,  3132,  1273, -1060,   -71,   370, -1060,
    2561, -1060,  2697,  6456,   515, -1060,     4, -1060,  3214,  1275,
     422,   481,   751,  1269,  1253,  1258,  1255,  1279,   442,   454,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,   761, -1060,
   -1060,  1264,  1436,  5782, -1060,    81, -1060,  1270,  1271,  1276,
    4898,   543,  1277,  1365,   215,  5864,   647,  1278,  6456,  6456,
   -1060,  7122, -1060,  6456, -1060,    59, -1060,  6456,  6456,  1289,
    2262,  1281,  1282,  1283,  1284,  1285,  1286,  1291,   703,  2262,
    1292,  1295,   877,    92,    33,  7204, -1060,    92,   489,   207,
     395,   399,   -10,  1047,  3589,  1441,  6456,    -3,  1465,  1467,
      92,   695,   905,  1297,  1298,  1300,  1301,  1302,  1305, -1060,
    1296,   111,  1317,  1303,  1319,  1304,    92,    92,    92,  1307,
    1312,  1327,  6456, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060,    92,  1308,  6456,    92, -1060,  1481,  1483, -1060,
     296,  1311,  1050,    64,   643,   250,  3776,  1321,  1323,  1324,
    1329, -1060, -1060, -1060,  1061, -1060,  1063, -1060,   832,  5116,
    6456,  6456,   267,    29,   402, -1060,   306,  6456,  6456, -1060,
    6456,  6456, -1060,  6456,  6456,  6456,  6456,  6456,  6456,  6456,
    6456, -1060,  1272,  1222, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060,  7309,  6456,  2779,  3132, -1060, -1060,  6456,  2915,
    3132,  1325,  1073,  2262,   934,  1314,  1322,   876,  6456, -1060,
   -1060,  6456, -1060, -1060,  6456, -1060,  1078, -1060,  1082,  1099,
   -1060,  5782,  1419,  1495,    86,  1101,  6456, -1060, -1060,  6456,
   -1060, -1060,  6456, -1060,  1153,  1166, -1060,  1168,  1170, -1060,
   -1060,  1340, -1060,  1328,   532,  1499, -1060, -1060, -1060,  1112,
   -1060,  3132, -1060,  6456,  6456,  1128, -1060,   152, -1060,  1334,
    1335,  4898,   942,  1336,  1311,  1132,  1337,  1339,  6456,  1134,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,  1139, -1060,
   -1060, -1060,  1294, -1060, -1060,  4898,    92, -1060, -1060,  1504,
   -1060,  1509,  1511,  1508,    -5, -1060,   -10,  6082,  6187,  4068,
   -1060,   428, -1060, -1060,  1535,  6456, -1060, -1060, -1060,    34,
   -1060,   567, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060,  6456,    92, -1060, -1060, -1060,  1342,  1369,  1520,
   -1060, -1060, -1060, -1060,  6456,  1371, -1060, -1060,    92,  1366,
   -1060, -1060,   346,   922,  1121,  1488, -1060, -1060, -1060,  1311,
    1311,  1149,  1154,  1541,  1542,  1543, -1060, -1060, -1060, -1060,
   -1060,   504, -1060, -1060, -1060,   894,  1370,   447,  6456,  6456,
    6456,  6456,  6456,  6456, -1060, -1060, -1060, -1060,   522,   563,
     602,   608,   611,   628,   630,   633,   642,   646,   652,   658,
     564,   568, -1060,  1378, -1060,  1156,  1296,  1380, -1060,  1162,
    1164,  1296, -1060,  1381,  1171, -1060, -1060, -1060, -1060,  1382,
    1383,  1385, -1060, -1060, -1060,  1377,  6456,  1386,   533, -1060,
    1389,  1391,  1392, -1060, -1060,  1551,  6000, -1060,  2262,   943,
   -1060, -1060,  1393,  1479,  1490, -1060, -1060,  1173, -1060,  3132,
   -1060, -1060,  1311, -1060, -1060,   972,  1398,  1400,    33, -1060,
    1175,  1318,    92,    92,    92, -1060,  1555, -1060,    -5, -1060,
     397,    37, -1060,   340,  6456,  6456,  6456,  6456,  6456,  6456,
    6456,  6456, -1060, -1060, -1060,  1568,  6456,    66,  1573,  1574,
      92,  1397, -1060, -1060,    92,  1407,  6456, -1060, -1060, -1060,
     882,   897,   127,   135,  1060,  1408,  1409,  1091, -1060, -1060,
    1311,  1311,  1584,  1588,  1589,  6456,  1431,   553, -1060,   661,
     664,   666,   669,   670,   688, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060,  6456,  6456, -1060,
   -1060, -1060, -1060, -1060, -1060,  1421, -1060, -1060, -1060, -1060,
   -1060,  6456,  6000, -1060, -1060, -1060,  1430, -1060,  1422,  1177,
   -1060, -1060,  6456,  6456,  2262,   976, -1060, -1060,  3132, -1060,
   -1060, -1060, -1060,    92,    92, -1060, -1060, -1060, -1060, -1060,
   -1060,  6456,  6456,  6456, -1060, -1060, -1060,   690,   698,   701,
     715,   716,   719,   721,   740, -1060, -1060,  1590,  6456, -1060,
   -1060, -1060, -1060, -1060,  6456,  1423, -1060,   148,  1527, -1060,
     165, -1060,  1595,  1596,  1432,  6456, -1060,  1597,  1598,  1433,
    6456, -1060,   200,   229,  1434,  1435, -1060, -1060,  1437,   917,
   -1060, -1060, -1060, -1060, -1060,  1439,  6456,  6456, -1060, -1060,
   -1060, -1060, -1060, -1060,   741,   742, -1060,  1438,  1440, -1060,
   -1060,  1443, -1060, -1060,  1179, -1060,   978,  1333, -1060,   750,
     765,   774, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060, -1060,  1444,    92,  1442,  1183,  1429,  3963,  1442,  1185,
    1445,  1447, -1060,  1449,   -30,   182, -1060,  1451,  1604,  1608,
    1452,  6456, -1060,  1625,  1626,  1460,  6456, -1060, -1060, -1060,
     170,   177, -1060, -1060,   777,   788, -1060, -1060, -1060, -1060,
   -1060,  1476, -1060,    92, -1060, -1060, -1060, -1060,  6456, -1060,
   -1060,  1442, -1060,   580,  6082,  6187,  4150, -1060,   463, -1060,
    1442, -1060, -1060, -1060,  6456, -1060,  6456, -1060, -1060,  1477,
    1478, -1060,  1480,  1482,  1484, -1060,  1485,  1442,  1187,  1442,
    1189, -1060, -1060, -1060, -1060,  1486, -1060,  6456,   523,   261,
   -1060,   342,  6456,  6456,  6456,  6456,  6456,  6456,  6456,  6456,
   -1060, -1060,  1489,  1491, -1060, -1060, -1060, -1060, -1060, -1060,
   -1060,  1442, -1060,  1442,    92,   794,  6456,  6456,  6456, -1060,
   -1060, -1060,   796,   805,   806,   829,   853,   854,   858,   859,
   -1060, -1060, -1060, -1060, -1060, -1060,   867,   868,   871, -1060,
   -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060, -1060
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   525,   526,   527,   528,   530,   531,
     538,   539,   564,   563,   565,   566,     0,     0,     0,     0,
       0,   540,   541,   542,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   547,   529,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     3,     4,   105,   106,
     124,   125,   126,   127,   107,   108,   134,   135,   136,   138,
     137,   115,     0,   359,   361,   366,   368,   396,   398,   407,
     409,   417,   421,   424,   432,   445,   451,   453,   469,   575,
     507,     0,     0,   534,   535,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   536,   537,
       0,     0,     0,     0,     0,     0,     0,   531,   538,   539,
     564,   563,     0,     0,     0,     0,   576,   449,   469,   575,
     507,   531,   538,   571,     0,     0,    43,     0,     0,   508,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   417,     0,     0,
     577,   357,   356,     0,    57,     0,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   161,
       0,   164,     0,     0,   128,     0,     0,   129,     0,     0,
       0,     3,     0,   503,   506,   109,     3,   114,   498,    59,
       0,   231,   235,     0,     0,     0,   450,     0,     0,   531,
     538,   539,     0,   116,     0,   454,     0,   532,   533,   448,
     447,   446,   456,   468,   461,   462,   463,   464,   467,   465,
     466,   459,   460,   457,   458,   497,   521,     0,   567,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   351,
     352,   353,   354,   355,   349,   476,     0,     0,     0,   350,
       0,     0,     0,     0,     0,     0,    47,   481,     0,   480,
     563,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   572,    36,     0,   521,     0,     0,
     511,     0,    46,   510,     0,     0,     6,     0,     0,    17,
       0,     0,    18,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,     0,    51,     0,     0,   142,     0,   154,     0,
       0,   173,     0,     0,     0,     3,   184,   291,   296,     0,
     309,   311,   313,     0,    54,     0,     0,     0,    56,     0,
       0,    62,     0,     0,     0,     0,   178,     0,     0,     0,
       0,     0,   157,     0,   160,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   499,     0,   501,     0,   504,
       0,     0,     0,     0,    58,     0,   238,     0,     0,     0,
     278,     3,     0,     0,     0,     0,     0,   117,   118,     0,
     579,     0,   522,     0,     0,   568,   316,   538,   539,   564,
       0,     0,     0,     0,     0,     0,   315,     0,   469,   507,
       0,     0,     0,     0,     0,     0,     0,     0,   358,   362,
       0,   365,   364,   363,   367,   369,   378,   381,   382,   377,
     376,   375,   374,   373,   372,   371,   370,   379,   380,   383,
     387,   388,   389,   390,   391,   384,   385,   386,   392,   393,
     394,   395,   397,   405,   404,   403,   402,   401,   400,   399,
     406,   408,   415,   414,   413,   412,   411,   410,   416,   420,
     419,   418,   422,   423,   430,   429,   428,   427,   426,   425,
     431,   441,   443,   444,   442,   438,   437,   436,   435,   434,
     433,   439,   440,   452,   492,     0,     0,   489,    11,    69,
       0,    98,    99,   100,   101,   102,    66,   475,    12,     0,
      22,     0,     0,     0,     0,   543,    13,   548,   556,    72,
       0,   484,     0,     0,   284,   473,     0,   471,   510,   549,
     551,   550,     0,     0,     0,     0,     0,     0,   492,   489,
     351,   352,   353,   354,   355,   349,   350,   573,     0,    37,
      45,     0,     0,     0,   512,     0,   509,     0,     0,     0,
       0,     0,     0,   528,   563,     0,     0,     0,     0,     0,
      33,     0,    34,     0,    35,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   522,     0,     0,   145,   522,     0,     0,
     410,   416,     0,     0,     0,     0,     0,   298,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    42,
     470,     0,     0,     0,     0,     0,   522,     0,   522,     0,
       0,     0,     0,   130,   131,   132,   133,   348,   500,   502,
     505,   110,     0,     0,     0,     0,   111,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   123,   455,   496,     0,   523,     0,   569,     0,     0,
       0,     0,     0,     0,     0,   341,     0,     0,     0,   342,
       0,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,   317,     0,     0,    81,    83,    85,    88,    86,    87,
      82,    84,     0,     0,     0,     0,    16,   485,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   560,
     562,     0,   559,   561,     0,   483,     0,   479,     0,     0,
     282,     0,     0,     0,     0,     0,     0,   552,   554,     0,
     553,   555,     0,   543,   548,   556,   549,   551,   550,   574,
      44,     0,   514,     0,   516,     0,     7,     5,     8,     0,
      19,     0,     9,     0,     0,     0,    26,     0,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    92,    91,    95,    93,    94,    90,    49,     0,    96,
      97,    52,   139,   144,   143,     0,     0,   153,   170,     0,
     172,     0,     0,     0,     0,   183,     0,     0,     0,     0,
     195,     0,   292,   305,     0,     0,   299,   300,   578,   301,
     297,     0,   310,   312,   314,    55,    60,    61,    64,    65,
      39,    38,     0,     0,   174,   557,   558,     0,     0,   155,
     158,   159,   162,   163,     0,     0,   176,   113,     0,     0,
     236,   237,     0,     0,     0,     0,   239,   233,   194,     0,
       0,     0,     0,     0,     0,     0,   277,   234,   119,   120,
     121,   548,   495,   524,   570,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   326,   333,   339,   340,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     492,   489,   360,     0,   491,     0,   494,     0,   488,     0,
       0,   493,    68,     0,     0,    23,   546,   545,   544,     0,
       0,     0,   482,   477,   478,     0,     0,     0,   284,   474,
       0,     0,     0,   515,   513,     0,     0,   517,     0,     0,
     288,   287,     0,     0,     0,    10,    28,     0,    29,     0,
      32,   188,     0,    41,    40,     0,   524,   524,     0,   146,
       0,   147,     0,     0,     0,   180,     0,   186,     0,   182,
       0,     0,   198,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   196,   294,   307,     0,     0,   302,     0,     0,
       0,     0,    79,    80,     0,     0,     0,   177,   112,   232,
       0,     0,     0,     0,     0,     0,     0,     0,   193,   191,
       0,     0,     0,     0,     0,     0,   543,   556,   336,     0,
       0,     0,     0,     0,     0,   344,   345,   346,   347,   318,
     320,   322,   325,   323,   324,   319,   321,     0,     0,    70,
     490,    71,   487,   486,    67,   524,    73,    77,    78,   283,
     285,     0,     0,    74,    76,    75,     0,   519,     0,     0,
      20,    27,     0,     0,     0,     0,   187,    14,     0,    50,
      53,   141,   140,   522,     0,   151,   171,   168,   169,   181,
     185,     0,     0,     0,   199,   200,   197,     0,     0,     0,
       0,     0,     0,     0,     0,   293,   306,     0,     0,   303,
     304,   179,   175,   156,     0,     0,   167,     0,     3,   271,
       0,   268,     0,     0,     0,     0,   245,     0,     0,     0,
       0,   240,     0,     0,     0,     0,   241,   242,     0,     0,
     190,   192,   279,   280,   281,     0,     0,     0,   327,   329,
     332,   330,   331,   328,     0,     0,    24,     0,     0,   520,
     518,   524,   290,   289,     0,    30,     0,   148,   149,     0,
       0,     0,   201,   203,   205,   208,   206,   207,   202,   204,
     295,   308,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   259,     0,     0,     0,
       0,     0,   247,     0,     0,     0,     0,   246,   243,   244,
       0,     0,   274,   122,     0,     0,   334,   335,   286,   283,
      21,   524,    15,     0,   152,   209,   211,   210,     0,   166,
     270,     0,   213,   564,     0,     0,     0,   212,     0,   267,
       0,   256,   257,   249,     0,   252,     0,   255,   248,     0,
       0,   260,     0,     0,     0,   261,     0,     0,     0,     0,
       0,   337,   338,    31,   150,     0,   269,     0,     0,     0,
     217,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     215,   266,     0,     0,   264,   265,   251,   262,   263,   250,
     276,     0,   273,     0,     0,     0,     0,     0,     0,   218,
     219,   216,     0,     0,     0,     0,     0,     0,     0,     0,
     253,   254,   275,   272,   165,   214,     0,     0,     0,   220,
     222,   224,   227,   225,   226,   221,   223,   228,   230,   229
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1060, -1060,     2, -1060,  -708,  -908, -1060, -1060, -1060, -1060,
   -1060,   323,  1630, -1060, -1060, -1060,   739, -1060,  -371, -1059,
     -50, -1060, -1060, -1060, -1060,   903, -1060, -1060,  -632,   991,
    -414,   494,  1326,    -2,  -272, -1060,   717,  1401,  1249,  1372,
   -1060,   699,   -28,   845,  1056,   783,   -12, -1060,  -162,  -300,
   -1060,     7,   444,     9,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   261,    77,    78,   716,    79,    80,    81,    82,
      83,  1225,    84,    85,   493,   741,   924,    86,   446,  1229,
     222,    87,    88,    89,   495,   770,    90,   775,   373,   686,
     453,   146,   209,   159,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   236,   255,
     107,   148,   160,   149,   150
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      92,   540,     2,   177,  1079,   419,   913,   147,   223,   108,
     848,   109,   110,   227,   228,   131,   855,   923,   154,   852,
     725,   400,  1086,   178,   934,   169,   171,   173,   175,   176,
     723,     3,   183,   185,   187,   498,   202,   191,   192,   447,
     448,   196,   198,   449,   614,   696,   617,   134,   226,   213,
     214,   839,   840,   218,   220,   767,   203,   239,   240,   241,
     431,  1105,  1231,   234,    29,   447,   448,   768,   431,   449,
     447,   448,   257,   259,   449,   447,   448,   644,   803,   449,
     432,   804,   805,   772,   658,   659,   205,   224,   432,   764,
     765,   806,   499,  1217,   807,   808,   874,   841,   809,   810,
     132,  1048,   133,   450,   451,   452,   206,   456,   225,   347,
     491,   492,   691,   349,   350,   351,   352,   353,   354,    31,
     357,   359,   361,   362,   439,   364,   692,   457,   365,   450,
     451,   452,   375,   376,   450,   451,   452,   693,  1354,   450,
     451,   452,  1232,   135,   161,   162,   163,   260,   164,   823,
    1237,   694,  1355,   396,  1233,   398,   952,   953,  1063,   447,
     448,   726,  1238,   449,   824,   935,   420,  1064,   430,   165,
     435,   825,   437,  1195,   166,   401,   447,   448,   445,   155,
     449,   447,   448,   445,   132,   449,   133,   204,   447,   448,
    1322,   179,   449,   471,   853,   473,    76,  1029,   478,   122,
     479,   480,  1106,   481,   482,  1234,   483,   485,   487,   483,
     489,   433,   722,  1239,   434,  1308,   136,   440,  1005,   433,
      76,    76,   434,   450,   451,   452,  1205,  1309,   769,   270,
     506,   401,   190,   766,  1218,  1340,   811,   207,   771,  1349,
     450,   451,   452,   894,  1313,   450,   451,   452,   980,   193,
     875,   571,   450,   451,   452,   875,  1314,   123,   458,   527,
     530,   531,   532,   533,   534,   535,   536,   537,   528,   538,
     109,   529,    76,   643,   221,   124,   797,   718,  1310,   379,
      76,   895,  1376,   727,   582,   583,   584,   585,   586,   587,
     588,  1391,   431,   128,   129,  1235,   798,   380,   998,   919,
     954,   422,   999,  1240,   884,   322,   323,  1315,  1400,  1236,
    1402,  1000,   432,   800,  1001,  1002,   167,  1241,  1003,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   613,  1294,   801,   983,   984,   985,   456,   568,   569,
     570,   122,  1422,   615,  1423,   221,   319,   320,   130,  1298,
    1356,   925,   728,   729,  1367,   431,   629,   457,   125,   981,
     982,  1369,   973,   974,  1357,   132,   975,   133,  1311,  1334,
    1191,   456,   200,   456,   459,   432,   652,    76,   654,   655,
     656,   194,  1312,   321,   660,   661,   662,   663,   664,   665,
     666,   457,   403,   457,   460,    76,   132,  1316,   133,   123,
     978,   676,   677,   678,   402,   679,   681,   403,   682,   687,
     260,  1317,   973,   974,   720,   721,   975,   789,   126,   701,
     702,   703,   704,   705,   706,   707,   208,   204,  1201,   710,
     711,   422,  1202,   504,   799,   505,   127,   179,   393,   986,
     394,   424,    76,   433,   425,   426,   434,   724,  1203,   428,
    1410,   392,   429,   380,   733,   734,  1004,   735,   736,  1094,
     737,   738,  1095,  1096,   207,   216,   743,   454,   745,   378,
     455,   802,  1097,   749,   750,  1098,  1099,   367,   976,  1100,
    1101,   467,   758,   368,   759,   977,   760,   921,   458,   762,
     235,   922,   842,   843,  1382,  1007,    91,  1383,  1384,   468,
     777,   778,   779,   776,   128,   129,   433,  1385,   316,   434,
    1386,  1387,  1020,  1021,  1388,  1389,   317,   318,   792,   793,
     794,   796,   458,  1071,   458,   461,   180,   182,   976,  1206,
     369,  1411,   319,   320,   125,  1119,   319,   320,   844,   199,
     201,   272,   273,   316,   857,   858,   828,   215,   217,   404,
    1022,   442,   443,  1087,  1406,  1089,   407,   422,  1407,   233,
     237,   829,   410,   319,   320,   356,   688,   424,   830,   321,
     425,   426,   421,   321,  1408,   422,   423,   374,  1107,   428,
     113,   114,   429,   238,   454,   424,  1204,   455,   425,   426,
     859,  1006,   427,   469,   126,   274,   275,   276,   319,   320,
     321,   399,   262,   860,   861,   263,   264,   405,  1128,  1129,
     270,   470,   790,   630,   408,   265,   631,  1102,   266,   267,
     411,   406,   268,   269,   689,   824,   839,   840,   409,   639,
     835,   836,   825,   475,   412,   321,  1138,   829,   690,   180,
     672,   850,   128,   129,   830,   270,   676,  1108,  1109,   862,
     121,   476,  1390,   673,   447,   448,   111,   112,   449,   113,
     114,   115,   116,   117,   118,   119,   503,   436,   271,   438,
     441,   873,  1135,  1121,   509,   842,   843,    76,   879,   278,
     279,   280,   281,   257,   378,   672,   889,   890,   277,   892,
     472,   893,   474,   128,   129,   896,   897,   791,   851,   619,
     620,  1186,  1055,  1055,   753,   428,   939,   940,   429,   311,
     941,  1145,  1409,   916,   120,  1056,  1172,  1200,   450,   451,
     452,  1257,   931,   428,   933,   880,   429,   343,   881,   121,
     510,   108,  1157,   109,   110,   514,  1158,   515,   282,   283,
     284,   285,   286,   287,   288,   754,   289,   824,  1377,   345,
     965,   829,  1146,   270,   825,    91,   290,   291,   830,  1250,
    1251,   348,   968,   113,   114,   386,   387,   388,   389,   390,
     942,   943,   944,   344,    92,   113,   114,   386,   387,   388,
     389,   390,   346,   108,   755,   109,   110,   995,   996,   997,
     756,  1147,   634,   814,   635,  1008,  1009,  1148,  1010,  1011,
    1149,  1012,  1013,  1014,  1015,  1016,  1017,  1018,  1019,   444,
     815,   366,   816,  1295,   445,   817,  1299,  1150,   391,  1151,
     695,  1023,  1152,   363,   818,   445,  1027,  1249,   819,   886,
     502,  1153,   887,   121,   820,  1154,  1039,   668,   377,  1040,
     821,  1155,  1041,   900,   382,   121,   902,  1156,   903,  1045,
    1258,   904,   905,  1259,  1050,  1260,   383,  1051,  1261,  1262,
    1052,   292,   293,   294,   295,   296,   297,   298,   299,   300,
     906,   188,   814,   708,   384,   189,   301,  1263,   712,  1282,
     815,  1060,  1061,   816,   719,   907,   378,  1283,   403,  1067,
    1284,   324,   325,   326,   327,   328,  1075,   817,   818,   732,
     381,   819,   416,   820,  1285,  1286,  1368,  1370,  1287,   740,
    1288,   331,   385,  1080,   332,   333,   334,   335,   336,   337,
     338,   339,   821,  1159,  1161,  1090,  1091,  1093,   302,  1289,
    1326,  1327,   900,  1104,   329,   330,   634,   242,   863,  1335,
     303,   304,   305,   306,   307,   308,   403,   902,   869,   243,
    1110,   244,   245,   462,  1336,   784,   906,   786,   788,  1166,
     246,   247,  1115,  1337,   464,   248,  1371,   340,   341,   342,
    1168,   249,   250,   309,   310,   463,   639,  1372,   814,   251,
     252,   253,   254,  1425,   761,  1429,   763,   815,   816,   539,
    1120,   541,   542,   543,  1430,  1431,  1139,  1140,  1141,  1142,
    1143,  1144,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   817,     4,     5,     6,     7,     8,   403,  1432,   994,
     151,   152,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,   818,   819,   413,   414,   415,
     820,   821,  1433,  1434,  1170,   145,    33,  1435,  1436,   900,
     902,   417,    39,   906,  1178,   418,  1437,  1438,    41,   911,
    1439,   403,   403,  1038,   832,  1227,    49,    50,    51,    52,
    1228,  1242,    55,   465,  1243,   834,  1244,  1245,   837,   634,
    1230,  1136,    60,    61,   846,  1228,   849,   945,    62,   466,
     403,   490,  1207,  1208,  1209,  1210,  1211,  1212,  1213,  1214,
    1321,    66,   512,   513,  1216,  1228,   242,   594,   595,   596,
     597,   598,   599,   600,  1226,   494,  1035,   496,   243,   403,
     244,   245,    67,    68,  1068,  1180,   500,  1069,   403,   246,
     247,   497,  1122,  1255,   248,  1123,  1124,  1125,  1126,   501,
     249,   250,   312,   313,   899,   508,   314,   315,   251,   252,
     253,   254,   511,   908,  1187,  1264,  1265,  1188,  1275,  1248,
    1332,   403,   516,   403,   589,   590,   591,   627,   403,  1267,
    1268,   618,   632,   633,   645,   646,   647,   648,    69,   637,
    1272,  1273,   671,   403,    70,    71,   699,   509,   512,   700,
      72,   184,    74,   714,   715,    75,   621,    76,   795,  1279,
    1280,  1281,   512,   709,   713,   513,   622,   912,   914,   717,
     513,   917,   918,   920,   730,   731,  1291,   638,    91,   746,
     513,   623,  1292,   641,   938,   748,   513,   512,   833,   640,
    1297,   926,   731,  1303,   979,   731,   624,   242,  1307,   625,
     959,   960,   961,   616,   626,   992,   403,   993,   403,   243,
     628,   244,   245,   636,  1324,  1325,   966,  1033,   403,   969,
     246,   247,  1042,   403,   642,   248,  1043,   646,  1025,  1026,
      91,   249,   250,  1030,  1031,   839,   840,  1034,   639,   251,
     252,   253,   254,  1044,   403,  1049,   646,   242,   842,   843,
     857,   858,   860,   861,   649,  1348,   512,  1058,   650,   243,
     651,   244,   245,   653,   108,   657,   109,   110,   669,  1362,
     246,   247,  1062,   887,  1366,   248,  1072,   731,  1076,   403,
     670,   249,   250,  1077,   403,  1059,  1078,   715,   674,   251,
     252,   253,   254,  1130,   731,   697,  1226,   698,  1131,   731,
    1160,   403,  1378,  1379,  1381,   739,  1162,   646,  1163,   403,
    1194,   715,  1392,   742,  1393,  1165,   403,   512,  1184,  1193,
     513,  1271,   403,  1331,   403,  1333,   715,  1341,   731,  1350,
     731,  1401,   731,  1403,   731,  1405,   210,   212,   592,   593,
    1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,   744,   747,
     751,   752,   753,   754,   755,   756,   757,   773,   774,   780,
     781,   782,   812,   827,  1426,  1427,  1428,   813,   822,   814,
    1081,   815,   816,   817,   818,     4,     5,     6,     7,     8,
     819,   820,   821,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,   302,   826,
     864,   838,   865,   856,   867,   866,   870,  1111,   145,    33,
     868,   871,   876,   877,   883,    39,   932,   898,   878,   882,
     888,    41,  1117,   900,   901,   902,   903,   904,   905,    49,
      50,    51,    52,   906,   909,    55,   936,   910,   937,   946,
     947,   403,   948,   949,   950,    60,    61,   951,   955,   962,
     957,    62,   956,   958,   963,   964,   970,   967,   971,   445,
     991,  1036,  1179,   988,    66,   989,   990,  1032,  1046,  1037,
    1047,  1053,  1054,  1185,  1057,  1082,  1065,  1066,  1070,  1073,
    1083,  1074,  1084,  1085,  1112,    67,    68,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
    1103,  1113,  1114,  1116,  1127,  1118,  1132,  1133,  1134,  1137,
    1159,  1169,  1161,  1164,  1166,  1167,  1176,  1168,  1182,  1171,
    1199,  1173,  1192,  1174,  1175,  1181,  1196,  1197,  1198,  1183,
    1189,    69,  1190,  1215,  1219,  1220,  1222,    70,    71,  1224,
    1246,  1247,  1252,    72,   156,   157,  1253,  1254,    75,  1256,
      76,  1269,   158,  1266,  1221,  1290,  1270,  1293,  1223,  1296,
    1300,  1301,  1304,  1305,  1302,  1306,  1318,  1319,  1342,  1359,
    1320,  1323,  1328,  1360,  1329,  1330,  1338,  1351,  1274,  1352,
    1228,  1353,  1276,  1358,  1361,     4,     5,     6,     7,     8,
    1363,  1364,  1365,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,  1373,  1394,
    1395,  1375,  1396,   181,  1397,  1088,  1398,  1399,   145,    33,
    1404,  1420,   972,  1421,   572,    39,   885,     0,   544,     0,
       0,    41,     0,     0,     0,     0,     0,  1277,  1278,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1339,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,     0,    70,    71,     0,
       0,     0,     0,    72,     0,    74,   355,     0,    75,     0,
      76,     0,   158,     0,     0,     0,     0,  1374,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   151,   152,   139,
     140,   370,    14,    15,   142,   143,   144,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   145,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,  1424,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    66,   151,
     152,   139,   140,   141,    14,    15,   142,   143,   144,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,   145,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
      66,    70,    71,     0,     0,     0,     0,    72,     0,    74,
     371,     0,    75,     0,    76,     0,   372,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
     145,    33,     0,    70,    71,     0,     0,    39,     0,    72,
       0,    74,   397,    41,    75,     0,    76,     0,   158,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    66,   151,   152,   139,
     140,   141,    14,    15,   142,   143,   144,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,   145,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,    66,    70,
      71,     0,     0,     0,     0,    72,     0,    74,     0,     0,
      75,   667,    76,     0,   158,     0,     0,     0,     0,    67,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,   145,    33,
       0,    70,    71,     0,     0,    39,     0,    72,     0,    74,
     783,    41,    75,     0,    76,     0,   158,     0,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,    74,   785,     0,    75,     0,
      76,     0,   158,     0,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     151,   152,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,   145,    33,     0,     0,    70,
      71,     0,    39,     0,     0,    72,     0,    74,    41,     0,
      75,   787,    76,     0,   158,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    66,   151,   152,   139,   140,   141,    14,    15,   142,
     143,   144,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,   145,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,    66,    70,    71,     0,     0,     0,     0,
      72,     0,    74,   831,     0,    75,     0,    76,     0,   158,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   151,   152,
     139,   140,   370,    14,    15,   142,   143,   144,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,   145,    33,     0,    70,    71,     0,     0,
      39,     0,    72,     0,    74,   845,    41,    75,     0,    76,
       0,   158,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    66,
     151,   152,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,   145,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    66,    70,    71,     0,     0,     0,     0,    72,     0,
      74,   847,     0,    75,     0,    76,     0,   372,     0,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   151,   152,   139,   140,
     370,    14,    15,   142,   143,   144,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,   145,    33,     0,    70,    71,     0,     0,    39,     0,
      72,     0,    74,  1024,    41,    75,     0,    76,     0,   158,
       0,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    66,   151,   152,
     139,   140,   141,    14,    15,   142,   143,   144,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   145,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,    74,  1028,
       0,    75,     0,    76,     0,   372,     0,     0,     0,     0,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    69,     0,   145,    33,
       0,     0,    70,    71,     0,    39,     0,     0,    72,     0,
      74,    41,     0,    75,     0,    76,     0,   675,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   151,   152,   139,   140,   370,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,    74,     0,     0,    75,     0,
      76,     0,   158,     0,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,     0,     0,     0,     0,    72,     0,    74,     0,     0,
      75,     0,    76,     0,   854,     4,     5,     6,     7,     8,
       0,     0,     0,     9,   517,   518,   519,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   520,   521,    29,     0,     0,    30,    31,    32,    33,
     522,   523,    36,    37,    38,    39,    40,     0,     0,     0,
       0,    41,    42,    43,    44,    45,    46,   524,   525,    49,
      50,    51,    52,    53,    54,    55,    56,    57,     0,     0,
       0,    58,    59,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,    63,     0,     0,     0,    64,
      65,     0,     0,     0,    66,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,     0,    70,    71,     0,
       0,     0,     0,    72,    73,    74,     0,     0,    75,     0,
      76,   526,     4,     5,     6,     7,     8,     0,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   927,   928,
      29,     0,     0,    30,    31,    32,    33,    34,   929,    36,
      37,    38,    39,    40,     0,     0,     0,     0,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,     0,     0,     0,    58,    59,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,    63,     0,     0,     0,    64,    65,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,     0,    70,    71,     0,     0,     0,     0,
      72,    73,    74,     0,     0,    75,     0,    76,   930,     4,
       5,     6,     7,     8,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,     0,     0,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,     0,     0,     0,     0,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,     0,     0,     0,    58,    59,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,    63,
       0,     0,     0,    64,    65,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
       0,    70,    71,     0,     0,     0,     0,    72,    73,    74,
       0,     0,    75,     0,    76,   987,     4,     5,     6,     7,
       8,     0,     0,     0,     9,    10,    11,  1343,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,  1344,  1345,    29,     0,     0,    30,    31,    32,
      33,    34,  1346,    36,    37,    38,    39,    40,     0,     0,
       0,     0,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,    63,     0,     0,     0,
      64,    65,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,    67,    68,     0,   151,
     152,   139,   140,   141,    14,    15,   142,   143,   144,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,    69,     0,     0,     0,     0,     0,    70,    71,
       0,    60,    61,     0,    72,    73,    74,    62,     0,    75,
       0,    76,  1347,     4,     5,     6,     7,     8,     0,     0,
      66,   151,   152,   139,   140,   141,    14,    15,   142,   143,
     144,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,   145,    33,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,    66,    70,    71,     0,     0,     0,     0,    72,
     184,    74,     0,     0,    75,     0,    76,  1092,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,     0,    70,    71,     0,     0,     0,
       0,    72,   184,    74,     0,     0,    75,     0,    76,  1380,
       4,     5,     6,     7,     8,     0,     0,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     0,
       0,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,     0,     0,     0,     0,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,     0,     0,     0,    58,    59,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
      63,     0,     0,     0,    64,    65,     0,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
      67,    68,     0,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,    69,     0,     0,     0,
       0,     0,    70,    71,     0,    60,    61,     0,    72,    73,
      74,    62,     0,    75,     0,    76,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,    74,     0,     0,    75,   153,
      76,     0,     4,     5,     6,     7,     8,    67,    68,     0,
     151,   152,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,    69,     0,     0,     0,     0,     0,    70,
      71,     0,    60,    61,     0,    72,   184,    74,    62,     0,
      75,     0,    76,     0,     4,     5,     6,     7,     8,     0,
       0,    66,   151,   152,   139,   140,   141,    14,    15,   142,
     143,   144,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,   145,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,    66,    70,    71,     0,     0,     0,     0,
      72,   186,    74,     0,     0,    75,     0,    76,     0,     4,
       5,     6,     7,     8,    67,    68,     0,   229,   230,   231,
     140,   141,    14,    15,   142,   143,   144,    19,    20,    21,
      22,    23,     0,     0,     0,   232,     0,     0,     0,     0,
       0,     0,   145,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
      69,     0,     0,     0,     0,     0,    70,    71,     0,    60,
      61,     0,    72,   219,    74,    62,     0,    75,     0,    76,
       0,     4,     5,     6,     7,     8,     0,     0,    66,   151,
     152,   139,   140,   141,    14,    15,   142,   143,   144,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,   145,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
      66,    70,    71,     0,     0,     0,     0,    72,     0,    74,
       0,     0,    75,     0,    76,     0,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
     145,    33,     0,    70,    71,     0,     0,    39,     0,    72,
       0,    74,   256,    41,    75,     0,    76,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    66,   151,   152,   139,
     140,   141,    14,    15,   142,   143,   144,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,   145,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,    66,    70,
      71,     0,     0,     0,     0,    72,     0,    74,     0,     0,
      75,   258,    76,     0,     4,     5,     6,     7,     8,    67,
      68,     0,   151,   152,   139,   140,   141,    14,    15,   142,
     143,   144,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,    69,     0,     0,     0,     0,
       0,    70,    71,   360,    60,    61,     0,    72,     0,    74,
      62,     0,    75,     0,    76,     0,     4,     5,     6,     7,
       8,     0,     0,    66,   151,   152,   139,   140,   141,    14,
      15,   142,   143,   144,    19,    20,    21,    22,    23,     0,
       0,   477,     0,     0,    67,    68,     0,     0,     0,   145,
      33,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,    66,    70,    71,     0,     0,
       0,     0,    72,   395,    74,     0,     0,    75,     0,    76,
       0,     4,     5,     6,     7,     8,    67,    68,     0,   151,
     152,   139,   140,   141,    14,    15,   142,   143,   144,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,    69,     0,     0,     0,     0,     0,    70,    71,
       0,    60,    61,     0,    72,     0,    74,    62,     0,    75,
       0,    76,     0,     4,     5,     6,     7,     8,     0,     0,
      66,   151,   152,   139,   140,   141,    14,    15,   142,   143,
     144,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,   145,    33,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,    66,    70,    71,     0,     0,     0,     0,    72,
       0,    74,     0,   484,    75,     0,    76,     0,     4,     5,
       6,     7,     8,    67,    68,     0,   151,   152,   139,   140,
     141,    14,    15,   142,   143,   144,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,    33,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,     0,     0,    55,    69,
       0,     0,     0,     0,     0,    70,    71,     0,    60,    61,
       0,    72,     0,    74,    62,   486,    75,   260,    76,     0,
       4,     5,     6,     7,     8,     0,     0,    66,   151,   152,
     139,   140,   141,    14,    15,   142,   143,   144,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   145,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,    74,     0,
       0,    75,     0,    76,     0,     4,     5,     6,     7,     8,
      67,    68,     0,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,    69,     0,     0,     0,
       0,     0,    70,    71,     0,    60,    61,     0,    72,     0,
      74,    62,   488,    75,     0,    76,     0,     4,     5,     6,
     683,     8,     0,     0,    66,   151,   152,   139,   140,   684,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,    74,   872,     0,    75,     0,
      76,     0,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   151,   152,   139,   140,   141,    14,    15,   142,   143,
     144,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,   145,    33,     0,    70,
      71,     0,     0,    39,     0,    72,     0,    74,   256,    41,
      75,     0,    76,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    66,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,   145,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,    66,    70,    71,     0,     0,     0,
       0,    72,     0,    74,  1177,     0,    75,     0,    76,     0,
       4,     5,     6,     7,     8,    67,    68,     0,   151,   152,
     139,   140,   141,    14,    15,   142,   143,   144,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   145,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,    69,     0,     0,     0,     0,     0,    70,    71,     0,
      60,    61,     0,    72,     0,   168,    62,     0,    75,     0,
      76,     0,     4,     5,     6,     7,     8,     0,     0,    66,
     151,   152,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,   145,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    66,    70,    71,     0,     0,     0,     0,    72,     0,
     170,     0,     0,    75,     0,    76,     0,     4,     5,     6,
       7,     8,    67,    68,     0,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,    69,     0,
       0,     0,     0,     0,    70,    71,     0,    60,    61,     0,
      72,     0,   172,    62,     0,    75,     0,    76,     0,     4,
       5,     6,     7,     8,     0,     0,    66,   151,   152,   139,
     140,   141,    14,    15,   142,   143,   144,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,   145,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,    66,    70,
      71,     0,     0,     0,     0,    72,     0,   174,     0,     0,
      75,     0,    76,     0,     4,     5,     6,     7,     8,    67,
      68,     0,   151,   152,   139,   140,   141,    14,    15,   142,
     143,   144,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   145,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,    69,     0,     0,     0,     0,
       0,    70,    71,     0,    60,    61,     0,    72,     0,    74,
      62,     0,    75,     0,    76,     0,     4,     5,     6,     7,
       8,     0,     0,    66,   151,   152,   139,   140,   141,    14,
      15,   142,   143,   144,    19,    20,    21,    22,    23,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,   145,
      33,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,    66,    70,    71,     0,     0,
       0,     0,    72,     0,   195,     0,     0,    75,     0,    76,
       0,     4,     5,     6,     7,     8,    67,    68,     0,   151,
     152,   139,   140,   141,    14,    15,   142,   143,   144,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   145,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,    69,     0,     0,     0,     0,     0,    70,    71,
       0,    60,    61,     0,    72,     0,   197,    62,     0,    75,
       0,    76,     0,     4,     5,     6,     7,     8,     0,     0,
      66,   151,   152,   139,   140,   141,    14,    15,   142,   143,
     144,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,   145,    33,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,    66,    70,    71,     0,     0,     0,     0,    72,
       0,    74,     0,     0,    75,     0,   211,     0,     4,     5,
       6,     7,     8,    67,    68,     0,   151,   152,   139,   140,
     141,    14,    15,   142,   143,   144,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   145,    33,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,     0,     0,    55,    69,
       0,     0,     0,     0,     0,    70,    71,     0,    60,    61,
       0,    72,     0,   358,    62,     0,    75,     0,    76,     0,
       4,     5,     6,   683,     8,     0,     0,    66,   151,   152,
     139,   140,   684,    14,    15,   142,   143,   144,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   145,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,   680,     0,
       0,    75,     0,    76,     0,     4,     5,     6,     7,     8,
      67,    68,     0,   151,   152,   139,   140,   141,    14,    15,
     142,   143,   144,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,    69,     0,     0,     0,
       0,     0,    70,    71,     0,    60,    61,     0,    72,     0,
     685,    62,     0,    75,     0,    76,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   151,   152,   139,   140,   141,
      14,    15,   142,   143,   144,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     145,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,   891,     0,     0,    75,     0,
      76,     0,     4,     5,     6,     7,     8,    67,    68,     0,
     137,   138,   139,   140,   141,    14,    15,   142,   143,   144,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,    69,     0,     0,     0,     0,     0,    70,
      71,     0,    60,    61,     0,    72,     0,   915,    62,     0,
      75,     0,    76,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -576,  -576,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,     0,    70,    71,     0,     0,     0,     0,
      72,  -576,    74,     0,     0,    75,     0,    76,  -576,  -576,
    -576,     0,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,     0,     0,
       0,  -576,     0,     0,     0,     0,     0,     0,     0,  -576,
    -576,     0,     0,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,     0,  -576,  -576,     0,     0,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,     0,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,     0,     0,  -576,
    -576,  -576,  -576,  -576,  -576,     0,  -576,  -576,  -576,     0,
       0,  -576,  -576,  -576,     0,  -576,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,     0,     0,     0,  -576,     0,     0,     0,     0,     0,
       0,     0,  -576,  -576,     0,     0,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,  -576,  -576,  -576,  -576,  -576,  -576,     0,  -576,  -576,
       0,     0,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
    -576,     0,  -576,  -576,  -576,  -576,  -576,  -576,  -576,  -576,
       0,     0,  -576,  -576,  -576,  -576,  -576,     0,     0,  -576,
    -576,  -576,     0,     0,   507
};

static const yytype_int16 yycheck[] =
{
       2,   273,     0,    31,   912,   167,   714,    19,    58,     2,
     642,     2,     2,    63,    64,    12,   648,    27,    20,    15,
      27,    15,    27,    15,    27,    27,    28,    29,    30,    31,
     444,     0,    34,    35,    36,    15,    31,    39,    40,    11,
      12,    43,    44,    15,   344,   416,   346,   186,    60,    51,
      52,   122,   123,    55,    56,    15,    51,    69,    70,    71,
      31,    27,  1121,    65,    31,    11,    12,    27,    31,    15,
      11,    12,    74,    75,    15,    11,    12,    15,    31,    15,
      51,    34,    35,   497,   384,   385,    31,    15,    51,    45,
      46,    44,    72,    27,    47,    48,    15,   168,    51,    52,
     181,    15,   183,    75,    76,    77,    51,    31,    36,   111,
      45,    46,   168,   115,   116,   117,   118,   119,   120,    35,
     122,   123,   124,   125,    92,   127,   182,    51,   130,    75,
      76,    77,   134,   135,    75,    76,    77,   168,   168,    75,
      76,    77,    15,   188,    11,    12,    13,    82,    15,   168,
      15,   182,   182,   155,    27,   157,    45,    46,     6,    11,
      12,   168,    27,    15,   183,   168,   168,    15,   170,    36,
     172,   190,   174,  1081,    41,   169,    11,    12,   188,    15,
      15,    11,    12,   188,   181,    15,   183,   182,    11,    12,
    1249,   183,    15,   195,   190,   197,   188,   829,   200,   110,
     202,   203,   168,   205,   206,    78,   208,   209,   210,   211,
     212,   182,   184,    78,   185,    15,   170,   185,   189,   182,
     188,   188,   185,    75,    76,    77,   189,    27,   188,   182,
     232,   169,    15,   189,   168,  1294,   189,   182,   184,  1298,
      75,    76,    77,   184,    15,    75,    76,    77,   184,    13,
     169,   301,    75,    76,    77,   169,    27,   168,   182,   261,
     262,   263,   264,   265,   266,   267,   268,   269,   261,   271,
     261,   261,   188,    89,    15,   186,    31,   439,    78,   170,
     188,   695,  1341,    11,   312,   313,   314,   315,   316,   317,
     318,  1350,    31,   113,   114,   168,    51,   188,    31,    92,
     189,    34,    35,   168,    89,   165,   166,    78,  1367,   182,
    1369,    44,    51,    31,    47,    48,   183,   182,    51,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   341,
     342,   343,   184,    51,    84,    85,    86,    31,     8,     9,
      10,   110,  1401,   345,  1403,    15,   137,   138,   168,   184,
     168,   722,    80,    81,   184,    31,   358,    51,   110,   773,
     774,   184,    66,    67,   182,   181,    70,   183,   168,  1277,
    1078,    31,   183,    31,    31,    51,   378,   188,   380,   381,
     382,    13,   182,   174,   386,   387,   388,   389,   390,   391,
     392,    51,   185,    51,    51,   188,   181,   168,   183,   168,
     771,   403,   404,   405,   182,   407,   408,   185,   410,   411,
      82,   182,    66,    67,   442,   443,    70,   186,   170,   421,
     422,   423,   424,   425,   426,   427,   188,   182,    31,   431,
     432,    34,    35,   168,   189,   170,   188,   183,   185,   189,
     187,    44,   188,   182,    47,    48,   185,   445,    51,   182,
     189,   168,   185,   188,   456,   457,   189,   459,   460,    31,
     462,   463,    34,    35,   182,   188,   468,   182,   470,   186,
     185,   189,    44,   475,   476,    47,    48,    15,   182,    51,
      52,   170,   484,    21,   486,   189,   488,    92,   182,   491,
      11,    92,   122,   123,    31,   189,     2,    34,    35,   188,
     502,   503,   504,   501,   113,   114,   182,    44,    82,   185,
      47,    48,   812,   813,    51,    52,    90,    91,   520,   521,
     522,   523,   182,   894,   182,   182,    32,    33,   182,   189,
      68,   189,   137,   138,   110,   189,   137,   138,   168,    45,
      46,   107,   108,    82,   122,   123,   168,    53,    54,   110,
     822,    90,    91,   924,    31,   926,   110,    34,    35,    65,
      11,   183,   110,   137,   138,   121,   110,    44,   190,   174,
      47,    48,    31,   174,    51,    34,    35,   133,    11,   182,
     113,   114,   185,    11,   182,    44,   189,   185,    47,    48,
     168,   189,    51,   170,   170,   161,   162,   163,   137,   138,
     174,   157,    31,   122,   123,    34,    35,   168,   979,   980,
     182,   188,   188,   182,   168,    44,   185,   189,    47,    48,
     168,   182,    51,    52,   168,   183,   122,   123,   182,   182,
     632,   633,   190,    34,   182,   174,   189,   183,   182,   145,
     170,   643,   113,   114,   190,   182,   648,    80,    81,   168,
     183,    52,   189,   183,    11,    12,   110,   111,    15,   113,
     114,   115,   116,   117,   118,   119,   168,   173,    89,   175,
     176,   673,   168,   973,   185,   122,   123,   188,   680,    93,
      94,    95,    96,   685,   186,   170,   688,   689,   106,   691,
     196,   693,   198,   113,   114,   697,   698,   168,   183,   182,
     183,  1072,   170,   170,   182,   182,    11,    12,   185,   120,
      15,   189,   189,   715,   168,   183,   183,  1088,    75,    76,
      77,   168,   724,   182,   726,   182,   185,   167,   185,   183,
     236,   724,   168,   724,   724,   185,   168,   187,   152,   153,
     154,   155,   156,   157,   158,   182,   160,   183,   168,   168,
     752,   183,   189,   182,   190,   261,   170,   171,   190,  1130,
    1131,    15,   764,   113,   114,   115,   116,   117,   118,   119,
      75,    76,    77,   181,   776,   113,   114,   115,   116,   117,
     118,   119,   181,   776,   182,   776,   776,   789,   790,   791,
     182,   189,   185,   182,   187,   797,   798,   189,   800,   801,
     189,   803,   804,   805,   806,   807,   808,   809,   810,   183,
     182,   182,   182,  1227,   188,   182,  1230,   189,   168,   189,
     183,   823,   189,    15,   182,   188,   828,  1127,   182,   182,
     168,   189,   185,   183,   182,   189,   838,   393,    15,   841,
     182,   189,   844,   182,   188,   183,   182,   189,   182,   851,
     189,   182,   182,   189,   856,   189,   170,   859,   189,   189,
     862,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     182,    11,   182,   429,   181,    15,   112,   189,   434,   189,
     182,   883,   884,   182,   440,   182,   186,   189,   185,   891,
     189,   132,   133,   134,   135,   136,   898,   182,   182,   455,
     186,   182,    15,   182,   189,   189,  1320,  1321,   189,   465,
     189,   121,   181,   915,   124,   125,   126,   127,   128,   129,
     130,   131,   182,   182,   182,   927,   928,   929,   164,   189,
     189,   189,   182,   935,   175,   176,   185,    15,   187,   189,
     139,   140,   141,   142,   143,   144,   185,   182,   187,    27,
     952,    29,    30,   168,   189,   511,   182,   513,   514,   182,
      38,    39,   964,   189,    79,    43,   189,   177,   178,   179,
     182,    49,    50,   172,   173,   168,   182,   189,   182,    57,
      58,    59,    60,   189,   490,   189,   492,   182,   182,   272,
      68,   274,   275,   276,   189,   189,   998,   999,  1000,  1001,
    1002,  1003,   303,   304,   305,   306,   307,   308,   309,   310,
     311,   182,     3,     4,     5,     6,     7,   185,   189,   187,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,   182,   182,    11,    12,    13,
     182,   182,   189,   189,  1046,    36,    37,   189,   189,   182,
     182,    11,    43,   182,  1056,    15,   189,   189,    49,   182,
     189,   185,   185,   187,   620,   183,    57,    58,    59,    60,
     188,    11,    63,   185,    14,   631,    16,    17,   634,   185,
     183,   187,    73,    74,   640,   188,   642,   182,    79,   188,
     185,    62,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,
     183,    92,   184,   185,  1106,   188,    15,   324,   325,   326,
     327,   328,   329,   330,  1116,   182,   182,   188,    27,   185,
      29,    30,   113,   114,   182,   182,   188,   185,   185,    38,
      39,   183,    11,  1135,    43,    14,    15,    16,    17,   188,
      49,    50,   146,   147,   700,   182,   150,   151,    57,    58,
      59,    60,   183,   709,   182,  1157,  1158,   185,   182,    68,
     182,   185,   189,   185,   319,   320,   321,   184,   185,  1171,
    1172,   182,   175,   176,   184,   185,   184,   185,   169,   171,
    1182,  1183,   184,   185,   175,   176,   184,   185,   184,   185,
     181,   182,   183,    32,    33,   186,   182,   188,   189,  1201,
    1202,  1203,   184,   185,   184,   185,   182,   713,   714,   184,
     185,   717,   718,   719,   184,   185,  1218,   189,   724,   184,
     185,   182,  1224,    15,   730,   184,   185,   184,   185,   183,
    1228,   184,   185,  1235,   184,   185,   182,    15,  1240,   182,
     746,   747,   748,    21,   182,   184,   185,   184,   185,    27,
     182,    29,    30,   182,  1256,  1257,   762,   184,   185,   765,
      38,    39,   184,   185,   183,    43,   184,   185,   824,   825,
     776,    49,    50,   829,   830,   122,   123,   833,   182,    57,
      58,    59,    60,   184,   185,   184,   185,    15,   122,   123,
     122,   123,   122,   123,   187,  1297,   184,   185,   189,    27,
     171,    29,    30,    15,  1297,    15,  1297,  1297,   182,  1311,
      38,    39,   184,   185,  1316,    43,   184,   185,   184,   185,
     182,    49,    50,   184,   185,   881,    32,    33,    15,    57,
      58,    59,    60,   184,   185,   168,  1338,   168,   184,   185,
     184,   185,  1344,  1345,  1346,   182,   184,   185,   184,   185,
      32,    33,  1354,    15,  1356,   184,   185,   184,   185,   184,
     185,   184,   185,   184,   185,    32,    33,   184,   185,   184,
     185,   184,   185,   184,   185,  1377,    50,    51,   322,   323,
    1382,  1383,  1384,  1385,  1386,  1387,  1388,  1389,    15,    32,
      11,   182,   182,   182,   182,   182,   189,   183,   183,    15,
     182,    11,   181,    15,  1406,  1407,  1408,   181,   109,   182,
     916,   182,   182,   182,   182,     3,     4,     5,     6,     7,
     182,   182,   182,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,   164,   182,
     171,   168,   189,   168,   189,   187,   182,   953,    36,    37,
     171,    15,   182,   182,    89,    43,    15,   168,   182,   182,
     182,    49,   968,   182,   182,   182,   182,   182,   182,    57,
      58,    59,    60,   182,   182,    63,    11,   182,    11,   182,
     182,   185,   182,   182,   182,    73,    74,   182,   171,   182,
     171,    79,   189,   189,   182,   168,    15,   189,    15,   188,
     171,   187,  1058,   182,    92,   182,   182,   182,    89,   187,
      15,   171,   184,  1069,    15,    11,   182,   182,   182,   182,
      11,   182,    11,    15,   182,   113,   114,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   294,   295,   296,   297,   298,   299,   300,
      15,   182,    32,   182,    66,   189,    15,    15,    15,   189,
     182,   184,   182,   182,   182,   182,    15,   182,    89,   183,
      15,   182,  1078,   182,   182,   182,  1082,  1083,  1084,    89,
     182,   169,   182,    15,    11,    11,   189,   175,   176,   182,
     182,   182,     8,   181,   182,   183,     8,     8,   186,   168,
     188,   171,   190,   182,  1110,    15,   184,   184,  1114,    82,
      15,    15,    15,    15,   182,   182,   182,   182,   189,    15,
     183,   182,   184,    15,   184,   182,   182,   182,  1184,   182,
     188,   182,  1188,   182,   182,     3,     4,     5,     6,     7,
      15,    15,   182,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,   182,   182,
     182,  1338,   182,    33,   182,   926,   182,   182,    36,    37,
     184,   182,   769,   182,   302,    43,   685,    -1,   277,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,  1193,  1194,    57,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1293,    -1,    -1,
      -1,  1297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,    -1,   186,    -1,
     188,    -1,   190,    -1,    -1,    -1,    -1,  1333,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1404,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    92,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      92,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,
     184,    -1,   186,    -1,   188,    -1,   190,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,   181,
      -1,   183,   184,    49,   186,    -1,   188,    -1,   190,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    92,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,
     176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    -1,
     186,   187,   188,    -1,   190,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    36,    37,
      -1,   175,   176,    -1,    -1,    43,    -1,   181,    -1,   183,
     184,    49,   186,    -1,   188,    -1,   190,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    92,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,    -1,   186,    -1,
     188,    -1,   190,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,   175,
     176,    -1,    43,    -1,    -1,   181,    -1,   183,    49,    -1,
     186,   187,   188,    -1,   190,    -1,    57,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    92,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,
     181,    -1,   183,   184,    -1,   186,    -1,   188,    -1,   190,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    36,    37,    -1,   175,   176,    -1,    -1,
      43,    -1,   181,    -1,   183,   184,    49,   186,    -1,   188,
      -1,   190,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    92,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,
     183,   184,    -1,   186,    -1,   188,    -1,   190,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,
     181,    -1,   183,   184,    49,   186,    -1,   188,    -1,   190,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    92,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,
     175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,   184,
      -1,   186,    -1,   188,    -1,   190,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    36,    37,
      -1,    -1,   175,   176,    -1,    43,    -1,    -1,   181,    -1,
     183,    49,    -1,   186,    -1,   188,    -1,   190,    -1,    57,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    92,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,    -1,    -1,   186,    -1,
     188,    -1,   190,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    -1,
     186,    -1,   188,    -1,   190,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    -1,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      -1,    69,    70,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      -1,    -1,    -1,   181,   182,   183,    -1,    -1,   186,    -1,
     188,   189,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    -1,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    -1,    -1,    -1,    -1,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    -1,    -1,    69,    70,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    83,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,
     181,   182,   183,    -1,    -1,   186,    -1,   188,   189,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    -1,    -1,    -1,    -1,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    69,    70,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      -1,   175,   176,    -1,    -1,    -1,    -1,   181,   182,   183,
      -1,    -1,   186,    -1,   188,   189,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,   113,   114,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    73,    74,    -1,   181,   182,   183,    79,    -1,   186,
      -1,   188,   189,     3,     4,     5,     6,     7,    -1,    -1,
      92,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,
     182,   183,    -1,    -1,   186,    -1,   188,   189,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,
      -1,   181,   182,   183,    -1,    -1,   186,    -1,   188,   189,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    -1,    -1,    -1,    -1,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    -1,    -1,    -1,    69,    70,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      83,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
     113,   114,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    73,    74,    -1,   181,   182,
     183,    79,    -1,   186,    -1,   188,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    92,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,    -1,    -1,   186,   187,
     188,    -1,     3,     4,     5,     6,     7,   113,   114,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,   169,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    73,    74,    -1,   181,   182,   183,    79,    -1,
     186,    -1,   188,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    92,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,
     181,   182,   183,    -1,    -1,   186,    -1,   188,    -1,     3,
       4,     5,     6,     7,   113,   114,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
     169,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    73,
      74,    -1,   181,   182,   183,    79,    -1,   186,    -1,   188,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    92,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      92,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,
      -1,    -1,   186,    -1,   188,    -1,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,   181,
      -1,   183,   184,    49,   186,    -1,   188,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    92,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,
     176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    -1,
     186,   187,   188,    -1,     3,     4,     5,     6,     7,   113,
     114,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,    -1,
      -1,   175,   176,   177,    73,    74,    -1,   181,    -1,   183,
      79,    -1,   186,    -1,   188,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    92,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    -1,    -1,   113,   114,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,    -1,   186,    -1,   188,
      -1,     3,     4,     5,     6,     7,   113,   114,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    73,    74,    -1,   181,    -1,   183,    79,    -1,   186,
      -1,   188,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      92,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,
      -1,   183,    -1,   185,   186,    -1,   188,    -1,     3,     4,
       5,     6,     7,   113,   114,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,   169,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    73,    74,
      -1,   181,    -1,   183,    79,   185,   186,    82,   188,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    92,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,
     175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,
      -1,   186,    -1,   188,    -1,     3,     4,     5,     6,     7,
     113,   114,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    73,    74,    -1,   181,    -1,
     183,    79,   185,   186,    -1,   188,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    92,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,   184,    -1,   186,    -1,
     188,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    36,    37,    -1,   175,
     176,    -1,    -1,    43,    -1,   181,    -1,   183,   184,    49,
     186,    -1,   188,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    92,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,    92,   175,   176,    -1,    -1,    -1,
      -1,   181,    -1,   183,   184,    -1,   186,    -1,   188,    -1,
       3,     4,     5,     6,     7,   113,   114,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,
      73,    74,    -1,   181,    -1,   183,    79,    -1,   186,    -1,
     188,    -1,     3,     4,     5,     6,     7,    -1,    -1,    92,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,
      -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,
     183,    -1,    -1,   186,    -1,   188,    -1,     3,     4,     5,
       6,     7,   113,   114,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,   169,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    73,    74,    -1,
     181,    -1,   183,    79,    -1,   186,    -1,   188,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    92,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,
     176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    -1,
     186,    -1,   188,    -1,     3,     4,     5,     6,     7,   113,
     114,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,    -1,
      -1,   175,   176,    -1,    73,    74,    -1,   181,    -1,   183,
      79,    -1,   186,    -1,   188,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    92,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,    -1,
      -1,    -1,   181,    -1,   183,    -1,    -1,   186,    -1,   188,
      -1,     3,     4,     5,     6,     7,   113,   114,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    73,    74,    -1,   181,    -1,   183,    79,    -1,   186,
      -1,   188,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      92,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,
      -1,   183,    -1,    -1,   186,    -1,   188,    -1,     3,     4,
       5,     6,     7,   113,   114,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,   169,
      -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    73,    74,
      -1,   181,    -1,   183,    79,    -1,   186,    -1,   188,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    92,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    92,
     175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,
      -1,   186,    -1,   188,    -1,     3,     4,     5,     6,     7,
     113,   114,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,
      58,    59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,
      -1,    -1,   175,   176,    -1,    73,    74,    -1,   181,    -1,
     183,    79,    -1,   186,    -1,   188,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    92,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,
      -1,    -1,    -1,   181,    -1,   183,    -1,    -1,   186,    -1,
     188,    -1,     3,     4,     5,     6,     7,   113,   114,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,
      -1,    -1,    63,   169,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    73,    74,    -1,   181,    -1,   183,    79,    -1,
     186,    -1,   188,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    -1,    -1,    -1,   175,   176,    -1,    -1,    -1,    -1,
     181,    82,   183,    -1,    -1,   186,    -1,   188,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,    -1,    -1,
      -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,
     121,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,    -1,   146,   147,    -1,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,    -1,   170,
     171,   172,   173,   174,    82,    -1,   177,   178,   179,    -1,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,    -1,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,   121,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,   147,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,   160,   161,   162,   163,   164,   165,   166,   167,
      -1,    -1,   170,   171,   172,   173,   174,    -1,    -1,   177,
     178,   179,    -1,    -1,   182
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   192,   193,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    69,    70,
      73,    74,    79,    83,    87,    88,    92,   113,   114,   169,
     175,   176,   181,   182,   183,   186,   188,   194,   195,   197,
     198,   199,   200,   201,   203,   204,   208,   212,   213,   214,
     217,   222,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   241,   242,   244,
     245,   110,   111,   113,   114,   115,   116,   117,   118,   119,
     168,   183,   110,   168,   186,   110,   170,   188,   113,   114,
     168,    12,   181,   183,   186,   188,   170,    11,    12,    13,
      14,    15,    18,    19,    20,    36,   222,   237,   242,   244,
     245,    11,    12,   187,   224,    15,   182,   183,   190,   224,
     243,    11,    12,    13,    15,    36,    41,   183,   183,   224,
     183,   224,   183,   224,   183,   224,   224,   233,    15,   183,
     222,   203,   222,   224,   182,   224,   182,   224,    11,    15,
      15,   224,   224,    13,    13,   183,   224,   183,   224,   222,
     183,   222,    31,    51,   182,    31,    51,   182,   188,   223,
     223,   188,   223,   224,   224,   222,   188,   222,   224,   182,
     224,    15,   211,   211,    15,    36,   237,   211,   211,    11,
      12,    13,    29,   222,   224,    11,   239,    11,    11,   237,
     237,   237,    15,    27,    29,    30,    38,    39,    43,    49,
      50,    57,    58,    59,    60,   240,   184,   224,   187,   224,
      82,   193,    31,    34,    35,    44,    47,    48,    51,    52,
     182,    89,   107,   108,   161,   162,   163,   106,    93,    94,
      95,    96,   152,   153,   154,   155,   156,   157,   158,   160,
     170,   171,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   112,   164,   139,   140,   141,   142,   143,   144,   172,
     173,   120,   146,   147,   150,   151,    82,    90,    91,   137,
     138,   174,   165,   166,   132,   133,   134,   135,   136,   175,
     176,   121,   124,   125,   126,   127,   128,   129,   130,   131,
     177,   178,   179,   167,   181,   168,   181,   224,    15,   224,
     224,   224,   224,   224,   224,   184,   243,   224,   183,   224,
     177,   224,   224,    15,   224,   224,   182,    15,    21,    68,
      15,   184,   190,   219,   243,   224,   224,    15,   186,   170,
     188,   186,   188,   170,   181,   181,   115,   116,   117,   118,
     119,   168,   168,   185,   187,   182,   224,   184,   224,   243,
      15,   169,   182,   185,   110,   168,   182,   110,   168,   182,
     110,   168,   182,    11,    12,    13,    15,    11,    15,   239,
     224,    31,    34,    35,    44,    47,    48,    51,   182,   185,
     224,    31,    51,   182,   185,   224,   222,   224,   222,    92,
     185,   222,    90,    91,   183,   188,   209,    11,    12,    15,
      75,    76,    77,   221,   182,   185,    31,    51,   182,    31,
      51,   182,   168,   168,    79,   185,   188,   170,   188,   170,
     188,   224,   222,   224,   222,    34,    52,    28,   224,   224,
     224,   224,   224,   224,   185,   224,   185,   224,   185,   224,
      62,    45,    46,   205,   182,   215,   188,   183,    15,    72,
     188,   188,   168,   168,   168,   170,   224,   182,   182,   185,
     222,   183,   184,   185,   185,   187,   189,    12,    13,    14,
      29,    30,    38,    39,    55,    56,   189,   224,   242,   245,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   227,
     225,   227,   227,   227,   228,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,     8,     9,
      10,   211,   230,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   235,   235,   236,   236,   236,   236,   236,   236,
     236,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   240,   224,    21,   240,   182,   182,
     183,   182,   182,   182,   182,   182,   182,   184,   182,   224,
     182,   185,   175,   176,   185,   187,   182,   171,   189,   182,
     183,    15,   183,    89,    15,   184,   185,   184,   185,   187,
     189,   171,   224,    15,   224,   224,   224,    15,   240,   240,
     224,   224,   224,   224,   224,   224,   224,   187,   243,   182,
     182,   184,   170,   183,    15,   190,   224,   224,   224,   224,
     183,   224,   224,     6,    15,   183,   220,   224,   110,   168,
     182,   168,   182,   168,   182,   183,   209,   168,   168,   184,
     185,   224,   224,   224,   224,   224,   224,   224,   243,   185,
     224,   224,   243,   184,    32,    33,   196,   184,   239,   243,
     233,   233,   184,   221,   193,    27,   168,    11,    80,    81,
     184,   185,   243,   224,   224,   224,   224,   224,   224,   182,
     243,   206,    15,   224,    15,   224,   184,    32,   184,   224,
     224,    11,   182,   182,   182,   182,   182,   189,   224,   224,
     224,   222,   224,   222,    45,    46,   189,    15,    27,   188,
     216,   184,   221,   183,   183,   218,   193,   224,   224,   224,
      15,   182,    11,   184,   243,   184,   243,   187,   243,   186,
     188,   168,   224,   224,   224,   189,   224,    31,    51,   189,
      31,    51,   189,    31,    34,    35,    44,    47,    48,    51,
      52,   189,   181,   181,   182,   182,   182,   182,   182,   182,
     182,   182,   109,   168,   183,   190,   182,    15,   168,   183,
     190,   184,   243,   185,   243,   224,   224,   243,   168,   122,
     123,   168,   122,   123,   168,   184,   243,   184,   219,   243,
     224,   183,    15,   190,   190,   219,   168,   122,   123,   168,
     122,   123,   168,   187,   171,   189,   187,   189,   171,   187,
     182,    15,   184,   224,    15,   169,   182,   182,   182,   224,
     182,   185,   182,    89,    89,   220,   182,   185,   182,   224,
     224,   183,   224,   224,   184,   221,   224,   224,   168,   243,
     182,   182,   182,   182,   182,   182,   182,   182,   243,   182,
     182,   182,   222,   195,   222,   183,   224,   222,   222,    92,
     222,    92,    92,    27,   207,   209,   184,    29,    30,    39,
     189,   224,    15,   224,    27,   168,    11,    11,   222,    11,
      12,    15,    75,    76,    77,   182,   182,   182,   182,   182,
     182,   182,    45,    46,   189,   171,   189,   171,   189,   222,
     222,   222,   182,   182,   168,   224,   222,   189,   224,   222,
      15,    15,   216,    66,    67,    70,   182,   189,   209,   184,
     184,   221,   221,    84,    85,    86,   189,   189,   182,   182,
     182,   171,   184,   184,   187,   224,   224,   224,    31,    35,
      44,    47,    48,    51,   189,   189,   189,   189,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     240,   240,   225,   224,   184,   243,   243,   224,   184,   219,
     243,   243,   182,   184,   243,   182,   187,   187,   187,   224,
     224,   224,   184,   184,   184,   224,    89,    15,    15,   184,
     224,   224,   224,   171,   184,   170,   183,    15,   185,   243,
     224,   224,   184,     6,    15,   182,   182,   224,   182,   185,
     182,   209,   184,   182,   182,   224,   184,   184,    32,   196,
     224,   222,    11,    11,    11,    15,    27,   209,   207,   209,
     224,   224,   189,   224,    31,    34,    35,    44,    47,    48,
      51,    52,   189,    15,   224,    27,   168,    11,    80,    81,
     224,   222,   182,   182,    32,   224,   182,   222,   189,   189,
      68,   240,    11,    14,    15,    16,    17,    66,   209,   209,
     184,   184,    15,    15,    15,   168,   187,   189,   189,   224,
     224,   224,   224,   224,   224,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   168,   168,   182,
     184,   182,   184,   184,   182,   184,   182,   182,   182,   184,
     224,   183,   183,   182,   182,   182,    15,   184,   224,   243,
     182,   182,    89,    89,   185,   243,   209,   182,   185,   182,
     182,   195,   222,   184,    32,   196,   222,   222,   222,    15,
     209,    31,    35,    51,   189,   189,   189,   224,   224,   224,
     224,   224,   224,   224,   224,    15,   224,    27,   168,    11,
      11,   222,   189,   222,   182,   202,   224,   183,   188,   210,
     183,   210,    15,    27,    78,   168,   182,    15,    27,    78,
     168,   182,    11,    14,    16,    17,   182,   182,    68,   240,
     209,   209,     8,     8,     8,   224,   168,   168,   189,   189,
     189,   189,   189,   189,   224,   224,   182,   224,   224,   171,
     184,   184,   224,   224,   243,   182,   243,   222,   222,   224,
     224,   224,   189,   189,   189,   189,   189,   189,   189,   189,
      15,   224,   224,   184,   184,   221,    82,   193,   184,   221,
      15,    15,   182,   224,    15,    15,   182,   224,    15,    27,
      78,   168,   182,    15,    27,    78,   168,   182,   182,   182,
     183,   183,   210,   182,   224,   224,   189,   189,   184,   184,
     182,   184,   182,    32,   196,   189,   189,   189,   182,   222,
     210,   184,   189,    14,    29,    30,    39,   189,   224,   210,
     184,   182,   182,   182,   168,   182,   168,   182,   182,    15,
      15,   182,   224,    15,    15,   182,   224,   184,   221,   184,
     221,   189,   189,   182,   222,   202,   210,   168,   224,   224,
     189,   224,    31,    34,    35,    44,    47,    48,    51,    52,
     189,   210,   224,   224,   182,   182,   182,   182,   182,   182,
     210,   184,   210,   184,   184,   224,    31,    35,    51,   189,
     189,   189,   224,   224,   224,   224,   224,   224,   224,   224,
     182,   182,   210,   210,   222,   189,   224,   224,   224,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   189
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   191,   192,   193,   193,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   194,   194,   194,   194,   194,   194,   194,   194,   195,
     195,   195,   195,   195,   195,   195,   195,   196,   196,   196,
     196,   196,   196,   197,   197,   198,   198,   198,   198,   199,
     199,   200,   200,   200,   201,   201,   201,   202,   203,   203,
     203,   203,   203,   203,   204,   204,   205,   205,   206,   206,
     207,   207,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   208,   208,   208,   208,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   211,   212,   213,   214,   215,   215,   215,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   217,   218,   218,
     218,   218,   219,   219,   219,   219,   219,   220,   220,   220,
     220,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   223,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     225,   225,   226,   226,   226,   226,   226,   227,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   229,   229,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     233,   233,   234,   234,   234,   235,   235,   235,   235,   235,
     235,   235,   235,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   237,   237,   237,   237,
     237,   237,   238,   238,   239,   239,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   241,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   244,   244,   244,   244,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245
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
       6,     6,     4,     7,     7,     7,     7,     7,     7,     6,
       6,     4,     4,     4,     4,     4,     4,     4,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     4,     4,     2,     1,     1,     1,     1,     1,     2,
       4,     4,     6,     5,     2,     1,     2,     3,     3,     5,
       5,     5,     8,     4,     1,     1,     1,     1,     2,     2,
       4,     4,     4,     4,     1,     1,     1,     1,     1,     5,
       7,     7,     3,     5,     5,     4,     6,     3,     5,     5,
       7,     4,     6,     5,     3,     5,     7,     3,     5,     5,
       3,     2,     5,     5,     2,    12,     9,     1,     7,     7,
       5,     7,     5,     3,     5,     7,     3,     4,     0,     4,
       2,     3,     6,     5,     3,     7,     6,     7,     6,     4,
       7,     6,     7,     6,     5,     3,     4,     5,     4,     5,
       5,     6,     6,     6,     6,     6,     6,     6,     6,     7,
       7,     7,     3,     3,     6,     4,     5,     4,     5,     5,
       6,     6,     6,     6,     6,     6,     6,     6,     7,     7,
       7,     1,     6,     5,     5,     0,     3,     3,     0,     2,
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
       4,     3,     4,     2,     3,     4,     2,     1,     1,     3,
       2,     2,     3,     5,     4,     5,     4,     5,     7,     6,
       7,     2,     3,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     4,     6,     6,     6,     1,     4,     4,
       4,     4,     5,     5,     5,     5,     4,     5,     5,     5,
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
#line 755 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 4275 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 765 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 4281 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 766 "raku.y"
                     { if ((yyvsp[0].node) && (yyvsp[0].node)->line == 0) (yyvsp[0].node)->line = (yylsp[0]).first_line; (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 4287 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 770 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 4293 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 772 "raku.y"
        { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), nul); }
#line 4299 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 774 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4305 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 776 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4311 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 778 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4317 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 780 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4324 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 783 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4330 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 785 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4336 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 787 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4342 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 789 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4348 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 791 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4356 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 795 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4362 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 797 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4368 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 799 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4374 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 801 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4380 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 803 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4388 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 807 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4396 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 811 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4402 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 813 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4410 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 817 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4418 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 821 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4424 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 823 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4430 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 825 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4436 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4442 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 829 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4448 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 831 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4456 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 835 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4464 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4470 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 841 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4476 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 843 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4482 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 845 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4488 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 847 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4494 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 849 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4500 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 851 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4506 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 853 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4512 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 855 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4518 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 857 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4524 "raku.tab.c"
    break;

  case 42: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 859 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4536 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 867 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4542 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 869 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4548 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 871 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4554 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 873 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4560 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 875 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4566 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 877 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4572 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 879 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4579 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 882 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4586 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 885 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4592 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 887 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4599 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 890 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4606 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 893 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4612 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 895 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4620 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 899 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4626 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 901 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4632 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 903 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4638 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 905 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4644 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 907 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4651 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 910 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4658 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 913 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4664 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 915 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4671 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 918 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4678 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 921 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4685 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 924 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4691 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 926 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-6].sval)),mc); }
#line 4700 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 931 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-5].sval)),mc); }
#line 4708 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 935 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-3].sval)),mc); }
#line 4716 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 939 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4723 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 942 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4730 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 945 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4738 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 949 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4745 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_SCALAR_IDX '[' expr ']' '=' expr ';'  */
#line 952 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4752 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_SCALAR_ANGLE '<' IDENT '>' '=' expr ';'  */
#line 955 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4759 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_SCALAR_HASH '{' expr '}' '=' expr ';'  */
#line 958 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4766 "raku.tab.c"
    break;

  case 77: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 961 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4773 "raku.tab.c"
    break;

  case 78: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 964 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4780 "raku.tab.c"
    break;

  case 79: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 967 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4787 "raku.tab.c"
    break;

  case 80: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 970 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4794 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_IF expr ';'  */
#line 973 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4800 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNLESS expr ';'  */
#line 975 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4806 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WHILE expr ';'  */
#line 977 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4812 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_UNTIL expr ';'  */
#line 979 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4818 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_FOR expr ';'  */
#line 981 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4825 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_WITH expr ';'  */
#line 984 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4831 "raku.tab.c"
    break;

  case 87: /* stmt: expr KW_WITHOUT expr ';'  */
#line 986 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4837 "raku.tab.c"
    break;

  case 88: /* stmt: expr KW_GIVEN expr ';'  */
#line 988 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4843 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 990 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4850 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 993 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4857 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 996 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4865 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 1000 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4872 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 1003 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4878 "raku.tab.c"
    break;

  case 94: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 1005 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4884 "raku.tab.c"
    break;

  case 95: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 1007 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4890 "raku.tab.c"
    break;

  case 96: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 1009 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4897 "raku.tab.c"
    break;

  case 97: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 1012 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4904 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 1015 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4911 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 1018 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4918 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 1021 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4925 "raku.tab.c"
    break;

  case 101: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 1024 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4932 "raku.tab.c"
    break;

  case 102: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1027 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4939 "raku.tab.c"
    break;

  case 103: /* stmt: expr ';'  */
#line 1029 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4945 "raku.tab.c"
    break;

  case 104: /* stmt: ';'  */
#line 1030 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4951 "raku.tab.c"
    break;

  case 105: /* stmt: if_stmt  */
#line 1031 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4957 "raku.tab.c"
    break;

  case 106: /* stmt: while_stmt  */
#line 1032 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4963 "raku.tab.c"
    break;

  case 107: /* stmt: for_stmt  */
#line 1033 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4969 "raku.tab.c"
    break;

  case 108: /* stmt: given_stmt  */
#line 1034 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4975 "raku.tab.c"
    break;

  case 109: /* stmt: KW_TRY block  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4981 "raku.tab.c"
    break;

  case 110: /* stmt: KW_TRY block KW_CATCH block  */
#line 1038 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4987 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1040 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4995 "raku.tab.c"
    break;

  case 112: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1044 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 5004 "raku.tab.c"
    break;

  case 113: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1049 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 5012 "raku.tab.c"
    break;

  case 114: /* stmt: KW_CATCH block  */
#line 1053 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5018 "raku.tab.c"
    break;

  case 115: /* stmt: block  */
#line 1055 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 5024 "raku.tab.c"
    break;

  case 116: /* stmt: PHASER block  */
#line 1057 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 5030 "raku.tab.c"
    break;

  case 117: /* stmt: PHASER block ';'  */
#line 1059 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 5036 "raku.tab.c"
    break;

  case 118: /* stmt: PHASER expr ';'  */
#line 1061 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),seq1((yyvsp[-1].node))); ct_drop((yyvsp[-2].sval)); }
#line 5042 "raku.tab.c"
    break;

  case 119: /* stmt: PHASER VAR_SCALAR '=' expr ';'  */
#line 1063 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5048 "raku.tab.c"
    break;

  case 120: /* stmt: PHASER VAR_ARRAY '=' expr ';'  */
#line 1065 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_arr_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5054 "raku.tab.c"
    break;

  case 121: /* stmt: PHASER VAR_HASH '=' expr ';'  */
#line 1067 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)))); ct_drop((yyvsp[-4].sval)); }
#line 5060 "raku.tab.c"
    break;

  case 122: /* stmt: PHASER VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 1069 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node));
          (yyval.node)=rk_phaser_mark((yyvsp[-7].sval),seq1(c)); ct_drop((yyvsp[-7].sval)); }
#line 5067 "raku.tab.c"
    break;

  case 123: /* stmt: PHASER KW_SAY expr ';'  */
#line 1072 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); (yyval.node)=rk_phaser_mark((yyvsp[-3].sval),seq1(s)); ct_drop((yyvsp[-3].sval)); }
#line 5073 "raku.tab.c"
    break;

  case 124: /* stmt: unless_stmt  */
#line 1073 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5079 "raku.tab.c"
    break;

  case 125: /* stmt: until_stmt  */
#line 1074 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5085 "raku.tab.c"
    break;

  case 126: /* stmt: repeat_stmt  */
#line 1075 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5091 "raku.tab.c"
    break;

  case 127: /* stmt: loop_stmt  */
#line 1076 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5097 "raku.tab.c"
    break;

  case 128: /* stmt: KW_LAST ';'  */
#line 1077 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 5103 "raku.tab.c"
    break;

  case 129: /* stmt: KW_NEXT ';'  */
#line 1078 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 5109 "raku.tab.c"
    break;

  case 130: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5115 "raku.tab.c"
    break;

  case 131: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5121 "raku.tab.c"
    break;

  case 132: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1084 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5127 "raku.tab.c"
    break;

  case 133: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1086 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5133 "raku.tab.c"
    break;

  case 134: /* stmt: sub_decl  */
#line 1087 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5139 "raku.tab.c"
    break;

  case 135: /* stmt: class_decl  */
#line 1088 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5145 "raku.tab.c"
    break;

  case 136: /* stmt: role_decl  */
#line 1089 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5151 "raku.tab.c"
    break;

  case 137: /* stmt: grammar_decl  */
#line 1090 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5157 "raku.tab.c"
    break;

  case 138: /* stmt: module_decl  */
#line 1091 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5163 "raku.tab.c"
    break;

  case 139: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1095 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5169 "raku.tab.c"
    break;

  case 140: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1097 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5175 "raku.tab.c"
    break;

  case 141: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1099 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5181 "raku.tab.c"
    break;

  case 142: /* if_stmt: KW_IF expr block  */
#line 1101 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5187 "raku.tab.c"
    break;

  case 143: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1103 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5193 "raku.tab.c"
    break;

  case 144: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1105 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5199 "raku.tab.c"
    break;

  case 145: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1107 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5205 "raku.tab.c"
    break;

  case 146: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1109 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5211 "raku.tab.c"
    break;

  case 147: /* elsif_tail: KW_ELSIF expr block  */
#line 1113 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5217 "raku.tab.c"
    break;

  case 148: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1115 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5223 "raku.tab.c"
    break;

  case 149: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1117 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5229 "raku.tab.c"
    break;

  case 150: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1119 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5235 "raku.tab.c"
    break;

  case 151: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1121 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5241 "raku.tab.c"
    break;

  case 152: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5247 "raku.tab.c"
    break;

  case 153: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1127 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5253 "raku.tab.c"
    break;

  case 154: /* while_stmt: KW_WHILE expr block  */
#line 1129 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5259 "raku.tab.c"
    break;

  case 155: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1133 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5265 "raku.tab.c"
    break;

  case 156: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1135 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5271 "raku.tab.c"
    break;

  case 157: /* unless_stmt: KW_UNLESS expr block  */
#line 1137 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5277 "raku.tab.c"
    break;

  case 158: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1139 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5283 "raku.tab.c"
    break;

  case 159: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1143 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5289 "raku.tab.c"
    break;

  case 160: /* until_stmt: KW_UNTIL expr block  */
#line 1145 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5295 "raku.tab.c"
    break;

  case 161: /* repeat_stmt: KW_REPEAT block  */
#line 1149 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5301 "raku.tab.c"
    break;

  case 162: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1151 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5307 "raku.tab.c"
    break;

  case 163: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1153 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5313 "raku.tab.c"
    break;

  case 164: /* loop_stmt: KW_LOOP block  */
#line 1157 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5319 "raku.tab.c"
    break;

  case 165: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1159 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5325 "raku.tab.c"
    break;

  case 166: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1161 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5331 "raku.tab.c"
    break;

  case 167: /* loop_incr: expr  */
#line 1164 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 5337 "raku.tab.c"
    break;

  case 168: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1168 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5347 "raku.tab.c"
    break;

  case 169: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1174 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5357 "raku.tab.c"
    break;

  case 170: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1180 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5369 "raku.tab.c"
    break;

  case 171: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1188 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5379 "raku.tab.c"
    break;

  case 172: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1194 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5388 "raku.tab.c"
    break;

  case 173: /* for_stmt: KW_FOR expr block  */
#line 1199 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5395 "raku.tab.c"
    break;

  case 174: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1204 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 5407 "raku.tab.c"
    break;

  case 175: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1212 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 5420 "raku.tab.c"
    break;

  case 176: /* catch_when_list: KW_WHEN expr block  */
#line 1223 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5426 "raku.tab.c"
    break;

  case 177: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1225 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5432 "raku.tab.c"
    break;

  case 178: /* when_list: %empty  */
#line 1228 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5438 "raku.tab.c"
    break;

  case 179: /* when_list: when_list KW_WHEN expr block  */
#line 1230 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5446 "raku.tab.c"
    break;

  case 180: /* sub_trait_list: TESTOP IDENT  */
#line 1235 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5452 "raku.tab.c"
    break;

  case 181: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1236 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5458 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1240 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5470 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1248 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5480 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1254 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5490 "raku.tab.c"
    break;

  case 185: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1260 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5502 "raku.tab.c"
    break;

  case 186: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1268 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5512 "raku.tab.c"
    break;

  case 187: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1274 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5524 "raku.tab.c"
    break;

  case 188: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1282 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5534 "raku.tab.c"
    break;

  case 189: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1288 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5544 "raku.tab.c"
    break;

  case 190: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1294 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5557 "raku.tab.c"
    break;

  case 191: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1303 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5568 "raku.tab.c"
    break;

  case 192: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1310 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5581 "raku.tab.c"
    break;

  case 193: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1319 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5594 "raku.tab.c"
    break;

  case 194: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1328 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5605 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list '}'  */
#line 1336 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5611 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr '}'  */
#line 1338 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5618 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1341 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5625 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1344 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5631 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1346 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5637 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1348 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5643 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1350 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5649 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1352 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5655 "raku.tab.c"
    break;

  case 203: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1354 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5661 "raku.tab.c"
    break;

  case 204: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1356 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5667 "raku.tab.c"
    break;

  case 205: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1358 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5674 "raku.tab.c"
    break;

  case 206: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1361 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5680 "raku.tab.c"
    break;

  case 207: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1363 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5686 "raku.tab.c"
    break;

  case 208: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1365 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5692 "raku.tab.c"
    break;

  case 209: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1367 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5699 "raku.tab.c"
    break;

  case 210: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1370 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5706 "raku.tab.c"
    break;

  case 211: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1373 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5714 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list '}'  */
#line 1378 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5720 "raku.tab.c"
    break;

  case 213: /* method_body: '{' YADA '}'  */
#line 1379 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5726 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1381 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5733 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr '}'  */
#line 1384 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5740 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1387 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5747 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1390 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5753 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1392 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5759 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1394 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5765 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5771 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1398 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5777 "raku.tab.c"
    break;

  case 222: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1400 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5783 "raku.tab.c"
    break;

  case 223: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1402 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5789 "raku.tab.c"
    break;

  case 224: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1404 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5796 "raku.tab.c"
    break;

  case 225: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1407 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5802 "raku.tab.c"
    break;

  case 226: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1409 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5808 "raku.tab.c"
    break;

  case 227: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1411 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5814 "raku.tab.c"
    break;

  case 228: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1413 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5821 "raku.tab.c"
    break;

  case 229: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1416 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5828 "raku.tab.c"
    break;

  case 230: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1419 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5836 "raku.tab.c"
    break;

  case 231: /* pkg_name: IDENT  */
#line 1424 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5842 "raku.tab.c"
    break;

  case 232: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1428 "raku.y"
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
#line 5860 "raku.tab.c"
    break;

  case 233: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1444 "raku.y"
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
#line 5877 "raku.tab.c"
    break;

  case 234: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1459 "raku.y"
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
#line 5894 "raku.tab.c"
    break;

  case 235: /* is_clauses: %empty  */
#line 1473 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5900 "raku.tab.c"
    break;

  case 236: /* is_clauses: is_clauses IDENT IDENT  */
#line 1475 "raku.y"
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
#line 5916 "raku.tab.c"
    break;

  case 237: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1487 "raku.y"
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
#line 5932 "raku.tab.c"
    break;

  case 238: /* class_body_list: %empty  */
#line 1500 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5938 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list ';'  */
#line 1501 "raku.y"
                           { (yyval.list) = (yyvsp[-1].list); }
#line 5944 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1503 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5951 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1506 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5958 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1509 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5965 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1512 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5972 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1515 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5979 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1518 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5986 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1521 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5993 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1524 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6000 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1527 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6007 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1530 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6014 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1533 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6021 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1536 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6028 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1539 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6039 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1546 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6049 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1552 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6059 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1558 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6070 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1565 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6081 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1572 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6092 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1579 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6100 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1583 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6108 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1587 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6116 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1591 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6124 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1595 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6135 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1602 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6146 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1609 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6157 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1616 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6168 "raku.tab.c"
    break;

  case 266: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1623 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6182 "raku.tab.c"
    break;

  case 267: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1633 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6194 "raku.tab.c"
    break;

  case 268: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1641 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6206 "raku.tab.c"
    break;

  case 269: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1649 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6219 "raku.tab.c"
    break;

  case 270: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1658 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6230 "raku.tab.c"
    break;

  case 271: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1665 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6241 "raku.tab.c"
    break;

  case 272: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1672 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6255 "raku.tab.c"
    break;

  case 273: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1682 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6267 "raku.tab.c"
    break;

  case 274: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1690 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 6279 "raku.tab.c"
    break;

  case 275: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1698 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6292 "raku.tab.c"
    break;

  case 276: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1707 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6303 "raku.tab.c"
    break;

  case 277: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1716 "raku.y"
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
#line 6320 "raku.tab.c"
    break;

  case 278: /* grammar_body_list: %empty  */
#line 1730 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 6326 "raku.tab.c"
    break;

  case 279: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1732 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6335 "raku.tab.c"
    break;

  case 280: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1737 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6344 "raku.tab.c"
    break;

  case 281: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1742 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6353 "raku.tab.c"
    break;

  case 282: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1749 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 6361 "raku.tab.c"
    break;

  case 283: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1753 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 6369 "raku.tab.c"
    break;

  case 284: /* named_arg_list: ':' IDENT  */
#line 1757 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6380 "raku.tab.c"
    break;

  case 285: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1764 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6388 "raku.tab.c"
    break;

  case 286: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1768 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6396 "raku.tab.c"
    break;

  case 287: /* pair_list: IDENT OP_FATARROW expr  */
#line 1774 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6402 "raku.tab.c"
    break;

  case 288: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1776 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6408 "raku.tab.c"
    break;

  case 289: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1778 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6414 "raku.tab.c"
    break;

  case 290: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1780 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6420 "raku.tab.c"
    break;

  case 291: /* param_list: VAR_SCALAR  */
#line 1783 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6426 "raku.tab.c"
    break;

  case 292: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1784 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6432 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1785 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6438 "raku.tab.c"
    break;

  case 294: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1786 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6444 "raku.tab.c"
    break;

  case 295: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1787 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6450 "raku.tab.c"
    break;

  case 296: /* param_list: VAR_ARRAY  */
#line 1788 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6456 "raku.tab.c"
    break;

  case 297: /* param_list: param_list ',' VAR_ARRAY  */
#line 1789 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6462 "raku.tab.c"
    break;

  case 298: /* param_list: IDENT VAR_SCALAR  */
#line 1790 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6468 "raku.tab.c"
    break;

  case 299: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1791 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6474 "raku.tab.c"
    break;

  case 300: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1792 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6480 "raku.tab.c"
    break;

  case 301: /* param_list: param_list ',' VAR_SCALAR  */
#line 1793 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6486 "raku.tab.c"
    break;

  case 302: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1794 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6492 "raku.tab.c"
    break;

  case 303: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1795 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6498 "raku.tab.c"
    break;

  case 304: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1796 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6504 "raku.tab.c"
    break;

  case 305: /* param_list: VAR_SCALAR '=' expr  */
#line 1797 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6510 "raku.tab.c"
    break;

  case 306: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1798 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6516 "raku.tab.c"
    break;

  case 307: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1799 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6522 "raku.tab.c"
    break;

  case 308: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1800 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6528 "raku.tab.c"
    break;

  case 309: /* param_list: SLURPY_POS  */
#line 1801 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6534 "raku.tab.c"
    break;

  case 310: /* param_list: param_list ',' SLURPY_POS  */
#line 1802 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6540 "raku.tab.c"
    break;

  case 311: /* param_list: SLURPY_LOL  */
#line 1803 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6546 "raku.tab.c"
    break;

  case 312: /* param_list: param_list ',' SLURPY_LOL  */
#line 1804 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6552 "raku.tab.c"
    break;

  case 313: /* param_list: SLURPY_NAMED  */
#line 1805 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6558 "raku.tab.c"
    break;

  case 314: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1806 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6564 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list '}'  */
#line 1809 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6570 "raku.tab.c"
    break;

  case 316: /* block: '{' YADA '}'  */
#line 1810 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6576 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list expr '}'  */
#line 1812 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6582 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6588 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1816 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6594 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1818 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6600 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1820 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6606 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1822 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6613 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1825 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6619 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1827 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6625 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1829 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6631 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1831 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 6637 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1833 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6644 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1836 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6651 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1839 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6659 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1843 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6666 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1846 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6673 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1849 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6680 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1852 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 6686 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1854 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6693 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1857 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6700 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1860 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,a); (yyval.node)=make_seq(l); }
#line 6707 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1863 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6714 "raku.tab.c"
    break;

  case 338: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1866 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6721 "raku.tab.c"
    break;

  case 339: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1869 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,t); (yyval.node)=make_seq(l); }
#line 6727 "raku.tab.c"
    break;

  case 340: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1871 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 6733 "raku.tab.c"
    break;

  case 341: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1873 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 6739 "raku.tab.c"
    break;

  case 342: /* block: '{' stmt_list KW_LAST '}'  */
#line 1875 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6745 "raku.tab.c"
    break;

  case 343: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1877 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6751 "raku.tab.c"
    break;

  case 344: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6757 "raku.tab.c"
    break;

  case 345: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1881 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6763 "raku.tab.c"
    break;

  case 346: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1883 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6769 "raku.tab.c"
    break;

  case 347: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1885 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6775 "raku.tab.c"
    break;

  case 348: /* closure: '{' expr '}'  */
#line 1888 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6781 "raku.tab.c"
    break;

  case 349: /* expr: VAR_SCALAR '=' expr  */
#line 1891 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6787 "raku.tab.c"
    break;

  case 350: /* expr: VAR_ARRAY '=' expr  */
#line 1892 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6793 "raku.tab.c"
    break;

  case 351: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1893 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6799 "raku.tab.c"
    break;

  case 352: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1894 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6805 "raku.tab.c"
    break;

  case 353: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1895 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6811 "raku.tab.c"
    break;

  case 354: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1896 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6817 "raku.tab.c"
    break;

  case 355: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1897 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6823 "raku.tab.c"
    break;

  case 356: /* expr: KW_GATHER block  */
#line 1898 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6833 "raku.tab.c"
    break;

  case 357: /* expr: KW_GATHER for_stmt  */
#line 1903 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6843 "raku.tab.c"
    break;

  case 358: /* expr: tern_expr OP_FATARROW expr  */
#line 1909 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6849 "raku.tab.c"
    break;

  case 359: /* expr: tern_expr  */
#line 1910 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6855 "raku.tab.c"
    break;

  case 360: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1914 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6861 "raku.tab.c"
    break;

  case 361: /* tern_expr: or_expr  */
#line 1915 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6867 "raku.tab.c"
    break;

  case 362: /* or_expr: or_expr OP_OR and_expr  */
#line 1918 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6873 "raku.tab.c"
    break;

  case 363: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1919 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6879 "raku.tab.c"
    break;

  case 364: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1920 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6885 "raku.tab.c"
    break;

  case 365: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1921 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6891 "raku.tab.c"
    break;

  case 366: /* or_expr: and_expr  */
#line 1922 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6897 "raku.tab.c"
    break;

  case 367: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1925 "raku.y"
                               { (yyval.node)=rk_logical_and((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6903 "raku.tab.c"
    break;

  case 368: /* and_expr: cmp_expr  */
#line 1926 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6909 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1929 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_EQ,rk_numeric_ctx((yyvsp[0].node))); }
#line 6915 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1930 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6921 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1931 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6927 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1932 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6933 "raku.tab.c"
    break;

  case 373: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1933 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6939 "raku.tab.c"
    break;

  case 374: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1934 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6945 "raku.tab.c"
    break;

  case 375: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1935 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6951 "raku.tab.c"
    break;

  case 376: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1936 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6957 "raku.tab.c"
    break;

  case 377: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1937 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6963 "raku.tab.c"
    break;

  case 378: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1938 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_NE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6969 "raku.tab.c"
    break;

  case 379: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1939 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6975 "raku.tab.c"
    break;

  case 380: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1940 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6981 "raku.tab.c"
    break;

  case 381: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1941 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6987 "raku.tab.c"
    break;

  case 382: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1942 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6993 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1943 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6999 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1944 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7005 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1945 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7011 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1946 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7017 "raku.tab.c"
    break;

  case 387: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1947 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7023 "raku.tab.c"
    break;

  case 388: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1948 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7029 "raku.tab.c"
    break;

  case 389: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1949 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7035 "raku.tab.c"
    break;

  case 390: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1950 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7041 "raku.tab.c"
    break;

  case 391: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1951 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7047 "raku.tab.c"
    break;

  case 392: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1953 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 7057 "raku.tab.c"
    break;

  case 393: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1959 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 7067 "raku.tab.c"
    break;

  case 394: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1965 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 7077 "raku.tab.c"
    break;

  case 395: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1971 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 7087 "raku.tab.c"
    break;

  case 396: /* cmp_expr: divis_expr  */
#line 1976 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 7093 "raku.tab.c"
    break;

  case 397: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1979 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7099 "raku.tab.c"
    break;

  case 398: /* divis_expr: jct_expr  */
#line 1980 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7105 "raku.tab.c"
    break;

  case 399: /* jct_expr: jct_expr '|' range_expr  */
#line 1983 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7111 "raku.tab.c"
    break;

  case 400: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1984 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7117 "raku.tab.c"
    break;

  case 401: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1985 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7123 "raku.tab.c"
    break;

  case 402: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1986 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7129 "raku.tab.c"
    break;

  case 403: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1987 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7135 "raku.tab.c"
    break;

  case 404: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1988 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7141 "raku.tab.c"
    break;

  case 405: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1989 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7147 "raku.tab.c"
    break;

  case 406: /* jct_expr: jct_expr '&' range_expr  */
#line 1990 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7153 "raku.tab.c"
    break;

  case 407: /* jct_expr: dor_expr  */
#line 1991 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7159 "raku.tab.c"
    break;

  case 408: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1995 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7165 "raku.tab.c"
    break;

  case 409: /* dor_expr: range_expr  */
#line 1996 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7171 "raku.tab.c"
    break;

  case 410: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1999 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7177 "raku.tab.c"
    break;

  case 411: /* range_expr: add_expr YADA add_expr  */
#line 2000 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7183 "raku.tab.c"
    break;

  case 412: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 2001 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7189 "raku.tab.c"
    break;

  case 413: /* range_expr: range_expr OP_COLL add_expr  */
#line 2002 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7195 "raku.tab.c"
    break;

  case 414: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 2003 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7201 "raku.tab.c"
    break;

  case 415: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 2004 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7207 "raku.tab.c"
    break;

  case 416: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 2005 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 7213 "raku.tab.c"
    break;

  case 417: /* range_expr: add_expr  */
#line 2006 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 7219 "raku.tab.c"
    break;

  case 418: /* add_expr: add_expr '~' repl_expr  */
#line 2009 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7225 "raku.tab.c"
    break;

  case 419: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 2010 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7231 "raku.tab.c"
    break;

  case 420: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 2011 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7237 "raku.tab.c"
    break;

  case 421: /* add_expr: repl_expr  */
#line 2012 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 7243 "raku.tab.c"
    break;

  case 422: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 2015 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7249 "raku.tab.c"
    break;

  case 423: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 2016 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 7255 "raku.tab.c"
    break;

  case 424: /* repl_expr: addsub_expr  */
#line 2017 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7261 "raku.tab.c"
    break;

  case 425: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 2020 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7267 "raku.tab.c"
    break;

  case 426: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 2021 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7273 "raku.tab.c"
    break;

  case 427: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 2022 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7279 "raku.tab.c"
    break;

  case 428: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 2023 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7285 "raku.tab.c"
    break;

  case 429: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 2024 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7291 "raku.tab.c"
    break;

  case 430: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 2025 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7297 "raku.tab.c"
    break;

  case 431: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 2026 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7303 "raku.tab.c"
    break;

  case 432: /* addsub_expr: mul_expr  */
#line 2027 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 7309 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr '*' unary_expr  */
#line 2030 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7315 "raku.tab.c"
    break;

  case 434: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 2031 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7321 "raku.tab.c"
    break;

  case 435: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 2032 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7327 "raku.tab.c"
    break;

  case 436: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 2033 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7333 "raku.tab.c"
    break;

  case 437: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 2034 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7339 "raku.tab.c"
    break;

  case 438: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 2035 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7345 "raku.tab.c"
    break;

  case 439: /* mul_expr: mul_expr '/' unary_expr  */
#line 2036 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7351 "raku.tab.c"
    break;

  case 440: /* mul_expr: mul_expr '%' unary_expr  */
#line 2037 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7357 "raku.tab.c"
    break;

  case 441: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 2039 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,rk_numeric_ctx((yyvsp[-2].node))); expr_add_child(c,rk_numeric_ctx((yyvsp[0].node))); (yyval.node)=c; }
#line 7363 "raku.tab.c"
    break;

  case 442: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2041 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7369 "raku.tab.c"
    break;

  case 443: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2043 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7375 "raku.tab.c"
    break;

  case 444: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2045 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7381 "raku.tab.c"
    break;

  case 445: /* mul_expr: unary_expr  */
#line 2046 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7387 "raku.tab.c"
    break;

  case 446: /* unary_expr: '-' unary_expr  */
#line 2049 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,rk_numeric_ctx((yyvsp[0].node))); }
#line 7393 "raku.tab.c"
    break;

  case 447: /* unary_expr: '+' unary_expr  */
#line 2050 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 7399 "raku.tab.c"
    break;

  case 448: /* unary_expr: '!' unary_expr  */
#line 2051 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7405 "raku.tab.c"
    break;

  case 449: /* unary_expr: CARET unary_expr  */
#line 2052 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7411 "raku.tab.c"
    break;

  case 450: /* unary_expr: OP_REDUCE unary_expr  */
#line 2054 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7420 "raku.tab.c"
    break;

  case 451: /* unary_expr: pow_expr  */
#line 2058 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7426 "raku.tab.c"
    break;

  case 452: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2061 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7432 "raku.tab.c"
    break;

  case 453: /* pow_expr: postfix_expr  */
#line 2062 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7438 "raku.tab.c"
    break;

  case 454: /* scalar_list: VAR_SCALAR  */
#line 2065 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7444 "raku.tab.c"
    break;

  case 455: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2066 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7450 "raku.tab.c"
    break;

  case 456: /* meth_name: IDENT  */
#line 2069 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7456 "raku.tab.c"
    break;

  case 457: /* meth_name: KW_SORT  */
#line 2070 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7462 "raku.tab.c"
    break;

  case 458: /* meth_name: KW_REVERSE  */
#line 2071 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7468 "raku.tab.c"
    break;

  case 459: /* meth_name: KW_MAP  */
#line 2072 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7474 "raku.tab.c"
    break;

  case 460: /* meth_name: KW_GREP  */
#line 2073 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7480 "raku.tab.c"
    break;

  case 461: /* meth_name: KW_SAY  */
#line 2074 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7486 "raku.tab.c"
    break;

  case 462: /* meth_name: KW_PRINT  */
#line 2075 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7492 "raku.tab.c"
    break;

  case 463: /* meth_name: KW_TAKE  */
#line 2076 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7498 "raku.tab.c"
    break;

  case 464: /* meth_name: KW_RETURN  */
#line 2077 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7504 "raku.tab.c"
    break;

  case 465: /* meth_name: KW_EXISTS  */
#line 2078 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7510 "raku.tab.c"
    break;

  case 466: /* meth_name: KW_DELETE  */
#line 2079 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7516 "raku.tab.c"
    break;

  case 467: /* meth_name: KW_JOIN  */
#line 2080 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7522 "raku.tab.c"
    break;

  case 468: /* meth_name: TESTOP  */
#line 2081 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7528 "raku.tab.c"
    break;

  case 469: /* postfix_expr: call_expr  */
#line 2083 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7534 "raku.tab.c"
    break;

  case 470: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2086 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7544 "raku.tab.c"
    break;

  case 471: /* call_expr: IDENT '(' arg_list ')'  */
#line 2092 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7553 "raku.tab.c"
    break;

  case 472: /* call_expr: IDENT '(' ')'  */
#line 2096 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7559 "raku.tab.c"
    break;

  case 473: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2098 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7565 "raku.tab.c"
    break;

  case 474: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2100 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7571 "raku.tab.c"
    break;

  case 475: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2102 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7580 "raku.tab.c"
    break;

  case 476: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2107 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7586 "raku.tab.c"
    break;

  case 477: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2109 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7599 "raku.tab.c"
    break;

  case 478: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2118 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7612 "raku.tab.c"
    break;

  case 479: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2127 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7623 "raku.tab.c"
    break;

  case 480: /* call_expr: IDENT '.' KW_NEW  */
#line 2134 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7631 "raku.tab.c"
    break;

  case 481: /* call_expr: IDENT '.' IDENT  */
#line 2138 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7640 "raku.tab.c"
    break;

  case 482: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2143 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7651 "raku.tab.c"
    break;

  case 483: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2150 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7660 "raku.tab.c"
    break;

  case 484: /* call_expr: IDENT '.' CARET IDENT  */
#line 2155 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7670 "raku.tab.c"
    break;

  case 485: /* call_expr: atom '.' CARET IDENT  */
#line 2161 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7680 "raku.tab.c"
    break;

  case 486: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2167 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7691 "raku.tab.c"
    break;

  case 487: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2174 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7702 "raku.tab.c"
    break;

  case 488: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2181 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7711 "raku.tab.c"
    break;

  case 489: /* call_expr: atom '.' meth_name  */
#line 2186 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7720 "raku.tab.c"
    break;

  case 490: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2191 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7731 "raku.tab.c"
    break;

  case 491: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2198 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7740 "raku.tab.c"
    break;

  case 492: /* call_expr: call_expr '.' meth_name  */
#line 2203 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7749 "raku.tab.c"
    break;

  case 493: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2208 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7760 "raku.tab.c"
    break;

  case 494: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2215 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7771 "raku.tab.c"
    break;

  case 495: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2222 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7782 "raku.tab.c"
    break;

  case 496: /* call_expr: '.' meth_name '(' ')'  */
#line 2229 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7791 "raku.tab.c"
    break;

  case 497: /* call_expr: '.' meth_name  */
#line 2234 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7800 "raku.tab.c"
    break;

  case 498: /* call_expr: KW_DIE expr  */
#line 2239 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7806 "raku.tab.c"
    break;

  case 499: /* call_expr: KW_MAP closure expr  */
#line 2241 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7812 "raku.tab.c"
    break;

  case 500: /* call_expr: KW_MAP closure ',' expr  */
#line 2243 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7818 "raku.tab.c"
    break;

  case 501: /* call_expr: KW_GREP closure expr  */
#line 2245 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7824 "raku.tab.c"
    break;

  case 502: /* call_expr: KW_GREP closure ',' expr  */
#line 2247 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7830 "raku.tab.c"
    break;

  case 503: /* call_expr: KW_SORT expr  */
#line 2249 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7836 "raku.tab.c"
    break;

  case 504: /* call_expr: KW_SORT closure expr  */
#line 2251 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7842 "raku.tab.c"
    break;

  case 505: /* call_expr: KW_SORT closure ',' expr  */
#line 2253 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7848 "raku.tab.c"
    break;

  case 506: /* call_expr: KW_REVERSE expr  */
#line 2255 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7854 "raku.tab.c"
    break;

  case 507: /* call_expr: atom  */
#line 2256 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7860 "raku.tab.c"
    break;

  case 508: /* arg_list: expr  */
#line 2259 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7866 "raku.tab.c"
    break;

  case 509: /* arg_list: arg_list ',' expr  */
#line 2260 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7872 "raku.tab.c"
    break;

  case 510: /* arg_list: arg_list ','  */
#line 2261 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7878 "raku.tab.c"
    break;

  case 511: /* arg_list: ':' IDENT  */
#line 2262 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7884 "raku.tab.c"
    break;

  case 512: /* arg_list: ':' '!' IDENT  */
#line 2263 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7890 "raku.tab.c"
    break;

  case 513: /* arg_list: ':' IDENT '(' expr ')'  */
#line 2264 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7896 "raku.tab.c"
    break;

  case 514: /* arg_list: ':' IDENT '(' ')'  */
#line 2265 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7902 "raku.tab.c"
    break;

  case 515: /* arg_list: ':' IDENT '<' IDENT '>'  */
#line 2266 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7908 "raku.tab.c"
    break;

  case 516: /* arg_list: arg_list ',' ':' IDENT  */
#line 2267 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7914 "raku.tab.c"
    break;

  case 517: /* arg_list: arg_list ',' ':' '!' IDENT  */
#line 2268 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7920 "raku.tab.c"
    break;

  case 518: /* arg_list: arg_list ',' ':' IDENT '(' expr ')'  */
#line 2269 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-6].list),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7926 "raku.tab.c"
    break;

  case 519: /* arg_list: arg_list ',' ':' IDENT '(' ')'  */
#line 2270 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-5].list),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7932 "raku.tab.c"
    break;

  case 520: /* arg_list: arg_list ',' ':' IDENT '<' IDENT '>'  */
#line 2271 "raku.y"
                                           { (yyval.list)=exprlist_append((yyvsp[-6].list),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7938 "raku.tab.c"
    break;

  case 521: /* paren_group: '(' ')'  */
#line 2274 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7944 "raku.tab.c"
    break;

  case 522: /* paren_group: '(' expr ')'  */
#line 2275 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7950 "raku.tab.c"
    break;

  case 523: /* paren_group: '(' expr ',' ')'  */
#line 2277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7956 "raku.tab.c"
    break;

  case 524: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2279 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7963 "raku.tab.c"
    break;

  case 525: /* atom: LIT_INT  */
#line 2283 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7969 "raku.tab.c"
    break;

  case 526: /* atom: LIT_BOOL  */
#line 2285 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7977 "raku.tab.c"
    break;

  case 527: /* atom: LIT_FLOAT  */
#line 2288 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7983 "raku.tab.c"
    break;

  case 528: /* atom: LIT_STR  */
#line 2289 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7989 "raku.tab.c"
    break;

  case 529: /* atom: WORDLIST  */
#line 2291 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 8002 "raku.tab.c"
    break;

  case 530: /* atom: LIT_INTERP_STR  */
#line 2299 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 8008 "raku.tab.c"
    break;

  case 531: /* atom: VAR_SCALAR  */
#line 2300 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8014 "raku.tab.c"
    break;

  case 532: /* atom: OP_INC VAR_SCALAR  */
#line 2301 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 8020 "raku.tab.c"
    break;

  case 533: /* atom: OP_DEC VAR_SCALAR  */
#line 2302 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 8026 "raku.tab.c"
    break;

  case 534: /* atom: VAR_SCALAR OP_INC  */
#line 2303 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 8032 "raku.tab.c"
    break;

  case 535: /* atom: VAR_SCALAR OP_DEC  */
#line 2304 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 8038 "raku.tab.c"
    break;

  case 536: /* atom: VAR_TWIGIL OP_INC  */
#line 2305 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 8044 "raku.tab.c"
    break;

  case 537: /* atom: VAR_TWIGIL OP_DEC  */
#line 2306 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 8050 "raku.tab.c"
    break;

  case 538: /* atom: VAR_ARRAY  */
#line 2307 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8056 "raku.tab.c"
    break;

  case 539: /* atom: VAR_HASH  */
#line 2308 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8062 "raku.tab.c"
    break;

  case 540: /* atom: VAR_CAPTURE  */
#line 2310 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8070 "raku.tab.c"
    break;

  case 541: /* atom: VAR_FH  */
#line 2314 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8078 "raku.tab.c"
    break;

  case 542: /* atom: VAR_NAMED_CAPTURE  */
#line 2318 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 8085 "raku.tab.c"
    break;

  case 543: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2321 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8091 "raku.tab.c"
    break;

  case 544: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2323 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 8097 "raku.tab.c"
    break;

  case 545: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2325 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 8103 "raku.tab.c"
    break;

  case 546: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2327 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 8109 "raku.tab.c"
    break;

  case 547: /* atom: ARR_ALL_SLICE  */
#line 2329 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 8115 "raku.tab.c"
    break;

  case 548: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2331 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8121 "raku.tab.c"
    break;

  case 549: /* atom: VAR_SCALAR_IDX '[' expr ']'  */
#line 2333 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8127 "raku.tab.c"
    break;

  case 550: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>'  */
#line 2335 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8133 "raku.tab.c"
    break;

  case 551: /* atom: VAR_SCALAR_HASH '{' expr '}'  */
#line 2337 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8139 "raku.tab.c"
    break;

  case 552: /* atom: VAR_SCALAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2339 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8145 "raku.tab.c"
    break;

  case 553: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>' ADV_EXISTS  */
#line 2341 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8151 "raku.tab.c"
    break;

  case 554: /* atom: VAR_SCALAR_HASH '{' expr '}' ADV_DELETE  */
#line 2343 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8157 "raku.tab.c"
    break;

  case 555: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>' ADV_DELETE  */
#line 2345 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8163 "raku.tab.c"
    break;

  case 556: /* atom: VAR_HASH '{' expr '}'  */
#line 2347 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8169 "raku.tab.c"
    break;

  case 557: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2349 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8175 "raku.tab.c"
    break;

  case 558: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2351 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8181 "raku.tab.c"
    break;

  case 559: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2353 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8187 "raku.tab.c"
    break;

  case 560: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2355 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8193 "raku.tab.c"
    break;

  case 561: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2357 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8199 "raku.tab.c"
    break;

  case 562: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2359 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8205 "raku.tab.c"
    break;

  case 563: /* atom: IDENT  */
#line 2360 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8211 "raku.tab.c"
    break;

  case 564: /* atom: VAR_TWIGIL  */
#line 2362 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8219 "raku.tab.c"
    break;

  case 565: /* atom: VAR_ARRAY_TWIGIL  */
#line 2366 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8227 "raku.tab.c"
    break;

  case 566: /* atom: VAR_HASH_TWIGIL  */
#line 2370 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8235 "raku.tab.c"
    break;

  case 567: /* atom: '[' ']'  */
#line 2373 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 8241 "raku.tab.c"
    break;

  case 568: /* atom: '[' expr ']'  */
#line 2375 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8247 "raku.tab.c"
    break;

  case 569: /* atom: '[' expr ',' ']'  */
#line 2377 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8253 "raku.tab.c"
    break;

  case 570: /* atom: '[' expr ',' arg_list ']'  */
#line 2379 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8260 "raku.tab.c"
    break;

  case 571: /* atom: DOLLAR_LBRACKET ']'  */
#line 2381 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 8266 "raku.tab.c"
    break;

  case 572: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2383 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8272 "raku.tab.c"
    break;

  case 573: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2385 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8278 "raku.tab.c"
    break;

  case 574: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2387 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8285 "raku.tab.c"
    break;

  case 575: /* atom: paren_group  */
#line 2389 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 8291 "raku.tab.c"
    break;

  case 576: /* atom: block  */
#line 2390 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8297 "raku.tab.c"
    break;

  case 577: /* atom: KW_SUB block  */
#line 2391 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8303 "raku.tab.c"
    break;

  case 578: /* atom: KW_SUB '(' param_list ')' block  */
#line 2393 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 8310 "raku.tab.c"
    break;

  case 579: /* atom: OP_ARROW scalar_list block  */
#line 2396 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 8317 "raku.tab.c"
    break;


#line 8321 "raku.tab.c"

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

#line 2399 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
