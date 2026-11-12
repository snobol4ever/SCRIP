#include "runtime/unification/un_head.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_06_pl.h"
extern int plw_unify_cells_x(DESCR_t *, DESCR_t *, pl_tr_ctx_t *);
extern void plw_bind_x(DESCR_t *, DESCR_t, pl_tr_ctx_t *);
extern const char *prolog_atom_name(int id);
int plc_unify_cells_cx(pl_cell_t *a, pl_cell_t *b, pl_tr_ctx_t *cx) { return cx ? plw_unify_cells_x(a, b, cx) : pl_unify(a, b); }
static void plc_bind_cx(pl_cell_t *cell, pl_cell_t word, pl_tr_ctx_t *cx) { if (cx) plw_bind_x(pl_deref(cell), word, cx); else pl_bind(cell, word); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_terms(void *l, void *r) { pl_cell_t *a = (pl_cell_t *)l, *b = (pl_cell_t *)r; if (!a || !b) return 0; if (!pl_unify(a, b)) { return 0; } return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_const(int slot, int kind, long ival, const char *sval, double dval) { (void)slot; (void)kind; (void)ival; (void)sval; (void)dval; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_arg_cell(void *n_cell, void *t_cell, void *arg_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *dN = n_cell ? pl_deref((pl_cell_t *)n_cell) : (pl_cell_t *)0;
    pl_cell_t *dT = t_cell ? pl_deref((pl_cell_t *)t_cell) : (pl_cell_t *)0;
    if (!dN || (int)dN->v != DT_I || !dT || (int)dT->v != DT_PLREF || !dT->p) { return 0; }
    long n = (long)dN->i;
    if (n < 1 || n > (long)plc_fid_arity(dT->slen)) { return 0; }
    if (!plc_unify_cells_cx((pl_cell_t *)arg_cell, &((pl_cell_t *)dT->p)[n - 1], cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_cell_name(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->s : ""; const char *n = prolog_atom_name((int)d->i); return n ? n : ""; }
PLR_WALK2(plc_compare_walk, rt_pl_cell_compare_leaf, pl_deref)
int rt_pl_cell_compare(pl_cell_t *ca, pl_cell_t *cb) {
    int ok, r = plc_cmp_bounded(ca, cb, 0, &ok);
    if (ok) return r;
    r = plc_cmp_bounded(ca, cb, 1, &ok);
    return ok ? r : plc_compare_walk(ca, cb, (void *)0, 0);
}
int rt_pl_atop_cell(int op, void *a_cell, void *b_cell) {
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell);
    if (op == 0) return c < 0;
    if (op == 1) return c <= 0;
    if (op == 2) return c > 0;
    if (op == 3) return c >= 0;
    if (op == 4) return c == 0;
    return c != 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_compare_cell(void *order_cell, void *a_cell, void *b_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *o = pl_deref((pl_cell_t *)order_cell);
    if (!pl_cell_unbound(o)) {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        const char *on = plc_is_atomlike(o) ? rt_pl_cell_name(o) : (const char *)0;
        if (!on) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("type_error", "atom", *o); return 0; }
        if (strcmp(on, "<") && strcmp(on, "=") && strcmp(on, ">")) { if (cx && !cx->ball) cx->ball = rt_pl_ball_kind2("domain_error", "order", *o); return 0; }
    }
    int c = rt_pl_cell_compare((pl_cell_t *)a_cell, (pl_cell_t *)b_cell);
    const char *nm = (c < 0) ? "<" : (c > 0) ? ">" : "=";
    const char *an = prolog_atom_name(prolog_atom_intern(nm));
    pl_cell_t ord = pl_make_atom(prolog_atom_intern(an ? an : nm));
    if (!plc_unify_cells_cx((pl_cell_t *)order_cell, &ord, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_vmap_ok(pl_vmap_t *m, pl_cell_t *x, pl_cell_t *y) {
    for (int i = 0; i < m->n; i++) { if (m->a[i] == x) return m->b[i] == y; if (m->b[i] == y) return 0; }
    if (m->n >= m->cap) return 0;
    m->a[m->n] = x;
    m->b[m->n] = y;
    m->n++;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_variant_rec(pl_cell_t *ca, pl_cell_t *cb, pl_vmap_t *m) {
    pl_cell_t *a = pl_deref(ca), *b = pl_deref(cb);
    int ua = pl_cell_unbound(a), ub = pl_cell_unbound(b);
    if (ua || ub) { if (ua != ub) return 0; return pl_vmap_ok(m, a, b); }
    if (((int)a->v == (int)DT_PLREF) != ((int)b->v == (int)DT_PLREF)) return 0;
    if ((int)a->v == (int)DT_PLREF) {
        int ara = plc_fid_arity(a->slen), arb = plc_fid_arity(b->slen);
        if (ara != arb || plc_fid_name(a->slen) != plc_fid_name(b->slen)) return 0;
        { pl_cell_t *aa = (pl_cell_t *)a->p, *bb = (pl_cell_t *)b->p; for (int i = 0; i < ara; i++) if (!pl_variant_rec(&aa[i], &bb[i], m)) return 0; return 1; }
    }
    return rt_pl_atop_cell(4, (void *)a, (void *)b);
}
int rt_pl_variant_cells(void *a_cell, void *b_cell) {
    pl_cell_t *a = (pl_cell_t *)a_cell, *b = (pl_cell_t *)b_cell;
    if (pl_acyclic_walk(a) && pl_acyclic_walk(b)) {
        int cap = plc_var_occ(a) + 1;
        pl_cell_t *ma[cap], *mb[cap];
        pl_vmap_t m;
        m.a = ma;
        m.b = mb;
        m.n = 0;
        m.cap = cap;
        return pl_variant_rec(a, b, &m);
    }
    {
        pl_cell_t ca = plc_copy(a), cb = plc_copy(b);
        int na = plc_var_occ(&ca) + 1, nb = plc_var_occ(&cb) + 1, ka = 0, kb = 0;
        pl_cell_t *va[na], *vb[nb];
        rt_pl_tv_walk(&ca, va, &ka);
        rt_pl_tv_walk(&cb, vb, &kb);
        if (ka != kb) return 0;
        for (int i = 0; i < ka; i++) { va[i]->v = (DTYPE_t)DT_PLVAR; va[i]->slen = 0; va[i]->p = (void *)vb[i]; }
        return rt_pl_cell_compare(&ca, &cb) == 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_var_occ_visit(pl_cell_t *d, void *ctx) { if (pl_cell_unbound(d)) ++*(int *)ctx; return PLR_V_GO; }
PLR_WALK1(plc_var_occ_walk, plc_var_occ_visit, pl_deref) int plc_var_occ(pl_cell_t *c) { int k = 0; if (c) (void)plc_var_occ_walk(c, (void *)&k); return k; }
int rt_pl_var_occ(void *cell) { return plc_var_occ((pl_cell_t *)cell); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long plc_list_cells(void *list_cell, int dot_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    void *ck = (void *)0;
    long n = 0, st = 0, pw = 1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2 && cur->p) {
        if (cur->p == ck) return -1;
        if (++st == pw) { ck = cur->p; pw <<= 1; st = 0; }
        n++;
        cur = pl_deref(&((pl_cell_t *)cur->p)[1]);
    }
    return n;
}
static int plc_list_len(void *list_cell, int dot_id) { long n = plc_list_cells(list_cell, dot_id); return n > 0 ? (int)n : 0; }
static int plc_tv_visit(pl_cell_t *d, void *ctx) {
    plc_tv_t *t = (plc_tv_t *)ctx;
    if (!pl_cell_unbound(d)) return PLR_V_GO;
    for (int i = 0; i < *t->n; i++) if (t->pool[i] == d) { if (t->counts) t->counts[i]++; return PLR_V_GO; }
    if (t->counts) t->counts[*t->n] = 1;
    t->pool[(*t->n)++] = d;
    return PLR_V_GO;
}
PLR_WALK1(plc_tv_walk, plc_tv_visit, pl_deref) void rt_pl_tv_walk(pl_cell_t *c, pl_cell_t **pool, int *pool_n) {
    plc_tv_t t;
    t.pool = pool;
    t.n = pool_n;
    t.counts = (int *)0;
    if (c) (void)plc_tv_walk(c, (void *)&t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_subsumes_cell(void *gen_cell, void *spec_cell) {
    pl_cell_t *g = (pl_cell_t *)gen_cell, *s = (pl_cell_t *)spec_cell;
    if (!g || !s) return 0;
    pl_cell_t *svars[plc_var_occ(s) + 1];
    int sn = 0;
    rt_pl_tv_walk(s, svars, &sn);
    int unified = pl_unify(g, s);
    int bound_spec = 0;
    if (unified) for (int i = 0; i < sn; i++) if (!pl_cell_unbound(pl_deref(svars[i]))) { bound_spec = 1; break; }
    return unified && !bound_spec;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_ord_cmp(int by_key, pl_cell_t **e, int a, int b) { return by_key ? rt_pl_cell_compare((pl_cell_t *)e[a]->p, (pl_cell_t *)e[b]->p) : rt_pl_cell_compare(e[a], e[b]); }
static int *plc_merge_order(int by_key, pl_cell_t **e, int n, int *ord, int *tmp) {
    for (int i = 0; i < n; i++) ord[i] = i;
    for (int w = 1; w < n; w *= 2) {
        for (int lo = 0; lo < n; lo += 2 * w) {
            int mid = lo + w < n ? lo + w : n, hi = lo + 2 * w < n ? lo + 2 * w : n, a = lo, b = mid, k = lo;
            while (a < mid && b < hi) tmp[k++] = plc_ord_cmp(by_key, e, ord[a], ord[b]) <= 0 ? ord[a++] : ord[b++];
            while (a < mid) tmp[k++] = ord[a++];
            while (b < hi) tmp[k++] = ord[b++];
        }
        { int *t = ord; ord = tmp; tmp = t; }
    }
    return ord;
}
int rt_pl_sort_cell(int do_msort, void *list_cell, void *result_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    long nl = plc_list_cells(list_cell, dot_id);
    if (nl < 0) return 0;
    pl_cell_t *elems[nl + 1];
    int n = 0;
    pl_cell_t *cur = pl_deref((pl_cell_t *)list_cell);
    while (n < nl && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = &aa[0];
        cur = pl_deref(&aa[1]);
    }
    int ordb[n + 1], tmpb[n + 1];
    int *ord = plc_merge_order(0, elems, n, ordb, tmpb);
    int m = 0;
    for (int i = 0; i < n; i++) { if (!do_msort && m > 0 && rt_pl_cell_compare(elems[ord[m - 1]], elems[ord[i]]) == 0) continue; ord[m++] = ord[i]; }
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[ord[i]], (int)elems[ord[i]]->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)result_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_pairs_extract(void *list_cell, pl_cell_t **elems, int dot_id, int dash_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    if (plc_list_cells(list_cell, dot_id) < 0) return -1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        pl_cell_t *pr = pl_deref(&aa[0]);
        if (!(pr && (int)pr->v == DT_PLREF && plc_fid_name(pr->slen) == dash_id && plc_fid_arity(pr->slen) == 2)) return -1;
        elems[n++] = pr;
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_plain_list_extract(void *list_cell, pl_cell_t **elems, int dot_id) {
    pl_cell_t *cur = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    int n = 0;
    if (plc_list_cells(list_cell, dot_id) < 0) return -1;
    while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == dot_id && plc_fid_arity(cur->slen) == 2) {
        pl_cell_t *aa = (pl_cell_t *)cur->p;
        elems[n++] = pl_deref(&aa[0]);
        cur = pl_deref(&aa[1]);
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bag_prep_cell(int is_setof, void *list_cell, void *result_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    for (int i = 1; i < n; i++) {
        pl_cell_t *key = elems[i];
        int j = i - 1;
        if (is_setof) {
            while (j >= 0 && rt_pl_cell_compare(elems[j], key) > 0) { elems[j + 1] = elems[j]; j--; }
        } else {
            while (j >= 0 && rt_pl_cell_compare((pl_cell_t *)elems[j]->p, (pl_cell_t *)key->p) > 0) { elems[j + 1] = elems[j]; j--; }
        }
        elems[j + 1] = key;
    }
    int m = 0;
    int out_idx[n + 1];
    for (int i = 0; i < n; i++) { if (is_setof && m > 0 && rt_pl_cell_compare(elems[out_idx[m - 1]], elems[i]) == 0) continue; out_idx[m++] = i; }
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = m - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[out_idx[i]], (int)elems[out_idx[i]]->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!pl_unify((pl_cell_t *)result_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_keysort_cell(void *list_cell, void *result_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    int ordb[n + 1], tmpb[n + 1];
    int *ord = plc_merge_order(1, elems, n, ordb, tmpb);
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = nil;
    for (int i = n - 1; i >= 0; i--) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_ref(elems[ord[i]], (int)elems[ord[i]]->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)result_cell, &result, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_inplace_cmp(int mode, pl_cell_t *vals, int a, int b) { return mode == 2 ? rt_pl_cell_compare((pl_cell_t *)vals[a].p, (pl_cell_t *)vals[b].p) : rt_pl_cell_compare(&vals[a], &vals[b]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_sort_in_place_cell(int mode, void *list_cell) {
    int dot_id = prolog_atom_intern(".");
    int m = 0;
    long nl = plc_list_cells(list_cell, dot_id);
    int n = nl > 0 ? (int)nl : 0;
    pl_cell_t *cur;
    if (nl < 0) return 0;
    if (n == 0) return 1;
    pl_cell_t *kids[n];
    pl_cell_t vals[n];
    int ordb[n], tmpb[n];
    int *ord = ordb, *tmp = tmpb;
    cur = pl_deref((pl_cell_t *)list_cell);
    for (int i = 0; i < n; i++) {
        pl_cell_t *d;
        kids[i] = (pl_cell_t *)cur->p;
        d = pl_deref(&kids[i][0]);
        vals[i] = pl_cell_unbound(d) ? pl_make_ref(d, (int)d->slen) : *d;
        ord[i] = i;
        cur = pl_deref(&kids[i][1]);
    }
    for (int w = 1; w < n; w *= 2) {
        for (int lo = 0; lo < n; lo += 2 * w) {
            int mid = lo + w < n ? lo + w : n, hi = lo + 2 * w < n ? lo + 2 * w : n, a = lo, b = mid, k = lo;
            while (a < mid && b < hi) tmp[k++] = plc_inplace_cmp(mode, vals, ord[a], ord[b]) <= 0 ? ord[a++] : ord[b++];
            while (a < mid) tmp[k++] = ord[a++];
            while (b < hi) tmp[k++] = ord[b++];
        }
        { int *t = ord; ord = tmp; tmp = t; }
    }
    for (int i = 0; i < n; i++) { if (mode == 0 && m > 0 && rt_pl_cell_compare(&vals[ord[m - 1]], &vals[ord[i]]) == 0) continue; ord[m++] = ord[i]; }
    for (int i = 0; i < m; i++) kids[i][0] = vals[ord[i]];
    if (m < n) kids[m - 1][1] = plc_nil_cell();
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_group_pairs_by_key_cell(void *list_cell, void *result_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *elems[plc_list_len(list_cell, dot_id) + 1];
    int n = plc_pairs_extract(list_cell, elems, dot_id, dash_id);
    if (n < 0) return 0;
    pl_cell_t groups[n + 1];
    int ng = 0;
    pl_cell_t nil = plc_nil_cell();
    int i = 0;
    while (i < n) {
        pl_cell_t *key = pl_deref(&((pl_cell_t *)elems[i]->p)[0]);
        pl_cell_t *vlist[n - i];
        int nv = 0;
        int j = i;
        while (j < n && rt_pl_cell_compare(pl_deref(&((pl_cell_t *)elems[j]->p)[0]), key) == 0) { vlist[nv++] = &((pl_cell_t *)elems[j]->p)[1]; j++; }
        pl_cell_t vals = nil;
        for (int k = nv - 1; k >= 0; k--) vals = plc_cons(dot_id, pl_make_ref(vlist[k], (int)vlist[k]->slen), vals);
        groups[ng++] = plc_cons(dash_id, pl_make_ref(key, (int)key->slen), vals);
        i = j;
    }
    pl_cell_t result = nil;
    for (int k = ng - 1; k >= 0; k--) result = plc_cons(dot_id, groups[k], result);
    if (!pl_unify((pl_cell_t *)result_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_pairs_keys_values_cell(void *pairs_cell, void *keys_cell, void *values_cell) {
    int dot_id = prolog_atom_intern(".");
    int dash_id = prolog_atom_intern("-");
    pl_cell_t *pd = pairs_cell ? pl_deref((pl_cell_t *)pairs_cell) : (pl_cell_t *)0;
    if (pd && !pl_cell_unbound(pd)) {
        pl_cell_t *elems[plc_list_len(pairs_cell, dot_id) + 1];
        int n = plc_pairs_extract(pairs_cell, elems, dot_id, dash_id);
        if (n < 0) return 0;
        pl_cell_t keys = plc_nil_cell(), vals = plc_nil_cell();
        for (int i = n - 1; i >= 0; i--) {
            pl_cell_t *aa = (pl_cell_t *)elems[i]->p;
            keys = plc_cons(dot_id, pl_make_ref(&aa[0], (int)aa[0].slen), keys);
            vals = plc_cons(dot_id, pl_make_ref(&aa[1], (int)aa[1].slen), vals);
        }
        if (!pl_unify((pl_cell_t *)keys_cell, &keys) || !pl_unify((pl_cell_t *)values_cell, &vals)) { return 0; }
        return 1;
    }
    pl_cell_t *kelems[plc_list_len(keys_cell, dot_id) + 1];
    int nk = plc_plain_list_extract(keys_cell, kelems, dot_id);
    pl_cell_t *velems[plc_list_len(values_cell, dot_id) + 1];
    int nv = plc_plain_list_extract(values_cell, velems, dot_id);
    if (nk < 0 || nv < 0 || nk != nv) return 0;
    pl_cell_t result = plc_nil_cell();
    for (int i = nk - 1; i >= 0; i--) {
        pl_cell_t pair = plc_cons(dash_id, pl_make_ref(kelems[i], (int)kelems[i]->slen), pl_make_ref(velems[i], (int)velems[i]->slen));
        result = plc_cons(dot_id, pair, result);
    }
    if (!pl_unify((pl_cell_t *)pairs_cell, &result)) { return 0; }
    return 1;
}
static int plc_nv_visit(pl_cell_t *d, void *ctx) {
    plc_nv_t *k = (plc_nv_t *)ctx;
    if (pl_cell_unbound(d)) { pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1); *a = pl_make_int(k->counter++); plc_bind_cx(d, pl_make_compound(k->var_id, 1, a), k->cx); }
    return PLR_V_GO;
}
PLR_WALK1(plc_nv_walk, plc_nv_visit, pl_deref) static long pl_numbervars_walk(pl_cell_t *c, long counter, int var_id, pl_tr_ctx_t *cx) {
    plc_nv_t k;
    k.counter = counter;
    k.var_id = var_id;
    k.cx = cx;
    (void)plc_nv_walk(c, (void *)&k);
    return k.counter;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars_cell(void *term_cell, void *start_cell, void *end_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *st = pl_deref((pl_cell_t *)start_cell);
    if (!st || (int)st->v != DT_I) return 0;
    long counter = (long)st->i;
    int var_id = prolog_atom_intern("$VAR");
    counter = pl_numbervars_walk((pl_cell_t *)term_cell, counter, var_id, cx);
    pl_cell_t endv = pl_make_int((int64_t)counter);
    if (!plc_unify_cells_cx((pl_cell_t *)end_cell, &endv, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_numbervars1_cell(void *term_cell, pl_tr_ctx_t *cx) { int var_id = prolog_atom_intern("$VAR"); pl_numbervars_walk((pl_cell_t *)term_cell, 0, var_id, cx); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_get_print_stream_cell(void *stream_cell) {
    extern int fh_current_output(void);
    int stream_id = prolog_atom_intern("$stream");
    pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
    *arg = pl_make_int(fh_current_output());
    pl_cell_t sc = pl_make_compound(stream_id, 1, arg);
    if (!pl_unify((pl_cell_t *)stream_cell, &sc)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_distinct_vars_walk(pl_cell_t *c, pl_cell_t **pool, int *counts, int *pool_n) { plc_tv_t t; t.pool = pool; t.n = pool_n; t.counts = counts; if (c) (void)plc_tv_walk(c, (void *)&t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_atom_text(pl_cell_t *d) {
    if ((int)d->v == DT_S || d->v == DT_SNUL) return d->s ? d->s : "";
    if ((int)d->v == DT_PLATOM) { const char *nm = prolog_atom_name((int)d->i); return nm ? nm : ""; }
    return (const char *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_is_nil(pl_cell_t *d) { const char *s = pl_atom_text(d); return s && !strcmp(s, "[]"); }
static pl_cell_t pl_nil_cell(void) { return pl_make_atom(g_runtime.atoms.atom_nil); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_singleton_vars_cell(void *term_cell) {
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1;
    pl_cell_t *pool[occ];
    int counts[occ];
    int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, counts, &pn);
    int varname_id = prolog_atom_intern("$VARNAME");
    int underscore_id = prolog_atom_intern("_");
    for (int i = 0; i < pn; i++) {
        if (counts[i] != 1) continue;
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
        *arg = pl_make_atom(underscore_id);
        pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_name_query_vars_cell(void *list_cell, void *rest_cell) {
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    int dot_id = prolog_atom_intern(".");
    int varname_id = prolog_atom_intern("$VARNAME");
    if (plc_list_cells(list_cell, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref((pl_cell_t *)list_cell)); return 0; }
    pl_cell_t *rest_pairs[plc_list_len(list_cell, dot_id) + 1];
    int nr = 0;
    pl_cell_t *cur = (pl_cell_t *)list_cell;
    for (;;) {
        pl_cell_t *d = pl_deref(cur);
        if (pl_cell_unbound(d)) { rt_pl_iso_throw_instantiation(); return 0; }
        if (pl_is_nil(d)) break;
        if ((int)d->v != DT_PLREF || pl_arity(d) != 2 || plc_functor(d) != dot_id) { rt_pl_iso_throw_type("list", *d); return 0; }
        pl_cell_t *kids = (pl_cell_t *)pl_compound_heap(d);
        pl_cell_t *pair = pl_deref(&kids[0]);
        int bound_ok = 0;
        if ((int)pair->v == DT_PLREF && pl_arity(pair) == 2) {
            const char *fn = prolog_atom_name(plc_functor(pair));
            if (fn && !strcmp(fn, "=")) {
                pl_cell_t *pk = (pl_cell_t *)pl_compound_heap(pair);
                pl_cell_t *nm = pl_deref(&pk[0]);
                pl_cell_t *vr = pl_deref(&pk[1]);
                const char *nm_txt = pl_atom_text(nm);
                if (nm_txt && pl_cell_unbound(vr)) { pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1); *arg = *nm; pl_bind(vr, pl_make_compound(varname_id, 1, arg)); bound_ok = 1; }
            }
        }
        if (!bound_ok) rest_pairs[nr++] = &kids[0];
        cur = &kids[1];
    }
    pl_cell_t result = pl_nil_cell();
    for (int i = nr - 1; i >= 0; i--) { pl_cell_t *blk = (pl_cell_t *)rt_ws_alloc_descr(2); blk[0] = *rest_pairs[i]; blk[1] = result; result = pl_make_compound(dot_id, 2, blk); }
    if (!pl_unify((pl_cell_t *)rest_cell, &result)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_namevars_letters(long n, char *buf, size_t cap) {
    long letter = n % 26, block = n / 26;
    if (block == 0) snprintf(buf, cap, "%c", (int)('A' + letter));
    else snprintf(buf, cap, "%c%ld", (int)('A' + letter), block);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_bind_variables_cell(void *term_cell, void *options_cell) {
    extern void rt_pl_iso_throw_instantiation(void);
    extern void rt_pl_iso_throw_type(const char *, DESCR_t);
    extern void rt_pl_iso_throw_domain(const char *, DESCR_t);
    int dot_id = prolog_atom_intern(".");
    int var_id = prolog_atom_intern("$VAR");
    int varname_id = prolog_atom_intern("$VARNAME");
    int use_namevars = 0;
    long from_n = 0;
    pl_cell_t *next_var = (pl_cell_t *)0;
    pl_cell_t *exclude_list = (pl_cell_t *)0;
    pl_cell_t *cur = (pl_cell_t *)options_cell;
    if (plc_list_cells(options_cell, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref((pl_cell_t *)options_cell)); return 0; }
    for (;;) {
        pl_cell_t *d = pl_deref(cur);
        if (pl_cell_unbound(d)) { rt_pl_iso_throw_instantiation(); return 0; }
        if (pl_is_nil(d)) break;
        if ((int)d->v != DT_PLREF || pl_arity(d) != 2 || plc_functor(d) != dot_id) { rt_pl_iso_throw_type("list", *d); return 0; }
        pl_cell_t *kids = (pl_cell_t *)pl_compound_heap(d);
        pl_cell_t *opt = pl_deref(&kids[0]);
        if (pl_cell_unbound(opt)) { rt_pl_iso_throw_instantiation(); return 0; }
        const char *opt_txt = pl_atom_text(opt);
        if (opt_txt) {
            if (!strcmp(opt_txt, "numbervars")) use_namevars = 0;
            else if (!strcmp(opt_txt, "namevars")) use_namevars = 1;
            else { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
        } else if ((int)opt->v == DT_PLREF && pl_arity(opt) == 1) {
            const char *f = prolog_atom_name(plc_functor(opt));
            pl_cell_t *oa = (pl_cell_t *)pl_compound_heap(opt);
            pl_cell_t *av = pl_deref(&oa[0]);
            if (f && !strcmp(f, "from")) {
                if (pl_cell_unbound(av)) { rt_pl_iso_throw_instantiation(); return 0; }
                if ((int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
                from_n = (long)av->i;
            } else if (f && !strcmp(f, "next")) {
                if (!pl_cell_unbound(av) && (int)av->v != DT_I) { rt_pl_iso_throw_domain("var_binding_option", *opt); return 0; }
                next_var = &oa[0];
            } else if (f && !strcmp(f, "exclude")) {
                exclude_list = &oa[0];
            } else {
                rt_pl_iso_throw_domain("var_binding_option", *opt);
                return 0;
            }
        } else {
            rt_pl_iso_throw_domain("var_binding_option", *opt);
            return 0;
        }
        cur = &kids[1];
    }
    if (exclude_list && plc_list_cells(exclude_list, dot_id) < 0) { rt_pl_iso_throw_type("list", *pl_deref(exclude_list)); return 0; }
    long used[exclude_list ? plc_list_len(exclude_list, dot_id) + 1 : 1];
    int nused = 0;
    if (exclude_list) {
        pl_cell_t *ecur = exclude_list;
        for (;;) {
            pl_cell_t *ed = pl_deref(ecur);
            if (pl_is_nil(ed)) break;
            if ((int)ed->v != DT_PLREF || pl_arity(ed) != 2 || plc_functor(ed) != dot_id) break;
            pl_cell_t *ekids = (pl_cell_t *)pl_compound_heap(ed);
            pl_cell_t *el = pl_deref(&ekids[0]);
            if ((int)el->v == DT_PLREF && pl_arity(el) == 1 && plc_functor(el) == var_id) {
                pl_cell_t *ea = (pl_cell_t *)pl_compound_heap(el);
                pl_cell_t *ev = pl_deref(&ea[0]);
                if ((int)ev->v == DT_I) used[nused++] = (long)ev->i;
            }
            ecur = &ekids[1];
        }
    }
    int occ = plc_var_occ((pl_cell_t *)term_cell) + 1;
    pl_cell_t *pool[occ];
    int dummy_counts[occ];
    int pn = 0;
    pl_distinct_vars_walk((pl_cell_t *)term_cell, pool, dummy_counts, &pn);
    long n = from_n;
    for (int i = 0; i < pn; i++) {
        for (;;) { int clash = 0; for (int k = 0; k < nused; k++) if (used[k] == n) { clash = 1; break; } if (!clash) break; n++; }
        pl_cell_t *arg = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (use_namevars) {
            char nb[32];
            pl_namevars_letters(n, nb, sizeof nb);
            *arg = pl_make_atom(prolog_atom_intern(nb));
            pl_bind(pool[i], pl_make_compound(varname_id, 1, arg));
        } else {
            *arg = pl_make_int(n);
            pl_bind(pool[i], pl_make_compound(var_id, 1, arg));
        }
        n++;
    }
    if (next_var) { pl_cell_t nv = pl_make_int(n); if (!pl_unify(next_var, &nv)) { return 0; } }
    return 1;
}
int pl_acyclic_walk(pl_cell_t *c) {
    pl_cell_t *first = (pl_cell_t *)0;
    long nmarked = 0;
    int ret = 1;
    for (;;) {
        pl_cell_t *d = pl_deref(c);
        if (d->v == PL_ACYC_ON_PATH) { ret = 0; break; }
        if ((int)d->v != DT_PLREF) break;
        int ar = pl_arity(d);
        pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(d);
        if (ar <= 0) break;
        d->v = PL_ACYC_ON_PATH;
        if (!nmarked++) first = d;
        int bad = 0;
        for (int i = 0; i < ar - 1; i++) if (!pl_acyclic_walk(&aa[i])) { bad = 1; break; }
        if (bad) { ret = 0; break; }
        c = &aa[ar - 1];
    }
    for (pl_cell_t *d = first; nmarked > 0; nmarked--) { d->v = (DTYPE_t)DT_PLREF; if (nmarked > 1) d = pl_deref(&((pl_cell_t *)pl_compound_heap(d))[pl_arity(d) - 1]); }
    return ret;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_acyclic_cell(void *term_cell) { return term_cell ? pl_acyclic_walk((pl_cell_t *)term_cell) : 1; }
int rt_pl_factorize_term_cell(void *term_cell, void *skel_cell, void *subst_cell, pl_tr_ctx_t *cx) {
    pl_cell_t f = plc_factorize_x((pl_cell_t *)term_cell, 0, 1);
    pl_cell_t *fa = (pl_cell_t *)f.p;
    return plc_unify_cells_cx((pl_cell_t *)skel_cell, &fa[0], cx) && plc_unify_cells_cx((pl_cell_t *)subst_cell, &fa[1], cx) ? 1 : 0;
}
int rt_pl_copy_term_nat_cell(void *term_cell, void *copy_cell, pl_tr_ctx_t *cx) {
    pl_cell_t copy = plc_copy_x((pl_cell_t *)term_cell, 0);
    if (!plc_unify_cells_cx((pl_cell_t *)copy_cell, &copy, cx)) { return 0; }
    return 1;
}
static inline pl_cell_t *fa_items(DESCR_t *a) { return (pl_cell_t *)a[1].ptr; }
static inline int fa_n(DESCR_t *a) { return (int)a[0].i; }
static inline int fa_cap(DESCR_t *a) { return (int)a[0].slen; }
static inline void fa_set(DESCR_t *a, int n, int cap) { a[0].v = (DTYPE_t)DT_I; a[0].slen = (uint32_t)cap; a[0].i = (int64_t)n; }
static inline void fa_items_set(DESCR_t *a, pl_cell_t *p) { a[1].v = (DTYPE_t)DT_DATA; a[1].slen = DATA_ELEMS_SLEN; a[1].ptr = (void *)p; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_findall_begin(void) {
    DESCR_t *a = (DESCR_t *)rt_ws_alloc_descr(2);
    if (!a) return (void *)0;
    { pl_cell_t *it = (pl_cell_t *)rt_ws_alloc_descr(16); if (!it) return (void *)0; fa_set(a, 0, 16); fa_items_set(a, it); }
    return (void *)a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_collect(void *acc_v, void *tmpl_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    if (!a || !fa_items(a)) return;
    if (fa_n(a) >= fa_cap(a)) {
        extern int rt_pl_cap_refuses(unsigned long long, unsigned long long);
        extern int rt_gcheap_cap_refused(void);
        int nc = fa_cap(a) * 2;
        if (rt_pl_cap_refuses((unsigned long long)nc + (unsigned long long)fa_cap(a), sizeof(DESCR_t)) && rt_gcheap_cap_refused()) return;
        pl_cell_t *ni = (pl_cell_t *)rt_ws_alloc_descr((size_t)nc);
        if (!ni) return;
        for (int i = 0; i < fa_n(a); i++) ni[i] = fa_items(a)[i];
        fa_items_set(a, ni);
        fa_set(a, fa_n(a), nc);
    }
    { int n = fa_n(a); pl_cell_t item = plc_copy_x((pl_cell_t *)tmpl_term, 1); fa_items(a)[n] = item; fa_set(a, n + 1, fa_cap(a)); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_count(void *acc_v) { DESCR_t *a = (DESCR_t *)acc_v; return a ? fa_n(a) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_findall_item(void *acc_v, int i, void *out) { DESCR_t *a = (DESCR_t *)acc_v; if (!a || !out || i < 0 || i >= fa_n(a)) return; *(pl_cell_t *)out = fa_items(a)[i]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_findall_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int dot = prolog_atom_intern(".");
    pl_cell_t lst;
    { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name(prolog_atom_intern("[]")); lst = pl_make_atom(prolog_atom_intern(nm ? nm : "")); }
    int n = a ? fa_n(a) : 0;
    for (int i = n - 1; i >= 0; i--) { pl_cell_t *c = (pl_cell_t *)rt_ws_alloc_descr(2); if (!c) return 0; c[0] = fa_items(a)[i]; c[1] = lst; lst = pl_make_compound(dot, 2, c); }
    if (!pl_unify((pl_cell_t *)result_term, &lst)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_count_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    pl_cell_t cnt = pl_make_int((int64_t)(a ? fa_n(a) : 0));
    if (!pl_unify((pl_cell_t *)result_term, &cnt)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int agg_num(pl_cell_t *c, long *iv, double *dv, int *isf) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    if (!d) return 0;
    if ((int)d->v == DT_I) { *iv = (long)d->i; *isf = 0; return 1; }
    if ((int)d->v == DT_R) { *dv = d->r; *isf = 1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_sum_finish(void *acc_v, void *result_term) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    long si = 0;
    double sd = 0.0;
    int isf = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0;
        double dv = 0.0;
        int ef = 0;
        if (!agg_num(&fa_items(a)[i], &iv, &dv, &ef)) return 0;
        if (ef) { sd += dv; isf = 1; } else { si += iv; sd += (double)iv; }
    }
    pl_cell_t r = isf ? pl_make_float(sd) : pl_make_int((int64_t)si);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_agg_minmax_finish(void *acc_v, void *result_term, int want_max) {
    DESCR_t *a = (DESCR_t *)acc_v;
    int n = a ? fa_n(a) : 0;
    if (n <= 0) return 0;
    long bi = 0;
    double bd = 0.0;
    int isf = 0, have = 0;
    for (int i = 0; i < n; i++) {
        long iv = 0;
        double dv = 0.0;
        int ef = 0;
        if (!agg_num(&fa_items(a)[i], &iv, &dv, &ef)) return 0;
        double cur = ef ? dv : (double)iv;
        double best = isf ? bd : (double)bi;
        if (!have || (want_max ? (cur > best) : (cur < best))) { if (ef) { bd = dv; isf = 1; } else { bi = iv; bd = (double)iv; isf = 0; } have = 1; }
    }
    pl_cell_t r = isf ? pl_make_float(bd) : pl_make_int((int64_t)bi);
    if (!pl_unify((pl_cell_t *)result_term, &r)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_agg_max_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 1); }
int rt_pl_agg_min_finish(void *acc_v, void *result_term) { return rt_pl_agg_minmax_finish(acc_v, result_term, 0); }
