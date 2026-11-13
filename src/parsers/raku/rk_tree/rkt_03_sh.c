#include "parsers/raku/rk_tree/rkt_head.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ir/ct_arena.h"
#include "parsers/snobol4/scrip_cc.h"
#include "parsers/raku/rk_tree.h"
#include "parsers/raku/rk_tree/rkt_01_sh.h"
#include "parsers/raku/rk_tree/rkt_02_sh.h"
#include "parsers/raku/rk_tree/rkt_03_sh.h"
#include "parsers/raku/rk_tree/rkt_04_sh.h"
#include "parsers/raku/rk_tree/rkt_05_rk.h"
tree_t *rk_defined_op(RkB *b, int kind, tree_t *l, tree_t *r);
static tree_t *rkb_smartmatch_raw(RkB *b, tree_t *l, tree_t *r);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void tl_add(TL *l, tree_t *t) { GROW(l->v, l->n, l->cap, tree_t *); l->v[l->n++] = t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *mk_junction(const char *flav, tree_t *l, tree_t *r) {
    tree_t *e = make_call(flav);
    if (l && l->t == TT_FNC && l->v.sval && strcmp(l->v.sval, flav) == 0) { for (int i = 1; i < l->n; i++) expr_add_child(e, l->c[i]); }
    else expr_add_child(e, l);
    expr_add_child(e, r);
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_group_targets(tree_t *grp, TL *t) {
    if (grp && grp->t == TT_FNC && grp->v.sval && strcmp(grp->v.sval, "__rk_arr") == 0) { for (int i = 1; i < grp->n; i++) tl_add(t, grp->c[i]); }
    else if (grp) tl_add(t, grp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_range_op(tree_t *lo, tree_t *hi, int fl) { tree_t *to = expr_binary(TT_TO, lo, hi); to->v.ival = fl; return to; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_tree_clone(tree_t *e) {
    if (!e) return NULL;
    tree_t *c = ast_node_new(e->t); c->v = e->v; c->line = e->line; c->slen = e->slen;
    if ((e->t == TT_VAR || e->t == TT_QLIT || e->t == TT_FNC) && e->v.sval) c->v.sval = ct_strdup(e->v.sval);
    for (int i = 0; i < e->n; i++) expr_add_child(c, rk_tree_clone(e->c[i]));
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_star(tree_t *t) { return t && t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "*"); }
static int rk_has_star(tree_t *t) {
    if (!t) return 0;
    if (rk_is_star(t)) return 1;
    for (int i = 0; i < t->n; i++) if (rk_has_star(t->c[i])) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_wc(tree_t *t) { return t && t->t == TT_ANON_BLOCK && t->n >= 2 && t->c[1]->t == TT_VAR && t->c[1]->v.sval && !strncmp(t->c[1]->v.sval, "__wc", 4); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_wc_operand(RkB *b, tree_t *o) { return !b->nocurry && (rk_is_star(o) || rk_is_wc(o)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_wc_take(RkB *b, tree_t *o, TL *params) {
    if (rk_is_star(o) && !b->nocurry) { tree_t *v = leaf_sval(TT_VAR, fmt("__wc%d", b->wc_uid++)); tl_add(params, leaf_sval(TT_VAR, v->v.sval)); return v; }
    if (rk_is_wc(o) && !b->nocurry) { for (int i = 1; i < o->n; i++) tl_add(params, o->c[i]); return o->c[0]->c[0]; }
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_wc_close(tree_t *body, TL *params) {
    tree_t *a = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(sq, body); expr_add_child(a, sq);
    for (int i = 0; i < params->n; i++) expr_add_child(a, params->v[i]);
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_pure_base(tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_VAR || t->t == TT_ILIT || t->t == TT_QLIT) return 1;
    if (t->t != TT_FNC || !t->v.sval || strcmp(t->v.sval, "__rk_arr")) return 0;
    for (int i = 1; i < t->n; i++) if (!rk_pure_base(t->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_star_elems(tree_t *idx, tree_t *base) {
    if (!idx || !rk_pure_base(base)) return idx;
    if (rk_is_star(idx)) { tree_t *el = ast_node_new(TT_METHCALL); ast_push(el, rk_tree_clone(base)); ast_push(el, leaf_sval(TT_QLIT, "elems")); return el; }
    for (int i = 0; i < idx->n; i++) idx->c[i] = rk_star_elems(idx->c[i], base);
    return idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_scalar_rhs(tree_t *rhs) {
    if (!rhs || rhs->t != TT_XREP || rhs->n < 2) return rhs;
    tree_t *c = make_call("__rk_rep"); expr_add_child(c, rhs->c[0]); expr_add_child(c, rhs->c[1]); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pend_fill(RkB *b, RkPend *q) { RkDecl *d = q->d; if (!d) return; q->d = NULL; tree_t *t = build_decl(b, d, NULL, 0); *q->ph = *t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void el_fill(RkB *b, RkEl *e) { for (int i = 0; i < e->npd; i++) pend_fill(b, &e->pd[i]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *el_tree(RkB *b, RkEl *e) {
    el_fill(b, e);
    tree_t *t = e->t ? e->t : ast_node_new(TT_NUL);
    if (t == e->t0.t || (e->npd && t == e->pd[0].ph)) return t;
    tree_t *c = ast_node_new(t->t); c->v = t->v; c->line = t->line; c->slen = t->slen;
    for (int i = 0; i < t->n; i++) ast_push(c, t->c[i]);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *el_rest(RkB *b, RkEl *e) { el_fill(b, e); return e->rest; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *el_t0(RkB *b, RkEl *e) {
    if (e->t0.kind == TK_DECL && !e->t0.t && e->npd) { pend_fill(b, &e->pd[0]); return e->pd[0].ph; }
    return e->t0.t ? e->t0.t : ast_node_new(TT_NUL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plain(RkTerm *t, int cls) { return t && t->kind == TK_VAR && t->cls == cls && !t->npost && !t->npre; }
static const char *const lv_mul[] = { "*", "\xc3\xb7", "\xc3\x97", "~&", "mod", "lcm", "/", "%", "div", "gcd", "+&", "+<", "%%", "+>", 0 };
static const char *const lv_addsub[] = { "+", "\xe2\x88\x92", "?^", "?|", "~|", "+|", "-", "+^", 0 };
static const char *const lv_repl[] = { "x", "xx", 0 };
static const char *const lv_cat[] = { "~", "\xe2\x88\x98", "o", 0 };
static const char *const lv_range1[] = { "..", "..^", 0 };
static const char *const lv_range2[] = { "unicmp", "coll", "^..^", "^..", 0 };
static const char *const lv_dor[] = { "//", 0 };
static const char *const lv_jct[] = { "|", "(^)", "(-)", "(+)", "(|)", "(.)", "(&)", "&", "\xe2\x88\xaa", "\xe2\x88\xa9", "\xe2\x8a\x96", "\xe2\x8a\x8e", "\xe2\x88\x96", "\xe2\x8a\x8d", 0 };
static const char *const lv_divis[] = { 0 };
static const char *const lv_cmp[] = { "==", "!=", "<", ">", "<=", ">=", "\xe2\x89\xa4", "\xe2\x89\xa5", "\xe2\x89\xa0", "!~~", "=~=", "(elem)", "(cont)", "after", "before", "eqv", "===",
                                      "eq", "<=>", "cmp", "leg", "ne", "lt", "le", "gt", "ge", "~~",
                                      "\xe2\x88\x88", "\xe2\x88\x8a", "\xe2\x88\x89", "\xe2\x88\x8b", "\xe2\x88\x8d", "\xe2\x88\x8c", "(<)", "\xe2\x8a\x82", "\xe2\x8a\x84", "(>)", "\xe2\x8a\x83", "\xe2\x8a\x85",
                                      "(==)", "\xe2\x89\xa1", "\xe2\x89\xa2", "(<=)", "\xe2\x8a\x86", "\xe2\x8a\x88", "(>=)", "\xe2\x8a\x87", "\xe2\x8a\x89", "(<+)", "\xe2\x89\xbc", "(>+)", "\xe2\x89\xbd",
                                      "\xe2\xa9\xb5", "\xe2\xa9\xb6", "\xe2\x89\x85", "=:=", 0 };
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
int rkb_op_index_rev(int lv, const char *op, int *rev) {
    int k = rkb_op_index(lv, op); *rev = 0;
    if (k < 0 && op && op[0] == 'R' && op[1]) { k = rkb_op_index(lv, op + 1); if (k >= 0) *rev = 1; }
    return k;
}
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
static int rk_is_ctrl(const tree_t *r) { return r && (r->t == TT_RETURN || r->t == TT_DIE || r->t == TT_LOOP_BREAK || r->t == TT_LOOP_NEXT); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_ctrl_guard(int is_or, tree_t *l, tree_t *r) {
    if (rk_is_ctrl(l)) return l;
    tree_t *g = ast_node_new(is_or ? TT_UNLESS : TT_IF);
    tree_t *sq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(g, l);
    expr_add_child(sq, r);
    expr_add_child(g, sq);
    return g;
}
tree_t *rkb_binop_raw(RkB *b, int lv, int k, tree_t *l, tree_t *r) {
    if (k >= RK_UK) return call2(rk_user_name(b, k - RK_UK, "infix"), l, r);
    static const char *const jfn[] = { NULL, "__rk_set_sym", "__rk_set_dif", "__rk_set_sum", "__rk_set_uni", "__rk_set_mul", "__rk_set_int", NULL, "__rk_set_uni", "__rk_set_int", "__rk_set_sym", "__rk_set_sum", "__rk_set_dif", "__rk_set_mul" };
    switch (lv) {
    case LV_MUL: return b_mul(b, k, l, r);
    case LV_ADDSUB: return b_addsub(b, k, l, r);
    case LV_REPL: return k == 0 ? expr_binary(TT_XREP, l, r) : call2("__rk_arr_xx", l, r);
    case LV_CAT: return k == 0 ? expr_binary(TT_CAT, l, r) : call2("__rk_compose", l, r);
    case LV_RANGE1: return rk_range_op(l, r, k >= 1 ? 2 : 0);
    case LV_RANGE2: return k >= 2 ? rk_range_op(l, r, k == 2 ? 3 : 1) : call2(k == 0 ? "__rk_unicmp" : "__rk_coll", l, r);
    case LV_DOR: return (r && (r->t == TT_VAR || r->t == TT_ILIT || r->t == TT_QLIT || r->t == TT_FLIT)) ? call2("__rk_dor", l, r) : rk_defined_op(b, 0, l, r);
    case LV_JCT: return k == 0 ? mk_junction("any", l, r) : k == 7 ? mk_junction("all", l, r) : call2(jfn[k], l, r);
    case LV_DIVIS: return expr_binary(TT_DIVIS, l, r);
    case LV_CMP: return b_cmp(b, k, l, r);
    case LV_AND: return (rk_is_ctrl(r) || rk_is_ctrl(l)) ? rk_ctrl_guard(0, l, r) : rk_logical_and(l, r);
    case LV_OR: return k == 0 ? ((rk_is_ctrl(r) || rk_is_ctrl(l)) ? rk_ctrl_guard(1, l, r) : expr_binary(TT_ALT, l, r)) : call2(k == 1 ? "__rk_xor" : k == 2 ? "__rk_max" : "__rk_min", l, r);
    case LV_POW: return expr_binary(TT_POW, nctx(b, l), nctx(b, r));
    case LV_PAIR: {
        if (l && l->t == TT_VAR && (l->slen & 1) && !l->n && l->v.sval) l = leaf_sval(TT_QLIT, l->v.sval);
        tree_t *pc = make_call("__rk_pair"); expr_add_child(pc, l); expr_add_child(pc, r); return pc; }
    default: return l;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_binop_curries(int lv, int k) {
    switch (lv) {
    case LV_POW: case LV_MUL: case LV_ADDSUB: case LV_CAT: case LV_JCT: case LV_DIVIS: case LV_CMP: return 1;
    case LV_REPL: return k == 0;
    case LV_RANGE2: return k < 2;
    case LV_OR: return k >= 2;
    default: return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_nocurry(RkB *b, int v) { int o = b->nocurry; b->nocurry = v; return o; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_binop(RkB *b, int lv, int k, tree_t *l, tree_t *r) {
    if (!rk_binop_curries(lv, k) || (!rk_wc_operand(b, l) && !rk_wc_operand(b, r))) return rkb_binop_raw(b, lv, k, l, r);
    TL ps = { 0 }; tree_t *lb = rk_wc_take(b, l, &ps), *rb = rk_wc_take(b, r, &ps);
    return rk_wc_close(rkb_binop_raw(b, lv, k, lb, rb), &ps);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_neg_index(int lv, const char *op, int *rev) {
    *rev = 0;
    if (lv != LV_CMP || !op || op[0] != '!' || !op[1]) return -1;
    return rkb_op_index_rev(lv, op + 1, rev);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_hyper_index(int lv, const char *op, int *rev, int *hy) {
    *rev = 0; *hy = 0;
    if (!op) return -1;
    size_t n = strlen(op);
    int lw, rw;
    if (n < 5) return -1;
    if (!strncmp(op, "<<", 2) || !strncmp(op, "\xc2\xab", 2)) lw = 1; else if (!strncmp(op, ">>", 2) || !strncmp(op, "\xc2\xbb", 2)) lw = 0; else return -1;
    if (!strcmp(op + n - 2, ">>") || !strcmp(op + n - 2, "\xc2\xbb")) rw = 1; else if (!strcmp(op + n - 2, "<<") || !strcmp(op + n - 2, "\xc2\xab")) rw = 0; else return -1;
    char *inner = trimdup(op + 2, (int) n - 4);
    size_t il = strlen(inner);
    if (il > 2 && inner[0] == '[' && inner[il - 1] == ']') inner = trimdup(inner + 1, (int) il - 2);
    int k = rkb_op_index_rev(lv, inner, rev);
    if (k >= 0) *hy = 1 | (lw << 1) | (rw << 2);
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_defined_op(RkB *b, int kind, tree_t *l, tree_t *r) {
    const char *tmp = fmt("__lg_%d", b->post_uid++);
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), l));
    tree_t *dcall = make_call("__rk_defined");
    expr_add_child(dcall, leaf_sval(TT_VAR, tmp));
    int topical = kind != 0;
    tree_t *rside = topical ? rk_loose_topical(b, r, tmp) : r;
    if (rk_is_ctrl(r) && kind != 2) {
        expr_add_child(seq, rk_ctrl_guard(1, dcall, rside));
        expr_add_child(seq, leaf_sval(TT_VAR, tmp));
        return seq;
    }
    tree_t *yes = kind == 2 ? rside : kind == 3 ? var_node(b, "Empty") : leaf_sval(TT_VAR, tmp);
    tree_t *no = kind == 2 ? var_node(b, "Empty") : rside;
    expr_add_child(seq, rkb_ternary(b, dcall, yes, no));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_loose(RkB *b, int kind, tree_t *l, tree_t *r) {
    if (kind == 1) return rkb_binop_raw(b, LV_AND, 0, l, r);
    if (kind == 4) return rkb_binop_raw(b, LV_OR, 0, l, r);
    if (kind == 5) return rkb_binop_raw(b, LV_OR, 1, l, r);
    return rk_defined_op(b, kind == 6 ? 1 : kind == 2 ? 2 : 3, l, r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_ternary(RkB *b, tree_t *l, tree_t *mid, tree_t *r) {
    (void) b;
    tree_t *t = ast_node_new(TT_TERNARY); ast_push(t, l); ast_push(t, mid ? mid : ast_node_new(TT_NUL)); ast_push(t, r); return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_assign_op(RkB *b, const char *name, const char *op, tree_t *r) {
    int lv, k; if (!rkb_compound_base(op, &lv, &k)) return NULL;
    return expr_binary(TT_ASSIGN, var_node(b, name), rk_scalar_rhs(rkb_binop(b, lv, k, var_node(b, name), r)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_is_elem(const tree_t *g) { return g && (g->t == TT_ARR_GET || g->t == TT_HASH_GET) && g->n >= 2; }
static int rk_is_acc(const tree_t *g) { return g && g->t == TT_METHCALL && g->n == 2 && g->c[0] && g->c[1] && g->c[1]->t == TT_QLIT && g->c[1]->v.sval && g->c[1]->v.sval[0]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_unany(tree_t *rd) { tree_t *c = make_call("__rk_unany"); expr_add_child(c, rd); return c; }
static tree_t *rk_elem_store(tree_t *g, tree_t *val) {
    if (g->t == TT_METHCALL) { tree_t *fe = ast_node_new(TT_FIELD); fe->v.sval = (char *) intern(g->c[1]->v.sval); ast_push(fe, rk_tree_clone(g->c[0])); return expr_binary(TT_ASSIGN, fe, val); }
    tree_t *c = ast_node_new(g->t == TT_ARR_GET ? TT_ARR_SET : TT_HASH_SET); ast_push(c, rk_tree_clone(g->c[0])); ast_push(c, rk_tree_clone(g->c[1])); ast_push(c, val); return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_elem_incdec(RkB *b, tree_t *g, int add, int post) {
    if (post) {
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        const char *tmp = fmt("__post_%d", b->post_uid++);
        expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), rk_unany(rk_tree_clone(g))));
        expr_add_child(seq, rk_elem_store(g, expr_binary(add ? TT_ADD : TT_SUB, leaf_sval(TT_VAR, tmp), rk_ilit(1))));
        expr_add_child(seq, leaf_sval(TT_VAR, tmp));
        return seq;
    }
    tree_t *seq = ast_node_new(TT_SEQ_EXPR);
    const char *tmp = fmt("__pre_%d", b->post_uid++);
    expr_add_child(seq, rk_quiet_store(leaf_sval(TT_VAR, tmp), expr_binary(add ? TT_ADD : TT_SUB, rk_unany(rk_tree_clone(g)), rk_ilit(1))));
    expr_add_child(seq, rk_elem_store(g, leaf_sval(TT_VAR, tmp)));
    expr_add_child(seq, leaf_sval(TT_VAR, tmp));
    return seq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_prefix_raw(RkB *b, const char *op, tree_t *x) {
    if (!strcmp(op, "-")) return expr_unary(TT_MNS, nctx(b, x));
    if (!strcmp(op, "+") && x && x->t == TT_TO) { tree_t *c = make_call("__rk_numify"); expr_add_child(c, x); return c; }
    if (!strcmp(op, "+")) { tree_t *n = nctx(b, x); if (n == x && x->t != TT_ILIT && x->t != TT_FLIT) n = expr_binary(TT_ADD, x, rk_ilit(0)); return n; }
    if ((!strcmp(op, "?") || !strcmp(op, "so")) && x && x->t == TT_SMATCH) return rkb_ternary(b, x, mkbool_lit(1), mkbool_lit(0));
    if (!strcmp(op, "?") || !strcmp(op, "so")) { tree_t *m = make_call("__rk_mkbool"); expr_add_child(m, x); return m; }
    if (!strcmp(op, "!") || !strcmp(op, "not")) return expr_unary(TT_NOT, x);
    if (!strcmp(op, "~")) { tree_t *m = make_call("__rk_str"); expr_add_child(m, x); return m; }
    if (!strcmp(op, "temp") && x) { tree_t *m = make_call("__rk_temp"); expr_add_child(m, x); return m; }
    if (!strcmp(op, "^")) return rk_range_op(rk_ilit(0), x, 2);
    if (!strcmp(op, "|")) { tree_t *m = make_call("__rk_arr_slip"); expr_add_child(m, x); return m; }
    { int ui = rkb_user_op_find(b, 'p', op); if (ui >= 0) return rk_user_call1(b, ui, "prefix", x); }
    return x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_prefix_apply(RkB *b, const char *op, tree_t *x) {
    size_t opn = strlen(op);
    if (opn > 2 && (!strcmp(op + opn - 2, "<<") || !strcmp(op + opn - 2, "\xc2\xab"))) return rkb_hyper_prefix(b, trimdup(op, (int) opn - 2), x);
    int cur = !strcmp(op, "-") || !strcmp(op, "+") || !strcmp(op, "?") || !strcmp(op, "!") || !strcmp(op, "~");
    if (!cur || !rk_wc_operand(b, x)) return rkb_prefix_raw(b, op, x);
    TL ps = { 0 }; tree_t *xb = rk_wc_take(b, x, &ps);
    return rk_wc_close(rkb_prefix_raw(b, op, xb), &ps);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_incdec(RkB *b, const char *var, int add) { return rk_incdec(b, var, add); }
tree_t *rkb_smartmatch_term(RkB *b, tree_t *l, RkTerm *x) {
    if (!rk_wc_operand(b, l)) return rkb_smartmatch_term_raw(b, l, x);
    TL ps = { 0 };
    tree_t *m = rkb_smartmatch_term_raw(b, rk_wc_take(b, l, &ps), x);
    return m ? rk_wc_close(m, &ps) : NULL;
}
tree_t *rkb_smartmatch(RkB *b, tree_t *l, tree_t *r) {
    if (rk_wc_operand(b, l)) { TL ps = { 0 }; tree_t *lb = rk_wc_take(b, l, &ps); return rk_wc_close(rkb_smartmatch_raw(b, lb, r), &ps); }
    return rkb_smartmatch_raw(b, l, r);
}
static tree_t *rkb_smartmatch_raw(RkB *b, tree_t *l, tree_t *r) {
    (void) b;
    if (r && r->t == TT_TO && r->n == 2) {
        tree_t *c = make_call("__rk_in_range"); expr_add_child(c, l); expr_add_child(c, r->c[0]); expr_add_child(c, r->c[1]); rk_range_fl_arg(c, r); return c;
    }
    return call2("__rk_smartmatch", l, r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_cross(RkB *b, const char *op, RkList *L, RkList **rs, int nr) {
    tree_t *c = make_call(op[0] == 'Z' ? "__rk_zip" : "__rk_cross");
    expr_add_child(c, rk_list_operand(b, L));
    for (int i = 0; i < nr; i++) expr_add_child(c, rk_list_operand(b, rs[i]));
    int ilv, ik, irv;
    if (op[1] && rkb_cross_inner(op + 1, &ilv, &ik, &irv)) {
        tree_t *acc = rk_hyper_elem(0);
        for (int i = 1; i <= nr; i++) { tree_t *ei = rk_hyper_elem(i); acc = rkb_binop_raw(b, ilv, ik, irv ? ei : acc, irv ? acc : ei); }
        tree_t *blk = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR);
        expr_add_child(sq, acc);
        expr_add_child(blk, sq);
        tree_t *mc = ast_node_new(TT_METHCALL);
        ast_push(mc, c);
        ast_push(mc, leaf_sval(TT_QLIT, "map"));
        ast_push(mc, blk);
        c = mc;
    }
    RkEl e; memset(&e, 0, sizeof e); e.t = c; e.nitem = 1; e.t0.kind = TK_TREE; e.t0.t = c;
    L->v[0] = e; L->n = 1; L->nitem = 1; L->op1 = NULL; L->trailing = 0;
    L->rg_lo = L->rg_hi = NULL; L->rg_op = NULL; L->rg_nitem = 0; memset(&L->t1, 0, sizeof L->t1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_bracket(RkB *b, RkList *L) {
    tree_t *call = make_call("__rk_arr_lit");
    for (int i = 0; L && i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void flatten_paren_args(RkList *L, TL *pos) {
    if (pos->n == 1 && L && L->nitem == 1 && L->n == 1 && L->v[0].t0.kind == TK_PAREN && !L->v[0].t0.npost && !L->v[0].t0.npre && pos->v[0] && pos->v[0]->t == TT_FNC && pos->v[0]->v.sval
        && !strcmp(pos->v[0]->v.sval, "__rk_arr")) {
        tree_t *a = pos->v[0]; pos->n = 0;
        for (int i = 1; i < a->n; i++) tl_add(pos, a->c[i]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_postfix(RkB *b, RkTerm *it, RkPf *pf) {
    int first = it->npost == 0;
    int adj = pf->from == it->core_to;
    tree_t *e = it->t;
    if ((pf->k == 'M' || pf->k == '[') && rk_wc_operand(b, e)) {
        TL ps = { 0 }; it->t = rk_wc_take(b, e, &ps);
        rkb_postfix(b, it, pf);
        it->t = rk_wc_close(it->t, &ps); return;
    }
    it->npost++; it->to = pf->to;
    if (pf->k == 'P' && pf->txt && !strncmp(pf->txt, "\xe2\x9a\x9b", 3) && (pf->txt[3] == '+' || pf->txt[3] == '-') && pf->txt[3] == pf->txt[4] && !pf->txt[5]) pf->txt += 3;
    if (pf->k == 'P' && pf->txt) { int ui = rkb_user_op_find(b, 'P', pf->txt); if (ui >= 0) { it->t = rk_user_call1(b, ui, "postfix", e); return; } }
    if (pf->k == 'U' && pf->txt) {
        int ui = rkb_user_op_find(b, 'k', pf->txt);
        if (ui >= 0) {
            tree_t *c = make_call(rk_user_name(b, ui, "postcircumfix")); TL pos = { 0 };
            expr_add_child(c, e);
            if (pf->inner) arglist(b, pf->inner, 1, &pos, NULL);
            for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
            it->t = c; return;
        }
    }
    if (!first && pf->k == 'P' && pf->txt && (!strcmp(pf->txt, "++") || !strcmp(pf->txt, "--")) && (rk_is_elem(e) || rk_is_acc(e))) { it->t = rkb_elem_incdec(b, e, pf->txt[0] == '+', 1); return; }
    if (first && it->kind == TK_NAME && it->name && e && e->t == TT_VAR && pf->k == 'P' && pf->txt && (!strcmp(pf->txt, "++") || !strcmp(pf->txt, "--"))) { it->t = rk_post_incdec(b, it->name, pf->txt[0] == '+'); return; }
    if (first && it->kind == TK_VAR && pf->hyper && pf->k == 'P' && pf->txt && it->cls == 'A' && (!strcmp(pf->txt, "++") || !strcmp(pf->txt, "--"))) {
        tree_t *c = make_call("__rk_hyper_incdec"); expr_add_child(c, var_node(b, it->name)); expr_add_child(c, rk_ilit(pf->txt[0] == '+' ? 1 : -1));
        it->t = expr_binary(TT_ASSIGN, var_node(b, it->name), c); return;
    }
    if (first && it->kind == TK_VAR) {
        const char *name = it->name; int cls = it->cls;
        if (cls == 'A' && pf->k == '[') {
            RkList *xs = pf->inner;
            RkTerm *x0 = xs && xs->n ? &xs->v[0].t0 : NULL;
            if (x0 && xs->nitem == 1 && x0->kind == TK_STAR && !x0->npost) { it->t = rk_arr_all(b, name); return; }
            if (x0 && xs->n == 1 && xs->nitem >= 2 && (x0->kind == TK_STAR || rk_is_star(xs->v[0].t)) && !x0->npost && xs->v[0].op1 && (!strcmp(xs->v[0].op1, "-") || !strcmp(xs->v[0].op1, "+"))) {
                it->t = rk_arr_end_index(b, name, el_rest(b, &xs->v[0]), xs->v[0].op1[0] == '-' ? TT_SUB : TT_ADD); return;
            }
            if (xs && xs->n > 1) {
                tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, var_node(b, name));
                int s0 = x0 && x0->kind == TK_STAR && !x0->npost && xs->op1 && (!strcmp(xs->op1, "-") || !strcmp(xs->op1, "+"));
                for (int i = 0; i < xs->n; i++) {
                    tree_t *el = s0 && !i ? expr_binary(xs->op1[0] == '-' ? TT_SUB : TT_ADD, leaf_sval(TT_VAR, "*"), el_rest(b, &xs->v[0])) : el_tree(b, &xs->v[i]);
                    expr_add_child(call, rk_star_elems(el, var_node(b, name)));
                }
                it->t = call; return;
            }
            it->t = rk_arr_index(b, name, rk_star_elems(rkb_expr(b, xs), var_node(b, name))); return;
        }
        if ((cls == 'H' || (cls == 'S' && adj)) && (pf->k == '{' || pf->k == '<')) {
            tree_t *key = rk_hash_key(b, pf);
            tree_t *c = ast_node_new(TT_HASH_GET); ast_push(c, var_node(b, name)); ast_push(c, key); it->t = c; return;
        }
        if (cls == 'S' && adj && pf->k == '[') { it->t = rk_arr_index(b, name, rk_star_elems(rkb_expr(b, pf->inner), var_node(b, name))); return; }
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
        if (pf->mod == '=' && e && e->t == TT_VAR && e->v.sval && e->v.sval[0]) {
            tree_t *mc = it->t;
            if (e->v.sval[0] == '@' || e->v.sval[0] == '%') { tree_t *cv = make_call(e->v.sval[0] == '@' ? "__rk_to_array" : "__rk_to_hash"); expr_add_child(cv, mc); it->t = expr_binary(TT_ASSIGN, rk_tree_clone(e), cv); }
            else it->t = rk_quiet_store(rk_tree_clone(e), mc);
            return;
        }
        if (pf->hyper && pf->k == 'M' && !pf->mod) {
            tree_t *h = make_call("__rk_hyper_meth"); tree_t *inv = it->t->c[0];
            if (inv && inv->t == TT_TO && inv->n >= 2) {
                tree_t *r = make_call("__rk_range_arr"); expr_add_child(r, inv->c[0]); expr_add_child(r, inv->c[1]); rk_range_fl_arg(r, inv); inv = r;
            }
            expr_add_child(h, inv); for (int i = 1; i < it->t->n; i++) expr_add_child(h, it->t->c[i]); it->t = h;
        }
        return;
    }
    if (pf->k == '[') {
        RkList *xs = pf->inner;
        if (xs && xs->n > 1) {
            tree_t *call = make_call("__rk_arr_pick"); expr_add_child(call, e);
            for (int i = 0; i < xs->n; i++) expr_add_child(call, rk_star_elems(el_tree(b, &xs->v[i]), e));
            it->t = call; return;
        }
        tree_t *idx = rk_star_elems(rkb_expr(b, xs), e);
        if (idx && idx->t == TT_TO && idx->n >= 2) {
            tree_t *call = make_call("__rk_arr_slice"); expr_add_child(call, e); expr_add_child(call, idx->c[0]); expr_add_child(call, idx->c[1]); rk_range_fl_arg(call, idx); it->t = call; return;
        }
        tree_t *c = ast_node_new(TT_ARR_GET); ast_push(c, e); ast_push(c, idx); it->t = c; return;
    }
    if (pf->k == '{' || pf->k == '<') {
        tree_t *key = rk_hash_key(b, pf);
        tree_t *c = ast_node_new(TT_HASH_GET); ast_push(c, e); ast_push(c, key); it->t = c; return;
    }
    if (pf->k == '(' && e && e->t == TT_FNC && e->v.sval && !strcmp(e->v.sval, "__rk_typeobj") && e->n >= 2 && e->c[1] && e->c[1]->t == TT_QLIT && e->c[1]->v.sval) {
        const char *tn = e->c[1]->v.sval;
        TL cpos = { 0 };
        if ((!strcmp(tn, "Int") || !strcmp(tn, "Num") || !strcmp(tn, "Str") || !strcmp(tn, "Rat") || !strcmp(tn, "FatRat") || !strcmp(tn, "Bool") || !strcmp(tn, "Real") || !strcmp(tn, "Numeric")) && pf->args) {
            arglist(b, pf->args, 0, &cpos, NULL);
            if (cpos.n == 1) { tree_t *c = ast_node_new(TT_METHCALL); ast_push(c, cpos.v[0]); ast_push(c, leaf_sval(TT_QLIT, tn)); it->t = c; return; }
        }
    }
    if (pf->k == '(') { tree_t *c = ast_node_new(TT_INVOKE); expr_add_child(c, e); TL pos = { 0 }; arglist(b, pf->args, 0, &pos, NULL); for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
        it->t = c; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rs_list(RkB *b, RkEl **rs, int nrs) {
    tree_t *call = make_call("__rk_arr");
    for (int i = 0; i < nrs; i++) expr_add_child(call, el_tree(b, rs[i]));
    return call;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *assign_forms(RkB *b, RkList *L, int tail, int bk) {
    (void) tail; (void) bk;
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    const char *op1 = L->nitem > 1 ? L->op1 : NULL;
    if (!op1) return NULL;
    if (!strcmp(op1, "=") && t0->kind == TK_NAME && !t0->npost && !t0->npre && t0->t && t0->t->t == TT_VAR && t0->name && L->n == 1) return expr_binary(TT_ASSIGN, var_node(b, t0->name), el_rest(b, e0));
    int eq = !strcmp(op1, "=") || (!strcmp(op1, ":=") && !plain(t0, 'S'));
    if (!strcmp(op1, ":=") && plain(t0, 'S')) {
        tree_t *e = el_rest(b, e0);
        tree_t *bd = rk_bind(var_node(b, t0->name), rk_scalar_rhs(e));
        return bd ? bd : rk_container_bind(var_node(b, t0->name), e);
    }
    if (eq && t0->npost && !t0->npre && t0->t && t0->t->t == TT_METHCALL && t0->t->n == 2 && L->n == 1) {
        tree_t *rhs = el_rest(b, e0);
        tree_t *fe = ast_node_new(TT_FIELD); fe->v.sval = (char *) intern(t0->t->c[1]->v.sval); expr_add_child(fe, t0->t->c[0]);
        return expr_binary(TT_ASSIGN, fe, rhs);
    }
    if (eq && plain(t0, 'T') && L->n == 1) {
        tree_t *rhs = el_rest(b, e0);
        tree_t *fe = ast_node_new(TT_TWIGIL_FIELD); fe->v.sval = (char *) intern(rk_tw_bare(t0->name + 1));
        return expr_binary(TT_ASSIGN, fe, rhs);
    }
    if (eq && t0->kind == TK_VAR && t0->npost >= 1 && !t0->npre && t0->t && L->n > 1 && t0->t->t == TT_HASH_GET && t0->t->n >= 2 && rk_is_slice_key(t0->t->c[1])) {
        tree_t *g = t0->t;
        tree_t *rhs = make_call("__rk_arr");
        expr_add_child(rhs, el_rest(b, e0));
        for (int i = 1; i < L->n; i++) expr_add_child(rhs, el_tree(b, &L->v[i]));
        tree_t *c = ast_node_new(TT_HASH_SET); ast_push(c, g->c[0]); ast_push(c, g->c[1]); ast_push(c, rhs);
        return c;
    }
    if (eq && t0->kind == TK_VAR && t0->npost >= 1 && !t0->npre && t0->t && L->n == 1) {
        tree_t *g = t0->t;
        if (g->t == TT_ARR_GET) {
            tree_t *rhs = el_rest(b, e0);
            tree_t *c = ast_node_new(TT_ARR_SET);
            ast_push(c, g->c[0]);
            ast_push(c, g->c[1]);
            ast_push(c, rhs);
            if (!strcmp(op1, ":=")) c->v.ival = 2;
            return c;
        }
        if (g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "__rk_arr_slice") && (g->n == 4 || g->n == 5)) {
            tree_t *rhs = el_rest(b, e0); tree_t *c = ast_node_new(TT_ARR_SET); ast_push(c, g->c[1]); ast_push(c, rk_range_op(g->c[2], g->c[3], g->n == 5 ? (int) g->c[4]->v.ival : 0));
            ast_push(c, rhs);
            return c;
        }
        if (g->t == TT_HASH_GET) { tree_t *rhs = el_rest(b, e0); if (rk_is_slice_key(g->c[1]) && rhs && rhs->t == TT_TO && rhs->n >= 2) { tree_t *rr = make_call("__rk_range_arr"); expr_add_child(rr, rhs->c[0]); expr_add_child(rr, rhs->c[1]); rk_range_fl_arg(rr, rhs); rhs = rr; } tree_t *c = ast_node_new(TT_HASH_SET); ast_push(c, g->c[0]); ast_push(c, g->c[1]); ast_push(c, rhs);
            return c; }
    }
    int clv, ck;
    if (!eq && t0->npost >= 1 && !t0->npre && rk_is_acc(t0->t) && L->n == 1 && rkb_compound_base(op1, &clv, &ck)) {
        tree_t *g = t0->t; tree_t *rhs = el_rest(b, e0);
        return rk_elem_store(g, rk_scalar_rhs(rkb_binop(b, clv, ck, rk_unany(rk_tree_clone(g)), rhs)));
    }
    if (!eq && t0->kind == TK_VAR && t0->npost >= 1 && !t0->npre && rk_is_elem(t0->t) && L->n == 1 && rkb_compound_base(op1, &clv, &ck)) {
        tree_t *g = t0->t; tree_t *rhs = el_rest(b, e0);
        return rk_elem_store(g, rk_scalar_rhs(rkb_binop(b, clv, ck, rk_unany(rk_tree_clone(g)), rhs)));
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *stmt_plain(RkB *b, RkList *L) {
    if (!L->n) return ast_node_new(TT_NUL);
    RkEl *e0 = &L->v[0]; RkTerm *t0 = &e0->t0;
    const char *op1 = L->nitem > 1 ? L->op1 : NULL;
    if (t0->kind == TK_DECL && !t0->npost && !t0->npre && e0->nitem == 1) return build_decl(b, t0->decl, L, 1);
    int dlv, dk;
    if (t0->kind == TK_DECL && !t0->npost && !t0->npre && e0->nitem == 2 && op1 && !t0->decl->init_op && !t0->decl->type && t0->decl->sigil == '$' && rkb_compound_base(op1, &dlv, &dk)) {
        tree_t *seq = ast_node_new(TT_SEQ_EXPR), *rhs = el_rest(b, e0);
        expr_add_child(seq, build_decl(b, t0->decl, L, 1));
        expr_add_child(seq, expr_binary(TT_ASSIGN, var_node(b, t0->decl->name), rk_scalar_rhs(rkb_binop(b, dlv, dk, var_node(b, t0->decl->name), rhs))));
        return seq;
    }
    if (op1 && !strcmp(op1, "=") && t0->kind == TK_VAR && t0->npre == 1 && t0->pre && !strcmp(t0->pre[0], "temp") && !t0->npost && t0->t && t0->t->t == TT_VAR && t0->t->v.sval) {
        tree_t *vt = t0->t;
        t0->npre = 0;
        tree_t *ra = stmt_plain(b, L);
        t0->npre = 1;
        tree_t *m = make_call("__rk_temp"); expr_add_child(m, rk_tree_clone(vt)); expr_add_child(m, ra);
        return m;
    }
    if (op1 && !strcmp(op1, "=") && plain(t0, 'S') && L->n == 1) {
        tree_t *rhs = el_rest(b, e0);
        mark_arrlit(b, strip_sigil(t0->name), rhs);
        return expr_binary(TT_ASSIGN, var_node(b, t0->name), rk_scalar_rhs(rhs));
    }
    if (op1 && !strcmp(op1, ":=") && (plain(t0, 'A') || plain(t0, 'H')) && L->n == 1) return rk_container_bind(var_node(b, t0->name), el_rest(b, e0));
    if (op1 && !strcmp(op1, "=") && plain(t0, 'A')) {
        tree_t *first = el_rest(b, e0);
        if (L->n == 1 && !L->trailing) return expr_binary(TT_ASSIGN, var_node(b, t0->name), rk_arr_rhs(first));
        tree_t *call = make_call("__rk_arr"); expr_add_child(call, first);
        for (int i = 1; i < L->n; i++) expr_add_child(call, el_tree(b, &L->v[i]));
        return expr_binary(TT_ASSIGN, var_node(b, t0->name), call);
    }
    if (op1 && !strcmp(op1, "=") && (plain(t0, 'H') || plain(t0, 'U') || plain(t0, 'W'))) {
        tree_t *first = el_rest(b, e0);
        int arr = plain(t0, 'U');
        tree_t *lhs = plain(t0, 'H') ? var_node(b, t0->name) : rk_tw_field(t0->name + 1), *rhs;
        if (L->n == 1) rhs = arr ? rk_arr_rhs(first) : first;
        else { rhs = make_call("__rk_arr"); expr_add_child(rhs, first); for (int i = 1; i < L->n; i++) expr_add_child(rhs, el_tree(b, &L->v[i])); }
        { tree_t *th = make_call(arr ? "__rk_to_array" : "__rk_to_hash"); expr_add_child(th, rhs); rhs = th; }
        return expr_binary(TT_ASSIGN, lhs, rhs);
    }
    if (op1 && !strcmp(op1, "=") && L->n == 1 && t0->t) {
        tree_t *g = t0->t, *var = NULL, *from = NULL, *len = NULL;
        if (g->t == TT_FNC && g->v.sval && !strcmp(g->v.sval, "substr-rw") && g->n >= 3) { var = g->c[1]; from = g->c[2]; len = g->n >= 4 ? g->c[3] : NULL; }
        else if (g->t == TT_METHCALL && g->n >= 3 && g->c[1] && g->c[1]->v.sval && !strcmp(g->c[1]->v.sval, "substr-rw")) { var = g->c[0]; from = g->c[2]; len = g->n >= 4 ? g->c[3] : NULL; }
        if (var && var->t == TT_VAR && var->v.sval && var->v.sval[0] != '@' && var->v.sval[0] != '%') {
            tree_t *c = make_call("__rk_substr_replace"); expr_add_child(c, rk_tree_clone(var)); expr_add_child(c, from);
            if (len) expr_add_child(c, len);
            expr_add_child(c, el_rest(b, e0));
            return expr_binary(TT_ASSIGN, rk_tree_clone(var), c);
        }
    }
    if (op1 && t0->kind == TK_PAREN && !t0->npost && !t0->npre && L->n == 1 && t0->t
        && ((t0->t->t == TT_ASSIGN && t0->t->n == 2 && t0->t->c[0] && ((t0->t->c[0]->t == TT_VAR && t0->t->c[0]->v.sval && t0->t->c[0]->v.sval[0] != '@'
             && t0->t->c[0]->v.sval[0] != '%') || rk_is_elem(t0->t->c[0])))
            || ((t0->t->t == TT_ARR_SET || t0->t->t == TT_HASH_SET) && t0->t->n == 3))) {
        int clv, ck; int eq = !strcmp(op1, "=");
        if (eq || rkb_compound_base(op1, &clv, &ck)) {
            tree_t *inner = t0->t, *lhs = inner->c[0], *rhs = el_rest(b, e0);
            if (inner->t == TT_ARR_SET || inner->t == TT_HASH_SET) { lhs = ast_node_new(inner->t == TT_ARR_SET ? TT_ARR_GET : TT_HASH_GET); ast_push(lhs, rk_tree_clone(inner->c[0])); ast_push(lhs, rk_tree_clone(inner->c[1])); }
            tree_t *val = eq ? rk_scalar_rhs(rhs) : rk_scalar_rhs(rkb_binop(b, clv, ck, rk_is_elem(lhs) ? rk_unany(rk_tree_clone(lhs)) : rk_tree_clone(lhs), rhs));
            tree_t *seq = ast_node_new(TT_SEQ_EXPR); expr_add_child(seq, inner);
            expr_add_child(seq, rk_is_elem(lhs) ? rk_elem_store(lhs, val) : expr_binary(TT_ASSIGN, rk_tree_clone(lhs), val));
            return seq;
        }
    }
    if (op1 && !strcmp(op1, "=") && t0->kind == TK_PAREN && !t0->npost && !t0->npre) {
        tree_t *rhs = el_rest(b, e0);
        TL targets = { 0 }; rk_group_targets(t0->t, &targets);
        return rk_destructure(b, &targets, rhs);
    }
    if (op1 && !strcmp(op1, ".=") && (plain(t0, 'A') || plain(t0, 'H')) && L->nitem == 2 && L->t1.kind == TK_DOTTY && L->t1.t && L->t1.t->t == TT_METHCALL) {
        tree_t *mc = L->t1.t; mc->c[0] = var_node(b, t0->name);
        tree_t *cv = make_call(plain(t0, 'A') ? "__rk_to_array" : "__rk_to_hash"); expr_add_child(cv, mc);
        return expr_binary(TT_ASSIGN, var_node(b, t0->name), cv);
    }
    if (op1 && !strcmp(op1, ".=") && plain(t0, 'S') && L->nitem == 2 && L->t1.kind == TK_DOTTY && L->t1.t && L->t1.t->t == TT_METHCALL) {
        tree_t *mc = L->t1.t; mc->c[0] = var_node(b, t0->name);
        return rk_quiet_store(var_node(b, t0->name), mc);
    }
    tree_t *a = assign_forms(b, L, 0, BK_MAIN);
    if (a) return a;
    if (L->n > 1) {
        tree_t *seq = ast_node_new(TT_SEQ_EXPR);
        for (int i = 0; i < L->n; i++) expr_add_child(seq, el_tree(b, &L->v[i]));
        return seq;
    }
    return el_tree(b, e0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *stmt_tail(RkB *b, RkList *L, int bk) {
    if (!L->n) return ast_node_new(TT_NUL);
    tree_t *a = assign_forms(b, L, 1, bk);
    if (!a && L->nitem > 1 && L->op1 && (!strcmp(L->op1, ".=") || (!strcmp(L->op1, "=") && L->v[0].t0.kind == TK_PAREN && !L->v[0].t0.npost && !L->v[0].t0.npre))) a = stmt_plain(b, L);
    if (!a && L->nitem > 1 && L->op1 && !strcmp(L->op1, "=") && (plain(&L->v[0].t0, 'H') || plain(&L->v[0].t0, 'U') || plain(&L->v[0].t0, 'W'))) a = stmt_plain(b, L);
    if (a) { if (bk == BK_SUB || bk == BK_METHOD) { tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, a); return r; } return a; }
    RkTerm *t0 = &L->v[0].t0;
    int clv0, ck0, asg = L->nitem > 1 && L->op1 && (!strcmp(L->op1, "=") || rkb_compound_base(L->op1, &clv0, &ck0));
    tree_t *e = ((L->n > 1 || L->trailing) && !asg) ? rkb_paren(b, L, 0) : el_tree(b, &L->v[0]);
    if (bk == BK_SUB || bk == BK_METHOD) {
        if (L->nitem == 1 && t0->kind == TK_CALL && t0->name && (!strcmp(t0->name, "say") || !strcmp(t0->name, "print") || !strcmp(t0->name, "return"))) return e;
        if (e && e->t == TT_YADA && bk == BK_METHOD) return e;
        tree_t *r = ast_node_new(TT_RETURN); expr_add_child(r, e); return r;
    }
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_unpack_scalars(RkB *b, const char *s, TL *vars, TL *acc) {
    const char *q = s + 1;
    for (;;) {
        while (*q == ' ' || *q == '\t' || *q == '\n') q++;
        if (*q == ')') return q[1] ? 0 : vars->n;
        if (isupper((unsigned char) *q)) {
            const char *t = q;
            while (isalnum((unsigned char) *t) || *t == '_' || *t == ':' || (*t == '-' && isalpha((unsigned char) t[1]))) t++;
            if (*t != ' ' && *t != '\t') return 0;
            q = t;
            while (*q == ' ' || *q == '\t') q++;
        }
        if (*q == ':' && isalpha((unsigned char) q[1])) {
            const char *w = q + 1, *we = w;
            while (isalnum((unsigned char) *we) || *we == '_' || *we == '-') we++;
            if (*we != '(' || we[1] != '$') return 0;
            const char *v = we + 1, *ve = v + 1;
            while (isalnum((unsigned char) *ve) || *ve == '_' || ((*ve == '-' || *ve == '\'') && isalpha((unsigned char) ve[1]))) ve++;
            if (ve == v + 1 || *ve != ')') return 0;
            while (acc->n < vars->n) tl_add(acc, NULL);
            tl_add(vars, var_node(b, trimdup(v, (int) (ve - v))));
            tl_add(acc, leaf_sval(TT_QLIT, trimdup(w, (int) (we - w))));
            q = ve + 1;
            while (*q == ' ' || *q == '\t' || *q == '\n') q++;
            if (*q == ',') { q++; continue; }
            if (*q != ')') return 0;
            continue;
        }
        if (*q != '$' && *q != '@' && *q != '%') return 0;
        const char *e = q + 1;
        while (isalnum((unsigned char) *e) || *e == '_' || ((*e == '-' || *e == '\'') && isalpha((unsigned char) e[1]))) e++;
        if (e == q + 1) return 0;
        tl_add(vars, var_node(b, trimdup(q, (int) (e - q))));
        q = e;
        for (;;) {
            while (*q == ' ' || *q == '\t' || *q == '\n') q++;
            if (q[0] != 'i' || q[1] != 's' || (q[2] != ' ' && q[2] != '\t')) break;
            q += 2;
            while (*q == ' ' || *q == '\t') q++;
            const char *w = q;
            while (isalnum((unsigned char) *q) || *q == '_' || *q == '-') q++;
            if (q == w) return 0;
        }
        if (*q == ',') { q++; continue; }
        if (*q != ')') return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_collect_ph(const tree_t *t, TL *out) {
    if (!t || t->t == TT_ANON_BLOCK || t->t == TT_SUB_DECL) return;
    if (t->t == TT_VAR && t->v.sval && t->v.sval[0] == '^') {
        int seen = 0;
        for (int i = 0; i < out->n; i++) if (!strcmp(out->v[i]->v.sval, t->v.sval)) seen = 1;
        if (!seen) tl_add(out, (tree_t *) t);
    }
    for (int i = 0; i < t->n; i++) rk_collect_ph(t->c[i], out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_ph_ren(tree_t *t, const char *from, const char *to) {
    if (!t) return;
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, from)) t->v.sval = (char *) to;
    for (int i = 0; i < t->n; i++) rk_ph_ren(t->c[i], from, to);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_range_end_int(const tree_t *e) {
    if (!e) return 1;
    if (e->t == TT_FLIT) return 0;
    if (e->t == TT_QLIT && e->v.sval) { char *ep; if (!*e->v.sval) return 0; strtoll(e->v.sval, &ep, 10); return !*ep; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_for(RkB *b, RkList *list, tree_t *sig, tree_t *blk) {
    TL phs = { 0 };
    if (!sig && blk) rk_collect_ph(blk, &phs);
    if (phs.n) {
        for (int a = 1; a < phs.n; a++) { tree_t *key = phs.v[a]; int k = a - 1; while (k >= 0 && strcmp(phs.v[k]->v.sval, key->v.sval) > 0) { phs.v[k + 1] = phs.v[k]; k--; } phs.v[k + 1] = key; }
        sig = ast_node_new(TT_SEQ_EXPR);
        for (int i = 0; i < phs.n; i++) { const char *pn = phs.v[i]->v.sval; ast_push(sig, leaf_sval(TT_VAR, pn)); rk_ph_ren(blk, intern(pn + 1), pn); }
    }
    int np = sig ? sig->n : 0;
    int n = list ? list->n : 0;
    const char *vn = NULL;
    if (np == 1 && sig->c[0] && sig->c[0]->t == TT_VAR && !sig->c[0]->n && sig->c[0]->v.sval && sig->c[0]->v.sval[0] != '@' && sig->c[0]->v.sval[0] != '%') vn = sig->c[0]->v.sval;
    if (vn && n == 1 && list->nitem >= 2 && list->rg_op && (!strcmp(list->rg_op, "..") || !strcmp(list->rg_op, "..^")) && list->rg_nitem == list->nitem && rk_range_end_int(list->rg_lo) && rk_range_end_int(list->rg_hi)) {
        el_fill(b, &list->v[0]);
        tree_t *r = ast_node_new(TT_FOR_RANGE);
        ast_push(r, leaf_sval(TT_VAR, vn)); ast_push(r, list->rg_lo); ast_push(r, list->rg_op[2] == '^' ? rk_dec(list->rg_hi) : list->rg_hi); ast_push(r, blk); ast_push(r, rk_ilit(0));
        return r;
    }
    tree_t *lst;
    if (n > 1) { lst = make_call("__rk_arr"); for (int i = 0; i < n; i++) expr_add_child(lst, el_tree(b, &list->v[i])); }
    else lst = rkb_expr(b, list);
    if (np > 1) { TL vars = { 0 }; for (int i = 0; i < np; i++) tl_add(&vars, sig->c[i]); return rk_for_multi(b, &vars, NULL, lst, blk, 0); }
    if (np == 1 && sig->c[0] && sig->c[0]->t == TT_VAR && sig->c[0]->v.sval && sig->c[0]->v.sval[0] == '(') {
        TL vars = { 0 }, acc = { 0 };
        if (rk_unpack_scalars(b, sig->c[0]->v.sval, &vars, &acc) >= 1) return rk_for_multi(b, &vars, &acc, lst, blk, 1);
    }
    tree_t *gen = expr_unary(TT_ITERATE, lst);
    if (np == 1 && sig->c[0]) { gen->v.sval = sig->c[0]->v.sval; gen->slen |= sig->c[0]->slen & (RK_PV_COPY | RK_PV_RW); }
    if (np == 1 && sig->c[0]) for (int j = 0; j < sig->c[0]->n; j++) if (sig->c[0]->c[j] && sig->c[0]->c[j]->t == TT_SEQ_EXPR) ast_push(gen, sig->c[0]->c[j]);
    return expr_binary(TT_EVERY, gen, blk);
}
