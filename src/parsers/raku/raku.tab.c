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
  YYSYMBOL_191_ = 191,                     /* '?'  */
  YYSYMBOL_YYACCEPT = 192,                 /* $accept  */
  YYSYMBOL_program = 193,                  /* program  */
  YYSYMBOL_stmt_list = 194,                /* stmt_list  */
  YYSYMBOL_stmt = 195,                     /* stmt  */
  YYSYMBOL_if_stmt = 196,                  /* if_stmt  */
  YYSYMBOL_elsif_tail = 197,               /* elsif_tail  */
  YYSYMBOL_while_stmt = 198,               /* while_stmt  */
  YYSYMBOL_unless_stmt = 199,              /* unless_stmt  */
  YYSYMBOL_until_stmt = 200,               /* until_stmt  */
  YYSYMBOL_repeat_stmt = 201,              /* repeat_stmt  */
  YYSYMBOL_loop_stmt = 202,                /* loop_stmt  */
  YYSYMBOL_loop_incr = 203,                /* loop_incr  */
  YYSYMBOL_for_stmt = 204,                 /* for_stmt  */
  YYSYMBOL_given_stmt = 205,               /* given_stmt  */
  YYSYMBOL_catch_when_list = 206,          /* catch_when_list  */
  YYSYMBOL_when_list = 207,                /* when_list  */
  YYSYMBOL_sub_trait_list = 208,           /* sub_trait_list  */
  YYSYMBOL_sub_decl = 209,                 /* sub_decl  */
  YYSYMBOL_sub_body = 210,                 /* sub_body  */
  YYSYMBOL_method_body = 211,              /* method_body  */
  YYSYMBOL_pkg_name = 212,                 /* pkg_name  */
  YYSYMBOL_class_decl = 213,               /* class_decl  */
  YYSYMBOL_role_decl = 214,                /* role_decl  */
  YYSYMBOL_module_decl = 215,              /* module_decl  */
  YYSYMBOL_is_clauses = 216,               /* is_clauses  */
  YYSYMBOL_class_body_list = 217,          /* class_body_list  */
  YYSYMBOL_grammar_decl = 218,             /* grammar_decl  */
  YYSYMBOL_grammar_body_list = 219,        /* grammar_body_list  */
  YYSYMBOL_named_arg_list = 220,           /* named_arg_list  */
  YYSYMBOL_pair_list = 221,                /* pair_list  */
  YYSYMBOL_param_list = 222,               /* param_list  */
  YYSYMBOL_block = 223,                    /* block  */
  YYSYMBOL_closure = 224,                  /* closure  */
  YYSYMBOL_expr = 225,                     /* expr  */
  YYSYMBOL_tern_expr = 226,                /* tern_expr  */
  YYSYMBOL_or_expr = 227,                  /* or_expr  */
  YYSYMBOL_and_expr = 228,                 /* and_expr  */
  YYSYMBOL_cmp_expr = 229,                 /* cmp_expr  */
  YYSYMBOL_divis_expr = 230,               /* divis_expr  */
  YYSYMBOL_jct_expr = 231,                 /* jct_expr  */
  YYSYMBOL_dor_expr = 232,                 /* dor_expr  */
  YYSYMBOL_range_expr = 233,               /* range_expr  */
  YYSYMBOL_add_expr = 234,                 /* add_expr  */
  YYSYMBOL_repl_expr = 235,                /* repl_expr  */
  YYSYMBOL_addsub_expr = 236,              /* addsub_expr  */
  YYSYMBOL_mul_expr = 237,                 /* mul_expr  */
  YYSYMBOL_unary_expr = 238,               /* unary_expr  */
  YYSYMBOL_pow_expr = 239,                 /* pow_expr  */
  YYSYMBOL_scalar_list = 240,              /* scalar_list  */
  YYSYMBOL_meth_name = 241,                /* meth_name  */
  YYSYMBOL_postfix_expr = 242,             /* postfix_expr  */
  YYSYMBOL_call_expr = 243,                /* call_expr  */
  YYSYMBOL_arg_list = 244,                 /* arg_list  */
  YYSYMBOL_paren_group = 245,              /* paren_group  */
  YYSYMBOL_atom = 246                      /* atom  */
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
#define YYLAST   7942

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  192
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  580
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1442

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
     170,   168,   171,   191,     2,     2,     2,     2,     2,     2,
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
    2037,  2038,  2040,  2042,  2044,  2046,  2049,  2050,  2054,  2055,
    2056,  2057,  2062,  2065,  2066,  2069,  2070,  2073,  2074,  2075,
    2076,  2077,  2078,  2079,  2080,  2081,  2082,  2083,  2084,  2085,
    2087,  2089,  2095,  2100,  2101,  2103,  2105,  2110,  2112,  2121,
    2130,  2137,  2141,  2146,  2153,  2158,  2164,  2170,  2177,  2184,
    2189,  2194,  2201,  2206,  2211,  2218,  2225,  2232,  2237,  2242,
    2244,  2246,  2248,  2250,  2252,  2254,  2256,  2258,  2260,  2263,
    2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,  2272,  2273,
    2274,  2275,  2278,  2279,  2280,  2282,  2287,  2288,  2292,  2293,
    2294,  2303,  2304,  2305,  2306,  2307,  2308,  2309,  2310,  2311,
    2312,  2313,  2317,  2321,  2324,  2326,  2328,  2330,  2332,  2334,
    2336,  2338,  2340,  2342,  2344,  2346,  2348,  2350,  2352,  2354,
    2356,  2358,  2360,  2362,  2364,  2365,  2369,  2373,  2377,  2378,
    2380,  2382,  2385,  2386,  2388,  2390,  2393,  2394,  2395,  2396,
    2399
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
  "':'", "'?'", "$accept", "program", "stmt_list", "stmt", "if_stmt",
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

#define YYPACT_NINF (-1069)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-578)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1069,    23,  2221, -1069, -1069, -1069, -1069, -1069, -1069,   786,
     -75,   373,   -70,    39, -1069, -1069,  -134,   -68,   137,  7564,
     968, -1069, -1069, -1069,   163,  2326,   298,  6127,  6262,  6344,
    6479,  6561,     1,   -16,  6561,  4091,  4173,   549,   244,  6561,
    6561,   406,   432,  6696,  6778,   179,   150,    48,    76,   260,
     260,  6913,  6561,   179,   277,  6561,  4308,   459,   459,   134,
    7564, -1069, -1069,   459,   459,  4390,   506,   594,   615,  7564,
    7564,  7564,  1274, -1069,  4525,  4607,   408,  7564, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069,  7667,   603,   424,   466,   529,   679,   974,   746,
     543,  1014,   261,   182,   605,   998, -1069, -1069,   511,   520,
     552,   530,  6561,   717, -1069, -1069,  6561,  6561,  6561,  6561,
    6561,  6561,  2431,  6561,  6995,  4742,  6561,   728,  6561, -1069,
   -1069,  6561,   601,   329,  2567,  6561,  6561,   815,   383,   608,
    -108,   421,  -112,   686,   742,   694,   189, -1069, -1069,   699,
   -1069,   753,   643,   -43, -1069,   835,  4824, -1069,  2649,    16,
   -1069,   -88,   126,   252,   342,   909,   859,   579,   506,  4525,
     536,  4525,   407,  4525,   179,  4525,   179,   -33,   417,   328,
     585, -1069, -1069, -1069,    67, -1069,    97, -1069,   264,   794,
     799,   866,   791,   808,   -26,   263,  4525,   179,  4525,   179,
     430,  4959, -1069,  6561,  6561, -1069,  6561,  6561, -1069,  6561,
    5041,  5176,  5258,  5393, -1069, -1069,   894,   427, -1069, -1069,
   -1069,   830, -1069, -1069,   828,   841,   302, -1069,   861,   871,
     795,   309,   502,  6561,  7760,   939, -1069,    36, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069,   948, -1069,   709, -1069,
     993,   957,  1423, -1069,  6561,  6561,  6561,  6561,  6561,  6561,
    6561,  6561, -1069,  6561,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,   649,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,  7564,
    7564,  7564,  7564,  7564,  7564,  7564,  1274,  6561,  1233,   992,
     583,  1011,  1017,  1035,  1037,  1039,  1048, -1069,   849,  1052,
    4525,   548,   667,   999,  1057,  1010,  1066,  1059, -1069,  1087,
    1234,  1092,    46, -1069,    24,   879,   884,  1094,  1099,  1123,
    4742,  1281,  6561,  6561,  6561,  1287,  1274,  1233,  6561,  6561,
    6561,  6561,  6561,  6561,  6561,  2785, -1069, -1069,  1126,  1128,
     709,   900,   315,  1290, -1069,  5475,  6561,  6561, -1069,  6561,
    7130, -1069,  6561,  7212, -1069,   351,   246,   420,   517,  1152,
    1169,   919,   985,  6561,  6561,  6561,  6561,  6561,  6561,  6561,
   -1069,  5610,  1020,  6561,  6561, -1069,  5610,  1022,  1177,  1028,
   -1069,   506,  5610, -1069,  7564,  7564,    55, -1069, -1069,    18,
   -1069,    78, -1069, -1069, -1069,  1031, -1069,  5610,  6561,  6561,
   -1069,  6561,  6561, -1069,  6561,  6561,  1161,  5610, -1069,  1338,
    6561,  1346,  6561,  1042,  1323,  1044, -1069,  6561,  6561,  1353,
    1214,  1215,  1216,  1217,  1218,  1187,  6561, -1069,  6561, -1069,
    6561, -1069,   179,  6561,   179,    57, -1069,     9, -1069,    63,
    1220,  1221, -1069, -1069,  6561,  6561,  6561,  1386,  1223, -1069,
   -1069,  1398, -1069,  2867, -1069,  3003,  3085, -1069, -1069,   303,
     448,   433,  6127,  6262,  6561,  3220,    37,   172, -1069,    61,
    1229,  1230,  1231,  1232,  1236,  1237,  1238,  1240,  1241,  1242,
   -1069,   529,  1306,   529,   529,   529,   679,  1248,  1248,  1248,
    1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,
    1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,  1248,
   -1069, -1069, -1069, -1069,   746,  1014,  1014,  1014,  1014,  1014,
    1014,  1014,  1014,  1014,   479,   479,   479,   479,   479,   479,
     479,   182,   182,   182,   605,   605,   998,   998,   998,   998,
     998,   998,   998, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069,   -15,  1249,  1401,   185,
   -1069, -1069,  3302, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069,  1053, -1069,  5610,  6561,  6561,  5610,  1264, -1069,   -62,
      19, -1069,  3438, -1069,  3520,  6561,   439, -1069,     0, -1069,
    5692,  1265,   409,   497,  1000,  1285,  1266,  1282,  1279,  1299,
     354,   516, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
    1003, -1069, -1069,  1289,  1474,  5827, -1069,    69, -1069,  1308,
    1309,  1312,  4525,   560,  1313,  1409,   159,  5909,   562,  1317,
    6561,  6561, -1069,  7347, -1069,  6561, -1069,    70, -1069,  6561,
    6561,  1332,  3003,  1319,  1321,  1322,  1325,  1326,  1327,  1331,
     611,  3003,  1335,  1336,   696,   179,    -9,  7429, -1069,   179,
      36,   227,   292,   304,    -7,  1060,  1634,  1490,  6561,    38,
    1501,  1503,   179,   692,   710,  1337,  1339,  1341,  1342,  1343,
    1344, -1069,  1345,    91,  1349,  1333,  1357,  1350,   179,   179,
     179,  1347,  1352,  1348,  6561, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,   179,  1351,  6561,   179, -1069,  1520,
    1523, -1069,   460,  1354,  1062,    89,   585,   316,  1843,  1359,
    1361,  1362,  1374, -1069, -1069, -1069,  1074, -1069,  1080, -1069,
    1004,  4742,  6561,  6561,   494,   194,   546, -1069,   184,  6561,
    6561, -1069,  6561,  6561, -1069,  6561,  6561,  6561,  6561,  6561,
    6561,  6561,  6561, -1069,  1274,  1233, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,  7564,  6561,  3656,  5610, -1069, -1069,
    6561,  3738,  5610,  1364,  1082,  3003,   812,  1360,  1363,  1007,
    6561, -1069, -1069,  6561, -1069, -1069,  6561, -1069,  1084, -1069,
    1089,  1093, -1069,  5827,  1459,  1534,    75,  1095,  6561, -1069,
   -1069,  6561, -1069, -1069,  6561, -1069,  1162,  1164, -1069,  1192,
    1196, -1069, -1069,  1380, -1069,  1368,   486,  1538, -1069, -1069,
   -1069,  1115, -1069,  5610, -1069,  6561,  6561,  1137, -1069,   268,
   -1069,  1372,  1373,  4525,   850,  1375,  1354,  1141,  1376,  1378,
    6561,  1143, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
    1145, -1069, -1069, -1069,  1303, -1069, -1069,  4525,   179, -1069,
   -1069,  1545, -1069,  1550,  1551,  1572,    -6, -1069,    -7,  6127,
    6262,  3874, -1069,   505, -1069, -1069,  1573,  6561, -1069, -1069,
   -1069,    49, -1069,    80, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,  6561,   179, -1069, -1069, -1069,  1407,
    1408,  1559, -1069, -1069, -1069, -1069,  6561,  1411, -1069, -1069,
     179,  1405, -1069, -1069,   540,   980,  1125,  1529, -1069, -1069,
   -1069,  1354,  1354,  1155,  1157,  1581,  1582,  1585, -1069, -1069,
   -1069, -1069, -1069,   518, -1069, -1069, -1069,  1013,  1412,   -32,
    6561,  6561,  6561,  6561,  6561,  6561, -1069, -1069, -1069, -1069,
     581,   589,   610,   613,   630,   632,   633,   635,   636,   638,
     658,   663,   226,   484, -1069,  1420, -1069,  1163,  1345,  1421,
   -1069,  1165,  1167,  1345, -1069,  1425,  1173, -1069, -1069, -1069,
   -1069,  1426,  1428,  1431, -1069, -1069, -1069,  1432,  6561,  1434,
     525, -1069,  1433,  1436,  1437, -1069, -1069,  1605,  6045, -1069,
    3003,   898, -1069, -1069,  1439,  1533,  1535, -1069, -1069,  1175,
   -1069,  5610, -1069, -1069,  1354, -1069, -1069,   920,  1441,  1443,
      -9, -1069,  1178,  1334,   179,   179,   179, -1069,  1611, -1069,
      -6, -1069,   528,   254, -1069,   265,  6561,  6561,  6561,  6561,
    6561,  6561,  6561,  6561, -1069, -1069, -1069,  1612,  6561,    50,
    1617,  1618,   179,  1442, -1069, -1069,   179,  1448,  6561, -1069,
   -1069, -1069,   531,   596,   136,   173,  1136,  1450,  1451,  1193,
   -1069, -1069,  1354,  1354,  1626,  1627,  1628,  6561,  1476,   559,
   -1069,   664,   666,   672,   676,   678,   681, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,  6561,
    6561, -1069, -1069, -1069, -1069, -1069, -1069,  1484, -1069, -1069,
   -1069, -1069, -1069,  6561,  6045, -1069, -1069, -1069,  1496, -1069,
    1495,  1184, -1069, -1069,  6561,  6561,  3003,   935, -1069, -1069,
    5610, -1069, -1069, -1069, -1069,   179,   179, -1069, -1069, -1069,
   -1069, -1069, -1069,  6561,  6561,  6561, -1069, -1069, -1069,   684,
     687,   693,   734,   735,   743,   744,   749, -1069, -1069,  1665,
    6561, -1069, -1069, -1069, -1069, -1069,  6561,  1497, -1069,   165,
    1600, -1069,   180, -1069,  1685,  1686,  1524,  6561, -1069,  1687,
    1690,  1527,  6561, -1069,   285,   287,  1528,  1530, -1069, -1069,
    1531,   599, -1069, -1069, -1069, -1069, -1069,  1536,  6561,  6561,
   -1069, -1069, -1069, -1069, -1069, -1069,   754,   758, -1069,  1532,
    1539, -1069, -1069,  1537, -1069, -1069,  1186, -1069,   963,  1340,
   -1069,   760,   762,   766, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069, -1069,  1542,   179,  1540,  1190,  1522,  2032,
    1540,  1194,  1543,  1547, -1069,  1548,   457,   463, -1069,  1549,
    1700,  1705,  1552,  6561, -1069,  1712,  1717,  1553,  6561, -1069,
   -1069, -1069,   201,   222, -1069, -1069,   768,   771, -1069, -1069,
   -1069, -1069, -1069,  1554, -1069,   179, -1069, -1069, -1069, -1069,
    6561, -1069, -1069,  1540, -1069,   500,  6127,  6262,  3956, -1069,
     534, -1069,  1540, -1069, -1069, -1069,  6561, -1069,  6561, -1069,
   -1069,  1555,  1556, -1069,  1557,  1558,  1561, -1069,  1562,  1540,
    1206,  1540,  1208, -1069, -1069, -1069, -1069,  1565, -1069,  6561,
     564,   255, -1069,   305,  6561,  6561,  6561,  6561,  6561,  6561,
    6561,  6561, -1069, -1069,  1563,  1568, -1069, -1069, -1069, -1069,
   -1069, -1069, -1069,  1540, -1069,  1540,   179,   776,  6561,  6561,
    6561, -1069, -1069, -1069,   777,   779,   788,   824,   826,   832,
     854,   862, -1069, -1069, -1069, -1069, -1069, -1069,   863,   868,
     876, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069, -1069,
   -1069, -1069
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   526,   527,   528,   529,   531,   532,
     539,   540,   565,   564,   566,   567,     0,     0,     0,     0,
       0,   541,   542,   543,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   548,   530,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     3,     0,     4,   105,
     106,   124,   125,   126,   127,   107,   108,   134,   135,   136,
     138,   137,   115,     0,   359,   361,   366,   368,   396,   398,
     407,   409,   417,   421,   424,   432,   445,   452,   454,   470,
     576,   508,     0,     0,   535,   536,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     538,     0,     0,     0,     0,     0,     0,     0,   532,   539,
     540,   565,   564,     0,     0,     0,     0,   577,   450,   470,
     576,   508,   532,   539,   572,     0,     0,    43,     0,     0,
     509,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   417,     0,
       0,   578,   357,   356,     0,    57,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     161,     0,   164,     0,     0,   128,     0,     0,   129,     0,
       0,     0,     3,     0,   504,   507,   109,     3,   114,   499,
      59,     0,   231,   235,     0,     0,     0,   451,     0,     0,
     532,   539,   540,     0,   116,     0,   455,     0,   533,   534,
     449,   447,   446,   457,   469,   462,   463,   464,   465,   468,
     466,   467,   460,   461,   458,   459,   498,   522,     0,   568,
       0,     0,     0,   448,     0,     0,     0,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   351,   352,   353,   354,   355,   349,   477,     0,     0,
       0,   350,     0,     0,     0,     0,     0,     0,    47,   482,
       0,   481,   564,   473,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,    36,     0,   522,
       0,     0,   512,     0,    46,   511,     0,     0,     6,     0,
       0,    17,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      48,     0,     0,     0,     0,    51,     0,     0,   142,     0,
     154,     0,     0,   173,     0,     0,     0,     3,   184,   291,
     296,     0,   309,   311,   313,     0,    54,     0,     0,     0,
      56,     0,     0,    62,     0,     0,     0,     0,   178,     0,
       0,     0,     0,     0,   157,     0,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   500,     0,   502,
       0,   505,     0,     0,     0,     0,    58,     0,   238,     0,
       0,     0,   278,     3,     0,     0,     0,     0,     0,   117,
     118,     0,   580,     0,   523,     0,     0,   569,   316,   539,
     540,   565,     0,     0,     0,     0,     0,     0,   315,     0,
     470,   508,     0,     0,     0,     0,     0,     0,     0,     0,
     358,   362,     0,   365,   364,   363,   367,   369,   378,   381,
     382,   377,   376,   375,   374,   373,   372,   371,   370,   379,
     380,   383,   387,   388,   389,   390,   391,   384,   385,   386,
     392,   393,   394,   395,   397,   405,   404,   403,   402,   401,
     400,   399,   406,   408,   415,   414,   413,   412,   411,   410,
     416,   420,   419,   418,   422,   423,   430,   429,   428,   427,
     426,   425,   431,   441,   443,   444,   442,   438,   437,   436,
     435,   434,   433,   439,   440,   453,   493,     0,     0,   490,
      11,    69,     0,    98,    99,   100,   101,   102,    66,   476,
      12,     0,    22,     0,     0,     0,     0,   544,    13,   549,
     557,    72,     0,   485,     0,     0,   284,   474,     0,   472,
     511,   550,   552,   551,     0,     0,     0,     0,     0,     0,
     493,   490,   351,   352,   353,   354,   355,   349,   350,   574,
       0,    37,    45,     0,     0,     0,   513,     0,   510,     0,
       0,     0,     0,     0,     0,   529,   564,     0,     0,     0,
       0,     0,    33,     0,    34,     0,    35,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   523,     0,     0,   145,   523,
       0,     0,   410,   416,     0,     0,     0,     0,     0,   298,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42,   471,     0,     0,     0,     0,     0,   523,     0,
     523,     0,     0,     0,     0,   130,   131,   132,   133,   348,
     501,   503,   506,   110,     0,     0,     0,     0,   111,     0,
       0,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   123,   456,   497,     0,   524,     0,   570,
       0,     0,     0,     0,     0,     0,     0,   341,     0,     0,
       0,   342,     0,     0,   343,     0,     0,     0,     0,     0,
       0,     0,     0,   317,     0,     0,    81,    83,    85,    88,
      86,    87,    82,    84,     0,     0,     0,     0,    16,   486,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   561,   563,     0,   560,   562,     0,   484,     0,   480,
       0,     0,   282,     0,     0,     0,     0,     0,     0,   553,
     555,     0,   554,   556,     0,   544,   549,   557,   550,   552,
     551,   575,    44,     0,   515,     0,   517,     0,     7,     5,
       8,     0,    19,     0,     9,     0,     0,     0,    26,     0,
      25,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    89,    92,    91,    95,    93,    94,    90,    49,
       0,    96,    97,    52,   139,   144,   143,     0,     0,   153,
     170,     0,   172,     0,     0,     0,     0,   183,     0,     0,
       0,     0,   195,     0,   292,   305,     0,     0,   299,   300,
     579,   301,   297,     0,   310,   312,   314,    55,    60,    61,
      64,    65,    39,    38,     0,     0,   174,   558,   559,     0,
       0,   155,   158,   159,   162,   163,     0,     0,   176,   113,
       0,     0,   236,   237,     0,     0,     0,     0,   239,   233,
     194,     0,     0,     0,     0,     0,     0,     0,   277,   234,
     119,   120,   121,   549,   496,   525,   571,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   326,   333,   339,   340,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   493,   490,   360,     0,   492,     0,   495,     0,
     489,     0,     0,   494,    68,     0,     0,    23,   547,   546,
     545,     0,     0,     0,   483,   478,   479,     0,     0,     0,
     284,   475,     0,     0,     0,   516,   514,     0,     0,   518,
       0,     0,   288,   287,     0,     0,     0,    10,    28,     0,
      29,     0,    32,   188,     0,    41,    40,     0,   525,   525,
       0,   146,     0,   147,     0,     0,     0,   180,     0,   186,
       0,   182,     0,     0,   198,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   196,   294,   307,     0,     0,   302,
       0,     0,     0,     0,    79,    80,     0,     0,     0,   177,
     112,   232,     0,     0,     0,     0,     0,     0,     0,     0,
     193,   191,     0,     0,     0,     0,     0,     0,   544,   557,
     336,     0,     0,     0,     0,     0,     0,   344,   345,   346,
     347,   318,   320,   322,   325,   323,   324,   319,   321,     0,
       0,    70,   491,    71,   488,   487,    67,   525,    73,    77,
      78,   283,   285,     0,     0,    74,    76,    75,     0,   520,
       0,     0,    20,    27,     0,     0,     0,     0,   187,    14,
       0,    50,    53,   141,   140,   523,     0,   151,   171,   168,
     169,   181,   185,     0,     0,     0,   199,   200,   197,     0,
       0,     0,     0,     0,     0,     0,     0,   293,   306,     0,
       0,   303,   304,   179,   175,   156,     0,     0,   167,     0,
       3,   271,     0,   268,     0,     0,     0,     0,   245,     0,
       0,     0,     0,   240,     0,     0,     0,     0,   241,   242,
       0,     0,   190,   192,   279,   280,   281,     0,     0,     0,
     327,   329,   332,   330,   331,   328,     0,     0,    24,     0,
       0,   521,   519,   525,   290,   289,     0,    30,     0,   148,
     149,     0,     0,     0,   201,   203,   205,   208,   206,   207,
     202,   204,   295,   308,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   258,     0,     0,     0,   259,     0,
       0,     0,     0,     0,   247,     0,     0,     0,     0,   246,
     243,   244,     0,     0,   274,   122,     0,     0,   334,   335,
     286,   283,    21,   525,    15,     0,   152,   209,   211,   210,
       0,   166,   270,     0,   213,   565,     0,     0,     0,   212,
       0,   267,     0,   256,   257,   249,     0,   252,     0,   255,
     248,     0,     0,   260,     0,     0,     0,   261,     0,     0,
       0,     0,     0,   337,   338,    31,   150,     0,   269,     0,
       0,     0,   217,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   215,   266,     0,     0,   264,   265,   251,   262,
     263,   250,   276,     0,   273,     0,     0,     0,     0,     0,
       0,   218,   219,   216,     0,     0,     0,     0,     0,     0,
       0,     0,   253,   254,   275,   272,   165,   214,     0,     0,
       0,   220,   222,   224,   227,   225,   226,   221,   223,   228,
     230,   229
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1069, -1069,     3, -1069,  -709,  -909, -1069, -1069, -1069, -1069,
   -1069,   393,  1708, -1069, -1069, -1069,   823, -1069,  -378, -1068,
     -50, -1069, -1069, -1069, -1069,   981, -1069, -1069,  -633,  1067,
    -416,   447,  1330,    -2,  -273, -1069,   856,  1477,  1284,  1449,
   -1069,   802,   -27,   732,  1070,   761,   -13, -1069,  -159,  -300,
   -1069,    -1,   235,     8,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   262,    78,    79,   718,    80,    81,    82,    83,
      84,  1227,    85,    86,   495,   743,   926,    87,   448,  1231,
     223,    88,    89,    90,   497,   772,    91,   777,   375,   688,
     455,   147,   210,   160,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   237,   256,
     108,   149,   161,   150,   151
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      93,   109,   542,     2,   178,  1081,   148,   915,   224,   421,
     110,   850,   111,   228,   229,   854,   179,   857,   155,    31,
     925,  1088,    29,     3,   769,   170,   172,   174,   176,   177,
     725,   402,   184,   186,   188,   123,   770,   192,   193,   646,
     698,   197,   199,   129,   130,   727,   616,   227,   619,   214,
     215,   132,   135,   219,   221,  1233,   240,   241,   242,   441,
     841,   842,   381,   235,   263,   936,   449,   450,   799,   133,
     451,   134,   258,   260,   449,   450,  1107,  1219,   451,   203,
     382,   449,   450,   774,   876,   451,   660,   661,   800,   729,
    1050,  1109,   805,   124,   404,   806,   807,   405,   131,   204,
     449,   450,   766,   767,   451,   808,   843,   206,   809,   810,
     349,   125,   811,   812,   351,   352,   353,   354,   355,   356,
     136,   359,   361,   363,   364,   394,   366,   207,   458,   367,
     452,   453,   454,   377,   378,   645,   954,   955,   452,   453,
     454,   844,   845,   380,   469,   452,   453,   454,   459,   225,
     641,  1234,   442,   825,   398,    76,   400,  1140,   730,   731,
    1110,  1111,   470,  1235,   452,   453,   454,   422,   826,   432,
     226,   437,    76,   439,  1197,   827,   449,   450,   156,    76,
     451,   447,   447,  1324,   180,   403,   728,   846,  1239,    76,
     855,   449,   450,   403,   473,   451,   475,   771,  1031,   480,
    1240,   481,   482,   802,   483,   484,   937,   485,   487,   489,
     485,   491,   449,   450,  1236,   458,   451,  1108,  1220,   205,
     133,   511,   134,   803,    76,   433,   801,   133,  1342,   134,
     205,   508,  1351,   449,   450,   459,   406,   451,   877,   724,
     452,   453,   454,   272,   877,   434,   768,   773,   886,   456,
     813,  1241,   457,   573,   896,   452,   453,   454,   208,   191,
     529,   530,   532,   533,   534,   535,   536,   537,   538,   539,
     110,   540,   531,   982,  1065,  1378,   452,   453,   454,   460,
     956,   897,   720,  1066,  1393,   433,   433,   584,   585,   586,
     587,   588,   589,   590,   407,   461,   458,   452,   453,   454,
    1310,  1402,  1315,  1404,  1237,   434,   434,   137,   408,   162,
     163,   164,  1311,   165,  1316,   462,   459,   500,  1238,   921,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   201,   166,  1424,   458,  1425,    76,   167,
     133,  1242,   134,   318,   369,   617,   927,   324,   325,  1296,
     370,   319,   320,   830,   208,  1243,   459,   358,   631,   983,
     984,   804,   409,  1312,  1300,  1317,   460,    76,   831,   376,
    1336,  1193,   180,  1009,   501,   832,   435,    76,   654,   436,
     656,   657,   658,  1007,   923,  1369,   662,   663,   664,   665,
     666,   667,   668,   401,  1159,   980,   924,   371,   321,   322,
     985,   986,   987,   678,   679,   680,  1371,   681,   683,   826,
     684,   689,   405,   123,   693,    76,   827,   722,   723,   194,
     410,   703,   704,   705,   706,   707,   708,   709,   694,   321,
     322,   712,   713,   471,   411,   323,   435,   435,   433,   436,
     436,   321,   322,  1207,  1412,   195,   463,   460,   209,    92,
     726,   472,   412,  1313,  1208,  1318,   735,   736,   434,   737,
     738,   690,   739,   740,   477,   217,   323,  1314,   745,  1319,
     747,   124,   493,   494,   222,   751,   752,   505,   323,   181,
     183,   168,   478,   126,   760,   674,   761,   460,   762,   791,
     261,   764,   200,   202,  1413,   380,   114,   115,   675,   318,
     216,   218,   779,   780,   781,   988,   778,   444,   445,   261,
     413,   446,   234,   273,  1022,  1023,   447,   236,  1073,   691,
     794,   795,   796,   798,   414,  1000,   975,   976,   424,  1001,
     977,   859,   860,   692,   129,   130,  1096,   826,  1002,  1097,
    1098,  1003,  1004,   127,   827,  1005,   129,   130,  1089,  1099,
    1091,  1024,  1100,  1101,   321,   322,  1102,  1103,   126,  1203,
     189,   128,   424,  1204,   190,  1384,   122,   423,  1385,  1386,
     424,   425,   426,   274,   275,   427,   428,   861,  1387,  1205,
     426,  1388,  1389,   427,   428,  1390,  1391,   429,   695,   435,
     419,   323,   436,   181,   420,  1408,   449,   450,   424,  1409,
     451,   793,   696,  1130,  1131,   238,   975,   976,   426,   674,
     977,   427,   428,   129,   130,  1410,   321,   322,   127,   862,
     863,   438,   853,   440,   443,  1356,   239,   276,   277,   278,
     670,  1358,   837,   838,   264,   279,   792,   265,   266,  1357,
     841,   842,   978,   852,   474,  1359,   476,   267,   678,   979,
     268,   269,  1160,   323,   270,   271,  1057,   570,   571,   572,
     452,   453,   454,   313,   222,   864,   710,   831,  1379,  1058,
     506,   714,   507,   875,   832,  1123,   430,   721,   345,   431,
     881,   844,   845,  1006,   512,   258,  1137,   272,   891,   892,
     382,   894,   734,   895,  1104,  1057,  1188,   898,   899,   831,
     697,   346,   742,   941,   942,   447,   832,   943,  1174,    92,
     430,   348,  1202,   431,  1229,   918,   272,  1206,   430,  1230,
     347,   431,   978,  1392,   933,   109,   935,  1259,   456,  1121,
     632,   457,   350,   633,   110,  1008,   111,   326,   327,   328,
     329,   330,   882,   365,   888,   883,   430,   889,   786,   431,
     788,   790,   967,  1411,  1252,  1253,   114,   115,   388,   389,
     390,   391,   392,   755,   970,   621,   622,   944,   945,   946,
    1147,   756,   280,   281,   282,   283,    93,   109,  1148,  1232,
     331,   332,  1323,   368,  1230,   272,   110,  1230,   111,   997,
     998,   999,   757,   909,   380,   758,   405,  1010,  1011,  1149,
    1012,  1013,  1150,  1014,  1015,  1016,  1017,  1018,  1019,  1020,
    1021,   393,   816,  1297,   817,   818,  1301,   819,   820,  1151,
     821,  1152,  1153,  1025,  1154,  1155,   122,  1156,  1029,  1251,
     379,   284,   285,   286,   287,   288,   289,   290,  1041,   291,
     822,  1042,   634,   635,  1043,   823,   902,  1157,   904,   292,
     293,  1047,  1158,  1260,   905,  1261,  1052,   834,   906,  1053,
     907,  1262,  1054,   908,   385,  1263,   816,  1264,   836,   817,
    1265,   839,   383,  1284,   418,   818,  1285,   848,   913,   851,
     386,   405,  1286,  1062,  1063,   305,   306,   307,   308,   309,
     310,  1069,   947,   514,   515,   405,   112,   113,  1077,   114,
     115,   116,   117,   118,   119,   120,  1370,  1372,   114,   115,
     388,   389,   390,   391,   392,  1082,   819,   820,   311,   312,
     415,   416,   417,  1287,  1288,   821,   822,  1092,  1093,  1095,
     384,   823,  1289,  1290,   387,  1106,  1161,   901,  1291,   763,
    1163,   765,   902,  1328,   904,   466,   910,  1329,   908,  1337,
    1168,  1338,  1112,  1170,   121,  1339,   492,  1373,   641,   816,
    1374,   817,   464,   504,  1117,  1427,  1431,   465,  1432,   122,
     818,     4,     5,     6,     7,     8,   467,  1433,   122,   152,
     153,   140,   141,   142,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,  1037,   243,   468,   405,  1141,  1142,
    1143,  1144,  1145,  1146,   146,    33,   819,   244,   820,   245,
     246,    39,   496,  1434,   821,  1435,   498,    41,   247,   248,
     395,  1436,   396,   249,   499,    49,    50,    51,    52,   250,
     251,    55,  1070,   629,   405,  1071,   822,   252,   253,   254,
     255,    60,    61,  1437,   823,   902,  1172,    62,  1122,   502,
     904,  1438,  1439,   591,   592,   593,  1180,  1440,   908,   503,
      66,  1027,  1028,   647,   648,  1441,  1032,  1033,   649,   650,
    1036,   294,   295,   296,   297,   298,   299,   300,   301,   302,
    1182,    67,    68,   405,   673,   405,   303,   596,   597,   598,
     599,   600,   601,   602,  1209,  1210,  1211,  1212,  1213,  1214,
    1215,  1216,  1189,   701,   511,  1190,  1218,   575,   576,   577,
     578,   579,   580,   581,   582,   583,  1228,  1277,  1061,   333,
     405,   510,   334,   335,   336,   337,   338,   339,   340,   341,
     541,   513,   543,   544,   545,  1257,  1124,    69,   304,  1125,
    1126,  1127,  1128,    70,    71,  1334,   518,  1244,   405,    72,
    1245,    74,  1246,  1247,    75,   154,    76,  1266,  1267,    77,
     314,   315,   914,   916,   316,   317,   919,   920,   922,   514,
     702,  1269,  1270,    92,   620,   342,   343,   344,   516,   940,
     517,   639,  1274,  1275,   636,   636,   637,   865,   405,   405,
     871,   996,   405,   623,  1040,   961,   962,   963,   636,   624,
    1138,  1281,  1282,  1283,   514,   711,   715,   515,   243,   716,
     717,   968,   719,   515,   971,   732,   733,   625,  1293,   626,
     244,   627,   245,   246,  1294,    92,   748,   515,   750,   515,
     628,   247,   248,  1299,   630,  1305,   249,   514,   835,   638,
    1309,   641,   250,   251,   928,   733,   981,   733,   243,   643,
     252,   253,   254,   255,   618,   640,  1326,  1327,   994,   405,
     244,  1250,   245,   246,   995,   405,  1035,   405,  1044,   405,
     642,   247,   248,  1045,   648,   644,   249,  1046,   405,  1051,
     648,   651,   250,   251,   841,   842,   844,   845,   652,   243,
     252,   253,   254,   255,   653,  1181,   655,  1350,   109,   514,
    1060,   244,   659,   245,   246,   676,  1187,   110,   671,   111,
     672,  1364,   247,   248,   859,   860,  1368,   249,   862,   863,
     699,  1064,   889,   250,   251,  1074,   733,  1078,   405,  1079,
     405,   252,   253,   254,   255,  1080,   717,   700,  1228,  1132,
     733,  1133,   733,   741,  1380,  1381,  1383,  1162,   405,  1164,
     648,  1165,   405,   744,  1394,   749,  1395,  1167,   405,   514,
    1186,   746,  1195,   515,   753,  1083,  1196,   717,  1273,   405,
    1333,   405,  1335,   717,  1343,   733,   759,  1407,  1352,   733,
     211,   213,  1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,
    1403,   733,  1405,   733,   594,   595,   754,   755,   756,   757,
     758,   782,  1113,   775,   776,   783,  1428,  1429,  1430,   784,
     814,   815,   304,   816,   817,   824,   829,  1119,   818,   819,
     820,  1276,   821,   822,   823,  1278,     4,     5,     6,     7,
       8,   828,   840,   858,     9,   519,   520,   521,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   522,   523,    29,   867,   866,    30,    31,    32,
      33,   524,   525,    36,    37,    38,    39,    40,   869,   868,
     870,   872,    41,    42,    43,    44,    45,    46,   526,   527,
      49,    50,    51,    52,    53,    54,    55,    56,    57,   873,
     878,   879,    58,    59,   880,   884,    60,    61,   885,   890,
     900,   902,    62,   903,   904,   934,    63,   905,   906,   907,
      64,    65,   938,   908,   939,    66,   966,   911,   912,   948,
     957,   949,   958,   950,   951,   952,   953,  1194,   959,   964,
     405,  1198,  1199,  1200,   965,   972,    67,    68,   973,   960,
     969,   990,   447,   991,   992,   993,  1034,  1038,  1048,  1049,
    1039,  1055,  1056,  1059,  1067,  1068,  1084,  1072,  1075,  1223,
    1076,  1085,  1086,  1225,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,  1087,  1105,  1114,
    1115,  1116,    69,  1118,  1120,  1129,  1134,  1135,    70,    71,
    1136,  1139,  1161,  1163,    72,    73,    74,  1166,  1168,    75,
    1169,    76,   528,  1170,    77,  1175,  1171,  1173,  1176,  1177,
    1178,  1183,  1184,  1191,  1185,  1192,  1201,  1217,  1221,  1222,
    1226,  1224,  1248,  1249,  1254,  1255,  1256,     4,     5,     6,
       7,     8,  1279,  1280,  1258,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,   929,   930,    29,  1268,  1271,    30,    31,
      32,    33,    34,   931,    36,    37,    38,    39,    40,  1272,
    1292,  1295,  1298,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
    1302,  1303,  1306,    58,    59,  1307,  1304,    60,    61,  1308,
    1320,  1344,  1321,    62,  1322,  1361,  1330,    63,  1325,  1332,
    1362,    64,    65,  1331,  1340,  1353,    66,  1365,  1230,  1354,
    1355,  1360,  1366,  1377,  1363,  1367,  1375,  1396,  1397,  1398,
    1399,   182,  1341,  1400,  1401,  1422,    92,    67,    68,  1406,
    1423,  1090,   974,   574,   887,     0,   546,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1376,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,     0,     0,     0,     0,    72,    73,    74,     0,     0,
      75,     0,    76,   932,     0,    77,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,  1426,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     0,     0,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,     0,     0,
       0,     0,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,     0,
       0,     0,    58,    59,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,    63,     0,     0,     0,
      64,    65,     0,     0,     0,    66,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,     0,    70,    71,
       0,     0,     0,     0,    72,    73,    74,     0,     0,    75,
       0,    76,   989,     0,    77,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,  1345,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,  1346,  1347,    29,     0,     0,    30,    31,    32,    33,
      34,  1348,    36,    37,    38,    39,    40,     0,     0,     0,
       0,    41,    42,    43,    44,    45,    46,    47,    48,    49,
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
      76,  1349,     0,    77,     4,     5,     6,     7,     8,     0,
       0,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,     0,     0,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,     0,     0,     0,     0,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,     0,     0,     0,
      58,    59,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,    63,     0,     0,     0,    64,    65,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,    67,    68,     0,   152,   153,   140,
     141,   142,    14,    15,   143,   144,   145,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   146,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
      69,     0,     0,     0,     0,     0,    70,    71,     0,    60,
      61,     0,    72,    73,    74,    62,     0,    75,     0,    76,
       0,     0,    77,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,    67,
      68,     0,   152,   153,   140,   141,   142,    14,    15,   143,
     144,   145,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   146,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,    69,     0,     0,     0,     0,
       0,    70,    71,     0,    60,    61,     0,    72,   157,   158,
      62,     0,    75,     0,    76,     0,   159,    77,     0,     0,
       0,     0,     0,    66,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   152,   153,
     140,   141,   372,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,   146,    33,     0,    70,    71,     0,     0,
      39,     0,    72,     0,    74,   357,    41,    75,     0,    76,
       0,   159,    77,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    66,
     152,   153,   140,   141,   142,    14,    15,   143,   144,   145,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,   146,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    66,    70,    71,     0,     0,     0,     0,    72,     0,
      74,   373,     0,    75,     0,    76,     0,   374,    77,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,   146,    33,     0,    70,    71,     0,     0,    39,     0,
      72,     0,    74,   399,    41,    75,     0,    76,     0,   159,
      77,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    66,   152,   153,
     140,   141,   142,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   146,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,    74,     0,
       0,    75,   669,    76,     0,   159,    77,     0,     0,     0,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   152,   153,   140,   141,   142,    14,
      15,   143,   144,   145,    19,    20,    21,    22,    23,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,   146,
      33,     0,    70,    71,     0,     0,    39,     0,    72,     0,
      74,   785,    41,    75,     0,    76,     0,   159,    77,     0,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    66,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,   146,    33,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,    66,    70,    71,
       0,     0,     0,     0,    72,     0,    74,   787,     0,    75,
       0,    76,     0,   159,    77,     0,     0,     0,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   152,   153,   140,   141,   142,    14,    15,   143,   144,
     145,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,   146,    33,     0,     0,
      70,    71,     0,    39,     0,     0,    72,     0,    74,    41,
       0,    75,   789,    76,     0,   159,    77,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    66,   152,   153,   140,   141,   142,    14,    15,
     143,   144,   145,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,   146,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,    66,    70,    71,     0,     0,     0,
       0,    72,   185,    74,     0,     0,    75,     0,    76,   797,
       0,    77,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   152,
     153,   140,   141,   142,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,   146,    33,     0,    70,    71,     0,
       0,    39,     0,    72,     0,    74,   833,    41,    75,     0,
      76,     0,   159,    77,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      66,   152,   153,   140,   141,   372,    14,    15,   143,   144,
     145,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,   146,    33,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,    66,    70,    71,     0,     0,     0,     0,    72,
       0,    74,   847,     0,    75,     0,    76,     0,   159,    77,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   152,   153,   140,
     141,   142,    14,    15,   143,   144,   145,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,   146,    33,     0,    70,    71,     0,     0,    39,
       0,    72,     0,    74,   849,    41,    75,     0,    76,     0,
     374,    77,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    66,   152,
     153,   140,   141,   372,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,   146,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
      66,    70,    71,     0,     0,     0,     0,    72,     0,    74,
    1026,     0,    75,     0,    76,     0,   159,    77,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   152,   153,   140,   141,   142,
      14,    15,   143,   144,   145,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
     146,    33,     0,    70,    71,     0,     0,    39,     0,    72,
       0,    74,  1030,    41,    75,     0,    76,     0,   374,    77,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    66,   152,   153,   140,
     141,   142,    14,    15,   143,   144,   145,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,   146,    33,     0,     0,     0,     0,     0,    39,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
       0,     0,     0,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,    66,    70,
      71,     0,     0,     0,     0,    72,   185,    74,     0,     0,
      75,     0,    76,  1094,     0,    77,     0,     0,     0,    67,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   152,   153,   140,   141,   142,    14,    15,   143,
     144,   145,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,    69,     0,   146,    33,     0,
       0,    70,    71,     0,    39,     0,     0,    72,   185,    74,
      41,     0,    75,     0,    76,  1382,     0,    77,    49,    50,
      51,    52,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,    66,   152,   153,   140,   141,   142,    14,
      15,   143,   144,   145,    19,    20,    21,    22,    23,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,   146,
      33,     0,     0,     0,     0,     0,    39,     0,     0,     0,
       0,     0,    41,     0,     0,     0,     0,     0,     0,     0,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     0,     0,
      69,     0,     0,     0,     0,    66,    70,    71,     0,     0,
       0,     0,    72,   185,    74,     0,     0,    75,     0,    76,
       0,     0,    77,     0,     0,     0,    67,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   152,
     153,   140,   141,   142,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,    69,     0,   146,    33,     0,     0,    70,    71,
       0,    39,     0,     0,    72,   187,    74,    41,     0,    75,
       0,    76,     0,     0,    77,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      66,   230,   231,   232,   141,   142,    14,    15,   143,   144,
     145,    19,    20,    21,    22,    23,     0,     0,     0,   233,
       0,    67,    68,     0,     0,     0,   146,    33,     0,     0,
       0,     0,     0,    39,     0,     0,     0,     0,     0,    41,
       0,     0,     0,     0,     0,     0,     0,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
       0,     0,    66,    70,    71,     0,     0,     0,     0,    72,
     220,    74,     0,     0,    75,     0,    76,     0,     0,    77,
       0,     0,     0,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,    69,
       0,   146,    33,     0,     0,    70,    71,     0,    39,     0,
       0,    72,     0,    74,    41,     0,    75,     0,    76,     0,
       0,    77,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    66,   152,   153,
     140,   141,   142,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   146,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,    74,   257,
       0,    75,     0,    76,     0,     0,    77,     0,     0,     0,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   152,   153,   140,   141,   142,    14,    15,
     143,   144,   145,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    69,     0,   146,    33,
       0,     0,    70,    71,     0,    39,     0,     0,    72,     0,
      74,    41,     0,    75,   259,    76,     0,     0,    77,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   152,   153,   140,   141,   142,
      14,    15,   143,   144,   145,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     146,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,   362,
       0,     0,     0,    72,     0,    74,     0,     0,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     152,   153,   140,   141,   142,    14,    15,   143,   144,   145,
      19,    20,    21,    22,    23,     0,     0,   479,     0,     0,
       0,     0,     0,    69,     0,   146,    33,     0,     0,    70,
      71,     0,    39,     0,     0,    72,   397,    74,    41,     0,
      75,     0,    76,     0,     0,    77,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    66,   152,   153,   140,   141,   142,    14,    15,   143,
     144,   145,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,   146,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,    66,    70,    71,     0,     0,     0,     0,
      72,     0,    74,     0,     0,    75,     0,    76,     0,     0,
      77,     0,     0,     0,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   152,   153,   140,
     141,   142,    14,    15,   143,   144,   145,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
      69,     0,   146,    33,     0,     0,    70,    71,     0,    39,
       0,     0,    72,     0,    74,    41,   486,    75,     0,    76,
       0,     0,    77,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    66,   152,
     153,   140,   141,   142,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,   146,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
     261,     0,     0,     0,     0,    69,     0,     0,     0,     0,
      66,    70,    71,     0,     0,     0,     0,    72,     0,    74,
       0,   488,    75,     0,    76,     0,     0,    77,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   152,   153,   140,   141,   142,    14,
      15,   143,   144,   145,    19,    20,    21,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,   146,
      33,     0,     0,    70,    71,     0,    39,     0,     0,    72,
       0,    74,    41,     0,    75,     0,    76,     0,     0,    77,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    66,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,   146,    33,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,    66,    70,    71,
       0,     0,     0,     0,    72,     0,    74,     0,   490,    75,
       0,    76,     0,     0,    77,     0,     0,     0,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   152,   153,   140,   141,   142,    14,    15,   143,   144,
     145,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,   146,    33,     0,     0,
      70,    71,     0,    39,     0,     0,    72,     0,    74,    41,
       0,    75,     0,    76,     0,   677,    77,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    66,   152,   153,   140,   141,   372,    14,    15,
     143,   144,   145,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,   146,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,    66,    70,    71,     0,     0,     0,
       0,    72,     0,    74,     0,     0,    75,     0,    76,     0,
     159,    77,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   152,   153,
     140,   141,   142,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,   146,    33,     0,     0,    70,    71,     0,
      39,     0,     0,    72,     0,    74,    41,     0,    75,     0,
      76,     0,   856,    77,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     4,     5,     6,   685,     8,     0,     0,    66,
     152,   153,   140,   141,   686,    14,    15,   143,   144,   145,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,   146,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    66,    70,    71,     0,     0,     0,     0,    72,     0,
      74,   874,     0,    75,     0,    76,     0,     0,    77,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,   146,    33,     0,    70,    71,     0,     0,    39,     0,
      72,     0,    74,   257,    41,    75,     0,    76,     0,     0,
      77,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    66,   152,   153,
     140,   141,   142,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,   146,    33,     0,     0,     0,     0,     0,
      39,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,     0,     0,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     0,     0,    69,     0,     0,     0,     0,    66,
      70,    71,     0,     0,     0,     0,    72,     0,    74,  1179,
       0,    75,     0,    76,     0,     0,    77,     0,     0,     0,
      67,    68,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   152,   153,   140,   141,   142,    14,    15,
     143,   144,   145,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    69,     0,   146,    33,
       0,     0,    70,    71,     0,    39,     0,     0,    72,     0,
     169,    41,     0,    75,     0,    76,     0,     0,    77,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    66,   152,   153,   140,   141,   142,
      14,    15,   143,   144,   145,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
     146,    33,     0,     0,     0,     0,     0,    39,     0,     0,
       0,     0,     0,    41,     0,     0,     0,     0,     0,     0,
       0,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,    69,     0,     0,     0,     0,    66,    70,    71,     0,
       0,     0,     0,    72,     0,   171,     0,     0,    75,     0,
      76,     0,     0,    77,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     152,   153,   140,   141,   142,    14,    15,   143,   144,   145,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,   146,    33,     0,     0,    70,
      71,     0,    39,     0,     0,    72,     0,   173,    41,     0,
      75,     0,    76,     0,     0,    77,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    66,   152,   153,   140,   141,   142,    14,    15,   143,
     144,   145,    19,    20,    21,    22,    23,     0,     0,     0,
       0,     0,    67,    68,     0,     0,     0,   146,    33,     0,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
      41,     0,     0,     0,     0,     0,     0,     0,    49,    50,
      51,    52,     0,     0,    55,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,     0,     0,     0,     0,
      62,     0,     0,     0,     0,     0,     0,     0,    69,     0,
       0,     0,     0,    66,    70,    71,     0,     0,     0,     0,
      72,     0,   175,     0,     0,    75,     0,    76,     0,     0,
      77,     0,     0,     0,    67,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   152,   153,   140,
     141,   142,    14,    15,   143,   144,   145,    19,    20,    21,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
      69,     0,   146,    33,     0,     0,    70,    71,     0,    39,
       0,     0,    72,     0,    74,    41,     0,    75,     0,    76,
       0,     0,    77,    49,    50,    51,    52,     0,     0,    55,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,     0,     0,     0,     0,    62,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    66,   152,
     153,   140,   141,   142,    14,    15,   143,   144,   145,    19,
      20,    21,    22,    23,     0,     0,     0,     0,     0,    67,
      68,     0,     0,     0,   146,    33,     0,     0,     0,     0,
       0,    39,     0,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,    49,    50,    51,    52,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    60,    61,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,    69,     0,     0,     0,     0,
      66,    70,    71,     0,     0,     0,     0,    72,     0,   196,
       0,     0,    75,     0,    76,     0,     0,    77,     0,     0,
       0,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   152,   153,   140,   141,   142,    14,
      15,   143,   144,   145,    19,    20,    21,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,    69,     0,   146,
      33,     0,     0,    70,    71,     0,    39,     0,     0,    72,
       0,   198,    41,     0,    75,     0,    76,     0,     0,    77,
      49,    50,    51,    52,     0,     0,    55,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,    61,     0,     0,
       0,     0,    62,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    66,   152,   153,   140,   141,
     142,    14,    15,   143,   144,   145,    19,    20,    21,    22,
      23,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,   146,    33,     0,     0,     0,     0,     0,    39,     0,
       0,     0,     0,     0,    41,     0,     0,     0,     0,     0,
       0,     0,    49,    50,    51,    52,     0,     0,    55,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    60,    61,
       0,     0,     0,     0,    62,     0,     0,     0,     0,     0,
       0,     0,    69,     0,     0,     0,     0,    66,    70,    71,
       0,     0,     0,     0,    72,     0,    74,     0,     0,    75,
       0,   212,     0,     0,    77,     0,     0,     0,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
       0,   152,   153,   140,   141,   142,    14,    15,   143,   144,
     145,    19,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,    69,     0,   146,    33,     0,     0,
      70,    71,     0,    39,     0,     0,    72,     0,   360,    41,
       0,    75,     0,    76,     0,     0,    77,    49,    50,    51,
      52,     0,     0,    55,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    61,     0,     0,     0,     0,    62,
       0,     0,     0,     0,     0,     4,     5,     6,   685,     8,
       0,     0,    66,   152,   153,   140,   141,   686,    14,    15,
     143,   144,   145,    19,    20,    21,    22,    23,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,   146,    33,
       0,     0,     0,     0,     0,    39,     0,     0,     0,     0,
       0,    41,     0,     0,     0,     0,     0,     0,     0,    49,
      50,    51,    52,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,     0,     0,     0,
       0,    62,     0,     0,     0,     0,     0,     0,     0,    69,
       0,     0,     0,     0,    66,    70,    71,     0,     0,     0,
       0,    72,     0,   682,     0,     0,    75,     0,    76,     0,
       0,    77,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   152,   153,
     140,   141,   142,    14,    15,   143,   144,   145,    19,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,    69,     0,   146,    33,     0,     0,    70,    71,     0,
      39,     0,     0,    72,     0,   687,    41,     0,    75,     0,
      76,     0,     0,    77,    49,    50,    51,    52,     0,     0,
      55,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    61,     0,     0,     0,     0,    62,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,    66,
     152,   153,   140,   141,   142,    14,    15,   143,   144,   145,
      19,    20,    21,    22,    23,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,   146,    33,     0,     0,     0,
       0,     0,    39,     0,     0,     0,     0,     0,    41,     0,
       0,     0,     0,     0,     0,     0,    49,    50,    51,    52,
       0,     0,    55,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    60,    61,     0,     0,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,    69,     0,     0,     0,
       0,    66,    70,    71,     0,     0,     0,     0,    72,     0,
     893,     0,     0,    75,     0,    76,     0,     0,    77,     0,
       0,     0,    67,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,     0,   138,   139,   140,   141,   142,
      14,    15,   143,   144,   145,    19,    20,    21,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,    69,     0,
     146,     0,     0,     0,    70,    71,     0,    39,     0,     0,
      72,     0,   917,    41,     0,    75,     0,    76,     0,     0,
      77,    49,    50,    51,    52,     0,     0,    55,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    60,    61,     0,
       0,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -577,  -577,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,     0,     0,     0,     0,     0,    70,
      71,     0,     0,     0,     0,    72,     0,    74,     0,  -577,
      75,     0,    76,     0,     0,    77,  -577,  -577,  -577,     0,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,     0,     0,     0,  -577,
       0,     0,     0,     0,     0,     0,     0,  -577,  -577,     0,
       0,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,     0,  -577,  -577,     0,     0,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,     0,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,     0,     0,  -577,  -577,  -577,
    -577,  -577,  -577,     0,  -577,  -577,  -577,     0,     0,  -577,
    -577,  -577,     0,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,     0,
       0,     0,  -577,     0,     0,     0,     0,     0,     0,     0,
    -577,  -577,     0,     0,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,
    -577,  -577,  -577,  -577,  -577,     0,  -577,  -577,     0,     0,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,     0,
    -577,  -577,  -577,  -577,  -577,  -577,  -577,  -577,     0,     0,
    -577,  -577,  -577,  -577,  -577,     0,     0,  -577,  -577,  -577,
       0,     0,   509
};

static const yytype_int16 yycheck[] =
{
       2,     2,   275,     0,    31,   914,    19,   716,    58,   168,
       2,   644,     2,    63,    64,    15,    15,   650,    20,    35,
      27,    27,    31,     0,    15,    27,    28,    29,    30,    31,
     446,    15,    34,    35,    36,   110,    27,    39,    40,    15,
     418,    43,    44,   113,   114,    27,   346,    60,   348,    51,
      52,    12,   186,    55,    56,  1123,    69,    70,    71,    92,
     122,   123,   170,    65,    77,    27,    11,    12,    31,   181,
      15,   183,    74,    75,    11,    12,    27,    27,    15,    31,
     188,    11,    12,   499,    15,    15,   386,   387,    51,    11,
      15,    11,    31,   168,   182,    34,    35,   185,   168,    51,
      11,    12,    45,    46,    15,    44,   168,    31,    47,    48,
     112,   186,    51,    52,   116,   117,   118,   119,   120,   121,
     188,   123,   124,   125,   126,   168,   128,    51,    31,   131,
      75,    76,    77,   135,   136,    89,    45,    46,    75,    76,
      77,   122,   123,   186,   170,    75,    76,    77,    51,    15,
     182,    15,   185,   168,   156,   188,   158,   189,    80,    81,
      80,    81,   188,    27,    75,    76,    77,   169,   183,   171,
      36,   173,   188,   175,  1083,   190,    11,    12,    15,   188,
      15,   188,   188,  1251,   183,   169,   168,   168,    15,   188,
     190,    11,    12,   169,   196,    15,   198,   188,   831,   201,
      27,   203,   204,    31,   206,   207,   168,   209,   210,   211,
     212,   213,    11,    12,    78,    31,    15,   168,   168,   182,
     181,   185,   183,    51,   188,    31,   189,   181,  1296,   183,
     182,   233,  1300,    11,    12,    51,   110,    15,   169,   184,
      75,    76,    77,   182,   169,    51,   189,   184,    89,   182,
     189,    78,   185,   303,   184,    75,    76,    77,   182,    15,
     262,   262,   264,   265,   266,   267,   268,   269,   270,   271,
     262,   273,   262,   184,     6,  1343,    75,    76,    77,   182,
     189,   697,   441,    15,  1352,    31,    31,   314,   315,   316,
     317,   318,   319,   320,   168,    31,    31,    75,    76,    77,
      15,  1369,    15,  1371,   168,    51,    51,   170,   182,    11,
      12,    13,    27,    15,    27,    51,    51,    15,   182,    92,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   183,    36,  1403,    31,  1405,   188,    41,
     181,   168,   183,    82,    15,   347,   724,   165,   166,   184,
      21,    90,    91,   168,   182,   182,    51,   122,   360,   775,
     776,   189,   110,    78,   184,    78,   182,   188,   183,   134,
    1279,  1080,   183,   189,    72,   190,   182,   188,   380,   185,
     382,   383,   384,   189,    92,   184,   388,   389,   390,   391,
     392,   393,   394,   158,   168,   773,    92,    68,   137,   138,
      84,    85,    86,   405,   406,   407,   184,   409,   410,   183,
     412,   413,   185,   110,   168,   188,   190,   444,   445,    13,
     168,   423,   424,   425,   426,   427,   428,   429,   182,   137,
     138,   433,   434,   170,   182,   174,   182,   182,    31,   185,
     185,   137,   138,   189,   189,    13,   182,   182,   188,     2,
     447,   188,   110,   168,   189,   168,   458,   459,    51,   461,
     462,   110,   464,   465,    34,   188,   174,   182,   470,   182,
     472,   168,    45,    46,    15,   477,   478,   168,   174,    32,
      33,   183,    52,   110,   486,   170,   488,   182,   490,   186,
      82,   493,    45,    46,   189,   186,   113,   114,   183,    82,
      53,    54,   504,   505,   506,   189,   503,    90,    91,    82,
     168,   183,    65,    89,   814,   815,   188,    11,   896,   168,
     522,   523,   524,   525,   182,    31,    66,    67,    34,    35,
      70,   122,   123,   182,   113,   114,    31,   183,    44,    34,
      35,    47,    48,   170,   190,    51,   113,   114,   926,    44,
     928,   824,    47,    48,   137,   138,    51,    52,   110,    31,
      11,   188,    34,    35,    15,    31,   183,    31,    34,    35,
      34,    35,    44,   107,   108,    47,    48,   168,    44,    51,
      44,    47,    48,    47,    48,    51,    52,    51,   168,   182,
      11,   174,   185,   146,    15,    31,    11,    12,    34,    35,
      15,   168,   182,   981,   982,    11,    66,    67,    44,   170,
      70,    47,    48,   113,   114,    51,   137,   138,   170,   122,
     123,   174,   183,   176,   177,   168,    11,   161,   162,   163,
     395,   168,   634,   635,    31,   106,   188,    34,    35,   182,
     122,   123,   182,   645,   197,   182,   199,    44,   650,   189,
      47,    48,   168,   174,    51,    52,   170,     8,     9,    10,
      75,    76,    77,   120,    15,   168,   431,   183,   168,   183,
     168,   436,   170,   675,   190,   975,   182,   442,   167,   185,
     682,   122,   123,   189,   237,   687,   168,   182,   690,   691,
     188,   693,   457,   695,   189,   170,  1074,   699,   700,   183,
     183,   181,   467,    11,    12,   188,   190,    15,   183,   262,
     182,   181,  1090,   185,   183,   717,   182,   189,   182,   188,
     168,   185,   182,   189,   726,   726,   728,   168,   182,   189,
     182,   185,    15,   185,   726,   189,   726,   132,   133,   134,
     135,   136,   182,    15,   182,   185,   182,   185,   513,   185,
     515,   516,   754,   189,  1132,  1133,   113,   114,   115,   116,
     117,   118,   119,   182,   766,   182,   183,    75,    76,    77,
     189,   182,    93,    94,    95,    96,   778,   778,   189,   183,
     175,   176,   183,   182,   188,   182,   778,   188,   778,   791,
     792,   793,   182,   182,   186,   182,   185,   799,   800,   189,
     802,   803,   189,   805,   806,   807,   808,   809,   810,   811,
     812,   168,   182,  1229,   182,   182,  1232,   182,   182,   189,
     182,   189,   189,   825,   189,   189,   183,   189,   830,  1129,
      15,   152,   153,   154,   155,   156,   157,   158,   840,   160,
     182,   843,   175,   176,   846,   182,   182,   189,   182,   170,
     171,   853,   189,   189,   182,   189,   858,   622,   182,   861,
     182,   189,   864,   182,   170,   189,   182,   189,   633,   182,
     189,   636,   186,   189,    15,   182,   189,   642,   182,   644,
     181,   185,   189,   885,   886,   139,   140,   141,   142,   143,
     144,   893,   182,   184,   185,   185,   110,   111,   900,   113,
     114,   115,   116,   117,   118,   119,  1322,  1323,   113,   114,
     115,   116,   117,   118,   119,   917,   182,   182,   172,   173,
      11,    12,    13,   189,   189,   182,   182,   929,   930,   931,
     188,   182,   189,   189,   181,   937,   182,   702,   189,   492,
     182,   494,   182,   189,   182,    79,   711,   189,   182,   189,
     182,   189,   954,   182,   168,   189,    62,   189,   182,   182,
     189,   182,   168,   168,   966,   189,   189,   168,   189,   183,
     182,     3,     4,     5,     6,     7,   185,   189,   183,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,   182,    15,   188,   185,  1000,  1001,
    1002,  1003,  1004,  1005,    36,    37,   182,    27,   182,    29,
      30,    43,   182,   189,   182,   189,   188,    49,    38,    39,
     185,   189,   187,    43,   183,    57,    58,    59,    60,    49,
      50,    63,   182,   184,   185,   185,   182,    57,    58,    59,
      60,    73,    74,   189,   182,   182,  1048,    79,    68,   188,
     182,   189,   189,   321,   322,   323,  1058,   189,   182,   188,
      92,   826,   827,   184,   185,   189,   831,   832,   184,   185,
     835,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     182,   113,   114,   185,   184,   185,   112,   326,   327,   328,
     329,   330,   331,   332,  1096,  1097,  1098,  1099,  1100,  1101,
    1102,  1103,   182,   184,   185,   185,  1108,   305,   306,   307,
     308,   309,   310,   311,   312,   313,  1118,   182,   883,   121,
     185,   182,   124,   125,   126,   127,   128,   129,   130,   131,
     274,   183,   276,   277,   278,  1137,    11,   169,   164,    14,
      15,    16,    17,   175,   176,   182,   189,    11,   185,   181,
      14,   183,    16,    17,   186,   187,   188,  1159,  1160,   191,
     146,   147,   715,   716,   150,   151,   719,   720,   721,   184,
     185,  1173,  1174,   726,   182,   177,   178,   179,   185,   732,
     187,   171,  1184,  1185,   185,   185,   187,   187,   185,   185,
     187,   187,   185,   182,   187,   748,   749,   750,   185,   182,
     187,  1203,  1204,  1205,   184,   185,   184,   185,    15,    32,
      33,   764,   184,   185,   767,   184,   185,   182,  1220,   182,
      27,   182,    29,    30,  1226,   778,   184,   185,   184,   185,
     182,    38,    39,  1230,   182,  1237,    43,   184,   185,   182,
    1242,   182,    49,    50,   184,   185,   184,   185,    15,    15,
      57,    58,    59,    60,    21,   189,  1258,  1259,   184,   185,
      27,    68,    29,    30,   184,   185,   184,   185,   184,   185,
     183,    38,    39,   184,   185,   183,    43,   184,   185,   184,
     185,   187,    49,    50,   122,   123,   122,   123,   189,    15,
      57,    58,    59,    60,   171,  1060,    15,  1299,  1299,   184,
     185,    27,    15,    29,    30,    15,  1071,  1299,   182,  1299,
     182,  1313,    38,    39,   122,   123,  1318,    43,   122,   123,
     168,   184,   185,    49,    50,   184,   185,   184,   185,   184,
     185,    57,    58,    59,    60,    32,    33,   168,  1340,   184,
     185,   184,   185,   182,  1346,  1347,  1348,   184,   185,   184,
     185,   184,   185,    15,  1356,    32,  1358,   184,   185,   184,
     185,    15,   184,   185,    11,   918,    32,    33,   184,   185,
     184,   185,    32,    33,   184,   185,   189,  1379,   184,   185,
      50,    51,  1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,
     184,   185,   184,   185,   324,   325,   182,   182,   182,   182,
     182,    15,   955,   183,   183,   182,  1408,  1409,  1410,    11,
     181,   181,   164,   182,   182,   109,    15,   970,   182,   182,
     182,  1186,   182,   182,   182,  1190,     3,     4,     5,     6,
       7,   182,   168,   168,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,   189,   171,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,   189,   187,
     171,   182,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    15,
     182,   182,    69,    70,   182,   182,    73,    74,    89,   182,
     168,   182,    79,   182,   182,    15,    83,   182,   182,   182,
      87,    88,    11,   182,    11,    92,   168,   182,   182,   182,
     171,   182,   189,   182,   182,   182,   182,  1080,   171,   182,
     185,  1084,  1085,  1086,   182,    15,   113,   114,    15,   189,
     189,   182,   188,   182,   182,   171,   182,   187,    89,    15,
     187,   171,   184,    15,   182,   182,    11,   182,   182,  1112,
     182,    11,    11,  1116,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,    15,    15,   182,
     182,    32,   169,   182,   189,    66,    15,    15,   175,   176,
      15,   189,   182,   182,   181,   182,   183,   182,   182,   186,
     182,   188,   189,   182,   191,   182,   184,   183,   182,   182,
      15,   182,    89,   182,    89,   182,    15,    15,    11,    11,
     182,   189,   182,   182,     8,     8,     8,     3,     4,     5,
       6,     7,  1195,  1196,   168,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,   182,   171,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,   184,
      15,   184,    82,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      15,    15,    15,    69,    70,    15,   182,    73,    74,   182,
     182,   189,   182,    79,   183,    15,   184,    83,   182,   182,
      15,    87,    88,   184,   182,   182,    92,    15,   188,   182,
     182,   182,    15,  1340,   182,   182,   182,   182,   182,   182,
     182,    33,  1295,   182,   182,   182,  1299,   113,   114,   184,
     182,   928,   771,   304,   687,    -1,   279,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1335,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,    -1,
     186,    -1,   188,   189,    -1,   191,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,  1406,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      -1,    -1,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    -1,
      -1,    -1,    69,    70,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    83,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,   176,
      -1,    -1,    -1,    -1,   181,   182,   183,    -1,    -1,   186,
      -1,   188,   189,    -1,   191,     3,     4,     5,     6,     7,
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
     188,   189,    -1,   191,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    -1,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    -1,    -1,    -1,    -1,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    -1,    -1,
      69,    70,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,   113,   114,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,
     169,    -1,    -1,    -1,    -1,    -1,   175,   176,    -1,    73,
      74,    -1,   181,   182,   183,    79,    -1,   186,    -1,   188,
      -1,    -1,   191,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,   113,
     114,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,
      59,    60,    -1,    -1,    63,   169,    -1,    -1,    -1,    -1,
      -1,   175,   176,    -1,    73,    74,    -1,   181,   182,   183,
      79,    -1,   186,    -1,   188,    -1,   190,   191,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    36,    37,    -1,   175,   176,    -1,    -1,
      43,    -1,   181,    -1,   183,   184,    49,   186,    -1,   188,
      -1,   190,   191,    -1,    57,    58,    59,    60,    -1,    -1,
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
     183,   184,    -1,   186,    -1,   188,    -1,   190,   191,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,
     181,    -1,   183,   184,    49,   186,    -1,   188,    -1,   190,
     191,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
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
     175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,
      -1,   186,   187,   188,    -1,   190,   191,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,    36,
      37,    -1,   175,   176,    -1,    -1,    43,    -1,   181,    -1,
     183,   184,    49,   186,    -1,   188,    -1,   190,   191,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    92,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,
      -1,    -1,    -1,    -1,   181,    -1,   183,   184,    -1,   186,
      -1,   188,    -1,   190,   191,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,
     175,   176,    -1,    43,    -1,    -1,   181,    -1,   183,    49,
      -1,   186,   187,   188,    -1,   190,   191,    57,    58,    59,
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
      -1,   181,   182,   183,    -1,    -1,   186,    -1,   188,   189,
      -1,   191,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    -1,    36,    37,    -1,   175,   176,    -1,
      -1,    43,    -1,   181,    -1,   183,   184,    49,   186,    -1,
     188,    -1,   190,   191,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      92,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,
      -1,   183,   184,    -1,   186,    -1,   188,    -1,   190,   191,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    -1,    36,    37,    -1,   175,   176,    -1,    -1,    43,
      -1,   181,    -1,   183,   184,    49,   186,    -1,   188,    -1,
     190,   191,    -1,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
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
     184,    -1,   186,    -1,   188,    -1,   190,   191,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,   181,
      -1,   183,   184,    49,   186,    -1,   188,    -1,   190,   191,
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
     176,    -1,    -1,    -1,    -1,   181,   182,   183,    -1,    -1,
     186,    -1,   188,   189,    -1,   191,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   169,    -1,    36,    37,    -1,
      -1,   175,   176,    -1,    43,    -1,    -1,   181,   182,   183,
      49,    -1,   186,    -1,   188,   189,    -1,   191,    57,    58,
      59,    60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    92,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    -1,    -1,    -1,    92,   175,   176,    -1,    -1,
      -1,    -1,   181,   182,   183,    -1,    -1,   186,    -1,   188,
      -1,    -1,   191,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    36,    37,    -1,    -1,   175,   176,
      -1,    43,    -1,    -1,   181,   182,   183,    49,    -1,   186,
      -1,   188,    -1,    -1,   191,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      92,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    29,
      -1,   113,   114,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    49,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    74,    -1,    -1,    -1,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    -1,
      -1,    -1,    92,   175,   176,    -1,    -1,    -1,    -1,   181,
     182,   183,    -1,    -1,   186,    -1,   188,    -1,    -1,   191,
      -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,
      -1,    36,    37,    -1,    -1,   175,   176,    -1,    43,    -1,
      -1,   181,    -1,   183,    49,    -1,   186,    -1,   188,    -1,
      -1,   191,    57,    58,    59,    60,    -1,    -1,    63,    -1,
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
      -1,   186,    -1,   188,    -1,    -1,   191,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    36,    37,
      -1,    -1,   175,   176,    -1,    43,    -1,    -1,   181,    -1,
     183,    49,    -1,   186,   187,   188,    -1,    -1,   191,    57,
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
      -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,   177,
      -1,    -1,    -1,   181,    -1,   183,    -1,    -1,   186,    -1,
     188,    -1,    -1,   191,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,   175,
     176,    -1,    43,    -1,    -1,   181,   182,   183,    49,    -1,
     186,    -1,   188,    -1,    -1,   191,    57,    58,    59,    60,
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
     181,    -1,   183,    -1,    -1,   186,    -1,   188,    -1,    -1,
     191,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    36,    37,    -1,    -1,   175,   176,    -1,    43,
      -1,    -1,   181,    -1,   183,    49,   185,   186,    -1,   188,
      -1,    -1,   191,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    92,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,   113,
     114,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    57,    58,    59,    60,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    74,    -1,    -1,    -1,    -1,    79,    -1,    -1,
      82,    -1,    -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,
      92,   175,   176,    -1,    -1,    -1,    -1,   181,    -1,   183,
      -1,   185,   186,    -1,   188,    -1,    -1,   191,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    36,
      37,    -1,    -1,   175,   176,    -1,    43,    -1,    -1,   181,
      -1,   183,    49,    -1,   186,    -1,   188,    -1,    -1,   191,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    92,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,
      -1,    -1,    -1,    -1,   181,    -1,   183,    -1,   185,   186,
      -1,   188,    -1,    -1,   191,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,
     175,   176,    -1,    43,    -1,    -1,   181,    -1,   183,    49,
      -1,   186,    -1,   188,    -1,   190,   191,    57,    58,    59,
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
      -1,   181,    -1,   183,    -1,    -1,   186,    -1,   188,    -1,
     190,   191,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    36,    37,    -1,    -1,   175,   176,    -1,
      43,    -1,    -1,   181,    -1,   183,    49,    -1,   186,    -1,
     188,    -1,   190,   191,    57,    58,    59,    60,    -1,    -1,
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
     183,   184,    -1,   186,    -1,   188,    -1,    -1,   191,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      -1,    36,    37,    -1,   175,   176,    -1,    -1,    43,    -1,
     181,    -1,   183,   184,    49,   186,    -1,   188,    -1,    -1,
     191,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
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
      -1,   186,    -1,   188,    -1,    -1,   191,    -1,    -1,    -1,
     113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    36,    37,
      -1,    -1,   175,   176,    -1,    43,    -1,    -1,   181,    -1,
     183,    49,    -1,   186,    -1,   188,    -1,    -1,   191,    57,
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
     188,    -1,    -1,   191,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,   175,
     176,    -1,    43,    -1,    -1,   181,    -1,   183,    49,    -1,
     186,    -1,   188,    -1,    -1,   191,    57,    58,    59,    60,
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
     181,    -1,   183,    -1,    -1,   186,    -1,   188,    -1,    -1,
     191,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     169,    -1,    36,    37,    -1,    -1,   175,   176,    -1,    43,
      -1,    -1,   181,    -1,   183,    49,    -1,   186,    -1,   188,
      -1,    -1,   191,    57,    58,    59,    60,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,
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
      -1,    -1,   186,    -1,   188,    -1,    -1,   191,    -1,    -1,
      -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,    36,
      37,    -1,    -1,   175,   176,    -1,    43,    -1,    -1,   181,
      -1,   183,    49,    -1,   186,    -1,   188,    -1,    -1,   191,
      57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,    -1,
      -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    92,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    57,    58,    59,    60,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,
      -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,    -1,    -1,    -1,    -1,    92,   175,   176,
      -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    -1,   186,
      -1,   188,    -1,    -1,   191,    -1,    -1,    -1,   113,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    36,    37,    -1,    -1,
     175,   176,    -1,    43,    -1,    -1,   181,    -1,   183,    49,
      -1,   186,    -1,   188,    -1,    -1,   191,    57,    58,    59,
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
      -1,   181,    -1,   183,    -1,    -1,   186,    -1,   188,    -1,
      -1,   191,    -1,    -1,    -1,   113,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   169,    -1,    36,    37,    -1,    -1,   175,   176,    -1,
      43,    -1,    -1,   181,    -1,   183,    49,    -1,   186,    -1,
     188,    -1,    -1,   191,    57,    58,    59,    60,    -1,    -1,
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
     183,    -1,    -1,   186,    -1,   188,    -1,    -1,   191,    -1,
      -1,    -1,   113,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   169,    -1,
      36,    -1,    -1,    -1,   175,   176,    -1,    43,    -1,    -1,
     181,    -1,   183,    49,    -1,   186,    -1,   188,    -1,    -1,
     191,    57,    58,    59,    60,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   113,   114,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,    -1,    -1,    -1,    -1,   175,
     176,    -1,    -1,    -1,    -1,   181,    -1,   183,    -1,    82,
     186,    -1,   188,    -1,    -1,   191,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,    -1,    -1,    -1,   112,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,   121,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,    -1,   146,   147,    -1,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,    -1,   170,   171,   172,
     173,   174,    82,    -1,   177,   178,   179,    -1,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,    -1,
      -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,   121,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,   146,   147,    -1,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,    -1,
     170,   171,   172,   173,   174,    -1,    -1,   177,   178,   179,
      -1,    -1,   182
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   193,   194,     0,     3,     4,     5,     6,     7,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    69,    70,
      73,    74,    79,    83,    87,    88,    92,   113,   114,   169,
     175,   176,   181,   182,   183,   186,   188,   191,   195,   196,
     198,   199,   200,   201,   202,   204,   205,   209,   213,   214,
     215,   218,   223,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   238,   239,   242,   243,
     245,   246,   110,   111,   113,   114,   115,   116,   117,   118,
     119,   168,   183,   110,   168,   186,   110,   170,   188,   113,
     114,   168,    12,   181,   183,   186,   188,   170,    11,    12,
      13,    14,    15,    18,    19,    20,    36,   223,   238,   243,
     245,   246,    11,    12,   187,   225,    15,   182,   183,   190,
     225,   244,    11,    12,    13,    15,    36,    41,   183,   183,
     225,   183,   225,   183,   225,   183,   225,   225,   234,    15,
     183,   223,   204,   223,   225,   182,   225,   182,   225,    11,
      15,    15,   225,   225,    13,    13,   183,   225,   183,   225,
     223,   183,   223,    31,    51,   182,    31,    51,   182,   188,
     224,   224,   188,   224,   225,   225,   223,   188,   223,   225,
     182,   225,    15,   212,   212,    15,    36,   238,   212,   212,
      11,    12,    13,    29,   223,   225,    11,   240,    11,    11,
     238,   238,   238,    15,    27,    29,    30,    38,    39,    43,
      49,    50,    57,    58,    59,    60,   241,   184,   225,   187,
     225,    82,   194,   238,    31,    34,    35,    44,    47,    48,
      51,    52,   182,    89,   107,   108,   161,   162,   163,   106,
      93,    94,    95,    96,   152,   153,   154,   155,   156,   157,
     158,   160,   170,   171,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   112,   164,   139,   140,   141,   142,   143,
     144,   172,   173,   120,   146,   147,   150,   151,    82,    90,
      91,   137,   138,   174,   165,   166,   132,   133,   134,   135,
     136,   175,   176,   121,   124,   125,   126,   127,   128,   129,
     130,   131,   177,   178,   179,   167,   181,   168,   181,   225,
      15,   225,   225,   225,   225,   225,   225,   184,   244,   225,
     183,   225,   177,   225,   225,    15,   225,   225,   182,    15,
      21,    68,    15,   184,   190,   220,   244,   225,   225,    15,
     186,   170,   188,   186,   188,   170,   181,   181,   115,   116,
     117,   118,   119,   168,   168,   185,   187,   182,   225,   184,
     225,   244,    15,   169,   182,   185,   110,   168,   182,   110,
     168,   182,   110,   168,   182,    11,    12,    13,    15,    11,
      15,   240,   225,    31,    34,    35,    44,    47,    48,    51,
     182,   185,   225,    31,    51,   182,   185,   225,   223,   225,
     223,    92,   185,   223,    90,    91,   183,   188,   210,    11,
      12,    15,    75,    76,    77,   222,   182,   185,    31,    51,
     182,    31,    51,   182,   168,   168,    79,   185,   188,   170,
     188,   170,   188,   225,   223,   225,   223,    34,    52,    28,
     225,   225,   225,   225,   225,   225,   185,   225,   185,   225,
     185,   225,    62,    45,    46,   206,   182,   216,   188,   183,
      15,    72,   188,   188,   168,   168,   168,   170,   225,   182,
     182,   185,   223,   183,   184,   185,   185,   187,   189,    12,
      13,    14,    29,    30,    38,    39,    55,    56,   189,   225,
     243,   246,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   228,   226,   228,   228,   228,   229,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
     230,   230,   230,   230,   230,   230,   230,   230,   230,   230,
       8,     9,    10,   212,   231,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   234,   234,   234,   234,
     234,   235,   235,   235,   236,   236,   237,   237,   237,   237,
     237,   237,   237,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   241,   225,    21,   241,
     182,   182,   183,   182,   182,   182,   182,   182,   182,   184,
     182,   225,   182,   185,   175,   176,   185,   187,   182,   171,
     189,   182,   183,    15,   183,    89,    15,   184,   185,   184,
     185,   187,   189,   171,   225,    15,   225,   225,   225,    15,
     241,   241,   225,   225,   225,   225,   225,   225,   225,   187,
     244,   182,   182,   184,   170,   183,    15,   190,   225,   225,
     225,   225,   183,   225,   225,     6,    15,   183,   221,   225,
     110,   168,   182,   168,   182,   168,   182,   183,   210,   168,
     168,   184,   185,   225,   225,   225,   225,   225,   225,   225,
     244,   185,   225,   225,   244,   184,    32,    33,   197,   184,
     240,   244,   234,   234,   184,   222,   194,    27,   168,    11,
      80,    81,   184,   185,   244,   225,   225,   225,   225,   225,
     225,   182,   244,   207,    15,   225,    15,   225,   184,    32,
     184,   225,   225,    11,   182,   182,   182,   182,   182,   189,
     225,   225,   225,   223,   225,   223,    45,    46,   189,    15,
      27,   188,   217,   184,   222,   183,   183,   219,   194,   225,
     225,   225,    15,   182,    11,   184,   244,   184,   244,   187,
     244,   186,   188,   168,   225,   225,   225,   189,   225,    31,
      51,   189,    31,    51,   189,    31,    34,    35,    44,    47,
      48,    51,    52,   189,   181,   181,   182,   182,   182,   182,
     182,   182,   182,   182,   109,   168,   183,   190,   182,    15,
     168,   183,   190,   184,   244,   185,   244,   225,   225,   244,
     168,   122,   123,   168,   122,   123,   168,   184,   244,   184,
     220,   244,   225,   183,    15,   190,   190,   220,   168,   122,
     123,   168,   122,   123,   168,   187,   171,   189,   187,   189,
     171,   187,   182,    15,   184,   225,    15,   169,   182,   182,
     182,   225,   182,   185,   182,    89,    89,   221,   182,   185,
     182,   225,   225,   183,   225,   225,   184,   222,   225,   225,
     168,   244,   182,   182,   182,   182,   182,   182,   182,   182,
     244,   182,   182,   182,   223,   196,   223,   183,   225,   223,
     223,    92,   223,    92,    92,    27,   208,   210,   184,    29,
      30,    39,   189,   225,    15,   225,    27,   168,    11,    11,
     223,    11,    12,    15,    75,    76,    77,   182,   182,   182,
     182,   182,   182,   182,    45,    46,   189,   171,   189,   171,
     189,   223,   223,   223,   182,   182,   168,   225,   223,   189,
     225,   223,    15,    15,   217,    66,    67,    70,   182,   189,
     210,   184,   184,   222,   222,    84,    85,    86,   189,   189,
     182,   182,   182,   171,   184,   184,   187,   225,   225,   225,
      31,    35,    44,    47,    48,    51,   189,   189,   189,   189,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   241,   241,   226,   225,   184,   244,   244,   225,
     184,   220,   244,   244,   182,   184,   244,   182,   187,   187,
     187,   225,   225,   225,   184,   184,   184,   225,    89,    15,
      15,   184,   225,   225,   225,   171,   184,   170,   183,    15,
     185,   244,   225,   225,   184,     6,    15,   182,   182,   225,
     182,   185,   182,   210,   184,   182,   182,   225,   184,   184,
      32,   197,   225,   223,    11,    11,    11,    15,    27,   210,
     208,   210,   225,   225,   189,   225,    31,    34,    35,    44,
      47,    48,    51,    52,   189,    15,   225,    27,   168,    11,
      80,    81,   225,   223,   182,   182,    32,   225,   182,   223,
     189,   189,    68,   241,    11,    14,    15,    16,    17,    66,
     210,   210,   184,   184,    15,    15,    15,   168,   187,   189,
     189,   225,   225,   225,   225,   225,   225,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,   168,
     168,   182,   184,   182,   184,   184,   182,   184,   182,   182,
     182,   184,   225,   183,   183,   182,   182,   182,    15,   184,
     225,   244,   182,   182,    89,    89,   185,   244,   210,   182,
     185,   182,   182,   196,   223,   184,    32,   197,   223,   223,
     223,    15,   210,    31,    35,    51,   189,   189,   189,   225,
     225,   225,   225,   225,   225,   225,   225,    15,   225,    27,
     168,    11,    11,   223,   189,   223,   182,   203,   225,   183,
     188,   211,   183,   211,    15,    27,    78,   168,   182,    15,
      27,    78,   168,   182,    11,    14,    16,    17,   182,   182,
      68,   241,   210,   210,     8,     8,     8,   225,   168,   168,
     189,   189,   189,   189,   189,   189,   225,   225,   182,   225,
     225,   171,   184,   184,   225,   225,   244,   182,   244,   223,
     223,   225,   225,   225,   189,   189,   189,   189,   189,   189,
     189,   189,    15,   225,   225,   184,   184,   222,    82,   194,
     184,   222,    15,    15,   182,   225,    15,    15,   182,   225,
      15,    27,    78,   168,   182,    15,    27,    78,   168,   182,
     182,   182,   183,   183,   211,   182,   225,   225,   189,   189,
     184,   184,   182,   184,   182,    32,   197,   189,   189,   189,
     182,   223,   211,   184,   189,    14,    29,    30,    39,   189,
     225,   211,   184,   182,   182,   182,   168,   182,   168,   182,
     182,    15,    15,   182,   225,    15,    15,   182,   225,   184,
     222,   184,   222,   189,   189,   182,   223,   203,   211,   168,
     225,   225,   189,   225,    31,    34,    35,    44,    47,    48,
      51,    52,   189,   211,   225,   225,   182,   182,   182,   182,
     182,   182,   211,   184,   211,   184,   184,   225,    31,    35,
      51,   189,   189,   189,   225,   225,   225,   225,   225,   225,
     225,   225,   182,   182,   211,   211,   223,   189,   225,   225,
     225,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   192,   193,   194,   194,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   195,   195,   195,   195,   195,   195,   196,
     196,   196,   196,   196,   196,   196,   196,   197,   197,   197,
     197,   197,   197,   198,   198,   199,   199,   199,   199,   200,
     200,   201,   201,   201,   202,   202,   202,   203,   204,   204,
     204,   204,   204,   204,   205,   205,   206,   206,   207,   207,
     208,   208,   209,   209,   209,   209,   209,   209,   209,   209,
     209,   209,   209,   209,   209,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   212,   213,   214,   215,   216,   216,   216,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   218,   219,   219,
     219,   219,   220,   220,   220,   220,   220,   221,   221,   221,
     221,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   224,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     226,   226,   227,   227,   227,   227,   227,   228,   228,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   230,   230,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   232,   232,
     233,   233,   233,   233,   233,   233,   233,   233,   234,   234,
     234,   234,   235,   235,   235,   236,   236,   236,   236,   236,
     236,   236,   236,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   238,   238,   238,   238,
     238,   238,   238,   239,   239,   240,   240,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     242,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   245,   245,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246
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
       2,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     4,     3,     4,     6,     4,     3,     6,     6,
       5,     3,     3,     6,     5,     4,     4,     6,     6,     5,
       3,     6,     5,     3,     5,     5,     5,     4,     2,     2,
       3,     4,     3,     4,     2,     3,     4,     2,     1,     1,
       3,     2,     2,     3,     5,     4,     5,     4,     5,     7,
       6,     7,     2,     3,     4,     5,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     1,
       1,     1,     1,     1,     4,     6,     6,     6,     1,     4,
       4,     4,     4,     5,     5,     5,     5,     4,     5,     5,
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
#line 4334 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 765 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 4340 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 766 "raku.y"
                     { if ((yyvsp[0].node) && (yyvsp[0].node)->line == 0) (yyvsp[0].node)->line = (yylsp[0]).first_line; (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 4346 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 770 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 4352 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 772 "raku.y"
        { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), nul); }
#line 4358 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 774 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4364 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 776 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4370 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 778 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4376 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 780 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4383 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 783 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4389 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 785 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4395 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 787 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4401 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 789 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4407 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 791 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4415 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 795 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4421 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 797 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4427 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 799 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4433 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 801 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4439 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 803 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4447 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 807 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4455 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 811 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4461 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 813 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4469 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 817 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4477 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 821 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4483 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 823 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4489 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 825 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4495 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 827 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4501 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 829 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4507 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 831 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4515 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 835 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4523 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 839 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4529 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 841 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4535 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 843 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4541 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 845 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4547 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 847 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4553 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 849 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4559 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 851 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4565 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 853 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4571 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 855 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4577 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 857 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4583 "raku.tab.c"
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
#line 4595 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 867 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4601 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 869 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4607 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 871 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4613 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 873 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4619 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 875 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4625 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 877 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4631 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 879 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4638 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 882 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4645 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 885 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4651 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 887 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4658 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 890 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4665 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 893 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4671 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 895 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4679 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 899 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4685 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 901 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4691 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 903 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4697 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 905 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4703 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 907 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4710 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 910 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4717 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 913 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4723 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 915 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4730 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 918 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4737 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 921 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4744 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 924 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4750 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 926 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-6].sval)),mc); }
#line 4759 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 931 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-5].sval)),mc); }
#line 4767 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 935 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-3].sval)),mc); }
#line 4775 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 939 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4782 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 942 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4789 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 945 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4797 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 949 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4804 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_SCALAR_IDX '[' expr ']' '=' expr ';'  */
#line 952 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4811 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_SCALAR_ANGLE '<' IDENT '>' '=' expr ';'  */
#line 955 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4818 "raku.tab.c"
    break;

  case 76: /* stmt: VAR_SCALAR_HASH '{' expr '}' '=' expr ';'  */
#line 958 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4825 "raku.tab.c"
    break;

  case 77: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 961 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4832 "raku.tab.c"
    break;

  case 78: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 964 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4839 "raku.tab.c"
    break;

  case 79: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 967 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4846 "raku.tab.c"
    break;

  case 80: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 970 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4853 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_IF expr ';'  */
#line 973 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4859 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_UNLESS expr ';'  */
#line 975 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4865 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WHILE expr ';'  */
#line 977 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4871 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_UNTIL expr ';'  */
#line 979 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4877 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_FOR expr ';'  */
#line 981 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4884 "raku.tab.c"
    break;

  case 86: /* stmt: expr KW_WITH expr ';'  */
#line 984 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4890 "raku.tab.c"
    break;

  case 87: /* stmt: expr KW_WITHOUT expr ';'  */
#line 986 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4896 "raku.tab.c"
    break;

  case 88: /* stmt: expr KW_GIVEN expr ';'  */
#line 988 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4902 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 990 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4909 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 993 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4916 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 996 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4924 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 1000 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4931 "raku.tab.c"
    break;

  case 93: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 1003 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4937 "raku.tab.c"
    break;

  case 94: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 1005 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4943 "raku.tab.c"
    break;

  case 95: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 1007 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4949 "raku.tab.c"
    break;

  case 96: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 1009 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4956 "raku.tab.c"
    break;

  case 97: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 1012 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4963 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 1015 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4970 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 1018 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4977 "raku.tab.c"
    break;

  case 100: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 1021 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4984 "raku.tab.c"
    break;

  case 101: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 1024 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4991 "raku.tab.c"
    break;

  case 102: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1027 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4998 "raku.tab.c"
    break;

  case 103: /* stmt: expr ';'  */
#line 1029 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 5004 "raku.tab.c"
    break;

  case 104: /* stmt: ';'  */
#line 1030 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 5010 "raku.tab.c"
    break;

  case 105: /* stmt: if_stmt  */
#line 1031 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5016 "raku.tab.c"
    break;

  case 106: /* stmt: while_stmt  */
#line 1032 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5022 "raku.tab.c"
    break;

  case 107: /* stmt: for_stmt  */
#line 1033 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5028 "raku.tab.c"
    break;

  case 108: /* stmt: given_stmt  */
#line 1034 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5034 "raku.tab.c"
    break;

  case 109: /* stmt: KW_TRY block  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5040 "raku.tab.c"
    break;

  case 110: /* stmt: KW_TRY block KW_CATCH block  */
#line 1038 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5046 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1040 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 5054 "raku.tab.c"
    break;

  case 112: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1044 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 5063 "raku.tab.c"
    break;

  case 113: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1049 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 5071 "raku.tab.c"
    break;

  case 114: /* stmt: KW_CATCH block  */
#line 1053 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5077 "raku.tab.c"
    break;

  case 115: /* stmt: block  */
#line 1055 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 5083 "raku.tab.c"
    break;

  case 116: /* stmt: PHASER block  */
#line 1057 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 5089 "raku.tab.c"
    break;

  case 117: /* stmt: PHASER block ';'  */
#line 1059 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 5095 "raku.tab.c"
    break;

  case 118: /* stmt: PHASER expr ';'  */
#line 1061 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),seq1((yyvsp[-1].node))); ct_drop((yyvsp[-2].sval)); }
#line 5101 "raku.tab.c"
    break;

  case 119: /* stmt: PHASER VAR_SCALAR '=' expr ';'  */
#line 1063 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5107 "raku.tab.c"
    break;

  case 120: /* stmt: PHASER VAR_ARRAY '=' expr ';'  */
#line 1065 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_arr_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5113 "raku.tab.c"
    break;

  case 121: /* stmt: PHASER VAR_HASH '=' expr ';'  */
#line 1067 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)))); ct_drop((yyvsp[-4].sval)); }
#line 5119 "raku.tab.c"
    break;

  case 122: /* stmt: PHASER VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 1069 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node));
          (yyval.node)=rk_phaser_mark((yyvsp[-7].sval),seq1(c)); ct_drop((yyvsp[-7].sval)); }
#line 5126 "raku.tab.c"
    break;

  case 123: /* stmt: PHASER KW_SAY expr ';'  */
#line 1072 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); (yyval.node)=rk_phaser_mark((yyvsp[-3].sval),seq1(s)); ct_drop((yyvsp[-3].sval)); }
#line 5132 "raku.tab.c"
    break;

  case 124: /* stmt: unless_stmt  */
#line 1073 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5138 "raku.tab.c"
    break;

  case 125: /* stmt: until_stmt  */
#line 1074 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5144 "raku.tab.c"
    break;

  case 126: /* stmt: repeat_stmt  */
#line 1075 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5150 "raku.tab.c"
    break;

  case 127: /* stmt: loop_stmt  */
#line 1076 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5156 "raku.tab.c"
    break;

  case 128: /* stmt: KW_LAST ';'  */
#line 1077 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 5162 "raku.tab.c"
    break;

  case 129: /* stmt: KW_NEXT ';'  */
#line 1078 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 5168 "raku.tab.c"
    break;

  case 130: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1080 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5174 "raku.tab.c"
    break;

  case 131: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1082 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5180 "raku.tab.c"
    break;

  case 132: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1084 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5186 "raku.tab.c"
    break;

  case 133: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1086 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5192 "raku.tab.c"
    break;

  case 134: /* stmt: sub_decl  */
#line 1087 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5198 "raku.tab.c"
    break;

  case 135: /* stmt: class_decl  */
#line 1088 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5204 "raku.tab.c"
    break;

  case 136: /* stmt: role_decl  */
#line 1089 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5210 "raku.tab.c"
    break;

  case 137: /* stmt: grammar_decl  */
#line 1090 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5216 "raku.tab.c"
    break;

  case 138: /* stmt: module_decl  */
#line 1091 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5222 "raku.tab.c"
    break;

  case 139: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1095 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5228 "raku.tab.c"
    break;

  case 140: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1097 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5234 "raku.tab.c"
    break;

  case 141: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1099 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5240 "raku.tab.c"
    break;

  case 142: /* if_stmt: KW_IF expr block  */
#line 1101 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5246 "raku.tab.c"
    break;

  case 143: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1103 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5252 "raku.tab.c"
    break;

  case 144: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1105 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5258 "raku.tab.c"
    break;

  case 145: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1107 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5264 "raku.tab.c"
    break;

  case 146: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1109 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5270 "raku.tab.c"
    break;

  case 147: /* elsif_tail: KW_ELSIF expr block  */
#line 1113 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5276 "raku.tab.c"
    break;

  case 148: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1115 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5282 "raku.tab.c"
    break;

  case 149: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1117 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5288 "raku.tab.c"
    break;

  case 150: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1119 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5294 "raku.tab.c"
    break;

  case 151: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1121 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5300 "raku.tab.c"
    break;

  case 152: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5306 "raku.tab.c"
    break;

  case 153: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1127 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5312 "raku.tab.c"
    break;

  case 154: /* while_stmt: KW_WHILE expr block  */
#line 1129 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5318 "raku.tab.c"
    break;

  case 155: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1133 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5324 "raku.tab.c"
    break;

  case 156: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1135 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5330 "raku.tab.c"
    break;

  case 157: /* unless_stmt: KW_UNLESS expr block  */
#line 1137 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5336 "raku.tab.c"
    break;

  case 158: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1139 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5342 "raku.tab.c"
    break;

  case 159: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1143 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5348 "raku.tab.c"
    break;

  case 160: /* until_stmt: KW_UNTIL expr block  */
#line 1145 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5354 "raku.tab.c"
    break;

  case 161: /* repeat_stmt: KW_REPEAT block  */
#line 1149 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5360 "raku.tab.c"
    break;

  case 162: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1151 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5366 "raku.tab.c"
    break;

  case 163: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1153 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5372 "raku.tab.c"
    break;

  case 164: /* loop_stmt: KW_LOOP block  */
#line 1157 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5378 "raku.tab.c"
    break;

  case 165: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1159 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5384 "raku.tab.c"
    break;

  case 166: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1161 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5390 "raku.tab.c"
    break;

  case 167: /* loop_incr: expr  */
#line 1164 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 5396 "raku.tab.c"
    break;

  case 168: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1168 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5406 "raku.tab.c"
    break;

  case 169: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1174 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5416 "raku.tab.c"
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
#line 5428 "raku.tab.c"
    break;

  case 171: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1188 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5438 "raku.tab.c"
    break;

  case 172: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1194 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5447 "raku.tab.c"
    break;

  case 173: /* for_stmt: KW_FOR expr block  */
#line 1199 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5454 "raku.tab.c"
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
#line 5466 "raku.tab.c"
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
#line 5479 "raku.tab.c"
    break;

  case 176: /* catch_when_list: KW_WHEN expr block  */
#line 1223 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5485 "raku.tab.c"
    break;

  case 177: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1225 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5491 "raku.tab.c"
    break;

  case 178: /* when_list: %empty  */
#line 1228 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5497 "raku.tab.c"
    break;

  case 179: /* when_list: when_list KW_WHEN expr block  */
#line 1230 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5505 "raku.tab.c"
    break;

  case 180: /* sub_trait_list: TESTOP IDENT  */
#line 1235 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5511 "raku.tab.c"
    break;

  case 181: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1236 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5517 "raku.tab.c"
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
#line 5529 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1248 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5539 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1254 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5549 "raku.tab.c"
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
#line 5561 "raku.tab.c"
    break;

  case 186: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1268 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5571 "raku.tab.c"
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
#line 5583 "raku.tab.c"
    break;

  case 188: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1282 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5593 "raku.tab.c"
    break;

  case 189: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1288 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5603 "raku.tab.c"
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
#line 5616 "raku.tab.c"
    break;

  case 191: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1303 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5627 "raku.tab.c"
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
#line 5640 "raku.tab.c"
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
#line 5653 "raku.tab.c"
    break;

  case 194: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1328 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5664 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list '}'  */
#line 1336 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5670 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list expr '}'  */
#line 1338 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5677 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1341 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5684 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1344 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5690 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1346 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5696 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1348 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5702 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1350 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5708 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1352 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5714 "raku.tab.c"
    break;

  case 203: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1354 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5720 "raku.tab.c"
    break;

  case 204: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1356 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5726 "raku.tab.c"
    break;

  case 205: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1358 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5733 "raku.tab.c"
    break;

  case 206: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1361 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5739 "raku.tab.c"
    break;

  case 207: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1363 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5745 "raku.tab.c"
    break;

  case 208: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1365 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5751 "raku.tab.c"
    break;

  case 209: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1367 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5758 "raku.tab.c"
    break;

  case 210: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1370 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5765 "raku.tab.c"
    break;

  case 211: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1373 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5773 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list '}'  */
#line 1378 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5779 "raku.tab.c"
    break;

  case 213: /* method_body: '{' YADA '}'  */
#line 1379 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5785 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1381 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5792 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list expr '}'  */
#line 1384 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5799 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1387 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5806 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1390 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5812 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1392 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5818 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1394 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5824 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1396 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5830 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1398 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5836 "raku.tab.c"
    break;

  case 222: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1400 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5842 "raku.tab.c"
    break;

  case 223: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1402 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5848 "raku.tab.c"
    break;

  case 224: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1404 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5855 "raku.tab.c"
    break;

  case 225: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1407 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5861 "raku.tab.c"
    break;

  case 226: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1409 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5867 "raku.tab.c"
    break;

  case 227: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1411 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5873 "raku.tab.c"
    break;

  case 228: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1413 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5880 "raku.tab.c"
    break;

  case 229: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1416 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5887 "raku.tab.c"
    break;

  case 230: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1419 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5895 "raku.tab.c"
    break;

  case 231: /* pkg_name: IDENT  */
#line 1424 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5901 "raku.tab.c"
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
#line 5919 "raku.tab.c"
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
#line 5936 "raku.tab.c"
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
#line 5953 "raku.tab.c"
    break;

  case 235: /* is_clauses: %empty  */
#line 1473 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5959 "raku.tab.c"
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
#line 5975 "raku.tab.c"
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
#line 5991 "raku.tab.c"
    break;

  case 238: /* class_body_list: %empty  */
#line 1500 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5997 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list ';'  */
#line 1501 "raku.y"
                           { (yyval.list) = (yyvsp[-1].list); }
#line 6003 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1503 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 6010 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1506 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 6017 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1509 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 6024 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1512 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6031 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1515 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6038 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1518 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 6045 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1521 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6052 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1524 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6059 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1527 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6066 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1530 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6073 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1533 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6080 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1536 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6087 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1539 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6098 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1546 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6108 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1552 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6118 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1558 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6129 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1565 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6140 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1572 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6151 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1579 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6159 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1583 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6167 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1587 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6175 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1591 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6183 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1595 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6194 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1602 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6205 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1609 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6216 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1616 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6227 "raku.tab.c"
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
#line 6241 "raku.tab.c"
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
#line 6253 "raku.tab.c"
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
#line 6265 "raku.tab.c"
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
#line 6278 "raku.tab.c"
    break;

  case 270: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1658 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6289 "raku.tab.c"
    break;

  case 271: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1665 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6300 "raku.tab.c"
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
#line 6314 "raku.tab.c"
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
#line 6326 "raku.tab.c"
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
#line 6338 "raku.tab.c"
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
#line 6351 "raku.tab.c"
    break;

  case 276: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1707 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6362 "raku.tab.c"
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
#line 6379 "raku.tab.c"
    break;

  case 278: /* grammar_body_list: %empty  */
#line 1730 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 6385 "raku.tab.c"
    break;

  case 279: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1732 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6394 "raku.tab.c"
    break;

  case 280: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1737 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6403 "raku.tab.c"
    break;

  case 281: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1742 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6412 "raku.tab.c"
    break;

  case 282: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1749 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 6420 "raku.tab.c"
    break;

  case 283: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1753 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 6428 "raku.tab.c"
    break;

  case 284: /* named_arg_list: ':' IDENT  */
#line 1757 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6439 "raku.tab.c"
    break;

  case 285: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1764 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6447 "raku.tab.c"
    break;

  case 286: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1768 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6455 "raku.tab.c"
    break;

  case 287: /* pair_list: IDENT OP_FATARROW expr  */
#line 1774 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6461 "raku.tab.c"
    break;

  case 288: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1776 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6467 "raku.tab.c"
    break;

  case 289: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1778 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6473 "raku.tab.c"
    break;

  case 290: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1780 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6479 "raku.tab.c"
    break;

  case 291: /* param_list: VAR_SCALAR  */
#line 1783 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6485 "raku.tab.c"
    break;

  case 292: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1784 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6491 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1785 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6497 "raku.tab.c"
    break;

  case 294: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1786 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6503 "raku.tab.c"
    break;

  case 295: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1787 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6509 "raku.tab.c"
    break;

  case 296: /* param_list: VAR_ARRAY  */
#line 1788 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6515 "raku.tab.c"
    break;

  case 297: /* param_list: param_list ',' VAR_ARRAY  */
#line 1789 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6521 "raku.tab.c"
    break;

  case 298: /* param_list: IDENT VAR_SCALAR  */
#line 1790 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6527 "raku.tab.c"
    break;

  case 299: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1791 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6533 "raku.tab.c"
    break;

  case 300: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1792 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6539 "raku.tab.c"
    break;

  case 301: /* param_list: param_list ',' VAR_SCALAR  */
#line 1793 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6545 "raku.tab.c"
    break;

  case 302: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1794 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6551 "raku.tab.c"
    break;

  case 303: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1795 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6557 "raku.tab.c"
    break;

  case 304: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1796 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6563 "raku.tab.c"
    break;

  case 305: /* param_list: VAR_SCALAR '=' expr  */
#line 1797 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6569 "raku.tab.c"
    break;

  case 306: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1798 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6575 "raku.tab.c"
    break;

  case 307: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1799 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6581 "raku.tab.c"
    break;

  case 308: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1800 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6587 "raku.tab.c"
    break;

  case 309: /* param_list: SLURPY_POS  */
#line 1801 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6593 "raku.tab.c"
    break;

  case 310: /* param_list: param_list ',' SLURPY_POS  */
#line 1802 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6599 "raku.tab.c"
    break;

  case 311: /* param_list: SLURPY_LOL  */
#line 1803 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6605 "raku.tab.c"
    break;

  case 312: /* param_list: param_list ',' SLURPY_LOL  */
#line 1804 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6611 "raku.tab.c"
    break;

  case 313: /* param_list: SLURPY_NAMED  */
#line 1805 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6617 "raku.tab.c"
    break;

  case 314: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1806 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6623 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list '}'  */
#line 1809 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6629 "raku.tab.c"
    break;

  case 316: /* block: '{' YADA '}'  */
#line 1810 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6635 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list expr '}'  */
#line 1812 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6641 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1814 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6647 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1816 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6653 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1818 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6659 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1820 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6665 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1822 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6672 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1825 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6678 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1827 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6684 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1829 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6690 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1831 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 6696 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1833 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6703 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1836 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6710 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1839 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6718 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1843 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6725 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1846 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6732 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1849 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6739 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1852 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 6745 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1854 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6752 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1857 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6759 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1860 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,a); (yyval.node)=make_seq(l); }
#line 6766 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1863 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6773 "raku.tab.c"
    break;

  case 338: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1866 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6780 "raku.tab.c"
    break;

  case 339: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1869 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,t); (yyval.node)=make_seq(l); }
#line 6786 "raku.tab.c"
    break;

  case 340: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1871 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 6792 "raku.tab.c"
    break;

  case 341: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1873 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 6798 "raku.tab.c"
    break;

  case 342: /* block: '{' stmt_list KW_LAST '}'  */
#line 1875 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6804 "raku.tab.c"
    break;

  case 343: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1877 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6810 "raku.tab.c"
    break;

  case 344: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1879 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6816 "raku.tab.c"
    break;

  case 345: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1881 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6822 "raku.tab.c"
    break;

  case 346: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1883 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6828 "raku.tab.c"
    break;

  case 347: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1885 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6834 "raku.tab.c"
    break;

  case 348: /* closure: '{' expr '}'  */
#line 1888 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6840 "raku.tab.c"
    break;

  case 349: /* expr: VAR_SCALAR '=' expr  */
#line 1891 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6846 "raku.tab.c"
    break;

  case 350: /* expr: VAR_ARRAY '=' expr  */
#line 1892 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6852 "raku.tab.c"
    break;

  case 351: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1893 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6858 "raku.tab.c"
    break;

  case 352: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1894 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6864 "raku.tab.c"
    break;

  case 353: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1895 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6870 "raku.tab.c"
    break;

  case 354: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1896 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6876 "raku.tab.c"
    break;

  case 355: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1897 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6882 "raku.tab.c"
    break;

  case 356: /* expr: KW_GATHER block  */
#line 1898 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6892 "raku.tab.c"
    break;

  case 357: /* expr: KW_GATHER for_stmt  */
#line 1903 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6902 "raku.tab.c"
    break;

  case 358: /* expr: tern_expr OP_FATARROW expr  */
#line 1909 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6908 "raku.tab.c"
    break;

  case 359: /* expr: tern_expr  */
#line 1910 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6914 "raku.tab.c"
    break;

  case 360: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1914 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6920 "raku.tab.c"
    break;

  case 361: /* tern_expr: or_expr  */
#line 1915 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6926 "raku.tab.c"
    break;

  case 362: /* or_expr: or_expr OP_OR and_expr  */
#line 1918 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6932 "raku.tab.c"
    break;

  case 363: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1919 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6938 "raku.tab.c"
    break;

  case 364: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1920 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6944 "raku.tab.c"
    break;

  case 365: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1921 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6950 "raku.tab.c"
    break;

  case 366: /* or_expr: and_expr  */
#line 1922 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6956 "raku.tab.c"
    break;

  case 367: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1925 "raku.y"
                               { (yyval.node)=rk_logical_and((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6962 "raku.tab.c"
    break;

  case 368: /* and_expr: cmp_expr  */
#line 1926 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6968 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1929 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_EQ,rk_numeric_ctx((yyvsp[0].node))); }
#line 6974 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1930 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6980 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1931 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6986 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1932 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6992 "raku.tab.c"
    break;

  case 373: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1933 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6998 "raku.tab.c"
    break;

  case 374: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1934 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7004 "raku.tab.c"
    break;

  case 375: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1935 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7010 "raku.tab.c"
    break;

  case 376: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1936 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7016 "raku.tab.c"
    break;

  case 377: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1937 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7022 "raku.tab.c"
    break;

  case 378: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1938 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_NE,rk_numeric_ctx((yyvsp[0].node))); }
#line 7028 "raku.tab.c"
    break;

  case 379: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1939 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LT,rk_numeric_ctx((yyvsp[0].node))); }
#line 7034 "raku.tab.c"
    break;

  case 380: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1940 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GT,rk_numeric_ctx((yyvsp[0].node))); }
#line 7040 "raku.tab.c"
    break;

  case 381: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1941 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LE,rk_numeric_ctx((yyvsp[0].node))); }
#line 7046 "raku.tab.c"
    break;

  case 382: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1942 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GE,rk_numeric_ctx((yyvsp[0].node))); }
#line 7052 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1943 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7058 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1944 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7064 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1945 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7070 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1946 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7076 "raku.tab.c"
    break;

  case 387: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1947 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7082 "raku.tab.c"
    break;

  case 388: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1948 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7088 "raku.tab.c"
    break;

  case 389: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1949 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7094 "raku.tab.c"
    break;

  case 390: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1950 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7100 "raku.tab.c"
    break;

  case 391: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1951 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7106 "raku.tab.c"
    break;

  case 392: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1953 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 7116 "raku.tab.c"
    break;

  case 393: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1959 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 7126 "raku.tab.c"
    break;

  case 394: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1965 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 7136 "raku.tab.c"
    break;

  case 395: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1971 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 7146 "raku.tab.c"
    break;

  case 396: /* cmp_expr: divis_expr  */
#line 1976 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 7152 "raku.tab.c"
    break;

  case 397: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1979 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7158 "raku.tab.c"
    break;

  case 398: /* divis_expr: jct_expr  */
#line 1980 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7164 "raku.tab.c"
    break;

  case 399: /* jct_expr: jct_expr '|' range_expr  */
#line 1983 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7170 "raku.tab.c"
    break;

  case 400: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1984 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7176 "raku.tab.c"
    break;

  case 401: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1985 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7182 "raku.tab.c"
    break;

  case 402: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1986 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7188 "raku.tab.c"
    break;

  case 403: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1987 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7194 "raku.tab.c"
    break;

  case 404: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1988 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7200 "raku.tab.c"
    break;

  case 405: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1989 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7206 "raku.tab.c"
    break;

  case 406: /* jct_expr: jct_expr '&' range_expr  */
#line 1990 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7212 "raku.tab.c"
    break;

  case 407: /* jct_expr: dor_expr  */
#line 1991 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7218 "raku.tab.c"
    break;

  case 408: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1995 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7224 "raku.tab.c"
    break;

  case 409: /* dor_expr: range_expr  */
#line 1996 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7230 "raku.tab.c"
    break;

  case 410: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1999 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7236 "raku.tab.c"
    break;

  case 411: /* range_expr: add_expr YADA add_expr  */
#line 2000 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7242 "raku.tab.c"
    break;

  case 412: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 2001 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7248 "raku.tab.c"
    break;

  case 413: /* range_expr: range_expr OP_COLL add_expr  */
#line 2002 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7254 "raku.tab.c"
    break;

  case 414: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 2003 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7260 "raku.tab.c"
    break;

  case 415: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 2004 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7266 "raku.tab.c"
    break;

  case 416: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 2005 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 7272 "raku.tab.c"
    break;

  case 417: /* range_expr: add_expr  */
#line 2006 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 7278 "raku.tab.c"
    break;

  case 418: /* add_expr: add_expr '~' repl_expr  */
#line 2009 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7284 "raku.tab.c"
    break;

  case 419: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 2010 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7290 "raku.tab.c"
    break;

  case 420: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 2011 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7296 "raku.tab.c"
    break;

  case 421: /* add_expr: repl_expr  */
#line 2012 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 7302 "raku.tab.c"
    break;

  case 422: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 2015 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7308 "raku.tab.c"
    break;

  case 423: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 2016 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 7314 "raku.tab.c"
    break;

  case 424: /* repl_expr: addsub_expr  */
#line 2017 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7320 "raku.tab.c"
    break;

  case 425: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 2020 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7326 "raku.tab.c"
    break;

  case 426: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 2021 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7332 "raku.tab.c"
    break;

  case 427: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 2022 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7338 "raku.tab.c"
    break;

  case 428: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 2023 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7344 "raku.tab.c"
    break;

  case 429: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 2024 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7350 "raku.tab.c"
    break;

  case 430: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 2025 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7356 "raku.tab.c"
    break;

  case 431: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 2026 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7362 "raku.tab.c"
    break;

  case 432: /* addsub_expr: mul_expr  */
#line 2027 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 7368 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr '*' unary_expr  */
#line 2030 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7374 "raku.tab.c"
    break;

  case 434: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 2031 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7380 "raku.tab.c"
    break;

  case 435: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 2032 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7386 "raku.tab.c"
    break;

  case 436: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 2033 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7392 "raku.tab.c"
    break;

  case 437: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 2034 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7398 "raku.tab.c"
    break;

  case 438: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 2035 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7404 "raku.tab.c"
    break;

  case 439: /* mul_expr: mul_expr '/' unary_expr  */
#line 2036 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7410 "raku.tab.c"
    break;

  case 440: /* mul_expr: mul_expr '%' unary_expr  */
#line 2037 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7416 "raku.tab.c"
    break;

  case 441: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 2039 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,rk_numeric_ctx((yyvsp[-2].node))); expr_add_child(c,rk_numeric_ctx((yyvsp[0].node))); (yyval.node)=c; }
#line 7422 "raku.tab.c"
    break;

  case 442: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2041 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7428 "raku.tab.c"
    break;

  case 443: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2043 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7434 "raku.tab.c"
    break;

  case 444: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2045 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7440 "raku.tab.c"
    break;

  case 445: /* mul_expr: unary_expr  */
#line 2046 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7446 "raku.tab.c"
    break;

  case 446: /* unary_expr: '-' unary_expr  */
#line 2049 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,rk_numeric_ctx((yyvsp[0].node))); }
#line 7452 "raku.tab.c"
    break;

  case 447: /* unary_expr: '+' unary_expr  */
#line 2051 "raku.y"
        { tree_t *n=rk_numeric_ctx((yyvsp[0].node));
          if (n==(yyvsp[0].node) && (yyvsp[0].node)->t!=TT_ILIT && (yyvsp[0].node)->t!=TT_FLIT) { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; n=expr_binary(TT_ADD,(yyvsp[0].node),z); }
          (yyval.node)=n; }
#line 7460 "raku.tab.c"
    break;

  case 448: /* unary_expr: '?' unary_expr  */
#line 2054 "raku.y"
                                   { tree_t *c=make_call("__rk_mkbool"); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7466 "raku.tab.c"
    break;

  case 449: /* unary_expr: '!' unary_expr  */
#line 2055 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7472 "raku.tab.c"
    break;

  case 450: /* unary_expr: CARET unary_expr  */
#line 2056 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7478 "raku.tab.c"
    break;

  case 451: /* unary_expr: OP_REDUCE unary_expr  */
#line 2058 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7487 "raku.tab.c"
    break;

  case 452: /* unary_expr: pow_expr  */
#line 2062 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7493 "raku.tab.c"
    break;

  case 453: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2065 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7499 "raku.tab.c"
    break;

  case 454: /* pow_expr: postfix_expr  */
#line 2066 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7505 "raku.tab.c"
    break;

  case 455: /* scalar_list: VAR_SCALAR  */
#line 2069 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7511 "raku.tab.c"
    break;

  case 456: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2070 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7517 "raku.tab.c"
    break;

  case 457: /* meth_name: IDENT  */
#line 2073 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7523 "raku.tab.c"
    break;

  case 458: /* meth_name: KW_SORT  */
#line 2074 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7529 "raku.tab.c"
    break;

  case 459: /* meth_name: KW_REVERSE  */
#line 2075 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7535 "raku.tab.c"
    break;

  case 460: /* meth_name: KW_MAP  */
#line 2076 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7541 "raku.tab.c"
    break;

  case 461: /* meth_name: KW_GREP  */
#line 2077 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7547 "raku.tab.c"
    break;

  case 462: /* meth_name: KW_SAY  */
#line 2078 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7553 "raku.tab.c"
    break;

  case 463: /* meth_name: KW_PRINT  */
#line 2079 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7559 "raku.tab.c"
    break;

  case 464: /* meth_name: KW_TAKE  */
#line 2080 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7565 "raku.tab.c"
    break;

  case 465: /* meth_name: KW_RETURN  */
#line 2081 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7571 "raku.tab.c"
    break;

  case 466: /* meth_name: KW_EXISTS  */
#line 2082 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7577 "raku.tab.c"
    break;

  case 467: /* meth_name: KW_DELETE  */
#line 2083 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7583 "raku.tab.c"
    break;

  case 468: /* meth_name: KW_JOIN  */
#line 2084 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7589 "raku.tab.c"
    break;

  case 469: /* meth_name: TESTOP  */
#line 2085 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7595 "raku.tab.c"
    break;

  case 470: /* postfix_expr: call_expr  */
#line 2087 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7601 "raku.tab.c"
    break;

  case 471: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2090 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7611 "raku.tab.c"
    break;

  case 472: /* call_expr: IDENT '(' arg_list ')'  */
#line 2096 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7620 "raku.tab.c"
    break;

  case 473: /* call_expr: IDENT '(' ')'  */
#line 2100 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7626 "raku.tab.c"
    break;

  case 474: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2102 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7632 "raku.tab.c"
    break;

  case 475: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2104 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7638 "raku.tab.c"
    break;

  case 476: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2106 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7647 "raku.tab.c"
    break;

  case 477: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2111 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7653 "raku.tab.c"
    break;

  case 478: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2113 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7666 "raku.tab.c"
    break;

  case 479: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2122 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7679 "raku.tab.c"
    break;

  case 480: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2131 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7690 "raku.tab.c"
    break;

  case 481: /* call_expr: IDENT '.' KW_NEW  */
#line 2138 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7698 "raku.tab.c"
    break;

  case 482: /* call_expr: IDENT '.' IDENT  */
#line 2142 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7707 "raku.tab.c"
    break;

  case 483: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2147 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7718 "raku.tab.c"
    break;

  case 484: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2154 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7727 "raku.tab.c"
    break;

  case 485: /* call_expr: IDENT '.' CARET IDENT  */
#line 2159 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7737 "raku.tab.c"
    break;

  case 486: /* call_expr: atom '.' CARET IDENT  */
#line 2165 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7747 "raku.tab.c"
    break;

  case 487: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2171 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7758 "raku.tab.c"
    break;

  case 488: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2178 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7769 "raku.tab.c"
    break;

  case 489: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2185 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7778 "raku.tab.c"
    break;

  case 490: /* call_expr: atom '.' meth_name  */
#line 2190 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7787 "raku.tab.c"
    break;

  case 491: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2195 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7798 "raku.tab.c"
    break;

  case 492: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2202 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7807 "raku.tab.c"
    break;

  case 493: /* call_expr: call_expr '.' meth_name  */
#line 2207 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7816 "raku.tab.c"
    break;

  case 494: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2212 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7827 "raku.tab.c"
    break;

  case 495: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2219 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7838 "raku.tab.c"
    break;

  case 496: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2226 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7849 "raku.tab.c"
    break;

  case 497: /* call_expr: '.' meth_name '(' ')'  */
#line 2233 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7858 "raku.tab.c"
    break;

  case 498: /* call_expr: '.' meth_name  */
#line 2238 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7867 "raku.tab.c"
    break;

  case 499: /* call_expr: KW_DIE expr  */
#line 2243 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7873 "raku.tab.c"
    break;

  case 500: /* call_expr: KW_MAP closure expr  */
#line 2245 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7879 "raku.tab.c"
    break;

  case 501: /* call_expr: KW_MAP closure ',' expr  */
#line 2247 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7885 "raku.tab.c"
    break;

  case 502: /* call_expr: KW_GREP closure expr  */
#line 2249 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7891 "raku.tab.c"
    break;

  case 503: /* call_expr: KW_GREP closure ',' expr  */
#line 2251 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7897 "raku.tab.c"
    break;

  case 504: /* call_expr: KW_SORT expr  */
#line 2253 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7903 "raku.tab.c"
    break;

  case 505: /* call_expr: KW_SORT closure expr  */
#line 2255 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7909 "raku.tab.c"
    break;

  case 506: /* call_expr: KW_SORT closure ',' expr  */
#line 2257 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7915 "raku.tab.c"
    break;

  case 507: /* call_expr: KW_REVERSE expr  */
#line 2259 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7921 "raku.tab.c"
    break;

  case 508: /* call_expr: atom  */
#line 2260 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7927 "raku.tab.c"
    break;

  case 509: /* arg_list: expr  */
#line 2263 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7933 "raku.tab.c"
    break;

  case 510: /* arg_list: arg_list ',' expr  */
#line 2264 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7939 "raku.tab.c"
    break;

  case 511: /* arg_list: arg_list ','  */
#line 2265 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7945 "raku.tab.c"
    break;

  case 512: /* arg_list: ':' IDENT  */
#line 2266 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7951 "raku.tab.c"
    break;

  case 513: /* arg_list: ':' '!' IDENT  */
#line 2267 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7957 "raku.tab.c"
    break;

  case 514: /* arg_list: ':' IDENT '(' expr ')'  */
#line 2268 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7963 "raku.tab.c"
    break;

  case 515: /* arg_list: ':' IDENT '(' ')'  */
#line 2269 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7969 "raku.tab.c"
    break;

  case 516: /* arg_list: ':' IDENT '<' IDENT '>'  */
#line 2270 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7975 "raku.tab.c"
    break;

  case 517: /* arg_list: arg_list ',' ':' IDENT  */
#line 2271 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7981 "raku.tab.c"
    break;

  case 518: /* arg_list: arg_list ',' ':' '!' IDENT  */
#line 2272 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7987 "raku.tab.c"
    break;

  case 519: /* arg_list: arg_list ',' ':' IDENT '(' expr ')'  */
#line 2273 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-6].list),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7993 "raku.tab.c"
    break;

  case 520: /* arg_list: arg_list ',' ':' IDENT '(' ')'  */
#line 2274 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-5].list),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7999 "raku.tab.c"
    break;

  case 521: /* arg_list: arg_list ',' ':' IDENT '<' IDENT '>'  */
#line 2275 "raku.y"
                                           { (yyval.list)=exprlist_append((yyvsp[-6].list),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 8005 "raku.tab.c"
    break;

  case 522: /* paren_group: '(' ')'  */
#line 2278 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 8011 "raku.tab.c"
    break;

  case 523: /* paren_group: '(' expr ')'  */
#line 2279 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 8017 "raku.tab.c"
    break;

  case 524: /* paren_group: '(' expr ',' ')'  */
#line 2281 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8023 "raku.tab.c"
    break;

  case 525: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2283 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8030 "raku.tab.c"
    break;

  case 526: /* atom: LIT_INT  */
#line 2287 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 8036 "raku.tab.c"
    break;

  case 527: /* atom: LIT_BOOL  */
#line 2289 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 8044 "raku.tab.c"
    break;

  case 528: /* atom: LIT_FLOAT  */
#line 2292 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 8050 "raku.tab.c"
    break;

  case 529: /* atom: LIT_STR  */
#line 2293 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 8056 "raku.tab.c"
    break;

  case 530: /* atom: WORDLIST  */
#line 2295 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 8069 "raku.tab.c"
    break;

  case 531: /* atom: LIT_INTERP_STR  */
#line 2303 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 8075 "raku.tab.c"
    break;

  case 532: /* atom: VAR_SCALAR  */
#line 2304 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8081 "raku.tab.c"
    break;

  case 533: /* atom: OP_INC VAR_SCALAR  */
#line 2305 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 8087 "raku.tab.c"
    break;

  case 534: /* atom: OP_DEC VAR_SCALAR  */
#line 2306 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 8093 "raku.tab.c"
    break;

  case 535: /* atom: VAR_SCALAR OP_INC  */
#line 2307 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 8099 "raku.tab.c"
    break;

  case 536: /* atom: VAR_SCALAR OP_DEC  */
#line 2308 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 8105 "raku.tab.c"
    break;

  case 537: /* atom: VAR_TWIGIL OP_INC  */
#line 2309 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 8111 "raku.tab.c"
    break;

  case 538: /* atom: VAR_TWIGIL OP_DEC  */
#line 2310 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 8117 "raku.tab.c"
    break;

  case 539: /* atom: VAR_ARRAY  */
#line 2311 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8123 "raku.tab.c"
    break;

  case 540: /* atom: VAR_HASH  */
#line 2312 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8129 "raku.tab.c"
    break;

  case 541: /* atom: VAR_CAPTURE  */
#line 2314 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8137 "raku.tab.c"
    break;

  case 542: /* atom: VAR_FH  */
#line 2318 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8145 "raku.tab.c"
    break;

  case 543: /* atom: VAR_NAMED_CAPTURE  */
#line 2322 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 8152 "raku.tab.c"
    break;

  case 544: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2325 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8158 "raku.tab.c"
    break;

  case 545: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2327 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 8164 "raku.tab.c"
    break;

  case 546: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2329 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 8170 "raku.tab.c"
    break;

  case 547: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2331 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 8176 "raku.tab.c"
    break;

  case 548: /* atom: ARR_ALL_SLICE  */
#line 2333 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 8182 "raku.tab.c"
    break;

  case 549: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2335 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8188 "raku.tab.c"
    break;

  case 550: /* atom: VAR_SCALAR_IDX '[' expr ']'  */
#line 2337 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8194 "raku.tab.c"
    break;

  case 551: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>'  */
#line 2339 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8200 "raku.tab.c"
    break;

  case 552: /* atom: VAR_SCALAR_HASH '{' expr '}'  */
#line 2341 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8206 "raku.tab.c"
    break;

  case 553: /* atom: VAR_SCALAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2343 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8212 "raku.tab.c"
    break;

  case 554: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>' ADV_EXISTS  */
#line 2345 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8218 "raku.tab.c"
    break;

  case 555: /* atom: VAR_SCALAR_HASH '{' expr '}' ADV_DELETE  */
#line 2347 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8224 "raku.tab.c"
    break;

  case 556: /* atom: VAR_SCALAR_ANGLE '<' IDENT '>' ADV_DELETE  */
#line 2349 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8230 "raku.tab.c"
    break;

  case 557: /* atom: VAR_HASH '{' expr '}'  */
#line 2351 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8236 "raku.tab.c"
    break;

  case 558: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2353 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8242 "raku.tab.c"
    break;

  case 559: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2355 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8248 "raku.tab.c"
    break;

  case 560: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2357 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8254 "raku.tab.c"
    break;

  case 561: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2359 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8260 "raku.tab.c"
    break;

  case 562: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2361 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8266 "raku.tab.c"
    break;

  case 563: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2363 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8272 "raku.tab.c"
    break;

  case 564: /* atom: IDENT  */
#line 2364 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8278 "raku.tab.c"
    break;

  case 565: /* atom: VAR_TWIGIL  */
#line 2366 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8286 "raku.tab.c"
    break;

  case 566: /* atom: VAR_ARRAY_TWIGIL  */
#line 2370 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8294 "raku.tab.c"
    break;

  case 567: /* atom: VAR_HASH_TWIGIL  */
#line 2374 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8302 "raku.tab.c"
    break;

  case 568: /* atom: '[' ']'  */
#line 2377 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 8308 "raku.tab.c"
    break;

  case 569: /* atom: '[' expr ']'  */
#line 2379 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8314 "raku.tab.c"
    break;

  case 570: /* atom: '[' expr ',' ']'  */
#line 2381 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8320 "raku.tab.c"
    break;

  case 571: /* atom: '[' expr ',' arg_list ']'  */
#line 2383 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8327 "raku.tab.c"
    break;

  case 572: /* atom: DOLLAR_LBRACKET ']'  */
#line 2385 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 8333 "raku.tab.c"
    break;

  case 573: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2387 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8339 "raku.tab.c"
    break;

  case 574: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2389 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8345 "raku.tab.c"
    break;

  case 575: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2391 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8352 "raku.tab.c"
    break;

  case 576: /* atom: paren_group  */
#line 2393 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 8358 "raku.tab.c"
    break;

  case 577: /* atom: block  */
#line 2394 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8364 "raku.tab.c"
    break;

  case 578: /* atom: KW_SUB block  */
#line 2395 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8370 "raku.tab.c"
    break;

  case 579: /* atom: KW_SUB '(' param_list ')' block  */
#line 2397 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 8377 "raku.tab.c"
    break;

  case 580: /* atom: OP_ARROW scalar_list block  */
#line 2400 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 8384 "raku.tab.c"
    break;


#line 8388 "raku.tab.c"

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

#line 2403 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    size_t n = strlen(src); char *text = (char *) ct_alloc(n + 3);
    if (!text) return NULL;
    memcpy(text, src, n); memcpy(text + n, "\n;", 3);
    void *buf = raku_yy_scan_string(text);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    ct_drop(text);
    return raku_prog_result;
}
