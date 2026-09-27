#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "ct_arena.h"
#include "ast.h"
#include "../snobol4/scrip_cc.h"
#include "rk_tree.h"
#include "rk_syntax.h"
#include "rk_opname.h"
/*====================================================================================================================================================================================================*/
typedef struct { tree_t **v; int n, cap; } TL;
typedef struct { const char **k; int n, cap; } NS;
struct RkB {
    const char *s; int len;
    int *nlp; int nnlp, cnlp;
    NS arrn, als;
    int post_uid, twpost_uid, destr_uid, fm_uid;
    int after_line;
    tree_t *tail_list, *tail_tree;
    int nonwhen;
};
#define GROW(arr, n, cap, type) do { if ((n) >= (cap)) { (cap) = (cap) ? (cap) * 2 : 8; (arr) = (type *) ct_grow((arr), sizeof(type) * (size_t) (cap)); } } while (0)
/*====================================================================================================================================================================================================*/
static char *fmt(const char *f, ...) {
    va_list a; va_start(a, f); int n = vsnprintf(NULL, 0, f, a); va_end(a);
    char *r = (char *) ct_alloc((size_t) n + 1); va_start(a, f); vsnprintf(r, (size_t) n + 1, f, a); va_end(a); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct { char *v; int n, cap; } SB;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sb_c(SB *s, char c) { GROW(s->v, s->n, s->cap, char); s->v[s->n++] = c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void sb_cp(SB *s, unsigned long cp) {
    if (cp < 0x80) sb_c(s, (char) cp);
    else if (cp < 0x800) { sb_c(s, (char) (0xC0 | (cp >> 6))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
    else if (cp < 0x10000) { sb_c(s, (char) (0xE0 | (cp >> 12))); sb_c(s, (char) (0x80 | ((cp >> 6) & 0x3F))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
    else { sb_c(s, (char) (0xF0 | (cp >> 18))); sb_c(s, (char) (0x80 | ((cp >> 12) & 0x3F))); sb_c(s, (char) (0x80 | ((cp >> 6) & 0x3F))); sb_c(s, (char) (0x80 | (cp & 0x3F))); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *sb_str(SB *s) { sb_c(s, 0); s->n--; return s->v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void tl_add(TL *l, tree_t *t) { GROW(l->v, l->n, l->cap, tree_t *); l->v[l->n++] = t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *spn(RkB *b, int from, int to) { if (to < from) to = from; char *r = (char *) ct_alloc((size_t) (to - from) + 1); memcpy(r, b->s + from, (size_t) (to - from)); r[to - from] = 0; return r;
    }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *trimdup(const char *s, int n) {
    while (n > 0 && isspace((unsigned char) s[n - 1])) n--;
    while (n > 0 && isspace((unsigned char) *s)) { s++; n--; }
    char *r = (char *) ct_alloc((size_t) n + 1); memcpy(r, s, (size_t) n); r[n] = 0; return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int line_at(RkB *b, int pos) {
    int lo = 0, hi = b->nnlp; if (pos > b->len) pos = b->len;
    while (lo < hi) { int m = lo + (hi - lo) / 2; if (b->nlp[m] < pos) lo = m + 1; else hi = m; }
    return lo + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int next_tok_pos(RkB *b, int pos) {
    int q = pos;
    for (;;) {
        while (q < b->len && isspace((unsigned char) b->s[q])) q++;
        if (q < b->len && b->s[q] == '#') { while (q < b->len && b->s[q] != '\n') q++; continue; }
        return q;
    }
}
/*====================================================================================================================================================================================================*/
static tree_t *leaf_sval(tree_e k, const char *s) { tree_t *e = ast_node_new(k); e->v.sval = intern(s); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *strip_sigil(const char *s) { if (s && (s[0] == '$' || s[0] == '@' || s[0] == '%')) return s + 1; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_tw_bare(const char *s) { return (s && (s[0] == '.' || s[0] == '!')) ? s + 1 : s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned ns_hash(const char *s) { unsigned h = 2166136261u; while (*s) h = (h ^ (unsigned char) *s++) * 16777619u; return h; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ns_has(const NS *t, const char *s) {
    if (!s || !t->cap) return 0;
    for (unsigned m = (unsigned) t->cap - 1, i = ns_hash(s) & m; t->k[i]; i = (i + 1) & m) if (!strcmp(t->k[i], s)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ns_put(NS *t, const char *s) { unsigned m = (unsigned) t->cap - 1, i = ns_hash(s) & m; while (t->k[i]) i = (i + 1) & m; t->k[i] = s; t->n++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void ns_add(NS *t, const char *s) {
    if (!s || ns_has(t, s)) return;
    if (2 * (t->n + 1) > t->cap) {
        int cap = t->cap ? t->cap * 2 : 16; NS g = { (const char **) ct_zalloc((size_t) cap, sizeof(const char *)), 0, cap };
        for (int i = 0; i < t->cap; i++) if (t->k[i]) ns_put(&g, t->k[i]);
        *t = g;
    }
    ns_put(t, intern(s));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_arr(RkB *b, const char *bare) { ns_add(&b->arrn, bare); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_arr(RkB *b, const char *bare) { return ns_has(&b->arrn, bare); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_arrlit(RkB *b, const char *bare, const tree_t *rhs) {
    if (!bare || !rhs || rhs->t != TT_FNC || !rhs->v.sval || strcmp(rhs->v.sval, "__rk_arr_lit")) return;
    ns_add(&b->als, bare);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *var_ident(const char *s) { if (s && (s[0] == '@' || s[0] == '%')) return s; return strip_sigil(s); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *var_node(RkB *b, const char *name) {
    const char *id = var_ident(name);
    if (name && name[0] == '@') mark_arr(b, id);
    tree_t *e = leaf_sval(TT_VAR, id);
    if (name && name[0] != '$' && name[0] != '@' && name[0] != '%') e->slen = 1;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_call(const char *name) { tree_t *e = leaf_sval(TT_FNC, name); tree_t *n = ast_node_new(TT_VAR); n->v.sval = intern(name); expr_add_child(e, n); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *call2(const char *fn, tree_t *l, tree_t *r) { tree_t *c = make_call(fn); expr_add_child(c, l); expr_add_child(c, r); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_ilit(long long v) { tree_t *e = ast_node_new(TT_ILIT); e->v.ival = v; return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *testop_rt(const char *s) {
    static const char *const map[][2] = { { "plan", "__rk_test_plan" }, { "ok", "__rk_test_ok" }, { "nok", "__rk_test_nok" }, { "is", "__rk_test_is" }, { "isnt", "__rk_test_isnt" },
        { "done-testing", "__rk_test_done" }, { "skip-rest", "__rk_test_skip_rest" }, { "skip", "__rk_test_skip" }, { "todo", "__rk_test_todo" }, { "diag", "__rk_test_diag" },
        { "pass", "__rk_test_pass" }, { "flunk", "__rk_test_flunk" }, { "subtest", "__rk_test_subtest" }, { "is-deeply", "__rk_test_is_deeply" }, { "is-approx", "__rk_test_is_approx" },
        { "isa-ok", "__rk_test_isa_ok" }, { "does-ok", "__rk_test_does_ok" }, { "cmp-ok", "__rk_test_cmp_ok" }, { "lives-ok", "__rk_test_lives_ok" }, { "dies-ok", "__rk_test_dies_ok" },
        { "throws-like", "__rk_test_throws_like" }, { "eval-lives-ok", "__rk_test_eval_lives_ok" }, { "eval-dies-ok", "__rk_test_eval_dies_ok" }, { NULL, NULL } };
    for (int i = 0; map[i][0]; i++) if (!strcmp(map[i][0], s)) return map[i][1];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_testop(const char *s) { return s && testop_rt(s) != NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_testop_call(const char *name, TL *a) {
    tree_t *c = make_call(name);
    if (!a) return c;
    if (a->n == 1 && a->v[0] && a->v[0]->t == TT_FNC && a->v[0]->v.sval && !strcmp(a->v[0]->v.sval, "__rk_arr")) {
        tree_t *lst = a->v[0];
        for (int i = 1; i < lst->n; i++) expr_add_child(c, lst->c[i]);
        return c;
    }
    if (a->n == 1 && a->v[0] && a->v[0]->t == TT_FNC && a->v[0]->v.sval && !strcmp(a->v[0]->v.sval, "__rk_pair") && !strcmp(name, "__rk_test_subtest") && a->v[0]->n >= 3) {
        tree_t *pr = a->v[0];
        for (int i = 1; i < pr->n; i++) expr_add_child(c, pr->c[i]);
        return c;
    }
    for (int i = 0; i < a->n; i++) expr_add_child(c, a->v[i]);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_junction(const char *flav, tree_t *l, tree_t *r) {
    tree_t *e = make_call(flav);
    if (l && l->t == TT_FNC && l->v.sval && strcmp(l->v.sval, flav) == 0) { for (int i = 1; i < l->n; i++) expr_add_child(e, l->c[i]); }
    else expr_add_child(e, l);
    expr_add_child(e, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_adverb_bool(int v) { tree_t *b = make_call("__rk_mkbool"); expr_add_child(b, rk_ilit(v)); return b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_multi_mangle(const char *base, TL *params) {
    int np = params ? params->n : 0;
    SB b = { 0 };
    for (const char *c = fmt("%s$%d", base, np); *c; c++) sb_c(&b, *c);
    for (int i = 0; i < np; i++) {
        tree_t *p = params->v[i];
        const char *ty = (p && p->n > 0 && p->c[0] && p->c[0]->v.sval) ? p->c[0]->v.sval : "Any";
        if (!strcmp(ty, "*@") || !strcmp(ty, "**@")) ty = "Slurpy";
        sb_c(&b, '$');
        for (const char *c = ty; *c; c++) sb_c(&b, (*c == ':') ? '_' : *c);
    }
    return sb_str(&b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_typed_param(RkB *b, const char *type, const char *name) { tree_t *p = var_node(b, name); expr_add_child(p, leaf_sval(TT_QLIT, type)); return p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *make_seq(TL *stmts) { tree_t *seq = ast_node_new(TT_SEQ_EXPR); if (stmts) for (int i = 0; i < stmts->n; i++) expr_add_child(seq, stmts->v[i]); return seq; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *seq1(tree_t *stmt) { tree_t *seq = ast_node_new(TT_SEQ_EXPR); if (stmt) expr_add_child(seq, stmt); return seq; }
/*====================================================================================================================================================================================================*/
#define RK_PH_BODY 4
#define RK_PH_DROP (-1)
#define RK_PH_LOOP (-2)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_phaser_mark(const char *name, tree_t *body) {
    tree_t *m = make_call(fmt("__rk_phaser_%s", name ? name : "")); expr_add_child(m, body ? body : ast_node_new(TT_SEQ_EXPR)); return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_phaser_of(tree_t *n) { if (!n || n->t != TT_FNC || !n->v.sval || strncmp(n->v.sval, "__rk_phaser_", 12) != 0) return NULL; return n->v.sval + 12; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_phaser_body(tree_t *n) { return (n && n->n >= 2) ? n->c[n->n - 1] : ast_node_new(TT_SEQ_EXPR); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_phaser_is_loop(const char *p) { return !strcmp(p, "FIRST") || !strcmp(p, "LAST") || !strcmp(p, "NEXT") || !strcmp(p, "once"); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_phaser_unplaced(const char *p) {
    extern void rt_script_die_surface(const char *msg);
    rt_script_die_surface(fmt("%s { } is implemented at mainline scope and as a loop phaser at the top level of a loop body, not here", p));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_phaser_inline_tree(tree_t *n) {
    if (!n) return;
    for (int i = 0; i < n->n; i++) {
        const char *p = rk_phaser_of(n->c[i]);
        if (p) { if (rk_phaser_is_loop(p)) rk_phaser_unplaced(p); n->c[i] = rk_phaser_body(n->c[i]); }
        rk_phaser_inline_tree(n->c[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static TL *rk_phasers_place(TL *l, int mainline) {
    int any = 0; tree_t *tail_ret = NULL;
    for (int i = 0; i < l->n; i++) if (rk_phaser_of(l->v[i])) { any = 1; break; }
    if (!any) return l;
    if (!mainline && l->n > 0 && l->v[l->n - 1] && l->v[l->n - 1]->t == TT_RETURN) tail_ret = l->v[--l->n];
    TL *out = (TL *) ct_zalloc(1, sizeof(TL));
    for (int rank = 0; rank <= 7; rank++) {
        for (int i = 0; i < l->n; i++) {
            tree_t *it = l->v[i];
            const char *p = rk_phaser_of(it);
            int r = p ? rk_phaser_rank(p, mainline) : RK_PH_BODY;
            if (r == RK_PH_DROP) continue;
            if (r == RK_PH_LOOP && !mainline) { if (rank == RK_PH_BODY) tl_add(out, it); continue; }
            if (r == RK_PH_LOOP) r = RK_PH_BODY;
            if (r != rank) continue;
            tl_add(out, p ? rk_phaser_body(it) : it);
        }
    }
    if (tail_ret) tl_add(out, tail_ret);
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
    switch (r->t) { case TT_VAR: case TT_ARR_GET: case TT_HASH_GET: case TT_FIELD: case TT_TWIGIL_FIELD: case TT_INDIRECT: return 1; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_bind(tree_t *target, tree_t *rhs) { if (rk_rhs_is_aliasable(rhs)) return NULL; return expr_binary(TT_ASSIGN, target, rhs); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_logical_and(tree_t *a, tree_t *c) { tree_t *s = expr_binary(TT_SEQ, a, c); s->v.ival = 1; return s; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_seq_is_logical_and(const tree_t *t) { return t && t->t == TT_SEQ && t->n == 2 && t->v.ival == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_tail_value(TL *l) {
    if (!l || l->n <= 0) return;
    tree_t *last = l->v[l->n - 1];
    if (!last || (rk_tail_is_statement(last->t) && !rk_seq_is_logical_and(last))) return;
    tree_t *r = ast_node_new(TT_RETURN); r->line = last->line; expr_add_child(r, last); l->v[l->n - 1] = r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_phaser_join(tree_t *a, tree_t *c) { if (!a) return c; TL l = { 0 }; tl_add(&l, a); tl_add(&l, c); return make_seq(&l); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_loop_phasers(RkB *b, tree_t *loop, tree_t *body) {
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
    const char *g = fmt("__rk_ph_g%d", b->after_line);
    TL inner = { 0 };
    if (first) {
        tree_t *t = ast_node_new(TT_SEQ_EXPR); expr_add_child(t, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(0))); expr_add_child(t, first);
        tree_t *iff = ast_node_new(TT_IF); expr_add_child(iff, leaf_sval(TT_VAR, g)); expr_add_child(iff, t); tl_add(&inner, iff);
    }
    else if (last) tl_add(&inner, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(0)));
    for (int i = 0; i < keep; i++) tl_add(&inner, body->c[i]);
    if (nxt) tl_add(&inner, nxt);
    body->n = 0;
    for (int i = 0; i < inner.n; i++) ast_push(body, inner.v[i]);
    if (!first && !last) return loop;
    TL outer = { 0 };
    tl_add(&outer, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, g), rk_ilit(1)));
    tl_add(&outer, loop);
    if (last) { tree_t *u = ast_node_new(TT_UNLESS); ast_push(u, leaf_sval(TT_VAR, g)); ast_push(u, seq1(last)); tl_add(&outer, u); }
    return make_seq(&outer);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_cstyle_loop(tree_t *init, tree_t *cond, tree_t *incr, tree_t *body) {
    tree_t *n = ast_node_new(TT_CLOOP); expr_add_child(n, init); expr_add_child(n, cond); expr_add_child(n, incr); expr_add_child(n, body); return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_quiet_store(tree_t *target, tree_t *rhs) { tree_t *a = expr_binary(TT_ASSIGN, target, rhs); a->v.ival = 1; return a; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_incdec(RkB *b, const char *var, int add) { return rk_quiet_store(var_node(b, var), expr_binary(add ? TT_ADD : TT_SUB, var_node(b, var), rk_ilit(1))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_post_incdec(RkB *b, const char *var, int add) {
    const char *tmp = fmt("__post_%d", b->post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), var_node(b, var)));
    expr_add_child(seq, rk_quiet_store(var_node(b, var), expr_binary(add ? TT_ADD : TT_SUB, var_node(b, var), rk_ilit(1))));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tw_field(const char *name) { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(name)); return fe; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tw_post_incdec(RkB *b, const char *var, int add) {
    const char *tmp = fmt("__twpost_%d", b->twpost_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, tmp), rk_tw_field(var)));
    expr_add_child(seq, expr_binary(TT_ASSIGN, rk_tw_field(var), expr_binary(add ? TT_ADD : TT_SUB, rk_tw_field(var), rk_ilit(1))));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_group_targets(tree_t *grp, TL *t) {
    if (grp && grp->t == TT_FNC && grp->v.sval && strcmp(grp->v.sval, "__rk_arr") == 0) { for (int i = 1; i < grp->n; i++) tl_add(t, grp->c[i]); }
    else if (grp) tl_add(t, grp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_destructure(RkB *b, TL *targets, tree_t *rhs_arr) {
    const char *tmp = fmt("__destr_%d", b->destr_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), rhs_arr));
    for (int i = 0; targets && i < targets->n; i++) {
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, tmp)); expr_add_child(get, rk_ilit(i));
        expr_add_child(seq, rk_quiet_store(targets->v[i], get));
    }
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_rhs(tree_t *rhs);
static tree_t *rk_for_multi(RkB *b, TL *vars, tree_t *list, tree_t *body) {
    const char *av = fmt("__fm_a_%d", b->fm_uid), *iv = fmt("__fm_i_%d", b->fm_uid); b->fm_uid++;
    int k = vars ? vars->n : 0;
    if (list && list->t == TT_TO && list->n >= 2) list = rk_arr_rhs(list);
    tree_t *hoist = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(av)), list);
    tree_t *init = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), rk_ilit(0));
    tree_t *el = make_call("elems"); expr_add_child(el, leaf_sval(TT_VAR, intern(av)));
    tree_t *cond = expr_binary(TT_LT, leaf_sval(TT_VAR, intern(iv)), el);
    tree_t *incr = expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, intern(iv)), expr_binary(TT_ADD, leaf_sval(TT_VAR, intern(iv)), rk_ilit(k)));
    tree_t *seq = ast_node_new(TT_SEQ);
    for (int i = 0; i < k; i++) {
        tree_t *idx = leaf_sval(TT_VAR, intern(iv));
        if (i) idx = expr_binary(TT_ADD, idx, rk_ilit(i));
        tree_t *get = make_call("__rk_arr_at"); expr_add_child(get, leaf_sval(TT_VAR, intern(av))); expr_add_child(get, idx);
        expr_add_child(seq, expr_binary(TT_ASSIGN, vars->v[i], get));
    }
    expr_add_child(seq, body);
    tree_t *outer = ast_node_new(TT_SEQ);
    expr_add_child(outer, hoist); expr_add_child(outer, rk_cstyle_loop(init, cond, incr, seq));
    return outer;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_with_mod(tree_t *stmt, tree_t *cond, int negate) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, cond);
    tree_t *dcall = make_call("__rk_defined"); expr_add_child(dcall, leaf_sval(TT_VAR, "_"));
    tree_t *gate = ast_node_new(negate ? TT_UNLESS : TT_IF); expr_add_child(gate, dcall); expr_add_child(gate, seq1(stmt));
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, gate);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_given_mod(tree_t *stmt, tree_t *topicval) {
    tree_t *topic = ast_node_new(TT_ASSIGN); expr_add_child(topic, leaf_sval(TT_VAR, "_")); expr_add_child(topic, topicval);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, topic); expr_add_child(seq, stmt);
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_range_ex(RkB *b, tree_t *lo, tree_t *hi) {
    if (hi && hi->t == TT_ILIT) return expr_binary(TT_TO, lo, rk_ilit(hi->v.ival - 1));
    if (hi && hi->t == TT_VAR && is_arr(b, hi->v.sval)) { tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, hi); ast_push(el, leaf_sval(TT_QLIT, "elems")); hi = el; }
    return expr_binary(TT_TO, lo, expr_binary(TT_SUB, hi, rk_ilit(1)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *nctx(RkB *b, tree_t *e) {
    if (e && e->t == TT_VAR && is_arr(b, e->v.sval)) { tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, e); ast_push(el, leaf_sval(TT_QLIT, "elems")); return el; }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_one_scalar(const tree_t *e) {
    if (!e) return 0;
    switch (e->t) {
    case TT_QLIT: case TT_ILIT: case TT_FLIT: case TT_CAT: case TT_XREP: case TT_ADD: case TT_SUB: case TT_MUL: case TT_DIV: case TT_MOD: case TT_POW: case TT_MNS: return 1;
    case TT_VAR: return e->v.sval && e->v.sval[0] != '@' && e->v.sval[0] != '%' && !(e->slen & 1);
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs) return rhs;
    if (rhs->t == TT_TO && rhs->n >= 2) {
        tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]); return call;
    }
    if (!rk_one_scalar(rhs)) return rhs;
    tree_t *call = make_call("__rk_arr"); expr_add_child(call, rhs); return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_index(RkB *b, const char *arr, tree_t *idx) {
    if (idx && idx->t == TT_TO && idx->n >= 2) {
        tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(b, arr)); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]); return call;
    }
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(b, arr)); ast_push(c, idx); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_dec(tree_t *hi) { if (hi && hi->t == TT_ILIT) return rk_ilit(hi->v.ival - 1); return expr_binary(TT_SUB, hi, rk_ilit(1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_end_index(RkB *b, const char *arr, tree_t *off, tree_e op) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(b, arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, var_node(b, arr)); ast_push(c, expr_binary(op, el, off)); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_arr_all(RkB *b, const char *arr) {
    tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, var_node(b, arr)); ast_push(el, leaf_sval(TT_QLIT, "elems"));
    tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, var_node(b, arr)); expr_add_child(call, rk_ilit(0)); expr_add_child(call, rk_dec(el));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v; c->line = e->line; c->slen = e->slen;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_scalar_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_XREP || rhs->n < 2) return rhs;
    tree_t *c = make_call("__rk_rep"); expr_add_child(c, rhs->c[0]); expr_add_child(c, rhs->c[1]); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_named_call(const char *fname, TL *pos, TL *named) {
    tree_t *c = make_call("__rk_named_call");
    expr_add_child(c, leaf_sval(TT_QLIT, fname));
    expr_add_child(c, rk_ilit(pos ? pos->n : 0));
    if (pos) for (int i = 0; i < pos->n; i++) expr_add_child(c, pos->v[i]);
    if (named) for (int i = 0; i < named->n; i++) expr_add_child(c, named->v[i]);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_adhoc_new(const char *cname, TL *named, TL *pos) {
    if (!cname || strcmp(cname, "X::AdHoc")) return NULL;
    tree_t *val = NULL;
    if (named) for (int i = 0; i + 1 < named->n; i += 2) {
        tree_t *k = named->v[i]; const char *ks = k ? k->v.sval : NULL;
        if (ks && (!strcmp(ks, "payload") || !strcmp(ks, "message"))) { val = named->v[i + 1]; break; }
    }
    if (!val && pos && pos->n > 0) val = pos->v[0];
    if (!val) { val = ast_node_new(TT_QLIT); val->v.sval = (char *) intern(""); }
    return val;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_catch_when_cond(tree_t *e) { if (e && e->t == TT_VAR && e->v.sval && !strcmp(e->v.sval, "X::AdHoc")) return ast_node_new(TT_NUL); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_defaults_prologue(TL *params, tree_t *body) {
    if (!params) return body;
    TL pro = { 0 };
    for (int i = 0; i < params->n; i++) {
        tree_t *p = params->v[i];
        if (!p || p->t != TT_ASSIGN || p->n < 2) continue;
        tree_t *pv = p->c[0]; tree_t *dv = p->c[1];
        params->v[i] = pv;
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, rk_tree_clone(pv)); ast_push(mc, leaf_sval(TT_QLIT, "defined"));
        tree_t *un = ast_node_new(TT_UNLESS); ast_push(un, mc); ast_push(un, seq1(expr_binary(TT_ASSIGN, rk_tree_clone(pv), dv)));
        tl_add(&pro, un);
    }
    if (!pro.n) return body;
    for (int i = 0; body && i < body->n; i++) tl_add(&pro, body->c[i]);
    return make_seq(&pro);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_chain_cmp(tree_e k) { return k == TT_LT || k == TT_GT || k == TT_LE || k == TT_GE || k == TT_EQ || k == TT_NE || k == TT_LEQ || k == TT_LNE; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_last_operand(tree_t *left) {
    if (!left) return NULL;
    if (rk_is_chain_cmp(left->t) && left->n == 2) return expr_right(left);
    if (left->t == TT_SEQ && left->n == 2) return rk_chain_last_operand(expr_right(left));
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_chain_cmp(tree_t *left, tree_e op, tree_t *right) {
    tree_t *last = rk_chain_last_operand(left);
    if (last) return rk_logical_and(left, expr_binary(op, rk_tree_clone(last), right));
    return expr_binary(op, left, right);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_order_call(const char *fn, tree_t *left, tree_t *right) {
    tree_t *c = ast_node_new(TT_FNC); c->v.sval = (char *) intern(fn);
    ast_push(c, leaf_sval(TT_VAR, fn)); ast_push(c, left); ast_push(c, right); return c;
}
/*====================================================================================================================================================================================================*/
static tree_t *rk_interp_primary(RkB *b, const char *s, int *ip, int len) {
    int i = *ip;
    while (i < len && s[i] == ' ') i++;
    if (i < len && (s[i] == '$' || s[i] == '@')) {
        char sig = s[i]; i++;
        SB nm = { 0 }; sb_c(&nm, sig);
        while (i < len && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) { sb_c(&nm, s[i]); i++; }
        *ip = i;
        return var_node(b, sb_str(&nm));
    }
    if (i < len && s[i] >= '0' && s[i] <= '9') {
        long v = 0;
        while (i < len && s[i] >= '0' && s[i] <= '9') { v = v * 10 + (s[i] - '0'); i++; }
        *ip = i;
        return rk_ilit(v);
    }
    *ip = i; return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_interp_subexpr(RkB *b, const char *s, int *ip, int len) {
    tree_t *left = rk_interp_primary(b, s, ip, len);
    if (!left) return NULL;
    int i = *ip;
    while (i < len && s[i] == ' ') i++;
    if (i < len && (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '%')) {
        char op = s[i]; i++; *ip = i;
        tree_t *right = rk_interp_primary(b, s, ip, len);
        if (!right) return left;
        tree_e k = op == '+' ? TT_ADD : op == '-' ? TT_SUB : op == '*' ? TT_MUL : op == '/' ? TT_DIV : TT_MOD;
        return expr_binary(k, nctx(b, left), nctx(b, right));
    }
    *ip = i; return left;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *lower_interp_str(RkB *b, const char *s) {
    int len = s ? (int) strlen(s) : 0;
    tree_t *result = NULL;
    SB lit = { 0 }; int i = 0;
    while (i < len) {
        if (s[i] == '$' && i + 1 < len && (s[i + 1] == '_' || (s[i + 1] >= 'A' && s[i + 1] <= 'Z') || (s[i + 1] >= 'a' && s[i + 1] <= 'z'))) {
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            i++;
            SB vn = { 0 };
            while (i < len && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) { sb_c(&vn, s[i]); i++; }
            tree_t *var = leaf_sval(TT_VAR, sb_str(&vn));
            result = result ? expr_binary(TT_CAT, result, var) : var;
        }
        else if (s[i] == '@' && i + 1 < len && (s[i + 1] == '_' || (s[i + 1] >= 'A' && s[i + 1] <= 'Z') || (s[i + 1] >= 'a' && s[i + 1] <= 'z'))) {
            if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; lit.n = 0; }
            SB vn = { 0 }; sb_c(&vn, s[i]); i++;
            while (i < len && (s[i] == '_' || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9'))) { sb_c(&vn, s[i]); i++; }
            char *vname = sb_str(&vn);
            tree_t *arrpart;
            if (i < len && s[i] == '[') {
                i++;
                tree_t *idx = rk_interp_subexpr(b, s, &i, len);
                while (i < len && s[i] != ']') i++;
                if (i < len && s[i] == ']') i++;
                arrpart = idx ? rk_arr_index(b, vname, idx) : leaf_sval(TT_VAR, vname);
            }
            else arrpart = leaf_sval(TT_VAR, vname);
            result = result ? expr_binary(TT_CAT, result, arrpart) : arrpart;
        }
        else { sb_c(&lit, s[i]); i++; }
    }
    if (lit.n > 0) { tree_t *lq = leaf_sval(TT_QLIT, sb_str(&lit)); result = result ? expr_binary(TT_CAT, result, lq) : lq; }
    return result ? result : leaf_sval(TT_QLIT, "");
}
/*====================================================================================================================================================================================================*/
static const char *named_rule_class(const char *n, int len) {
    static const char *const map[][2] = { { "alpha", "[A-Za-z]" }, { "digit", "[0-9]" }, { "alnum", "[A-Za-z0-9]" }, { "upper", "[A-Z]" }, { "lower", "[a-z]" },
                                          { "space", "\\s" }, { "xdigit", "[0-9A-Fa-f]" }, { "ws", "\\s*" }, { "punct", "[!-/:-@\\[-`{-~]" }, { NULL, NULL } };
    for (int i = 0; map[i][0]; i++) if ((int) strlen(map[i][0]) == len && !strncmp(map[i][0], n, (size_t) len)) return map[i][1];
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int re_is_meta(char c) { return c && strchr("\\()[]{}<>|*+?.^$", c) != NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *regex_to_engine(const char *r) {
    size_t n = strlen(r); char *o = (char *) ct_alloc(n * 4 + 16); size_t k = 0; int sp = 0;
    for (size_t i = 0; i < n; ) {
        char c = r[i];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { i++; continue; }
        if (c == '#') { while (i < n && r[i] != '\n') i++; continue; }
        if (c == '\'' || c == '"') { char q = c; i++; while (i < n && r[i] != q) { if (r[i] == '\\' && i + 1 < n) i++; if (re_is_meta(r[i])) o[k++] = '\\'; o[k++] = r[i++]; } if (i < n) i++;
            continue; }
        if (c == '\\' && i + 1 < n) { o[k++] = r[i++]; o[k++] = r[i++]; continue; }
        if (c == '|' && i + 1 < n && r[i + 1] == '|') { o[k++] = '|'; i += 2; continue; }
        if (c == '$' && i + 1 < n && r[i + 1] == '<') {
            size_t j = i + 2; while (j < n && (isalnum((unsigned char) r[j]) || r[j] == '_' || r[j] == '-')) j++;
            if (j < n && r[j] == '>' && j + 2 < n && r[j + 1] == '=' && r[j + 2] == '(') { o[k++] = '<'; memcpy(o + k, r + i + 2, j - i - 2); k += j - i - 2; o[k++] = '>'; o[k++] = '('; sp++;
                i = j + 3; continue; }
        }
        if (c == '<' && i + 1 < n && (r[i + 1] == '[' || (r[i + 1] == '-' && i + 2 < n && r[i + 2] == '['))) {
            int neg = r[i + 1] == '-'; i += neg ? 3 : 2; o[k++] = '['; if (neg) o[k++] = '^';
            for (;;) {
                while (i < n && r[i] != ']') {
                    char d = r[i];
                    if (d == ' ' || d == '\t' || d == '\n') { i++; continue; }
                    if (d == '\\' && i + 1 < n) { o[k++] = r[i++]; o[k++] = r[i++]; continue; }
                    if (d == '.' && i + 1 < n && r[i + 1] == '.') { o[k++] = '-'; i += 2; continue; }
                    if (d == '-' || d == '^' || d == ']') o[k++] = '\\';
                    o[k++] = r[i++];
                }
                if (i < n) i++;
                if (i + 1 < n && r[i] == '+' && r[i + 1] == '[') { i += 2; continue; }
                break;
            }
            o[k++] = ']'; if (i < n && r[i] == '>') i++;
            continue;
        }
        if (c == '<') {
            size_t j = i + 1; while (j < n && (isalnum((unsigned char) r[j]) || r[j] == '_' || r[j] == '-')) j++;
            const char *cls = (j < n && r[j] == '>') ? named_rule_class(r + i + 1, (int) (j - i - 1)) : NULL;
            if (cls) { size_t L = strlen(cls); memcpy(o + k, cls, L); k += L; i = j + 1; continue; }
        }
        if (c == '[') { o[k++] = '('; o[k++] = '?'; o[k++] = ':'; sp++; i++; continue; }
        if (c == ']') { if (sp > 0) sp--; o[k++] = ')'; i++; continue; }
        if (c == '(') { sp++; o[k++] = r[i++]; continue; }
        if (c == ')') { if (sp > 0) sp--; o[k++] = r[i++]; continue; }
        o[k++] = r[i++];
    }
    o[k] = '\0';
    return o;
}
/*====================================================================================================================================================================================================*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dq_closure_end(const char *s, int i, int n) {
    int j = i + 1;
    for (;;) {
        if (j >= n) return -1;
        char c = s[j];
        if (c == '}') return j + 1;
        if (c == '"' || c == '\n') return -1;
        if (c == '{') {
            int k = j + 1;
            while (k < n && s[k] != '}') { if (s[k] == '{' || s[k] == '"' || s[k] == '\n') return -1; k++; }
            if (k >= n) return -1;
            j = k + 1; continue;
        }
        j++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int ishex(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *dq_segment(RkB *b, const char *seg) {
    if (strchr(seg, '$') || strchr(seg, '@')) return lower_interp_str(b, seg);
    return leaf_sval(TT_QLIT, seg);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static RkClosure *closure_at(RkClosure *cl, int ncl, int pos) { for (int i = 0; i < ncl; i++) if (cl[i].pos == pos) return &cl[i]; return NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *closure_expr(RkB *b, RkClosure *c) { if (!c) return leaf_sval(TT_QLIT, ""); return rkb_paren(b, c->last, c->cnt); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int dq_named_chars(SB *buf, const char *p, int n) {
    int start = buf->n, k = 0;
    while (k < n) {
        int e = k; while (e < n && p[e] != ',') e++;
        const char *seq = rk_uniname_seq(p + k, e - k);
        if (seq) { while (*seq) { char *x; unsigned long v = strtoul(seq, &x, 10); sb_cp(buf, v); seq = *x == ',' ? x + 1 : x; } }
        else { unsigned v = rk_uniname_cp(p + k, e - k); if (!v) { buf->n = start; return 0; } sb_cp(buf, v); }
        k = e + 1;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_dq(RkB *b, RkClosure *cl, int ncl, int from, int to) {
    const char *s = b->s; SB buf = { 0 };
    typedef struct { int at; int pos; } Mk;
    Mk *mk = NULL; int nmk = 0, cmk = 0;
    int i = from;
    while (i < to) {
        char c = s[i];
        if (c == '\\' && i + 1 < to) {
            char d = s[i + 1];
            if (d == '{') { sb_c(&buf, '{'); i += 2; continue; }
            if (d == '}') { sb_c(&buf, '}'); i += 2; continue; }
            if (d == 'n') { sb_c(&buf, '\n'); i += 2; continue; }
            if (d == 't') { sb_c(&buf, '\t'); i += 2; continue; }
            if (d == 'x' && i + 2 < to && s[i + 2] == '[') {
                int j = i + 3; while (j < to && (ishex(s[j]) || s[j] == ',' || s[j] == ' ')) j++;
                if (j < to && s[j] == ']' && j > i + 3) {
                    const char *p = s + i + 3;
                    while (p < s + j) { while (p < s + j && (*p == ' ' || *p == ',')) p++; if (p >= s + j) break; char *e; unsigned long cp = strtoul(p, &e, 16); if (e == p) break; sb_cp(&buf, cp);
                        p = e; }
                    i = j + 1; continue;
                }
            }
            if (d == 'x' && i + 2 < to && ishex(s[i + 2])) { int j = i + 2; while (j < to && ishex(s[j])) j++; sb_cp(&buf, strtoul(spn(b, i + 2, j), NULL, 16)); i = j; continue; }
            if (d == 'c' && i + 2 < to && s[i + 2] == '[') {
                int j = i + 3; while (j < to && s[j] != ']') j++;
                if (j < to && dq_named_chars(&buf, s + i + 3, j - i - 3)) { i = j + 1; continue; }
            }
            if (d == 'c' && i + 2 < to && isdigit((unsigned char) s[i + 2])) { int j = i + 2; while (j < to && isdigit((unsigned char) s[j])) j++; sb_cp(&buf, strtoul(spn(b, i + 2, j), NULL, 10)); i = j; continue; }
            if (d == 'c' && i + 2 < to && strchr("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_?", s[i + 2])) { sb_cp(&buf, s[i + 2] == '?' ? 127u : (unsigned) (s[i + 2] ^ 0x40)); i += 3; continue; }
            if (d == 'b') { sb_c(&buf, '\b'); i += 2; continue; }
            if (d == 'r') { sb_c(&buf, '\r'); i += 2; continue; }
            if (d == 'a') { sb_c(&buf, '\a'); i += 2; continue; }
            if (d == 'e') { sb_c(&buf, '\033'); i += 2; continue; }
            if (d == 'f') { sb_c(&buf, '\f'); i += 2; continue; }
            if (d == '\\') { sb_c(&buf, '\\'); i += 2; continue; }
            if (d == '"') { sb_c(&buf, '"'); i += 2; continue; }
            sb_c(&buf, c); i++; continue;
        }
        if (c == '{') {
            int e = dq_closure_end(s, i, to);
            if (e > 0) { GROW(mk, nmk, cmk, Mk); mk[nmk].at = buf.n; mk[nmk].pos = i; nmk++; sb_c(&buf, '\x02'); i = e; continue; }
        }
        sb_c(&buf, c); i++;
    }
    char *str = sb_str(&buf);
    if (!nmk) return dq_segment(b, str);
    tree_t *acc = NULL; int segstart = 0;
    for (int k = 0; k <= nmk; k++) {
        int segend = k < nmk ? mk[k].at : buf.n;
        char *seg = (char *) ct_alloc((size_t) (segend - segstart) + 1); memcpy(seg, str + segstart, (size_t) (segend - segstart)); seg[segend - segstart] = 0;
        tree_t *st = leaf_sval(TT_QLIT, seg);
        if (strchr(seg, '$') || strchr(seg, '@')) st = lower_interp_str(b, seg);
        acc = acc ? expr_binary(TT_CAT, acc, st) : st;
        if (k < nmk) { acc = expr_binary(TT_CAT, acc, closure_expr(b, closure_at(cl, ncl, mk[k].pos))); segstart = mk[k].at + 1; }
    }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_sq(RkB *b, int from, int to) {
    SB buf = { 0 };
    for (int i = from; i < to; ) {
        if (b->s[i] == '\\' && i + 1 < to && (b->s[i + 1] == '\'' || b->s[i + 1] == '\\')) { sb_c(&buf, b->s[i + 1]); i += 2; continue; }
        sb_c(&buf, b->s[i]); i++;
    }
    return leaf_sval(TT_QLIT, sb_str(&buf));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *regex_body(RkB *b, int from, int to, int stop) {
    SB buf = { 0 };
    for (int i = from; i < to; i++) {
        if (b->s[i] == '\\' && i + 1 < to && b->s[i + 1] == '/') { sb_c(&buf, '/'); i++; continue; }
        if (b->s[i] == stop) break;
        sb_c(&buf, b->s[i]);
    }
    return sb_str(&buf);
}
/*====================================================================================================================================================================================================*/
static int words_ok(const char *s, int n) {
    int words = 0, inw = 0;
    for (int i = 0; i < n; i++) {
        char c = s[i];
        if (c == ' ' || c == '\t') { inw = 0; continue; }
        if (!(isalnum((unsigned char) c) || strchr("_.:/*+?=!$,;#@|\\-", c))) return 0;
        if (!inw) { words++; inw = 1; }
    }
    return words >= 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_words(RkB *b, const char *s, int n) {
    tree_t *call = make_call("__rk_arr"); int wc = 0;
    int i = 0;
    while (i < n) {
        while (i < n && (s[i] == ' ' || s[i] == '\t')) i++;
        if (i >= n) break;
        int w = i; while (i < n && s[i] != ' ' && s[i] != '\t') i++;
        SB t = { 0 };
        for (int j = w; j < i; j++) { if (s[j] == '\\' && j + 1 < i && s[j + 1] == '\\') { sb_c(&t, '\\'); j++; } else sb_c(&t, s[j]); }
        expr_add_child(call, leaf_sval(TT_QLIT, sb_str(&t))); wc++;
    }
    if (wc == 1) { tree_t *only = call->c[1]; return only; }
    (void) b;
    return call;
}
/*====================================================================================================================================================================================================*/
RkB *rkb_new(const char *src, int len) {
    RkB *b = (RkB *) ct_zalloc(1, sizeof(RkB)); b->s = src; b->len = len;
    for (int i = 0; i < len; i++) if (src[i] == '\n') { GROW(b->nlp, b->nnlp, b->cnlp, int); b->nlp[b->nnlp++] = i; }
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_set_after_line(RkB *b, int pos) { int q = pos; while (q > 0 && isspace((unsigned char) b->s[q - 1])) q--; b->after_line = line_at(b, q > 0 ? q - 1 : 0); }
/*====================================================================================================================================================================================================*/
RkList *rkb_list_new(void) { return (RkList *) ct_zalloc(1, sizeof(RkList)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RkEl *rkb_list_el(RkList *L) { GROW(L->v, L->n, L->cap, RkEl); RkEl *e = &L->v[L->n++]; memset(e, 0, sizeof *e); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_list_end(RkList *L) { if (L->n && L->v[L->n - 1].nitem == 1 && L->v[L->n - 1].t0.kind == TK_EMPTY) { L->n--; L->trailing = L->nitem > 1; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_pend(RkEl *e, tree_t *ph, RkDecl *d) { GROW(e->pd, e->npd, e->cpd, RkPend); e->pd[e->npd].ph = ph; e->pd[e->npd].d = d; e->npd++; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *build_decl(RkB *b, RkDecl *d, RkList *outer, int ofirst);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pend_fill(RkB *b, RkPend *q) { RkDecl *d = q->d; if (!d) return; q->d = NULL; tree_t *t = build_decl(b, d, NULL, 0); *q->ph = *t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void el_fill(RkB *b, RkEl *e) { for (int i = 0; i < e->npd; i++) pend_fill(b, &e->pd[i]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *el_tree(RkB *b, RkEl *e) {
    el_fill(b, e);
    tree_t *t = e->t ? e->t : ast_node_new(TT_NUL);
    if (t == e->t0.t || (e->npd && t == e->pd[0].ph)) return t;
    tree_t *c = ast_node_new(t->t); c->v = t->v; c->line = t->line; c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, t->c[i]);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *el_rest(RkB *b, RkEl *e) { el_fill(b, e); return e->rest; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *el_t0(RkB *b, RkEl *e) {
    if (e->t0.kind == TK_DECL && !e->t0.t && e->npd) { pend_fill(b, &e->pd[0]); return e->pd[0].ph; }
    return e->t0.t ? e->t0.t : ast_node_new(TT_NUL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plain(RkTerm *t, int cls) { return t && t->kind == TK_VAR && t->cls == cls && !t->npost && !t->npre; }
/*====================================================================================================================================================================================================*/
static const char *const lv_mul[] = { "*", "\xc3\xb7", "\xc3\x97", "~&", "mod", "lcm", "/", "%", "div", "gcd", "+&", "+<", 0 };
static const char *const lv_addsub[] = { "+", "\xe2\x88\x92", "?^", "?|", "~|", "+|", "-", 0 };
static const char *const lv_repl[] = { "x", "xx", 0 };
static const char *const lv_cat[] = { "~", "\xe2\x88\x98", "o", 0 };
static const char *const lv_range1[] = { "..", "...", "..^", 0 };
static const char *const lv_range2[] = { "unicmp", "coll", "^..^", "^..", 0 };
static const char *const lv_dor[] = { "//", 0 };
static const char *const lv_jct[] = { "|", "(^)", "(-)", "(+)", "(|)", "(.)", "(&)", "&", 0 };
static const char *const lv_divis[] = { "%%", 0 };
static const char *const lv_cmp[] = { "==", "!=", "<", ">", "<=", ">=", "\xe2\x89\xa4", "\xe2\x89\xa5", "\xe2\x89\xa0", "!~~", "=~=", "(elem)", "(cont)", "after", "before", "eqv", "===",
                                      "eq", "<=>", "cmp", "leg", "ne", "lt", "le", "gt", "ge", "~~", 0 };
static const char *const lv_and[] = { "&&", 0 };
static const char *const lv_or[] = { "||", "^^", "max", "min", 0 };
static const char *const lv_pow[] = { "**", 0 };
static const char *const lv_tern[] = { "??", 0 };
static const char *const lv_pair[] = { "=>", 0 };
static const char *const lv_compound[] = { "+=", "-=", "*=", "/=", "~=", 0 };
static const char *const *const lv_ops[] = { lv_mul, lv_addsub, lv_repl, lv_cat, lv_range1, lv_range2, lv_dor, lv_jct, lv_divis, lv_cmp, lv_and, lv_or, lv_pow, lv_tern, lv_pair, lv_compound };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_op_index(int lv, const char *op) { if (!op) return -1; for (int i = 0; lv_ops[lv][i]; i++) if (!strcmp(lv_ops[lv][i], op)) return i; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_compound_base(const char *op, int *lv, int *k) {
    static const int lvs[] = { LV_MUL, LV_ADDSUB, LV_REPL, LV_CAT, LV_DOR, LV_JCT, LV_DIVIS, LV_AND, LV_OR, LV_POW };
    size_t n = op ? strlen(op) : 0;
    if (n < 2 || op[n - 1] != '=') return 0;
    char *base = trimdup(op, (int) n - 1);
    for (size_t i = 0; i < sizeof lvs / sizeof lvs[0]; i++) { int j = rkb_op_index(lvs[i], base); if (j >= 0) { *lv = lvs[i]; *k = j; return 1; } }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_mul(RkB *b, int k, tree_t *l, tree_t *r) {
    switch (k) {
    case 0: return expr_binary(TT_MUL, nctx(b, l), nctx(b, r));
    case 1: return call2("__rk_div", l, r);
    case 2: return call2("__rk_mul", l, r);
    case 3: return call2("__rk_sband", l, r);
    case 4: return call2("__rk_mod", l, r);
    case 5: return call2("__rk_lcm", l, r);
    case 6: return expr_binary(TT_DIV, nctx(b, l), nctx(b, r));
    case 7: return expr_binary(TT_MOD, nctx(b, l), nctx(b, r));
    case 8: return call2("__rk_intdiv", nctx(b, l), nctx(b, r));
    case 9: return call2("__rk_gcd", l, r);
    case 10: return call2("iand", l, r);
    default: return call2("ishift", l, r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_addsub(RkB *b, int k, tree_t *l, tree_t *r) {
    switch (k) {
    case 0: return expr_binary(TT_ADD, nctx(b, l), nctx(b, r));
    case 1: return call2("__rk_sub", l, r);
    case 2: return call2("__rk_lbxor", l, r);
    case 3: return call2("__rk_lbor", l, r);
    case 4: return call2("__rk_sbor", l, r);
    case 5: return call2("__rk_bor", l, r);
    default: return expr_binary(TT_SUB, nctx(b, l), nctx(b, r));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *b_cmp(RkB *b, int k, tree_t *l, tree_t *r) {
    switch (k) {
    case 0: return rk_chain_cmp(nctx(b, l), TT_EQ, nctx(b, r));
    case 1: case 8: return rk_chain_cmp(nctx(b, l), TT_NE, nctx(b, r));
    case 2: return rk_chain_cmp(nctx(b, l), TT_LT, nctx(b, r));
    case 3: return rk_chain_cmp(nctx(b, l), TT_GT, nctx(b, r));
    case 4: case 6: return rk_chain_cmp(nctx(b, l), TT_LE, nctx(b, r));
    case 5: case 7: return rk_chain_cmp(nctx(b, l), TT_GE, nctx(b, r));
    case 9: return call2("__rk_not_smartmatch", l, r);
    case 10: return call2("__rk_approx", l, r);
    case 11: return call2("__rk_set_elem", l, r);
    case 12: return call2("__rk_set_cont", l, r);
    case 13: return call2("__rk_after", l, r);
    case 14: return call2("__rk_before", l, r);
    case 15: return call2("__rk_eqv", l, r);
    case 16: return call2("__rk_ident", l, r);
    case 17: return expr_binary(TT_LEQ, l, r);
    case 18: return rk_order_call("__rk_cmp3", l, r);
    case 19: return rk_order_call("__rk_cmpg", l, r);
    case 20: return rk_order_call("__rk_leg", l, r);
    case 21: return expr_binary(TT_LNE, l, r);
    case 22: return expr_binary(TT_LLT, l, r);
    case 23: return expr_binary(TT_LLE, l, r);
    case 24: return expr_binary(TT_LGT, l, r);
    default: return expr_binary(TT_LGE, l, r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_binop(RkB *b, int lv, int k, tree_t *l, tree_t *r) {
    static const char *const jfn[] = { NULL, "__rk_set_sym", "__rk_set_dif", "__rk_set_sum", "__rk_set_uni", "__rk_set_mul", "__rk_set_int", NULL };
    switch (lv) {
    case LV_MUL: return b_mul(b, k, l, r);
    case LV_ADDSUB: return b_addsub(b, k, l, r);
    case LV_REPL: return k == 0 ? expr_binary(TT_XREP, l, r) : call2("__rk_arr_xx", l, r);
    case LV_CAT: return k == 0 ? expr_binary(TT_CAT, l, r) : call2("__rk_compose", l, r);
    case LV_RANGE1: return k < 2 ? expr_binary(TT_TO, l, r) : rk_range_ex(b, l, r);
    case LV_RANGE2: return call2(k == 0 ? "__rk_unicmp" : k == 1 ? "__rk_coll" : k == 2 ? "__rk_range_xb" : "__rk_range_xl", l, r);
    case LV_DOR: return call2("__rk_dor", l, r);
    case LV_JCT: return k == 0 ? mk_junction("any", l, r) : k == 7 ? mk_junction("all", l, r) : call2(jfn[k], l, r);
    case LV_DIVIS: return expr_binary(TT_DIVIS, l, r);
    case LV_CMP: return b_cmp(b, k, l, r);
    case LV_AND: return rk_logical_and(l, r);
    case LV_OR: return k == 0 ? expr_binary(TT_ALT, l, r) : call2(k == 1 ? "__rk_xor" : k == 2 ? "__rk_max" : "__rk_min", l, r);
    case LV_POW: return expr_binary(TT_POW, nctx(b, l), nctx(b, r));
    case LV_PAIR: { tree_t *pc = make_call("__rk_pair"); expr_add_child(pc, l); expr_add_child(pc, r); return pc; }
    default: return l;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_ternary(RkB *b, tree_t *l, tree_t *mid, tree_t *r) {
    (void) b;
    tree_t *t = ast_node_new(TT_TERNARY); ast_push(t, l); ast_push(t, mid ? mid : ast_node_new(TT_NUL)); ast_push(t, r); return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_assign(RkB *b, int cls, const char *name, int k, tree_t *r) {
    static const tree_e ck[] = { TT_ADD, TT_SUB, TT_MUL, TT_DIV, TT_CAT };
    if (k < 0) return expr_binary(TT_ASSIGN, var_node(b, name), cls == 'A' ? rk_arr_rhs(r) : r);
    tree_t *v = var_node(b, name); return expr_binary(TT_ASSIGN, var_node(b, name), expr_binary(ck[k], v, r));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_assign_op(RkB *b, const char *name, const char *op, tree_t *r) {
    int lv, k; if (!rkb_compound_base(op, &lv, &k)) return NULL;
    return expr_binary(TT_ASSIGN, var_node(b, name), rk_scalar_rhs(rkb_binop(b, lv, k, var_node(b, name), r)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_elem(const tree_t *g) { return g && (g->t == TT_ARR_GET || g->t == TT_HASH_GET) && g->n >= 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_elem_store(tree_t *g, tree_t *val) {
    tree_t *c = ast_node_new(g->t == TT_ARR_GET ? TT_ARR_SET : TT_HASH_SET); ast_push(c, rk_tree_clone(g->c[0])); ast_push(c, rk_tree_clone(g->c[1])); ast_push(c, val); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_elem_incdec(RkB *b, tree_t *g, int add, int post) {
    if (post) {
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        const char *tmp = fmt("__post_%d", b->post_uid++);
        expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), rk_tree_clone(g)));
        expr_add_child(seq, rk_elem_store(g, expr_binary(add ? TT_ADD : TT_SUB, leaf_sval(TT_VAR, tmp), rk_ilit(1))));
        expr_add_child(seq, leaf_sval(TT_VAR, tmp));
        return seq;
    }
    return rk_elem_store(g, expr_binary(add ? TT_ADD : TT_SUB, rk_tree_clone(g), rk_ilit(1)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_prefix_apply(RkB *b, const char *op, tree_t *x) {
    if (!strcmp(op, "-")) return expr_unary(TT_MNS, nctx(b, x));
    if (!strcmp(op, "+")) { tree_t *n = nctx(b, x); if (n == x && x->t != TT_ILIT && x->t != TT_FLIT) n = expr_binary(TT_ADD, x, rk_ilit(0)); return n; }
    if (!strcmp(op, "?") || !strcmp(op, "so")) { tree_t *m = make_call("__rk_mkbool"); expr_add_child(m, x); return m; }
    if (!strcmp(op, "!") || !strcmp(op, "not")) return expr_unary(TT_NOT, x);
    if (!strcmp(op, "~")) { tree_t *m = make_call("__rk_str"); expr_add_child(m, x); return m; }
    if (!strcmp(op, "^")) return rk_range_ex(b, rk_ilit(0), x);
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_incdec(RkB *b, const char *var, int add) { return rk_incdec(b, var, add); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_smartmatch_term(RkB *b, tree_t *l, RkTerm *x) {
    if (x->kind == TK_QUOTE && !x->npost && !x->npre) {
        const char *s = b->s + x->from;
        const char *kind = NULL; char *body = NULL;
        if (s[0] == '/') { kind = "match"; body = regex_to_engine(regex_body(b, x->from + 1, x->to, '/')); }
        else if (!strncmp(s, "m:g/", 4)) { kind = "match_global"; body = regex_to_engine(regex_body(b, x->from + 4, x->to, '/')); }
        else if (s[0] == 's' && s[1] == '/') {
            kind = "subst";
            char *pat = regex_to_engine(regex_body(b, x->from + 2, x->to, '/'));
            int i = x->from + 2; while (i < x->to && !(b->s[i] == '/' && b->s[i - 1] != '\\')) i++;
            char *rep = regex_body(b, i + 1, x->to, '/');
            int g = b->s[x->to - 1] == 'g';
            size_t pl = strlen(pat), rl = strlen(rep);
            body = (char *) ct_alloc(pl + rl + 4); memcpy(body, pat, pl); body[pl] = '\x01'; memcpy(body + pl + 1, rep, rl); body[pl + 1 + rl] = '\x01'; body[pl + 2 + rl] = g ? 'g' : '-';
            body[pl + 3 + rl] = 0;
        }
        if (kind) { tree_t *m = ast_node_new(TT_SMATCH); ast_push(m, l); ast_push(m, leaf_sval(TT_QLIT, body)); ast_push(m, leaf_sval(TT_QLIT, kind)); return m; }
    }
    if ((x->kind == TK_NAME || (x->kind == TK_CALL && x->t && x->t->t == TT_VAR)) && !x->npost && !x->npre) {
        tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, l); ast_push(mc, leaf_sval(TT_QLIT, "does")); ast_push(mc, leaf_sval(TT_QLIT, x->name)); return mc;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_smartmatch(RkB *b, tree_t *l, tree_t *r) { (void) b; return call2("__rk_smartmatch", l, r); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rkb_wrap_call(const char *fn, tree_t *x) { tree_t *e = make_call(fn); expr_add_child(e, x); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rkb_reduce_name(RkB *b, int ofrom, int oto) {
    char *op = spn(b, ofrom, oto);
    return !strcmp(op, "+") ? "__rk_reduce_add" : !strcmp(op, "-") ? "__rk_reduce_sub" : !strcmp(op, "*") ? "__rk_reduce_mul" : !strcmp(op, "~") ? "__rk_reduce_cat"
         : !strcmp(op, "min") ? "__rk_reduce_min" : "__rk_reduce_max";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_adverb(RkB *b, tree_t *t, const char *key) {
    (void) b;
    if (!t || t->t != TT_HASH_GET) return;
    if (!strcmp(key, "exists")) t->t = TT_HASH_EXISTS; else if (!strcmp(key, "delete")) t->t = TT_HASH_DELETE;
}
/*====================================================================================================================================================================================================*/
tree_t *rkb_expr(RkB *b, RkList *L) { if (!L || !L->n) return ast_node_new(TT_NUL); return el_tree(b, &L->v[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_list_operand(RkB *b, RkList *L) {
    if (L->n == 1 && !L->trailing) return rk_arr_rhs(el_tree(b, &L->v[0]));
    tree_t *a = make_call("__rk_arr");
    for (int i = 0; i < L->n; i++) expr_add_child(a, el_tree(b, &L->v[i]));
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_cross(RkB *b, const char *op, RkList *L, RkList **rs, int nr) {
    tree_t *c = make_call(op[0] == 'Z' ? "__rk_zip" : "__rk_cross");
    expr_add_child(c, rk_list_operand(b, L));
    for (int i = 0; i < nr; i++) expr_add_child(c, rk_list_operand(b, rs[i]));
    RkEl e; memset(&e, 0, sizeof e); e.t = c; e.nitem = 1; e.t0.kind = TK_TREE; e.t0.t = c;
    L->v[0] = e; L->n = 1; L->nitem = 1; L->op1 = NULL; L->trailing = 0;
    L->rg_lo = L->rg_hi = NULL; L->rg_op = NULL; L->rg_nitem = 0; memset(&L->t1, 0, sizeof L->t1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_paren(RkB *b, RkList *L, int nstmts) {
    (void) nstmts;
    if (!L || !L->nitem) return make_call("__rk_arr");
    if (L->n == 1 && !L->trailing) return el_tree(b, &L->v[0]);
    tree_t *call = make_call("__rk_arr");
    for (int i = 0; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_bracket(RkB *b, RkList *L) {
    tree_t *call = make_call("__rk_arr_lit");
    for (int i = 0; L && i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
    return call;
}
/*====================================================================================================================================================================================================*/
static int named_form(RkTerm *t, int first) {
    if (!t || t->npre || t->npost) return 0;
    if (t->kind == TK_FAT) return 1;
    if (t->kind == TK_CP && t->ck == 'v' && t->cnt == 'P') return 1;
    if (t->kind == TK_CP && t->ck == 'n' && first) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void add_named(RkTerm *t, TL *named) {
    tl_add(named, leaf_sval(TT_QLIT, t->name));
    if (t->kind == TK_CP && t->ck == 'n') {
        tree_t *tb = ast_node_new(TT_FNC); tb->v.sval = (char *) "__rk_mkbool"; tree_t *tn = ast_node_new(TT_VAR); tn->v.sval = (char *) "__rk_mkbool"; ast_push(tb, tn); ast_push(tb, rk_ilit(1));
        tl_add(named, tb); return;
    }
    tl_add(named, t->val);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *pos_arg(RkB *b, RkEl *e) {
    if (e->nitem == 1) {
        RkTerm *t = &e->t0;
        if (t->kind == TK_CP && !t->npre && !t->npost) {
            if (t->ck == 'n') return rk_adverb_bool(1);
            if (t->ck == '!') return rk_adverb_bool(0);
            if (t->ck == 'v' && t->cnt == 'P') return t->val ? t->val : make_call("__rk_undef");
            if (t->ck == 'v' && t->cnt == 'W') return t->val;
        }
    }
    return el_tree(b, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int arglist(RkB *b, RkList *L, int ctx, TL *pos, TL *named) {
    int innamed = 0;
    if (!L) return 0;
    for (int i = 0; i < L->n; i++) {
        RkEl *e = &L->v[i];
        int can = ctx == 1 || ((ctx == 2 || ctx == 3) && (i == 0 || innamed));
        if (named && can && e->nitem == 1 && named_form(&e->t0, !innamed)) { innamed = 1; add_named(&e->t0, named); continue; }
        tl_add(pos, pos_arg(b, e));
    }
    return innamed;
}
/*====================================================================================================================================================================================================*/
static int var_cls_of(const char *t, int n) {
    if (n < 1) return 0;
    char s0 = t[0];
    if (s0 == '$') {
        if (n == 1) return 0;
        char c1 = t[1];
        if ((n == 7 && !strncmp(t, "$*STDIN", 7)) || (n == 8 && (!strncmp(t, "$*STDOUT", 8) || !strncmp(t, "$*STDERR", 8)))) return 'F';
        if (isdigit((unsigned char) c1)) return 'P';
        if (n == 6 && !strncmp(t, "$?LINE", 6)) return 'L';
        if ((c1 == '.' || c1 == '!') && n > 2 && (isalpha((unsigned char) t[2]) || t[2] == '_')) return 'T';
        return 'S';
    }
    if (s0 == '@') { if (n > 2 && (t[1] == '.' || t[1] == '!')) return 'U'; return 'A'; }
    if (s0 == '%') { if (n > 2 && (t[1] == '.' || t[1] == '!')) return 'W'; return 'H'; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_var(RkB *b, RkTerm *it, int from, int to, const char *nc, int nclen) {
    memset(it, 0, sizeof *it); it->kind = TK_VAR; it->from = from; it->to = it->core_to = to;
    if (nc) { tree_t *c = ast_node_new(TT_NAMED_CAPTURE); ast_push(c, leaf_sval(TT_QLIT, trimdup(nc, nclen))); it->t = c; it->cls = 'N'; return; }
    char *name = spn(b, from, to); it->name = name;
    int n = (int) strlen(name);
    it->cls = var_cls_of(name, n);
    switch (it->cls) {
    case 'F': { tree_t *c = ast_node_new(TT_FH_CAPTURE); ast_push(c, rk_ilit(!strcmp(name, "$*STDIN") ? 0 : !strcmp(name, "$*STDOUT") ? 1 : 2)); it->t = c; return; }
    case 'P': { tree_t *c = ast_node_new(TT_CAPTURE); ast_push(c, rk_ilit(atoi(name + 1))); it->t = c; return; }
    case 'L': it->t = rk_ilit(line_at(b, from)); return;
    case 'T': case 'U': case 'W': { tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(name + 1)); it->t = fe; return; }
    default: it->t = var_node(b, name); return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_number(RkB *b, RkTerm *it, int from, int to) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    char *t = spn(b, from, to); int len = (int) strlen(t);
    int alld = len > 0; for (int i = 0; i < len; i++) if (!isdigit((unsigned char) t[i])) alld = 0;
    if (alld) { it->t = rk_ilit(atol(t)); return; }
    int i = 0; while (i < len && isdigit((unsigned char) t[i])) i++;
    int fl = 0;
    if (i > 0 && i < len && t[i] == '.' && i + 1 < len && isdigit((unsigned char) t[i + 1])) { i++; while (i < len && isdigit((unsigned char) t[i])) i++; fl = 1; }
    if (i > 0 && i < len && (t[i] == 'e' || t[i] == 'E')) { int j = i + 1; if (j < len && (t[j] == '+' || t[j] == '-')) j++; if (j < len && isdigit((unsigned char) t[j])) { while (j < len &&
        isdigit((unsigned char) t[j])) j++;
        i = j; fl = 1; } }
    if (fl && i == len) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = atof(t); it->t = e; return; }
    SB s = { 0 }; for (int k = 0; k < len; k++) if (t[k] != '_') sb_c(&s, t[k]);
    char *u = sb_str(&s);
    if (!strncmp(u, "Inf", 3) || !strncmp(u, "NaN", 3)) { it->t = var_node(b, u); return; }
    if (strchr(u, '.') || ((strchr(u, 'e') || strchr(u, 'E')) && strncmp(u, "0x", 2))) { tree_t *e = ast_node_new(TT_FLIT); e->v.dval = atof(u); it->t = e; return; }
    it->t = rk_ilit(strtoll(u, NULL, 0));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_quote(RkB *b, RkTerm *it, int from, int to, RkClosure *cl, int ncl) {
    memset(it, 0, sizeof *it); it->kind = TK_QUOTE; it->from = from; it->to = it->core_to = to;
    const unsigned char *u = (const unsigned char *) b->s + from;
    if (u[0] == '"') it->t = b_dq(b, cl, ncl, from + 1, to - 1);
    else if (u[0] == '\'') it->t = b_sq(b, from + 1, to - 1);
    else if (u[0] == 0xEF && u[1] == 0xBD && u[2] == 0xA2) it->t = leaf_sval(TT_QLIT, spn(b, from + 3, to - 3));
    else if (u[0] == '/' || !strncmp((const char *) u, "rx/", 3) || !strncmp((const char *) u, "m/", 2)) {
        int at = from + (u[0] == '/' ? 1 : u[0] == 'r' ? 3 : 2);
        it->t = make_call("__rk_regex"); expr_add_child(it->t, leaf_sval(TT_QLIT, regex_to_engine(regex_body(b, at, to, '/'))));
    }
    else it->t = leaf_sval(TT_QLIT, spn(b, from, to));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_words(RkB *b, RkTerm *it, int from, int to, int ifrom, int ito) {
    memset(it, 0, sizeof *it); it->kind = TK_WORDS; it->from = from; it->to = it->core_to = to;
    it->name = spn(b, ifrom, ito);
    it->t = b_words(b, b->s + ifrom, ito - ifrom);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mkbool_lit(int v) { tree_t *bl = ast_node_new(TT_FNC); bl->v.sval = (char *) intern("__rk_mkbool"); ast_push(bl, leaf_sval(TT_VAR, "__rk_mkbool")); ast_push(bl, rk_ilit(v)); return bl;
    }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_name(RkB *b, RkTerm *it, int from, int to, int namelen) {
    memset(it, 0, sizeof *it); it->kind = TK_NAME; it->from = from; it->to = it->core_to = to;
    char *nm = spn(b, from, from + namelen); it->name = nm;
    if (!strcmp(nm, "True") || !strcmp(nm, "False")) { it->t = mkbool_lit(!strcmp(nm, "True")); return; }
    it->t = var_node(b, nm);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *closure_of(RkTerm *t) { if (!t || t->kind != TK_BLOCK) return NULL; return t->lop; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void flatten_paren_args(RkList *L, TL *pos) {
    if (pos->n == 1 && L && L->nitem == 1 && L->n == 1 && L->v[0].t0.kind == TK_PAREN && !L->v[0].t0.npost && !L->v[0].t0.npre && pos->v[0] && pos->v[0]->t == TT_FNC && pos->v[0]->v.sval
        && !strcmp(pos->v[0]->v.sval, "__rk_arr")) {
        tree_t *a = pos->v[0]; pos->n = 0;
        for (int i = 1; i < a->n; i++) tl_add(pos, a->c[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_flat_call(TL *pos) {
    tree_t *c = make_call("__rk_arr");
    for (int i = 0; i < pos->n; i++) { tree_t *a = pos->v[i]; expr_add_child(c, a && a->t == TT_TO && a->n >= 2 ? rk_arr_rhs(a) : a); }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_await_call(TL *pos) {
    if (pos->n == 1) return pos->v[0];
    tree_t *c = make_call("__rk_arr"); for (int i = 0; i < pos->n; i++) expr_add_child(c, pos->v[i]); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_listop_substitutes(const char *name, int namelen) {
    static const char *const own[] = { "say", "print", "take", "return", "fail", "exit", "die", "join", "map", "grep", "sort", "reverse", "exists", "delete", 0 };
    char *nm = trimdup(name, namelen);
    if (testop_rt(nm)) return 0;
    for (int i = 0; own[i]; i++) if (!strcmp(own[i], nm)) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_listop_call(RkB *b, const char *name, int namelen, RkTerm *paren) {
    TL pos = { 0 }, kw = { 0 };
    char *nm = trimdup(name, namelen);
    int named = arglist(b, paren->in, 1, &pos, &kw);
    if (named) return rk_named_call(nm, pos.n ? &pos : NULL, &kw);
    if (!strcmp(nm, "flat")) return rk_flat_call(&pos);
    if (!strcmp(nm, "await")) return rk_await_call(&pos);
    tree_t *call = make_call(nm); for (int i = 0; i < pos.n; i++) expr_add_child(call, pos.v[i]);
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_call(RkB *b, RkTerm *it, int from, int to, int namelen, RkList *args, int form) {
    memset(it, 0, sizeof *it); it->kind = TK_CALL; it->from = from; it->to = it->core_to = to;
    char *nm = spn(b, from, from + namelen); it->name = nm;
    if (!args || !args->nitem) args = NULL;
    const char *rt = testop_rt(nm);
    if (form == 0 || !args) {
        if (!strcmp(nm, "True") || !strcmp(nm, "False")) { it->kind = TK_NAME; it->t = mkbool_lit(!strcmp(nm, "True")); return; }
        if (!strcmp(nm, "last")) { it->t = ast_node_new(TT_LOOP_BREAK); return; }
        if (!strcmp(nm, "next")) { it->t = ast_node_new(TT_LOOP_NEXT); return; }
        if (!strcmp(nm, "return") || !strcmp(nm, "fail")) { it->t = ast_node_new(TT_RETURN); return; }
        if (!strcmp(nm, "exit")) { tree_t *c = make_call("__rk_exit"); expr_add_child(c, rk_ilit(0)); it->t = c; return; }
        if (rt) { it->t = make_call(rt); return; }
        if (form == 0) { it->t = var_node(b, nm); return; }
        it->t = make_call(nm); return;
    }
    TL pos = { 0 }, named = { 0 };
    if (rt) { arglist(b, args, 0, &pos, NULL); it->t = rk_testop_call(rt, &pos); return; }
    if (!strcmp(nm, "say") || !strcmp(nm, "print")) {
        arglist(b, args, 0, &pos, NULL); flatten_paren_args(args, &pos);
        tree_t *c = ast_node_new(nm[0] == 's' ? TT_SAY : TT_PRINT); for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]); it->t = c; return;
    }
    if (!strcmp(nm, "take")) {
        arglist(b, args, 0, &pos, NULL);
        if (pos.n == 1) { it->t = expr_unary(TT_SUSPEND, pos.v[0]); return; }
        tree_t *call = make_call("__rk_arr"); for (int i = 0; i < pos.n; i++) expr_add_child(call, pos.v[i]); it->t = expr_unary(TT_SUSPEND, call); return;
    }
    if (!strcmp(nm, "return")) { arglist(b, args, 0, &pos, NULL); tree_t *r = ast_node_new(TT_RETURN); if (pos.n) expr_add_child(r, pos.v[0]); it->t = r; return; }
    if (!strcmp(nm, "fail")) { it->t = ast_node_new(TT_RETURN); return; }
    if (!strcmp(nm, "exit")) { arglist(b, args, 0, &pos, NULL); tree_t *c = make_call("__rk_exit"); if (pos.n) expr_add_child(c, pos.v[0]); it->t = c; return; }
    if (!strcmp(nm, "die")) { arglist(b, args, 0, &pos, NULL); tree_t *d = ast_node_new(TT_DIE); if (pos.n) expr_add_child(d, pos.v[0]); it->t = d; return; }
    if (!strcmp(nm, "flat")) { arglist(b, args, 0, &pos, NULL); it->t = rk_flat_call(&pos); return; }
    if (!strcmp(nm, "await")) { arglist(b, args, 0, &pos, NULL); it->t = rk_await_call(&pos); return; }
    if (!strcmp(nm, "join") && form == 2) { arglist(b, args, 0, &pos, NULL); tree_t *e = make_call("join"); for (int i = 0; i < pos.n; i++) expr_add_child(e, pos.v[i]); it->t = e; return; }
    if (!strcmp(nm, "map") || !strcmp(nm, "grep") || !strcmp(nm, "sort")) {
        tree_e k = nm[0] == 'm' ? TT_MAP : nm[0] == 'g' ? TT_GREP : TT_SORT;
        tree_t *c = ast_node_new(k);
        int lo = 0;
        if (args->n && args->v[0].t0.kind == TK_BLOCK && args->v[0].nitem == 1) { ast_push(c, closure_of(&args->v[0].t0)); lo = 1; }
        for (int i = lo; i < args->n; i++) ast_push(c, el_tree(b, &args->v[i]));
        if (form == 1 && lo == 0 && args->n > 1) { tree_t *a = make_call("__rk_arr"); for (int i = 0; i < c->n; i++) expr_add_child(a, c->c[i]); c->n = 0; ast_push(c, a); }
        it->t = c; return;
    }
    if (!strcmp(nm, "reverse")) { tree_t *c = ast_node_new(TT_REVERSE); ast_push(c, rkb_paren(b, args, 1)); it->t = c; return; }
    if (!strcmp(nm, "exists") || !strcmp(nm, "delete")) {
        tree_t *a = rkb_expr(b, args);
        if (a && a->t == TT_HASH_GET) { a->t = nm[0] == 'e' ? TT_HASH_EXISTS : TT_HASH_DELETE; it->t = a; return; }
        tree_t *c = make_call(nm); expr_add_child(c, a); it->t = c; return;
    }
    if (form == 2 && args->subst) { it->t = rkb_expr(b, args); return; }
    int hn = arglist(b, args, 1, &pos, &named);
    if (hn) { it->t = rk_named_call(nm, pos.n ? &pos : NULL, &named); return; }
    tree_t *e = make_call(nm); for (int i = 0; i < pos.n; i++) expr_add_child(e, pos.v[i]); it->t = e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_colonpair(RkB *b, RkTerm *it, int from, int to, int ck, int kfrom, int kto, tree_t *val, int vfrom, int vto, int vkind) {
    memset(it, 0, sizeof *it); it->kind = TK_CP; it->from = from; it->to = it->core_to = to; it->ck = ck; it->cnt = vkind;
    it->name = spn(b, kfrom, kto);
    if (vkind == 'W') it->val = leaf_sval(TT_QLIT, trimdup(b->s + vfrom, vto - vfrom)); else it->val = val;
    it->t = rk_adverb_bool(ck != '!');
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_fatarrow(RkB *b, RkTerm *it, int from, int to, int kfrom, int kto, RkList *val) {
    memset(it, 0, sizeof *it); it->kind = TK_FAT; it->from = from; it->to = it->core_to = to;
    it->name = spn(b, kfrom, kto); it->val = rkb_expr(b, val);
    tree_t *c = make_call("__rk_pair"); expr_add_child(c, var_node(b, it->name)); expr_add_child(c, it->val); it->t = c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_prefix(RkTerm *it, const char *s, int n) { GROW(it->pre, it->npre, it->cpre, const char *); it->pre[it->npre++] = trimdup(s, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_reduce(RkB *b, RkTerm *it, int from, int to, const char *rop, RkList *args) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    if (!args || !args->nitem) { it->t = make_call(rop); return; }
    tree_t *l = rkb_paren(b, args, 0);
    if (l->t == TT_TO && l->n >= 2) { tree_t *r = make_call("__rk_range_arr"); expr_add_child(r, l->c[0]); expr_add_child(r, l->c[1]); l = r; }
    it->t = rkb_wrap_call(rop, l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *method_call(RkB *b, tree_t *inv, RkPf *pf) {
    char *nm = trimdup(pf->txt ? pf->txt : "", pf->txt ? (int) strlen(pf->txt) : 0);
    if (pf->mod == '^') { char *x = (char *) ct_alloc(strlen(nm) + 2); x[0] = '^'; strcpy(x + 1, nm); nm = x; }
    tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, inv); ast_push(c, leaf_sval(TT_QLIT, nm));
    if (pf->args) {
        TL pos = { 0 }, named = { 0 };
        arglist(b, pf->args, pf->form == 2 ? 4 : 2, &pos, &named);
        for (int i = 0; i < pos.n; i++) ast_push(c, pos.v[i]);
        for (int i = 0; i < named.n; i++) ast_push(c, named.v[i]);
    }
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_method_term(RkB *b, RkTerm *it, RkPf *pf) {
    memset(it, 0, sizeof *it); it->from = pf->from; it->to = it->core_to = pf->to;
    if (pf->k == 'M') { it->kind = TK_DOTTY; it->name = pf->txt; it->t = method_call(b, var_node(b, "$_"), pf); return; }
    it->kind = TK_TREE; it->t = ast_node_new(TT_NUL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_postfix(RkB *b, RkTerm *it, RkPf *pf) {
    int first = it->npost == 0;
    int adj = pf->from == it->core_to;
    tree_t *e = it->t;
    it->npost++; it->to = pf->to;
    if (!first && pf->k == 'P' && pf->txt && (!strcmp(pf->txt, "++") || !strcmp(pf->txt, "--")) && rk_is_elem(e)) { it->t = rkb_elem_incdec(b, e, pf->txt[0] == '+', 1); return; }
    if (first && it->kind == TK_VAR) {
        const char *name = it->name; int cls = it->cls;
        if (cls == 'A' && pf->k == '[') {
            RkList *xs = pf->inner;
            RkTerm *x0 = xs && xs->n ? &xs->v[0].t0 : NULL;
            if (x0 && xs->nitem == 1 && x0->kind == TK_STAR && !x0->npost) { it->t = rk_arr_all(b, name); return; }
            if (x0 && xs->nitem >= 2 && x0->kind == TK_STAR && !x0->npost && xs->op1 && (!strcmp(xs->op1, "-") || !strcmp(xs->op1, "+"))) {
                it->t = rk_arr_end_index(b, name, el_rest(b, &xs->v[0]), xs->op1[0] == '-' ? TT_SUB : TT_ADD); return;
            }
            if (xs && xs->n > 1) {
                tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(b, name));
                for (int i = 0; i < xs->n; i++) expr_add_child(call, el_tree(b, &xs->v[i]));
                it->t = call; return;
            }
            it->t = rk_arr_index(b, name, rkb_expr(b, xs)); return;
        }
        if ((cls == 'H' || (cls == 'S' && adj)) && (pf->k == '{' || pf->k == '<')) {
            tree_t *key = pf->k == '<' ? leaf_sval(TT_QLIT, trimdup(pf->txt, (int) strlen(pf->txt))) : rkb_expr(b, pf->inner);
            tree_t *c = ast_node_new(TT_HASH_GET); ast_push(c, var_node(b, name)); ast_push(c, key); it->t = c; return;
        }
        if (cls == 'S' && adj && pf->k == '[') { it->t = rk_arr_index(b, name, rkb_expr(b, pf->inner)); return; }
        if (cls == 'S' && pf->k == '(') {
            tree_t *c = ast_node_new(TT_INVOKE); expr_add_child(c, var_node(b, name));
            TL pos = { 0 }; arglist(b, pf->args, 0, &pos, NULL);
            for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
            it->t = c; return;
        }
        if ((cls == 'S' || cls == 'T') && pf->k == 'P' && pf->txt && (!strcmp(pf->txt, "++") || !strcmp(pf->txt, "--"))) {
            if (cls == 'S') it->t = rk_post_incdec(b, name, pf->txt[0] == '+');
            else it->t = rk_tw_post_incdec(b, name + 1, pf->txt[0] == '+');
            return;
        }
    }
    if (pf->k == 'M') {
        char *nm = pf->txt ? trimdup(pf->txt, (int) strlen(pf->txt)) : ct_strdup("");
        if (first && !pf->mod && !strcmp(nm, "new") && (it->kind == TK_NAME || (it->kind == TK_CALL && e && e->t == TT_VAR))) {
            TL pos = { 0 }, named = { 0 };
            int hn = pf->args ? arglist(b, pf->args, 3, &pos, &named) : 0;
            if (pf->form == 1) { tree_t *ah = rk_adhoc_new(it->name, hn ? &named : NULL, hn ? NULL : &pos); if (ah) { it->t = ah; return; } }
            tree_t *c = ast_node_new(TT_NEW); ast_push(c, leaf_sval(TT_QLIT, it->name));
            for (int i = 0; i < pos.n; i++) ast_push(c, pos.v[i]);
            for (int i = 0; i < named.n; i++) ast_push(c, named.v[i]);
            it->t = c; return;
        }
        it->t = method_call(b, e, pf);
        if (pf->hyper && pf->k == 'M' && !pf->mod) {
            tree_t *h = make_call("__rk_hyper_meth"); tree_t *inv = it->t->c[0];
            if (inv && inv->t == TT_TO && inv->n >= 2) { tree_t *r = make_call("__rk_range_arr"); expr_add_child(r, inv->c[0]); expr_add_child(r, inv->c[1]); inv = r; }
            expr_add_child(h, inv); for (int i = 1; i < it->t->n; i++) expr_add_child(h, it->t->c[i]); it->t = h;
        }
        return;
    }
    if (pf->k == '[') { tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, e); ast_push(c, rkb_expr(b, pf->inner)); it->t = c; return; }
    if (pf->k == '{' || pf->k == '<') {
        tree_t *key = pf->k == '<' ? leaf_sval(TT_QLIT, trimdup(pf->txt, (int) strlen(pf->txt))) : rkb_expr(b, pf->inner);
        tree_t *c = ast_node_new(TT_HASH_GET); ast_push(c, e); ast_push(c, key); it->t = c; return;
    }
    if (pf->k == '(') { tree_t *c = ast_node_new(TT_INVOKE); expr_add_child(c, e); TL pos = { 0 }; arglist(b, pf->args, 0, &pos, NULL); for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
        it->t = c; return; }
}
/*====================================================================================================================================================================================================*/
tree_t *rkb_param(RkB *b, int prefix, const char *var, int varlen, int *tf, int *tt, int ntypes, RkList *dflt, int suffix, int named) {
    (void) suffix; (void) named;
    char *v = trimdup(var ? var : "", var ? varlen : 0);
    tree_t *p;
    if (prefix == '*' || prefix == 'L') p = var_node(b, v), expr_add_child(p, leaf_sval(TT_QLIT, intern(prefix == 'L' ? "**@" : v[0] == '%' ? "*%" : "*@")));
    else if (v[0] == '@' && !ntypes) { p = var_node(b, v); expr_add_child(p, leaf_sval(TT_QLIT, "@")); }
    else if (ntypes) p = rk_typed_param(b, spn(b, tf[0], tt[0]), v);
    else p = var_node(b, v);
    if (dflt) p = expr_binary(TT_ASSIGN, p, rkb_expr(b, dflt));
    return p;
}
/*====================================================================================================================================================================================================*/
static void app(RkB *b, tree_t *list, tree_t *t, int from) { if (!t) return; if (!t->line) t->line = line_at(b, from); ast_push(list, t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_empty(RkB *b, tree_t *list, int bk) { (void) b; if (bk == BK_CLASS || bk == BK_GRAMMAR || bk == BK_ITEMS || !list) return; ast_push(list, ast_node_new(TT_SEQ_EXPR)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_swap_nonwhen(RkB *b, int v) { int o = b->nonwhen; b->nonwhen = v; return o; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pair_seg(RkEl *e) {
    RkTerm *t = &e->t0;
    if (e->nitem == 1 && t->kind == TK_FAT && !t->npre && !t->npost) return 1;
    if (e->nitem >= 2 && t->kind == TK_QUOTE && !t->npre && !t->npost && t->t && t->t->t == TT_QLIT && e->op1 && !strcmp(e->op1, "=>")) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pair_add(RkB *b, tree_t *h, RkEl *e) {
    RkTerm *t = &e->t0;
    if (t->kind == TK_FAT) { expr_add_child(h, leaf_sval(TT_QLIT, t->name)); expr_add_child(h, t->val); return; }
    expr_add_child(h, leaf_sval(TT_QLIT, t->t->v.sval)); expr_add_child(h, el_rest(b, e));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *paren_hash(RkB *b, RkList *in) {
    if (!in || !in->n) return NULL;
    for (int i = 0; i < in->n; i++) if (!pair_seg(&in->v[i])) return NULL;
    tree_t *h = make_call("__rk_hash");
    for (int i = 0; i < in->n; i++) pair_add(b, h, &in->v[i]);
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *hash_pairs(RkB *b, RkEl **rs, int nrs) {
    if (nrs == 1 && rs[0]->nitem == 1 && rs[0]->t0.kind == TK_PAREN) return paren_hash(b, rs[0]->t0.in);
    for (int i = 0; i < nrs; i++) if (!pair_seg(rs[i])) return NULL;
    tree_t *h = make_call("__rk_hash");
    for (int i = 0; i < nrs; i++) pair_add(b, h, rs[i]);
    return h;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rs_list(RkB *b, RkEl **rs, int nrs) {
    tree_t *call = make_call("__rk_arr");
    for (int i = 0; i < nrs; i++) expr_add_child(call, el_tree(b, rs[i]));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *decl_node(RkB *b, const char *type, tree_t *var, tree_t *val) {
    tree_t *e = ast_node_new(TT_DECL); ast_push(e, leaf_sval(TT_VAR, type)); ast_push(e, var); if (val) ast_push(e, val); (void) b; return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *build_decl(RkB *b, RkDecl *d, RkList *outer, int ofirst) {
    RkEl **rs = NULL; int nrs = 0, crs = 0;
    if (d->init) for (int i = 0; i < d->init->n; i++) { GROW(rs, nrs, crs, RkEl *); rs[nrs++] = &d->init->v[i]; }
    if (outer) for (int i = ofirst; i < outer->n; i++) { GROW(rs, nrs, crs, RkEl *); rs[nrs++] = &outer->v[i]; }
    const char *name = d->name ? d->name : "$";
    const char *type = d->type;
    int sig = d->sigil;
    if (sig == '(') {
        tree_t *rhs = nrs == 1 ? el_tree(b, rs[0]) : rs_list(b, rs, nrs);
        TL targets = { 0 };
        for (int i = 0; d->sig && i < d->sig->n; i++) tl_add(&targets, d->sig->c[i]);
        return rk_destructure(b, &targets, rhs);
    }
    if (!d->init_op || !nrs) {
        tree_t *var = var_node(b, name);
        if (type) return decl_node(b, type, var, NULL);
        if (sig == '$') { tree_t *nul = ast_node_new(TT_NUL); nul->v.ival = 1; return expr_binary(TT_ASSIGN, var, nul); }
        return expr_binary(TT_ASSIGN, var, make_call("__rk_undef"));
    }
    if (!strcmp(d->init_op, ":=")) {
        tree_t *e = el_tree(b, rs[0]);
        tree_t *var = var_node(b, name);
        if (sig != '$') return expr_binary(TT_ASSIGN, var, e);
        tree_t *bd = rk_bind(var, rk_scalar_rhs(e));
        if (!bd) bd = expr_binary(TT_ASSIGN, var, e);
        if (type) return decl_node(b, type, var_node(b, name), bd->c[1]);
        return bd;
    }
    if (sig == '@') {
        tree_t *val;
        if (nrs == 1) { tree_t *e = el_tree(b, rs[0]); val = rk_arr_rhs(e); }
        else val = rs_list(b, rs, nrs);
        tree_t *var = var_node(b, name);
        return type ? decl_node(b, type, var, val) : expr_binary(TT_ASSIGN, var, val);
    }
    if (sig == '%') {
        tree_t *val = type ? NULL : hash_pairs(b, rs, nrs);
        if (!val) val = el_tree(b, rs[0]);
        tree_t *var = var_node(b, name);
        return type ? decl_node(b, type, var, val) : expr_binary(TT_ASSIGN, var, val);
    }
    tree_t *e = el_tree(b, rs[0]);
    if (type) return decl_node(b, type, var_node(b, name), e);
    mark_arrlit(b, strip_sigil(name), e);
    return expr_binary(TT_ASSIGN, var_node(b, name), rk_scalar_rhs(e));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_decl(RkB *b, RkTerm *it, int from, int to, RkDecl *d) { (void) b; memset(it, 0, sizeof *it); it->kind = TK_DECL; it->from = from; it->to = it->core_to = to; it->decl = d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void has_items(RkB *b, tree_t *list, RkDecl *d, int from) {
    const char *nm = d->name ? d->name : "$";
    int twig = strlen(nm) > 2 && (nm[1] == '.' || nm[1] == '!');
    const char *fn = twig ? nm + 1 : strip_sigil(nm);
    const char *tw = NULL, *tn = NULL, *handles = NULL;
    for (int i = 0; i < d->ntr; i++) { if (!strcmp(d->tr[i].word, "handles")) handles = d->tr[i].name; else if (!tw) { tw = d->tr[i].word; tn = d->tr[i].name; } }
    if (handles) { tree_t *fv = ast_node_new(TT_HANDLES_DECL); fv->v.sval = (char *) intern(fn); expr_add_child(fv, leaf_sval(TT_QLIT, handles)); app(b, list, fv, from); return; }
    int has_init = d->init_op && !strcmp(d->init_op, "=") && d->init;
    if (nm[0] == '@' || nm[0] == '%') {
        tree_t *fv = ast_node_new(nm[0] == '@' ? TT_ARR_DECL : TT_HASH_DECL); fv->v.sval = (char *) intern(fn); app(b, list, fv, from); return;
    }
    if (has_init) {
        tree_t *e = rkb_expr(b, d->init);
        if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "rw")) { tree_t *rw = ast_node_new(TT_RW_DECL); rw->v.sval = (char *) intern(fn); app(b, list, rw, from); }
        tree_t *fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *) intern(fn); expr_add_child(fv, e); app(b, list, fv, from); return;
    }
    tree_t *fv;
    if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "required")) { fv = ast_node_new(TT_HAS_DECL); fv->v.sval = (char *) intern(fn); }
    else if (tw && !strcmp(tw, "is") && tn && !strcmp(tn, "rw")) { fv = ast_node_new(TT_RW_DECL); fv->v.sval = (char *) intern(fn); }
    else fv = leaf_sval(TT_VAR, fn);
    app(b, list, fv, from);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *wrap_mod(RkB *b, const char *k, RkList *cx, tree_t *base) {
    if (!strcmp(k, "for")) { tree_t *cond = rkb_paren(b, cx, 1); tree_t *gen = expr_unary(TT_ITERATE, cond); gen->v.sval = (char *) intern("_"); return expr_binary(TT_EVERY, gen, seq1(base)); }
    tree_t *cond = rkb_expr(b, cx);
    if (!strcmp(k, "if") || !strcmp(k, "when")) { tree_t *e = ast_node_new(TT_IF); expr_add_child(e, cond); expr_add_child(e, seq1(base)); return e; }
    if (!strcmp(k, "unless")) { tree_t *e = ast_node_new(TT_UNLESS); ast_push(e, cond); ast_push(e, seq1(base)); return e; }
    if (!strcmp(k, "while")) return expr_binary(TT_WHILE, cond, seq1(base));
    if (!strcmp(k, "until")) { tree_t *e = ast_node_new(TT_UNTIL); expr_add_child(e, cond); expr_add_child(e, seq1(base)); return e; }
    if (!strcmp(k, "with")) return rk_with_mod(base, cond, 0);
    if (!strcmp(k, "without")) return rk_with_mod(base, cond, 1);
    return rk_given_mod(base, cond);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *assign_forms(RkB *b, RkList *L, int tail, int bk) {
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    const char *op1 = L->nitem > 1 ? L->op1 : NULL;
    if (!op1) return NULL;
    int eq = !strcmp(op1, "=");
    if (!strcmp(op1, ":=") && plain(t0, 'S') && (!tail || bk == BK_BLOCK || bk == BK_CATCH || bk == BK_GIVEN)) {
        tree_t *e = el_rest(b, e0);
        tree_t *bd = rk_bind(var_node(b, t0->name), rk_scalar_rhs(e));
        return bd ? bd : expr_binary(TT_ASSIGN, var_node(b, t0->name), e);
    }
    if (eq && t0->npost && !t0->npre && t0->t && t0->t->t == TT_METHCALL && t0->t->n == 2 && L->n == 1 && (!tail || bk == BK_BLOCK || bk == BK_CATCH || bk == BK_GIVEN)) {
        tree_t *rhs = el_rest(b, e0);
        tree_t *fe = ast_node_new(TT_FIELD); fe->v.sval = (char *) intern(t0->t->c[1]->v.sval); expr_add_child(fe, t0->t->c[0]);
        return expr_binary(TT_ASSIGN, fe, rhs);
    }
    if (eq && plain(t0, 'T') && L->n == 1 && (!tail || bk != BK_SUB)) {
        tree_t *rhs = el_rest(b, e0);
        tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(t0->name + 1));
        return expr_binary(TT_ASSIGN, fe, rhs);
    }
    if (eq && t0->kind == TK_VAR && t0->npost == 1 && !t0->npre && t0->t && L->n == 1 && (!tail || bk == BK_BLOCK || bk == BK_CATCH || bk == BK_GIVEN)) {
        tree_t *g = t0->t;
        if (g->t == TT_ARR_GET && (!tail || t0->cls == 'A')) { tree_t *rhs = el_rest(b, e0); tree_t *c = ast_node_new(TT_ARR_SET); ast_push(c, g->c[0]); ast_push(c, g->c[1]); ast_push(c, rhs);
            return c; }
        if (g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "__rk_arr_slice") && g->n == 4 && (!tail || t0->cls == 'A')) {
            tree_t *rhs = el_rest(b, e0); tree_t *c = ast_node_new(TT_ARR_SET); ast_push(c, g->c[1]); ast_push(c, expr_binary(TT_TO, g->c[2], g->c[3])); ast_push(c, rhs); return c;
        }
        if (g->t == TT_HASH_GET && (!tail || t0->cls == 'H')) { tree_t *rhs = el_rest(b, e0); tree_t *c = ast_node_new(TT_HASH_SET); ast_push(c, g->c[0]); ast_push(c, g->c[1]); ast_push(c, rhs);
            return c; }
    }
    int clv, ck;
    if (!eq && t0->kind == TK_VAR && t0->npost == 1 && !t0->npre && rk_is_elem(t0->t) && L->n == 1 && (!tail || bk == BK_BLOCK || bk == BK_CATCH || bk == BK_GIVEN)
        && rkb_compound_base(op1, &clv, &ck)) {
        tree_t *g = t0->t; tree_t *rhs = el_rest(b, e0);
        return rk_elem_store(g, rk_scalar_rhs(rkb_binop(b, clv, ck, rk_tree_clone(g), rhs)));
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *stmt_plain(RkB *b, RkList *L) {
    if (!L->n) return ast_node_new(TT_NUL);
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    const char *op1 = L->nitem > 1 ? L->op1 : NULL;
    if (t0->kind == TK_DECL && !t0->npost && !t0->npre && e0->nitem == 1) return build_decl(b, t0->decl, L, 1);
    if (op1 && !strcmp(op1, "=") && plain(t0, 'S') && L->n == 1) {
        tree_t *rhs = el_rest(b, e0);
        mark_arrlit(b, strip_sigil(t0->name), rhs);
        return expr_binary(TT_ASSIGN, var_node(b, t0->name), rk_scalar_rhs(rhs));
    }
    if (op1 && !strcmp(op1, "=") && plain(t0, 'A')) {
        tree_t *first = el_rest(b, e0);
        if (L->n == 1) return expr_binary(TT_ASSIGN, var_node(b, t0->name), rk_arr_rhs(first));
        tree_t *call = make_call("__rk_arr"); expr_add_child(call, first);
        for (int i = 1; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
        return expr_binary(TT_ASSIGN, var_node(b, t0->name), call);
    }
    if (op1 && !strcmp(op1, "=") && t0->kind == TK_PAREN && !t0->npost && !t0->npre) {
        tree_t *rhs = el_rest(b, e0);
        TL targets = { 0 }; rk_group_targets(t0->t, &targets);
        return rk_destructure(b, &targets, rhs);
    }
    if (op1 && !strcmp(op1, ".=") && plain(t0, 'S') && L->nitem == 2 && L->t1.kind == TK_DOTTY && L->t1.t && L->t1.t->t == TT_METHCALL) {
        tree_t *mc = L->t1.t; mc->c[0] = var_node(b, t0->name);
        return rk_quiet_store(var_node(b, t0->name), mc);
    }
    tree_t *a = assign_forms(b, L, 0, BK_MAIN);
    if (a) return a;
    return el_tree(b, e0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *stmt_tail(RkB *b, RkList *L, int bk) {
    if (!L->n) return ast_node_new(TT_NUL);
    tree_t *a = assign_forms(b, L, 1, bk);
    if (a) return a;
    RkTerm *t0 = &L->v[0].t0;
    tree_t *e = el_tree(b, &L->v[0]);
    if (bk == BK_SUB || bk == BK_METHOD) {
        if (L->nitem == 1 && t0->kind == TK_CALL && t0->name && (!strcmp(t0->name, "say") || !strcmp(t0->name, "print") || !strcmp(t0->name, "return"))) return e;
        if (e && e->t == TT_YADA && bk == BK_METHOD) return e;
        tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, e); return r;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_statement(RkB *b, tree_t *list, int bk, int from, int to, RkList *L, int nmods, const char **modk, RkList **modx, int semi, int last) {
    (void) to;
    if (bk == BK_ITEMS || !list || !L || !L->nitem || !L->n) return;
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    int single = L->nitem == 1;
    if (bk == BK_CLASS) {
        if (single && t0->kind == TK_DECL && t0->decl && t0->decl->scope == 2) { has_items(b, list, t0->decl, from); return; }
        if (single && t0->kind == TK_BSTMT) { app(b, list, t0->t, from); return; }
        app(b, list, stmt_plain(b, L), from); return;
    }
    if (bk == BK_GRAMMAR) { app(b, list, el_t0(b, e0), from); return; }
    int ns = 1;
    if (single && !nmods && !t0->npre && !t0->npost) {
        if (t0->kind == TK_BSTMT) ns = t0->ck == 2 ? 2 : t0->ck == 1 ? 1 : 0;
        else if (t0->kind == TK_BLOCK && t0->val) ns = 0;
    }
    if (bk == BK_CATCH) b->nonwhen++;
    if (ns != 1) {
        app(b, list, t0->kind == TK_BLOCK ? t0->val : t0->t, from);
        if (semi && ns == 0) rkb_empty(b, list, bk);
        return;
    }
    int tail = !semi && last && (bk == BK_BLOCK || bk == BK_SUB || bk == BK_METHOD || bk == BK_CATCH || bk == BK_GIVEN);
    tree_t *tr;
    if (nmods) { tr = assign_forms(b, L, 0, BK_MAIN); if (!tr) tr = el_tree(b, e0); for (int i = 0; i < nmods; i++) tr = wrap_mod(b, modk[i], modx[i], tr); }
    else if (tail) tr = stmt_tail(b, L, bk);
    else tr = stmt_plain(b, L);
    app(b, list, tr, from);
    if (tail) { b->tail_list = list; b->tail_tree = tr; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_control(RkB *b, tree_t *list, int bk, int from, tree_t *t, int ns, int semi, int last) {
    (void) last;
    if (!list || bk == BK_ITEMS) return;
    if (bk == BK_CATCH) b->nonwhen++;
    app(b, list, t, from);
    if (semi && ns == 0) rkb_empty(b, list, bk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_block_seq(RkB *b, tree_t *list, int bk, int yada) {
    (void) yada;
    TL st = { 0 };
    for (int i = 0; list && i < list->n; i++) tl_add(&st, list->c[i]);
    tree_t *tail = NULL;
    if (b->tail_list == list && st.n && st.v[st.n - 1] == b->tail_tree) { tail = st.v[--st.n]; b->tail_list = NULL; }
    if (bk == BK_CLASS || bk == BK_GRAMMAR || bk == BK_MODULE) { if (tail) tl_add(&st, tail); return make_seq(&st); }
    if ((bk == BK_BLOCK || bk == BK_METHOD || bk == BK_CATCH || bk == BK_GIVEN) && tail && tail->t == TT_YADA && !st.n) { TL y = { 0 }; tl_add(&y, tail); return make_seq(&y); }
    TL *l;
    if (bk == BK_SUB || bk == BK_METHOD) {
        if (tail) { l = rk_phasers_place(&st, 0); tl_add(l, tail); }
        else { rk_tail_value(&st); l = rk_phasers_place(&st, 0); }
    }
    else { l = rk_phasers_place(&st, 0); if (tail) tl_add(l, tail); }
    return make_seq(l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_block_term(RkB *b, RkTerm *it, int from, int to, tree_t *seq, tree_t *sig, int sub, RkList *last, int nstmts) {
    (void) sub;
    memset(it, 0, sizeof *it); it->kind = TK_BLOCK; it->from = from; it->to = it->core_to = to; it->val = seq; it->lop = rkb_paren(b, last, nstmts);
    tree_t *a = ast_node_new(TT_ANON_BLOCK); expr_add_child(a, seq);
    for (int i = 0; sig && i < sig->n; i++) { tree_t *p = sig->c[i]; if (p && p->t == TT_ASSIGN && p->n) p = p->c[0]; expr_add_child(a, p); }
    it->t = a;
}
/*====================================================================================================================================================================================================*/
tree_t *rkb_routine(RkB *b, int kind, int multi, const char *name, int namelen, int prefix, tree_t *sig, int has_parens, tree_t *body, RkTrait *tr, int ntr) {
    (void) prefix; (void) has_parens; (void) tr; (void) ntr;
    TL params = { 0 };
    for (int i = 0; sig && i < sig->n; i++) tl_add(&params, sig->c[i]);
    if (!body) body = ast_node_new(TT_SEQ_EXPR);
    tree_t *rkbody = rk_defaults_prologue(&params, body);
    char *nm = trimdup(name ? name : "", name ? namelen : 0);
    const char *lt = strstr(nm, ":<");
    if (lt && nm[strlen(nm) - 1] == '>') {
        char *cat = trimdup(nm, (int) (lt - nm));
        int rl = (int) strlen(lt + 2) - 1; char *raw = trimdup(lt + 2, rl < 0 ? 0 : rl);
        size_t cn = strlen(cat) + strlen(raw) + 64; char *cb = (char *) ct_alloc(cn); rk_op_canon_base(cat, raw, cb, cn); nm = cb;
    }
    const char *mn = multi == 1 ? rk_multi_mangle(nm, &params) : intern(nm);
    tree_t *e;
    if (kind == 0) { e = leaf_sval(TT_SUB_DECL, mn); e->v.ival = params.n; }
    else { e = ast_node_new(TT_SUB_DECL); e->v.ival = params.n + 1; }
    tree_t *nn = ast_node_new(TT_VAR); nn->v.sval = intern(mn); expr_add_child(e, nn);
    for (int i = 0; i < params.n; i++) expr_add_child(e, params.v[i]);
    for (int i = 0; i < rkbody->n; i++) expr_add_child(e, rkbody->c[i]);
    (void) b;
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_package(RkB *b, const char *kind, const char *name, int namelen, RkTrait *tr, int ntr, tree_t *body) {
    (void) b;
    char *nm = trimdup(name ? name : "", name ? namelen : 0);
    tree_e k = !strcmp(kind, "class") ? TT_CLASS_DECL : !strcmp(kind, "role") ? TT_ROLE_DECL : !strcmp(kind, "grammar") ? TT_GRAMMAR_DECL : TT_MODULE_DECL;
    tree_t *cd = ast_node_new(k);
    if (k == TT_CLASS_DECL) {
        SB s = { 0 };
        for (int i = 0; i < ntr; i++) {
            char tag = !strcmp(tr[i].word, "is") ? 'i' : !strcmp(tr[i].word, "does") ? 'd' : 0;
            if (!tag || !tr[i].name) continue;
            if (s.n) sb_c(&s, '\x01');
            sb_c(&s, tag); for (const char *c = tr[i].name; *c; c++) sb_c(&s, *c);
        }
        if (s.n) cd->v.sval = sb_str(&s);
    }
    ast_push(cd, leaf_sval(TT_VAR, nm));
    for (int i = 0; body && i < body->n; i++) if (body->c[i]) ast_push(cd, body->c[i]);
    return cd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_regex_decl(RkB *b, int kind, const char *name, int namelen, int brace) {
    tree_t *rd = ast_node_new(TT_REGEX_DECL); rd->v.ival = kind == 1 ? 0 : kind == 0 ? 1 : 2;
    ast_push(rd, leaf_sval(TT_VAR, trimdup(name ? name : "", name ? namelen : 0)));
    SB s = { 0 }; int depth = 1;
    for (int i = brace + 1; i < b->len; i++) {
        char c = b->s[i];
        if (c == '\\' && i + 1 < b->len) { sb_c(&s, c); sb_c(&s, b->s[i + 1]); i++; continue; }
        if (c == '{') { depth++; sb_c(&s, c); continue; }
        if (c == '}') { if (--depth == 0) break; sb_c(&s, c); continue; }
        sb_c(&s, c);
    }
    ast_push(rd, leaf_sval(TT_QLIT, sb_str(&s)));
    return rd;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_enum(RkB *b, const char *text, int n) {
    TL l = { 0 }; int idx = 0, i = 0;
    while (i < n) {
        while (i < n && (text[i] == ' ' || text[i] == '\t' || text[i] == '\n')) i++;
        if (i >= n) break;
        int w = i; while (i < n && text[i] != ' ' && text[i] != '\t' && text[i] != '\n') i++;
        char *tok = trimdup(text + w, i - w);
        tree_t *val = rk_ilit(idx++);
        tl_add(&l, expr_binary(TT_ASSIGN, var_node(b, tok), val));
    }
    return make_seq(&l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_constant(RkB *b, const char *name, int namelen, RkList *init) {
    tree_t *e = rkb_expr(b, init);
    return expr_binary(TT_ASSIGN, var_node(b, trimdup(name, namelen)), e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_use(RkB *b, const char *name, int namelen, RkList *args) {
    char *nm = trimdup(name, namelen);
    tree_t *u = ast_node_new(TT_USE_DECL);
    char *dot = nm[0] == 'v' && isdigit((unsigned char) nm[1]) ? strchr(nm, '.') : NULL;
    if (dot) {
        *dot = 0; u->v.sval = nm;
        tree_t *acc = var_node(b, "$_");
        char *part = dot + 1;
        while (*part) { char *nx = strchr(part, '.'); if (nx) *nx = 0; tree_t *mc = ast_node_new(TT_METHCALL); ast_push(mc, acc); ast_push(mc, leaf_sval(TT_QLIT, part)); acc = mc; if (!nx) break;
            part = nx + 1; }
        ast_push(u, acc);
        return u;
    }
    u->v.sval = nm;
    if (args && args->nitem) ast_push(u, rkb_expr(b, args));
    return u;
}
/*====================================================================================================================================================================================================*/
tree_t *rkb_if(RkB *b, int n, const char **kw, RkList **cond, tree_t **blk, tree_t *els) {
    (void) kw;
    tree_t **cs = (tree_t **) ct_alloc(sizeof(tree_t *) * (size_t) (n ? n : 1));
    for (int i = 0; i < n; i++) cs[i] = rkb_expr(b, cond[i]);
    tree_t *acc = els;
    for (int i = n - 1; i >= 0; i--) { tree_t *e = ast_node_new(TT_IF); expr_add_child(e, cs[i]); expr_add_child(e, blk[i]); if (acc) expr_add_child(e, acc); acc = e; }
    return acc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_unless(RkB *b, RkList *cond, tree_t *blk) { tree_t *e = ast_node_new(TT_UNLESS); ast_push(e, rkb_expr(b, cond)); ast_push(e, blk); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_while(RkB *b, int until, RkList *cond, tree_t *blk) {
    tree_t *c = rkb_expr(b, cond);
    if (!until) return rk_loop_phasers(b, expr_binary(TT_WHILE, c, blk), blk);
    tree_t *e = ast_node_new(TT_UNTIL); expr_add_child(e, c); expr_add_child(e, blk); return rk_loop_phasers(b, e, blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_repeat(RkB *b, int until, tree_t *blk, RkList *cond) {
    tree_t *e = ast_node_new(TT_REPEAT); expr_add_child(e, blk);
    if (cond) { expr_add_child(e, rkb_expr(b, cond)); e->v.ival = until ? 2 : 1; } else e->v.ival = 0;
    return rk_loop_phasers(b, e, blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_loop(RkB *b, RkList *e1, RkList *e2, RkList *e3, int parens, tree_t *blk) {
    if (!parens) return rk_loop_phasers(b, expr_binary(TT_WHILE, rk_ilit(1), blk), blk);
    tree_t *init;
    if (e1 && e1->nitem == 1 && e1->n && e1->v[0].t0.kind == TK_DECL && e1->v[0].t0.decl) {
        RkDecl *d = e1->v[0].t0.decl;
        tree_t *v = rkb_expr(b, d->init);
        init = expr_binary(TT_ASSIGN, var_node(b, d->name), v);
    }
    else init = rkb_expr(b, e1);
    tree_t *cond = rkb_expr(b, e2);
    tree_t *incr = rkb_expr(b, e3);
    return rk_loop_phasers(b, rk_cstyle_loop(init, cond, incr, blk), blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_unpack_scalars(RkB *b, const char *s, TL *vars) {
    const char *q = s + 1;
    for (;;) {
        while (*q == ' ' || *q == '\t' || *q == '\n') q++;
        if (*q == ')') return q[1] ? 0 : vars->n;
        if (*q != '$') return 0;
        const char *e = q + 1;
        while (isalnum((unsigned char) *e) || *e == '_' || ((*e == '-' || *e == '\'') && isalpha((unsigned char) e[1]))) e++;
        if (e == q + 1) return 0;
        tl_add(vars, var_node(b, trimdup(q, (int) (e - q))));
        q = e;
        while (*q == ' ' || *q == '\t' || *q == '\n') q++;
        if (*q == ',') { q++; continue; }
        if (*q != ')') return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_for(RkB *b, RkList *list, tree_t *sig, tree_t *blk) {
    int np = sig ? sig->n : 0;
    int n = list ? list->n : 0;
    const char *vn = NULL;
    if (np == 1 && sig->c[0] && sig->c[0]->t == TT_VAR && !sig->c[0]->n && sig->c[0]->v.sval && sig->c[0]->v.sval[0] != '@' && sig->c[0]->v.sval[0] != '%') vn = sig->c[0]->v.sval;
    if (vn && n == 1 && list->nitem >= 2 && list->rg_op && (!strcmp(list->rg_op, "..") || !strcmp(list->rg_op, "..^")) && list->rg_nitem == list->nitem) {
        el_fill(b, &list->v[0]);
        tree_t *r = ast_node_new(TT_FOR_RANGE);
        ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, list->rg_lo); ast_push(r, list->rg_op[2] == '^' ? rk_dec(list->rg_hi) : list->rg_hi); ast_push(r, blk); ast_push(r, rk_ilit(0));
        return rk_loop_phasers(b, r, blk);
    }
    tree_t *lst;
    if (n > 1) { lst = make_call("__rk_arr"); for (int i = 0; i < n; i++) expr_add_child(lst, el_tree(b, &list->v[i])); }
    else lst = rkb_expr(b, list);
    if (np > 1) { TL vars = { 0 }; for (int i = 0; i < np; i++) tl_add(&vars, sig->c[i]); return rk_loop_phasers(b, rk_for_multi(b, &vars, lst, blk), blk); }
    if (np == 1 && sig->c[0] && sig->c[0]->t == TT_VAR && sig->c[0]->v.sval && sig->c[0]->v.sval[0] == '(') {
        TL vars = { 0 };
        if (rk_unpack_scalars(b, sig->c[0]->v.sval, &vars) >= 1) return rk_loop_phasers(b, rk_for_multi(b, &vars, lst, blk), blk);
    }
    tree_t *gen = expr_unary(TT_ITERATE, lst);
    if (np == 1 && sig->c[0]) gen->v.sval = sig->c[0]->v.sval;
    return rk_loop_phasers(b, expr_binary(TT_EVERY, gen, blk), blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_given(RkB *b, RkList *topic, tree_t *whens) {
    tree_t *ec = ast_node_new(TT_CASE); expr_add_child(ec, rkb_expr(b, topic));
    for (int i = 0; whens && i < whens->n; i++) expr_add_child(ec, whens->c[i]);
    return ec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_when(RkB *b, tree_t *list, int bk, RkList *cond, tree_t *blk) {
    if (!list) return;
    tree_t *c = rkb_expr(b, cond);
    if (bk == BK_CATCH) c = rk_catch_when_cond(c);
    if (bk == BK_GIVEN || bk == BK_CATCH) { ast_push(list, c); ast_push(list, blk); return; }
    tree_t *e = ast_node_new(TT_IF); expr_add_child(e, c); expr_add_child(e, blk); ast_push(list, e);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_default(RkB *b, tree_t *list, tree_t *blk) { (void) b; if (!list) return; ast_push(list, ast_node_new(TT_NUL)); ast_push(list, blk); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_catch(RkB *b, tree_t *body, int whens) {
    tree_t *e = ast_node_new(TT_CATCH);
    if (whens && body && body->n) {
        tree_t *ec = ast_node_new(TT_CASE); expr_add_child(ec, leaf_sval(TT_VAR, intern("_")));
        for (int i = 0; i < body->n; i++) expr_add_child(ec, body->c[i]);
        ast_push(e, seq1(ec)); return e;
    }
    ast_push(e, rkb_block_seq(b, body, BK_BLOCK, 0));
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int is_phaser_word(const char *w, int blk) {
    static const char *const ph[] = { "BEGIN", "CHECK", "INIT", "END", "ENTER", "LEAVE", "KEEP", "UNDO", "PRE", "POST", "FIRST", "LAST", "NEXT", "TEMP", 0 };
    for (int i = 0; ph[i]; i++) if (!strcmp(ph[i], w)) return 1;
    if (blk && (!strcmp(w, "once") || !strcmp(w, "quietly") || !strcmp(w, "react") || !strcmp(w, "do"))) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_do_for_map(tree_t *st) {
    if (!st || st->t != TT_EVERY || st->n != 2 || !st->c[0] || st->c[0]->t != TT_ITERATE || st->c[0]->n != 1) return NULL;
    const char *v = st->c[0]->v.sval;
    if (v && strcmp(v, "_")) return NULL;
    tree_t *e = st->c[1];
    while (e && e->t == TT_SEQ_EXPR && e->n == 1) e = e->c[0];
    if (!e || e->t == TT_SEQ_EXPR) return NULL;
    tree_t *m = ast_node_new(TT_MAP); ast_push(m, e); ast_push(m, st->c[0]->c[0]); return m;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_sprefix_term(RkB *b, RkTerm *it, int from, int to, const char *word, int wlen, tree_t *blk, tree_t *stmt) {
    (void) b;
    memset(it, 0, sizeof *it); it->from = from; it->to = it->core_to = to;
    char *w = trimdup(word, wlen);
    if (is_phaser_word(w, blk != NULL)) { it->kind = TK_BSTMT; it->ck = blk ? 2 : 1; it->t = rk_phaser_mark(w, blk ? blk : seq1(stmt)); return; }
    if (!strcmp(w, "try")) { it->kind = blk ? TK_BSTMT : TK_TREE; tree_t *e = ast_node_new(TT_TRY); ast_push(e, blk ? blk : seq1(stmt)); it->t = e; return; }
    if (!strcmp(w, "gather")) { it->kind = TK_TREE; tree_t *g = ast_node_new(TT_GATHER); expr_add_child(g, blk ? blk : stmt); it->t = g; return; }
    if (!strcmp(w, "do") && !blk) { tree_t *m = rk_do_for_map(stmt); if (m) { it->kind = TK_TREE; it->t = m; return; } }
    it->kind = TK_TREE; it->t = blk ? blk : stmt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void mark_arrays(RkB *b, tree_t *t) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && (is_arr(b, t->v.sval) || ns_has(&b->als, t->v.sval))) t->slen |= 2;
    for (int i = 0; i < t->n; i++) mark_arrays(b, t->c[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_program(RkB *b, tree_t *list) {
    TL l = { 0 };
    for (int i = 0; list && i < list->n; i++) tl_add(&l, list->c[i]);
    TL *all = rk_phasers_place(&l, 1);
    tree_t *prog = ast_stmt_new(TT_PROGRAM);
    for (int i = 0; i < all->n; i++) {
        tree_t *e = all->v[i];
        if (!e) continue;
        rk_phaser_inline_tree(e);
        tree_t *st = ast_stmt_new(TT_STMT);
        expr_add_child(st, ast_attr_int(":line", 0));
        expr_add_child(st, ast_attr_int(":stno", 0));
        expr_add_child(st, ast_attr_expr(":subj", e));
        expr_add_child(prog, st);
    }
    mark_arrays(b, prog);
    return prog;
}
