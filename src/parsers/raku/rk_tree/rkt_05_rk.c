#include "parsers/raku/rk_tree/rkt_head.h"
#include "parsers/raku/rk_tree/rkt_01_sh.h"
#include "parsers/raku/rk_tree/rkt_03_sh.h"
#include "parsers/raku/rk_tree/rkt_04_sh.h"
#include "parsers/raku/rk_tree/rkt_05_rk.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_capture_like(const tree_t *e) {
    if (!e) return 0;
    if (e->t == TT_CAPTURE || e->t == TT_NAMED_CAPTURE) return 1;
    if (e->t == TT_VAR && e->v.sval && (!strcmp(e->v.sval, "/") || !strcmp(e->v.sval, "__slash"))) return 1;
    return e->t == TT_FNC && e->v.sval && (!strcmp(e->v.sval, "__rk_cap_named") || !strcmp(e->v.sval, "__rk_cap_pos"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *nctx(RkB *b, tree_t *e) {
    if (rk_capture_like(e)) { tree_t *c = make_call("__rk_numify"); expr_add_child(c, e); return c; }
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
tree_t *rk_arr_rhs(tree_t *rhs) {
    if (!rhs) return rhs;
    if (rhs->t == TT_TO && rhs->n >= 2) {
        tree_t *call = make_call("__rk_range_arr"); expr_add_child(call, rhs->c[0]); expr_add_child(call, rhs->c[1]); rk_range_fl_arg(call, rhs); return call;
    }
    int item = rhs->t == TT_METHCALL && rhs->n == 2 && rhs->c[1] && rhs->c[1]->t == TT_QLIT && rhs->c[1]->v.sval && !strcmp(rhs->c[1]->v.sval, "item");
    if (!item && !rk_one_scalar(rhs)) return rhs;
    tree_t *call = make_call("__rk_arr"); expr_add_child(call, rhs); return call;
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
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_mul(RkB *b, int k, tree_t *l, tree_t *r) {
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
    case 11: return call2("ishift", l, r);
    case 13: return call2("ishift", l, expr_binary(TT_SUB, rk_ilit(0), r));
    default: return expr_binary(TT_DIVIS, l, r);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_addsub(RkB *b, int k, tree_t *l, tree_t *r) {
    switch (k) {
    case 0: return expr_binary(TT_ADD, nctx(b, l), nctx(b, r));
    case 1: return call2("__rk_sub", l, r);
    case 2: return call2("__rk_lbxor", l, r);
    case 3: return call2("__rk_lbor", l, r);
    case 4: return call2("__rk_sbor", l, r);
    case 5: return call2("__rk_bor", l, r);
    case 7: return call2("__rk_bxor", l, r);
    default: return expr_binary(TT_SUB, nctx(b, l), nctx(b, r));
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_rx_obj_of(tree_t *r) {
    if (r && r->t == TT_SMATCH && r->n >= 3 && r->c[0] && r->c[0]->t == TT_VAR && r->c[0]->v.sval && !strcmp(r->c[0]->v.sval, "_") && r->c[2] && r->c[2]->v.sval
        && (!strcmp(r->c[2]->v.sval, "match") || !strcmp(r->c[2]->v.sval, "match_global"))) {
        tree_t *c = make_call("__rk_regex");
        expr_add_child(c, r->c[1]);
        return c;
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *b_cmp(RkB *b, int k, tree_t *l, tree_t *r) {
    switch (k) {
    case 0: return rk_chain_cmp(nctx(b, l), TT_EQ, nctx(b, r));
    case 1: case 8: return rk_chain_cmp(nctx(b, l), TT_NE, nctx(b, r));
    case 2: return rk_chain_cmp(nctx(b, l), TT_LT, nctx(b, r));
    case 3: return rk_chain_cmp(nctx(b, l), TT_GT, nctx(b, r));
    case 4: case 6: return rk_chain_cmp(nctx(b, l), TT_LE, nctx(b, r));
    case 5: case 7: return rk_chain_cmp(nctx(b, l), TT_GE, nctx(b, r));
    case 9: return call2("__rk_not_smartmatch", l, rk_rx_obj_of(r));
    case 10: return call2("__rk_approx", l, r);
    case 11: case 27: case 28: return call2("__rk_set_elem", l, r);
    case 12: case 30: case 31: return call2("__rk_set_cont", l, r);
    case 29: return call2("__rk_set_nelem", l, r);
    case 32: return call2("__rk_set_ncont", l, r);
    case 33: case 34: return call2("__rk_set_psub", l, r);
    case 35: return call2("__rk_set_npsub", l, r);
    case 36: case 37: return call2("__rk_set_psup", l, r);
    case 38: return call2("__rk_set_npsup", l, r);
    case 39: case 40: return call2("__rk_set_eq", l, r);
    case 41: return call2("__rk_set_ne", l, r);
    case 42: case 43: return call2("__rk_set_sub", l, r);
    case 44: return call2("__rk_set_nsub", l, r);
    case 45: case 46: return call2("__rk_set_sup", l, r);
    case 47: return call2("__rk_set_nsup", l, r);
    case 48: case 49: return call2("__rk_bag_sub", l, r);
    case 50: case 51: return call2("__rk_bag_sup", l, r);
    case 52: return rk_chain_cmp(nctx(b, l), TT_EQ, nctx(b, r));
    case 53: return call2("__rk_ident", l, r);
    case 54: return call2("__rk_approx", l, r);
    case 55: return call2("__rk_ident", l, r);
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
void rkb_user_op(RkB *b, char cat, const char *sym, int len, const char *raw, int rawlen, int prec) {
    static const char *const cn[] = { "infix", "prefix", "postfix", "circumfix", "postcircumfix", "term" };
    const char *cs = cat == 'i' ? cn[0] : cat == 'p' ? cn[1] : cat == 'P' ? cn[2] : cat == 'c' ? cn[3] : cat == 'k' ? cn[4] : cn[5];
    char *sy = trimdup(sym, len), *rw = trimdup(raw, rawlen);
    size_t n = strlen(cs) + 2 * strlen(rw) + 8;
    char *canon = (char *) ct_alloc(n);
    rk_op_canon_base(cs, rw, canon, n);
    int at = -1;
    for (int i = 0; i < b->nuop; i++) if (b->uop[i].cat == cat && !strcmp(b->uop[i].sym, sy)) { at = i; break; }
    if (at < 0) { GROW(b->uop, b->nuop, b->cuop, __typeof__(*b->uop)); at = b->nuop++; }
    b->uop[at].cat = cat; b->uop[at].sym = sy; b->uop[at].raw = rw; b->uop[at].canon = canon; b->uop[at].prec = prec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_user_op_find(RkB *b, char cat, const char *sym) {
    for (int i = b->nuop - 1; i >= 0; i--) if (b->uop[i].cat == cat && !strcmp(b->uop[i].sym, sym)) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_user_op_prec(RkB *b, int idx) { return b->uop[idx].prec; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_user_name(RkB *b, int idx, const char *cat) { (void) cat; return b->uop[idx].canon; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_user_canon_known(RkB *b, const char *cn) {
    for (int i = 0; i < b->nuop; i++) if (!strcmp(b->uop[i].canon, cn)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_user_call1(RkB *b, int idx, const char *cat, tree_t *x) { tree_t *c = make_call(rk_user_name(b, idx, cat)); expr_add_child(c, x); return c; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_user_term(RkB *b, RkTerm *it, int from, int to) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    char *sy = spn(b, from, to);
    int ui = rkb_user_op_find(b, 't', sy);
    it->t = ui >= 0 ? make_call(rk_user_name(b, ui, "term")) : var_node(b, sy);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_circumfix_call(RkB *b, RkTerm *it, int from, int to, const char *open, RkList *in) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    int ui = rkb_user_op_find(b, 'c', open);
    tree_t *c = make_call(ui >= 0 ? rk_user_name(b, ui, "circumfix") : "__rk_arr");
    TL pos = { 0 };
    if (in) arglist(b, in, 1, &pos, NULL);
    for (int i = 0; i < pos.n; i++) expr_add_child(c, pos.v[i]);
    it->t = c;
}
static int rk_hyper_pure(tree_t *t) {
    if (!t) return 0;
    if (t->t == TT_VAR || t->t == TT_ILIT || t->t == TT_QLIT) return 1;
    if (t->t == TT_METHCALL && t->n == 2 && t->c[1] && t->c[1]->t == TT_QLIT && t->c[1]->v.sval && !strcmp(t->c[1]->v.sval, "list")) return rk_hyper_pure(t->c[0]);
    if (t->t != TT_FNC || !t->v.sval || (strcmp(t->v.sval, "__rk_arr") && strcmp(t->v.sval, "__rk_arr_lit") && strcmp(t->v.sval, "__rk_range_arr"))) return 0;
    for (int i = 1; i < t->n; i++) if (!rk_hyper_pure(t->c[i])) return 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rk_opref_text(const char *nm, int *unary) {
    *unary = 0;
    if (!nm || nm[0] != '&') return NULL;
    size_t n = strlen(nm);
    if (n > 3 && nm[1] == '[' && nm[n - 1] == ']') return trimdup(nm + 2, (int) (n - 3));
    static const char *const pre[] = { "&infix:", "&prefix:", "&postfix:", NULL };
    for (int i = 0; pre[i]; i++) {
        size_t pl = strlen(pre[i]);
        if (strncmp(nm, pre[i], pl)) continue;
        const char *q = nm + pl;
        size_t ql = n - pl;
        *unary = i;
        if (ql > 4 && !strncmp(q, "<<", 2) && !strcmp(q + ql - 2, ">>")) return trimdup(q + 2, (int) ql - 4);
        if (ql > 2 && q[0] == '<' && q[ql - 1] == '>') return trimdup(q + 1, (int) ql - 2);
        if (ql > 4 && !strncmp(q, "\xc2\xab", 2) && !strcmp(q + ql - 2, "\xc2\xbb")) return trimdup(q + 2, (int) ql - 4);
        return NULL;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rkb_cross_inner(const char *op, int *lv, int *k, int *rev) {
    *lv = -1; *k = -1; *rev = 0;
    if (!op || !*op) return 0;
    char *inner = trimdup(op, (int) strlen(op));
    size_t il = strlen(inner);
    if (il > 2 && inner[0] == '[' && inner[il - 1] == ']') inner = trimdup(inner + 1, (int) il - 2);
    int un;
    char *rf = rk_opref_text(inner, &un);
    if (rf && !un) inner = rf;
    for (int i = 0; i <= LV_PAIR && *k < 0; i++) { if (i == LV_TERN) continue; int kk = rkb_op_index_rev(i, inner, rev); if (kk >= 0) { *lv = i; *k = kk; } }
    return *k >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_hyper_elem(int i) {
    tree_t *e = ast_node_new(TT_ARR_GET);
    ast_push(e, leaf_sval(TT_VAR, "_"));
    ast_push(e, rk_ilit(i));
    return e;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_hyper_map(tree_t *zip, tree_t *body, int hy, tree_t *kl, tree_t *kr) {
    tree_t *blk = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(sq, body);
    expr_add_child(blk, sq);
    tree_t *mc = ast_node_new(TT_METHCALL);
    ast_push(mc, zip);
    ast_push(mc, leaf_sval(TT_QLIT, "map"));
    ast_push(mc, blk);
    tree_t *sh = make_call("__rk_hyper_shape");
    expr_add_child(sh, rk_ilit(hy));
    expr_add_child(sh, mc);
    expr_add_child(sh, kl);
    expr_add_child(sh, kr);
    return sh;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_hyper_range(tree_t *x) {
    if (!x || x->t != TT_TO || x->n < 2) return x;
    tree_t *mc = ast_node_new(TT_METHCALL);
    ast_push(mc, rk_arr_rhs(x));
    ast_push(mc, leaf_sval(TT_QLIT, "list"));
    return mc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_hyper(RkB *b, int lv, int k, int hy, tree_t *l, tree_t *r) {
    l = rk_hyper_range(l);
    r = rk_hyper_range(r);
    int both = rk_hyper_pure(l) && rk_hyper_pure(r);
    tree_t *kl = both ? rk_tree_clone(l) : ast_node_new(TT_NUL), *kr = both ? rk_tree_clone(r) : ast_node_new(TT_NUL);
    tree_t *zip = make_call("__rk_hyper_zip");
    expr_add_child(zip, rk_ilit(hy));
    expr_add_child(zip, l);
    expr_add_child(zip, r);
    return rk_hyper_map(zip, rkb_binop_raw(b, lv, k, rk_hyper_elem(0), rk_hyper_elem(1)), hy, kl, kr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_loose_topical(RkB *b, tree_t *r, const char *tmp) {
    if (r && r->t == TT_ANON_BLOCK) {
        tree_t *iv = ast_node_new(TT_INVOKE);
        ast_push(iv, r);
        ast_push(iv, leaf_sval(TT_VAR, tmp));
        return iv;
    }
    tree_t *sq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(sq, expr_binary(TT_ASSIGN, leaf_sval(TT_VAR, "_"), leaf_sval(TT_VAR, tmp)));
    expr_add_child(sq, r);
    return sq;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_hyper_prefix(RkB *b, const char *op, tree_t *x) {
    x = rk_hyper_range(x);
    tree_t *kx = rk_hyper_pure(x) ? rk_tree_clone(x) : ast_node_new(TT_NUL);
    tree_t *zip = make_call("__rk_hyper_zip");
    expr_add_child(zip, rk_ilit(15));
    expr_add_child(zip, x);
    expr_add_child(zip, rk_ilit(0));
    return rk_hyper_map(zip, rkb_prefix_raw(b, op, rk_hyper_elem(0)), 15, kx, ast_node_new(TT_NUL));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_assign(RkB *b, int cls, const char *name, int k, tree_t *r) {
    static const tree_e ck[] = { TT_ADD, TT_SUB, TT_MUL, TT_DIV, TT_CAT };
    if (k < 0) return expr_binary(TT_ASSIGN, var_node(b, name), cls == 'A' ? rk_arr_rhs(r) : r);
    tree_t *v = var_node(b, name); return expr_binary(TT_ASSIGN, var_node(b, name), expr_binary(ck[k], v, r));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rkb_dotassign(RkB *b, int cls, const char *name, tree_t *r) {
    if (!r || r->t != TT_METHCALL || r->n < 2) return r;
    r->c[0] = var_node(b, name);
    if (cls == 'A' || cls == 'H') { tree_t *cv = make_call(cls == 'A' ? "__rk_to_array" : "__rk_to_hash"); expr_add_child(cv, r); return expr_binary(TT_ASSIGN, var_node(b, name), cv); }
    return rk_quiet_store(var_node(b, name), r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rkb_wrap_call(const char *fn, tree_t *x) { tree_t *e = make_call(fn); expr_add_child(e, x); return e; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rkb_reduce_name(RkB *b, int ofrom, int oto) {
    char *op = spn(b, ofrom, oto);
    b->rop_txt = op;
    return !strcmp(op, "+") ? "__rk_reduce_add" : !strcmp(op, "-") ? "__rk_reduce_sub" : !strcmp(op, "*") ? "__rk_reduce_mul" : !strcmp(op, "~") ? "__rk_reduce_cat"
         : !strcmp(op, "min") ? "__rk_reduce_min" : !strcmp(op, "max") ? "__rk_reduce_max" : NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_adv_name(const char *k) { return k && (!strcmp(k, "exists") || !strcmp(k, "delete") || !strcmp(k, "kv") || !strcmp(k, "p") || !strcmp(k, "k") || !strcmp(k, "v")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_adv_call(tree_t *t, tree_t *cont, tree_t *key, tree_t *first) {
    t->t = TT_FNC;
    t->v.sval = (char *) intern("__rk_subscr_adv");
    t->n = 0;
    ast_push(t, leaf_sval(TT_VAR, "__rk_subscr_adv"));
    ast_push(t, cont);
    ast_push(t, key);
    ast_push(t, first);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_fnc_named(const tree_t *t, const char *lit) { return t->t == TT_FNC && t->v.sval && !memcmp(t->v.sval, lit, strlen(lit) + 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_adverb(RkB *b, tree_t *t, RkTerm *tm) {
    const char *key = tm ? tm->name : NULL;
    (void) b;
    if (!t || !rk_adv_name(key)) return;
    if (rk_fnc_named(t, "__rk_subscr_adv")) { ast_push(t, tm->t); return; }
    if ((t->t == TT_HASH_EXISTS || t->t == TT_HASH_DELETE) && t->n >= 2) { tree_t *c = t->c[0], *k = t->c[1]; rk_adv_call(t, c, k, rk_adv_pair(t->t == TT_HASH_EXISTS ? "exists" : "delete")); ast_push(t, tm->t); return; }
    if (t->t == TT_HASH_GET && t->n >= 2 && !rk_is_slice_key(t->c[1]) && tm->ck == 'n' && (!strcmp(key, "exists") || !strcmp(key, "delete"))) { t->t = key[0] == 'e' ? TT_HASH_EXISTS : TT_HASH_DELETE; return; }
    if ((t->t == TT_HASH_GET || t->t == TT_ARR_GET) && t->n >= 2) { tree_t *c = t->c[0], *k = t->c[1]; rk_adv_call(t, c, k, tm->t); return; }
    if (rk_fnc_named(t, "__rk_arr_pick") && t->n >= 3) {
        tree_t *c = t->c[1], *k = make_call("__rk_arr");
        for (int i = 2; i < t->n; i++) expr_add_child(k, t->c[i]);
        rk_adv_call(t, c, k, tm->t);
        return;
    }
    if (rk_fnc_named(t, "__rk_arr_slice") && t->n == 4) {
        tree_t *c = t->c[1], *k = make_call("__rk_range_arr");
        expr_add_child(k, t->c[2]);
        expr_add_child(k, t->c[3]);
        rk_adv_call(t, c, k, tm->t);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_list_operand(RkB *b, RkList *L) {
    if (L->n == 1 && !L->trailing) return rk_arr_rhs(el_tree(b, &L->v[0]));
    tree_t *a = make_call("__rk_arr");
    for (int i = 0; i < L->n; i++) expr_add_child(a, el_tree(b, &L->v[i]));
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_sequence(RkB *b, const char *op, RkList *L, RkList *R) {
    size_t ol = strlen(op);
    int excl = ol > 0 && op[ol - 1] == '^';
    tree_t *c = make_call("__rk_seq");
    expr_add_child(c, rk_ilit(excl));
    expr_add_child(c, rk_list_operand(b, L));
    expr_add_child(c, rk_list_operand(b, R));
    RkEl e; memset(&e, 0, sizeof e); e.t = c; e.nitem = 1; e.t0.kind = TK_TREE; e.t0.t = c;
    L->v[0] = e; L->n = 1; L->nitem = 1; L->op1 = NULL; L->trailing = 0;
    L->rg_lo = L->rg_hi = NULL; L->rg_op = NULL; L->rg_nitem = 0; memset(&L->t1, 0, sizeof L->t1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_opref(RkB *b, const char *name) {
    int un;
    char *op = rk_opref_text(name, &un);
    if (!op || !*op || un == 2) return NULL;
    tree_t *a = leaf_sval(TT_VAR, "^a"), *c = leaf_sval(TT_VAR, "^b"), *body = NULL;
    if (un == 1) {
        static const char *const pf[] = { "-", "+", "!", "?", "~", "^", "so", "not", NULL };
        for (int i = 0; pf[i] && !body; i++) if (!strcmp(op, pf[i])) body = rkb_prefix_raw(b, op, a);
    }
    else if (!strcmp(op, ",")) { body = make_call("__rk_arr"); expr_add_child(body, a); expr_add_child(body, c); }
    else {
        int lv, k, rv;
        if (rkb_cross_inner(op, &lv, &k, &rv) && lv != LV_RANGE1 && lv != LV_PAIR + 100) body = !strcmp(op, "~~") ? rkb_smartmatch(b, rv ? c : a, rv ? a : c) : rkb_binop_raw(b, lv, k, rv ? c : a, rv ? a : c);
    }
    if (!body) return NULL;
    tree_t *blk = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR);
    expr_add_child(sq, body);
    expr_add_child(blk, sq);
    return blk;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_prefix(RkTerm *it, const char *s, int n) { GROW(it->pre, it->npre, it->cpre, const char *); it->pre[it->npre++] = trimdup(s, n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_ph(const char *nm) { return leaf_sval(TT_VAR, nm); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_reduce_general(RkB *b, const char *op, tree_t *l) {
    if (!strcmp(op, ",")) return l;
    int rv = 0, lv = -1, k = -1;
    const char *o = op;
    if (o[0] == 'R' && o[1]) { rv = 1; o++; }
    for (int i = 0; i <= LV_PAIR && k < 0; i++) { if (i == LV_TERN) continue; k = rkb_op_index(i, o); if (k >= 0) lv = i; }
    if (k < 0) { int ui = rkb_user_op_find(b, 'i', o); if (ui >= 0) { lv = LV_ADDSUB; k = RK_UK + ui; } }
    if (k < 0) return rkb_wrap_call("__rk_reduce_max", l);
    int chain = lv == LV_CMP && strcmp(o, "<=>") && strcmp(o, "cmp") && strcmp(o, "leg");
    int right = (lv == LV_POW || lv == LV_PAIR) ^ rv;
    int swap = rv ^ right;
    tree_t *a = rk_ph("^a"), *c = rk_ph("^b");
    tree_t *blk = ast_node_new(TT_ANON_BLOCK), *sq = ast_node_new(TT_SEQ_EXPR);
    if (chain) {
        a = rk_hyper_elem(0);
        c = rk_hyper_elem(1);
    }
    tree_t *body = !strcmp(o, "~~") ? rkb_smartmatch(b, rv ? c : a, rv ? a : c) : rkb_binop_raw(b, lv, k, swap ? c : a, swap ? a : c);
    expr_add_child(sq, body);
    expr_add_child(blk, sq);
    tree_t *src = l;
    if (chain) {
        tree_t *pr = make_call("__rk_adjacent_pairs");
        expr_add_child(pr, l);
        src = pr;
    }
    else if (right) {
        src = ast_node_new(TT_METHCALL);
        ast_push(src, l);
        ast_push(src, leaf_sval(TT_QLIT, "reverse"));
    }
    tree_t *mc = ast_node_new(TT_METHCALL);
    ast_push(mc, src);
    ast_push(mc, leaf_sval(TT_QLIT, chain ? "map" : "reduce"));
    ast_push(mc, blk);
    return chain ? rkb_wrap_call("__rk_all_true", mc) : mc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rkb_reduce(RkB *b, RkTerm *it, int from, int to, const char *rop, RkList *args) {
    memset(it, 0, sizeof *it); it->kind = TK_TREE; it->from = from; it->to = it->core_to = to;
    if (!args || !args->nitem) { it->t = rop ? make_call(rop) : rkb_wrap_call("__rk_reduce_max", make_call("__rk_arr")); return; }
    tree_t *l = rkb_paren(b, args, 0);
    if (l->t == TT_TO && l->n >= 2) { tree_t *r = make_call("__rk_range_arr"); expr_add_child(r, l->c[0]); expr_add_child(r, l->c[1]); rk_range_fl_arg(r, l); l = r; }
    it->t = rop ? rkb_wrap_call(rop, l) : rk_reduce_general(b, b->rop_txt, l);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_slice_key(const tree_t *k) {
    return k && k->t == TT_FNC && k->v.sval && (!strcmp(k->v.sval, "__rk_arr") || !strcmp(k->v.sval, "__rk_hkeys") || !strcmp(k->v.sval, "__rk_range_arr"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
tree_t *rk_hash_key(RkB *b, RkPf *pf) {
    if (pf->k == '<') {
        const char *t = pf->txt ? pf->txt : "";
        int nw = 0;
        for (const char *q = t; *q; ) { while (*q && isspace((unsigned char) *q)) q++; if (!*q) break; nw++; while (*q && !isspace((unsigned char) *q)) q++; }
        if (nw < 2) return leaf_sval(TT_QLIT, trimdup(t, (int) strlen(t)));
        tree_t *a = make_call("__rk_arr");
        for (const char *q = t; *q; ) {
            while (*q && isspace((unsigned char) *q)) q++;
            if (!*q) break;
            const char *w = q;
            while (*q && !isspace((unsigned char) *q)) q++;
            expr_add_child(a, leaf_sval(TT_QLIT, trimdup(w, (int) (q - w))));
        }
        return a;
    }
    RkList *in = pf->inner;
    if (in && in->n > 1) { tree_t *a = make_call("__rk_arr"); for (int i = 0; i < in->n; i++) expr_add_child(a, el_tree(b, &in->v[i])); return a; }
    tree_t *k = rkb_expr(b, in);
    if (k && k->t == TT_VAR && k->v.sval && k->v.sval[0] == '@') { tree_t *m = make_call("__rk_hkeys"); expr_add_child(m, k); return m; }
    if (k && k->t == TT_TO && k->n >= 2) { tree_t *r = make_call("__rk_range_arr"); expr_add_child(r, k->c[0]); expr_add_child(r, k->c[1]); rk_range_fl_arg(r, k); k = r; }
    if (k && k->t == TT_FNC && k->v.sval && !strcmp(k->v.sval, "__rk_range_arr")) { tree_t *m = make_call("__rk_hkeys"); expr_add_child(m, k); return m; }
    return k;
}
char *rk_op_name_canon(char *nm) {
    const char *lt = strstr(nm, ":<"), *gq = strstr(nm, ":\xC2\xAB"); size_t nl0 = strlen(nm);
    if (gq && nl0 >= 2 && (unsigned char) nm[nl0 - 2] == 0xC2 && (unsigned char) nm[nl0 - 1] == 0xBB && (size_t) (gq - nm) + 5 <= nl0) {
        char *cat = trimdup(nm, (int) (gq - nm)); int rl = (int) (nl0 - (size_t) (gq - nm) - 5); char *raw = trimdup(gq + 3, rl < 0 ? 0 : rl);
        size_t cn = strlen(cat) + strlen(raw) + 64; char *cb = (char *) ct_alloc(cn); rk_op_canon_base(cat, raw, cb, cn); return cb;
    }
    if (lt && nm[nl0 - 1] == '>') {
        char *cat = trimdup(nm, (int) (lt - nm));
        int rl = (int) strlen(lt + 2) - 1; char *raw = trimdup(lt + 2, rl < 0 ? 0 : rl);
        size_t cn = strlen(cat) + strlen(raw) + 64; char *cb = (char *) ct_alloc(cn); rk_op_canon_base(cat, raw, cb, cn); return cb;
    }
    return nm;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_slash_param(const TL *params) {
    for (int i = 0; i < params->n; i++) { tree_t *p = params->v[i]; if (p && p->t == TT_ASSIGN && p->n) p = p->c[0]; if (p && p->t == TT_VAR && p->v.sval && !strcmp(p->v.sval, "/")) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static tree_t *rk_slash_cap(const char *fn, tree_t *key) {
    tree_t *c = make_call(fn);
    expr_add_child(c, leaf_sval(TT_VAR, "__slash"));
    expr_add_child(c, key);
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_slash_rename(tree_t *t) {
    if (!t) return;
    for (int i = 0; i < t->n; i++) {
        tree_t *c = t->c[i];
        if (!c) continue;
        if (c->t == TT_NAMED_CAPTURE && c->n == 1 && c->c[0]) { t->c[i] = rk_slash_cap("__rk_cap_named", c->c[0]); continue; }
        if (c->t == TT_CAPTURE && c->n == 1 && c->c[0]) { t->c[i] = rk_slash_cap("__rk_cap_pos", c->c[0]); continue; }
        rk_slash_rename(c);
    }
    if (t->t == TT_VAR && t->v.sval && !strcmp(t->v.sval, "/")) t->v.sval = (char *) intern("__slash");
}
