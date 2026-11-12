#include "runtime/unification/un_head.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_06_pl.h"
#include "runtime/unification/un_08_pl.h"
int rt_pl_trie_tbl_vt(void *root);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t plc_nil_cell(void) { return pl_make_atom(g_runtime.atoms.atom_nil); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t plc_cons(int fid, pl_cell_t head, pl_cell_t tail) {
    pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
    blk[0] = head;
    blk[1] = tail;
    return pl_make_compound(fid, 2, blk);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_trie_gc_visit(uint16_t type, void *p) {
    extern void rt_gc_visit_raw(const char **);
    extern void rt_gc_visit_descr(DESCR_t *);
    if (type == HB_TRIE) {
        pl_trie_t *t = (pl_trie_t *)p;
        if (t->root) rt_gc_visit_raw((const char **)&t->root);
        if (t->ents) rt_gc_visit_raw((const char **)&t->ents);
        rt_gc_visit_descr(&t->status);
        rt_gc_visit_descr(&t->data);
        if (t->sl.pcons.v) rt_gc_visit_descr(&t->sl.pcons);
        if (t->sl.ncons.v) rt_gc_visit_descr(&t->sl.ncons);
        if (t->sl.sched) rt_gc_visit_raw((const char **)(void *)&t->sl.sched);
        return;
    }
    {
        pl_trie_node_t *n = (pl_trie_node_t *)p;
        rt_gc_visit_descr(&n->tok);
        rt_gc_visit_descr(&n->val);
        if (n->parent) rt_gc_visit_raw((const char **)&n->parent);
        if (n->kids) rt_gc_visit_raw((const char **)&n->kids);
    }
}
static pl_trie_node_t * pl_trie_node_new(pl_trie_node_t *parent, int kind, DESCR_t tok) {
    pl_trie_node_t *n = (pl_trie_node_t *)rt_pl_struct_alloc(HB_TRIEN, sizeof *n);
    if (!n) return (pl_trie_node_t *)0;
    memset(n, 0, sizeof *n);
    n->tok = tok;
    n->kind = kind;
    n->parent = parent;
    n->ent = -1;
    n->depth = parent ? parent->depth + 1 : 0;
    return n;
}
pl_trie_t * pl_trie_at(void *root, int64_t id) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    if (!r || !r->tries || id < 0 || id >= r->ntries) return (pl_trie_t *)0;
    { pl_trie_t *t = (pl_trie_t *)r->tries[id]; return (t && t->alive) ? t : (pl_trie_t *)0; }
}
int rt_pl_trie_new(void *root) {
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return -1;
    if (r->ntries >= r->tcap) { int nc = r->tcap > 0 ? 2 * r->tcap : 8; void **nv = (void **)rt_pvec_realloc(r->tries, (size_t)nc); if (!nv) return -1; r->tries = nv; r->tcap = nc; }
    {
        pl_trie_t *t = (pl_trie_t *)rt_pl_struct_alloc(HB_TRIE, sizeof *t);
        DESCR_t none;
        if (!t) return -1;
        memset(t, 0, sizeof *t);
        memset(&none, 0, sizeof none);
        t->root = pl_trie_node_new((pl_trie_node_t *)0, 0, none);
        if (!t->root) return -1;
        t->alive = 1;
        r->tries[r->ntries] = t;
        return r->ntries++;
    }
}
int rt_pl_trie_alive(void *root, int64_t id) { return pl_trie_at(root, id) ? 1 : 0; }
int rt_pl_trie_destroy(void *root, int64_t id) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_trie_t *t = pl_trie_at(root, id);
    if (!t) return 0;
    t->alive = 0;
    r->tries[id] = (void *)0;
    if (r->tbl_vt == id) r->tbl_vt = -1;
    return 1;
}
static int pl_trie_tok_eq(int ka, const DESCR_t *a, int kb, const DESCR_t *b) {
    if (ka != kb) return 0;
    if (ka != 1) return a->i == b->i;
    if (a->v != b->v) return 0;
    if ((int)a->v == DT_PLATOM || (int)a->v == DT_I) return a->i == b->i;
    if ((int)a->v == DT_R) return !memcmp(&a->r, &b->r, sizeof a->r);
    return rt_pl_cell_compare((pl_cell_t *)a, (pl_cell_t *)b) == 0;
}
static uint64_t pl_trie_tok_hash(int kind, const DESCR_t *t) {
    uint64_t h = (uint64_t)kind * 0x9E3779B97F4A7C15ull;
    if (kind != 1 || (int)t->v == DT_PLATOM || (int)t->v == DT_I) h ^= (uint64_t)t->i;
    else if ((int)t->v == DT_R) { uint64_t b; memcpy(&b, &t->r, sizeof b); h ^= b; }
    if (kind == 1) h ^= (uint64_t)(uint32_t)t->v << 40;
    h *= 0xFF51AFD7ED558CCDull;
    return h ^ (h >> 29);
}
static int pl_trie_kid_slot(void **kids, int cap, int kind, const DESCR_t *tok) {
    int m = cap - 1, i = (int)(pl_trie_tok_hash(kind, tok) & (uint64_t)m);
    for (pl_trie_node_t *c; (c = (pl_trie_node_t *)kids[i]) != 0; i = (i + 1) & m) if (pl_trie_tok_eq(c->kind, &c->tok, kind, tok)) return i;
    return i;
}
static int pl_trie_kids_rehash(pl_trie_node_t *n, int nc) {
    void **nv = (void **)rt_pvec_alloc((size_t)nc);
    if (!nv) return 0;
    if (n->kcap > PL_TRIE_LINEAR_KIDS) {
        for (int i = 0; i < n->kcap; i++) if (n->kids[i]) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; nv[pl_trie_kid_slot(nv, nc, c->kind, &c->tok)] = c; }
    } else {
        for (int i = 0; i < n->nkids; i++) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; nv[pl_trie_kid_slot(nv, nc, c->kind, &c->tok)] = c; }
    }
    n->kids = nv;
    n->kcap = nc;
    return 1;
}
static pl_trie_node_t * pl_trie_child(pl_trie_node_t *n, int kind, const DESCR_t *tok, int create) {
    if (n->kcap > PL_TRIE_LINEAR_KIDS) {
        int s = pl_trie_kid_slot(n->kids, n->kcap, kind, tok);
        pl_trie_node_t *c = (pl_trie_node_t *)n->kids[s];
        if (c || !create) return c;
        if (2 * (n->nkids + 1) > n->kcap) { if (!pl_trie_kids_rehash(n, 2 * n->kcap)) return (pl_trie_node_t *)0; s = pl_trie_kid_slot(n->kids, n->kcap, kind, tok); }
        if (!(c = pl_trie_node_new(n, kind, *tok))) return (pl_trie_node_t *)0;
        n->kids[s] = (void *)c;
        n->nkids++;
        return c;
    }
    for (int i = 0; i < n->nkids; i++) { pl_trie_node_t *c = (pl_trie_node_t *)n->kids[i]; if (pl_trie_tok_eq(c->kind, &c->tok, kind, tok)) return c; }
    if (!create) return (pl_trie_node_t *)0;
    if (n->nkids >= PL_TRIE_LINEAR_KIDS) return pl_trie_kids_rehash(n, 4 * PL_TRIE_LINEAR_KIDS) ? pl_trie_child(n, kind, tok, create) : (pl_trie_node_t *)0;
    if (n->nkids >= n->kcap) { int nc = n->kcap > 0 ? 2 * n->kcap : 4; void **nv = (void **)rt_pvec_realloc(n->kids, (size_t)nc); if (!nv) return (pl_trie_node_t *)0; n->kids = nv; n->kcap = nc; }
    { pl_trie_node_t *c = pl_trie_node_new(n, kind, *tok); if (!c) return (pl_trie_node_t *)0; n->kids[n->nkids++] = (void *)c; return c; }
}
static int pl_trie_walk_run(void *wv) {
    pl_trie_w_t *w = (pl_trie_w_t *)wv;
    for (;;) {
        pl_cell_t *d = pl_deref(w->s);
        DESCR_t tok;
        int kind;
        if (d->v == PLR_VCOPY) {
            kind = 3;
            tok = pl_make_int((int64_t)(intptr_t)d->p);
        } else if (pl_cell_unbound(d)) {
            uint64_t w0;
            if (pl_attv_is(d)) { w->attv = 1; w->miss = 1; }
            if (!plr_room(&w->lg)) return plr_more(&w->ag, &w->lg, pl_trie_walk_run, w);
            memcpy(&w0, d, 8);
            plr_push(&w->lg, d, d->p, w0);
            kind = 3;
            tok = pl_make_int(w->nvar);
            d->v = PLR_VCOPY;
            d->p = (void *)(intptr_t)w->nvar;
            w->nvar++;
        } else if ((int)d->v == DT_PLREF) {
            int ar = plc_fid_arity(d->slen);
            if (ar > 0 && !plr_room(&w->ag)) return plr_more(&w->ag, &w->lg, pl_trie_walk_run, w);
            kind = 2;
            tok = pl_make_int((int64_t)d->slen);
            if (ar > 0) plr_push(&w->ag, d->p, (void *)0, (uint64_t)ar);
        } else {
            kind = 1;
            tok = *d;
        }
        if (!w->miss) { pl_trie_node_t *c = pl_trie_child(w->cur, kind, &tok, w->create); if (c) w->cur = c; else w->miss = 1; }
        if (plr_empty(&w->ag)) break;
        { plr_ent_t *t = plr_peek(&w->ag); w->s = (pl_cell_t *)t->x; t->x = (void *)(w->s + 1); if (!--t->n) plr_drop(&w->ag); }
    }
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
static pl_trie_node_t * pl_trie_descend(pl_trie_t *t, pl_cell_t *key, int create, int *attv) {
    pl_trie_w_t w;
    plr_stk_init(&w.ag);
    plr_stk_init(&w.lg);
    w.s = key;
    w.cur = t->root;
    w.create = create;
    w.nvar = 0;
    w.miss = 0;
    w.attv = 0;
    (void)pl_trie_walk_run(&w);
    if (attv) *attv = w.attv;
    return w.miss ? (pl_trie_node_t *)0 : w.cur;
}
int rt_pl_trie_insert(void *root, int64_t id, void *key, void *val, int update) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n;
    if (!t) return -1;
    { int av = 0; n = pl_trie_descend(t, (pl_cell_t *)key, 1, &av); if (av) return -4; }
    if (!n) return -1;
    if (n->has_val && !update) return rt_pl_cell_compare(pl_deref((pl_cell_t *)val), &n->val) == 0 ? -2 : -3;
    if (n->ent < 0) {
        if (t->nents >= t->ecap) { int nc = t->ecap > 0 ? 2 * t->ecap : 8; void **nv = (void **)rt_pvec_realloc(t->ents, (size_t)nc); if (!nv) return -1; t->ents = nv; t->ecap = nc; }
        n->ent = t->nents;
        t->ents[t->nents++] = (void *)n;
    }
    if (!n->has_val) t->nvals++;
    n->val = plc_copy(pl_deref((pl_cell_t *)val));
    n->has_val = 1;
    return n->ent;
}
int pl_trie_give(pl_cell_t src, void *out_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(1);
    if (!box) return 0;
    *box = src;
    return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0;
}
int rt_pl_trie_lookup(void *root, int64_t id, void *key, void *val_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n = t ? pl_trie_descend(t, (pl_cell_t *)key, 0, (int *)0) : (pl_trie_node_t *)0;
    if (!n || !n->has_val) return 0;
    return pl_trie_give(plc_copy(&n->val), val_cell, cx);
}
int rt_pl_trie_delete(void *root, int64_t id, void *key, void *val_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n = t ? pl_trie_descend(t, (pl_cell_t *)key, 0, (int *)0) : (pl_trie_node_t *)0;
    pl_cell_t v;
    if (!n || !n->has_val) return 0;
    v = n->val;
    n->has_val = 0;
    memset(&n->val, 0, sizeof n->val);
    if (n->ent >= 0) { t->ents[n->ent] = (void *)0; n->ent = -1; }
    t->nvals--;
    return pl_trie_give(v, val_cell, cx);
}
static int pl_trie_ents_list(pl_trie_t *t, int64_t from, int64_t hi, pl_cell_t *out) {
    pl_cell_t acc = plc_nil_cell();
    int dot_id = prolog_atom_intern("."), e_id = prolog_atom_intern("e");
    for (int64_t i = hi - 1; i >= from; i--) {
        pl_trie_node_t *n = (pl_trie_node_t *)t->ents[i];
        pl_cell_t *blk;
        if (!n || !n->has_val) continue;
        blk = (pl_cell_t *)rt_ws_alloc_descr(3);
        if (!blk) return 0;
        blk[0] = pl_make_int(i);
        blk[1] = pl_trie_key_term(n);
        blk[2] = plc_copy(&n->val);
        acc = plc_cons(dot_id, pl_make_compound(e_id, 3, blk), acc);
    }
    *out = acc;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_trie_entries(void *root, int64_t id, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t acc;
    if (!t || !pl_trie_ents_list(t, 0, t->nents, &acc)) return 0;
    return pl_trie_give(acc, out_cell, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_trie_slice(void *root, int64_t id, int64_t from, int64_t to, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t acc, *pr;
    if (!t || !pl_trie_ents_list(t, from < 0 ? 0 : from, (to < 0 || to > t->nents) ? t->nents : to, &acc)) return 0;
    pr = (pl_cell_t *)rt_ws_alloc_descr(2);
    if (!pr) return 0;
    pr[0] = pl_make_int(t->nents);
    pr[1] = acc;
    return pl_trie_give(pl_make_compound(prolog_atom_intern("-"), 2, pr), out_cell, cx);
}
int rt_pl_trie_count(void *root, int64_t id) { pl_trie_t *t = pl_trie_at(root, id); return t ? t->nvals : -1; }
int rt_pl_trie_nents(void *root, int64_t id) { pl_trie_t *t = pl_trie_at(root, id); return t ? t->nents : -1; }
int rt_pl_tbl_abolish_id(void *root, int64_t id) {
    pl_trie_t *t = pl_trie_at(root, id), *vt = pl_trie_at(root, rt_pl_trie_tbl_vt(root));
    if (!t) return 0;
    if (vt) {
        pl_cell_t *d = pl_deref((pl_cell_t *)&t->data);
        if ((int)d->v == DT_PLREF && d->p) {
            pl_trie_node_t *n = pl_trie_descend(vt, (pl_cell_t *)d->p, 0, (int *)0);
            if (n && n->has_val) { n->has_val = 0; memset(&n->val, 0, sizeof n->val); if (n->ent >= 0) { vt->ents[n->ent] = (void *)0; n->ent = -1; } vt->nvals--; }
        }
    }
    return rt_pl_trie_destroy(root, id);
}
pl_trie_sl_t * rt_pl_trie_sl(void *root, int64_t id) { pl_trie_t *t = pl_trie_at(root, id); return t ? &t->sl : (pl_trie_sl_t *)0; }
int rt_pl_trie_meta_set(void *root, int64_t id, int which, void *cell) {
    pl_trie_t *t = pl_trie_at(root, id);
    if (!t) return 0;
    if (which) t->data = plc_copy(pl_deref((pl_cell_t *)cell));
    else t->status = plc_copy(pl_deref((pl_cell_t *)cell));
    return 1;
}
int rt_pl_trie_meta_get(void *root, int64_t id, int which, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_cell_t *src;
    if (!t) return 0;
    src = which ? &t->data : &t->status;
    if (src->v == DT_SNUL) return 0;
    return pl_trie_give(plc_copy(src), out_cell, cx);
}
int rt_pl_trie_tbl_vt(void *root) {
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return -1;
    if (r->tbl_vt < 0 || !pl_trie_at(root, r->tbl_vt)) r->tbl_vt = rt_pl_trie_new(root);
    return r->tbl_vt;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t * pl_tbh_cur(void *root) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
    pl_cell_t **cell = k >= 0 ? (pl_cell_t **)pl_db_cell_addr(root, k, 0) : (pl_cell_t **)0;
    pl_cell_t *v = (cell && *cell && !plc_gvar_absent(*cell)) ? pl_deref(*cell) : (pl_cell_t *)0;
    return (v && (int)v->v == DT_I && v->i) ? (DESCR_t *)(uintptr_t)v->i : (DESCR_t *)0;
}
static DESCR_t * pl_tbh_fwd(DESCR_t *h, int f) { return h ? ((int)h[f].v == DT_RAW ? (DESCR_t *)(uintptr_t)h[f].i : &h[f]) : (DESCR_t *)0; }
static DESCR_t * pl_tbh_owner(void *root, int f) { return pl_tbh_fwd(pl_tbh_cur(root), f); }
int rt_pl_tbh_open(void *root, void *arg, pl_tr_ctx_t *cx) {
    DESCR_t *a = (DESCR_t *)arg;
    if (!root || !a || (int)a->v != DT_I || !a->i) return 0;
    {
        DESCR_t *h = (DESCR_t *)(uintptr_t)a->i;
        DESCR_t *prev = pl_tbh_cur(root);
        int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
        h[0] = (DESCR_t){ .v = DT_I, .i = (int64_t)(uintptr_t)prev };
        for (int f = 1; f < PL_TBH_CELLS; f++) h[f] = (DESCR_t){ .v = DT_RAW, .i = (int64_t)(uintptr_t)pl_tbh_fwd(prev, f) };
        h[PL_TBH_CELLS + 1] = (DESCR_t){ .v = DT_I, .i = (int64_t)(uintptr_t)cx->tr };
        return k >= 0 && rt_pl_b_set(root, k, arg, cx) && rt_pl_tbh_floor_sync(root, cx->tr);
    }
}
int rt_pl_tbh_floor_sync(void *root, char *tr) {
    DESCR_t *h = pl_tbh_cur(root);
    if (!tr) return 1;
    *pl_tr_floor_slot(pl_tr_base_of(tr)) = (h && (int)h[PL_TBH_CELLS + 1].v == DT_I) ? (char *)(uintptr_t)h[PL_TBH_CELLS + 1].i : (char *)0;
    return 1;
}
int rt_pl_tbh_close(void *root, pl_tr_ctx_t *cx) {
    DESCR_t *h = pl_tbh_cur(root);
    int64_t k = rt_pl_db_cell_for(root, "$tbl_cur", -1);
    if (!h || k < 0) return 0;
    { DESCR_t prev = (DESCR_t){ .v = DT_I, .i = h[0].i }; return rt_pl_b_set(root, k, (void *)&prev, cx) && rt_pl_tbh_floor_sync(root, cx->tr); }
}
int rt_pl_tbh_leading(void *root) { return pl_tbh_cur(root) != (DESCR_t *)0; }
DESCR_t * rt_pl_tbh_current(void *root) { return pl_tbh_cur(root); }
int64_t rt_pl_tbl_chain_get(void *root) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_chain", -1);
    pl_cell_t **cell = k >= 0 ? (pl_cell_t **)pl_db_cell_addr(root, k, 0) : (pl_cell_t **)0;
    pl_cell_t *v = (cell && *cell && !plc_gvar_absent(*cell)) ? pl_deref(*cell) : (pl_cell_t *)0;
    return (v && (int)v->v == DT_I) ? v->i - 1 : -1;
}
int rt_pl_tbl_chain_set(void *root, int64_t id) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_chain", -1);
    DESCR_t v = (DESCR_t){ .v = DT_I, .i = id + 1 };
    return k >= 0 && rt_pl_nb_set(root, k, (void *)&v);
}
int64_t rt_pl_tbh_table(void *root) { DESCR_t *c = pl_tbh_owner(root, 1); return (c && (int)c->v == DT_I) ? c->i : -1; }
int rt_pl_tbh_get(void *root, int64_t f, void *out, pl_tr_ctx_t *cx) {
    DESCR_t *c = (f >= 1 && f < PL_TBH_CELLS) ? pl_tbh_owner(root, (int)f) : (DESCR_t *)0;
    return c && plc_unify_cells_cx((pl_cell_t *)out, (pl_cell_t *)c, cx) ? 1 : 0;
}
int rt_pl_tbh_put(void *root, int64_t f, int mode, void *val, pl_tr_ctx_t *cx) {
    DESCR_t *h = pl_tbh_cur(root);
    if (f < 1 || f >= PL_TBH_CELLS) return 0;
    if (!h) return mode != 0;
    {
        DESCR_t *c = mode == 0 ? &h[f] : pl_tbh_owner(root, (int)f);
        if (!c) return 1;
        { pl_cell_t v = plc_copy(pl_deref((pl_cell_t *)val)); char probe; if (mode == 2 && pl_tr_needs_log(cx, c, &probe)) pl_tr_push(cx, c); *(pl_cell_t *)c = v; return 1; }
    }
}
