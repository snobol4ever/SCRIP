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

#line 669 "raku.tab.c"

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
#define YYLAST   7065

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  188
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  542
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1325

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
       0,   694,   694,   705,   706,   709,   711,   713,   715,   719,
     721,   723,   725,   727,   731,   735,   737,   741,   745,   747,
     749,   751,   753,   755,   759,   763,   765,   767,   769,   771,
     773,   775,   777,   779,   781,   783,   791,   793,   795,   797,
     799,   801,   803,   806,   809,   811,   814,   817,   819,   823,
     825,   827,   829,   831,   834,   837,   839,   842,   845,   848,
     850,   855,   859,   863,   866,   869,   873,   876,   879,   882,
     885,   888,   890,   892,   894,   896,   899,   901,   903,   905,
     908,   911,   915,   918,   920,   922,   924,   927,   930,   933,
     936,   939,   942,   945,   946,   947,   948,   949,   950,   951,
     953,   955,   959,   964,   968,   970,   972,   974,   976,   977,
     978,   979,   980,   981,   982,   984,   986,   988,   990,   991,
     992,   993,   994,   997,   999,  1001,  1003,  1005,  1007,  1009,
    1011,  1015,  1017,  1019,  1021,  1023,  1025,  1029,  1031,  1035,
    1037,  1039,  1041,  1045,  1047,  1051,  1053,  1055,  1059,  1061,
    1063,  1067,  1070,  1076,  1082,  1090,  1096,  1101,  1106,  1114,
    1125,  1127,  1131,  1132,  1138,  1139,  1142,  1150,  1156,  1162,
    1170,  1176,  1184,  1190,  1196,  1205,  1212,  1221,  1230,  1239,
    1240,  1243,  1246,  1248,  1250,  1252,  1254,  1256,  1258,  1260,
    1263,  1265,  1267,  1269,  1272,  1275,  1281,  1282,  1283,  1286,
    1289,  1292,  1294,  1296,  1298,  1300,  1302,  1304,  1306,  1309,
    1311,  1313,  1315,  1318,  1321,  1327,  1330,  1346,  1361,  1376,
    1377,  1389,  1403,  1404,  1407,  1410,  1413,  1416,  1419,  1422,
    1425,  1428,  1431,  1434,  1437,  1440,  1447,  1453,  1459,  1466,
    1473,  1480,  1484,  1488,  1492,  1496,  1503,  1510,  1517,  1524,
    1534,  1542,  1550,  1559,  1566,  1573,  1583,  1591,  1599,  1608,
    1617,  1632,  1633,  1638,  1643,  1650,  1654,  1658,  1665,  1669,
    1675,  1677,  1679,  1681,  1685,  1686,  1687,  1688,  1689,  1690,
    1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,  1699,  1700,
    1701,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1711,  1712,
    1713,  1715,  1717,  1719,  1721,  1723,  1726,  1728,  1730,  1732,
    1734,  1737,  1740,  1744,  1747,  1750,  1753,  1755,  1758,  1761,
    1764,  1767,  1770,  1772,  1774,  1776,  1778,  1780,  1782,  1784,
    1786,  1790,  1793,  1794,  1795,  1796,  1797,  1798,  1799,  1800,
    1805,  1810,  1812,  1815,  1817,  1820,  1821,  1822,  1823,  1824,
    1827,  1828,  1831,  1832,  1833,  1834,  1835,  1836,  1837,  1838,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,
    1849,  1850,  1851,  1852,  1853,  1854,  1860,  1866,  1872,  1878,
    1881,  1882,  1885,  1886,  1887,  1888,  1889,  1890,  1891,  1892,
    1893,  1896,  1898,  1901,  1902,  1903,  1904,  1905,  1906,  1907,
    1910,  1911,  1912,  1913,  1916,  1917,  1918,  1921,  1922,  1923,
    1924,  1925,  1926,  1927,  1928,  1931,  1932,  1933,  1934,  1935,
    1936,  1937,  1938,  1939,  1941,  1943,  1945,  1947,  1950,  1951,
    1952,  1953,  1954,  1959,  1962,  1963,  1966,  1967,  1970,  1971,
    1972,  1973,  1974,  1975,  1976,  1977,  1978,  1979,  1980,  1981,
    1982,  1984,  1986,  1992,  1997,  1998,  2000,  2002,  2007,  2009,
    2018,  2027,  2034,  2038,  2043,  2050,  2055,  2061,  2067,  2074,
    2081,  2086,  2091,  2098,  2103,  2108,  2115,  2122,  2129,  2134,
    2139,  2141,  2143,  2145,  2147,  2149,  2151,  2153,  2156,  2157,
    2158,  2161,  2162,  2163,  2165,  2170,  2171,  2175,  2176,  2177,
    2186,  2187,  2188,  2189,  2190,  2191,  2192,  2193,  2194,  2195,
    2196,  2200,  2204,  2207,  2209,  2211,  2213,  2215,  2217,  2219,
    2221,  2223,  2225,  2227,  2229,  2231,  2233,  2234,  2238,  2242,
    2246,  2247,  2249,  2251,  2254,  2255,  2257,  2259,  2262,  2263,
    2264,  2265,  2268
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

#define YYPACT_NINF (-1015)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-540)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1015,    91,  2925, -1015, -1015, -1015, -1015, -1015, -1015,   461,
     124,   152,   218,    36, -1015, -1015,  6789,  3012, -1015, -1015,
   -1015,    50,  3147,    43,  5370,  5505,  5584,  5719,  5798,     4,
      13,  5798,  3226,  3361,   121,    70,  5798,  5798,   215,   293,
    5933,  6012,    78,   225,    52,   263,   105,   105,  6147,  5798,
      78,   133,  5798,  3440,   364,   364,   316,  6789, -1015, -1015,
     364,   364,    78,   327,   384,   403,  6789,  6789,  6789,  1151,
   -1015,  3575,  3654,   367, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,  6889,   638,
     258,   333,   365,   728,   736,   781,   388,   727,   259,   -42,
     451,   766, -1015, -1015,   357,   312,   376,   387,   538, -1015,
   -1015,  5798,  5798,  5798,  5798,  5798,  5798,  3789,  6226,  3868,
     573,  5798, -1015, -1015,  5798,   399,   416,  1303,   165,   429,
     285,   347,    42,   326, -1015, -1015,   442, -1015,   473,   534,
     335, -1015,    85,  4003, -1015,  4082, -1015,   187,   247,   280,
     424,   846,   647,   141,   327,  3575,   611,  3575,   118,  3575,
      78,  3575,    78,   -17,   378,   329,   472, -1015, -1015, -1015,
     537, -1015,   268, -1015,   277,   503,   520,   642,   552,   559,
     373,   381,  3575,    78,  3575,    78,   564,  4218, -1015,  5798,
    5798, -1015,  5798,  5798, -1015,  5798,  4297,  5798,  4432,  5798,
   -1015, -1015,   698,    61, -1015, -1015, -1015,   607, -1015, -1015,
     639,   648,    81, -1015,   701,   743,   612, -1015,   453, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015,   757, -1015,   780,
   -1015,   660,   726,  1888,  5798,  5798,  5798,  5798,  5798,  5798,
    5798,  5798, -1015,  5798,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,   741,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,  6789,
    6789,  6789,  6789,  6789,  6789,  1151,  5798,  1042,   693,   768,
     799,   825,   830,   838,   872, -1015,   880,  3575,   549,   855,
     716,   792,   869,   886, -1015,   879,  1055,   901,   172, -1015,
    1072,   882,   892,  3868,  1080,  5798,  1151,  1042,  5798,  5798,
    5798,  5798,  5798,  5798,  5798,  4511, -1015, -1015,   931,   953,
     780,   894, -1015,  5798,  5798, -1015,  6361, -1015,  6440, -1015,
     440,   488,   492,   801,   947,   951,   898,   902,  5798,  5798,
    5798,  5798,  5798,  5798,  5798, -1015,  5798,   904,  5798,  5798,
   -1015,  5798,   912,  1073,   927, -1015,   327,  5798, -1015,  6789,
    6789,    25, -1015, -1015,    -4, -1015,   262, -1015, -1015, -1015,
     933, -1015,  5798,  5798,  5798, -1015,  5798,  5798, -1015,  5798,
    5798,   976,  5798, -1015,  1108,  5798,  1131,  5798,   937,  1147,
     940, -1015,  5798,  5798,  1174,  1002,  1013,  1025,  1057,  1069,
    1038,  5798, -1015, -1015, -1015,    78,  5798,    78,    40, -1015,
      28, -1015,    51,  1058,  1070, -1015, -1015, -1015,  1238, -1015,
    4646, -1015,  4725,  4861, -1015, -1015,   434,   449,   244,  5370,
    5505,  5798,  2527,    29,    31, -1015,   336,  1074,  1075,  1076,
    1077,  1078,  1079,  1082,  1083,  1084,  1085, -1015,   365,  1145,
     365,   365,   365,   728,  1093,  1093,  1093,  1093,  1093,  1093,
    1093,  1093,  1093,  1093,  1093,  1093,  1093,  1093,  1093,  1093,
    1093,  1093,  1093,  1093,  1093,  1093,  1093, -1015, -1015, -1015,
   -1015,   781,   727,   727,   727,   727,   727,   727,   727,   727,
     727,   456,   456,   456,   456,   456,   456,   -42,   -42,   -42,
     451,   451,   766,   766,   766,   766,   766,   766,   766, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,   -72,  1094,  1244,   134, -1015,  4940, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015,   944, -1015,  5798,  5798,  5798,
    5798,  1109,   342,   354, -1015,  5076, -1015,  1488,  5798,  1095,
   -1015,    -6, -1015,  1703,   805,  1110,  1091,  -159,   -43, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015,   853, -1015, -1015,
    1100, -1015,  1101,  3575,   644,  1195,   224,  5155,   664,  1103,
    5798, -1015,  6575, -1015,  5798, -1015,    56, -1015,  5798,  5798,
    1118,  4725,  1105,  1106,  1107,  1113,  1114,  1115,  1116,   720,
    4725,  1117,  1119,   732,    78,   -10,  6654, -1015,    78,   453,
     135,   -40,    -1,   -16,   946,  2072,  1272,  5798,    54,  1277,
    1286,    78,   691,   839,  1120,  1121,  1122,  1123,  1124,  1125,
   -1015,  1129,    64,  1137,  1126,  1158,  1141,    78,    78,    78,
    1150,  1152,  1165,  5798, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015,    78,  1146,  5798,    78, -1015,  1318,  1319, -1015,    -3,
    1153,   949,    75,   472,   179,  2256, -1015, -1015,   955, -1015,
     957, -1015,   864,  3868,  5798,  5798,   375,   144,   196, -1015,
     264,  5798,  5798, -1015,  5798,  5798, -1015,  5798,  5798,  5798,
    5798,  5798,  5798,  5798,  5798, -1015,  1151,  1042, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015,  6789,  5798,  5291,  5798,
   -1015, -1015,  5798,  1567,  5798,  1156,   959,  4725,   843,  1155,
    1157,   868,  5798, -1015, -1015,  5798, -1015, -1015,  5798, -1015,
     961, -1015,   967,   972, -1015,  5798,  1254,  1327,   977, -1015,
    1043,  1045, -1015, -1015, -1015,   986, -1015,  5798,  5798,  5798,
     988, -1015,   203, -1015,  1166,  3575,   844,  1167,  1153,   990,
    1168,  1169,  5798,   992, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015,   998, -1015, -1015, -1015,  1154, -1015, -1015,  3575,
      78, -1015, -1015,  1333, -1015,  1339,  1341,  1338,    17, -1015,
     -16,  5370,  5505,  2662, -1015,   454, -1015, -1015,  1340,  5798,
   -1015, -1015, -1015,    65, -1015,   323, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015,  5798,    78, -1015, -1015,
   -1015,  1175,  1177,  1335, -1015, -1015, -1015, -1015,  5798,  1183,
   -1015, -1015,    78,  1179, -1015, -1015,   269,   889,   952,  1304,
   -1015, -1015,  1153,  1153,  1007,  1018,  1351,  1353,  1354, -1015,
   -1015, -1015, -1015, -1015,   874,  1184,     0,  5798,  5798,  5798,
    5798,  5798,  5798, -1015, -1015, -1015, -1015,   315,   414,   436,
     494,   498,   502,   521,   525,   529,   531,   541,   567,   183,
     405, -1015,  1192, -1015,  1021,  1129,  1193, -1015,  1028,  1030,
    1129, -1015,  1196,  1032, -1015, -1015, -1015, -1015,  1198,  1199,
    1202, -1015, -1015, -1015,  1203,  5798,  1200, -1015,  4725,   850,
   -1015, -1015,  1204,  1290,  1296, -1015,  1034, -1015,  5798, -1015,
   -1015,  1153, -1015, -1015,   854,  1206,  1207,   -10, -1015,  1036,
    1190,    78,    78,    78, -1015,  1372, -1015,    17, -1015,   511,
     229, -1015,   265,  5798,  5798,  5798,  5798,  5798,  5798,  5798,
    5798, -1015, -1015, -1015,  1373,  5798,    68,  1378,  1379,    78,
    1205, -1015, -1015,    78,  1214,  5798, -1015, -1015, -1015,   820,
     821,    60,   103,   968,  1215,  1216,   984, -1015, -1015,  1153,
    1153,  1388,  1389,  1390,  1234,   397, -1015,   568,   580,   582,
     583,   588,   592, -1015, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015,  5798,  5798, -1015, -1015, -1015,
   -1015, -1015, -1015,  1221, -1015, -1015, -1015, -1015, -1015,  5798,
    1041, -1015, -1015,  5798,  5798,  4725,   863, -1015, -1015,  5798,
   -1015, -1015, -1015, -1015,    78,    78, -1015, -1015, -1015, -1015,
   -1015, -1015,  5798,  5798,  5798, -1015, -1015, -1015,   593,   596,
     598,   601,   602,   606,   616,   622, -1015, -1015,  1386,  5798,
   -1015, -1015, -1015, -1015, -1015,  5798,  1222, -1015,    90,  1323,
   -1015,   127, -1015,  1391,  1392,  1225,  5798, -1015,  1393,  1394,
    1226,  5798, -1015,   130,   136,  1231,  1232, -1015, -1015,  1235,
     822, -1015, -1015, -1015, -1015, -1015,  5798,  5798, -1015, -1015,
   -1015, -1015, -1015, -1015,   625,   626, -1015,  1236,  1233, -1015,
   -1015,  1044, -1015,   865,  1201, -1015,   627,   628,   630, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015, -1015,  1237,
      78,  1239,  1051,  1240,  2440,  1239,  1053,  1241,  1242, -1015,
    1243,   493,   513, -1015,  1246,  1403,  1404,  1274,  5798, -1015,
    1439,  1441,  1278,  5798, -1015, -1015, -1015,   162,   192, -1015,
     643,   681, -1015, -1015, -1015, -1015,  1279, -1015,    78, -1015,
   -1015, -1015, -1015,  5798, -1015, -1015,  1239, -1015,   307,  5370,
    5505,  2741, -1015,   519, -1015,  1239, -1015, -1015, -1015,  5798,
   -1015,  5798, -1015, -1015,  1281,  1282, -1015,  1283,  1284,  1285,
   -1015,  1287,  1239,  1059,  1239,  1061, -1015, -1015, -1015, -1015,
    1289, -1015,  5798,   566,   250, -1015,   274,  5798,  5798,  5798,
    5798,  5798,  5798,  5798,  5798, -1015, -1015,  1288,  1292, -1015,
   -1015, -1015, -1015, -1015, -1015, -1015,  1239, -1015,  1239,    78,
     682,  5798,  5798,  5798, -1015, -1015, -1015,   683,   744,   747,
     748,   752,   753,   769,   770, -1015, -1015, -1015, -1015, -1015,
   -1015,   773,   778,   779, -1015, -1015, -1015, -1015, -1015, -1015,
   -1015, -1015, -1015, -1015, -1015
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     2,     1,   495,   496,   497,   498,   500,   501,
     508,   509,   527,   526,   528,   529,     0,     0,   510,   511,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   517,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     0,     3,     4,    95,    96,   108,   109,   110,
     111,    97,    98,   118,   119,   120,   122,   121,   105,     0,
     342,   344,   349,   351,   379,   381,   390,   392,   399,   403,
     406,   414,   427,   433,   435,   451,   538,   487,     0,   504,
     505,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   506,   507,     0,     0,     0,     0,   501,   508,
     509,   527,   526,     0,   539,   431,   451,   538,   487,   501,
     508,   534,     0,     0,    36,     0,   488,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,   540,   340,   339,
       0,    50,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   145,     0,   148,     0,
       0,   112,     0,     0,   113,     0,     0,     0,     3,     0,
     484,   486,    99,     3,   104,   480,    52,     0,   215,   219,
       0,     0,     0,   432,     0,     0,   106,   436,     0,   502,
     503,   430,   429,   428,   438,   450,   443,   444,   445,   446,
     449,   447,   448,   441,   442,   439,   440,   479,   491,     0,
     530,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   334,
     335,   336,   337,   338,   332,   458,     0,     0,   333,     0,
       0,     0,     0,     0,    40,   463,     0,   462,   526,   454,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,    29,     0,   491,
       0,     0,    39,   490,     0,     6,     0,    10,     0,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,     0,     0,     0,     0,
      44,     0,     0,   126,     0,   138,     0,     0,   157,     0,
       0,     0,     3,   168,   274,   279,     0,   292,   294,   296,
       0,    47,     0,     0,     0,    49,     0,     0,    55,     0,
       0,     0,     0,   162,     0,     0,     0,     0,     0,   141,
       0,   144,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   481,   483,   485,     0,     0,     0,     0,    51,
       0,   222,     0,     0,     0,   261,     3,   107,     0,   542,
       0,   492,     0,     0,   531,   299,   508,   509,   527,     0,
       0,     0,     0,     0,     0,   298,     0,   451,   487,     0,
       0,     0,     0,     0,     0,     0,     0,   341,   345,     0,
     348,   347,   346,   350,   352,   361,   364,   365,   360,   359,
     358,   357,   356,   355,   354,   353,   362,   363,   366,   370,
     371,   372,   373,   374,   367,   368,   369,   375,   376,   377,
     378,   380,   388,   387,   386,   385,   384,   383,   382,   389,
     391,   397,   396,   395,   394,   393,   398,   402,   401,   400,
     404,   405,   412,   411,   410,   409,   408,   407,   413,   423,
     425,   426,   424,   420,   419,   418,   417,   416,   415,   421,
     422,   434,   474,     0,     0,   471,    62,     0,    88,    89,
      90,    91,    92,    59,   457,     0,    15,     0,     0,     0,
       0,   513,   518,   519,    65,     0,   466,     0,     0,   267,
     455,     0,   453,   490,     0,     0,     0,   474,   471,   334,
     335,   336,   337,   338,   332,   333,   536,     0,    30,    38,
       0,   489,     0,     0,     0,   498,   526,     0,     0,     0,
       0,    26,     0,    27,     0,    28,     0,   173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   492,     0,     0,   129,   492,     0,
       0,   393,   398,     0,     0,     0,     0,     0,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,   452,     0,     0,     0,     0,     0,   492,     0,   492,
       0,     0,     0,     0,   114,   115,   116,   117,   331,   482,
     100,     0,     0,     0,     0,   101,     0,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   437,   478,     0,   493,
       0,   532,     0,     0,     0,     0,     0,     0,     0,   324,
       0,     0,     0,   325,     0,     0,   326,     0,     0,     0,
       0,     0,     0,     0,     0,   300,     0,     0,    71,    73,
      75,    78,    76,    77,    72,    74,     0,     0,     0,     0,
       9,   467,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   523,   525,     0,   522,   524,     0,   465,
       0,   461,     0,     0,   265,     0,     0,     0,     0,   513,
     518,   519,   537,    37,     5,     0,    12,     0,     0,     0,
       0,    19,     0,    18,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    79,    82,    81,    85,    83,    84,
      80,    42,     0,    86,    87,    45,   123,   128,   127,     0,
       0,   137,   154,     0,   156,     0,     0,     0,     0,   167,
       0,     0,     0,     0,   179,     0,   275,   288,     0,     0,
     282,   283,   541,   284,   280,     0,   293,   295,   297,    48,
      53,    54,    57,    58,    32,    31,     0,     0,   158,   520,
     521,     0,     0,   139,   142,   143,   146,   147,     0,     0,
     160,   103,     0,     0,   220,   221,     0,     0,     0,     0,
     217,   178,     0,     0,     0,     0,     0,     0,     0,   260,
     218,   477,   494,   533,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   309,   316,   322,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   474,
     471,   343,     0,   473,     0,   476,     0,   470,     0,     0,
     475,    61,     0,     0,    16,   516,   515,   514,     0,     0,
       0,   464,   459,   460,     0,     0,     0,   456,     0,     0,
     271,   270,     0,     0,     0,    21,     0,    22,     0,    25,
     172,     0,    34,    33,     0,   494,   494,     0,   130,     0,
     131,     0,     0,     0,   164,     0,   170,     0,   166,     0,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   277,   290,     0,     0,   285,     0,     0,     0,
       0,    69,    70,     0,     0,     0,   161,   102,   216,     0,
       0,     0,     0,     0,     0,     0,     0,   177,   175,     0,
       0,     0,     0,     0,   513,   519,   319,     0,     0,     0,
       0,     0,     0,   327,   328,   329,   330,   301,   303,   305,
     308,   306,   307,   302,   304,     0,     0,    63,   472,    64,
     469,   468,    60,   494,    66,    67,    68,   266,   268,     0,
       0,    13,    20,     0,     0,     0,     0,   171,     7,     0,
      43,    46,   125,   124,   492,     0,   135,   155,   152,   153,
     165,   169,     0,     0,     0,   183,   184,   181,     0,     0,
       0,     0,     0,     0,     0,     0,   276,   289,     0,     0,
     286,   287,   163,   159,   140,     0,     0,   151,     0,     3,
     254,     0,   251,     0,     0,     0,     0,   228,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   224,   225,     0,
       0,   174,   176,   262,   263,   264,     0,     0,   310,   312,
     315,   313,   314,   311,     0,     0,    17,     0,   494,   273,
     272,     0,    23,     0,   132,   133,     0,     0,     0,   185,
     187,   189,   192,   190,   191,   186,   188,   278,   291,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,   242,     0,     0,     0,     0,     0,   230,
       0,     0,     0,     0,   229,   226,   227,     0,     0,   257,
       0,     0,   317,   318,   269,    14,   494,     8,     0,   136,
     193,   195,   194,     0,   150,   253,     0,   197,   527,     0,
       0,     0,   196,     0,   250,     0,   239,   240,   232,     0,
     235,     0,   238,   231,     0,     0,   243,     0,     0,     0,
     244,     0,     0,     0,     0,     0,   320,   321,    24,   134,
       0,   252,     0,     0,     0,   201,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   199,   249,     0,     0,   247,
     248,   234,   245,   246,   233,   259,     0,   256,     0,     0,
       0,     0,     0,     0,   202,   203,   200,     0,     0,     0,
       0,     0,     0,     0,     0,   236,   237,   258,   255,   149,
     198,     0,     0,     0,   204,   206,   208,   211,   209,   210,
     205,   207,   212,   214,   213
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1015, -1015,     1, -1015,  -650,  -822, -1015, -1015, -1015, -1015,
   -1015,   242,  1443, -1015, -1015, -1015,   634, -1015,  -369, -1014,
     -28, -1015, -1015, -1015, -1015,   771, -1015, -1015,  -587,   851,
    -409,   446,  1197,    -2,  -252, -1015,   758,  1218,  1170,  1212,
   -1015,   563,   -11,   394,   943,   666,   213, -1015,  -147,  -283,
   -1015,     9,   393,    10,    11
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,   243,    74,    75,   657,    76,    77,    78,    79,
      80,  1116,    81,    82,   458,   682,   838,    83,   413,  1120,
     209,    84,    85,    86,   460,   709,    87,   714,   351,   628,
     420,   134,   196,   146,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   218,   237,
     104,   136,   147,   137,   138
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,     2,   664,   499,   978,   827,  1122,   386,   837,   786,
     782,   105,   106,   107,   637,   142,   788,   164,    26,   165,
     666,   758,   156,   158,   160,   162,   163,   210,   759,   170,
     172,   174,   214,   215,   178,   179,   414,   415,   183,   185,
     416,   985,   572,   706,   575,    28,   200,   201,   125,   835,
     205,   207,   707,   711,   148,   149,   150,   731,   151,   734,
     887,   888,   414,   415,   889,   143,   416,   414,   415,   239,
     241,   416,   406,   607,   608,  1123,   152,   732,   848,   735,
     189,   153,   703,   704,  1124,   177,   414,   415,   836,  1004,
     416,     3,  1108,   757,   300,   301,   463,   417,   418,   419,
     190,   414,   415,   456,   457,   416,   866,   867,   758,   329,
     330,   331,   332,   333,   334,   759,   338,   340,  1128,   342,
     303,   304,   343,   417,   418,   419,  1209,  1129,   417,   418,
     419,   302,   175,   300,   301,  1125,   176,   763,   414,   415,
     242,   368,   416,   370,   764,  1195,   398,   417,   418,   419,
     464,  1200,   384,   387,  1196,   397,   385,   402,  1086,   404,
    1201,   667,   417,   418,   419,   407,   399,  1225,    73,   412,
     302,  1234,   398,   414,   415,    73,   938,   416,  1130,   594,
     438,   787,   440,   890,   166,   445,  1036,   446,   447,    73,
     448,   449,   399,   450,   452,   453,   450,   454,    73,   417,
     418,   419,   412,   414,   415,  1197,   663,   416,   191,   963,
     194,  1202,  1261,   708,   126,   733,   127,   736,   964,   849,
     126,  1276,   127,   154,   833,  1126,   705,   809,   180,   135,
    1005,   191,   710,  1109,   417,   418,   419,   808,  1285,  1127,
    1287,   486,   489,   490,   491,   492,   493,   494,   495,   496,
     868,   497,   487,   106,   488,   530,   893,   398,   598,   659,
     896,   897,   898,    73,   417,   418,   419,   365,  1131,   366,
     213,  1181,  1307,   668,  1308,   109,   110,   399,   398,   221,
     222,   223,  1132,   541,   542,   543,   544,   545,   546,   118,
     195,   192,   423,   423,   839,  1198,   423,   400,   399,   762,
     401,  1203,   423,   894,   895,   426,   181,   119,  1185,  1199,
     799,   193,   424,   424,   763,  1204,   424,   373,   203,   120,
      73,   764,   424,   400,   573,   427,   401,  1082,   122,   123,
     914,   211,   887,   888,  1006,   585,   889,   121,   217,   669,
     670,   891,  1219,  1252,   253,   117,   298,   299,  1055,   212,
     126,   604,   127,   606,   122,   123,   609,   610,   611,   612,
     613,   614,   615,   758,   737,   899,   372,   738,   739,   373,
     759,   621,   622,  1254,   624,   421,   629,   740,   422,   208,
     741,   742,   915,   124,   743,   744,   642,   643,   644,   645,
     646,   647,   648,   300,   301,   219,   651,   652,   661,   662,
    1007,  1008,   126,   907,   127,   187,   389,   908,   400,   725,
      73,   401,   374,   665,   220,  1096,   909,   122,   123,   910,
     911,   674,   675,   912,   676,   677,   375,   678,   679,   400,
     302,   345,   401,   684,   346,   686,  1295,   254,   255,   970,
     690,   691,   194,   425,   425,   376,   242,   425,    88,   699,
     916,  1097,   354,   425,   701,  1018,   428,   122,   123,   377,
    1296,   773,   774,   929,   930,   409,   410,   715,   259,   986,
     355,   988,  1262,   776,   777,   167,   169,   726,   727,   728,
     730,   347,   993,   414,   415,   994,   995,   416,   186,   188,
     325,   256,   257,   258,   694,   996,   202,   204,   997,   998,
     364,  1043,   999,  1000,   931,   293,   166,   775,   216,   411,
     336,    73,   300,   301,   412,   252,   776,   777,   353,   778,
     352,   324,   745,  1027,  1028,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   371,  1092,
     434,   326,   389,  1093,   417,   418,   419,  1267,   436,   302,
    1268,  1269,   391,   328,   395,   392,   393,   396,   435,  1094,
    1270,   913,  1147,  1271,  1272,   327,   437,  1273,  1274,   108,
    1056,   109,   110,   111,   112,   113,   114,   115,   344,   167,
     305,   306,   307,   308,   309,   763,   769,   770,   341,   378,
     300,   301,   764,   695,  1291,   442,   784,   389,  1292,   118,
    1044,   621,  1077,   379,  1020,   630,   403,   391,   405,   408,
     392,   393,   353,   443,  1293,   696,   120,   723,  1091,   631,
     356,   795,  1045,   310,   311,   239,   116,   302,   804,   439,
     806,   441,   807,   252,   724,   468,   810,   811,    73,   388,
    1001,   117,   389,   390,   109,   110,   358,   359,   360,   361,
     362,   357,   391,   632,   830,   392,   393,   634,  1239,   394,
    1141,  1142,   383,   845,   469,   847,   244,   633,   429,   245,
     246,   635,  1240,   697,   105,   106,   107,   748,  1241,   247,
    1046,   749,   248,   249,  1047,   430,   250,   251,  1048,    88,
     395,   879,  1242,   396,   547,   548,   549,  1095,   252,   363,
     750,   882,   853,   854,   751,  1275,   855,  1049,   752,  1182,
     753,  1050,  1186,    89,   117,  1051,   421,  1052,   431,   422,
     754,   904,   905,   906,   105,   106,   107,  1053,   586,   917,
     918,   587,   919,   920,   432,   921,   922,   923,   924,   925,
     926,   927,   928,  1140,   433,   395,   755,   814,   396,   527,
     528,   529,  1294,  1054,  1148,   932,   208,   455,   617,   816,
     936,   817,   818,   856,   857,   858,  1149,   819,  1150,  1151,
     948,   820,   748,   949,  1152,   749,   950,   750,  1153,  1169,
     751,   752,  1170,   954,  1171,   753,   459,  1172,  1173,   649,
     395,   467,  1174,   396,   653,   754,   960,   961,  1253,  1255,
     660,   755,  1175,   966,  1057,  1059,   814,   816,  1176,   820,
     974,  1212,  1213,  1220,  1221,   673,  1222,   252,   260,   261,
     262,   263,  1064,   796,   461,   681,   797,   979,   462,  1256,
     274,   275,   276,   277,   278,   279,   280,   281,   282,   989,
     990,   992,   473,   801,   474,   283,   802,  1003,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   380,   381,   382,
    1066,   594,   748,   718,  1009,   720,   722,  1257,  1310,  1314,
     294,   295,   576,   577,   296,   297,  1014,   264,   265,   266,
     267,   268,   269,   270,   312,   271,   465,   313,   314,   315,
     316,   317,   318,   319,   320,   272,   273,   284,   590,   821,
     591,   700,   373,   702,   224,  1037,  1038,  1039,  1040,  1041,
    1042,   825,   475,   225,   373,   226,   227,   285,   286,   287,
     288,   289,   290,   749,   228,   229,   750,   751,   466,   230,
    1315,   752,   753,  1316,  1317,   231,   232,   470,  1318,  1319,
     321,   322,   323,   233,   234,   235,   236,   578,   754,   755,
     291,   292,   814,  1068,  1019,  1320,  1321,   816,   820,  1322,
     592,   471,   472,  1021,  1323,  1324,  1022,  1023,  1024,  1025,
     766,   552,   553,   554,   555,   556,   557,   558,   579,  1133,
     768,   636,  1134,   771,  1135,  1136,   412,   590,   780,   789,
     783,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,   224,
    1118,  1121,  1208,  1107,   580,  1119,  1119,  1119,   225,   581,
     226,   227,   498,  1117,   500,   501,   502,   582,   859,   228,
     229,   373,   944,   967,   230,   373,   968,   588,   589,  1071,
     231,   232,   373,  1078,   813,   373,  1079,   792,   233,   234,
     235,   236,  1162,   822,  1217,   373,   373,   373,   903,  1139,
     373,   583,   947,  1154,  1155,   593,   590,   224,  1034,   595,
     574,   584,   373,   600,   601,   594,   225,  1157,   226,   227,
     596,  1159,  1160,   602,   603,   620,   373,   228,   229,   640,
     468,   597,   230,   471,   641,   471,   650,   599,   231,   232,
    1166,  1167,  1168,   654,   472,   605,   233,   234,   235,   236,
     826,   828,   655,   656,   831,   832,   834,  1178,   658,   472,
     618,    88,   638,  1179,   671,   672,   639,   852,   687,   472,
    1184,   689,   472,   683,  1190,   471,   767,   840,   672,  1194,
     892,   672,   619,   873,   874,   875,   901,   373,   902,   373,
     942,   373,   951,   373,  1210,  1211,   685,   880,   952,   601,
     883,   934,   935,   953,   373,   680,   939,   940,   957,   601,
     943,    88,   773,   774,   776,   777,   224,   471,   958,   962,
     802,   971,   672,   975,   373,   225,   688,   226,   227,   976,
     373,   693,  1233,   977,   656,   692,   228,   229,  1029,   672,
     959,   230,   694,   105,   106,   107,  1247,   231,   232,  1030,
     672,  1251,  1058,   373,   695,   233,   234,   235,   236,  1060,
     601,  1061,   373,  1063,   373,   471,  1075,  1084,   472,  1085,
     656,  1117,  1158,   373,   698,  1216,   373,  1263,  1264,  1266,
    1218,   656,  1226,   672,  1235,   672,   696,  1277,   712,  1278,
    1286,   672,  1288,   672,   197,   199,   550,   551,   697,   716,
     713,   756,   746,   747,   284,   748,   749,   750,   751,   761,
    1290,   752,   753,   754,   755,  1297,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,   760,   772,   785,   980,   791,   790,   793,
     794,   798,   803,   812,   814,   815,   816,   846,   850,  1311,
    1312,  1313,   817,   818,   819,   820,   823,   851,   824,   860,
     861,   862,   863,   864,   865,   869,     4,     5,     6,     7,
       8,   373,   870,  1010,   139,   140,   130,   131,   348,    14,
      15,    16,    17,    18,    19,    20,   871,   872,  1016,   876,
     878,   877,   881,   884,   885,   941,   133,    30,   412,   945,
     955,   946,   956,    36,   981,   965,   969,   972,   973,    38,
     982,  1070,   983,   984,  1011,  1002,  1012,    46,    47,    48,
      49,  1076,  1015,    52,  1013,  1017,  1031,  1026,  1032,  1033,
    1035,  1057,  1059,    57,    58,  1062,  1073,  1064,  1065,    59,
    1069,  1066,  1074,  1072,  1067,  1080,  1081,  1090,  1106,  1110,
    1111,  1113,    63,  1115,  1137,  1138,  1143,  1144,  1145,  1146,
    1156,  1177,  1183,  1180,  1189,  1193,  1187,  1188,  1191,  1192,
    1205,  1206,  1215,    64,    65,  1207,  1223,  1214,  1244,  1245,
    1236,  1237,  1238,  1083,  1119,  1243,  1227,  1087,  1088,  1089,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,  1246,  1248,  1112,  1249,  1250,  1258,  1114,
    1279,  1280,  1281,  1282,  1283,  1260,  1284,  1305,  1161,    66,
    1289,  1306,  1163,   168,   987,    67,    68,   503,   800,   886,
       0,    69,     0,    71,   349,     0,    72,     0,    73,     0,
     350,     4,     5,     6,     7,     8,   531,     0,     0,   139,
     140,   130,   131,   348,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   133,    30,     0,     0,     0,     0,     0,    36,     0,
    1164,  1165,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   139,   140,
     130,   131,   348,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     133,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,  1224,    52,     0,     0,
      88,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,  1259,     0,    69,     0,    71,   781,
       0,    72,     0,    73,     0,   350,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   139,   140,   130,   131,   348,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,    66,     0,  1309,   133,    30,     0,    67,
      68,     0,     0,    36,     0,    69,     0,    71,   937,    38,
      72,     0,    73,     0,   350,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,     0,    71,     0,     0,    72,     0,    73,     0,
     350,     4,     5,     6,     7,     8,     0,     0,     0,     9,
     476,   477,   478,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,   479,   480,    26,     0,     0,    27,
      28,    29,    30,   481,   482,    33,    34,    35,    36,    37,
       0,     0,     0,     0,    38,    39,    40,    41,    42,    43,
     483,   484,    46,    47,    48,    49,    50,    51,    52,    53,
      54,     0,     0,     0,    55,    56,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,    60,     0,
       0,     0,    61,    62,     0,     0,     0,    63,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,     0,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,    70,    71,     0,
       0,    72,     0,    73,   485,     4,     5,     6,     7,     8,
       0,     0,     0,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,   841,   842,
      26,     0,     0,    27,    28,    29,    30,    31,   843,    33,
      34,    35,    36,    37,     0,     0,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,    55,    56,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,    60,     0,     0,     0,    61,    62,     0,     0,
       0,    63,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
       0,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,    70,    71,     0,     0,    72,     0,    73,   844,     4,
       5,     6,     7,     8,     0,     0,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,     0,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,     0,     0,
       0,     0,    38,    39,    40,    41,    42,    43,    44,    45,
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
       0,    73,   900,     4,     5,     6,     7,     8,     0,     0,
       0,     9,    10,    11,  1228,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,  1229,  1230,    26,     0,
       0,    27,    28,    29,    30,    31,  1231,    33,    34,    35,
      36,    37,     0,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,     0,     0,     0,    55,    56,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
      60,     0,     0,     0,    61,    62,     0,     0,     0,    63,
       4,     5,     6,     7,     8,     0,     0,     0,   139,   140,
     130,   131,   132,    14,    15,    16,    17,    18,    19,    20,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
     133,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,    66,     0,     0,     0,
       0,     0,    67,    68,     0,     0,    63,     0,    69,    70,
      71,     0,     0,    72,     0,    73,  1232,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   139,   140,   130,   131,   132,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   133,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,   171,    71,    38,     0,
      72,     0,    73,   729,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   133,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,   171,    71,     0,     0,    72,     0,    73,   991,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,     0,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
     171,    71,     0,     0,    72,     0,    73,  1265,     4,     5,
       6,     7,     8,     0,     0,     0,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,     0,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,     0,     0,     0,
       0,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,     0,     0,
       0,    55,    56,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,    60,     0,     0,     0,    61,
      62,     0,     0,     0,    63,     4,     5,     6,     7,     8,
       0,     0,     0,   139,   140,   130,   131,   132,    14,    15,
      16,    17,    18,    19,    20,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,   133,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,    66,     0,     0,     0,     0,     0,    67,    68,     0,
       0,    63,     0,    69,    70,    71,     0,     0,    72,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   139,   140,
     130,   131,   132,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
     133,    30,     0,     0,    67,    68,     0,    36,     0,     0,
      69,     0,    71,    38,     0,    72,   141,    73,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    63,   139,   140,   130,
     131,   132,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,   133,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    63,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,   144,   145,     0,     0,
      72,     0,    73,     0,     0,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,   133,    30,     0,     0,    67,    68,
       0,    36,     0,     0,    69,   171,    71,    38,     0,    72,
       0,    73,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      63,   139,   140,   130,   131,   132,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   133,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
     173,    71,     0,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   139,   140,   130,   131,
     132,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   133,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,   206,
      71,    38,     0,    72,     0,    73,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   139,   140,   130,   131,   132,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   133,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,     0,    71,   238,     0,    72,     0,
      73,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     139,   140,   130,   131,   132,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   133,    30,     0,     0,    67,    68,     0,    36,
       0,     0,    69,     0,    71,    38,     0,    72,   240,    73,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    63,   139,
     140,   130,   131,   132,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,   133,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    63,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,    71,
     335,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   139,   140,   130,   131,   132,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   133,    30,     0,     0,
      67,    68,   339,    36,     0,     0,    69,     0,    71,    38,
       0,    72,     0,    73,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    63,   139,   140,   130,   131,   132,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,   133,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    63,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,   367,    71,     0,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,     0,   139,
     140,   130,   131,   132,    14,    15,    16,    17,    18,    19,
      20,     0,     0,   444,     0,     0,     0,     0,    66,     0,
       0,   133,    30,     0,    67,    68,     0,     0,    36,     0,
      69,     0,    71,   369,    38,    72,     0,    73,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,    63,   139,   140,
     130,   131,   132,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    64,    65,
     133,    30,     0,     0,     0,     0,     0,    36,     0,     0,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,    63,     0,     0,     0,
      67,    68,     0,     0,     0,     0,    69,     0,    71,     0,
       0,    72,     0,    73,     0,     0,     0,    64,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,     0,   139,   140,   130,   131,   132,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,   133,    30,     0,     0,    67,
      68,     0,    36,     0,     0,    69,     0,    71,    38,   451,
      72,     0,    73,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,   242,     0,     0,     4,     5,     6,     7,     8,     0,
       0,    63,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    64,    65,   133,    30,     0,     0,     0,     0,
       0,    36,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
      63,     0,     0,     0,    67,    68,     0,     0,     0,     0,
      69,     0,    71,     0,     0,    72,     0,    73,     0,     0,
       0,    64,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   139,   140,   130,
     131,   132,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,   133,
      30,     0,     0,    67,    68,     0,    36,     0,     0,    69,
       0,    71,    38,     0,    72,   616,    73,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,    63,   139,   140,   130,   131,
     132,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   133,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,     0,    71,   717,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,   133,    30,     0,    67,    68,     0,
       0,    36,     0,    69,     0,    71,   719,    38,    72,     0,
      73,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      63,   139,   140,   130,   131,   132,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   133,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,     0,     0,    72,   721,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,     0,   139,   140,   130,
     131,   132,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,    66,     0,     0,   133,
      30,     0,    67,    68,     0,     0,    36,     0,    69,     0,
      71,   765,    38,    72,     0,    73,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     4,     5,
       6,   625,     8,     0,     0,    63,   139,   140,   130,   131,
     626,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    65,   133,    30,
       0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
       0,    38,     0,     0,     0,     0,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,    63,     0,     0,     0,    67,    68,
       0,     0,     0,     0,    69,     0,    71,   779,     0,    72,
       0,    73,     0,     0,     0,    64,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,    66,     0,     0,   133,    30,     0,    67,    68,     0,
       0,    36,     0,    69,     0,    71,   238,    38,    72,     0,
      73,     0,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,     7,     8,     0,     0,
      63,   139,   140,   130,   131,   132,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   133,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,    71,   933,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   139,   140,   130,   131,
     132,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   133,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
     155,    38,     0,    72,     0,    73,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   139,   140,   130,   131,   132,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   133,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,     0,   157,     0,     0,    72,     0,
      73,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     139,   140,   130,   131,   132,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   133,    30,     0,     0,    67,    68,     0,    36,
       0,     0,    69,     0,   159,    38,     0,    72,     0,    73,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     4,     5,     6,     7,     8,     0,     0,    63,   139,
     140,   130,   131,   132,    14,    15,    16,    17,    18,    19,
      20,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,   133,    30,     0,     0,     0,     0,     0,    36,     0,
       0,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,    46,    47,    48,    49,     0,     0,    52,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    57,    58,
       0,     0,     0,     0,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,    63,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,   161,
       0,     0,    72,     0,    73,     0,     0,     0,    64,    65,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     5,     6,     7,
       8,     0,     0,     0,   139,   140,   130,   131,   132,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,    66,     0,   133,    30,     0,     0,
      67,    68,     0,    36,     0,     0,    69,     0,    71,    38,
       0,    72,     0,    73,     0,     0,     0,    46,    47,    48,
      49,     0,     0,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,    58,     0,     0,     0,     0,    59,
       0,     0,     0,     0,     0,     4,     5,     6,     7,     8,
       0,     0,    63,   139,   140,   130,   131,   132,    14,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    65,   133,    30,     0,     0,     0,
       0,     0,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,    46,    47,    48,    49,
       0,     0,    52,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,    58,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
       0,    63,     0,     0,     0,    67,    68,     0,     0,     0,
       0,    69,     0,   182,     0,     0,    72,     0,    73,     0,
       0,     0,    64,    65,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     5,     6,     7,     8,     0,     0,     0,   139,   140,
     130,   131,   132,    14,    15,    16,    17,    18,    19,    20,
       0,     0,     0,     0,     0,     0,     0,     0,    66,     0,
     133,    30,     0,     0,    67,    68,     0,    36,     0,     0,
      69,     0,   184,    38,     0,    72,     0,    73,     0,     0,
       0,    46,    47,    48,    49,     0,     0,    52,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    58,     0,
       0,     0,     0,    59,     0,     0,     0,     0,     0,     4,
       5,     6,     7,     8,     0,     0,    63,   139,   140,   130,
     131,   132,    14,    15,    16,    17,    18,    19,    20,     0,
       0,     0,     0,     0,     0,     0,     0,    64,    65,   133,
      30,     0,     0,     0,     0,     0,    36,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
      46,    47,    48,    49,     0,     0,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    57,    58,     0,     0,
       0,     0,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,     0,    63,     0,     0,     0,    67,
      68,     0,     0,     0,     0,    69,     0,    71,     0,     0,
      72,     0,   198,     0,     0,     0,    64,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     4,     5,     6,     7,     8,     0,
       0,     0,   139,   140,   130,   131,   132,    14,    15,    16,
      17,    18,    19,    20,     0,     0,     0,     0,     0,     0,
       0,     0,    66,     0,   133,    30,     0,     0,    67,    68,
       0,    36,     0,     0,    69,     0,   337,    38,     0,    72,
       0,    73,     0,     0,     0,    46,    47,    48,    49,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    57,    58,     0,     0,     0,     0,    59,     0,     0,
       0,     0,     0,     4,     5,     6,   625,     8,     0,     0,
      63,   139,   140,   130,   131,   626,    14,    15,    16,    17,
      18,    19,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    64,    65,   133,    30,     0,     0,     0,     0,     0,
      36,     0,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,    46,    47,    48,    49,     0,     0,
      52,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,    59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,     0,    63,
       0,     0,     0,    67,    68,     0,     0,     0,     0,    69,
       0,   623,     0,     0,    72,     0,    73,     0,     0,     0,
      64,    65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     5,
       6,     7,     8,     0,     0,     0,   139,   140,   130,   131,
     132,    14,    15,    16,    17,    18,    19,    20,     0,     0,
       0,     0,     0,     0,     0,     0,    66,     0,   133,    30,
       0,     0,    67,    68,     0,    36,     0,     0,    69,     0,
     627,    38,     0,    72,     0,    73,     0,     0,     0,    46,
      47,    48,    49,     0,     0,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    57,    58,     0,     0,     0,
       0,    59,     0,     0,     0,     0,     0,     4,     5,     6,
       7,     8,     0,     0,    63,   139,   140,   130,   131,   132,
      14,    15,    16,    17,    18,    19,    20,     0,     0,     0,
       0,     0,     0,     0,     0,    64,    65,   133,    30,     0,
       0,     0,     0,     0,    36,     0,     0,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,    46,    47,
      48,    49,     0,     0,    52,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,     0,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,     0,    63,     0,     0,     0,    67,    68,     0,
       0,     0,     0,    69,     0,   805,     0,     0,    72,     0,
      73,     0,     0,     0,    64,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     4,     5,     6,     7,     8,     0,     0,     0,
     128,   129,   130,   131,   132,    14,    15,    16,    17,    18,
      19,    20,     0,     0,     0,     0,     0,     0,     0,     0,
      66,     0,   133,     0,     0,     0,    67,    68,     0,    36,
       0,     0,    69,     0,   829,    38,     0,    72,     0,    73,
       0,     0,     0,    46,    47,    48,    49,     0,     0,    52,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
      58,     0,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    64,
      65,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -539,  -539,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,     0,     0,     0,     0,
       0,    67,    68,     0,     0,     0,     0,    69,     0,    71,
       0,     0,    72,     0,    73,  -539,  -539,  -539,     0,  -539,
    -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,
    -539,  -539,  -539,  -539,  -539,     0,     0,     0,  -539,     0,
       0,     0,     0,     0,     0,     0,  -539,  -539,     0,     0,
    -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,
    -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,  -539,
    -539,     0,  -539,  -539,     0,     0,  -539,  -539,  -539,  -539,
    -539,  -539,  -539,  -539,  -539,     0,  -539,  -539,  -539,  -539,
    -539,  -539,  -539,  -539,     0,     0,  -539,  -539,  -539,  -539,
    -539,     0,     0,  -539,  -539,  -539
};

static const yytype_int16 yycheck[] =
{
       2,     0,   411,   255,   826,   655,  1020,   154,    24,    15,
     597,     2,     2,     2,   383,    17,   603,    28,    28,    15,
      24,   180,    24,    25,    26,    27,    28,    55,   187,    31,
      32,    33,    60,    61,    36,    37,    11,    12,    40,    41,
      15,    24,   325,    15,   327,    32,    48,    49,    12,    89,
      52,    53,    24,   462,    11,    12,    13,    28,    15,    28,
      63,    64,    11,    12,    67,    15,    15,    11,    12,    71,
      72,    15,    89,   356,   357,    15,    33,    48,    24,    48,
      28,    38,    42,    43,    24,    15,    11,    12,    89,    24,
      15,     0,    24,   165,   134,   135,    15,    72,    73,    74,
      48,    11,    12,    42,    43,    15,    42,    43,   180,   111,
     112,   113,   114,   115,   116,   187,   118,   119,    15,   121,
     162,   163,   124,    72,    73,    74,  1140,    24,    72,    73,
      74,   171,    11,   134,   135,    75,    15,   180,    11,    12,
      79,   143,    15,   145,   187,    15,    28,    72,    73,    74,
      69,    15,    11,   155,    24,   157,    15,   159,   980,   161,
      24,   165,    72,    73,    74,   182,    48,  1181,   185,   185,
     171,  1185,    28,    11,    12,   185,   763,    15,    75,   179,
     182,   187,   184,   186,   180,   187,   186,   189,   190,   185,
     192,   193,    48,   195,   196,   197,   198,   199,   185,    72,
      73,    74,   185,    11,    12,    75,   181,    15,   179,     6,
     179,    75,  1226,   185,   178,   186,   180,   186,    15,   165,
     178,  1235,   180,   180,    89,   165,   186,   636,    13,    16,
     165,   179,   181,   165,    72,    73,    74,   181,  1252,   179,
    1254,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     186,   253,   243,   243,   243,   283,   181,    28,    86,   406,
      81,    82,    83,   185,    72,    73,    74,   182,   165,   184,
      57,   181,  1286,    11,  1288,   110,   111,    48,    28,    66,
      67,    68,   179,   294,   295,   296,   297,   298,   299,   165,
     185,    28,    28,    28,   663,   165,    28,   179,    48,   165,
     182,   165,    28,   712,   713,    28,    13,   183,   181,   179,
      86,    48,    48,    48,   180,   179,    48,   182,   185,   167,
     185,   187,    48,   179,   326,    48,   182,   977,   110,   111,
     186,    15,    63,    64,    11,   337,    67,   185,    11,    77,
      78,   710,  1164,   181,    86,   180,    87,    88,   165,    33,
     178,   353,   180,   355,   110,   111,   358,   359,   360,   361,
     362,   363,   364,   180,    28,   186,   179,    31,    32,   182,
     187,   373,   374,   181,   376,   179,   378,    41,   182,    15,
      44,    45,   186,   165,    48,    49,   388,   389,   390,   391,
     392,   393,   394,   134,   135,    11,   398,   399,   409,   410,
      77,    78,   178,    28,   180,   180,    31,    32,   179,   165,
     185,   182,   165,   412,    11,   186,    41,   110,   111,    44,
      45,   423,   424,    48,   426,   427,   179,   429,   430,   179,
     171,    15,   182,   435,    18,   437,   186,   104,   105,   808,
     442,   443,   179,   179,   179,   165,    79,   179,     2,   451,
     186,   186,   167,   179,   456,   186,   179,   110,   111,   179,
     186,   119,   120,   746,   747,    87,    88,   466,   103,   838,
     185,   840,   165,   119,   120,    29,    30,   479,   480,   481,
     482,    65,    28,    11,    12,    31,    32,    15,    42,    43,
     178,   158,   159,   160,   179,    41,    50,    51,    44,    45,
     165,   186,    48,    49,   756,   117,   180,   165,    62,   180,
     117,   185,   134,   135,   185,   179,   119,   120,   183,   165,
     127,   164,   186,   892,   893,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   145,    28,
     167,   165,    31,    32,    72,    73,    74,    28,   167,   171,
      31,    32,    41,    15,   179,    44,    45,   182,   185,    48,
      41,   186,   165,    44,    45,   178,   185,    48,    49,   108,
     165,   110,   111,   112,   113,   114,   115,   116,   179,   133,
     129,   130,   131,   132,   133,   180,   588,   589,    15,   165,
     134,   135,   187,   179,    28,    31,   598,    31,    32,   165,
     186,   603,   971,   179,   887,   165,   160,    41,   162,   163,
      44,    45,   183,    49,    48,   179,   167,   183,   987,   179,
     178,   623,   186,   172,   173,   627,   165,   171,   630,   183,
     632,   185,   634,   179,   185,   182,   638,   639,   185,    28,
     186,   180,    31,    32,   110,   111,   112,   113,   114,   115,
     116,   178,    41,   165,   656,    44,    45,   165,   165,    48,
    1029,  1030,    15,   665,   218,   667,    28,   179,   165,    31,
      32,   179,   179,   179,   665,   665,   665,   179,   165,    41,
     186,   179,    44,    45,   186,   165,    48,    49,   186,   243,
     179,   693,   179,   182,   300,   301,   302,   186,   179,   165,
     179,   703,    11,    12,   179,   186,    15,   186,   179,  1118,
     179,   186,  1121,   715,   180,   186,   179,   186,    76,   182,
     179,   723,   724,   725,   715,   715,   715,   186,   179,   731,
     732,   182,   734,   735,   182,   737,   738,   739,   740,   741,
     742,   743,   744,  1026,   185,   179,   179,   179,   182,     8,
       9,    10,   186,   186,   186,   757,    15,    59,   365,   179,
     762,   179,   179,    72,    73,    74,   186,   179,   186,   186,
     772,   179,   179,   775,   186,   179,   778,   179,   186,   186,
     179,   179,   186,   785,   186,   179,   179,   186,   186,   396,
     179,   179,   186,   182,   401,   179,   798,   799,  1207,  1208,
     407,   179,   186,   805,   179,   179,   179,   179,   186,   179,
     812,   186,   186,   186,   186,   422,   186,   179,    90,    91,
      92,    93,   179,   179,   185,   432,   182,   829,   180,   186,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   841,
     842,   843,   182,   179,   184,   109,   182,   849,   285,   286,
     287,   288,   289,   290,   291,   292,   293,    11,    12,    13,
     179,   179,   179,   470,   866,   472,   473,   186,   186,   186,
     143,   144,   179,   180,   147,   148,   878,   149,   150,   151,
     152,   153,   154,   155,   118,   157,   185,   121,   122,   123,
     124,   125,   126,   127,   128,   167,   168,   161,   182,   179,
     184,   455,   182,   457,    15,   907,   908,   909,   910,   911,
     912,   179,   186,    24,   182,    26,    27,   136,   137,   138,
     139,   140,   141,   179,    35,    36,   179,   179,   185,    40,
     186,   179,   179,   186,   186,    46,    47,   180,   186,   186,
     174,   175,   176,    54,    55,    56,    57,   179,   179,   179,
     169,   170,   179,   955,    65,   186,   186,   179,   179,   186,
     168,   181,   182,    11,   186,   186,    14,    15,    16,    17,
     577,   305,   306,   307,   308,   309,   310,   311,   179,    11,
     587,   180,    14,   590,    16,    17,   185,   182,   595,   184,
     597,   993,   994,   995,   996,   997,   998,   999,  1000,    15,
     180,   180,   180,  1005,   179,   185,   185,   185,    24,   179,
      26,    27,   254,  1015,   256,   257,   258,   179,   179,    35,
      36,   182,   179,   179,    40,   182,   182,   172,   173,   179,
      46,    47,   182,   179,   641,   182,   182,   184,    54,    55,
      56,    57,   179,   650,   179,   182,   182,   182,   184,    65,
     182,   179,   184,  1055,  1056,   186,   182,    15,   184,   180,
      18,   181,   182,   181,   182,   179,    24,  1069,    26,    27,
      15,  1073,  1074,   181,   182,   181,   182,    35,    36,   181,
     182,   180,    40,   181,   182,   181,   182,    15,    46,    47,
    1092,  1093,  1094,   181,   182,    15,    54,    55,    56,    57,
     654,   655,    29,    30,   658,   659,   660,  1109,   181,   182,
     179,   665,   165,  1115,   181,   182,   165,   671,   181,   182,
    1119,   181,   182,    15,  1126,   181,   182,   181,   182,  1131,
     181,   182,   179,   687,   688,   689,   181,   182,   181,   182,
     181,   182,   181,   182,  1146,  1147,    15,   701,   181,   182,
     704,   758,   759,   181,   182,   179,   763,   764,   181,   182,
     767,   715,   119,   120,   119,   120,    15,   181,   182,   181,
     182,   181,   182,   181,   182,    24,    29,    26,    27,   181,
     182,   179,  1184,    29,    30,    11,    35,    36,   181,   182,
     797,    40,   179,  1184,  1184,  1184,  1198,    46,    47,   181,
     182,  1203,   181,   182,   179,    54,    55,    56,    57,   181,
     182,   181,   182,   181,   182,   181,   182,   181,   182,    29,
      30,  1223,   181,   182,   186,   181,   182,  1229,  1230,  1231,
      29,    30,   181,   182,   181,   182,   179,  1239,   180,  1241,
     181,   182,   181,   182,    47,    48,   303,   304,   179,    11,
     180,   106,   178,   178,   161,   179,   179,   179,   179,    15,
    1262,   179,   179,   179,   179,  1267,  1268,  1269,  1270,  1271,
    1272,  1273,  1274,   179,   165,   180,   830,   186,   168,   179,
     179,    86,   179,   165,   179,   179,   179,    15,    11,  1291,
    1292,  1293,   179,   179,   179,   179,   179,    11,   179,   179,
     179,   179,   179,   179,   179,   168,     3,     4,     5,     6,
       7,   182,   186,   867,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,   168,   186,   882,   179,
     165,   179,   186,    15,    15,   179,    33,    34,   185,   184,
      86,   184,    15,    40,    11,   179,   179,   179,   179,    46,
      11,   958,    11,    15,   179,    15,   179,    54,    55,    56,
      57,   968,   179,    60,    29,   186,    15,    63,    15,    15,
     186,   179,   179,    70,    71,   179,    86,   179,   179,    76,
     180,   179,    86,   179,   181,   179,   179,    15,    15,    11,
      11,   186,    89,   179,   179,   179,     8,     8,     8,   165,
     179,    15,    79,   181,   179,   179,    15,    15,    15,    15,
     179,   179,   179,   110,   111,   180,   179,   181,    15,    15,
     179,   179,   179,   977,   185,   179,   186,   981,   982,   983,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   179,    15,  1009,    15,   179,   179,  1013,
     179,   179,   179,   179,   179,  1223,   179,   179,  1075,   166,
     181,   179,  1079,    30,   840,   172,   173,   259,   627,   708,
      -1,   178,    -1,   180,   181,    -1,   183,    -1,   185,    -1,
     187,     3,     4,     5,     6,     7,   284,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
    1084,  1085,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    89,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,  1180,    60,    -1,    -1,
    1184,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    89,    -1,    -1,    -1,
     172,   173,    -1,    -1,  1218,    -1,   178,    -1,   180,   181,
      -1,   183,    -1,   185,    -1,   187,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,  1289,    33,    34,    -1,   172,
     173,    -1,    -1,    40,    -1,   178,    -1,   180,   181,    46,
     183,    -1,   185,    -1,   187,    -1,    -1,    54,    55,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   166,
      -1,    -1,    -1,    -1,    -1,   172,   173,    -1,    -1,    -1,
      -1,   178,    -1,   180,    -1,    -1,   183,    -1,   185,    -1,
     187,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    -1,    66,    67,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,
     172,   173,    -1,    -1,    -1,    -1,   178,   179,   180,    -1,
      -1,   183,    -1,   185,   186,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    -1,    -1,    -1,    -1,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    -1,    80,    -1,    -1,    -1,    84,    85,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
       3,     4,     5,     6,     7,    -1,    -1,    -1,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    76,    -1,    -1,   166,    -1,    -1,    -1,
      -1,    -1,   172,   173,    -1,    -1,    89,    -1,   178,   179,
     180,    -1,    -1,   183,    -1,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,
     173,    -1,    40,    -1,    -1,   178,   179,   180,    46,    -1,
     183,    -1,   185,   186,    -1,    -1,    54,    55,    56,    57,
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
     178,   179,   180,    -1,    -1,   183,    -1,   185,   186,    -1,
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
      85,    -1,    -1,    -1,    89,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,   172,   173,    -1,
      -1,    89,    -1,   178,   179,   180,    -1,    -1,   183,    -1,
     185,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     173,    -1,    -1,    -1,    -1,   178,   179,   180,    -1,    -1,
     183,    -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   166,    -1,    33,    34,    -1,    -1,   172,   173,
      -1,    40,    -1,    -1,   178,   179,   180,    46,    -1,   183,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    33,    34,
      -1,    -1,   172,   173,    -1,    40,    -1,    -1,   178,   179,
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
      -1,    -1,    -1,   178,    -1,   180,   181,    -1,   183,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   166,    -1,    33,    34,    -1,    -1,
     172,   173,   174,    40,    -1,    -1,   178,    -1,   180,    46,
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
      -1,   178,   179,   180,    -1,    -1,   183,    -1,   185,    -1,
      -1,    -1,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,   166,    -1,
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
     173,    -1,    40,    -1,    -1,   178,    -1,   180,    46,   182,
     183,    -1,   185,    -1,    -1,    -1,    54,    55,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    76,    -1,
      -1,    79,    -1,    -1,     3,     4,     5,     6,     7,    -1,
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
      -1,   180,    46,    -1,   183,   184,   185,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   180,    -1,    -1,   183,   184,   185,    -1,    -1,    -1,
     110,   111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    33,
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
      -1,    -1,    -1,    -1,   178,    -1,   180,   181,    -1,   183,
      -1,   185,    -1,    -1,    -1,   110,   111,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   180,   181,    -1,   183,    -1,   185,    -1,    -1,    -1,
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
     166,    -1,    33,    -1,    -1,    -1,   172,   173,    -1,    40,
      -1,    -1,   178,    -1,   180,    46,    -1,   183,    -1,   185,
      -1,    -1,    -1,    54,    55,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   172,   173,    -1,    -1,    -1,    -1,   178,    -1,   180,
      -1,    -1,   183,    -1,   185,    86,    87,    88,    -1,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,    -1,    -1,    -1,   109,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,   118,    -1,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,    -1,   143,   144,    -1,    -1,   147,   148,   149,   150,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   170,
     171,    -1,    -1,   174,   175,   176
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
     232,   233,   234,   235,   238,   239,   241,   242,   108,   110,
     111,   112,   113,   114,   115,   116,   165,   180,   165,   183,
     167,   185,   110,   111,   165,    12,   178,   180,    11,    12,
      13,    14,    15,    33,   219,   234,   239,   241,   242,    11,
      12,   184,   221,    15,   179,   180,   221,   240,    11,    12,
      13,    15,    33,    38,   180,   180,   221,   180,   221,   180,
     221,   180,   221,   221,   230,    15,   180,   219,   200,   219,
     221,   179,   221,   179,   221,    11,    15,    15,   221,   221,
      13,    13,   180,   221,   180,   221,   219,   180,   219,    28,
      48,   179,    28,    48,   179,   185,   220,   220,   185,   220,
     221,   221,   219,   185,   219,   221,   179,   221,    15,   208,
     208,    15,    33,   234,   208,   208,   219,    11,   236,    11,
      11,   234,   234,   234,    15,    24,    26,    27,    35,    36,
      40,    46,    47,    54,    55,    56,    57,   237,   181,   221,
     184,   221,    79,   190,    28,    31,    32,    41,    44,    45,
      48,    49,   179,    86,   104,   105,   158,   159,   160,   103,
      90,    91,    92,    93,   149,   150,   151,   152,   153,   154,
     155,   157,   167,   168,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   109,   161,   136,   137,   138,   139,   140,
     141,   169,   170,   117,   143,   144,   147,   148,    87,    88,
     134,   135,   171,   162,   163,   129,   130,   131,   132,   133,
     172,   173,   118,   121,   122,   123,   124,   125,   126,   127,
     128,   174,   175,   176,   164,   178,   165,   178,    15,   221,
     221,   221,   221,   221,   221,   181,   240,   180,   221,   174,
     221,    15,   221,   221,   179,    15,    18,    65,    15,   181,
     187,   216,   240,   183,   167,   185,   178,   178,   112,   113,
     114,   115,   116,   165,   165,   182,   184,   179,   221,   181,
     221,   240,   179,   182,   165,   179,   165,   179,   165,   179,
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
     229,   230,   230,   230,   230,   230,   230,   231,   231,   231,
     232,   232,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     234,   234,   237,   221,    18,   237,   179,   180,   179,   179,
     179,   179,   179,   179,   181,   221,   179,   182,   172,   173,
     182,   184,   168,   186,   179,   180,    15,   180,    86,    15,
     181,   182,   181,   182,   221,    15,   221,   237,   237,   221,
     221,   221,   221,   221,   221,   221,   184,   240,   179,   179,
     181,   221,   221,   180,   221,     6,    15,   180,   217,   221,
     165,   179,   165,   179,   165,   179,   180,   206,   165,   165,
     181,   182,   221,   221,   221,   221,   221,   221,   221,   240,
     182,   221,   221,   240,   181,    29,    30,   193,   181,   236,
     240,   230,   230,   181,   218,   190,    24,   165,    11,    77,
      78,   181,   182,   240,   221,   221,   221,   221,   221,   221,
     179,   240,   203,    15,   221,    15,   221,   181,    29,   181,
     221,   221,    11,   179,   179,   179,   179,   179,   186,   221,
     219,   221,   219,    42,    43,   186,    15,    24,   185,   213,
     181,   218,   180,   180,   215,   190,    11,   181,   240,   181,
     240,   184,   240,   183,   185,   165,   221,   221,   221,   186,
     221,    28,    48,   186,    28,    48,   186,    28,    31,    32,
      41,    44,    45,    48,    49,   186,   178,   178,   179,   179,
     179,   179,   179,   179,   179,   179,   106,   165,   180,   187,
     179,    15,   165,   180,   187,   181,   240,   182,   240,   221,
     221,   240,   165,   119,   120,   165,   119,   120,   165,   181,
     240,   181,   216,   240,   221,   180,    15,   187,   216,   184,
     168,   186,   184,   179,   179,   221,   179,   182,    86,    86,
     217,   179,   182,   179,   221,   180,   221,   221,   181,   218,
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
     221,   221,   181,     6,    15,   179,   221,   179,   182,   179,
     206,   181,   179,   179,   221,   181,   181,    29,   193,   221,
     219,    11,    11,    11,    15,    24,   206,   204,   206,   221,
     221,   186,   221,    28,    31,    32,    41,    44,    45,    48,
      49,   186,    15,   221,    24,   165,    11,    77,    78,   221,
     219,   179,   179,    29,   221,   179,   219,   186,   186,    65,
     237,    11,    14,    15,    16,    17,    63,   206,   206,   181,
     181,    15,    15,    15,   184,   186,   186,   221,   221,   221,
     221,   221,   221,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   165,   165,   179,   181,   179,
     181,   181,   179,   181,   179,   179,   179,   181,   221,   180,
     240,   179,   179,    86,    86,   182,   240,   206,   179,   182,
     179,   179,   192,   219,   181,    29,   193,   219,   219,   219,
      15,   206,    28,    32,    48,   186,   186,   186,   221,   221,
     221,   221,   221,   221,   221,   221,    15,   221,    24,   165,
      11,    11,   219,   186,   219,   179,   199,   221,   180,   185,
     207,   180,   207,    15,    24,    75,   165,   179,    15,    24,
      75,   165,   179,    11,    14,    16,    17,   179,   179,    65,
     237,   206,   206,     8,     8,     8,   165,   165,   186,   186,
     186,   186,   186,   186,   221,   221,   179,   221,   181,   221,
     221,   240,   179,   240,   219,   219,   221,   221,   221,   186,
     186,   186,   186,   186,   186,   186,   186,    15,   221,   221,
     181,   181,   218,    79,   190,   181,   218,    15,    15,   179,
     221,    15,    15,   179,   221,    15,    24,    75,   165,   179,
      15,    24,    75,   165,   179,   179,   179,   180,   180,   207,
     221,   221,   186,   186,   181,   179,   181,   179,    29,   193,
     186,   186,   186,   179,   219,   207,   181,   186,    14,    26,
      27,    36,   186,   221,   207,   181,   179,   179,   179,   165,
     179,   165,   179,   179,    15,    15,   179,   221,    15,    15,
     179,   221,   181,   218,   181,   218,   186,   186,   179,   219,
     199,   207,   165,   221,   221,   186,   221,    28,    31,    32,
      41,    44,    45,    48,    49,   186,   207,   221,   221,   179,
     179,   179,   179,   179,   179,   207,   181,   207,   181,   181,
     221,    28,    32,    48,   186,   186,   186,   221,   221,   221,
     221,   221,   221,   221,   221,   179,   179,   207,   207,   219,
     186,   221,   221,   221,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186
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
     191,   191,   191,   192,   192,   192,   192,   192,   192,   192,
     192,   193,   193,   193,   193,   193,   193,   194,   194,   195,
     195,   195,   195,   196,   196,   197,   197,   197,   198,   198,
     198,   199,   200,   200,   200,   200,   200,   200,   201,   201,
     202,   202,   203,   203,   204,   204,   205,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   205,   205,   205,   206,
     206,   206,   206,   206,   206,   206,   206,   206,   206,   206,
     206,   206,   206,   206,   206,   206,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   208,   209,   210,   211,   212,
     212,   212,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   215,   215,   215,   215,   216,   216,   216,   216,   216,
     217,   217,   217,   217,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   220,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   222,   222,   223,   223,   223,   223,   223,
     224,   224,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     226,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   228,   228,   229,   229,   229,   229,   229,   229,   229,
     230,   230,   230,   230,   231,   231,   231,   232,   232,   232,
     232,   232,   232,   232,   232,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   234,   234,
     234,   234,   234,   234,   235,   235,   236,   236,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   238,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   240,   240,
     240,   241,   241,   241,   241,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   242
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     5,     3,     7,     9,     4,
       3,     3,     5,     7,     9,     4,     6,     8,     5,     5,
       7,     6,     6,     8,    10,     6,     4,     4,     4,     3,
       4,     5,     5,     6,     6,     4,     2,     5,     4,     3,
       3,     3,     5,     7,     3,     5,     7,     3,     5,     3,
       2,     3,     2,     5,     5,     3,     2,     5,     5,     4,
       7,     6,     4,     6,     6,     4,     7,     7,     7,     6,
       6,     4,     4,     4,     4,     4,     4,     4,     4,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     4,     4,
       4,     4,     4,     2,     1,     1,     1,     1,     1,     2,
       4,     4,     6,     5,     2,     1,     2,     3,     1,     1,
       1,     1,     2,     2,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     5,     7,     7,     3,     5,     5,     4,
       6,     3,     5,     5,     7,     4,     6,     5,     3,     5,
       7,     3,     5,     5,     3,     2,     5,     5,     2,    12,
       9,     1,     7,     7,     5,     7,     5,     3,     5,     7,
       3,     4,     0,     4,     2,     3,     6,     5,     3,     7,
       6,     7,     6,     4,     7,     6,     7,     6,     5,     3,
       4,     5,     4,     5,     5,     6,     6,     6,     6,     6,
       6,     6,     6,     7,     7,     7,     3,     3,     6,     4,
       5,     4,     5,     5,     6,     6,     6,     6,     6,     6,
       6,     6,     7,     7,     7,     1,     6,     5,     5,     0,
       3,     3,     0,     4,     4,     4,     5,     5,     4,     5,
       5,     6,     6,     7,     7,     6,     8,     8,     6,     6,
       6,     5,     5,     6,     6,     7,     7,     7,     7,     7,
       6,     4,     7,     6,     4,     8,     7,     5,     8,     7,
       5,     0,     4,     4,     4,     3,     5,     2,     5,     7,
       3,     3,     5,     5,     1,     3,     5,     4,     6,     1,
       3,     2,     3,     3,     3,     4,     5,     5,     3,     5,
       4,     6,     1,     3,     1,     3,     1,     3,     3,     3,
       4,     6,     6,     6,     6,     6,     6,     6,     6,     5,
       7,     7,     7,     7,     7,     7,     5,     8,     8,     6,
       9,     9,     5,     5,     4,     4,     4,     6,     6,     6,
       6,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     3,     1,     5,     1,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     1,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     2,     2,
       2,     2,     2,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     4,     3,     4,     6,     4,     3,     6,
       6,     5,     3,     3,     6,     5,     4,     4,     6,     6,
       5,     3,     6,     5,     3,     5,     5,     5,     4,     2,
       2,     3,     4,     3,     2,     3,     2,     1,     1,     3,
       2,     2,     3,     4,     5,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     1,
       1,     1,     1,     4,     6,     6,     6,     1,     4,     4,
       5,     5,     5,     5,     5,     5,     1,     1,     1,     1,
       2,     3,     4,     5,     2,     3,     4,     5,     1,     1,
       2,     5,     3
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
#line 695 "raku.y"
        {
            ExprList *all = rk_phasers_place((yyvsp[0].list), 1);
            if (all) {
                for (int i = 0; i < all->count; i++)
                    if (all->items[i]) { rk_phaser_inline_tree(all->items[i]); add_proc(all->items[i]); }
                exprlist_free(all);
            }
        }
#line 3929 "raku.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 705 "raku.y"
         { (yyval.list) = exprlist_new(); }
#line 3935 "raku.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 706 "raku.y"
                     { (yyval.list) = exprlist_append((yyvsp[-1].list), (yyvsp[0].node)); }
#line 3941 "raku.tab.c"
    break;

  case 5: /* stmt: KW_MY VAR_SCALAR '=' expr ';'  */
#line 710 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_scalar_rhs((yyvsp[-1].node))); }
#line 3947 "raku.tab.c"
    break;

  case 6: /* stmt: KW_MY VAR_SCALAR ';'  */
#line 712 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), ast_node_new(TT_NUL)); }
#line 3953 "raku.tab.c"
    break;

  case 7: /* stmt: KW_MY '(' scalar_list ')' '=' expr ';'  */
#line 714 "raku.y"
        { (yyval.node) = rk_destructure((yyvsp[-4].list), (yyvsp[-1].node)); }
#line 3959 "raku.tab.c"
    break;

  case 8: /* stmt: KW_MY '(' scalar_list ')' '=' expr ',' arg_list ';'  */
#line 716 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = rk_destructure((yyvsp[-6].list), call); }
#line 3967 "raku.tab.c"
    break;

  case 9: /* stmt: paren_group '=' expr ';'  */
#line 720 "raku.y"
        { (yyval.node) = rk_destructure(rk_group_targets((yyvsp[-3].node)), (yyvsp[-1].node)); }
#line 3973 "raku.tab.c"
    break;

  case 10: /* stmt: KW_MY VAR_ARRAY ';'  */
#line 722 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3979 "raku.tab.c"
    break;

  case 11: /* stmt: KW_MY VAR_HASH ';'  */
#line 724 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-1].sval)), make_call("__rk_undef")); }
#line 3985 "raku.tab.c"
    break;

  case 12: /* stmt: KW_MY VAR_ARRAY '=' expr ';'  */
#line 726 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 3991 "raku.tab.c"
    break;

  case 13: /* stmt: KW_MY VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 728 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 3999 "raku.tab.c"
    break;

  case 14: /* stmt: KW_MY VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 732 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4007 "raku.tab.c"
    break;

  case 15: /* stmt: VAR_ARRAY '=' expr ';'  */
#line 736 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), rk_arr_rhs((yyvsp[-1].node))); }
#line 4013 "raku.tab.c"
    break;

  case 16: /* stmt: VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 738 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), call); }
#line 4021 "raku.tab.c"
    break;

  case 17: /* stmt: VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 742 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-7].sval)), call); }
#line 4029 "raku.tab.c"
    break;

  case 18: /* stmt: KW_MY VAR_HASH '=' expr ';'  */
#line 746 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4035 "raku.tab.c"
    break;

  case 19: /* stmt: KW_MY VAR_HASH '=' pair_list ';'  */
#line 748 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); }
#line 4041 "raku.tab.c"
    break;

  case 20: /* stmt: KW_MY VAR_HASH '=' '(' pair_list ')' ';'  */
#line 750 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-5].sval)), (yyvsp[-2].node)); }
#line 4047 "raku.tab.c"
    break;

  case 21: /* stmt: KW_MY IDENT VAR_SCALAR '=' expr ';'  */
#line 752 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4053 "raku.tab.c"
    break;

  case 22: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ';'  */
#line 754 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,rk_arr_rhs((yyvsp[-1].node))); (yyval.node)=e; }
#line 4059 "raku.tab.c"
    break;

  case 23: /* stmt: KW_MY IDENT VAR_ARRAY '=' expr ',' arg_list ';'  */
#line 756 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *args=(yyvsp[-1].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-6].sval))); ct_drop((yyvsp[-6].sval)); ast_push(e,var_node((yyvsp[-5].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4067 "raku.tab.c"
    break;

  case 24: /* stmt: KW_MY IDENT VAR_ARRAY '=' '(' expr ',' arg_list ')' ';'  */
#line 760 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-4].node));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) expr_add_child(call,args->items[i]); exprlist_free(args); }
          tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-8].sval))); ct_drop((yyvsp[-8].sval)); ast_push(e,var_node((yyvsp[-7].sval))); ast_push(e,call); (yyval.node)=e; }
#line 4075 "raku.tab.c"
    break;

  case 25: /* stmt: KW_MY IDENT VAR_HASH '=' expr ';'  */
#line 764 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval)); ast_push(e,var_node((yyvsp[-3].sval))); ast_push(e,(yyvsp[-1].node)); (yyval.node)=e; }
#line 4081 "raku.tab.c"
    break;

  case 26: /* stmt: KW_MY IDENT VAR_SCALAR ';'  */
#line 766 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4087 "raku.tab.c"
    break;

  case 27: /* stmt: KW_MY IDENT VAR_ARRAY ';'  */
#line 768 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4093 "raku.tab.c"
    break;

  case 28: /* stmt: KW_MY IDENT VAR_HASH ';'  */
#line 770 "raku.y"
        { tree_t *e=ast_node_new(TT_DECL); ast_push(e,leaf_sval(TT_VAR,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); ast_push(e,var_node((yyvsp[-1].sval))); (yyval.node)=e; }
#line 4099 "raku.tab.c"
    break;

  case 29: /* stmt: KW_USE IDENT ';'  */
#line 772 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node)=u; }
#line 4105 "raku.tab.c"
    break;

  case 30: /* stmt: KW_USE IDENT expr ';'  */
#line 774 "raku.y"
        { tree_t *u=ast_node_new(TT_USE_DECL); u->v.sval=intern((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); ast_push(u,(yyvsp[-1].node)); (yyval.node)=u; }
#line 4111 "raku.tab.c"
    break;

  case 31: /* stmt: KW_CONSTANT IDENT '=' expr ';'  */
#line 776 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4117 "raku.tab.c"
    break;

  case 32: /* stmt: KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 778 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4123 "raku.tab.c"
    break;

  case 33: /* stmt: KW_MY KW_CONSTANT IDENT '=' expr ';'  */
#line 780 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4129 "raku.tab.c"
    break;

  case 34: /* stmt: KW_MY KW_CONSTANT VAR_SCALAR '=' expr ';'  */
#line 782 "raku.y"
        { (yyval.node) = expr_binary(TT_ASSIGN, var_node((yyvsp[-3].sval)), (yyvsp[-1].node)); ct_drop((yyvsp[-3].sval)); }
#line 4135 "raku.tab.c"
    break;

  case 35: /* stmt: KW_ENUM IDENT WORDLIST ';'  */
#line 784 "raku.y"
        { ExprList *l=exprlist_new(); char *s=(yyvsp[-1].sval); int idx=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1);
            memcpy(tok,w,L); tok[L]='\0';
            tree_t *val=ast_node_new(TT_ILIT); val->v.ival=idx++;
            exprlist_append(l, expr_binary(TT_ASSIGN, var_node(tok), val)); ct_drop(tok); }
          ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval)); (yyval.node) = make_seq(l); }
#line 4147 "raku.tab.c"
    break;

  case 36: /* stmt: TESTOP ';'  */
#line 792 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval)); }
#line 4153 "raku.tab.c"
    break;

  case 37: /* stmt: TESTOP '(' arg_list ')' ';'  */
#line 794 "raku.y"
        { ExprList *a=(yyvsp[-2].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-4].sval)), a); ct_drop((yyvsp[-4].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4159 "raku.tab.c"
    break;

  case 38: /* stmt: TESTOP '(' ')' ';'  */
#line 796 "raku.y"
        { (yyval.node)=make_call(testop_rt((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval)); }
#line 4165 "raku.tab.c"
    break;

  case 39: /* stmt: TESTOP arg_list ';'  */
#line 798 "raku.y"
        { ExprList *a=(yyvsp[-1].list); tree_t *c=rk_testop_call(testop_rt((yyvsp[-2].sval)), a); ct_drop((yyvsp[-2].sval)); if(a) exprlist_free(a); (yyval.node)=c; }
#line 4171 "raku.tab.c"
    break;

  case 40: /* stmt: IDENT VAR_ARRAY ';'  */
#line 800 "raku.y"
        { tree_t *c=make_call((yyvsp[-2].sval)); ct_drop((yyvsp[-2].sval)); expr_add_child(c,var_node((yyvsp[-1].sval))); (yyval.node)=c; }
#line 4177 "raku.tab.c"
    break;

  case 41: /* stmt: KW_SAY expr ';'  */
#line 802 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4183 "raku.tab.c"
    break;

  case 42: /* stmt: KW_SAY expr ',' arg_list ';'  */
#line 804 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4190 "raku.tab.c"
    break;

  case 43: /* stmt: KW_SAY '(' expr ',' arg_list ')' ';'  */
#line 807 "raku.y"
        { tree_t *c=ast_node_new(TT_SAY); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4197 "raku.tab.c"
    break;

  case 44: /* stmt: KW_PRINT expr ';'  */
#line 810 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4203 "raku.tab.c"
    break;

  case 45: /* stmt: KW_PRINT expr ',' arg_list ';'  */
#line 812 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4210 "raku.tab.c"
    break;

  case 46: /* stmt: KW_PRINT '(' expr ',' arg_list ')' ';'  */
#line 815 "raku.y"
        { tree_t *c=ast_node_new(TT_PRINT); expr_add_child(c,(yyvsp[-4].node));
          ExprList *a=(yyvsp[-2].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(c,a->items[i]); exprlist_free(a); } (yyval.node)=c; }
#line 4217 "raku.tab.c"
    break;

  case 47: /* stmt: KW_TAKE expr ';'  */
#line 818 "raku.y"
        { (yyval.node)=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); }
#line 4223 "raku.tab.c"
    break;

  case 48: /* stmt: KW_TAKE expr ',' arg_list ';'  */
#line 820 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); }
          (yyval.node)=expr_unary(TT_SUSPEND,call); }
#line 4231 "raku.tab.c"
    break;

  case 49: /* stmt: KW_RETURN expr ';'  */
#line 824 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); (yyval.node)=r; }
#line 4237 "raku.tab.c"
    break;

  case 50: /* stmt: KW_RETURN ';'  */
#line 826 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4243 "raku.tab.c"
    break;

  case 51: /* stmt: KW_FAIL expr ';'  */
#line 828 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4249 "raku.tab.c"
    break;

  case 52: /* stmt: KW_FAIL ';'  */
#line 830 "raku.y"
        { (yyval.node)=ast_node_new(TT_RETURN); }
#line 4255 "raku.tab.c"
    break;

  case 53: /* stmt: KW_RETURN expr KW_IF expr ';'  */
#line 832 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(r)); (yyval.node)=e; }
#line 4262 "raku.tab.c"
    break;

  case 54: /* stmt: KW_RETURN expr KW_UNLESS expr ';'  */
#line 835 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(r)); (yyval.node)=e; }
#line 4269 "raku.tab.c"
    break;

  case 55: /* stmt: KW_EXIT expr ';'  */
#line 838 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4275 "raku.tab.c"
    break;

  case 56: /* stmt: KW_EXIT ';'  */
#line 840 "raku.y"
        { tree_t *c=make_call("__rk_exit"); tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0;
          expr_add_child(c,z); (yyval.node)=c; }
#line 4282 "raku.tab.c"
    break;

  case 57: /* stmt: KW_EXIT expr KW_IF expr ';'  */
#line 843 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(c)); (yyval.node)=e; }
#line 4289 "raku.tab.c"
    break;

  case 58: /* stmt: KW_EXIT expr KW_UNLESS expr ';'  */
#line 846 "raku.y"
        { tree_t *c=make_call("__rk_exit"); expr_add_child(c,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(c)); (yyval.node)=e; }
#line 4296 "raku.tab.c"
    break;

  case 59: /* stmt: VAR_SCALAR '=' expr ';'  */
#line 849 "raku.y"
        { rk_mark_arrlit_scalar(strip_sigil((yyvsp[-3].sval)), (yyvsp[-1].node)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),rk_scalar_rhs((yyvsp[-1].node))); }
#line 4302 "raku.tab.c"
    break;

  case 60: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' arg_list ')' ';'  */
#line 851 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-6].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ExprList *args=(yyvsp[-2].list); if(args){ for(int i=0;i<args->count;i++) ast_push(mc,args->items[i]); exprlist_free(args); }
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-6].sval)),mc); }
#line 4311 "raku.tab.c"
    break;

  case 61: /* stmt: VAR_SCALAR OP_DOTEQ IDENT '(' ')' ';'  */
#line 856 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-5].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-5].sval)),mc); }
#line 4319 "raku.tab.c"
    break;

  case 62: /* stmt: VAR_SCALAR OP_DOTEQ IDENT ';'  */
#line 860 "raku.y"
        { tree_t *mc=ast_node_new(TT_METHCALL);
          ast_push(mc,var_node((yyvsp[-3].sval))); ast_push(mc,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),mc); }
#line 4327 "raku.tab.c"
    break;

  case 63: /* stmt: call_expr '.' meth_name '=' expr ';'  */
#line 864 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4334 "raku.tab.c"
    break;

  case 64: /* stmt: atom '.' meth_name '=' expr ';'  */
#line 867 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4341 "raku.tab.c"
    break;

  case 65: /* stmt: VAR_TWIGIL '=' expr ';'  */
#line 870 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); }
#line 4349 "raku.tab.c"
    break;

  case 66: /* stmt: VAR_ARRAY '[' expr ']' '=' expr ';'  */
#line 874 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4356 "raku.tab.c"
    break;

  case 67: /* stmt: VAR_HASH '<' IDENT '>' '=' expr ';'  */
#line 877 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-4].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4363 "raku.tab.c"
    break;

  case 68: /* stmt: VAR_HASH '{' expr '}' '=' expr ';'  */
#line 880 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET);
          ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 4370 "raku.tab.c"
    break;

  case 69: /* stmt: KW_DELETE VAR_HASH '<' IDENT '>' ';'  */
#line 883 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 4377 "raku.tab.c"
    break;

  case 70: /* stmt: KW_DELETE VAR_HASH '{' expr '}' ';'  */
#line 886 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE);
          ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 4384 "raku.tab.c"
    break;

  case 71: /* stmt: expr KW_IF expr ';'  */
#line 889 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4390 "raku.tab.c"
    break;

  case 72: /* stmt: expr KW_UNLESS expr ';'  */
#line 891 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4396 "raku.tab.c"
    break;

  case 73: /* stmt: expr KW_WHILE expr ';'  */
#line 893 "raku.y"
        { (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); }
#line 4402 "raku.tab.c"
    break;

  case 74: /* stmt: expr KW_UNTIL expr ';'  */
#line 895 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); (yyval.node)=e; }
#line 4408 "raku.tab.c"
    break;

  case 75: /* stmt: expr KW_FOR expr ';'  */
#line 897 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1((yyvsp[-3].node))); }
#line 4415 "raku.tab.c"
    break;

  case 76: /* stmt: expr KW_WITH expr ';'  */
#line 900 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); }
#line 4421 "raku.tab.c"
    break;

  case 77: /* stmt: expr KW_WITHOUT expr ';'  */
#line 902 "raku.y"
        { (yyval.node)=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); }
#line 4427 "raku.tab.c"
    break;

  case 78: /* stmt: expr KW_GIVEN expr ';'  */
#line 904 "raku.y"
        { (yyval.node)=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); }
#line 4433 "raku.tab.c"
    break;

  case 79: /* stmt: KW_SAY expr KW_IF expr ';'  */
#line 906 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); (yyval.node)=e; }
#line 4440 "raku.tab.c"
    break;

  case 80: /* stmt: KW_SAY expr KW_UNLESS expr ';'  */
#line 909 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); (yyval.node)=e; }
#line 4447 "raku.tab.c"
    break;

  case 81: /* stmt: KW_SAY expr KW_FOR expr ';'  */
#line 912 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          (yyval.node)=expr_binary(TT_EVERY, gen, seq1(s)); }
#line 4455 "raku.tab.c"
    break;

  case 82: /* stmt: KW_SAY expr KW_WHILE expr ';'  */
#line 916 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          (yyval.node)=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1(s)); }
#line 4462 "raku.tab.c"
    break;

  case 83: /* stmt: KW_SAY expr KW_WITH expr ';'  */
#line 919 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),0); }
#line 4468 "raku.tab.c"
    break;

  case 84: /* stmt: KW_SAY expr KW_WITHOUT expr ';'  */
#line 921 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_with_mod(s,(yyvsp[-1].node),1); }
#line 4474 "raku.tab.c"
    break;

  case 85: /* stmt: KW_SAY expr KW_GIVEN expr ';'  */
#line 923 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node)); (yyval.node)=rk_given_mod(s,(yyvsp[-1].node)); }
#line 4480 "raku.tab.c"
    break;

  case 86: /* stmt: KW_PRINT expr KW_IF expr ';'  */
#line 925 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(p)); (yyval.node)=e; }
#line 4487 "raku.tab.c"
    break;

  case 87: /* stmt: KW_PRINT expr KW_UNLESS expr ';'  */
#line 928 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(p)); (yyval.node)=e; }
#line 4494 "raku.tab.c"
    break;

  case 88: /* stmt: VAR_SCALAR OP_ADD_EQ expr ';'  */
#line 931 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_ADD,v,(yyvsp[-1].node))); }
#line 4501 "raku.tab.c"
    break;

  case 89: /* stmt: VAR_SCALAR OP_SUB_EQ expr ';'  */
#line 934 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_SUB,v,(yyvsp[-1].node))); }
#line 4508 "raku.tab.c"
    break;

  case 90: /* stmt: VAR_SCALAR OP_MUL_EQ expr ';'  */
#line 937 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_MUL,v,(yyvsp[-1].node))); }
#line 4515 "raku.tab.c"
    break;

  case 91: /* stmt: VAR_SCALAR OP_DIV_EQ expr ';'  */
#line 940 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_DIV,v,(yyvsp[-1].node))); }
#line 4522 "raku.tab.c"
    break;

  case 92: /* stmt: VAR_SCALAR OP_CAT_EQ expr ';'  */
#line 943 "raku.y"
        { tree_t *v=var_node((yyvsp[-3].sval));
          (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-3].sval)),expr_binary(TT_CAT,v,(yyvsp[-1].node))); }
#line 4529 "raku.tab.c"
    break;

  case 93: /* stmt: expr ';'  */
#line 945 "raku.y"
               { (yyval.node)=(yyvsp[-1].node); }
#line 4535 "raku.tab.c"
    break;

  case 94: /* stmt: ';'  */
#line 946 "raku.y"
          { (yyval.node)=make_seq(exprlist_new()); }
#line 4541 "raku.tab.c"
    break;

  case 95: /* stmt: if_stmt  */
#line 947 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4547 "raku.tab.c"
    break;

  case 96: /* stmt: while_stmt  */
#line 948 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4553 "raku.tab.c"
    break;

  case 97: /* stmt: for_stmt  */
#line 949 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4559 "raku.tab.c"
    break;

  case 98: /* stmt: given_stmt  */
#line 950 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4565 "raku.tab.c"
    break;

  case 99: /* stmt: KW_TRY block  */
#line 952 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4571 "raku.tab.c"
    break;

  case 100: /* stmt: KW_TRY block KW_CATCH block  */
#line 954 "raku.y"
        { tree_t *e=ast_node_new(TT_TRY); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4577 "raku.tab.c"
    break;

  case 101: /* stmt: KW_CATCH '{' catch_when_list '}'  */
#line 956 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-1].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4585 "raku.tab.c"
    break;

  case 102: /* stmt: KW_CATCH '{' catch_when_list KW_DEFAULT block '}'  */
#line 960 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          ExprList *ws=(yyvsp[-3].list); for(int i=0;i<ws->count;i++) expr_add_child(ec,ws->items[i]); exprlist_free(ws);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4594 "raku.tab.c"
    break;

  case 103: /* stmt: KW_CATCH '{' KW_DEFAULT block '}'  */
#line 965 "raku.y"
        { tree_t *ec=ast_node_new(TT_CASE); expr_add_child(ec,leaf_sval(TT_VAR,intern("_")));
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          tree_t *e=ast_node_new(TT_CATCH); ast_push(e,seq1(ec)); (yyval.node)=e; }
#line 4602 "raku.tab.c"
    break;

  case 104: /* stmt: KW_CATCH block  */
#line 969 "raku.y"
        { tree_t *e=ast_node_new(TT_CATCH); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4608 "raku.tab.c"
    break;

  case 105: /* stmt: block  */
#line 971 "raku.y"
        { (yyval.node)=(yyvsp[0].node); }
#line 4614 "raku.tab.c"
    break;

  case 106: /* stmt: PHASER block  */
#line 973 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-1].sval),(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); }
#line 4620 "raku.tab.c"
    break;

  case 107: /* stmt: PHASER block ';'  */
#line 975 "raku.y"
        { (yyval.node)=rk_phaser_mark((yyvsp[-2].sval),(yyvsp[-1].node)); ct_drop((yyvsp[-2].sval)); }
#line 4626 "raku.tab.c"
    break;

  case 108: /* stmt: unless_stmt  */
#line 976 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4632 "raku.tab.c"
    break;

  case 109: /* stmt: until_stmt  */
#line 977 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4638 "raku.tab.c"
    break;

  case 110: /* stmt: repeat_stmt  */
#line 978 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4644 "raku.tab.c"
    break;

  case 111: /* stmt: loop_stmt  */
#line 979 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4650 "raku.tab.c"
    break;

  case 112: /* stmt: KW_LAST ';'  */
#line 980 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_BREAK); }
#line 4656 "raku.tab.c"
    break;

  case 113: /* stmt: KW_NEXT ';'  */
#line 981 "raku.y"
                        { (yyval.node)=ast_node_new(TT_LOOP_NEXT); }
#line 4662 "raku.tab.c"
    break;

  case 114: /* stmt: KW_LAST KW_IF expr ';'  */
#line 983 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4668 "raku.tab.c"
    break;

  case 115: /* stmt: KW_LAST KW_UNLESS expr ';'  */
#line 985 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); (yyval.node)=e; }
#line 4674 "raku.tab.c"
    break;

  case 116: /* stmt: KW_NEXT KW_IF expr ';'  */
#line 987 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4680 "raku.tab.c"
    break;

  case 117: /* stmt: KW_NEXT KW_UNLESS expr ';'  */
#line 989 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); (yyval.node)=e; }
#line 4686 "raku.tab.c"
    break;

  case 118: /* stmt: sub_decl  */
#line 990 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4692 "raku.tab.c"
    break;

  case 119: /* stmt: class_decl  */
#line 991 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4698 "raku.tab.c"
    break;

  case 120: /* stmt: role_decl  */
#line 992 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4704 "raku.tab.c"
    break;

  case 121: /* stmt: grammar_decl  */
#line 993 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4710 "raku.tab.c"
    break;

  case 122: /* stmt: module_decl  */
#line 994 "raku.y"
                        { (yyval.node)=(yyvsp[0].node); }
#line 4716 "raku.tab.c"
    break;

  case 123: /* if_stmt: KW_IF '(' expr ')' block  */
#line 998 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4722 "raku.tab.c"
    break;

  case 124: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE block  */
#line 1000 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4728 "raku.tab.c"
    break;

  case 125: /* if_stmt: KW_IF '(' expr ')' block KW_ELSE if_stmt  */
#line 1002 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4734 "raku.tab.c"
    break;

  case 126: /* if_stmt: KW_IF expr block  */
#line 1004 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4740 "raku.tab.c"
    break;

  case 127: /* if_stmt: KW_IF expr block KW_ELSE block  */
#line 1006 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4746 "raku.tab.c"
    break;

  case 128: /* if_stmt: KW_IF expr block KW_ELSE if_stmt  */
#line 1008 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4752 "raku.tab.c"
    break;

  case 129: /* if_stmt: KW_IF expr block elsif_tail  */
#line 1010 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4758 "raku.tab.c"
    break;

  case 130: /* if_stmt: KW_IF '(' expr ')' block elsif_tail  */
#line 1012 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4764 "raku.tab.c"
    break;

  case 131: /* elsif_tail: KW_ELSIF expr block  */
#line 1016 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4770 "raku.tab.c"
    break;

  case 132: /* elsif_tail: KW_ELSIF '(' expr ')' block  */
#line 1018 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4776 "raku.tab.c"
    break;

  case 133: /* elsif_tail: KW_ELSIF expr block KW_ELSE block  */
#line 1020 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4782 "raku.tab.c"
    break;

  case 134: /* elsif_tail: KW_ELSIF '(' expr ')' block KW_ELSE block  */
#line 1022 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-4].node)); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4788 "raku.tab.c"
    break;

  case 135: /* elsif_tail: KW_ELSIF expr block elsif_tail  */
#line 1024 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4794 "raku.tab.c"
    break;

  case 136: /* elsif_tail: KW_ELSIF '(' expr ')' block elsif_tail  */
#line 1026 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4800 "raku.tab.c"
    break;

  case 137: /* while_stmt: KW_WHILE '(' expr ')' block  */
#line 1030 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4806 "raku.tab.c"
    break;

  case 138: /* while_stmt: KW_WHILE expr block  */
#line 1032 "raku.y"
        { (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,(yyvsp[-1].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4812 "raku.tab.c"
    break;

  case 139: /* unless_stmt: KW_UNLESS '(' expr ')' block  */
#line 1036 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4818 "raku.tab.c"
    break;

  case 140: /* unless_stmt: KW_UNLESS '(' expr ')' block KW_ELSE block  */
#line 1038 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-4].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4824 "raku.tab.c"
    break;

  case 141: /* unless_stmt: KW_UNLESS expr block  */
#line 1040 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4830 "raku.tab.c"
    break;

  case 142: /* unless_stmt: KW_UNLESS expr block KW_ELSE block  */
#line 1042 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-3].node)); ast_push(e,(yyvsp[-2].node)); ast_push(e,(yyvsp[0].node)); (yyval.node)=e; }
#line 4836 "raku.tab.c"
    break;

  case 143: /* until_stmt: KW_UNTIL '(' expr ')' block  */
#line 1046 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-2].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4842 "raku.tab.c"
    break;

  case 144: /* until_stmt: KW_UNTIL expr block  */
#line 1048 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,(yyvsp[0].node)); (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4848 "raku.tab.c"
    break;

  case 145: /* repeat_stmt: KW_REPEAT block  */
#line 1052 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[0].node)); e->v.ival=0; (yyval.node)=rk_loop_phasers(e,(yyvsp[0].node)); }
#line 4854 "raku.tab.c"
    break;

  case 146: /* repeat_stmt: KW_REPEAT block KW_WHILE expr ';'  */
#line 1054 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=1; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4860 "raku.tab.c"
    break;

  case 147: /* repeat_stmt: KW_REPEAT block KW_UNTIL expr ';'  */
#line 1056 "raku.y"
        { tree_t *e=ast_node_new(TT_REPEAT); expr_add_child(e,(yyvsp[-3].node)); expr_add_child(e,(yyvsp[-1].node)); e->v.ival=2; (yyval.node)=rk_loop_phasers(e,(yyvsp[-3].node)); }
#line 4866 "raku.tab.c"
    break;

  case 148: /* loop_stmt: KW_LOOP block  */
#line 1060 "raku.y"
        { tree_t *one=ast_node_new(TT_ILIT); one->v.ival=1; (yyval.node)=rk_loop_phasers(expr_binary(TT_WHILE,one,(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4872 "raku.tab.c"
    break;

  case 149: /* loop_stmt: KW_LOOP '(' KW_MY VAR_SCALAR '=' expr ';' expr ';' loop_incr ')' block  */
#line 1062 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop(expr_binary(TT_ASSIGN,var_node((yyvsp[-8].sval)),(yyvsp[-6].node)),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4878 "raku.tab.c"
    break;

  case 150: /* loop_stmt: KW_LOOP '(' expr ';' expr ';' loop_incr ')' block  */
#line 1064 "raku.y"
        { (yyval.node)=rk_loop_phasers(rk_cstyle_loop((yyvsp[-6].node),(yyvsp[-4].node),(yyvsp[-2].node),(yyvsp[0].node)),(yyvsp[0].node)); }
#line 4884 "raku.tab.c"
    break;

  case 151: /* loop_incr: expr  */
#line 1067 "raku.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 4890 "raku.tab.c"
    break;

  case 152: /* for_stmt: KW_FOR add_expr OP_RANGE add_expr OP_ARROW VAR_SCALAR block  */
#line 1071 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, (yyvsp[-3].node)); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 4900 "raku.tab.c"
    break;

  case 153: /* for_stmt: KW_FOR add_expr OP_RANGE_EX add_expr OP_ARROW VAR_SCALAR block  */
#line 1077 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *r = ast_node_new(TT_FOR_RANGE);
          ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, (yyvsp[-5].node)); ast_push(r, rk_dec((yyvsp[-3].node))); ast_push(r, (yyvsp[0].node));
          tree_t *ex = ast_node_new(TT_ILIT); ex->v.ival = 0; ast_push(r, ex);
          (yyval.node) = rk_loop_phasers(r, (yyvsp[0].node)); }
#line 4910 "raku.tab.c"
    break;

  case 154: /* for_stmt: KW_FOR expr OP_ARROW scalar_list block  */
#line 1083 "raku.y"
        { ExprList *vs = (yyvsp[-1].list);
          if (vs && vs->count == 1) {
              const char *vn = vs->items[0]->v.sval; exprlist_free(vs);
              tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-3].node));
              gen->v.sval = (char *)vn;
              (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node));
          } else (yyval.node) = rk_loop_phasers(rk_for_multi(vs, (yyvsp[-3].node), (yyvsp[0].node)), (yyvsp[0].node)); }
#line 4922 "raku.tab.c"
    break;

  case 155: /* for_stmt: KW_FOR expr ',' arg_list OP_ARROW VAR_SCALAR block  */
#line 1091 "raku.y"
        { const char *vn = intern(strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-5].node));
          ExprList *a=(yyvsp[-3].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst); gen->v.sval = (char *)vn;
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 4932 "raku.tab.c"
    break;

  case 156: /* for_stmt: KW_FOR expr ',' arg_list block  */
#line 1097 "raku.y"
        { tree_t *lst = make_call("__rk_arr"); expr_add_child(lst,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(lst,a->items[i]); exprlist_free(a); }
          tree_t *gen = expr_unary(TT_ITERATE, lst);
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 4941 "raku.tab.c"
    break;

  case 157: /* for_stmt: KW_FOR expr block  */
#line 1102 "raku.y"
        { tree_t *gen = expr_unary(TT_ITERATE, (yyvsp[-1].node));
          (yyval.node) = rk_loop_phasers(expr_binary(TT_EVERY, gen, (yyvsp[0].node)), (yyvsp[0].node)); }
#line 4948 "raku.tab.c"
    break;

  case 158: /* given_stmt: KW_GIVEN expr '{' when_list '}'  */
#line 1107 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-3].node));
          ExprList *whens=(yyvsp[-1].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          (yyval.node)=ec; }
#line 4960 "raku.tab.c"
    break;

  case 159: /* given_stmt: KW_GIVEN expr '{' when_list KW_DEFAULT block '}'  */
#line 1115 "raku.y"
        {
          tree_t *ec=ast_node_new(TT_CASE);
          expr_add_child(ec,(yyvsp[-5].node));
          ExprList *whens=(yyvsp[-3].list);
          for(int i=0;i<whens->count;i++) expr_add_child(ec,whens->items[i]);
          exprlist_free(whens);
          expr_add_child(ec,ast_node_new(TT_NUL)); expr_add_child(ec,(yyvsp[-1].node));
          (yyval.node)=ec; }
#line 4973 "raku.tab.c"
    break;

  case 160: /* catch_when_list: KW_WHEN expr block  */
#line 1126 "raku.y"
        { ExprList *l=exprlist_new(); exprlist_append(l,rk_catch_when_cond((yyvsp[-1].node))); exprlist_append(l,(yyvsp[0].node)); (yyval.list)=l; }
#line 4979 "raku.tab.c"
    break;

  case 161: /* catch_when_list: catch_when_list KW_WHEN expr block  */
#line 1128 "raku.y"
        { exprlist_append((yyvsp[-3].list),rk_catch_when_cond((yyvsp[-1].node))); exprlist_append((yyvsp[-3].list),(yyvsp[0].node)); (yyval.list)=(yyvsp[-3].list); }
#line 4985 "raku.tab.c"
    break;

  case 162: /* when_list: %empty  */
#line 1131 "raku.y"
       { (yyval.list)=exprlist_new(); }
#line 4991 "raku.tab.c"
    break;

  case 163: /* when_list: when_list KW_WHEN expr block  */
#line 1133 "raku.y"
        {
          exprlist_append((yyvsp[-3].list),(yyvsp[-1].node)); exprlist_append((yyvsp[-3].list),(yyvsp[0].node));
          (yyval.list)=(yyvsp[-3].list); }
#line 4999 "raku.tab.c"
    break;

  case 164: /* sub_trait_list: TESTOP IDENT  */
#line 1138 "raku.y"
                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5005 "raku.tab.c"
    break;

  case 165: /* sub_trait_list: sub_trait_list TESTOP IDENT  */
#line 1139 "raku.y"
                                  { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); }
#line 5011 "raku.tab.c"
    break;

  case 166: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1143 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5023 "raku.tab.c"
    break;

  case 167: /* sub_decl: KW_SUB IDENT '(' ')' sub_body  */
#line 1151 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5033 "raku.tab.c"
    break;

  case 168: /* sub_decl: KW_SUB IDENT sub_body  */
#line 1157 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5043 "raku.tab.c"
    break;

  case 169: /* sub_decl: KW_SUB IDENT '(' param_list ')' sub_trait_list sub_body  */
#line 1163 "raku.y"
        { ExprList *params=(yyvsp[-3].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-5].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-5].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5055 "raku.tab.c"
    break;

  case 170: /* sub_decl: KW_SUB IDENT '(' ')' sub_trait_list sub_body  */
#line 1171 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5065 "raku.tab.c"
    break;

  case 171: /* sub_decl: KW_MY KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1177 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-4].sval)); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-4].sval)); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5077 "raku.tab.c"
    break;

  case 172: /* sub_decl: KW_MY KW_SUB IDENT '(' ')' sub_body  */
#line 1185 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-3].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-3].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5087 "raku.tab.c"
    break;

  case 173: /* sub_decl: KW_MY KW_SUB IDENT sub_body  */
#line 1191 "raku.y"
        { tree_t *e=leaf_sval(TT_SUB_DECL,(yyvsp[-1].sval)); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern((yyvsp[-1].sval)); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          (yyval.node)=e; }
#line 5097 "raku.tab.c"
    break;

  case 174: /* sub_decl: KW_MULTI KW_SUB IDENT '(' param_list ')' sub_body  */
#line 1197 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5110 "raku.tab.c"
    break;

  case 175: /* sub_decl: KW_MULTI KW_SUB IDENT '(' ')' sub_body  */
#line 1206 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5121 "raku.tab.c"
    break;

  case 176: /* sub_decl: KW_MULTI KW_SUB OP_NAME '(' param_list ')' sub_body  */
#line 1213 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5134 "raku.tab.c"
    break;

  case 177: /* sub_decl: KW_MULTI IDENT '(' param_list ')' sub_body  */
#line 1222 "raku.y"
        { ExprList *params=(yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np=params?params->count:0;
          const char *mname=rk_multi_mangle((yyvsp[-4].sval),params);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)np;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          if(params){ for(int i=0;i<np;i++) expr_add_child(e,params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-4].sval)); (yyval.node)=e; }
#line 5147 "raku.tab.c"
    break;

  case 178: /* sub_decl: KW_MULTI IDENT '(' ')' sub_body  */
#line 1231 "raku.y"
        { const char *mname=rk_multi_mangle((yyvsp[-3].sval),NULL);
          tree_t *e=leaf_sval(TT_SUB_DECL,mname); e->v.ival=(long long)0;
          tree_t *nn=ast_node_new(TT_VAR); nn->v.sval=intern(mname); expr_add_child(e,nn);
          tree_t *body=(yyvsp[0].node);
          for(int i=0;i<body->n;i++) expr_add_child(e,body->c[i]);
          ct_drop((yyvsp[-3].sval)); (yyval.node)=e; }
#line 5158 "raku.tab.c"
    break;

  case 179: /* sub_body: '{' stmt_list '}'  */
#line 1239 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 5164 "raku.tab.c"
    break;

  case 180: /* sub_body: '{' stmt_list expr '}'  */
#line 1241 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5171 "raku.tab.c"
    break;

  case 181: /* sub_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1244 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5178 "raku.tab.c"
    break;

  case 182: /* sub_body: '{' stmt_list KW_RETURN '}'  */
#line 1247 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5184 "raku.tab.c"
    break;

  case 183: /* sub_body: '{' stmt_list KW_SAY expr '}'  */
#line 1249 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5190 "raku.tab.c"
    break;

  case 184: /* sub_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1251 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5196 "raku.tab.c"
    break;

  case 185: /* sub_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1253 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5202 "raku.tab.c"
    break;

  case 186: /* sub_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1255 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5208 "raku.tab.c"
    break;

  case 187: /* sub_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1257 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5214 "raku.tab.c"
    break;

  case 188: /* sub_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1259 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5220 "raku.tab.c"
    break;

  case 189: /* sub_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1261 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5227 "raku.tab.c"
    break;

  case 190: /* sub_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1264 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5233 "raku.tab.c"
    break;

  case 191: /* sub_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1266 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5239 "raku.tab.c"
    break;

  case 192: /* sub_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1268 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5245 "raku.tab.c"
    break;

  case 193: /* sub_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1270 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5252 "raku.tab.c"
    break;

  case 194: /* sub_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1273 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5259 "raku.tab.c"
    break;

  case 195: /* sub_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1276 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5267 "raku.tab.c"
    break;

  case 196: /* method_body: '{' stmt_list '}'  */
#line 1281 "raku.y"
                                 { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 5273 "raku.tab.c"
    break;

  case 197: /* method_body: '{' YADA '}'  */
#line 1282 "raku.y"
                                 { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 5279 "raku.tab.c"
    break;

  case 198: /* method_body: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1284 "raku.y"
        { tree_t *fe=rk_tw_field((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval));
          ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node))); (yyval.node)=make_seq(l); }
#line 5286 "raku.tab.c"
    break;

  case 199: /* method_body: '{' stmt_list expr '}'  */
#line 1287 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5293 "raku.tab.c"
    break;

  case 200: /* method_body: '{' stmt_list KW_RETURN expr '}'  */
#line 1290 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5300 "raku.tab.c"
    break;

  case 201: /* method_body: '{' stmt_list KW_RETURN '}'  */
#line 1293 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 5306 "raku.tab.c"
    break;

  case 202: /* method_body: '{' stmt_list KW_SAY expr '}'  */
#line 1295 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 5312 "raku.tab.c"
    break;

  case 203: /* method_body: '{' stmt_list KW_PRINT expr '}'  */
#line 1297 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 5318 "raku.tab.c"
    break;

  case 204: /* method_body: '{' stmt_list expr KW_IF expr '}'  */
#line 1299 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5324 "raku.tab.c"
    break;

  case 205: /* method_body: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1301 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5330 "raku.tab.c"
    break;

  case 206: /* method_body: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1303 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5336 "raku.tab.c"
    break;

  case 207: /* method_body: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1305 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5342 "raku.tab.c"
    break;

  case 208: /* method_body: '{' stmt_list expr KW_FOR expr '}'  */
#line 1307 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5349 "raku.tab.c"
    break;

  case 209: /* method_body: '{' stmt_list expr KW_WITH expr '}'  */
#line 1310 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5355 "raku.tab.c"
    break;

  case 210: /* method_body: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1312 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5361 "raku.tab.c"
    break;

  case 211: /* method_body: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1314 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5367 "raku.tab.c"
    break;

  case 212: /* method_body: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1316 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5374 "raku.tab.c"
    break;

  case 213: /* method_body: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1319 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5381 "raku.tab.c"
    break;

  case 214: /* method_body: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1322 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 5389 "raku.tab.c"
    break;

  case 215: /* pkg_name: IDENT  */
#line 1327 "raku.y"
             { (yyval.sval)=(yyvsp[0].sval); }
#line 5395 "raku.tab.c"
    break;

  case 216: /* class_decl: KW_CLASS pkg_name is_clauses '{' class_body_list '}'  */
#line 1331 "raku.y"
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
#line 5413 "raku.tab.c"
    break;

  case 217: /* role_decl: KW_ROLE pkg_name '{' class_body_list '}'  */
#line 1347 "raku.y"
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
#line 5430 "raku.tab.c"
    break;

  case 218: /* module_decl: KW_MODULE pkg_name '{' stmt_list '}'  */
#line 1362 "raku.y"
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
#line 5447 "raku.tab.c"
    break;

  case 219: /* is_clauses: %empty  */
#line 1376 "raku.y"
       { (yyval.sval) = (char *)0; }
#line 5453 "raku.tab.c"
    break;

  case 220: /* is_clauses: is_clauses IDENT IDENT  */
#line 1378 "raku.y"
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
#line 5469 "raku.tab.c"
    break;

  case 221: /* is_clauses: is_clauses TESTOP IDENT  */
#line 1390 "raku.y"
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
#line 5485 "raku.tab.c"
    break;

  case 222: /* class_body_list: %empty  */
#line 1403 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5491 "raku.tab.c"
    break;

  case 223: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL ';'  */
#line 1405 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5498 "raku.tab.c"
    break;

  case 224: /* class_body_list: class_body_list KW_HAS VAR_ARRAY_TWIGIL ';'  */
#line 1408 "raku.y"
        { tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5505 "raku.tab.c"
    break;

  case 225: /* class_body_list: class_body_list KW_HAS VAR_HASH_TWIGIL ';'  */
#line 1411 "raku.y"
        { tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5512 "raku.tab.c"
    break;

  case 226: /* class_body_list: class_body_list KW_HAS IDENT VAR_ARRAY_TWIGIL ';'  */
#line 1414 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_ARR_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5519 "raku.tab.c"
    break;

  case 227: /* class_body_list: class_body_list KW_HAS IDENT VAR_HASH_TWIGIL ';'  */
#line 1417 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HASH_DECL); fv->v.sval = (char *)intern((yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5526 "raku.tab.c"
    break;

  case 228: /* class_body_list: class_body_list KW_HAS VAR_SCALAR ';'  */
#line 1420 "raku.y"
        { tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), fv); }
#line 5533 "raku.tab.c"
    break;

  case 229: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL ';'  */
#line 1423 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, (yyvsp[-1].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5540 "raku.tab.c"
    break;

  case 230: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR ';'  */
#line 1426 "raku.y"
        { ct_drop((yyvsp[-2].sval)); tree_t *fv = leaf_sval(TT_VAR, strip_sigil((yyvsp[-1].sval))); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5547 "raku.tab.c"
    break;

  case 231: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL '=' expr ';'  */
#line 1429 "raku.y"
        { tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5554 "raku.tab.c"
    break;

  case 232: /* class_body_list: class_body_list KW_HAS VAR_SCALAR '=' expr ';'  */
#line 1432 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5561 "raku.tab.c"
    break;

  case 233: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL '=' expr ';'  */
#line 1435 "raku.y"
        { ct_drop((yyvsp[-4].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5568 "raku.tab.c"
    break;

  case 234: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR '=' expr ';'  */
#line 1438 "raku.y"
        { ct_drop((yyvsp[-4].sval)); const char *fn = strip_sigil((yyvsp[-3].sval)); tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); ct_drop((yyvsp[-3].sval)); expr_add_child(fv, (yyvsp[-1].node));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5575 "raku.tab.c"
    break;

  case 235: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT ';'  */
#line 1441 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5586 "raku.tab.c"
    break;

  case 236: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL IDENT IDENT '=' expr ';'  */
#line 1448 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5596 "raku.tab.c"
    break;

  case 237: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT '=' expr ';'  */
#line 1454 "raku.y"
        { const char *an = intern((yyvsp[-5].sval)); ExprList *l = (yyvsp[-7].list);
          if ((yyvsp[-4].sval) && !strcmp((yyvsp[-4].sval), "is") && (yyvsp[-3].sval) && !strcmp((yyvsp[-3].sval), "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *)an; l = exprlist_append(l, rw); }
          tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)an; expr_add_child(fv, (yyvsp[-1].node));
          ct_drop((yyvsp[-5].sval)); ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append(l, fv); }
#line 5606 "raku.tab.c"
    break;

  case 238: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL TESTOP IDENT ';'  */
#line 1460 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5617 "raku.tab.c"
    break;

  case 239: /* class_body_list: class_body_list KW_HAS VAR_SCALAR IDENT IDENT ';'  */
#line 1467 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5628 "raku.tab.c"
    break;

  case 240: /* class_body_list: class_body_list KW_HAS VAR_SCALAR TESTOP IDENT ';'  */
#line 1474 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5639 "raku.tab.c"
    break;

  case 241: /* class_body_list: class_body_list KW_HAS VAR_SCALAR KW_HANDLES ';'  */
#line 1481 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5647 "raku.tab.c"
    break;

  case 242: /* class_body_list: class_body_list KW_HAS VAR_TWIGIL KW_HANDLES ';'  */
#line 1485 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), fv); }
#line 5655 "raku.tab.c"
    break;

  case 243: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR KW_HANDLES ';'  */
#line 1489 "raku.y"
        { const char *fn = strip_sigil((yyvsp[-2].sval)); tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern(fn);
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5663 "raku.tab.c"
    break;

  case 244: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL KW_HANDLES ';'  */
#line 1493 "raku.y"
        { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *)intern((yyvsp[-2].sval));
          expr_add_child(fv, leaf_sval(TT_QLIT, (yyvsp[-1].sval))); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), fv); }
#line 5671 "raku.tab.c"
    break;

  case 245: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL IDENT IDENT ';'  */
#line 1497 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5682 "raku.tab.c"
    break;

  case 246: /* class_body_list: class_body_list KW_HAS IDENT VAR_TWIGIL TESTOP IDENT ';'  */
#line 1504 "raku.y"
        { tree_t *fv;
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern((yyvsp[-3].sval)); }
          else fv = leaf_sval(TT_VAR, (yyvsp[-3].sval));
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5693 "raku.tab.c"
    break;

  case 247: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR IDENT IDENT ';'  */
#line 1511 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5704 "raku.tab.c"
    break;

  case 248: /* class_body_list: class_body_list KW_HAS IDENT VAR_SCALAR TESTOP IDENT ';'  */
#line 1518 "raku.y"
        { tree_t *fv; const char *fn = strip_sigil((yyvsp[-3].sval));
          if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *)intern(fn); }
          else if ((yyvsp[-2].sval) && !strcmp((yyvsp[-2].sval), "is") && (yyvsp[-1].sval) && !strcmp((yyvsp[-1].sval), "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *)intern(fn); }
          else fv = leaf_sval(TT_VAR, fn);
          ct_drop((yyvsp[-4].sval)); ct_drop((yyvsp[-3].sval)); ct_drop((yyvsp[-2].sval)); ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), fv); }
#line 5715 "raku.tab.c"
    break;

  case 249: /* class_body_list: class_body_list KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1525 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-4].sval)); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5729 "raku.tab.c"
    break;

  case 250: /* class_body_list: class_body_list KW_METHOD meth_name '(' ')' method_body  */
#line 1535 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-3].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5741 "raku.tab.c"
    break;

  case 251: /* class_body_list: class_body_list KW_METHOD meth_name method_body  */
#line 1543 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern((yyvsp[-1].sval)); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5753 "raku.tab.c"
    break;

  case 252: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1551 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5766 "raku.tab.c"
    break;

  case 253: /* class_body_list: class_body_list KW_METHOD KW_NEW '(' ')' method_body  */
#line 1560 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-5].list), e); }
#line 5777 "raku.tab.c"
    break;

  case 254: /* class_body_list: class_body_list KW_METHOD KW_NEW method_body  */
#line 1567 "raku.y"
        { tree_t *e = ast_node_new(TT_SUB_DECL);
          e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern("new"); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-3].list), e); }
#line 5788 "raku.tab.c"
    break;

  case 255: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' param_list ')' method_body  */
#line 1574 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle((yyvsp[-4].sval), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-4].sval));
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5802 "raku.tab.c"
    break;

  case 256: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name '(' ')' method_body  */
#line 1584 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-3].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-3].sval));
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5814 "raku.tab.c"
    break;

  case 257: /* class_body_list: class_body_list KW_MULTI KW_METHOD meth_name method_body  */
#line 1592 "raku.y"
        { const char *mname = rk_multi_mangle((yyvsp[-1].sval), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          ct_drop((yyvsp[-1].sval));
          (yyval.list) = exprlist_append((yyvsp[-4].list), e); }
#line 5826 "raku.tab.c"
    break;

  case 258: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' param_list ')' method_body  */
#line 1600 "raku.y"
        { ExprList *params = (yyvsp[-2].list); tree_t *rkbody=rk_defaults_prologue(params,(yyvsp[0].node)); int np = params ? params->count : 0;
          const char *mname = rk_multi_mangle(ct_strdup("new"), params);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(np + 1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          if (params) { for (int i = 0; i < np; i++) expr_add_child(e, params->items[i]); exprlist_free(params); }
          tree_t *body=rkbody;
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-7].list), e); }
#line 5839 "raku.tab.c"
    break;

  case 259: /* class_body_list: class_body_list KW_MULTI KW_METHOD KW_NEW '(' ')' method_body  */
#line 1609 "raku.y"
        { const char *mname = rk_multi_mangle(ct_strdup("new"), NULL);
          tree_t *e = ast_node_new(TT_SUB_DECL); e->v.ival = (long long)(1);
          tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mname); expr_add_child(e, nn);
          tree_t *body = (yyvsp[0].node);
          for (int i = 0; i < body->n; i++) expr_add_child(e, body->c[i]);
          (yyval.list) = exprlist_append((yyvsp[-6].list), e); }
#line 5850 "raku.tab.c"
    break;

  case 260: /* grammar_decl: KW_GRAMMAR pkg_name '{' grammar_body_list '}'  */
#line 1618 "raku.y"
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
#line 5867 "raku.tab.c"
    break;

  case 261: /* grammar_body_list: %empty  */
#line 1632 "raku.y"
       { (yyval.list) = exprlist_new(); }
#line 5873 "raku.tab.c"
    break;

  case 262: /* grammar_body_list: grammar_body_list KW_TOKEN IDENT LIT_REGEX  */
#line 1634 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 0;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5882 "raku.tab.c"
    break;

  case 263: /* grammar_body_list: grammar_body_list KW_RULE IDENT LIT_REGEX  */
#line 1639 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 1;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5891 "raku.tab.c"
    break;

  case 264: /* grammar_body_list: grammar_body_list KW_REGEX IDENT LIT_REGEX  */
#line 1644 "raku.y"
        { tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = 2;
          ast_push(rd, leaf_sval(TT_VAR, intern((yyvsp[-1].sval)))); ct_drop((yyvsp[-1].sval));
          ast_push(rd, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          (yyval.list) = exprlist_append((yyvsp[-3].list), rd); }
#line 5900 "raku.tab.c"
    break;

  case 265: /* named_arg_list: IDENT OP_FATARROW expr  */
#line 1651 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyval.list), (yyvsp[0].node)); }
#line 5908 "raku.tab.c"
    break;

  case 266: /* named_arg_list: ':' IDENT '(' expr ')'  */
#line 1655 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyval.list), (yyvsp[-1].node)); }
#line 5916 "raku.tab.c"
    break;

  case 267: /* named_arg_list: ':' IDENT  */
#line 1659 "raku.y"
        { (yyval.list) = exprlist_new();
          exprlist_append((yyval.list), leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *)"__rk_mkbool";
          tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *)"__rk_mkbool"; ast_push(tb, tn);
          tree_t *one = ast_node_new(TT_ILIT); one->v.ival = 1; ast_push(tb, one);
          exprlist_append((yyval.list), tb); }
#line 5927 "raku.tab.c"
    break;

  case 268: /* named_arg_list: named_arg_list ',' IDENT OP_FATARROW expr  */
#line 1666 "raku.y"
        { exprlist_append((yyvsp[-4].list), leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          exprlist_append((yyvsp[-4].list), (yyvsp[0].node));
          (yyval.list) = (yyvsp[-4].list); }
#line 5935 "raku.tab.c"
    break;

  case 269: /* named_arg_list: named_arg_list ',' ':' IDENT '(' expr ')'  */
#line 1670 "raku.y"
        { exprlist_append((yyvsp[-6].list), leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          exprlist_append((yyvsp[-6].list), (yyvsp[-1].node));
          (yyval.list) = (yyvsp[-6].list); }
#line 5943 "raku.tab.c"
    break;

  case 270: /* pair_list: IDENT OP_FATARROW expr  */
#line 1676 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5949 "raku.tab.c"
    break;

  case 271: /* pair_list: LIT_STR OP_FATARROW expr  */
#line 1678 "raku.y"
        { tree_t *c=make_call("__rk_hash"); expr_add_child(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 5955 "raku.tab.c"
    break;

  case 272: /* pair_list: pair_list ',' IDENT OP_FATARROW expr  */
#line 1680 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval)); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5961 "raku.tab.c"
    break;

  case 273: /* pair_list: pair_list ',' LIT_STR OP_FATARROW expr  */
#line 1682 "raku.y"
        { expr_add_child((yyvsp[-4].node),leaf_sval(TT_QLIT,(yyvsp[-2].sval))); expr_add_child((yyvsp[-4].node),(yyvsp[0].node)); (yyval.node)=(yyvsp[-4].node); }
#line 5967 "raku.tab.c"
    break;

  case 274: /* param_list: VAR_SCALAR  */
#line 1685 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[0].sval))); }
#line 5973 "raku.tab.c"
    break;

  case 275: /* param_list: VAR_SCALAR TESTOP IDENT  */
#line 1686 "raku.y"
                              { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),var_node((yyvsp[-2].sval))); }
#line 5979 "raku.tab.c"
    break;

  case 276: /* param_list: param_list ',' VAR_SCALAR TESTOP IDENT  */
#line 1687 "raku.y"
                                             { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-4].list),var_node((yyvsp[-2].sval))); }
#line 5985 "raku.tab.c"
    break;

  case 277: /* param_list: IDENT VAR_SCALAR TESTOP IDENT  */
#line 1688 "raku.y"
                                    { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 5991 "raku.tab.c"
    break;

  case 278: /* param_list: param_list ',' IDENT VAR_SCALAR TESTOP IDENT  */
#line 1689 "raku.y"
                                                   { ct_drop((yyvsp[-1].sval)); ct_drop((yyvsp[0].sval)); (yyval.list)=exprlist_append((yyvsp[-5].list),rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval))); ct_drop((yyvsp[-3].sval)); }
#line 5997 "raku.tab.c"
    break;

  case 279: /* param_list: VAR_ARRAY  */
#line 1690 "raku.y"
                              { (yyval.list)=exprlist_append(exprlist_new(),rk_byref_param((yyvsp[0].sval))); }
#line 6003 "raku.tab.c"
    break;

  case 280: /* param_list: param_list ',' VAR_ARRAY  */
#line 1691 "raku.y"
                               { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_byref_param((yyvsp[0].sval))); }
#line 6009 "raku.tab.c"
    break;

  case 281: /* param_list: IDENT VAR_SCALAR  */
#line 1692 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6015 "raku.tab.c"
    break;

  case 282: /* param_list: IDENT OP_COLON_D VAR_SCALAR  */
#line 1693 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6021 "raku.tab.c"
    break;

  case 283: /* param_list: IDENT OP_COLON_U VAR_SCALAR  */
#line 1694 "raku.y"
                                  { (yyval.list)=exprlist_append(exprlist_new(),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6027 "raku.tab.c"
    break;

  case 284: /* param_list: param_list ',' VAR_SCALAR  */
#line 1695 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),var_node((yyvsp[0].sval))); }
#line 6033 "raku.tab.c"
    break;

  case 285: /* param_list: param_list ',' IDENT VAR_SCALAR  */
#line 1696 "raku.y"
                                      { (yyval.list)=exprlist_append((yyvsp[-3].list),rk_typed_param((yyvsp[-1].sval),(yyvsp[0].sval))); ct_drop((yyvsp[-1].sval)); }
#line 6039 "raku.tab.c"
    break;

  case 286: /* param_list: param_list ',' IDENT OP_COLON_D VAR_SCALAR  */
#line 1697 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":D",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6045 "raku.tab.c"
    break;

  case 287: /* param_list: param_list ',' IDENT OP_COLON_U VAR_SCALAR  */
#line 1698 "raku.y"
                                                 { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_typed_def_param((yyvsp[-2].sval),":U",(yyvsp[0].sval))); ct_drop((yyvsp[-2].sval)); }
#line 6051 "raku.tab.c"
    break;

  case 288: /* param_list: VAR_SCALAR '=' expr  */
#line 1699 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6057 "raku.tab.c"
    break;

  case 289: /* param_list: param_list ',' VAR_SCALAR '=' expr  */
#line 1700 "raku.y"
                                         { (yyval.list)=exprlist_append((yyvsp[-4].list),rk_param_default(var_node((yyvsp[-2].sval)),(yyvsp[0].node))); }
#line 6063 "raku.tab.c"
    break;

  case 290: /* param_list: IDENT VAR_SCALAR '=' expr  */
#line 1701 "raku.y"
                                { (yyval.list)=exprlist_append(exprlist_new(),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6069 "raku.tab.c"
    break;

  case 291: /* param_list: param_list ',' IDENT VAR_SCALAR '=' expr  */
#line 1702 "raku.y"
                                               { (yyval.list)=exprlist_append((yyvsp[-5].list),rk_param_default(rk_typed_param((yyvsp[-3].sval),(yyvsp[-2].sval)),(yyvsp[0].node))); ct_drop((yyvsp[-3].sval)); }
#line 6075 "raku.tab.c"
    break;

  case 292: /* param_list: SLURPY_POS  */
#line 1703 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_param((yyvsp[0].sval))); }
#line 6081 "raku.tab.c"
    break;

  case 293: /* param_list: param_list ',' SLURPY_POS  */
#line 1704 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_param((yyvsp[0].sval))); }
#line 6087 "raku.tab.c"
    break;

  case 294: /* param_list: SLURPY_LOL  */
#line 1705 "raku.y"
                             { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6093 "raku.tab.c"
    break;

  case 295: /* param_list: param_list ',' SLURPY_LOL  */
#line 1706 "raku.y"
                                { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_lol_param((yyvsp[0].sval))); }
#line 6099 "raku.tab.c"
    break;

  case 296: /* param_list: SLURPY_NAMED  */
#line 1707 "raku.y"
                               { (yyval.list)=exprlist_append(exprlist_new(),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6105 "raku.tab.c"
    break;

  case 297: /* param_list: param_list ',' SLURPY_NAMED  */
#line 1708 "raku.y"
                                  { (yyval.list)=exprlist_append((yyvsp[-2].list),rk_slurpy_named_param((yyvsp[0].sval))); }
#line 6111 "raku.tab.c"
    break;

  case 298: /* block: '{' stmt_list '}'  */
#line 1711 "raku.y"
                         { (yyval.node)=make_seq(rk_phasers_place((yyvsp[-1].list),0)); }
#line 6117 "raku.tab.c"
    break;

  case 299: /* block: '{' YADA '}'  */
#line 1712 "raku.y"
                         { ExprList *l = exprlist_new(); exprlist_append(l, ast_node_new(TT_YADA)); (yyval.node)=make_seq(l); }
#line 6123 "raku.tab.c"
    break;

  case 300: /* block: '{' stmt_list expr '}'  */
#line 1714 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,(yyvsp[-1].node)); (yyval.node)=make_seq(l); }
#line 6129 "raku.tab.c"
    break;

  case 301: /* block: '{' stmt_list expr KW_IF expr '}'  */
#line 1716 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6135 "raku.tab.c"
    break;

  case 302: /* block: '{' stmt_list expr KW_UNLESS expr '}'  */
#line 1718 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6141 "raku.tab.c"
    break;

  case 303: /* block: '{' stmt_list expr KW_WHILE expr '}'  */
#line 1720 "raku.y"
        { tree_t *e=expr_binary(TT_WHILE,(yyvsp[-1].node),seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6147 "raku.tab.c"
    break;

  case 304: /* block: '{' stmt_list expr KW_UNTIL expr '}'  */
#line 1722 "raku.y"
        { tree_t *e=ast_node_new(TT_UNTIL); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6153 "raku.tab.c"
    break;

  case 305: /* block: '{' stmt_list expr KW_FOR expr '}'  */
#line 1724 "raku.y"
        { tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1((yyvsp[-3].node))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6160 "raku.tab.c"
    break;

  case 306: /* block: '{' stmt_list expr KW_WITH expr '}'  */
#line 1727 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6166 "raku.tab.c"
    break;

  case 307: /* block: '{' stmt_list expr KW_WITHOUT expr '}'  */
#line 1729 "raku.y"
        { tree_t *e=rk_with_mod((yyvsp[-3].node),(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6172 "raku.tab.c"
    break;

  case 308: /* block: '{' stmt_list expr KW_GIVEN expr '}'  */
#line 1731 "raku.y"
        { tree_t *e=rk_given_mod((yyvsp[-3].node),(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6178 "raku.tab.c"
    break;

  case 309: /* block: '{' stmt_list KW_SAY expr '}'  */
#line 1733 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,s); (yyval.node)=make_seq(l); }
#line 6184 "raku.tab.c"
    break;

  case 310: /* block: '{' stmt_list KW_SAY expr KW_IF expr '}'  */
#line 1735 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6191 "raku.tab.c"
    break;

  case 311: /* block: '{' stmt_list KW_SAY expr KW_UNLESS expr '}'  */
#line 1738 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6198 "raku.tab.c"
    break;

  case 312: /* block: '{' stmt_list KW_SAY expr KW_FOR expr '}'  */
#line 1741 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *gen=expr_unary(TT_ITERATE,(yyvsp[-1].node)); gen->v.sval=(char*)intern("_");
          tree_t *e=expr_binary(TT_EVERY,gen,seq1(s)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6206 "raku.tab.c"
    break;

  case 313: /* block: '{' stmt_list KW_SAY expr KW_WITH expr '}'  */
#line 1745 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),0); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6213 "raku.tab.c"
    break;

  case 314: /* block: '{' stmt_list KW_SAY expr KW_WITHOUT expr '}'  */
#line 1748 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_with_mod(s,(yyvsp[-1].node),1); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6220 "raku.tab.c"
    break;

  case 315: /* block: '{' stmt_list KW_SAY expr KW_GIVEN expr '}'  */
#line 1751 "raku.y"
        { tree_t *s=ast_node_new(TT_SAY); expr_add_child(s,(yyvsp[-3].node));
          tree_t *e=rk_given_mod(s,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-5].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6227 "raku.tab.c"
    break;

  case 316: /* block: '{' stmt_list KW_PRINT expr '}'  */
#line 1754 "raku.y"
        { tree_t *p=ast_node_new(TT_PRINT); expr_add_child(p,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,p); (yyval.node)=make_seq(l); }
#line 6233 "raku.tab.c"
    break;

  case 317: /* block: '{' stmt_list call_expr '.' meth_name '=' expr '}'  */
#line 1756 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6240 "raku.tab.c"
    break;

  case 318: /* block: '{' stmt_list atom '.' meth_name '=' expr '}'  */
#line 1759 "raku.y"
        { tree_t *fe=ast_node_new(TT_FIELD); fe->v.sval=(char*)intern((yyvsp[-3].sval)); ct_drop((yyvsp[-3].sval)); expr_add_child(fe,(yyvsp[-5].node));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-6].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6247 "raku.tab.c"
    break;

  case 319: /* block: '{' stmt_list VAR_TWIGIL '=' expr '}'  */
#line 1762 "raku.y"
        { tree_t *fe=ast_node_new(TT_TWIGIL_FIELD); fe->v.sval=(char*)intern(rk_tw_bare((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          tree_t *a=expr_binary(TT_ASSIGN,fe,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,a); (yyval.node)=make_seq(l); }
#line 6254 "raku.tab.c"
    break;

  case 320: /* block: '{' stmt_list VAR_ARRAY '[' expr ']' '=' expr '}'  */
#line 1765 "raku.y"
        { tree_t *c=ast_node_new(TT_ARR_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6261 "raku.tab.c"
    break;

  case 321: /* block: '{' stmt_list VAR_HASH '{' expr '}' '=' expr '}'  */
#line 1768 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_SET); ast_push(c,var_node((yyvsp[-6].sval))); ast_push(c,(yyvsp[-4].node)); ast_push(c,(yyvsp[-1].node));
          ExprList *l=rk_phasers_place((yyvsp[-7].list),0); exprlist_append(l,c); (yyval.node)=make_seq(l); }
#line 6268 "raku.tab.c"
    break;

  case 322: /* block: '{' stmt_list KW_TAKE expr '}'  */
#line 1771 "raku.y"
        { tree_t *t=expr_unary(TT_SUSPEND,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,t); (yyval.node)=make_seq(l); }
#line 6274 "raku.tab.c"
    break;

  case 323: /* block: '{' stmt_list KW_RETURN expr '}'  */
#line 1773 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); expr_add_child(r,(yyvsp[-1].node)); ExprList *l=rk_phasers_place((yyvsp[-3].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6280 "raku.tab.c"
    break;

  case 324: /* block: '{' stmt_list KW_RETURN '}'  */
#line 1775 "raku.y"
        { tree_t *r=ast_node_new(TT_RETURN); ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,r); (yyval.node)=make_seq(l); }
#line 6286 "raku.tab.c"
    break;

  case 325: /* block: '{' stmt_list KW_LAST '}'  */
#line 1777 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_BREAK)); (yyval.node)=make_seq(l); }
#line 6292 "raku.tab.c"
    break;

  case 326: /* block: '{' stmt_list KW_NEXT '}'  */
#line 1779 "raku.y"
        { ExprList *l=rk_phasers_place((yyvsp[-2].list),0); exprlist_append(l,ast_node_new(TT_LOOP_NEXT)); (yyval.node)=make_seq(l); }
#line 6298 "raku.tab.c"
    break;

  case 327: /* block: '{' stmt_list KW_LAST KW_IF expr '}'  */
#line 1781 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6304 "raku.tab.c"
    break;

  case 328: /* block: '{' stmt_list KW_LAST KW_UNLESS expr '}'  */
#line 1783 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_BREAK))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6310 "raku.tab.c"
    break;

  case 329: /* block: '{' stmt_list KW_NEXT KW_IF expr '}'  */
#line 1785 "raku.y"
        { tree_t *e=ast_node_new(TT_IF); expr_add_child(e,(yyvsp[-1].node)); expr_add_child(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6316 "raku.tab.c"
    break;

  case 330: /* block: '{' stmt_list KW_NEXT KW_UNLESS expr '}'  */
#line 1787 "raku.y"
        { tree_t *e=ast_node_new(TT_UNLESS); ast_push(e,(yyvsp[-1].node)); ast_push(e,seq1(ast_node_new(TT_LOOP_NEXT))); ExprList *l=rk_phasers_place((yyvsp[-4].list),0); exprlist_append(l,e); (yyval.node)=make_seq(l); }
#line 6322 "raku.tab.c"
    break;

  case 331: /* closure: '{' expr '}'  */
#line 1790 "raku.y"
                    { (yyval.node)=(yyvsp[-1].node); }
#line 6328 "raku.tab.c"
    break;

  case 332: /* expr: VAR_SCALAR '=' expr  */
#line 1793 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),(yyvsp[0].node)); }
#line 6334 "raku.tab.c"
    break;

  case 333: /* expr: VAR_ARRAY '=' expr  */
#line 1794 "raku.y"
                           { (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),rk_arr_rhs((yyvsp[0].node))); }
#line 6340 "raku.tab.c"
    break;

  case 334: /* expr: VAR_SCALAR OP_ADD_EQ expr  */
#line 1795 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_ADD,v,(yyvsp[0].node))); }
#line 6346 "raku.tab.c"
    break;

  case 335: /* expr: VAR_SCALAR OP_SUB_EQ expr  */
#line 1796 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_SUB,v,(yyvsp[0].node))); }
#line 6352 "raku.tab.c"
    break;

  case 336: /* expr: VAR_SCALAR OP_MUL_EQ expr  */
#line 1797 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_MUL,v,(yyvsp[0].node))); }
#line 6358 "raku.tab.c"
    break;

  case 337: /* expr: VAR_SCALAR OP_DIV_EQ expr  */
#line 1798 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_DIV,v,(yyvsp[0].node))); }
#line 6364 "raku.tab.c"
    break;

  case 338: /* expr: VAR_SCALAR OP_CAT_EQ expr  */
#line 1799 "raku.y"
                                { tree_t *v=var_node((yyvsp[-2].sval)); (yyval.node)=expr_binary(TT_ASSIGN,var_node((yyvsp[-2].sval)),expr_binary(TT_CAT,v,(yyvsp[0].node))); }
#line 6370 "raku.tab.c"
    break;

  case 339: /* expr: KW_GATHER block  */
#line 1800 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6380 "raku.tab.c"
    break;

  case 340: /* expr: KW_GATHER for_stmt  */
#line 1805 "raku.y"
                           {
          tree_t *g = ast_node_new(TT_GATHER);
          expr_add_child(g, (yyvsp[0].node));
          (yyval.node) = g;
      }
#line 6390 "raku.tab.c"
    break;

  case 341: /* expr: tern_expr OP_FATARROW expr  */
#line 1811 "raku.y"
        { tree_t *c = make_call("__rk_pair"); expr_add_child(c, (yyvsp[-2].node)); expr_add_child(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6396 "raku.tab.c"
    break;

  case 342: /* expr: tern_expr  */
#line 1812 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6402 "raku.tab.c"
    break;

  case 343: /* tern_expr: or_expr OP_TERNARY1 tern_expr OP_TERNARY2 tern_expr  */
#line 1816 "raku.y"
        { tree_t *c = ast_node_new(TT_TERNARY); ast_push(c, (yyvsp[-4].node)); ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 6408 "raku.tab.c"
    break;

  case 344: /* tern_expr: or_expr  */
#line 1817 "raku.y"
                           { (yyval.node)=(yyvsp[0].node); }
#line 6414 "raku.tab.c"
    break;

  case 345: /* or_expr: or_expr OP_OR and_expr  */
#line 1820 "raku.y"
                               { (yyval.node)=expr_binary(TT_ALT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6420 "raku.tab.c"
    break;

  case 346: /* or_expr: or_expr OP_XOROP and_expr  */
#line 1821 "raku.y"
                                 { tree_t *c=make_call("__rk_xor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6426 "raku.tab.c"
    break;

  case 347: /* or_expr: or_expr OP_MAXOP and_expr  */
#line 1822 "raku.y"
                                 { tree_t *c=make_call("__rk_max"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6432 "raku.tab.c"
    break;

  case 348: /* or_expr: or_expr OP_MINOP and_expr  */
#line 1823 "raku.y"
                                 { tree_t *c=make_call("__rk_min"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6438 "raku.tab.c"
    break;

  case 349: /* or_expr: and_expr  */
#line 1824 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6444 "raku.tab.c"
    break;

  case 350: /* and_expr: and_expr OP_AND cmp_expr  */
#line 1827 "raku.y"
                               { (yyval.node)=expr_binary(TT_SEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6450 "raku.tab.c"
    break;

  case 351: /* and_expr: cmp_expr  */
#line 1828 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6456 "raku.tab.c"
    break;

  case 352: /* cmp_expr: cmp_expr OP_EQ divis_expr  */
#line 1831 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_EQ,(yyvsp[0].node)); }
#line 6462 "raku.tab.c"
    break;

  case 353: /* cmp_expr: cmp_expr OP_NSMARTM divis_expr  */
#line 1832 "raku.y"
                                      { tree_t *c=make_call("__rk_not_smartmatch"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6468 "raku.tab.c"
    break;

  case 354: /* cmp_expr: cmp_expr OP_APPROX divis_expr  */
#line 1833 "raku.y"
                                     { tree_t *c=make_call("__rk_approx"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6474 "raku.tab.c"
    break;

  case 355: /* cmp_expr: cmp_expr OP_SETELEM divis_expr  */
#line 1834 "raku.y"
                                      { tree_t *c=make_call("__rk_set_elem"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6480 "raku.tab.c"
    break;

  case 356: /* cmp_expr: cmp_expr OP_SETCONT divis_expr  */
#line 1835 "raku.y"
                                      { tree_t *c=make_call("__rk_set_cont"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6486 "raku.tab.c"
    break;

  case 357: /* cmp_expr: cmp_expr OP_AFTER divis_expr  */
#line 1836 "raku.y"
                                    { tree_t *c=make_call("__rk_after"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6492 "raku.tab.c"
    break;

  case 358: /* cmp_expr: cmp_expr OP_BEFORE divis_expr  */
#line 1837 "raku.y"
                                     { tree_t *c=make_call("__rk_before"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6498 "raku.tab.c"
    break;

  case 359: /* cmp_expr: cmp_expr OP_EQV divis_expr  */
#line 1838 "raku.y"
                                  { tree_t *c=make_call("__rk_eqv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6504 "raku.tab.c"
    break;

  case 360: /* cmp_expr: cmp_expr OP_IDENT3 divis_expr  */
#line 1839 "raku.y"
                                     { tree_t *c=make_call("__rk_ident"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6510 "raku.tab.c"
    break;

  case 361: /* cmp_expr: cmp_expr OP_NE divis_expr  */
#line 1840 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_NE,(yyvsp[0].node)); }
#line 6516 "raku.tab.c"
    break;

  case 362: /* cmp_expr: cmp_expr '<' divis_expr  */
#line 1841 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LT,(yyvsp[0].node)); }
#line 6522 "raku.tab.c"
    break;

  case 363: /* cmp_expr: cmp_expr '>' divis_expr  */
#line 1842 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GT,(yyvsp[0].node)); }
#line 6528 "raku.tab.c"
    break;

  case 364: /* cmp_expr: cmp_expr OP_LE divis_expr  */
#line 1843 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_LE,(yyvsp[0].node)); }
#line 6534 "raku.tab.c"
    break;

  case 365: /* cmp_expr: cmp_expr OP_GE divis_expr  */
#line 1844 "raku.y"
                                  { (yyval.node)=rk_chain_cmp((yyvsp[-2].node),TT_GE,(yyvsp[0].node)); }
#line 6540 "raku.tab.c"
    break;

  case 366: /* cmp_expr: divis_expr OP_SEQ divis_expr  */
#line 1845 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LEQ,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6546 "raku.tab.c"
    break;

  case 367: /* cmp_expr: divis_expr OP_CMP3 divis_expr  */
#line 1846 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmp3",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6552 "raku.tab.c"
    break;

  case 368: /* cmp_expr: divis_expr OP_CMPG divis_expr  */
#line 1847 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_cmpg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6558 "raku.tab.c"
    break;

  case 369: /* cmp_expr: divis_expr OP_LEG divis_expr  */
#line 1848 "raku.y"
                                     { (yyval.node)=rk_order_call("__rk_leg",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6564 "raku.tab.c"
    break;

  case 370: /* cmp_expr: divis_expr OP_SNE divis_expr  */
#line 1849 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LNE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6570 "raku.tab.c"
    break;

  case 371: /* cmp_expr: divis_expr OP_SLT divis_expr  */
#line 1850 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6576 "raku.tab.c"
    break;

  case 372: /* cmp_expr: divis_expr OP_SLE divis_expr  */
#line 1851 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LLE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6582 "raku.tab.c"
    break;

  case 373: /* cmp_expr: divis_expr OP_SGT divis_expr  */
#line 1852 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6588 "raku.tab.c"
    break;

  case 374: /* cmp_expr: divis_expr OP_SGE divis_expr  */
#line 1853 "raku.y"
                                    { (yyval.node)=expr_binary(TT_LGE,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6594 "raku.tab.c"
    break;

  case 375: /* cmp_expr: divis_expr OP_SMATCH LIT_REGEX  */
#line 1855 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match"));
          (yyval.node) = c; }
#line 6604 "raku.tab.c"
    break;

  case 376: /* cmp_expr: divis_expr OP_SMATCH LIT_MATCH_GLOBAL  */
#line 1861 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "match_global"));
          (yyval.node) = c; }
#line 6614 "raku.tab.c"
    break;

  case 377: /* cmp_expr: divis_expr OP_SMATCH LIT_SUBST  */
#line 1867 "raku.y"
        { tree_t *c = ast_node_new(TT_SMATCH);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval)));
          ast_push(c, leaf_sval(TT_QLIT, "subst"));
          (yyval.node) = c; }
#line 6624 "raku.tab.c"
    break;

  case 378: /* cmp_expr: divis_expr OP_SMATCH pkg_name  */
#line 1873 "raku.y"
        { tree_t *mc = ast_node_new(TT_METHCALL);
          ast_push(mc, (yyvsp[-2].node));
          ast_push(mc, leaf_sval(TT_QLIT, "does"));
          ast_push(mc, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = mc; }
#line 6634 "raku.tab.c"
    break;

  case 379: /* cmp_expr: divis_expr  */
#line 1878 "raku.y"
                                 { (yyval.node)=(yyvsp[0].node); }
#line 6640 "raku.tab.c"
    break;

  case 380: /* divis_expr: divis_expr OP_DIVIS jct_expr  */
#line 1881 "raku.y"
                                    { (yyval.node)=expr_binary(TT_DIVIS,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6646 "raku.tab.c"
    break;

  case 381: /* divis_expr: jct_expr  */
#line 1882 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6652 "raku.tab.c"
    break;

  case 382: /* jct_expr: jct_expr '|' range_expr  */
#line 1885 "raku.y"
                               { (yyval.node)=mk_junction("any",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6658 "raku.tab.c"
    break;

  case 383: /* jct_expr: jct_expr OP_SETSYM range_expr  */
#line 1886 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sym"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6664 "raku.tab.c"
    break;

  case 384: /* jct_expr: jct_expr OP_SETDIF range_expr  */
#line 1887 "raku.y"
                                     { tree_t *c=make_call("__rk_set_dif"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6670 "raku.tab.c"
    break;

  case 385: /* jct_expr: jct_expr OP_SETSUM range_expr  */
#line 1888 "raku.y"
                                     { tree_t *c=make_call("__rk_set_sum"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6676 "raku.tab.c"
    break;

  case 386: /* jct_expr: jct_expr OP_SETUNI range_expr  */
#line 1889 "raku.y"
                                     { tree_t *c=make_call("__rk_set_uni"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6682 "raku.tab.c"
    break;

  case 387: /* jct_expr: jct_expr OP_SETMUL range_expr  */
#line 1890 "raku.y"
                                     { tree_t *c=make_call("__rk_set_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6688 "raku.tab.c"
    break;

  case 388: /* jct_expr: jct_expr OP_SETINT range_expr  */
#line 1891 "raku.y"
                                     { tree_t *c=make_call("__rk_set_int"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6694 "raku.tab.c"
    break;

  case 389: /* jct_expr: jct_expr '&' range_expr  */
#line 1892 "raku.y"
                               { (yyval.node)=mk_junction("all",(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6700 "raku.tab.c"
    break;

  case 390: /* jct_expr: dor_expr  */
#line 1893 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6706 "raku.tab.c"
    break;

  case 391: /* dor_expr: dor_expr OP_DOR range_expr  */
#line 1897 "raku.y"
        { tree_t *c=make_call("__rk_dor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6712 "raku.tab.c"
    break;

  case 392: /* dor_expr: range_expr  */
#line 1898 "raku.y"
                               { (yyval.node)=(yyvsp[0].node); }
#line 6718 "raku.tab.c"
    break;

  case 393: /* range_expr: add_expr OP_RANGE add_expr  */
#line 1901 "raku.y"
                                    { (yyval.node)=expr_binary(TT_TO,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6724 "raku.tab.c"
    break;

  case 394: /* range_expr: range_expr OP_UNICMP add_expr  */
#line 1902 "raku.y"
                                     { tree_t *c=make_call("__rk_unicmp"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6730 "raku.tab.c"
    break;

  case 395: /* range_expr: range_expr OP_COLL add_expr  */
#line 1903 "raku.y"
                                   { tree_t *c=make_call("__rk_coll"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6736 "raku.tab.c"
    break;

  case 396: /* range_expr: range_expr OP_RANGE_XB add_expr  */
#line 1904 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xb"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6742 "raku.tab.c"
    break;

  case 397: /* range_expr: range_expr OP_RANGE_XL add_expr  */
#line 1905 "raku.y"
                                       { tree_t *c=make_call("__rk_range_xl"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6748 "raku.tab.c"
    break;

  case 398: /* range_expr: add_expr OP_RANGE_EX add_expr  */
#line 1906 "raku.y"
                                    { (yyval.node)=rk_range_ex((yyvsp[-2].node),(yyvsp[0].node)); }
#line 6754 "raku.tab.c"
    break;

  case 399: /* range_expr: add_expr  */
#line 1907 "raku.y"
                                    { (yyval.node)=(yyvsp[0].node); }
#line 6760 "raku.tab.c"
    break;

  case 400: /* add_expr: add_expr '~' repl_expr  */
#line 1910 "raku.y"
                              { (yyval.node)=expr_binary(TT_CAT,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6766 "raku.tab.c"
    break;

  case 401: /* add_expr: add_expr OP_COMPOSEU repl_expr  */
#line 1911 "raku.y"
                                      { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6772 "raku.tab.c"
    break;

  case 402: /* add_expr: add_expr OP_COMPOSE repl_expr  */
#line 1912 "raku.y"
                                     { tree_t *c=make_call("__rk_compose"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6778 "raku.tab.c"
    break;

  case 403: /* add_expr: repl_expr  */
#line 1913 "raku.y"
                              { (yyval.node)=(yyvsp[0].node); }
#line 6784 "raku.tab.c"
    break;

  case 404: /* repl_expr: repl_expr OP_REP_X addsub_expr  */
#line 1916 "raku.y"
                                      { (yyval.node)=expr_binary(TT_XREP,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6790 "raku.tab.c"
    break;

  case 405: /* repl_expr: repl_expr OP_REP_XX addsub_expr  */
#line 1917 "raku.y"
                                      { tree_t *call=make_call("__rk_arr_xx"); expr_add_child(call,(yyvsp[-2].node)); expr_add_child(call,(yyvsp[0].node)); (yyval.node)=call; }
#line 6796 "raku.tab.c"
    break;

  case 406: /* repl_expr: addsub_expr  */
#line 1918 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6802 "raku.tab.c"
    break;

  case 407: /* addsub_expr: addsub_expr '+' mul_expr  */
#line 1921 "raku.y"
                                { (yyval.node)=expr_binary(TT_ADD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6808 "raku.tab.c"
    break;

  case 408: /* addsub_expr: addsub_expr OP_UMINUS_I mul_expr  */
#line 1922 "raku.y"
                                        { tree_t *c=make_call("__rk_sub"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6814 "raku.tab.c"
    break;

  case 409: /* addsub_expr: addsub_expr OP_QBXOR mul_expr  */
#line 1923 "raku.y"
                                     { tree_t *c=make_call("__rk_lbxor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6820 "raku.tab.c"
    break;

  case 410: /* addsub_expr: addsub_expr OP_QBOR mul_expr  */
#line 1924 "raku.y"
                                    { tree_t *c=make_call("__rk_lbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6826 "raku.tab.c"
    break;

  case 411: /* addsub_expr: addsub_expr OP_NBOR mul_expr  */
#line 1925 "raku.y"
                                    { tree_t *c=make_call("__rk_sbor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6832 "raku.tab.c"
    break;

  case 412: /* addsub_expr: addsub_expr OP_BORT mul_expr  */
#line 1926 "raku.y"
                                    { tree_t *c=make_call("__rk_bor"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6838 "raku.tab.c"
    break;

  case 413: /* addsub_expr: addsub_expr '-' mul_expr  */
#line 1927 "raku.y"
                                { (yyval.node)=expr_binary(TT_SUB,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6844 "raku.tab.c"
    break;

  case 414: /* addsub_expr: mul_expr  */
#line 1928 "raku.y"
                                { (yyval.node)=(yyvsp[0].node); }
#line 6850 "raku.tab.c"
    break;

  case 415: /* mul_expr: mul_expr '*' unary_expr  */
#line 1931 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MUL,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6856 "raku.tab.c"
    break;

  case 416: /* mul_expr: mul_expr OP_UDIV unary_expr  */
#line 1932 "raku.y"
                                   { tree_t *c=make_call("__rk_div"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6862 "raku.tab.c"
    break;

  case 417: /* mul_expr: mul_expr OP_UMUL unary_expr  */
#line 1933 "raku.y"
                                   { tree_t *c=make_call("__rk_mul"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6868 "raku.tab.c"
    break;

  case 418: /* mul_expr: mul_expr OP_NBAND unary_expr  */
#line 1934 "raku.y"
                                    { tree_t *c=make_call("__rk_sband"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6874 "raku.tab.c"
    break;

  case 419: /* mul_expr: mul_expr OP_MODW unary_expr  */
#line 1935 "raku.y"
                                   { tree_t *c=make_call("__rk_mod"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6880 "raku.tab.c"
    break;

  case 420: /* mul_expr: mul_expr OP_LCM unary_expr  */
#line 1936 "raku.y"
                                  { tree_t *c=make_call("__rk_lcm"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6886 "raku.tab.c"
    break;

  case 421: /* mul_expr: mul_expr '/' unary_expr  */
#line 1937 "raku.y"
                                   { (yyval.node)=expr_binary(TT_DIV,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6892 "raku.tab.c"
    break;

  case 422: /* mul_expr: mul_expr '%' unary_expr  */
#line 1938 "raku.y"
                                   { (yyval.node)=expr_binary(TT_MOD,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6898 "raku.tab.c"
    break;

  case 423: /* mul_expr: mul_expr OP_DIV unary_expr  */
#line 1940 "raku.y"
        { tree_t *c=make_call("__rk_intdiv"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6904 "raku.tab.c"
    break;

  case 424: /* mul_expr: mul_expr OP_GCD unary_expr  */
#line 1942 "raku.y"
        { tree_t *c=make_call("__rk_gcd"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6910 "raku.tab.c"
    break;

  case 425: /* mul_expr: mul_expr OP_BAND unary_expr  */
#line 1944 "raku.y"
        { tree_t *c=make_call("iand"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6916 "raku.tab.c"
    break;

  case 426: /* mul_expr: mul_expr OP_SHL unary_expr  */
#line 1946 "raku.y"
        { tree_t *c=make_call("ishift"); expr_add_child(c,(yyvsp[-2].node)); expr_add_child(c,(yyvsp[0].node)); (yyval.node)=c; }
#line 6922 "raku.tab.c"
    break;

  case 427: /* mul_expr: unary_expr  */
#line 1947 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6928 "raku.tab.c"
    break;

  case 428: /* unary_expr: '-' unary_expr  */
#line 1950 "raku.y"
                                   { (yyval.node)=expr_unary(TT_MNS,(yyvsp[0].node)); }
#line 6934 "raku.tab.c"
    break;

  case 429: /* unary_expr: '+' unary_expr  */
#line 1951 "raku.y"
                                   { (yyval.node)=rk_numeric_ctx((yyvsp[0].node)); }
#line 6940 "raku.tab.c"
    break;

  case 430: /* unary_expr: '!' unary_expr  */
#line 1952 "raku.y"
                                   { (yyval.node)=expr_unary(TT_NOT,(yyvsp[0].node)); }
#line 6946 "raku.tab.c"
    break;

  case 431: /* unary_expr: CARET unary_expr  */
#line 1953 "raku.y"
                                   { tree_t *z=ast_node_new(TT_ILIT); z->v.ival=0; (yyval.node)=rk_range_ex(z,(yyvsp[0].node)); }
#line 6952 "raku.tab.c"
    break;

  case 432: /* unary_expr: OP_REDUCE unary_expr  */
#line 1955 "raku.y"
        { const char *rop = !strcmp((yyvsp[-1].sval),"+") ? "__rk_reduce_add" : !strcmp((yyvsp[-1].sval),"-") ? "__rk_reduce_sub"
                          : !strcmp((yyvsp[-1].sval),"*") ? "__rk_reduce_mul" : !strcmp((yyvsp[-1].sval),"~") ? "__rk_reduce_cat"
                          : !strcmp((yyvsp[-1].sval),"min") ? "__rk_reduce_min" : "__rk_reduce_max";
          tree_t *e=make_call(rop); expr_add_child(e,(yyvsp[0].node)); ct_drop((yyvsp[-1].sval)); (yyval.node)=e; }
#line 6961 "raku.tab.c"
    break;

  case 433: /* unary_expr: pow_expr  */
#line 1959 "raku.y"
                                   { (yyval.node)=(yyvsp[0].node); }
#line 6967 "raku.tab.c"
    break;

  case 434: /* pow_expr: postfix_expr OP_POW unary_expr  */
#line 1962 "raku.y"
                                      { (yyval.node)=expr_binary(TT_POW,(yyvsp[-2].node),(yyvsp[0].node)); }
#line 6973 "raku.tab.c"
    break;

  case 435: /* pow_expr: postfix_expr  */
#line 1963 "raku.y"
                                      { (yyval.node)=(yyvsp[0].node); }
#line 6979 "raku.tab.c"
    break;

  case 436: /* scalar_list: VAR_SCALAR  */
#line 1966 "raku.y"
                                    { (yyval.list) = exprlist_append(exprlist_new(), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 6985 "raku.tab.c"
    break;

  case 437: /* scalar_list: scalar_list ',' VAR_SCALAR  */
#line 1967 "raku.y"
                                    { (yyval.list) = exprlist_append((yyvsp[-2].list), var_node((yyvsp[0].sval))); ct_drop((yyvsp[0].sval)); }
#line 6991 "raku.tab.c"
    break;

  case 438: /* meth_name: IDENT  */
#line 1970 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 6997 "raku.tab.c"
    break;

  case 439: /* meth_name: KW_SORT  */
#line 1971 "raku.y"
                 { (yyval.sval)=ct_strdup("sort"); }
#line 7003 "raku.tab.c"
    break;

  case 440: /* meth_name: KW_REVERSE  */
#line 1972 "raku.y"
                 { (yyval.sval)=ct_strdup("reverse"); }
#line 7009 "raku.tab.c"
    break;

  case 441: /* meth_name: KW_MAP  */
#line 1973 "raku.y"
                 { (yyval.sval)=ct_strdup("map"); }
#line 7015 "raku.tab.c"
    break;

  case 442: /* meth_name: KW_GREP  */
#line 1974 "raku.y"
                 { (yyval.sval)=ct_strdup("grep"); }
#line 7021 "raku.tab.c"
    break;

  case 443: /* meth_name: KW_SAY  */
#line 1975 "raku.y"
                 { (yyval.sval)=ct_strdup("say"); }
#line 7027 "raku.tab.c"
    break;

  case 444: /* meth_name: KW_PRINT  */
#line 1976 "raku.y"
                 { (yyval.sval)=ct_strdup("print"); }
#line 7033 "raku.tab.c"
    break;

  case 445: /* meth_name: KW_TAKE  */
#line 1977 "raku.y"
                 { (yyval.sval)=ct_strdup("take"); }
#line 7039 "raku.tab.c"
    break;

  case 446: /* meth_name: KW_RETURN  */
#line 1978 "raku.y"
                 { (yyval.sval)=ct_strdup("return"); }
#line 7045 "raku.tab.c"
    break;

  case 447: /* meth_name: KW_EXISTS  */
#line 1979 "raku.y"
                 { (yyval.sval)=ct_strdup("exists"); }
#line 7051 "raku.tab.c"
    break;

  case 448: /* meth_name: KW_DELETE  */
#line 1980 "raku.y"
                 { (yyval.sval)=ct_strdup("delete"); }
#line 7057 "raku.tab.c"
    break;

  case 449: /* meth_name: KW_JOIN  */
#line 1981 "raku.y"
                 { (yyval.sval)=ct_strdup("join"); }
#line 7063 "raku.tab.c"
    break;

  case 450: /* meth_name: TESTOP  */
#line 1982 "raku.y"
                 { (yyval.sval)=(yyvsp[0].sval); }
#line 7069 "raku.tab.c"
    break;

  case 451: /* postfix_expr: call_expr  */
#line 1984 "raku.y"
                         { (yyval.node)=(yyvsp[0].node); }
#line 7075 "raku.tab.c"
    break;

  case 452: /* call_expr: KW_JOIN expr ',' arg_list  */
#line 1987 "raku.y"
        { tree_t *e=make_call("join");
          expr_add_child(e, (yyvsp[-2].node));
          ExprList *args=(yyvsp[0].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7085 "raku.tab.c"
    break;

  case 453: /* call_expr: IDENT '(' arg_list ')'  */
#line 1993 "raku.y"
        { tree_t *e=make_call((yyvsp[-3].sval));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7094 "raku.tab.c"
    break;

  case 454: /* call_expr: IDENT '(' ')'  */
#line 1997 "raku.y"
                     { (yyval.node)=make_call((yyvsp[-2].sval)); }
#line 7100 "raku.tab.c"
    break;

  case 455: /* call_expr: IDENT '(' named_arg_list ')'  */
#line 1999 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-3].sval), NULL, (yyvsp[-1].list)); ct_drop((yyvsp[-3].sval)); }
#line 7106 "raku.tab.c"
    break;

  case 456: /* call_expr: IDENT '(' arg_list ',' named_arg_list ')'  */
#line 2001 "raku.y"
        { (yyval.node) = rk_named_call((yyvsp[-5].sval), (yyvsp[-3].list), (yyvsp[-1].list)); ct_drop((yyvsp[-5].sval)); }
#line 7112 "raku.tab.c"
    break;

  case 457: /* call_expr: VAR_SCALAR '(' arg_list ')'  */
#line 2003 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-3].sval)));
          ExprList *args=(yyvsp[-1].list);
          if(args){ for(int i=0;i<args->count;i++) expr_add_child(e,args->items[i]); exprlist_free(args); }
          (yyval.node)=e; }
#line 7121 "raku.tab.c"
    break;

  case 458: /* call_expr: VAR_SCALAR '(' ')'  */
#line 2008 "raku.y"
        { tree_t *e=ast_node_new(TT_INVOKE); expr_add_child(e,var_node((yyvsp[-2].sval))); (yyval.node)=e; }
#line 7127 "raku.tab.c"
    break;

  case 459: /* call_expr: IDENT '.' KW_NEW '(' named_arg_list ')'  */
#line 2010 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), (yyvsp[-1].list), NULL);
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; } }
#line 7140 "raku.tab.c"
    break;

  case 460: /* call_expr: IDENT '.' KW_NEW '(' arg_list ')'  */
#line 2019 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-5].sval), NULL, (yyvsp[-1].list));
          if (ah) { ct_drop((yyvsp[-5].sval)); exprlist_free((yyvsp[-1].list)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; } }
#line 7153 "raku.tab.c"
    break;

  case 461: /* call_expr: IDENT '.' KW_NEW '(' ')'  */
#line 2028 "raku.y"
        { tree_t *ah = rk_adhoc_new((yyvsp[-4].sval), NULL, NULL);
          if (ah) { ct_drop((yyvsp[-4].sval)); (yyval.node) = ah; }
          else {
          tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          (yyval.node) = c; } }
#line 7164 "raku.tab.c"
    break;

  case 462: /* call_expr: IDENT '.' KW_NEW  */
#line 2035 "raku.y"
        { tree_t *c = ast_node_new(TT_NEW);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7172 "raku.tab.c"
    break;

  case 463: /* call_expr: IDENT '.' IDENT  */
#line 2039 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7181 "raku.tab.c"
    break;

  case 464: /* call_expr: IDENT '.' IDENT '(' arg_list ')'  */
#line 2044 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-5].sval))); ct_drop((yyvsp[-5].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7192 "raku.tab.c"
    break;

  case 465: /* call_expr: IDENT '.' IDENT '(' ')'  */
#line 2051 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-4].sval))); ct_drop((yyvsp[-4].sval));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7201 "raku.tab.c"
    break;

  case 466: /* call_expr: IDENT '.' CARET IDENT  */
#line 2056 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node((yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7211 "raku.tab.c"
    break;

  case 467: /* call_expr: atom '.' CARET IDENT  */
#line 2062 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-3].node));
          { size_t _l = strlen((yyvsp[0].sval)); char *_m = (char*)ct_alloc(_l+2); _m[0]='^'; memcpy(_m+1,(yyvsp[0].sval),_l); _m[_l+1]='\0'; ast_push(c, leaf_sval(TT_QLIT, _m)); ct_drop(_m); }
          ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7221 "raku.tab.c"
    break;

  case 468: /* call_expr: atom '.' meth_name '(' arg_list ')'  */
#line 2068 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7232 "raku.tab.c"
    break;

  case 469: /* call_expr: atom '.' meth_name '(' named_arg_list ')'  */
#line 2075 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *nargs = (yyvsp[-1].list);
          if (nargs) { for (int i = 0; i < nargs->count; i++) ast_push(c, nargs->items[i]); exprlist_free(nargs); }
          (yyval.node) = c; }
#line 7243 "raku.tab.c"
    break;

  case 470: /* call_expr: atom '.' meth_name '(' ')'  */
#line 2082 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7252 "raku.tab.c"
    break;

  case 471: /* call_expr: atom '.' meth_name  */
#line 2087 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7261 "raku.tab.c"
    break;

  case 472: /* call_expr: call_expr '.' meth_name '(' arg_list ')'  */
#line 2092 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-5].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7272 "raku.tab.c"
    break;

  case 473: /* call_expr: call_expr '.' meth_name '(' ')'  */
#line 2099 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7281 "raku.tab.c"
    break;

  case 474: /* call_expr: call_expr '.' meth_name  */
#line 2104 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-2].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7290 "raku.tab.c"
    break;

  case 475: /* call_expr: atom '.' meth_name ':' arg_list  */
#line 2109 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7301 "raku.tab.c"
    break;

  case 476: /* call_expr: call_expr '.' meth_name ':' arg_list  */
#line 2116 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, (yyvsp[-4].node));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          ExprList *args = (yyvsp[0].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7312 "raku.tab.c"
    break;

  case 477: /* call_expr: '.' meth_name '(' arg_list ')'  */
#line 2123 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-3].sval))); ct_drop((yyvsp[-3].sval));
          ExprList *args = (yyvsp[-1].list);
          if (args) { for (int i = 0; i < args->count; i++) ast_push(c, args->items[i]); exprlist_free(args); }
          (yyval.node) = c; }
#line 7323 "raku.tab.c"
    break;

  case 478: /* call_expr: '.' meth_name '(' ')'  */
#line 2130 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[-2].sval))); ct_drop((yyvsp[-2].sval));
          (yyval.node) = c; }
#line 7332 "raku.tab.c"
    break;

  case 479: /* call_expr: '.' meth_name  */
#line 2135 "raku.y"
        { tree_t *c = ast_node_new(TT_METHCALL);
          ast_push(c, var_node("$_"));
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = c; }
#line 7341 "raku.tab.c"
    break;

  case 480: /* call_expr: KW_DIE expr  */
#line 2140 "raku.y"
        { tree_t *d=ast_node_new(TT_DIE); expr_add_child(d,(yyvsp[0].node)); (yyval.node)=d; }
#line 7347 "raku.tab.c"
    break;

  case 481: /* call_expr: KW_MAP closure expr  */
#line 2142 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7353 "raku.tab.c"
    break;

  case 482: /* call_expr: KW_MAP closure ',' expr  */
#line 2144 "raku.y"
        { tree_t *c = ast_node_new(TT_MAP);  ast_push(c, (yyvsp[-2].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7359 "raku.tab.c"
    break;

  case 483: /* call_expr: KW_GREP closure expr  */
#line 2146 "raku.y"
        { tree_t *c = ast_node_new(TT_GREP); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7365 "raku.tab.c"
    break;

  case 484: /* call_expr: KW_SORT expr  */
#line 2148 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7371 "raku.tab.c"
    break;

  case 485: /* call_expr: KW_SORT closure expr  */
#line 2150 "raku.y"
        { tree_t *c = ast_node_new(TT_SORT); ast_push(c, (yyvsp[-1].node)); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7377 "raku.tab.c"
    break;

  case 486: /* call_expr: KW_REVERSE expr  */
#line 2152 "raku.y"
        { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, (yyvsp[0].node)); (yyval.node) = c; }
#line 7383 "raku.tab.c"
    break;

  case 487: /* call_expr: atom  */
#line 2153 "raku.y"
                     { (yyval.node)=(yyvsp[0].node); }
#line 7389 "raku.tab.c"
    break;

  case 488: /* arg_list: expr  */
#line 2156 "raku.y"
                        { (yyval.list)=exprlist_append(exprlist_new(),(yyvsp[0].node)); }
#line 7395 "raku.tab.c"
    break;

  case 489: /* arg_list: arg_list ',' expr  */
#line 2157 "raku.y"
                        { (yyval.list)=exprlist_append((yyvsp[-2].list),(yyvsp[0].node)); }
#line 7401 "raku.tab.c"
    break;

  case 490: /* arg_list: arg_list ','  */
#line 2158 "raku.y"
                        { (yyval.list)=(yyvsp[-1].list); }
#line 7407 "raku.tab.c"
    break;

  case 491: /* paren_group: '(' ')'  */
#line 2161 "raku.y"
                      { (yyval.node)=make_call("__rk_arr"); }
#line 7413 "raku.tab.c"
    break;

  case 492: /* paren_group: '(' expr ')'  */
#line 2162 "raku.y"
                      { (yyval.node)=(yyvsp[-1].node); }
#line 7419 "raku.tab.c"
    break;

  case 493: /* paren_group: '(' expr ',' ')'  */
#line 2164 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7425 "raku.tab.c"
    break;

  case 494: /* paren_group: '(' expr ',' arg_list ')'  */
#line 2166 "raku.y"
        { tree_t *call=make_call("__rk_arr"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7432 "raku.tab.c"
    break;

  case 495: /* atom: LIT_INT  */
#line 2170 "raku.y"
                      { tree_t *e=ast_node_new(TT_ILIT); e->v.ival=(yyvsp[0].ival); (yyval.node)=e; }
#line 7438 "raku.tab.c"
    break;

  case 496: /* atom: LIT_BOOL  */
#line 2172 "raku.y"
        { tree_t *iv=ast_node_new(TT_ILIT); iv->v.ival=(yyvsp[0].ival);
          tree_t *bl=ast_node_new(TT_FNC); bl->v.sval=(char *)intern("__rk_mkbool");
          ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, iv); (yyval.node)=bl; }
#line 7446 "raku.tab.c"
    break;

  case 497: /* atom: LIT_FLOAT  */
#line 2175 "raku.y"
                      { tree_t *e=ast_node_new(TT_FLIT); e->v.dval=(yyvsp[0].dval); (yyval.node)=e; }
#line 7452 "raku.tab.c"
    break;

  case 498: /* atom: LIT_STR  */
#line 2176 "raku.y"
                      { (yyval.node)=leaf_sval(TT_QLIT,(yyvsp[0].sval)); }
#line 7458 "raku.tab.c"
    break;

  case 499: /* atom: WORDLIST  */
#line 2178 "raku.y"
        { tree_t *call=make_call("__rk_arr"); char *s=(yyvsp[0].sval); int wc=0;
          while(*s){ while(*s==' '||*s=='\t')s++; if(!*s)break; char *w=s;
            while(*s&&*s!=' '&&*s!='\t')s++; int L=(int)(s-w); char *tok=(char*)ct_alloc(L+1); int ti=0;
            for(int wi=0;wi<L;wi++){ if(w[wi]=='\\'&&wi+1<L&&w[wi+1]=='\\'){ tok[ti++]='\\'; wi++; } else tok[ti++]=w[wi]; }
            tok[ti]='\0'; expr_add_child(call,leaf_sval(TT_QLIT,tok)); ct_drop(tok); wc++; }
          ct_drop((yyvsp[0].sval));
          if(wc==1){ tree_t *only=call->c[0]; call->c[0]=NULL; call->n=0; (yyval.node)=only; }
          else { (yyval.node)=call; } }
#line 7471 "raku.tab.c"
    break;

  case 500: /* atom: LIT_INTERP_STR  */
#line 2186 "raku.y"
                      { (yyval.node)=lower_interp_str((yyvsp[0].sval)); }
#line 7477 "raku.tab.c"
    break;

  case 501: /* atom: VAR_SCALAR  */
#line 2187 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7483 "raku.tab.c"
    break;

  case 502: /* atom: OP_INC VAR_SCALAR  */
#line 2188 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),1); }
#line 7489 "raku.tab.c"
    break;

  case 503: /* atom: OP_DEC VAR_SCALAR  */
#line 2189 "raku.y"
                        { (yyval.node)=rk_incdec((yyvsp[0].sval),0); }
#line 7495 "raku.tab.c"
    break;

  case 504: /* atom: VAR_SCALAR OP_INC  */
#line 2190 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),1); }
#line 7501 "raku.tab.c"
    break;

  case 505: /* atom: VAR_SCALAR OP_DEC  */
#line 2191 "raku.y"
                        { (yyval.node)=rk_post_incdec((yyvsp[-1].sval),0); }
#line 7507 "raku.tab.c"
    break;

  case 506: /* atom: VAR_TWIGIL OP_INC  */
#line 2192 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),1); ct_drop((yyvsp[-1].sval)); }
#line 7513 "raku.tab.c"
    break;

  case 507: /* atom: VAR_TWIGIL OP_DEC  */
#line 2193 "raku.y"
                        { (yyval.node)=rk_tw_post_incdec((yyvsp[-1].sval),0); ct_drop((yyvsp[-1].sval)); }
#line 7519 "raku.tab.c"
    break;

  case 508: /* atom: VAR_ARRAY  */
#line 2194 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7525 "raku.tab.c"
    break;

  case 509: /* atom: VAR_HASH  */
#line 2195 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7531 "raku.tab.c"
    break;

  case 510: /* atom: VAR_CAPTURE  */
#line 2197 "raku.y"
        { tree_t *c = ast_node_new(TT_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7539 "raku.tab.c"
    break;

  case 511: /* atom: VAR_FH  */
#line 2201 "raku.y"
        { tree_t *c = ast_node_new(TT_FH_CAPTURE);
          tree_t *idx = ast_node_new(TT_ILIT); idx->v.ival = (yyvsp[0].ival);
          ast_push(c, idx); (yyval.node) = c; }
#line 7547 "raku.tab.c"
    break;

  case 512: /* atom: VAR_NAMED_CAPTURE  */
#line 2205 "raku.y"
        { tree_t *c = ast_node_new(TT_NAMED_CAPTURE);
          ast_push(c, leaf_sval(TT_QLIT, (yyvsp[0].sval))); (yyval.node) = c; }
#line 7554 "raku.tab.c"
    break;

  case 513: /* atom: VAR_ARRAY '[' expr ']'  */
#line 2208 "raku.y"
        { (yyval.node) = rk_arr_index((yyvsp[-3].sval), (yyvsp[-1].node)); }
#line 7560 "raku.tab.c"
    break;

  case 514: /* atom: VAR_ARRAY '[' expr ',' arg_list ']'  */
#line 2210 "raku.y"
        { (yyval.node) = rk_arr_pick((yyvsp[-5].sval), (yyvsp[-3].node), (yyvsp[-1].list)); }
#line 7566 "raku.tab.c"
    break;

  case 515: /* atom: VAR_ARRAY '[' '*' '-' expr ']'  */
#line 2212 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_SUB); }
#line 7572 "raku.tab.c"
    break;

  case 516: /* atom: VAR_ARRAY '[' '*' '+' expr ']'  */
#line 2214 "raku.y"
        { (yyval.node) = rk_arr_end_index((yyvsp[-5].sval), (yyvsp[-1].node), TT_ADD); }
#line 7578 "raku.tab.c"
    break;

  case 517: /* atom: ARR_ALL_SLICE  */
#line 2216 "raku.y"
        { (yyval.node) = rk_arr_all((yyvsp[0].sval)); }
#line 7584 "raku.tab.c"
    break;

  case 518: /* atom: VAR_HASH '<' IDENT '>'  */
#line 2218 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7590 "raku.tab.c"
    break;

  case 519: /* atom: VAR_HASH '{' expr '}'  */
#line 2220 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_GET); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7596 "raku.tab.c"
    break;

  case 520: /* atom: KW_EXISTS VAR_HASH '<' IDENT '>'  */
#line 2222 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-1].sval))); (yyval.node)=c; }
#line 7602 "raku.tab.c"
    break;

  case 521: /* atom: KW_EXISTS VAR_HASH '{' expr '}'  */
#line 2224 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-3].sval))); ast_push(c,(yyvsp[-1].node)); (yyval.node)=c; }
#line 7608 "raku.tab.c"
    break;

  case 522: /* atom: VAR_HASH '{' expr '}' ADV_EXISTS  */
#line 2226 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7614 "raku.tab.c"
    break;

  case 523: /* atom: VAR_HASH '<' IDENT '>' ADV_EXISTS  */
#line 2228 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_EXISTS); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7620 "raku.tab.c"
    break;

  case 524: /* atom: VAR_HASH '{' expr '}' ADV_DELETE  */
#line 2230 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,(yyvsp[-2].node)); (yyval.node)=c; }
#line 7626 "raku.tab.c"
    break;

  case 525: /* atom: VAR_HASH '<' IDENT '>' ADV_DELETE  */
#line 2232 "raku.y"
        { tree_t *c=ast_node_new(TT_HASH_DELETE); ast_push(c,var_node((yyvsp[-4].sval))); ast_push(c,leaf_sval(TT_QLIT,(yyvsp[-2].sval))); (yyval.node)=c; }
#line 7632 "raku.tab.c"
    break;

  case 526: /* atom: IDENT  */
#line 2233 "raku.y"
                      { (yyval.node)=var_node((yyvsp[0].sval)); }
#line 7638 "raku.tab.c"
    break;

  case 527: /* atom: VAR_TWIGIL  */
#line 2235 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7646 "raku.tab.c"
    break;

  case 528: /* atom: VAR_ARRAY_TWIGIL  */
#line 2239 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7654 "raku.tab.c"
    break;

  case 529: /* atom: VAR_HASH_TWIGIL  */
#line 2243 "raku.y"
        { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD);
          fe->v.sval = (char *)intern(rk_tw_bare((yyvsp[0].sval))); ct_drop((yyvsp[0].sval));
          (yyval.node) = fe; }
#line 7662 "raku.tab.c"
    break;

  case 530: /* atom: '[' ']'  */
#line 2246 "raku.y"
                      { (yyval.node)=make_call("__rk_arr_lit"); }
#line 7668 "raku.tab.c"
    break;

  case 531: /* atom: '[' expr ']'  */
#line 2248 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7674 "raku.tab.c"
    break;

  case 532: /* atom: '[' expr ',' ']'  */
#line 2250 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7680 "raku.tab.c"
    break;

  case 533: /* atom: '[' expr ',' arg_list ']'  */
#line 2252 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7687 "raku.tab.c"
    break;

  case 534: /* atom: DOLLAR_LBRACKET ']'  */
#line 2254 "raku.y"
                           { (yyval.node)=make_call("__rk_arr_lit_item"); }
#line 7693 "raku.tab.c"
    break;

  case 535: /* atom: DOLLAR_LBRACKET expr ']'  */
#line 2256 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-1].node)); (yyval.node)=call; }
#line 7699 "raku.tab.c"
    break;

  case 536: /* atom: DOLLAR_LBRACKET expr ',' ']'  */
#line 2258 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-2].node)); (yyval.node)=call; }
#line 7705 "raku.tab.c"
    break;

  case 537: /* atom: DOLLAR_LBRACKET expr ',' arg_list ']'  */
#line 2260 "raku.y"
        { tree_t *call=make_call("__rk_arr_lit_item"); expr_add_child(call,(yyvsp[-3].node));
          ExprList *a=(yyvsp[-1].list); if(a){ for(int i=0;i<a->count;i++) expr_add_child(call,a->items[i]); exprlist_free(a); } (yyval.node)=call; }
#line 7712 "raku.tab.c"
    break;

  case 538: /* atom: paren_group  */
#line 2262 "raku.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 7718 "raku.tab.c"
    break;

  case 539: /* atom: block  */
#line 2263 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7724 "raku.tab.c"
    break;

  case 540: /* atom: KW_SUB block  */
#line 2264 "raku.y"
                      { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node)); (yyval.node)=b; }
#line 7730 "raku.tab.c"
    break;

  case 541: /* atom: KW_SUB '(' param_list ')' block  */
#line 2266 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *ps=(yyvsp[-2].list); if(ps){ for(int i=0;i<ps->count;i++) expr_add_child(b,ps->items[i]); exprlist_free(ps); } (yyval.node)=b; }
#line 7737 "raku.tab.c"
    break;

  case 542: /* atom: OP_ARROW scalar_list block  */
#line 2269 "raku.y"
        { tree_t *b=ast_node_new(TT_ANON_BLOCK); expr_add_child(b,(yyvsp[0].node));
          ExprList *vs=(yyvsp[-1].list); if(vs){ for(int i=0;i<vs->count;i++) expr_add_child(b,vs->items[i]); exprlist_free(vs); } (yyval.node)=b; }
#line 7744 "raku.tab.c"
    break;


#line 7748 "raku.tab.c"

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

#line 2272 "raku.y"

extern void *raku_yy_scan_string(const char *);
extern void  raku_yy_delete_buffer(void *);
tree_t *raku_parse_string(const char *src) {
    raku_prog_result = NULL;
    void *buf = raku_yy_scan_string(src);
    raku_yyparse();
    raku_yy_delete_buffer(buf);
    return raku_prog_result;
}
