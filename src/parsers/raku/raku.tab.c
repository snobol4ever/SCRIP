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
#define YYLAST   7750

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  567
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1399

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
       0,   753,   753,   764,   765,   768,   770,   772,   774,   776,
     778,   781,   783,   785,   787,   789,   793,   795,   797,   799,
     801,   805,   809,   811,   815,   819,   821,   823,   825,   827,
     829,   833,   837,   839,   841,   843,   845,   847,   849,   851,
     853,   855,   857,   865,   867,   869,   871,   873,   875,   877,
     880,   883,   885,   888,   891,   893,   897,   899,   901,   903,
     905,   908,   911,   913,   916,   919,   922,   924,   929,   933,
     937,   940,   943,   947,   950,   953,   956,   959,   962,   964,
     966,   968,   970,   973,   975,   977,   979,   982,   985,   989,
     992,   994,   996,   998,  1001,  1004,  1007,  1010,  1013,  1016,
    1019,  1020,  1021,  1022,  1023,  1024,  1025,  1027,  1029,  1033,
    1038,  1042,  1044,  1046,  1048,  1050,  1052,  1054,  1056,  1058,
    1061,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1071,  1073,
    1075,  1077,  1078,  1079,  1080,  1081,  1084,  1086,  1088,  1090,
    1092,  1094,  1096,  1098,  1102,  1104,  1106,  1108,  1110,  1112,
    1116,  1118,  1122,  1124,  1126,  1128,  1132,  1134,  1138,  1140,
    1142,  1146,  1148,  1150,  1154,  1157,  1163,  1169,  1177,  1183,
    1188,  1193,  1201,  1212,  1214,  1218,  1219,  1225,  1226,  1229,
    1237,  1243,  1249,  1257,  1263,  1271,  1277,  1283,  1292,  1299,
    1308,  1317,  1326,  1327,  1330,  1333,  1335,  1337,  1339,  1341,
    1343,  1345,  1347,  1350,  1352,  1354,  1356,  1359,  1362,  1368,
    1369,  1370,  1373,  1376,  1379,  1381,  1383,  1385,  1387,  1389,
    1391,  1393,  1396,  1398,  1400,  1402,  1405,  1408,  1414,  1417,
    1433,  1448,  1463,  1464,  1476,  1490,  1491,  1492,  1495,  1498,
    1501,  1504,  1507,  1510,  1513,  1516,  1519,  1522,  1525,  1528,
    1535,  1541,  1547,  1554,  1561,  1568,  1572,  1576,  1580,  1584,
    1591,  1598,  1605,  1612,  1622,  1630,  1638,  1647,  1654,  1661,
    1671,  1679,  1687,  1696,  1705,  1720,  1721,  1726,  1731,  1738,
    1742,  1746,  1753,  1757,  1763,  1765,  1767,  1769,  1773,  1774,
    1775,  1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1784,
    1785,  1786,  1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,
    1795,  1796,  1799,  1800,  1801,  1803,  1805,  1807,  1809,  1811,
    1814,  1816,  1818,  1820,  1822,  1825,  1828,  1832,  1835,  1838,
    1841,  1843,  1846,  1849,  1852,  1855,  1858,  1860,  1862,  1864,
    1866,  1868,  1870,  1872,  1874,  1878,  1881,  1882,  1883,  1884,
    1885,  1886,  1887,  1888,  1893,  1898,  1900,  1903,  1905,  1908,
    1909,  1910,  1911,  1912,  1915,  1916,  1919,  1920,  1921,  1922,
    1923,  1924,  1925,  1926,  1927,  1928,  1929,  1930,  1931,  1932,
    1933,  1934,  1935,  1936,  1937,  1938,  1939,  1940,  1941,  1942,
    1948,  1954,  1960,  1966,  1969,  1970,  1973,  1974,  1975,  1976,
    1977,  1978,  1979,  1980,  1981,  1984,  1986,  1989,  1990,  1991,
    1992,  1993,  1994,  1995,  1996,  1999,  2000,  2001,  2002,  2005,
    2006,  2007,  2010,  2011,  2012,  2013,  2014,  2015,  2016,  2017,
    2020,  2021,  2022,  2023,  2024,  2025,  2026,  2027,  2028,  2030,
    2032,  2034,  2036,  2039,  2040,  2041,  2042,  2043,  2048,  2051,
    2052,  2055,  2056,  2059,  2060,  2061,  2062,  2063,  2064,  2065,
    2066,  2067,  2068,  2069,  2070,  2071,  2073,  2075,  2081,  2086,
    2087,  2089,  2091,  2096,  2098,  2107,  2116,  2123,  2127,  2132,
    2139,  2144,  2150,  2156,  2163,  2170,  2175,  2180,  2187,  2192,
    2197,  2204,  2211,  2218,  2223,  2228,  2230,  2232,  2234,  2236,
    2238,  2240,  2242,  2245,  2246,  2247,  2248,  2249,  2250,  2251,
    2252,  2253,  2254,  2255,  2256,  2257,  2260,  2261,  2262,  2264,
    2269,  2270,  2274,  2275,  2276,  2285,  2286,  2287,  2288,  2289,
    2290,  2291,  2292,  2293,  2294,  2295,  2299,  2303,  2306,  2308,
    2310,  2312,  2314,  2316,  2318,  2320,  2322,  2324,  2326,  2328,
    2330,  2332,  2333,  2337,  2341,  2345,  2346,  2348,  2350,  2353,
    2354,  2356,  2358,  2361,  2362,  2363,  2364,  2367
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

#define YYPACT_NINF (-870)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-565)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -870,    33,  4386,  -870,  -870,  -870,  -870,  -870,  -870,   715,
     349,   -58,   267,    36,  -870,  -870,  7471,  4473,  -870,  -870,
    -870,    12,  1582,   137,  6108,  6187,  6322,  6401,  6536,     2,
      -4,  6536,  4608,  4687,   132,    69,  6536,  6536,    79,   113,
    6615,  6750,  -132,    42,    59,   125,   -48,   -48,  6829,  6536,
    -132,     0,  6536,  4822,   317,   317,    67,  7471,  -870,  -870,
     317,   317,  4901,   299,   422,   430,  7471,  7471,  7471,  1234,
    -870,  5036,  5115,   149,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  1447,   551,
     263,   565,   256,   717,   811,   719,   351,   944,   452,   238,
     622,   807,  -870,  -870,   355,   352,   362,   423,  6536,   625,
    -870,  -870,  6536,  6536,  6536,  6536,  6536,  6536,  1661,  6536,
    6964,  5250,  6536,   676,  6536,  -870,  -870,  6536,   536,   106,
    1797,   -22,   550,   306,   301,  -121,   163,  -870,  -870,   589,
    -870,   597,   564,   297,  -870,   -20,  5329,  -870,  1876,    30,
    -870,   210,   350,   437,   477,   851,   727,   506,   299,  5036,
     268,  5036,    88,  5036,  -132,  5036,  -132,   123,   454,   461,
     615,  -870,  -870,  -870,   270,  -870,   218,  -870,   249,   639,
     650,   711,   691,   692,   480,   482,  5036,  -132,  5036,  -132,
     655,  5464,  -870,  6536,  6536,  -870,  6536,  6536,  -870,  6536,
    5543,  6536,  5678,  6536,  -870,  -870,   759,   566,  -870,  -870,
    -870,   720,  -870,  -870,   709,   723,    27,  -870,   729,   733,
     722,   537,   170,  6536,  7571,   747,  -870,    -6,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,   779,  -870,   469,  -870,
      23,   800,  3349,  6536,  6536,  6536,  6536,  6536,  6536,  6536,
    6536,  -870,  6536,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,   730,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,  7471,
    7471,  7471,  7471,  7471,  7471,  1234,  6536,  1109,   814,   795,
     818,   824,   833,   870,   904,   922,  -870,   908,   931,  5036,
     377,   970,   316,   941,   962,   971,   969,  -870,  1004,  1202,
    1043,   185,  -870,    40,   965,   972,  5250,  1213,  6536,  1234,
    1109,  6536,  6536,  6536,  6536,  6536,  6536,  6536,  2012,  -870,
    -870,  1052,  1078,   469,   980,   356,  1219,  -870,  2950,  6536,
    6536,  -870,  6536,  7043,  -870,  6536,  7178,  -870,   478,   474,
     527,   534,  1097,  1099,   996,   998,  6536,  6536,  6536,  6536,
    6536,  6536,  6536,  -870,  3085,  1001,  6536,  6536,  -870,  3085,
    1006,  1165,  1017,  -870,   299,  3085,  -870,  7471,  7471,    29,
    -870,  -870,    50,  -870,   303,  -870,  -870,  -870,  1019,  -870,
    3085,  6536,  6536,  -870,  6536,  6536,  -870,  6536,  6536,  1080,
    3085,  -870,  1252,  6536,  1264,  6536,  1022,  1243,  1024,  -870,
    6536,  6536,  1289,  1127,  1153,  1154,  1156,  1159,  1108,  6536,
    -870,  -870,  -870,  -132,  6536,  -132,    37,  -870,    28,  -870,
      56,  1132,  1167,  -870,  -870,  6536,  6536,  6536,  1333,  1170,
    -870,  -870,  1339,  -870,  2091,  -870,  2227,  2306,  -870,  -870,
     387,   341,   455,  6108,  6187,  6536,  3988,    47,   237,  -870,
     313,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,  1181,
    1182,  -870,   256,  1247,   256,   256,   256,   717,  1201,  1201,
    1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,
    1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,  1201,
    1201,  -870,  -870,  -870,  -870,   719,   944,   944,   944,   944,
     944,   944,   944,   944,   944,   500,   500,   500,   500,   500,
     500,   500,   238,   238,   238,   622,   622,   807,   807,   807,
     807,   807,   807,   807,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,   153,  1187,  1352,
     348,  -870,  -870,  2441,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  1026,  -870,  3085,  6536,  6536,  3085,  1203,  -870,
      99,   129,  -870,  2520,  -870,  2656,  6536,   546,  -870,     3,
    -870,  3164,   499,  1204,  1183,   173,   309,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,   618,  -870,  -870,  1191,  1356,
    5757,  -870,    43,  -870,  1194,  1195,  1196,  5036,   587,  1198,
    1292,   338,  5893,   606,  1200,  6536,  6536,  -870,  7257,  -870,
    6536,  -870,    61,  -870,  6536,  6536,  1215,  2227,  1205,  1206,
    1207,  1208,  1209,  1210,  1211,   661,  2227,  1212,  1214,   898,
    -132,    -7,  7392,  -870,  -132,    -6,   213,   295,   336,    -8,
    1028,  3533,  1366,  6536,    74,  1371,  1372,  -132,   621,   899,
    1216,  1217,  1218,  1220,  1221,  1222,  -870,  1223,    48,  1224,
    1225,  1226,  1228,  -132,  -132,  -132,  1227,  1229,  1233,  6536,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -132,  1230,  6536,
    -132,  -870,  1387,  1388,  -870,   412,  1232,  1030,    66,   615,
     429,  3717,  1231,  1236,  1239,  1241,  -870,  -870,  -870,  1034,
    -870,  1040,  -870,   669,  5250,  6536,  6536,   390,    38,   586,
    -870,   240,  6536,  6536,  -870,  6536,  6536,  -870,  6536,  6536,
    6536,  6536,  6536,  6536,  6536,  6536,  -870,  1234,  1109,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  7471,  6536,  2735,
    3085,  -870,  -870,  6536,  2871,  3085,  1240,  1045,  2227,   917,
    1237,  1238,   859,  6536,  -870,  -870,  6536,  -870,  -870,  6536,
    -870,  1055,  -870,  1057,  1059,  -870,  5757,  1318,  1392,    70,
    1061,  -870,  1125,  1128,  -870,  -870,  1244,  -870,  1242,   554,
    1405,  -870,  -870,  -870,  1069,  -870,  3085,  -870,  6536,  6536,
    1071,  -870,   292,  -870,  1245,  1246,  5036,   918,  1248,  1232,
    1074,  1250,  1251,  6536,  1094,  -870,  -870,  -870,  -870,  -870,
    -870,  -870,  -870,  1096,  -870,  -870,  -870,  1253,  -870,  -870,
    5036,  -132,  -870,  -870,  1415,  -870,  1420,  1421,  1418,    -5,
    -870,    -8,  6108,  6187,  4123,  -870,   433,  -870,  -870,  1419,
    6536,  -870,  -870,  -870,    75,  -870,   476,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  6536,  -132,  -870,
    -870,  -870,  1256,  1258,  1407,  -870,  -870,  -870,  -870,  6536,
    1259,  -870,  -870,  -132,  1254,  -870,  -870,   442,   964,  1092,
    1376,  -870,  -870,  -870,  1232,  1232,  1103,  1111,  1426,  1427,
    1428,  -870,  -870,  -870,  -870,  -870,   171,  -870,  -870,  -870,
     992,  1260,   431,  6536,  6536,  6536,  6536,  6536,  6536,  -870,
    -870,  -870,  -870,   432,   557,   562,   571,   605,   607,   611,
     619,   620,   633,   634,   662,   457,   520,  -870,  1265,  -870,
    1115,  1223,  1269,  -870,  1117,  1123,  1223,  -870,  1270,  1126,
    -870,  -870,  -870,  -870,  1276,  1278,  1297,  -870,  -870,  -870,
    1296,  6536,  1309,   579,  -870,  -870,  -870,  1475,  5972,  -870,
    2227,   946,  -870,  -870,  1312,  1406,  1424,  -870,  -870,  1133,
    -870,  3085,  -870,  -870,  1232,  -870,  -870,   947,  1332,  1336,
      -7,  -870,  1135,  1257,  -132,  -132,  -132,  -870,  1497,  -870,
      -5,  -870,   530,   160,  -870,   241,  6536,  6536,  6536,  6536,
    6536,  6536,  6536,  6536,  -870,  -870,  -870,  1501,  6536,   107,
    1508,  1509,  -132,  1335,  -870,  -870,  -132,  1343,  6536,  -870,
    -870,  -870,   636,   696,   108,   136,  1105,  1345,  1346,  1058,
    -870,  -870,  1232,  1232,  1518,  1520,  1521,  6536,  1365,   487,
    -870,   663,   666,   700,   714,   718,   737,  -870,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  6536,
    6536,  -870,  -870,  -870,  -870,  -870,  -870,  1354,  -870,  -870,
    -870,  -870,  -870,  6536,  5972,  1369,  -870,  1353,  1137,  -870,
    -870,  6536,  6536,  2227,   955,  -870,  -870,  3085,  -870,  -870,
    -870,  -870,  -132,  -132,  -870,  -870,  -870,  -870,  -870,  -870,
    6536,  6536,  6536,  -870,  -870,  -870,   738,   781,   790,   792,
     794,   798,   806,   808,  -870,  -870,  1523,  6536,  -870,  -870,
    -870,  -870,  -870,  6536,  1361,  -870,    82,  1464,  -870,    93,
    -870,  1529,  1530,  1367,  6536,  -870,  1532,  1533,  1370,  6536,
    -870,   217,   220,  1373,  1374,  -870,  -870,  1375,   716,  -870,
    -870,  -870,  -870,  -870,  1377,  6536,  6536,  -870,  -870,  -870,
    -870,  -870,  -870,   810,   816,  -870,  1378,  1379,  -870,  -870,
    1382,  -870,  -870,  1139,  -870,   959,  1280,  -870,   819,   822,
     827,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    -870,  1383,  -132,  1380,  1141,  1364,  3901,  1380,  1143,  1384,
    1385,  -870,  1390,   538,   553,  -870,  1391,  1536,  1539,  1394,
    6536,  -870,  1551,  1559,  1396,  6536,  -870,  -870,  -870,   130,
     157,  -870,  -870,   828,   836,  -870,  -870,  -870,  -870,  -870,
    1397,  -870,  -132,  -870,  -870,  -870,  -870,  6536,  -870,  -870,
    1380,  -870,   493,  6108,  6187,  4202,  -870,   519,  -870,  1380,
    -870,  -870,  -870,  6536,  -870,  6536,  -870,  -870,  1398,  1399,
    -870,  1400,  1401,  1402,  -870,  1403,  1380,  1145,  1380,  1147,
    -870,  -870,  -870,  -870,  1409,  -870,  6536,   549,   197,  -870,
     258,  6536,  6536,  6536,  6536,  6536,  6536,  6536,  6536,  -870,
    -870,  1404,  1412,  -870,  -870,  -870,  -870,  -870,  -870,  -870,
    1380,  -870,  1380,  -132,   838,  6536,  6536,  6536,  -870,  -870,
    -870,   861,   865,   866,   867,   883,   884,   885,   886,  -870,
    -870,  -870,  -870,  -870,  -870,   888,   889,   900,  -870,  -870,
    -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870,  -870
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   520,   521,   522,   523,   525,   526,
     533,   534,   552,   551,   553,   554,     0,     0,   535,   536,
     537,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   542,   524,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     3,     4,   102,   103,   121,   122,   123,
     124,   104,   105,   131,   132,   133,   135,   134,   564,     0,
     356,   358,   363,   365,   393,   395,   404,   406,   414,   418,
     421,   429,   442,   448,   450,   466,   563,   502,     0,     0,
     529,   530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   531,   532,     0,     0,     0,
       0,   526,   533,   534,   552,   551,     0,   564,   446,   466,
     563,   502,   526,   533,   559,     0,     0,    43,     0,     0,
     503,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   414,     0,
       0,   565,   354,   353,     0,    57,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     158,     0,   161,     0,     0,   125,     0,     0,   126,     0,
       0,     0,     3,     0,   499,   501,   106,     3,   111,   495,
      59,     0,   228,   232,     0,     0,     0,   447,     0,     0,
     526,   533,   534,     0,   113,     0,   451,     0,   527,   528,
     445,   444,   443,   453,   465,   458,   459,   460,   461,   464,
     462,   463,   456,   457,   454,   455,   494,   516,     0,   555,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     348,   349,   350,   351,   352,   346,   473,     0,     0,     0,
     347,     0,     0,     0,     0,     0,     0,    47,   478,     0,
     477,   551,   469,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   560,
      36,     0,   516,     0,     0,   506,     0,    46,   505,     0,
       0,     6,     0,     0,    17,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    48,     0,     0,     0,     0,    51,     0,
       0,   139,     0,   151,     0,     0,   170,     0,     0,     0,
       3,   181,   288,   293,     0,   306,   308,   310,     0,    54,
       0,     0,     0,    56,     0,     0,    62,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,   154,     0,   157,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     496,   498,   500,     0,     0,     0,     0,    58,     0,   235,
       0,     0,     0,   275,     3,     0,     0,     0,     0,     0,
     114,   115,     0,   567,     0,   517,     0,     0,   556,   313,
     533,   534,   552,     0,     0,     0,     0,     0,     0,   312,
       0,   466,   502,     0,     0,     0,     0,     0,     0,     0,
       0,   355,   359,     0,   362,   361,   360,   364,   366,   375,
     378,   379,   374,   373,   372,   371,   370,   369,   368,   367,
     376,   377,   380,   384,   385,   386,   387,   388,   381,   382,
     383,   389,   390,   391,   392,   394,   402,   401,   400,   399,
     398,   397,   396,   403,   405,   412,   411,   410,   409,   408,
     407,   413,   417,   416,   415,   419,   420,   427,   426,   425,
     424,   423,   422,   428,   438,   440,   441,   439,   435,   434,
     433,   432,   431,   430,   436,   437,   449,   489,     0,     0,
     486,    11,    69,     0,    95,    96,    97,    98,    99,    66,
     472,    12,     0,    22,     0,     0,     0,     0,   538,    13,
     543,   544,    72,     0,   481,     0,     0,   281,   470,     0,
     468,   505,     0,     0,     0,   489,   486,   348,   349,   350,
     351,   352,   346,   347,   561,     0,    37,    45,     0,     0,
       0,   507,     0,   504,     0,     0,     0,     0,     0,     0,
     523,   551,     0,     0,     0,     0,     0,    33,     0,    34,
       0,    35,     0,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     517,     0,     0,   142,   517,     0,     0,   407,   413,     0,
       0,     0,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    42,   467,     0,     0,
       0,     0,     0,   517,     0,   517,     0,     0,     0,     0,
     127,   128,   129,   130,   345,   497,   107,     0,     0,     0,
       0,   108,     0,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   120,   452,   493,     0,
     518,     0,   557,     0,     0,     0,     0,     0,     0,     0,
     338,     0,     0,     0,   339,     0,     0,   340,     0,     0,
       0,     0,     0,     0,     0,     0,   314,     0,     0,    78,
      80,    82,    85,    83,    84,    79,    81,     0,     0,     0,
       0,    16,   482,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   548,   550,     0,   547,   549,     0,
     480,     0,   476,     0,     0,   279,     0,     0,     0,     0,
       0,   538,   543,   544,   562,    44,     0,   509,     0,   511,
       0,     7,     5,     8,     0,    19,     0,     9,     0,     0,
       0,    26,     0,    25,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    86,    89,    88,    92,    90,
      91,    87,    49,     0,    93,    94,    52,   136,   141,   140,
       0,     0,   150,   167,     0,   169,     0,     0,     0,     0,
     180,     0,     0,     0,     0,   192,     0,   289,   302,     0,
       0,   296,   297,   566,   298,   294,     0,   307,   309,   311,
      55,    60,    61,    64,    65,    39,    38,     0,     0,   171,
     545,   546,     0,     0,   152,   155,   156,   159,   160,     0,
       0,   173,   110,     0,     0,   233,   234,     0,     0,     0,
       0,   236,   230,   191,     0,     0,     0,     0,     0,     0,
       0,   274,   231,   116,   117,   118,   543,   492,   519,   558,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   323,
     330,   336,   337,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   489,   486,   357,     0,   488,
       0,   491,     0,   485,     0,     0,   490,    68,     0,     0,
      23,   541,   540,   539,     0,     0,     0,   479,   474,   475,
       0,     0,     0,   281,   471,   510,   508,     0,     0,   512,
       0,     0,   285,   284,     0,     0,     0,    10,    28,     0,
      29,     0,    32,   185,     0,    41,    40,     0,   519,   519,
       0,   143,     0,   144,     0,     0,     0,   177,     0,   183,
       0,   179,     0,     0,   195,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   193,   291,   304,     0,     0,   299,
       0,     0,     0,     0,    76,    77,     0,     0,     0,   174,
     109,   229,     0,     0,     0,     0,     0,     0,     0,     0,
     190,   188,     0,     0,     0,     0,     0,     0,   538,   544,
     333,     0,     0,     0,     0,     0,     0,   341,   342,   343,
     344,   315,   317,   319,   322,   320,   321,   316,   318,     0,
       0,    70,   487,    71,   484,   483,    67,   519,    73,    74,
      75,   280,   282,     0,     0,     0,   514,     0,     0,    20,
      27,     0,     0,     0,     0,   184,    14,     0,    50,    53,
     138,   137,   517,     0,   148,   168,   165,   166,   178,   182,
       0,     0,     0,   196,   197,   194,     0,     0,     0,     0,
       0,     0,     0,     0,   290,   303,     0,     0,   300,   301,
     176,   172,   153,     0,     0,   164,     0,     3,   268,     0,
     265,     0,     0,     0,     0,   242,     0,     0,     0,     0,
     237,     0,     0,     0,     0,   238,   239,     0,     0,   187,
     189,   276,   277,   278,     0,     0,     0,   324,   326,   329,
     327,   328,   325,     0,     0,    24,     0,     0,   515,   513,
     519,   287,   286,     0,    30,     0,   145,   146,     0,     0,
       0,   198,   200,   202,   205,   203,   204,   199,   201,   292,
     305,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   255,     0,     0,     0,   256,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,   243,   240,   241,     0,
       0,   271,   119,     0,     0,   331,   332,   283,   280,    21,
     519,    15,     0,   149,   206,   208,   207,     0,   163,   267,
       0,   210,   552,     0,     0,     0,   209,     0,   264,     0,
     253,   254,   246,     0,   249,     0,   252,   245,     0,     0,
     257,     0,     0,     0,   258,     0,     0,     0,     0,     0,
     334,   335,    31,   147,     0,   266,     0,     0,     0,   214,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     263,     0,     0,   261,   262,   248,   259,   260,   247,   273,
       0,   270,     0,     0,     0,     0,     0,     0,   215,   216,
     213,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     251,   272,   269,   162,   211,     0,     0,     0,   217,   219,
     221,   224,   222,   223,   218,   220,   225,   227,   226
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -870,  -870,     6,  -870,  -684,  -869,  -870,  -870,  -870,  -870,
    -870,   287,  1562,  -870,  -870,  -870,   721,  -870,  -396,  -840,
       1,  -870,  -870,  -870,  -870,   862,  -870,  -870,  -611,   943,
    -397,   495,  1283,    -2,  -263,  -870,   760,  1340,   667,  1314,
    -870,   736,   -25,   840,  1023,   852,    -3,  -870,  -148,  -333,
    -870,     7,   425,     9,    10
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   252,    74,    75,   693,    76,    77,    78,    79,
      80,  1184,    81,    82,   476,   718,   889,    83,   431,  1188,
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
      89,   523,   597,   168,   600,   673,     2,   878,  1041,   105,
     404,   106,   107,   138,   823,   145,   888,   169,   827,  1048,
     830,    26,   160,   162,   164,   166,   167,   146,    28,   174,
     176,   178,   700,     3,   182,   183,   635,   636,   187,   189,
     432,   433,   481,   742,   434,   385,   204,   205,   128,   122,
     209,   211,   743,    73,   217,   627,   214,   129,   839,   130,
     225,   218,   219,   230,   231,   232,   416,   432,   433,   248,
     250,   434,   432,   433,   702,   772,   434,   432,   433,   739,
     740,   434,   215,   747,   181,  1013,   417,   193,   110,   111,
     917,   918,   184,   432,   433,   773,   482,   434,   899,  1067,
     216,   435,   436,   437,   432,   433,   338,   194,   434,   123,
     340,   341,   342,   343,   344,   345,   416,   348,   350,   352,
     353,   358,   355,  1191,   359,   356,   185,   124,   435,   436,
     437,  1176,  1192,   435,   436,   437,   417,   199,   435,   436,
     437,   432,   433,   179,   381,   434,   383,   180,   152,   153,
     154,  1196,   155,   196,   435,   436,   437,   405,   118,   415,
    1197,   420,   378,   422,   379,   435,   436,   437,   432,   433,
     156,   360,   434,   197,  1154,   157,   492,   430,    73,    73,
     430,    73,   170,  1193,   456,   207,   458,    73,   416,   463,
     828,   464,   465,   994,   466,   467,   386,   468,   470,   471,
     468,   472,   435,   436,   437,   497,   386,   498,   417,   840,
     699,  1198,   424,   744,   129,   703,   130,   418,   814,   815,
     419,   489,   191,   741,   970,   416,   195,    73,   251,   435,
     436,   437,  1267,   774,   919,  1272,   840,   746,   195,   900,
    1068,  1268,   859,  1190,  1273,   417,   441,   945,   817,   818,
     510,   513,   514,   515,   516,   517,   518,   519,   520,   511,
     521,   106,   512,  1253,   816,   775,   442,   418,   441,   441,
     419,   626,  1177,  1194,  1257,   860,   695,   444,   565,   566,
     567,   568,   569,   570,   571,   776,   441,  1195,   442,   442,
     814,   815,  1269,   554,   819,  1274,   406,   445,  1025,   407,
     408,  1199,   884,   890,   198,   425,   442,  1026,    73,   409,
     226,  1326,   410,   411,   704,  1200,   412,   158,   798,   584,
     585,   586,   587,   588,   589,   590,   591,   592,   593,   594,
     595,   596,   212,   799,   598,   487,  1097,   488,  1328,   418,
     800,   778,   419,   170,   779,   780,  1164,   612,    73,   262,
     943,   946,   947,   799,   781,   368,  1150,   782,   783,   268,
     800,   784,   785,   129,   632,   130,   634,  1293,  1281,   637,
     638,   639,   640,   641,   642,   643,   418,   125,   126,   419,
     705,   706,  1270,  1369,   886,  1275,   653,   654,   655,   387,
     656,   658,   388,   659,   664,   388,  1271,   443,    73,  1276,
     313,   314,   697,   698,   678,   679,   680,   681,   682,   683,
     684,   125,   126,  1299,   687,   688,   198,  1308,   963,   443,
     443,   407,   964,   777,   849,   887,   972,  1165,   446,   310,
     311,   965,   127,   228,   966,   967,   701,   443,   968,   710,
     711,   229,   712,   713,  1370,   714,   715,   413,   122,   439,
     414,   720,   440,   722,   985,   986,   119,   389,   726,   727,
    1335,  1056,   377,  1033,  1057,  1058,   312,   735,   302,  1350,
     310,   311,   737,   367,  1059,   938,   939,  1060,  1061,   940,
     366,  1062,  1063,   752,   753,   754,  1359,  1069,  1361,   804,
     751,   368,   261,  1049,   119,  1051,   805,    88,   617,   786,
     618,   767,   768,   769,   771,   938,   939,   312,   123,   940,
     948,   949,   950,   803,   120,   390,   129,   402,   130,   334,
    1381,   403,  1382,   649,   171,   173,   765,   336,   804,   391,
     335,   307,   121,   307,   987,   805,   650,   190,   192,   308,
     309,   427,   428,   347,   392,   206,   208,  1341,  1090,  1091,
    1342,  1343,   120,  1070,  1071,   365,   613,   224,  1160,   614,
    1344,   407,  1161,  1345,  1346,   125,   126,  1347,  1348,   413,
     764,   409,   414,   384,   410,   411,   969,  1365,  1162,   253,
     407,  1366,   254,   255,   395,   665,   310,   311,   310,   311,
     409,   941,   256,   410,   411,   257,   258,  1367,   942,   259,
     260,   337,   393,   125,   126,  1083,   817,   818,   474,   475,
     622,   730,   261,   810,   811,   951,   394,  1100,  1107,  1064,
     766,   941,  1119,   312,   825,   312,   432,   433,  1081,   653,
     434,   171,   904,   905,   310,   311,   906,   799,  1145,   668,
     339,   429,   396,   666,   800,   251,   430,   452,   838,   454,
     495,   496,  1216,   669,  1159,   844,   397,   667,  1336,   421,
     248,   423,   426,   854,   855,   453,   857,   455,   858,   263,
     264,   312,   861,   862,   110,   111,   371,   372,   373,   374,
     375,   617,   457,   831,   459,  1120,   460,   435,   436,   437,
     881,   354,   670,   907,   908,   909,  1209,  1210,   261,   896,
     804,   898,   486,  1313,   461,  1349,   671,   805,   105,   413,
     106,   107,   414,   649,   672,   357,  1163,  1314,  1315,   430,
     366,  1017,   493,   265,   266,   267,   826,   930,   413,   376,
     261,   414,  1316,   366,  1018,  1368,   731,   933,   551,   552,
     553,   732,   401,  1108,   118,   212,  1017,    88,  1109,    89,
     733,   315,   316,   317,   318,   319,  1208,  1110,   105,  1134,
     106,   107,   960,   961,   962,   439,   845,   369,   440,   846,
     973,   974,   971,   975,   976,   370,   977,   978,   979,   980,
     981,   982,   983,   984,   789,   851,   790,   449,   852,  1254,
     791,  1111,  1258,  1112,   320,   321,   988,  1113,   792,   793,
     388,   992,   834,   645,   447,  1114,  1115,   269,   270,   271,
     272,  1004,   794,   795,  1005,   448,  1186,  1006,   473,  1116,
    1117,  1187,   108,   109,  1010,   110,   111,   112,   113,   114,
     115,   116,   110,   111,   371,   372,   373,   374,   375,   685,
     872,   796,   865,   388,   689,   867,  1022,  1023,  1118,  1217,
     696,   388,  1218,   959,  1029,   294,   295,   296,   297,   298,
     299,  1037,   398,   399,   400,   709,   273,   274,   275,   276,
     277,   278,   279,   450,   280,   717,  1189,   451,  1042,   868,
     117,  1187,  1327,  1329,   281,   282,  1219,   485,   300,   301,
    1052,  1053,  1055,   869,   479,   118,  1280,   870,  1066,   477,
    1220,  1187,   118,   480,  1221,   283,   284,   285,   286,   287,
     288,   289,   290,   291,   483,  1072,   871,   789,   484,   759,
     292,   761,   763,  1222,  1241,   322,   491,  1077,   323,   324,
     325,   326,   327,   328,   329,   330,   528,   529,   530,   531,
     532,   533,   534,   535,   536,   537,   538,   539,   540,   541,
     542,   543,   544,   545,   546,   547,   548,   549,   550,   494,
     790,  1101,  1102,  1103,  1104,  1105,  1106,  1242,   736,   791,
     738,   792,   293,   793,   602,   603,  1243,   794,  1244,   233,
    1245,   331,   332,   333,  1246,   795,   499,   796,   234,  1121,
     235,   236,  1247,   601,  1248,  1123,  1285,   604,   865,   237,
     238,   867,  1286,   605,   239,  1294,   871,  1128,  1295,  1132,
     240,   241,   606,  1296,  1330,  1130,  1137,   622,   242,   243,
     244,   245,  1331,   522,  1384,   524,   525,   526,   807,  1082,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   809,
     789,   388,   812,  1003,   790,   791,   792,  1388,   821,   607,
     824,  1389,  1390,  1391,  1166,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,   793,   794,   795,   796,  1175,   865,   867,  1392,
    1393,  1394,  1395,   233,  1396,  1397,  1185,   876,   910,   871,
     388,   388,   234,   608,   235,   236,  1398,   303,   304,   610,
     388,   305,   306,   237,   238,  1214,  1000,  1030,   239,   388,
    1031,   609,   864,  1084,   240,   241,  1085,  1086,  1087,  1088,
     611,   873,   242,   243,   244,   245,  1201,  1223,  1224,  1202,
     619,  1203,  1204,  1207,   233,  1139,  1146,   599,   388,  1147,
     620,  1226,  1227,   234,  1234,   235,   236,   388,  1291,  1231,
    1232,   388,   615,   616,   237,   238,   628,   629,   622,   239,
     572,   573,   574,   630,   631,   240,   241,   621,  1238,  1239,
    1240,   648,   388,   242,   243,   244,   245,   577,   578,   579,
     580,   581,   582,   583,   617,  1250,  1098,   676,   492,   495,
     677,  1251,   495,   686,   623,   877,   879,   690,   496,   882,
     883,   885,  1262,  1256,   691,   692,    88,  1266,   694,   496,
     707,   708,   903,   723,   496,   725,   496,   495,   808,   891,
     708,   944,   708,  1283,  1284,   957,   388,   624,   924,   925,
     926,   958,   388,   625,   990,   991,   998,   388,   633,   995,
     996,   646,   931,   999,   651,   934,  1007,   388,  1008,   629,
    1009,   388,  1014,   629,   814,   815,    88,   817,   818,   233,
     495,  1020,  1024,   852,  1307,  1034,   708,   647,   234,   716,
     235,   236,   674,   105,   675,   106,   107,   719,  1321,   237,
     238,  1021,   724,  1325,   239,  1038,   388,  1039,   388,   721,
     240,   241,  1040,   692,  1092,   708,  1153,   692,   242,   243,
     244,   245,  1093,   708,   734,  1185,  1122,   388,  1124,   629,
     728,  1337,  1338,  1340,  1125,   388,   729,  1127,   388,  1292,
     692,  1351,   748,  1352,   495,  1143,  1152,   496,  1230,   388,
    1290,   388,  1300,   708,  1309,   708,  1360,   708,  1362,   708,
     201,   203,   730,   731,  1364,   732,   575,   576,   733,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,  1378,   749,   755,   756,
     757,   787,   788,   797,   789,   790,   791,   792,   793,   794,
     795,   796,   293,  1385,  1386,  1387,   801,   802,   813,   833,
     835,   836,   832,   841,   842,   843,  1043,   847,   848,   853,
     863,   897,   901,   902,   865,   866,   867,   868,   869,   870,
     871,   874,   920,   875,   922,   911,   912,   913,   929,   914,
     915,   916,   935,   936,  1011,   388,   927,  1012,   928,   956,
     953,   921,  1015,  1073,   923,   954,   932,   430,   955,   997,
    1019,  1001,  1002,  1016,  1027,  1028,  1044,  1032,  1079,  1035,
    1036,  1045,  1046,  1047,  1065,  1074,  1076,  1075,  1078,  1089,
    1080,  1094,  1095,  1096,  1121,  1138,  1099,  -112,  1123,  1126,
    -112,  -112,  -112,  -112,  -112,  1128,  1144,  1129,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  1130,  1131,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  1133,
    1135,  1140,  1141,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,  -112,
    1142,  1148,  1158,  -112,  -112,  1149,  1174,  -112,  -112,  1178,
    1179,  1181,  1183,  -112,  1205,  1206,  1211,  -112,  1212,  1213,
    1215,  -112,  -112,  1225,  1229,  1151,  -112,  1228,  1249,  1155,
    1156,  1157,  1252,  1255,  1259,  1260,  1261,  1263,  1264,  1265,
    1301,  1318,  1277,  1278,  1319,  1279,  1282,  -112,  -112,  1287,
    1288,  1289,  1297,  1310,  1311,  1187,  1322,  1180,  1233,  1312,
    1317,  1182,  1235,  1320,  1323,  1324,  1332,  1353,  1354,  1355,
    1356,  1357,  1358,  1379,  1334,     4,     5,     6,     7,     8,
    1363,  1380,   172,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,   850,   937,   555,   527,     0,
       0,     0,  1050,  -112,     0,   136,    30,     0,     0,  -112,
    -112,     0,    36,     0,     0,  -112,  -112,  -112,    38,     0,
    -112,     0,  -112,  -112,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,  1236,  1237,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   136,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1298,    66,     0,
      63,    88,     0,     0,    67,    68,     0,     0,     0,     0,
      69,   147,   148,     0,     0,    72,     0,    73,     0,   149,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1333,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   142,   143,
     133,   134,   361,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
     136,    30,     0,    67,    68,     0,     0,    36,     0,    69,
       0,    71,   346,    38,    72,     0,    73,     0,   149,     0,
       0,    46,    47,    48,    49,     0,     0,    52,  1383,     0,
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
      68,     0,     0,     0,     0,    69,     0,    71,   362,     0,
      72,     0,    73,     0,   363,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   142,   143,   133,   134,   135,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,    66,     0,     0,   136,    30,     0,    67,    68,
       0,     0,    36,     0,    69,     0,    71,   382,    38,    72,
       0,    73,     0,   149,     0,     0,    46,    47,    48,    49,
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
      69,     0,    71,     0,     0,    72,   644,    73,     0,   149,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   142,   143,
     133,   134,   135,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
     136,    30,     0,    67,    68,     0,     0,    36,     0,    69,
       0,    71,   758,    38,    72,     0,    73,     0,   149,     0,
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
      68,     0,     0,     0,     0,    69,     0,    71,   760,     0,
      72,     0,    73,     0,   149,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   135,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,   136,    30,     0,     0,    67,    68,
       0,    36,     0,     0,    69,     0,    71,    38,     0,    72,
     762,    73,     0,   149,     0,    46,    47,    48,    49,     0,
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
       0,    71,   806,     0,    72,     0,    73,     0,   149,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   142,   143,   133,
     134,   361,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,   136,
      30,     0,    67,    68,     0,     0,    36,     0,    69,     0,
      71,   820,    38,    72,     0,    73,     0,   149,     0,     0,
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
       0,     0,     0,     0,    69,     0,    71,   822,     0,    72,
       0,    73,     0,   363,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   142,   143,   133,   134,   361,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,   136,    30,     0,    67,    68,     0,
       0,    36,     0,    69,     0,    71,   989,    38,    72,     0,
      73,     0,   149,     0,     0,    46,    47,    48,    49,     0,
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
       0,    71,   993,     0,    72,     0,    73,     0,   363,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   142,   143,   133,   134,
     135,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   136,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
      71,    38,     0,    72,     0,    73,     0,   652,     0,    46,
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
       0,     0,     0,    69,     0,    71,     0,     0,    72,     0,
      73,     0,   149,     0,    64,    65,     0,     0,     0,     0,
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
      72,     0,    73,   952,     4,     5,     6,     7,     8,     0,
       0,     0,     9,    10,    11,  1302,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,  1303,  1304,    26,
       0,     0,    27,    28,    29,    30,    31,  1305,    33,    34,
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
      70,    71,     0,     0,    72,     0,    73,  1306,     0,     0,
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
       0,    69,   175,    71,     0,     0,    72,     0,    73,  1054,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,   175,    71,     0,     0,    72,     0,    73,  1339,     4,
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
      69,     0,    71,  1136,    38,    72,     0,    73,     0,     0,
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
    -564,    71,     0,     0,    72,     0,    73,  -564,  -564,  -564,
       0,  -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,
    -564,  -564,  -564,  -564,  -564,  -564,  -564,     0,     0,     0,
    -564,     0,     0,     0,     0,     0,     0,     0,  -564,  -564,
       0,     0,  -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,
    -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,  -564,
    -564,  -564,  -564,     0,  -564,  -564,     0,     0,  -564,  -564,
    -564,  -564,  -564,  -564,  -564,  -564,  -564,     0,  -564,  -564,
    -564,  -564,  -564,  -564,  -564,  -564,     0,     0,  -564,  -564,
    -564,  -564,  -564,     0,     0,  -564,  -564,  -564,     0,     0,
     490
};

static const yytype_int16 yycheck[] =
{
       2,   264,   335,    28,   337,   401,     0,   691,   877,     2,
     158,     2,     2,    16,   625,    17,    24,    15,    15,    24,
     631,    28,    24,    25,    26,    27,    28,    15,    32,    31,
      32,    33,   429,     0,    36,    37,   369,   370,    40,    41,
      11,    12,    15,    15,    15,    15,    48,    49,    12,   107,
      52,    53,    24,   185,    57,    15,    55,   178,    15,   180,
      62,    60,    61,    66,    67,    68,    28,    11,    12,    71,
      72,    15,    11,    12,    24,    28,    15,    11,    12,    42,
      43,    15,    15,   480,    15,    15,    48,    28,   110,   111,
      42,    43,    13,    11,    12,    48,    69,    15,    24,    24,
      33,    72,    73,    74,    11,    12,   108,    48,    15,   167,
     112,   113,   114,   115,   116,   117,    28,   119,   120,   121,
     122,    15,   124,    15,    18,   127,    13,   185,    72,    73,
      74,    24,    24,    72,    73,    74,    48,   185,    72,    73,
      74,    11,    12,    11,   146,    15,   148,    15,    11,    12,
      13,    15,    15,    28,    72,    73,    74,   159,   180,   161,
      24,   163,   182,   165,   184,    72,    73,    74,    11,    12,
      33,    65,    15,    48,  1043,    38,   182,   185,   185,   185,
     185,   185,   180,    75,   186,   185,   188,   185,    28,   191,
     187,   193,   194,   804,   196,   197,   166,   199,   200,   201,
     202,   203,    72,    73,    74,   182,   166,   184,    48,   166,
     181,    75,    89,   185,   178,   165,   180,   179,   119,   120,
     182,   223,   180,   186,   186,    28,   179,   185,    79,    72,
      73,    74,    15,   186,   186,    15,   166,   181,   179,   165,
     165,    24,   181,  1083,    24,    48,    28,   181,   119,   120,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   252,
     262,   252,   252,   181,   165,    28,    48,   179,    28,    28,
     182,    86,   165,   165,   181,   672,   424,    28,   303,   304,
     305,   306,   307,   308,   309,    48,    28,   179,    48,    48,
     119,   120,    75,   292,   165,    75,    28,    48,     6,    31,
      32,   165,    89,   699,   179,   182,    48,    15,   185,    41,
      11,   181,    44,    45,    11,   179,    48,   180,   165,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,    15,   180,   336,   165,   165,   167,   181,   179,
     187,    28,   182,   180,    31,    32,   186,   349,   185,    86,
     746,   748,   749,   180,    41,   185,  1040,    44,    45,   103,
     187,    48,    49,   178,   366,   180,   368,  1236,  1208,   371,
     372,   373,   374,   375,   376,   377,   179,   110,   111,   182,
      77,    78,   165,   186,    89,   165,   388,   389,   390,   179,
     392,   393,   182,   395,   396,   182,   179,   179,   185,   179,
     162,   163,   427,   428,   406,   407,   408,   409,   410,   411,
     412,   110,   111,  1253,   416,   417,   179,  1257,    28,   179,
     179,    31,    32,   186,    86,    89,   186,   186,   179,   134,
     135,    41,   165,    11,    44,    45,   430,   179,    48,   441,
     442,    11,   444,   445,   186,   447,   448,   179,   107,   179,
     182,   453,   182,   455,   787,   788,   107,   107,   460,   461,
    1300,    28,   165,   859,    31,    32,   171,   469,   117,  1309,
     134,   135,   474,   167,    41,    63,    64,    44,    45,    67,
     183,    48,    49,   485,   486,   487,  1326,    11,  1328,   180,
     484,   185,   179,   889,   107,   891,   187,     2,   182,   186,
     184,   503,   504,   505,   506,    63,    64,   171,   167,    67,
      81,    82,    83,   165,   165,   165,   178,    11,   180,   164,
    1360,    15,  1362,   167,    29,    30,   185,   165,   180,   179,
     178,    79,   183,    79,   797,   187,   180,    42,    43,    87,
      88,    87,    88,   118,   107,    50,    51,    28,   944,   945,
      31,    32,   165,    77,    78,   130,   179,    62,    28,   182,
      41,    31,    32,    44,    45,   110,   111,    48,    49,   179,
     183,    41,   182,   148,    44,    45,   186,    28,    48,    28,
      31,    32,    31,    32,   107,   107,   134,   135,   134,   135,
      41,   179,    41,    44,    45,    44,    45,    48,   186,    48,
      49,   178,   165,   110,   111,   938,   119,   120,    42,    43,
     179,   179,   179,   615,   616,   186,   179,   186,   186,   186,
     165,   179,   165,   171,   626,   171,    11,    12,   186,   631,
      15,   136,    11,    12,   134,   135,    15,   180,  1034,   165,
      15,   180,   165,   165,   187,    79,   185,   167,   650,   167,
     181,   182,   165,   179,  1050,   657,   179,   179,   165,   164,
     662,   166,   167,   665,   666,   185,   668,   185,   670,   104,
     105,   171,   674,   675,   110,   111,   112,   113,   114,   115,
     116,   182,   187,   184,   189,   165,    31,    72,    73,    74,
     692,    15,   165,    72,    73,    74,  1092,  1093,   179,   701,
     180,   703,   165,   165,    49,   186,   179,   187,   701,   179,
     701,   701,   182,   167,   180,   179,   186,   179,   165,   185,
     183,   167,   227,   158,   159,   160,   180,   729,   179,   165,
     179,   182,   179,   183,   180,   186,   179,   739,     8,     9,
      10,   179,    15,   186,   180,    15,   167,   252,   186,   751,
     179,   129,   130,   131,   132,   133,  1089,   186,   751,   180,
     751,   751,   764,   765,   766,   179,   179,   178,   182,   182,
     772,   773,   186,   775,   776,   178,   778,   779,   780,   781,
     782,   783,   784,   785,   179,   179,   179,    76,   182,  1186,
     179,   186,  1189,   186,   172,   173,   798,   186,   179,   179,
     182,   803,   184,   378,   165,   186,   186,    90,    91,    92,
      93,   813,   179,   179,   816,   165,   180,   819,    59,   186,
     186,   185,   107,   108,   826,   110,   111,   112,   113,   114,
     115,   116,   110,   111,   112,   113,   114,   115,   116,   414,
     179,   179,   179,   182,   419,   179,   848,   849,   186,   186,
     425,   182,   186,   184,   856,   136,   137,   138,   139,   140,
     141,   863,    11,    12,    13,   440,   149,   150,   151,   152,
     153,   154,   155,   182,   157,   450,   180,   185,   880,   179,
     165,   185,  1279,  1280,   167,   168,   186,   165,   169,   170,
     892,   893,   894,   179,   185,   180,   180,   179,   900,   179,
     186,   185,   180,   180,   186,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   185,   917,   179,   179,   185,   494,
     109,   496,   497,   186,   186,   118,   179,   929,   121,   122,
     123,   124,   125,   126,   127,   128,   269,   270,   271,   272,
     273,   274,   275,   276,   277,   278,   279,   280,   281,   282,
     283,   284,   285,   286,   287,   288,   289,   290,   291,   180,
     179,   963,   964,   965,   966,   967,   968,   186,   473,   179,
     475,   179,   161,   179,   179,   180,   186,   179,   186,    15,
     186,   174,   175,   176,   186,   179,   186,   179,    24,   179,
      26,    27,   186,   179,   186,   179,   186,   179,   179,    35,
      36,   179,   186,   179,    40,   186,   179,   179,   186,  1011,
      46,    47,   179,   186,   186,   179,  1018,   179,    54,    55,
      56,    57,   186,   263,   186,   265,   266,   267,   603,    65,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   614,
     179,   182,   617,   184,   179,   179,   179,   186,   623,   179,
     625,   186,   186,   186,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,   179,   179,   179,   179,  1068,   179,   179,   186,
     186,   186,   186,    15,   186,   186,  1078,   179,   179,   179,
     182,   182,    24,   179,    26,    27,   186,   143,   144,   181,
     182,   147,   148,    35,    36,  1097,   179,   179,    40,   182,
     182,   179,   677,    11,    46,    47,    14,    15,    16,    17,
     179,   686,    54,    55,    56,    57,    11,  1119,  1120,    14,
     179,    16,    17,    65,    15,   179,   179,    18,   182,   182,
     168,  1133,  1134,    24,   179,    26,    27,   182,   179,  1141,
    1142,   182,   172,   173,    35,    36,   181,   182,   179,    40,
     310,   311,   312,   181,   182,    46,    47,   186,  1160,  1161,
    1162,   181,   182,    54,    55,    56,    57,   315,   316,   317,
     318,   319,   320,   321,   182,  1177,   184,   181,   182,   181,
     182,  1183,   181,   182,   180,   690,   691,   181,   182,   694,
     695,   696,  1194,  1187,    29,    30,   701,  1199,   181,   182,
     181,   182,   707,   181,   182,   181,   182,   181,   182,   181,
     182,   181,   182,  1215,  1216,   181,   182,    15,   723,   724,
     725,   181,   182,   180,   799,   800,   181,   182,    15,   804,
     805,   179,   737,   808,    15,   740,   181,   182,   181,   182,
     181,   182,   181,   182,   119,   120,   751,   119,   120,    15,
     181,   182,   181,   182,  1256,   181,   182,   179,    24,   179,
      26,    27,   165,  1256,   165,  1256,  1256,    15,  1270,    35,
      36,   846,    29,  1275,    40,   181,   182,   181,   182,    15,
      46,    47,    29,    30,   181,   182,    29,    30,    54,    55,
      56,    57,   181,   182,   186,  1297,   181,   182,   181,   182,
      11,  1303,  1304,  1305,   181,   182,   179,   181,   182,    29,
      30,  1313,   180,  1315,   181,   182,   181,   182,   181,   182,
     181,   182,   181,   182,   181,   182,   181,   182,   181,   182,
      47,    48,   179,   179,  1336,   179,   313,   314,   179,  1341,
    1342,  1343,  1344,  1345,  1346,  1347,  1348,   180,    15,   179,
      11,   178,   178,   106,   179,   179,   179,   179,   179,   179,
     179,   179,   161,  1365,  1366,  1367,   179,    15,   165,   186,
     179,    15,   168,   179,   179,   179,   881,   179,    86,   179,
     165,    15,    11,    11,   179,   179,   179,   179,   179,   179,
     179,   179,   168,   179,   168,   179,   179,   179,   165,   179,
     179,   179,    15,    15,    86,   182,   179,    15,   179,   168,
     179,   186,   168,   918,   186,   179,   186,   185,   179,   179,
      15,   184,   184,   181,   179,   179,    11,   179,   933,   179,
     179,    11,    11,    15,    15,   179,    29,   179,   179,    63,
     186,    15,    15,    15,   179,  1020,   186,     0,   179,   179,
       3,     4,     5,     6,     7,   179,  1031,   179,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,   179,   181,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,   180,
      15,   179,    86,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      86,   179,    15,    66,    67,   179,    15,    70,    71,    11,
      11,   186,   179,    76,   179,   179,     8,    80,     8,     8,
     165,    84,    85,   179,   181,  1040,    89,   168,    15,  1044,
    1045,  1046,   181,    79,    15,    15,   179,    15,    15,   179,
     186,    15,   179,   179,    15,   180,   179,   110,   111,   181,
     181,   179,   179,   179,   179,   185,    15,  1072,  1143,   179,
     179,  1076,  1147,   179,    15,   179,   179,   179,   179,   179,
     179,   179,   179,   179,  1297,     3,     4,     5,     6,     7,
     181,   179,    30,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   662,   744,   293,   268,    -1,
      -1,    -1,   891,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,   179,   180,    46,    -1,
     183,    -1,   185,   186,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,  1152,  1153,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    89,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,   111,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1252,   166,    -1,
      89,  1256,    -1,    -1,   172,   173,    -1,    -1,    -1,    -1,
     178,   179,   180,    -1,    -1,   183,    -1,   185,    -1,   187,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1292,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      33,    34,    -1,   172,   173,    -1,    -1,    40,    -1,   178,
      -1,   180,   181,    46,   183,    -1,   185,    -1,   187,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,  1363,    -1,
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
     183,    -1,   185,    -1,   187,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    -1,    33,    34,    -1,   172,   173,
      -1,    -1,    40,    -1,   178,    -1,   180,   181,    46,   183,
      -1,   185,    -1,   187,    -1,    -1,    54,    55,    56,    57,
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
     178,    -1,   180,    -1,    -1,   183,   184,   185,    -1,   187,
      -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,
      33,    34,    -1,   172,   173,    -1,    -1,    40,    -1,   178,
      -1,   180,   181,    46,   183,    -1,   185,    -1,   187,    -1,
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
     183,    -1,   185,    -1,   187,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,
      -1,    40,    -1,    -1,   178,    -1,   180,    46,    -1,   183,
     184,   185,    -1,   187,    -1,    54,    55,    56,    57,    -1,
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
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    33,    34,    -1,   172,   173,    -1,
      -1,    40,    -1,   178,    -1,   180,   181,    46,   183,    -1,
     185,    -1,   187,    -1,    -1,    54,    55,    56,    57,    -1,
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
      -1,   180,   181,    -1,   183,    -1,   185,    -1,   187,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,
      -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,    -1,
     180,    46,    -1,   183,    -1,   185,    -1,   187,    -1,    54,
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
     185,    -1,   187,    -1,   110,   111,    -1,    -1,    -1,    -1,
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
      67,   179,   186,   206,   181,   181,   218,   218,    81,    82,
      83,   186,   186,   179,   179,   179,   168,   181,   181,   184,
     221,   221,   221,    28,    32,    41,    44,    45,    48,   186,
     186,   186,   186,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   237,   237,   222,   221,   181,
     240,   240,   221,   181,   216,   240,   240,   179,   181,   240,
     179,   184,   184,   184,   221,   221,   221,   181,   181,   181,
     221,    86,    15,    15,   181,   168,   181,   167,   180,    15,
     182,   240,   221,   221,   181,     6,    15,   179,   179,   221,
     179,   182,   179,   206,   181,   179,   179,   221,   181,   181,
      29,   193,   221,   219,    11,    11,    11,    15,    24,   206,
     204,   206,   221,   221,   186,   221,    28,    31,    32,    41,
      44,    45,    48,    49,   186,    15,   221,    24,   165,    11,
      77,    78,   221,   219,   179,   179,    29,   221,   179,   219,
     186,   186,    65,   237,    11,    14,    15,    16,    17,    63,
     206,   206,   181,   181,    15,    15,    15,   165,   184,   186,
     186,   221,   221,   221,   221,   221,   221,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   165,
     165,   179,   181,   179,   181,   181,   179,   181,   179,   179,
     179,   181,   221,   180,   180,    15,   181,   221,   240,   179,
     179,    86,    86,   182,   240,   206,   179,   182,   179,   179,
     192,   219,   181,    29,   193,   219,   219,   219,    15,   206,
      28,    32,    48,   186,   186,   186,   221,   221,   221,   221,
     221,   221,   221,   221,    15,   221,    24,   165,    11,    11,
     219,   186,   219,   179,   199,   221,   180,   185,   207,   180,
     207,    15,    24,    75,   165,   179,    15,    24,    75,   165,
     179,    11,    14,    16,    17,   179,   179,    65,   237,   206,
     206,     8,     8,     8,   221,   165,   165,   186,   186,   186,
     186,   186,   186,   221,   221,   179,   221,   221,   168,   181,
     181,   221,   221,   240,   179,   240,   219,   219,   221,   221,
     221,   186,   186,   186,   186,   186,   186,   186,   186,    15,
     221,   221,   181,   181,   218,    79,   190,   181,   218,    15,
      15,   179,   221,    15,    15,   179,   221,    15,    24,    75,
     165,   179,    15,    24,    75,   165,   179,   179,   179,   180,
     180,   207,   179,   221,   221,   186,   186,   181,   181,   179,
     181,   179,    29,   193,   186,   186,   186,   179,   219,   207,
     181,   186,    14,    26,    27,    36,   186,   221,   207,   181,
     179,   179,   179,   165,   179,   165,   179,   179,    15,    15,
     179,   221,    15,    15,   179,   221,   181,   218,   181,   218,
     186,   186,   179,   219,   199,   207,   165,   221,   221,   186,
     221,    28,    31,    32,    41,    44,    45,    48,    49,   186,
     207,   221,   221,   179,   179,   179,   179,   179,   179,   207,
     181,   207,   181,   181,   221,    28,    32,    48,   186,   186,
     186,   221,   221,   221,   221,   221,   221,   221,   221,   179,
     179,   207,   207,   219,   186,   221,   221,   221,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186
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
     213,   213,   213,   213,   214,   215,   215,   215,   215,   216,
     216,   216,   216,   216,   217,   217,   217,   217,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   220,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   222,   222,   223,
     223,   223,   223,   223,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   228,   228,   229,   229,   229,
     229,   229,   229,   229,   229,   230,   230,   230,   230,   231,
     231,   231,   232,   232,   232,   232,   232,   232,   232,   232,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   234,   234,   234,   234,   234,   234,   235,
     235,   236,   236,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   237,   238,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   241,   241,   241,   241,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242
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
       5,     5,     0,     3,     3,     0,     2,     4,     4,     4,
       5,     5,     4,     5,     5,     6,     6,     7,     7,     6,
       8,     8,     6,     6,     6,     5,     5,     6,     6,     7,
       7,     7,     7,     7,     6,     4,     7,     6,     4,     8,
       7,     5,     8,     7,     5,     0,     4,     4,     4,     3,
       5,     2,     5,     7,     3,     3,     5,     5,     1,     3,
       5,     4,     6,     1,     3,     2,     3,     3,     3,     4,
       5,     5,     3,     5,     4,     6,     1,     3,     1,     3,
       1,     3,     3,     3,     4,     6,     6,     6,     6,     6,
       6,     6,     6,     5,     7,     7,     7,     7,     7,     7,
       5,     8,     8,     6,     9,     9,     5,     5,     4,     4,
       4,     6,     6,     6,     6,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     3,     1,     5,     1,     3,
       3,     3,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     4,     3,
       4,     6,     4,     3,     6,     6,     5,     3,     3,     6,
       5,     4,     4,     6,     6,     5,     3,     6,     5,     3,
       5,     5,     5,     4,     2,     2,     3,     4,     3,     2,
       3,     2,     1,     1,     3,     2,     2,     3,     5,     4,
       5,     4,     5,     7,     6,     7,     2,     3,     4,     5,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     4,     6,
       6,     6,     1,     4,     4,     5,     5,     5,     5,     5,
       5,     1,     1,     1,     1,     2,     3,     4,     5,     2,
       3,     4,     5,     1,     1,     2,     5,     3
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
#line 754 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 4270 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 764 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 4276 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 765 "raku.y"
                     { if ((yyvsp[0].node) && (yyvsp[0].node)->line == 0) (yyvsp[0].node)->line = (yylsp[0]).first_line; (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 4282 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 769 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 4288 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 771 "raku.y"
        { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), nul); }
#line 4294 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY VAR_SCALAR OP_BIND expr ';'  */
#line 773 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4300 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY VAR_ARRAY OP_BIND expr ';'  */
#line 775 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4306 "raku.tab.c"
    break;

  case 9: /* stmt: KW_MY VAR_HASH OP_BIND expr ';'  */
#line 777 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4312 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY IDENT VAR_SCALAR OP_BIND expr ';'  */
#line 779 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) { ct_drop((yyvsp[-4].sval)); YYERROR; }
          { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,b->c[1]); (yyval.node)=e; } }
#line 4319 "raku.tab.c"
    break;

  case 11: /* stmt: VAR_SCALAR OP_BIND expr ';'  */
#line 782 "raku.y"
        { tree_t *b = rk_bind(var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); if (!b) YYERROR; (yyval.node) = b; }
#line 4325 "raku.tab.c"
    break;

  case 12: /* stmt: VAR_ARRAY OP_BIND expr ';'  */
#line 784 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4331 "raku.tab.c"
    break;

  case 13: /* stmt: VAR_HASH OP_BIND expr ';'  */
#line 786 "raku.y"
        { (void)(yyvsp[-1].node); if (!rk_bind_container()) YYERROR; (yyval.node) = (tree_t *)0; }
#line 4337 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 788 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 4343 "raku.tab.c"
    break;

  case 15: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 790 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 4351 "raku.tab.c"
    break;

  case 16: /* stmt: paren_group '=' expr ';'  */
#line 794 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 4357 "raku.tab.c"
    break;

  case 17: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 796 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4363 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH ';'  */
#line 798 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 4369 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 800 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4375 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 802 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4383 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 806 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4391 "raku.tab.c"
    break;

  case 22: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 810 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4397 "raku.tab.c"
    break;

  case 23: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 812 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4405 "raku.tab.c"
    break;

  case 24: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 816 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4413 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 820 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4419 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 822 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4425 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 824 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4431 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 826 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4437 "raku.tab.c"
    break;

  case 29: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 828 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4443 "raku.tab.c"
    break;

  case 30: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 830 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4451 "raku.tab.c"
    break;

  case 31: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 834 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4459 "raku.tab.c"
    break;

  case 32: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 838 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4465 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 840 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4471 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 842 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4477 "raku.tab.c"
    break;

  case 35: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 844 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4483 "raku.tab.c"
    break;

  case 36: /* stmt: KW_USE IDENT ';'  */
#line 846 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4489 "raku.tab.c"
    break;

  case 37: /* stmt: KW_USE IDENT expr ';'  */
#line 848 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4495 "raku.tab.c"
    break;

  case 38: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 850 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4501 "raku.tab.c"
    break;

  case 39: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 852 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4507 "raku.tab.c"
    break;

  case 40: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 854 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4513 "raku.tab.c"
    break;

  case 41: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 856 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4519 "raku.tab.c"
    break;

  case 42: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 858 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4531 "raku.tab.c"
    break;

  case 43: /* stmt: TESTOP ';'  */
#line 866 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4537 "raku.tab.c"
    break;

  case 44: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 868 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4543 "raku.tab.c"
    break;

  case 45: /* stmt: TESTOP '(' ')' ';'  */
#line 870 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4549 "raku.tab.c"
    break;

  case 46: /* stmt: TESTOP arg_list ';'  */
#line 872 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4555 "raku.tab.c"
    break;

  case 47: /* stmt: IDENT VAR_ARRAY ';'  */
#line 874 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4561 "raku.tab.c"
    break;

  case 48: /* stmt: KW_SAY expr ';'  */
#line 876 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4567 "raku.tab.c"
    break;

  case 49: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 878 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4574 "raku.tab.c"
    break;

  case 50: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 881 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4581 "raku.tab.c"
    break;

  case 51: /* stmt: KW_PRINT expr ';'  */
#line 884 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4587 "raku.tab.c"
    break;

  case 52: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 886 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4594 "raku.tab.c"
    break;

  case 53: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 889 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4601 "raku.tab.c"
    break;

  case 54: /* stmt: KW_TAKE expr ';'  */
#line 892 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4607 "raku.tab.c"
    break;

  case 55: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 894 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4615 "raku.tab.c"
    break;

  case 56: /* stmt: KW_RETURN expr ';'  */
#line 898 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4621 "raku.tab.c"
    break;

  case 57: /* stmt: KW_RETURN ';'  */
#line 900 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4627 "raku.tab.c"
    break;

  case 58: /* stmt: KW_FAIL expr ';'  */
#line 902 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4633 "raku.tab.c"
    break;

  case 59: /* stmt: KW_FAIL ';'  */
#line 904 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4639 "raku.tab.c"
    break;

  case 60: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 906 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4646 "raku.tab.c"
    break;

  case 61: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 909 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4653 "raku.tab.c"
    break;

  case 62: /* stmt: KW_EXIT expr ';'  */
#line 912 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4659 "raku.tab.c"
    break;

  case 63: /* stmt: KW_EXIT ';'  */
#line 914 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4666 "raku.tab.c"
    break;

  case 64: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 917 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4673 "raku.tab.c"
    break;

  case 65: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 920 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4680 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 923 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4686 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 925 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-6].sval)),mc); }
#line 4695 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 930 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-5].sval)),mc); }
#line 4703 "raku.tab.c"
    break;

  case 69: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 934 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=rk_quiet_store(var_node((yyvsp[-3].sval)),mc); }
#line 4711 "raku.tab.c"
    break;

  case 70: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 938 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4718 "raku.tab.c"
    break;

  case 71: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 941 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4725 "raku.tab.c"
    break;

  case 72: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 944 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4733 "raku.tab.c"
    break;

  case 73: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 948 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4740 "raku.tab.c"
    break;

  case 74: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 951 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4747 "raku.tab.c"
    break;

  case 75: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 954 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4754 "raku.tab.c"
    break;

  case 76: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 957 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4761 "raku.tab.c"
    break;

  case 77: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 960 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4768 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_IF expr ';'  */
#line 963 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4774 "raku.tab.c"
    break;

  case 79: /* stmt: expr KW_UNLESS expr ';'  */
#line 965 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4780 "raku.tab.c"
    break;

  case 80: /* stmt: expr KW_WHILE expr ';'  */
#line 967 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4786 "raku.tab.c"
    break;

  case 81: /* stmt: expr KW_UNTIL expr ';'  */
#line 969 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4792 "raku.tab.c"
    break;

  case 82: /* stmt: expr KW_FOR expr ';'  */
#line 971 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4799 "raku.tab.c"
    break;

  case 83: /* stmt: expr KW_WITH expr ';'  */
#line 974 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4805 "raku.tab.c"
    break;

  case 84: /* stmt: expr KW_WITHOUT expr ';'  */
#line 976 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4811 "raku.tab.c"
    break;

  case 85: /* stmt: expr KW_GIVEN expr ';'  */
#line 978 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4817 "raku.tab.c"
    break;

  case 86: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 980 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4824 "raku.tab.c"
    break;

  case 87: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 983 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4831 "raku.tab.c"
    break;

  case 88: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 986 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4839 "raku.tab.c"
    break;

  case 89: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 990 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4846 "raku.tab.c"
    break;

  case 90: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 993 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4852 "raku.tab.c"
    break;

  case 91: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 995 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4858 "raku.tab.c"
    break;

  case 92: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 997 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4864 "raku.tab.c"
    break;

  case 93: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 999 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4871 "raku.tab.c"
    break;

  case 94: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 1002 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4878 "raku.tab.c"
    break;

  case 95: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 1005 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4885 "raku.tab.c"
    break;

  case 96: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 1008 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4892 "raku.tab.c"
    break;

  case 97: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 1011 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4899 "raku.tab.c"
    break;

  case 98: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 1014 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4906 "raku.tab.c"
    break;

  case 99: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 1017 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4913 "raku.tab.c"
    break;

  case 100: /* stmt: expr ';'  */
#line 1019 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4919 "raku.tab.c"
    break;

  case 101: /* stmt: ';'  */
#line 1020 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4925 "raku.tab.c"
    break;

  case 102: /* stmt: if_stmt  */
#line 1021 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4931 "raku.tab.c"
    break;

  case 103: /* stmt: while_stmt  */
#line 1022 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4937 "raku.tab.c"
    break;

  case 104: /* stmt: for_stmt  */
#line 1023 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4943 "raku.tab.c"
    break;

  case 105: /* stmt: given_stmt  */
#line 1024 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4949 "raku.tab.c"
    break;

  case 106: /* stmt: KW_TRY block  */
#line 1026 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4955 "raku.tab.c"
    break;

  case 107: /* stmt: KW_TRY block KW_CATCH block  */
#line 1028 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4961 "raku.tab.c"
    break;

  case 108: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 1030 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4969 "raku.tab.c"
    break;

  case 109: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 1034 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4978 "raku.tab.c"
    break;

  case 110: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 1039 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4986 "raku.tab.c"
    break;

  case 111: /* stmt: KW_CATCH block  */
#line 1043 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4992 "raku.tab.c"
    break;

  case 112: /* stmt: block  */
#line 1045 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4998 "raku.tab.c"
    break;

  case 113: /* stmt: PHASER block  */
#line 1047 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 5004 "raku.tab.c"
    break;

  case 114: /* stmt: PHASER block ';'  */
#line 1049 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 5010 "raku.tab.c"
    break;

  case 115: /* stmt: PHASER expr ';'  */
#line 1051 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),seq1((yyvsp[-1].node))); ct_drop((yyvsp[-2].sval)); }
#line 5016 "raku.tab.c"
    break;

  case 116: /* stmt: PHASER VAR_SCALAR '=' expr ';'  */
#line 1053 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5022 "raku.tab.c"
    break;

  case 117: /* stmt: PHASER VAR_ARRAY '=' expr ';'  */
#line 1055 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_arr_rhs((yyvsp[-1].node))))); ct_drop((yyvsp[-4].sval)); }
#line 5028 "raku.tab.c"
    break;

  case 118: /* stmt: PHASER VAR_HASH '=' expr ';'  */
#line 1057 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-4].sval),seq1(expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),(yyvsp[-1].node)))); ct_drop((yyvsp[-4].sval)); }
#line 5034 "raku.tab.c"
    break;

  case 119: /* stmt: PHASER VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 1059 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node));
          (yyval.node)=rk_phaser_mark((yyvsp[-7].sval),seq1(c)); ct_drop((yyvsp[-7].sval)); }
#line 5041 "raku.tab.c"
    break;

  case 120: /* stmt: PHASER KW_SAY expr ';'  */
#line 1062 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); (yyval.node)=rk_phaser_mark((yyvsp[-3].sval),seq1(s)); ct_drop((yyvsp[-3].sval)); }
#line 5047 "raku.tab.c"
    break;

  case 121: /* stmt: unless_stmt  */
#line 1063 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5053 "raku.tab.c"
    break;

  case 122: /* stmt: until_stmt  */
#line 1064 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5059 "raku.tab.c"
    break;

  case 123: /* stmt: repeat_stmt  */
#line 1065 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5065 "raku.tab.c"
    break;

  case 124: /* stmt: loop_stmt  */
#line 1066 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5071 "raku.tab.c"
    break;

  case 125: /* stmt: KW_LAST ';'  */
#line 1067 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 5077 "raku.tab.c"
    break;

  case 126: /* stmt: KW_NEXT ';'  */
#line 1068 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 5083 "raku.tab.c"
    break;

  case 127: /* stmt: KW_LAST KW_IF expr ';'  */
#line 1070 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5089 "raku.tab.c"
    break;

  case 128: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 1072 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 5095 "raku.tab.c"
    break;

  case 129: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 1074 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5101 "raku.tab.c"
    break;

  case 130: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 1076 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 5107 "raku.tab.c"
    break;

  case 131: /* stmt: sub_decl  */
#line 1077 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5113 "raku.tab.c"
    break;

  case 132: /* stmt: class_decl  */
#line 1078 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5119 "raku.tab.c"
    break;

  case 133: /* stmt: role_decl  */
#line 1079 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5125 "raku.tab.c"
    break;

  case 134: /* stmt: grammar_decl  */
#line 1080 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5131 "raku.tab.c"
    break;

  case 135: /* stmt: module_decl  */
#line 1081 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 5137 "raku.tab.c"
    break;

  case 136: /* if_stmt: KW_IF '(' expr ')' block  */
#line 1085 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5143 "raku.tab.c"
    break;

  case 137: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1087 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5149 "raku.tab.c"
    break;

  case 138: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1089 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5155 "raku.tab.c"
    break;

  case 139: /* if_stmt: KW_IF expr block  */
#line 1091 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5161 "raku.tab.c"
    break;

  case 140: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1093 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5167 "raku.tab.c"
    break;

  case 141: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1095 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5173 "raku.tab.c"
    break;

  case 142: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1097 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5179 "raku.tab.c"
    break;

  case 143: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1099 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5185 "raku.tab.c"
    break;

  case 144: /* elsif_tail: KW_ELSIF expr block  */
#line 1103 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5191 "raku.tab.c"
    break;

  case 145: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1105 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5197 "raku.tab.c"
    break;

  case 146: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1107 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5203 "raku.tab.c"
    break;

  case 147: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1109 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5209 "raku.tab.c"
    break;

  case 148: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1111 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5215 "raku.tab.c"
    break;

  case 149: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1113 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5221 "raku.tab.c"
    break;

  case 150: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1117 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5227 "raku.tab.c"
    break;

  case 151: /* while_stmt: KW_WHILE expr block  */
#line 1119 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5233 "raku.tab.c"
    break;

  case 152: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1123 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5239 "raku.tab.c"
    break;

  case 153: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1125 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5245 "raku.tab.c"
    break;

  case 154: /* unless_stmt: KW_UNLESS expr block  */
#line 1127 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5251 "raku.tab.c"
    break;

  case 155: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1129 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 5257 "raku.tab.c"
    break;

  case 156: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1133 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5263 "raku.tab.c"
    break;

  case 157: /* until_stmt: KW_UNTIL expr block  */
#line 1135 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5269 "raku.tab.c"
    break;

  case 158: /* repeat_stmt: KW_REPEAT block  */
#line 1139 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 5275 "raku.tab.c"
    break;

  case 159: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1141 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5281 "raku.tab.c"
    break;

  case 160: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1143 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 5287 "raku.tab.c"
    break;

  case 161: /* loop_stmt: KW_LOOP block  */
#line 1147 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5293 "raku.tab.c"
    break;

  case 162: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1149 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5299 "raku.tab.c"
    break;

  case 163: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1151 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 5305 "raku.tab.c"
    break;

  case 164: /* loop_incr: expr  */
#line 1154 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 5311 "raku.tab.c"
    break;

  case 165: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1158 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5321 "raku.tab.c"
    break;

  case 166: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1164 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 5331 "raku.tab.c"
    break;

  case 167: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1170 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5343 "raku.tab.c"
    break;

  case 168: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1178 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5353 "raku.tab.c"
    break;

  case 169: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1184 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5362 "raku.tab.c"
    break;

  case 170: /* for_stmt: KW_FOR expr block  */
#line 1189 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 5369 "raku.tab.c"
    break;

  case 171: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1194 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 5381 "raku.tab.c"
    break;

  case 172: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1202 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 5394 "raku.tab.c"
    break;

  case 173: /* catch_when_list: KW_WHEN expr block  */
#line 1213 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 5400 "raku.tab.c"
    break;

  case 174: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1215 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 5406 "raku.tab.c"
    break;

  case 175: /* when_list: %empty  */
#line 1218 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 5412 "raku.tab.c"
    break;

  case 176: /* when_list: when_list KW_WHEN expr block  */
#line 1220 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 5420 "raku.tab.c"
    break;

  case 177: /* sub_trait_list: TESTOP IDENT  */
#line 1225 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5426 "raku.tab.c"
    break;

  case 178: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1226 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5432 "raku.tab.c"
    break;

  case 179: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1230 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5444 "raku.tab.c"
    break;

  case 180: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1238 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5454 "raku.tab.c"
    break;

  case 181: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1244 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5464 "raku.tab.c"
    break;

  case 182: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1250 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5476 "raku.tab.c"
    break;

  case 183: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1258 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5486 "raku.tab.c"
    break;

  case 184: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1264 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5498 "raku.tab.c"
    break;

  case 185: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1272 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5508 "raku.tab.c"
    break;

  case 186: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1278 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5518 "raku.tab.c"
    break;

  case 187: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1284 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5531 "raku.tab.c"
    break;

  case 188: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1293 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5542 "raku.tab.c"
    break;

  case 189: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1300 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5555 "raku.tab.c"
    break;

  case 190: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1309 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5568 "raku.tab.c"
    break;

  case 191: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1318 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5579 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list '}'  */
#line 1326 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5585 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list expr '}'  */
#line 1328 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5592 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1331 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5599 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1334 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5605 "raku.tab.c"
    break;

  case 196: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1336 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5611 "raku.tab.c"
    break;

  case 197: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1338 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5617 "raku.tab.c"
    break;

  case 198: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1340 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5623 "raku.tab.c"
    break;

  case 199: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1342 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5629 "raku.tab.c"
    break;

  case 200: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1344 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5635 "raku.tab.c"
    break;

  case 201: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1346 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5641 "raku.tab.c"
    break;

  case 202: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1348 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5648 "raku.tab.c"
    break;

  case 203: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1351 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5654 "raku.tab.c"
    break;

  case 204: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1353 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5660 "raku.tab.c"
    break;

  case 205: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1355 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5666 "raku.tab.c"
    break;

  case 206: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1357 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5673 "raku.tab.c"
    break;

  case 207: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1360 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5680 "raku.tab.c"
    break;

  case 208: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1363 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5688 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list '}'  */
#line 1368 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place(rk_tail_value((yyvsp[-1].list)),0)); }
#line 5694 "raku.tab.c"
    break;

  case 210: /* method_body: '{' YADA '}'  */
#line 1369 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5700 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1371 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5707 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list expr '}'  */
#line 1374 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5714 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1377 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 5721 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1380 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 5727 "raku.tab.c"
    break;

  case 215: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1382 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 5733 "raku.tab.c"
    break;

  case 216: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1384 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 5739 "raku.tab.c"
    break;

  case 217: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1386 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5745 "raku.tab.c"
    break;

  case 218: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1388 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5751 "raku.tab.c"
    break;

  case 219: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1390 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5757 "raku.tab.c"
    break;

  case 220: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1392 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5763 "raku.tab.c"
    break;

  case 221: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1394 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5770 "raku.tab.c"
    break;

  case 222: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1397 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5776 "raku.tab.c"
    break;

  case 223: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1399 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5782 "raku.tab.c"
    break;

  case 224: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1401 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 5788 "raku.tab.c"
    break;

  case 225: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1403 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5795 "raku.tab.c"
    break;

  case 226: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1406 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5802 "raku.tab.c"
    break;

  case 227: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1409 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 5810 "raku.tab.c"
    break;

  case 228: /* pkg_name: IDENT  */
#line 1414 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5816 "raku.tab.c"
    break;

  case 229: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1418 "raku.y"
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
#line 5834 "raku.tab.c"
    break;

  case 230: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1434 "raku.y"
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
#line 5851 "raku.tab.c"
    break;

  case 231: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1449 "raku.y"
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
#line 5868 "raku.tab.c"
    break;

  case 232: /* is_clauses: %empty  */
#line 1463 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5874 "raku.tab.c"
    break;

  case 233: /* is_clauses: is_clauses IDENT IDENT  */
#line 1465 "raku.y"
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
#line 5890 "raku.tab.c"
    break;

  case 234: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1477 "raku.y"
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
#line 5906 "raku.tab.c"
    break;

  case 235: /* class_body_list: %empty  */
#line 1490 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5912 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list ';'  */
#line 1491 "raku.y"
                           { (yyval.list) = (yyvsp[-1].list); }
#line 5918 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1493 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5925 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1496 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5932 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1499 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5939 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1502 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5946 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1505 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5953 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1508 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5960 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1511 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5967 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1514 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5974 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1517 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5981 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1520 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5988 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1523 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5995 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1526 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6002 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1529 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6013 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1536 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6023 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1542 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 6033 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1548 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6044 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1555 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6055 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1562 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6066 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1569 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6074 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1573 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 6082 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1577 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6090 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1581 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 6098 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1585 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6109 "raku.tab.c"
    break;

  case 260: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1592 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6120 "raku.tab.c"
    break;

  case 261: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1599 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6131 "raku.tab.c"
    break;

  case 262: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1606 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 6142 "raku.tab.c"
    break;

  case 263: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1613 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6156 "raku.tab.c"
    break;

  case 264: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1623 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6168 "raku.tab.c"
    break;

  case 265: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1631 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6180 "raku.tab.c"
    break;

  case 266: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1639 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6193 "raku.tab.c"
    break;

  case 267: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1648 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 6204 "raku.tab.c"
    break;

  case 268: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1655 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 6215 "raku.tab.c"
    break;

  case 269: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1662 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6229 "raku.tab.c"
    break;

  case 270: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1672 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6241 "raku.tab.c"
    break;

  case 271: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1680 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 6253 "raku.tab.c"
    break;

  case 272: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1688 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 6266 "raku.tab.c"
    break;

  case 273: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1697 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 6277 "raku.tab.c"
    break;

  case 274: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1706 "raku.y"
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
#line 6294 "raku.tab.c"
    break;

  case 275: /* grammar_body_list: %empty  */
#line 1720 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 6300 "raku.tab.c"
    break;

  case 276: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1722 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6309 "raku.tab.c"
    break;

  case 277: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1727 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6318 "raku.tab.c"
    break;

  case 278: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1732 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 6327 "raku.tab.c"
    break;

  case 279: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1739 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 6335 "raku.tab.c"
    break;

  case 280: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1743 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 6343 "raku.tab.c"
    break;

  case 281: /* named_arg_list: ':' IDENT  */
#line 1747 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 6354 "raku.tab.c"
    break;

  case 282: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1754 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 6362 "raku.tab.c"
    break;

  case 283: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1758 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 6370 "raku.tab.c"
    break;

  case 284: /* pair_list: IDENT OP_FATARROW expr  */
#line 1764 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6376 "raku.tab.c"
    break;

  case 285: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1766 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6382 "raku.tab.c"
    break;

  case 286: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1768 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6388 "raku.tab.c"
    break;

  case 287: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1770 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 6394 "raku.tab.c"
    break;

  case 288: /* param_list: VAR_SCALAR  */
#line 1773 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 6400 "raku.tab.c"
    break;

  case 289: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1774 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 6406 "raku.tab.c"
    break;

  case 290: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1775 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 6412 "raku.tab.c"
    break;

  case 291: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1776 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6418 "raku.tab.c"
    break;

  case 292: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1777 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 6424 "raku.tab.c"
    break;

  case 293: /* param_list: VAR_ARRAY  */
#line 1778 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6430 "raku.tab.c"
    break;

  case 294: /* param_list: param_list ',' VAR_ARRAY  */
#line 1779 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6436 "raku.tab.c"
    break;

  case 295: /* param_list: IDENT VAR_SCALAR  */
#line 1780 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6442 "raku.tab.c"
    break;

  case 296: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1781 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6448 "raku.tab.c"
    break;

  case 297: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1782 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6454 "raku.tab.c"
    break;

  case 298: /* param_list: param_list ',' VAR_SCALAR  */
#line 1783 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6460 "raku.tab.c"
    break;

  case 299: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1784 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6466 "raku.tab.c"
    break;

  case 300: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1785 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6472 "raku.tab.c"
    break;

  case 301: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1786 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6478 "raku.tab.c"
    break;

  case 302: /* param_list: VAR_SCALAR '=' expr  */
#line 1787 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6484 "raku.tab.c"
    break;

  case 303: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1788 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6490 "raku.tab.c"
    break;

  case 304: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1789 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6496 "raku.tab.c"
    break;

  case 305: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1790 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6502 "raku.tab.c"
    break;

  case 306: /* param_list: SLURPY_POS  */
#line 1791 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6508 "raku.tab.c"
    break;

  case 307: /* param_list: param_list ',' SLURPY_POS  */
#line 1792 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6514 "raku.tab.c"
    break;

  case 308: /* param_list: SLURPY_LOL  */
#line 1793 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6520 "raku.tab.c"
    break;

  case 309: /* param_list: param_list ',' SLURPY_LOL  */
#line 1794 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6526 "raku.tab.c"
    break;

  case 310: /* param_list: SLURPY_NAMED  */
#line 1795 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6532 "raku.tab.c"
    break;

  case 311: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1796 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6538 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list '}'  */
#line 1799 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6544 "raku.tab.c"
    break;

  case 313: /* block: '{' YADA '}'  */
#line 1800 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6550 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list expr '}'  */
#line 1802 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6556 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1804 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6562 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1806 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6568 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1808 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6574 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1810 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6580 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1812 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6587 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1815 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6593 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1817 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6599 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1819 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6605 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1821 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,s); (yyval.node)=make_seq(l); }
#line 6611 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1823 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6618 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1826 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6625 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1829 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6633 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1833 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6640 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1836 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6647 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1839 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append_at(l,(yylsp[-4]).first_line,e); (yyval.node)=make_seq(l); }
#line 6654 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1842 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,p); (yyval.node)=make_seq(l); }
#line 6660 "raku.tab.c"
    break;

  case 331: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1844 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6667 "raku.tab.c"
    break;

  case 332: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1847 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append_at(l,(yylsp[-5]).first_line,a); (yyval.node)=make_seq(l); }
#line 6674 "raku.tab.c"
    break;

  case 333: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1850 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,a); (yyval.node)=make_seq(l); }
#line 6681 "raku.tab.c"
    break;

  case 334: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1853 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6688 "raku.tab.c"
    break;

  case 335: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1856 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append_at(l,(yylsp[-6]).first_line,c); (yyval.node)=make_seq(l); }
#line 6695 "raku.tab.c"
    break;

  case 336: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1859 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,t); (yyval.node)=make_seq(l); }
#line 6701 "raku.tab.c"
    break;

  case 337: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1861 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append_at(l,(yylsp[-2]).first_line,r); (yyval.node)=make_seq(l); }
#line 6707 "raku.tab.c"
    break;

  case 338: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1863 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,r); (yyval.node)=make_seq(l); }
#line 6713 "raku.tab.c"
    break;

  case 339: /* block: '{' stmt_list KW_LAST '}'  */
#line 1865 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6719 "raku.tab.c"
    break;

  case 340: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1867 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append_at(l,(yylsp[-1]).first_line,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6725 "raku.tab.c"
    break;

  case 341: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1869 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6731 "raku.tab.c"
    break;

  case 342: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1871 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6737 "raku.tab.c"
    break;

  case 343: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1873 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6743 "raku.tab.c"
    break;

  case 344: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1875 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append_at(l,(yylsp[-3]).first_line,e); (yyval.node)=make_seq(l); }
#line 6749 "raku.tab.c"
    break;

  case 345: /* closure: '{' expr '}'  */
#line 1878 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6755 "raku.tab.c"
    break;

  case 346: /* expr: VAR_SCALAR '=' expr  */
#line 1881 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6761 "raku.tab.c"
    break;

  case 347: /* expr: VAR_ARRAY '=' expr  */
#line 1882 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6767 "raku.tab.c"
    break;

  case 348: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1883 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6773 "raku.tab.c"
    break;

  case 349: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1884 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6779 "raku.tab.c"
    break;

  case 350: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1885 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6785 "raku.tab.c"
    break;

  case 351: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1886 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6791 "raku.tab.c"
    break;

  case 352: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1887 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6797 "raku.tab.c"
    break;

  case 353: /* expr: KW_GATHER block  */
#line 1888 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6807 "raku.tab.c"
    break;

  case 354: /* expr: KW_GATHER for_stmt  */
#line 1893 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6817 "raku.tab.c"
    break;

  case 355: /* expr: tern_expr OP_FATARROW expr  */
#line 1899 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6823 "raku.tab.c"
    break;

  case 356: /* expr: tern_expr  */
#line 1900 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6829 "raku.tab.c"
    break;

  case 357: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1904 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6835 "raku.tab.c"
    break;

  case 358: /* tern_expr: or_expr  */
#line 1905 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6841 "raku.tab.c"
    break;

  case 359: /* or_expr: or_expr OP_OR and_expr  */
#line 1908 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6847 "raku.tab.c"
    break;

  case 360: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1909 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6853 "raku.tab.c"
    break;

  case 361: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1910 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6859 "raku.tab.c"
    break;

  case 362: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1911 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6865 "raku.tab.c"
    break;

  case 363: /* or_expr: and_expr  */
#line 1912 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6871 "raku.tab.c"
    break;

  case 364: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1915 "raku.y"
                               { (yyval.node)=rk_logical_and((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6877 "raku.tab.c"
    break;

  case 365: /* and_expr: cmp_expr  */
#line 1916 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6883 "raku.tab.c"
    break;

  case 366: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1919 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_EQ,rk_numeric_ctx((yyvsp[0].node))); }
#line 6889 "raku.tab.c"
    break;

  case 367: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1920 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6895 "raku.tab.c"
    break;

  case 368: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1921 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6901 "raku.tab.c"
    break;

  case 369: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1922 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6907 "raku.tab.c"
    break;

  case 370: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1923 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6913 "raku.tab.c"
    break;

  case 371: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1924 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6919 "raku.tab.c"
    break;

  case 372: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1925 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6925 "raku.tab.c"
    break;

  case 373: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1926 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6931 "raku.tab.c"
    break;

  case 374: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1927 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6937 "raku.tab.c"
    break;

  case 375: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1928 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_NE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6943 "raku.tab.c"
    break;

  case 376: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1929 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6949 "raku.tab.c"
    break;

  case 377: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1930 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GT,rk_numeric_ctx((yyvsp[0].node))); }
#line 6955 "raku.tab.c"
    break;

  case 378: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1931 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_LE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6961 "raku.tab.c"
    break;

  case 379: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1932 "raku.y"
                                  { (yyval.node)=rk_chain_cmp(rk_numeric_ctx((yyvsp[-2].node)),TT_GE,rk_numeric_ctx((yyvsp[0].node))); }
#line 6967 "raku.tab.c"
    break;

  case 380: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1933 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6973 "raku.tab.c"
    break;

  case 381: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1934 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6979 "raku.tab.c"
    break;

  case 382: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1935 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6985 "raku.tab.c"
    break;

  case 383: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1936 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6991 "raku.tab.c"
    break;

  case 384: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1937 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6997 "raku.tab.c"
    break;

  case 385: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1938 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7003 "raku.tab.c"
    break;

  case 386: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1939 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7009 "raku.tab.c"
    break;

  case 387: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1940 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7015 "raku.tab.c"
    break;

  case 388: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1941 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7021 "raku.tab.c"
    break;

  case 389: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1943 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 7031 "raku.tab.c"
    break;

  case 390: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1949 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 7041 "raku.tab.c"
    break;

  case 391: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1955 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 7051 "raku.tab.c"
    break;

  case 392: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1961 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 7061 "raku.tab.c"
    break;

  case 393: /* cmp_expr: divis_expr  */
#line 1966 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 7067 "raku.tab.c"
    break;

  case 394: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1969 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7073 "raku.tab.c"
    break;

  case 395: /* divis_expr: jct_expr  */
#line 1970 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7079 "raku.tab.c"
    break;

  case 396: /* jct_expr: jct_expr '|' range_expr  */
#line 1973 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7085 "raku.tab.c"
    break;

  case 397: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1974 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7091 "raku.tab.c"
    break;

  case 398: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1975 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7097 "raku.tab.c"
    break;

  case 399: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1976 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7103 "raku.tab.c"
    break;

  case 400: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1977 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7109 "raku.tab.c"
    break;

  case 401: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1978 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7115 "raku.tab.c"
    break;

  case 402: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1979 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7121 "raku.tab.c"
    break;

  case 403: /* jct_expr: jct_expr '&' range_expr  */
#line 1980 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7127 "raku.tab.c"
    break;

  case 404: /* jct_expr: dor_expr  */
#line 1981 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7133 "raku.tab.c"
    break;

  case 405: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1985 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7139 "raku.tab.c"
    break;

  case 406: /* dor_expr: range_expr  */
#line 1986 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 7145 "raku.tab.c"
    break;

  case 407: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1989 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7151 "raku.tab.c"
    break;

  case 408: /* range_expr: add_expr YADA add_expr  */
#line 1990 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7157 "raku.tab.c"
    break;

  case 409: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1991 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7163 "raku.tab.c"
    break;

  case 410: /* range_expr: range_expr OP_COLL add_expr  */
#line 1992 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7169 "raku.tab.c"
    break;

  case 411: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1993 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7175 "raku.tab.c"
    break;

  case 412: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1994 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7181 "raku.tab.c"
    break;

  case 413: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1995 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 7187 "raku.tab.c"
    break;

  case 414: /* range_expr: add_expr  */
#line 1996 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 7193 "raku.tab.c"
    break;

  case 415: /* add_expr: add_expr '~' repl_expr  */
#line 1999 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7199 "raku.tab.c"
    break;

  case 416: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 2000 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7205 "raku.tab.c"
    break;

  case 417: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 2001 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7211 "raku.tab.c"
    break;

  case 418: /* add_expr: repl_expr  */
#line 2002 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 7217 "raku.tab.c"
    break;

  case 419: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 2005 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 7223 "raku.tab.c"
    break;

  case 420: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 2006 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 7229 "raku.tab.c"
    break;

  case 421: /* repl_expr: addsub_expr  */
#line 2007 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7235 "raku.tab.c"
    break;

  case 422: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 2010 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7241 "raku.tab.c"
    break;

  case 423: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 2011 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7247 "raku.tab.c"
    break;

  case 424: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 2012 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7253 "raku.tab.c"
    break;

  case 425: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 2013 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7259 "raku.tab.c"
    break;

  case 426: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 2014 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7265 "raku.tab.c"
    break;

  case 427: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 2015 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7271 "raku.tab.c"
    break;

  case 428: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 2016 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7277 "raku.tab.c"
    break;

  case 429: /* addsub_expr: mul_expr  */
#line 2017 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 7283 "raku.tab.c"
    break;

  case 430: /* mul_expr: mul_expr '*' unary_expr  */
#line 2020 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7289 "raku.tab.c"
    break;

  case 431: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 2021 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7295 "raku.tab.c"
    break;

  case 432: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 2022 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7301 "raku.tab.c"
    break;

  case 433: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 2023 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7307 "raku.tab.c"
    break;

  case 434: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 2024 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7313 "raku.tab.c"
    break;

  case 435: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 2025 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7319 "raku.tab.c"
    break;

  case 436: /* mul_expr: mul_expr '/' unary_expr  */
#line 2026 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7325 "raku.tab.c"
    break;

  case 437: /* mul_expr: mul_expr '%' unary_expr  */
#line 2027 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7331 "raku.tab.c"
    break;

  case 438: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 2029 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,rk_numeric_ctx((yyvsp[-2].node))); expr_add_child(c,rk_numeric_ctx((yyvsp[0].node))); (yyval.node)=c; }
#line 7337 "raku.tab.c"
    break;

  case 439: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 2031 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7343 "raku.tab.c"
    break;

  case 440: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 2033 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7349 "raku.tab.c"
    break;

  case 441: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 2035 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 7355 "raku.tab.c"
    break;

  case 442: /* mul_expr: unary_expr  */
#line 2036 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7361 "raku.tab.c"
    break;

  case 443: /* unary_expr: '-' unary_expr  */
#line 2039 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,rk_numeric_ctx((yyvsp[0].node))); }
#line 7367 "raku.tab.c"
    break;

  case 444: /* unary_expr: '+' unary_expr  */
#line 2040 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 7373 "raku.tab.c"
    break;

  case 445: /* unary_expr: '!' unary_expr  */
#line 2041 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 7379 "raku.tab.c"
    break;

  case 446: /* unary_expr: CARET unary_expr  */
#line 2042 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 7385 "raku.tab.c"
    break;

  case 447: /* unary_expr: OP_REDUCE unary_expr  */
#line 2044 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 7394 "raku.tab.c"
    break;

  case 448: /* unary_expr: pow_expr  */
#line 2048 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 7400 "raku.tab.c"
    break;

  case 449: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 2051 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,rk_numeric_ctx((yyvsp[-2].node)),rk_numeric_ctx((yyvsp[0].node))); }
#line 7406 "raku.tab.c"
    break;

  case 450: /* pow_expr: postfix_expr  */
#line 2052 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 7412 "raku.tab.c"
    break;

  case 451: /* scalar_list: VAR_SCALAR  */
#line 2055 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7418 "raku.tab.c"
    break;

  case 452: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 2056 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 7424 "raku.tab.c"
    break;

  case 453: /* meth_name: IDENT  */
#line 2059 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7430 "raku.tab.c"
    break;

  case 454: /* meth_name: KW_SORT  */
#line 2060 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7436 "raku.tab.c"
    break;

  case 455: /* meth_name: KW_REVERSE  */
#line 2061 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7442 "raku.tab.c"
    break;

  case 456: /* meth_name: KW_MAP  */
#line 2062 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7448 "raku.tab.c"
    break;

  case 457: /* meth_name: KW_GREP  */
#line 2063 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7454 "raku.tab.c"
    break;

  case 458: /* meth_name: KW_SAY  */
#line 2064 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7460 "raku.tab.c"
    break;

  case 459: /* meth_name: KW_PRINT  */
#line 2065 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7466 "raku.tab.c"
    break;

  case 460: /* meth_name: KW_TAKE  */
#line 2066 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7472 "raku.tab.c"
    break;

  case 461: /* meth_name: KW_RETURN  */
#line 2067 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7478 "raku.tab.c"
    break;

  case 462: /* meth_name: KW_EXISTS  */
#line 2068 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7484 "raku.tab.c"
    break;

  case 463: /* meth_name: KW_DELETE  */
#line 2069 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7490 "raku.tab.c"
    break;

  case 464: /* meth_name: KW_JOIN  */
#line 2070 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7496 "raku.tab.c"
    break;

  case 465: /* meth_name: TESTOP  */
#line 2071 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7502 "raku.tab.c"
    break;

  case 466: /* postfix_expr: call_expr  */
#line 2073 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7508 "raku.tab.c"
    break;

  case 467: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 2076 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7518 "raku.tab.c"
    break;

  case 468: /* call_expr: IDENT '(' arg_list ')'  */
#line 2082 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7527 "raku.tab.c"
    break;

  case 469: /* call_expr: IDENT '(' ')'  */
#line 2086 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7533 "raku.tab.c"
    break;

  case 470: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 2088 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7539 "raku.tab.c"
    break;

  case 471: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2090 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7545 "raku.tab.c"
    break;

  case 472: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2092 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7554 "raku.tab.c"
    break;

  case 473: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2097 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7560 "raku.tab.c"
    break;

  case 474: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2099 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7573 "raku.tab.c"
    break;

  case 475: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2108 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7586 "raku.tab.c"
    break;

  case 476: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2117 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7597 "raku.tab.c"
    break;

  case 477: /* call_expr: IDENT '.' KW_NEW  */
#line 2124 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7605 "raku.tab.c"
    break;

  case 478: /* call_expr: IDENT '.' IDENT  */
#line 2128 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7614 "raku.tab.c"
    break;

  case 479: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2133 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7625 "raku.tab.c"
    break;

  case 480: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2140 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7634 "raku.tab.c"
    break;

  case 481: /* call_expr: IDENT '.' CARET IDENT  */
#line 2145 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7644 "raku.tab.c"
    break;

  case 482: /* call_expr: atom '.' CARET IDENT  */
#line 2151 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7654 "raku.tab.c"
    break;

  case 483: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2157 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7665 "raku.tab.c"
    break;

  case 484: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2164 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7676 "raku.tab.c"
    break;

  case 485: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2171 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7685 "raku.tab.c"
    break;

  case 486: /* call_expr: atom '.' meth_name  */
#line 2176 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7694 "raku.tab.c"
    break;

  case 487: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2181 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7705 "raku.tab.c"
    break;

  case 488: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2188 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7714 "raku.tab.c"
    break;

  case 489: /* call_expr: call_expr '.' meth_name  */
#line 2193 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7723 "raku.tab.c"
    break;

  case 490: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2198 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7734 "raku.tab.c"
    break;

  case 491: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2205 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7745 "raku.tab.c"
    break;

  case 492: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2212 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7756 "raku.tab.c"
    break;

  case 493: /* call_expr: '.' meth_name '(' ')'  */
#line 2219 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7765 "raku.tab.c"
    break;

  case 494: /* call_expr: '.' meth_name  */
#line 2224 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7774 "raku.tab.c"
    break;

  case 495: /* call_expr: KW_DIE expr  */
#line 2229 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7780 "raku.tab.c"
    break;

  case 496: /* call_expr: KW_MAP closure expr  */
#line 2231 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7786 "raku.tab.c"
    break;

  case 497: /* call_expr: KW_MAP closure ',' expr  */
#line 2233 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7792 "raku.tab.c"
    break;

  case 498: /* call_expr: KW_GREP closure expr  */
#line 2235 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7798 "raku.tab.c"
    break;

  case 499: /* call_expr: KW_SORT expr  */
#line 2237 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7804 "raku.tab.c"
    break;

  case 500: /* call_expr: KW_SORT closure expr  */
#line 2239 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7810 "raku.tab.c"
    break;

  case 501: /* call_expr: KW_REVERSE expr  */
#line 2241 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7816 "raku.tab.c"
    break;

  case 502: /* call_expr: atom  */
#line 2242 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7822 "raku.tab.c"
    break;

  case 503: /* arg_list: expr  */
#line 2245 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7828 "raku.tab.c"
    break;

  case 504: /* arg_list: arg_list ',' expr  */
#line 2246 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7834 "raku.tab.c"
    break;

  case 505: /* arg_list: arg_list ','  */
#line 2247 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7840 "raku.tab.c"
    break;

  case 506: /* arg_list: ':' IDENT  */
#line 2248 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7846 "raku.tab.c"
    break;

  case 507: /* arg_list: ':' '!' IDENT  */
#line 2249 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7852 "raku.tab.c"
    break;

  case 508: /* arg_list: ':' IDENT '(' expr ')'  */
#line 2250 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7858 "raku.tab.c"
    break;

  case 509: /* arg_list: ':' IDENT '(' ')'  */
#line 2251 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7864 "raku.tab.c"
    break;

  case 510: /* arg_list: ':' IDENT '<' IDENT '>'  */
#line 2252 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7870 "raku.tab.c"
    break;

  case 511: /* arg_list: arg_list ',' ':' IDENT  */
#line 2253 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_adverb_bool(1)); ct_drop((yyvsp[0].sval)); }
#line 7876 "raku.tab.c"
    break;

  case 512: /* arg_list: arg_list ',' ':' '!' IDENT  */
#line 2254 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_adverb_bool(0)); ct_drop((yyvsp[0].sval)); }
#line 7882 "raku.tab.c"
    break;

  case 513: /* arg_list: arg_list ',' ':' IDENT '(' expr ')'  */
#line 2255 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-6].list),(yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 7888 "raku.tab.c"
    break;

  case 514: /* arg_list: arg_list ',' ':' IDENT '(' ')'  */
#line 2256 "raku.y"
                                          { (yyval.list)=exprlist_append((yyvsp[-5].list),make_call("__rk_undef")); ct_drop((yyvsp[-2].sval)); }
#line 7894 "raku.tab.c"
    break;

  case 515: /* arg_list: arg_list ',' ':' IDENT '<' IDENT '>'  */
#line 2257 "raku.y"
                                           { (yyval.list)=exprlist_append((yyvsp[-6].list),leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-1].sval)); }
#line 7900 "raku.tab.c"
    break;

  case 516: /* paren_group: '(' ')'  */
#line 2260 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7906 "raku.tab.c"
    break;

  case 517: /* paren_group: '(' expr ')'  */
#line 2261 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7912 "raku.tab.c"
    break;

  case 518: /* paren_group: '(' expr ',' ')'  */
#line 2263 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7918 "raku.tab.c"
    break;

  case 519: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2265 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7925 "raku.tab.c"
    break;

  case 520: /* atom: LIT_INT  */
#line 2269 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7931 "raku.tab.c"
    break;

  case 521: /* atom: LIT_BOOL  */
#line 2271 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7939 "raku.tab.c"
    break;

  case 522: /* atom: LIT_FLOAT  */
#line 2274 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7945 "raku.tab.c"
    break;

  case 523: /* atom: LIT_STR  */
#line 2275 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7951 "raku.tab.c"
    break;

  case 524: /* atom: WORDLIST  */
#line 2277 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7964 "raku.tab.c"
    break;

  case 525: /* atom: LIT_INTERP_STR  */
#line 2285 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7970 "raku.tab.c"
    break;

  case 526: /* atom: VAR_SCALAR  */
#line 2286 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7976 "raku.tab.c"
    break;

  case 527: /* atom: OP_INC VAR_SCALAR  */
#line 2287 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7982 "raku.tab.c"
    break;

  case 528: /* atom: OP_DEC VAR_SCALAR  */
#line 2288 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7988 "raku.tab.c"
    break;

  case 529: /* atom: VAR_SCALAR OP_INC  */
#line 2289 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7994 "raku.tab.c"
    break;

  case 530: /* atom: VAR_SCALAR OP_DEC  */
#line 2290 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 8000 "raku.tab.c"
    break;

  case 531: /* atom: VAR_TWIGIL OP_INC  */
#line 2291 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 8006 "raku.tab.c"
    break;

  case 532: /* atom: VAR_TWIGIL OP_DEC  */
#line 2292 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 8012 "raku.tab.c"
    break;

  case 533: /* atom: VAR_ARRAY  */
#line 2293 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8018 "raku.tab.c"
    break;

  case 534: /* atom: VAR_HASH  */
#line 2294 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8024 "raku.tab.c"
    break;

  case 535: /* atom: VAR_CAPTURE  */
#line 2296 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8032 "raku.tab.c"
    break;

  case 536: /* atom: VAR_FH  */
#line 2300 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 8040 "raku.tab.c"
    break;

  case 537: /* atom: VAR_NAMED_CAPTURE  */
#line 2304 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 8047 "raku.tab.c"
    break;

  case 538: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2307 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 8053 "raku.tab.c"
    break;

  case 539: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2309 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 8059 "raku.tab.c"
    break;

  case 540: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2311 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 8065 "raku.tab.c"
    break;

  case 541: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2313 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 8071 "raku.tab.c"
    break;

  case 542: /* atom: ARR_ALL_SLICE  */
#line 2315 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 8077 "raku.tab.c"
    break;

  case 543: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2317 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8083 "raku.tab.c"
    break;

  case 544: /* atom: VAR_HASH '{' expr '}'  */
#line 2319 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8089 "raku.tab.c"
    break;

  case 545: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2321 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 8095 "raku.tab.c"
    break;

  case 546: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2323 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 8101 "raku.tab.c"
    break;

  case 547: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2325 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8107 "raku.tab.c"
    break;

  case 548: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2327 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8113 "raku.tab.c"
    break;

  case 549: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2329 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 8119 "raku.tab.c"
    break;

  case 550: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2331 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 8125 "raku.tab.c"
    break;

  case 551: /* atom: IDENT  */
#line 2332 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 8131 "raku.tab.c"
    break;

  case 552: /* atom: VAR_TWIGIL  */
#line 2334 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8139 "raku.tab.c"
    break;

  case 553: /* atom: VAR_ARRAY_TWIGIL  */
#line 2338 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8147 "raku.tab.c"
    break;

  case 554: /* atom: VAR_HASH_TWIGIL  */
#line 2342 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 8155 "raku.tab.c"
    break;

  case 555: /* atom: '[' ']'  */
#line 2345 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 8161 "raku.tab.c"
    break;

  case 556: /* atom: '[' expr ']'  */
#line 2347 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8167 "raku.tab.c"
    break;

  case 557: /* atom: '[' expr ',' ']'  */
#line 2349 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8173 "raku.tab.c"
    break;

  case 558: /* atom: '[' expr ',' arg_list ']'  */
#line 2351 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8180 "raku.tab.c"
    break;

  case 559: /* atom: DOLLAR_LBRACKET ']'  */
#line 2353 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 8186 "raku.tab.c"
    break;

  case 560: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2355 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 8192 "raku.tab.c"
    break;

  case 561: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2357 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 8198 "raku.tab.c"
    break;

  case 562: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2359 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 8205 "raku.tab.c"
    break;

  case 563: /* atom: paren_group  */
#line 2361 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 8211 "raku.tab.c"
    break;

  case 564: /* atom: block  */
#line 2362 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8217 "raku.tab.c"
    break;

  case 565: /* atom: KW_SUB block  */
#line 2363 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 8223 "raku.tab.c"
    break;

  case 566: /* atom: KW_SUB '(' param_list ')' block  */
#line 2365 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 8230 "raku.tab.c"
    break;

  case 567: /* atom: OP_ARROW scalar_list block  */
#line 2368 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 8237 "raku.tab.c"
    break;


#line 8241 "raku.tab.c"

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

#line 2371 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
