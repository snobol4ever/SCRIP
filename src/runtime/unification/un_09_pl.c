#include "runtime/unification/un_head.h"
#include "ir/ct_arena.h"
#include "runtime/core/core.h"
#include "ir/bb_pool.h"
#include "runtime/rt/prolog_atom.h"
#include "ir/gc_frame_map.h"
#include <sys/mman.h>
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_08_pl.h"
#include "runtime/unification/un_09_pl.h"
extern const char *prolog_atom_name(int id);
static int pl_db_ground(const pl_cell_t *c);
extern void rt_pl_db_reclaimed_entry(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t pl_atom_cell(const char *a) { return pl_make_atom(prolog_atom_intern(a)); }
static int * pl_ix_kn(pl_ix_t *x, int p) { return (int *)(x + 1) + (size_t)p * ((size_t)x->cap + (size_t)x->hcap * (sizeof(pl_ix_ent_t) / sizeof(int))); }
static pl_ix_ent_t * pl_ix_tab(pl_ix_t *x, int p) { return (pl_ix_ent_t *)(pl_ix_kn(x, p) + x->cap); }
static size_t pl_ix_bytes(int cap, int hcap, int np) { return sizeof(pl_ix_t) + (size_t)np * ((size_t)cap * sizeof(int) + (size_t)hcap * sizeof(pl_ix_ent_t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_atomic_key(const pl_cell_t *a) {
    if ((int)a->v == DT_PLATOM) return (int64_t)(((uint64_t)(uint32_t)(int)a->i << 3) | PL_IX_ATOM);
    if ((int)a->v == DT_I && a->i >= -((int64_t)1 << 60) && a->i < ((int64_t)1 << 60)) return (int64_t)(((uint64_t)a->i << 3) | PL_IX_INT);
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_cell_key(const pl_cell_t *c, int deep) {
    pl_cell_t *a = pl_deref((pl_cell_t *)c);
    int64_t k = pl_ix_atomic_key(a), f, k0;
    if (k) return k;
    if ((int)a->v == DT_I) return (int64_t)PL_IX_OTHER;
    if ((int)a->v == DT_PLREF) {
        f = (int64_t)(((uint64_t)a->slen << 3) | PL_IX_FUNC);
        if (!deep || !a->p || plc_fid_arity(a->slen) < 1) return f;
        k0 = pl_ix_atomic_key(pl_deref(&((pl_cell_t *)a->p)[0]));
        return k0 ? (int64_t)((((uint64_t)f ^ ((uint64_t)k0 * 0x9E3779B97F4A7C15ull)) & ~(uint64_t)7) | PL_IX_FUNC) : 0;
    }
    if (pl_cell_unbound(a) || pl_attv_is(a)) return 0;
    return (int64_t)PL_IX_OTHER;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const pl_cell_t * pl_ix_arg(const pl_cell_t *head, int p) {
    pl_cell_t *h = pl_deref((pl_cell_t *)head);
    return ((int)h->v == DT_PLREF && h->p && plc_fid_arity(h->slen) > p) ? &((pl_cell_t *)h->p)[p] : (const pl_cell_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_slot_key(const pl_db_t *db, int i, int p, int deep) {
    pl_cell_t *pair = pl_deref((pl_cell_t *)&db->s[i].cl);
    const pl_cell_t *a0;
    if ((int)pair->v != DT_PLREF || !pair->p) return 0;
    a0 = pl_ix_arg(&((pl_cell_t *)pair->p)[0], p);
    return a0 ? pl_ix_cell_key(a0, deep) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_head_key_at(const pl_db_t *db, const void *head, int p, int deep) {
    pl_cell_t *th = pl_deref((pl_cell_t *)head), *sh, *s0;
    const pl_cell_t *a0;
    if (db->n < 1) return 0;
    s0 = pl_deref((pl_cell_t *)&db->s[0].cl);
    if ((int)s0->v != DT_PLREF || !s0->p) return 0;
    sh = pl_deref(&((pl_cell_t *)s0->p)[0]);
    if ((int)th->v != DT_PLREF || (int)sh->v != DT_PLREF || th->slen != sh->slen) return 0;
    a0 = pl_ix_arg(th, p);
    return a0 ? pl_ix_cell_key(a0, deep) : 0;
}
int64_t pl_ix_head_key(const pl_db_t *db, const void *head) { return db->ix ? pl_ix_head_key_at(db, head, 0, ((pl_ix_t *)db->ix)->deep[0]) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_target_key(const pl_db_t *db, const void *target, int p, int deep) {
    pl_cell_t *tg = pl_deref((pl_cell_t *)target), *s0;
    if (db->n < 1) return 0;
    s0 = pl_deref((pl_cell_t *)&db->s[0].cl);
    if ((int)s0->v != DT_PLREF || !s0->p) return 0;
    if ((int)tg->v == DT_PLREF && tg->p && tg->slen == s0->slen) return pl_ix_head_key_at(db, &((pl_cell_t *)tg->p)[0], p, deep);
    return pl_ix_head_key_at(db, tg, p, deep);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_ix_ent_t * pl_ix_find_at(pl_ix_t *x, int p, int64_t key) {
    pl_ix_ent_t *t = pl_ix_tab(x, p);
    uint64_t m = (uint64_t)x->hcap - 1, h = (uint64_t)key * 0x9E3779B97F4A7C15ull;
    h ^= h >> 29;
    for (uint64_t j = h & m;; j = (j + 1) & m) { if (t[j].key == key) return &t[j]; if (t[j].key == 0) return (pl_ix_ent_t *)0; }
}
pl_ix_ent_t * pl_ix_find(pl_ix_t *x, int64_t key) { return pl_ix_find_at(x, 0, key); }
int pl_ix_step(const pl_db_t *db, int64_t keyed, int i) { return keyed ? pl_ix_kn((pl_ix_t *)db->ix, 0)[i] : i + 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ix_link(pl_ix_t *x, int p, int i, int64_t key, int prepend) {
    pl_ix_ent_t *t = pl_ix_tab(x, p);
    int *kn = pl_ix_kn(x, p);
    uint64_t m = (uint64_t)x->hcap - 1, h = (uint64_t)key * 0x9E3779B97F4A7C15ull, j;
    h ^= h >> 29;
    for (j = h & m; t[j].key != key && t[j].key != 0; j = (j + 1) & m) { }
    if (t[j].key == 0) {
        t[j].key = key;
        t[j].first = i;
        t[j].last = i;
        t[j].n = 1;
        kn[i] = -1;
        x->hused[p]++;
    } else if (prepend) {
        kn[i] = t[j].first;
        t[j].first = i;
        t[j].n++;
    } else {
        kn[t[j].last] = i;
        kn[i] = -1;
        t[j].last = i;
        t[j].n++;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ix_scan(const pl_db_t *db, int p, int *deep) {
    *deep = 1;
    for (int j = db->head_idx; j >= 0; j = db->s[j].next_idx) { if (pl_ix_slot_key(db, j, p, 1)) continue; if (!pl_ix_slot_key(db, j, p, 0)) return 0; *deep = 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_ix_build(pl_db_t *db, int want2, int off2) {
    extern void * rt_heap_alloc_c(size_t);
    int cap = db->cap, hcap = 2 * db->cap, d[2] = {0, 0}, np;
    pl_ix_t *x;
    if (!pl_ix_scan(db, 0, &d[0])) { db->ix = (void *)0; db->ix_off = 1; return; }
    np = (want2 && !off2 && pl_ix_scan(db, 1, &d[1])) ? 2 : 1;
    x = (pl_ix_t *)rt_heap_alloc_c(pl_ix_bytes(cap, hcap, np));
    if (!x) { db->ix = (void *)0; return; }
    memset(x, 0, pl_ix_bytes(cap, hcap, np));
    x->cap = cap;
    x->hcap = hcap;
    x->np = np;
    x->off2 = off2 || (want2 && np == 1);
    x->deep[0] = d[0];
    x->deep[1] = d[1];
    for (int p = 0; p < np; p++) for (int j = db->head_idx; j >= 0; j = db->s[j].next_idx) pl_ix_link(x, p, j, pl_ix_slot_key(db, j, p, d[p]), 0);
    db->ix = (void *)x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_ix_note(pl_db_t *db, int i, int prepend) {
    pl_ix_t *x = (pl_ix_t *)db->ix;
    int64_t k[2] = {0, 0};
    if (db->ix_off) return;
    if (!pl_ix_slot_key(db, i, 0, 0)) { db->ix = (void *)0; db->ix_off = 1; return; }
    if (!x) { if (db->n >= PL_IX_MIN) pl_ix_build(db, 0, 0); return; }
    if (i >= x->cap || (x->hused[0] + 1) * 2 > x->hcap || (x->hused[1] + 1) * 2 > x->hcap) { pl_ix_build(db, x->np == 2, x->off2); return; }
    for (int p = 0; p < x->np; p++) if (!(k[p] = pl_ix_slot_key(db, i, p, x->deep[p]))) { pl_ix_build(db, x->np == 2, x->off2); return; }
    for (int p = 0; p < x->np; p++) pl_ix_link(x, p, i, k[p], prepend);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_ix_pick(pl_db_t *db, const void *target, int *pp) {
    pl_ix_t *x = (pl_ix_t *)db->ix;
    int64_t k;
    *pp = 0;
    if (!x || !target) return 0;
    if ((k = pl_ix_target_key(db, target, 0, x->deep[0])) != 0) {
        pl_ix_ent_t *e0 = pl_ix_find_at(x, 0, k);
        int n0 = e0 ? e0->n : 0;
        if (n0 < PL_IX_MIN || x->off2 || db->n < PL_IX_MIN || !pl_ix_target_key(db, target, 1, 0)) return k;
        if (x->np < 2) pl_ix_build(db, 1, 0);
        x = (pl_ix_t *)db->ix;
        if (!x || x->np < 2) return k;
        int64_t k1 = pl_ix_target_key(db, target, 1, x->deep[1]);
        pl_ix_ent_t *e1 = k1 ? pl_ix_find_at(x, 1, k1) : (pl_ix_ent_t *)0;
        if (!k1 || (e1 && e1->n >= n0)) return k;
        *pp = 1;
        return k1;
    }
    if (x->np < 2 && (x->off2 || db->n < PL_IX_MIN || !pl_ix_target_key(db, target, 1, 0))) return 0;
    if (x->np < 2) pl_ix_build(db, 1, 0);
    x = (pl_ix_t *)db->ix;
    if (!x || x->np < 2) return 0;
    *pp = 1;
    return pl_ix_target_key(db, target, 1, x->deep[1]);
}
static pl_cell_t * pl_db_target_head(const pl_db_t *db, void *target) {
    pl_cell_t *tg = target ? pl_deref((pl_cell_t *)target) : (pl_cell_t *)0, *s0;
    if (!tg || db->n < 1) return (pl_cell_t *)0;
    s0 = pl_deref((pl_cell_t *)&db->s[0].cl);
    if ((int)s0->v != DT_PLREF || !s0->p) return (pl_cell_t *)0;
    if ((int)tg->v == DT_PLREF && tg->p && tg->slen == s0->slen) tg = pl_deref(&((pl_cell_t *)tg->p)[0]);
    return ((int)tg->v == DT_PLREF && tg->p) ? tg : (pl_cell_t *)0;
}
static int pl_db_cand_rejects(const pl_db_t *db, int i, uint32_t fid, const int *bj, const pl_cell_t *bv, int nb) {
    pl_cell_t *pair = pl_deref((pl_cell_t *)&db->s[i].cl), *sh;
    if ((int)pair->v != DT_PLREF || !pair->p) return 0;
    sh = pl_deref(&((pl_cell_t *)pair->p)[0]);
    if ((int)sh->v != DT_PLREF || sh->slen != fid || !sh->p) return 0;
    for (int q = 0; q < nb; q++) {
        pl_cell_t *a = pl_deref(&((pl_cell_t *)sh->p)[bj[q]]);
        int at = (int)a->v;
        if ((at == DT_I || at == DT_PLATOM) && (at != (int)bv[q].v || a->i != bv[q].i)) return 1;
    }
    return 0;
}
static int pl_db_target_bound(const pl_cell_t *th, int *bj, pl_cell_t *bv) {
    int nb = 0;
    for (int j = 0, n = th ? plc_fid_arity(th->slen) : 0; j < n; j++) {
        pl_cell_t *t = pl_deref(&((pl_cell_t *)th->p)[j]);
        if ((int)t->v == DT_I || (int)t->v == DT_PLATOM) { bj[nb] = j; bv[nb] = *t; nb++; }
    }
    return nb;
}
static int pl_db_cand_step(const pl_db_t *db, int i, int64_t k, int p) {
    pl_ix_t *x = (pl_ix_t *)db->ix;
    if (!x || !k || p >= x->np) return db->s[i].next_idx;
    if (i < x->cap && pl_ix_slot_key(db, i, p, x->deep[p]) == k) return pl_ix_kn(x, p)[i];
    for (int j = db->s[i].next_idx; j >= 0; j = db->s[j].next_idx) if (pl_ix_slot_key(db, j, p, x->deep[p]) == k) return j;
    return -1;
}
int rt_pl_db_cand_first(void *db_v, void *target) {
    pl_db_t *db = (pl_db_t *)db_v;
    pl_ix_ent_t *e;
    pl_cell_t *th;
    int64_t k;
    int j, p;
    if (!db) return -1;
    k = pl_ix_pick(db, target, &p);
    if (!k) j = db->head_idx;
    else { e = pl_ix_find_at((pl_ix_t *)db->ix, p, k); j = e ? e->first : -1; }
    th = pl_db_target_head(db, target);
    {
        int na = th ? plc_fid_arity(th->slen) : 0, bj[na > 0 ? na : 1], nb;
        pl_cell_t bv[na > 0 ? na : 1];
        nb = pl_db_target_bound(th, bj, bv);
        while (j >= 0 && nb && pl_db_cand_rejects(db, j, th->slen, bj, bv, nb)) j = pl_db_cand_step(db, j, k, p);
    }
    return j;
}
int rt_pl_db_cand_next(void *db_v, int i, void *target) {
    pl_db_t *db = (pl_db_t *)db_v;
    pl_cell_t *th;
    int64_t k;
    int j, p;
    if (!db || i < 0 || i >= db->n) return -1;
    k = pl_ix_pick(db, target, &p);
    th = pl_db_target_head(db, target);
    {
        int na = th ? plc_fid_arity(th->slen) : 0, bj[na > 0 ? na : 1], nb;
        pl_cell_t bv[na > 0 ? na : 1];
        nb = pl_db_target_bound(th, bj, bv);
        for (j = pl_db_cand_step(db, i, k, p); j >= 0 && nb && pl_db_cand_rejects(db, j, th->slen, bj, bv, nb); j = pl_db_cand_step(db, j, k, p)) ;
    }
    return j;
}
int rt_pl_db_cand_last(void *db_v, int i, void *target, int g) {
    pl_db_t *db = (pl_db_t *)db_v;
    int p;
    if (!db || i < 0 || i >= db->n) return 1;
    int64_t k = pl_ix_pick(db, target, &p);
    pl_cell_t *th = pl_db_target_head(db, target);
    int na = th ? plc_fid_arity(th->slen) : 0, bj[na > 0 ? na : 1], nb;
    pl_cell_t bv[na > 0 ? na : 1];
    nb = pl_db_target_bound(th, bj, bv);
    for (int j = pl_db_cand_step(db, i, k, p); j >= 0; j = pl_db_cand_step(db, j, k, p)) {
        if (db->s[j].ref >= g || (db->s[j].erased != 0 && db->s[j].erased < g)) continue;
        if (!nb || !pl_db_cand_rejects(db, j, th->slen, bj, bv, nb)) return 0;
    }
    return 1;
}
int rt_pl_db_data_unify(void *db_v, int i, void *target, pl_tr_ctx_t *cx) {
    pl_db_t *db = (pl_db_t *)db_v;
    pl_cell_t *pair, *sh, *th, *sa, *ta;
    if (!db || i < 0 || i >= db->n) return 0;
    pair = pl_deref(&db->s[i].cl);
    if ((int)pair->v != DT_PLREF || !pair->p) return 0;
    sh = pl_deref(&((pl_cell_t *)pair->p)[0]);
    th = pl_deref((pl_cell_t *)target);
    if ((int)th->v == DT_PLREF && th->p && th->slen == pair->slen) th = pl_deref(&((pl_cell_t *)th->p)[0]);
    if ((int)sh->v != DT_PLREF || (int)th->v != DT_PLREF || th->slen != sh->slen || !sh->p || !th->p) return 0;
    if (!pl_db_ground(sh)) { pl_cell_t w = plc_copy_cx(sh, 0, cx); return plc_unify_cells_cx(th, &w, cx) ? 1 : 0; }
    sa = (pl_cell_t *)sh->p;
    ta = (pl_cell_t *)th->p;
    for (int j = 0, n = plc_fid_arity(sh->slen); j < n; j++) {
        pl_cell_t *sv = pl_deref(&sa[j]);
        pl_cell_t w = ((int)sv->v == DT_PLREF) ? plc_copy(sv) : *sv;
        if (!plc_unify_cells_cx(&ta[j], &w, cx)) return 0;
    }
    return 1;
}
static int pl_db_frag_pack(size_t m0, size_t m1) {
    long ps = bb_pool_page();
    size_t lo, hi;
    if (ps <= 0) return 0;
    lo = (m0 + (size_t)ps - 1) / (size_t)ps;
    hi = (m1 + (size_t)ps - 1) / (size_t)ps;
    if (hi <= lo || lo >= ((size_t)1 << 20) || hi - lo >= ((size_t)1 << 10)) return 0;
    return (int)(lo | ((hi - lo) << 20));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_frag_lo(int f) { const uint8_t *b = bb_pool_base(); return (PL_DB_FRAG_REC(f) && b) ? (const char *)b + (size_t)(f & 0xFFFFF) * (size_t)bb_pool_page() : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_db_frag_hi(int f) { const char *lo = pl_db_frag_lo(f); return lo ? lo + (size_t)PL_DB_FRAG_PAGES(f) * (size_t)bb_pool_page() : (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_frag_record(pl_db_t *db, int i, const void *fn) {
    const uint8_t *b = bb_pool_base(), *lo = bb_pool_last_lo(), *hi = bb_pool_last_hi();
    int f = (b && fn && (const uint8_t *)fn == lo && hi > lo) ? pl_db_frag_pack((size_t)(lo - b), (size_t)(hi - b)) : 0;
    db->s[i].frag = f;
    if (!f) { db->frag_top = -2; return; }
    if (db->frag_top != -2) db->frag_top = f & 0xFFFFF;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_ground(const pl_cell_t *c) {
    pl_cell_t *d = pl_deref((pl_cell_t *)c);
    for (;;) {
        int n;
        if (pl_cell_unbound(d) || pl_attv_is(d)) return 0;
        if ((int)d->v != DT_PLREF || !d->p) return 1;
        n = plc_fid_arity(d->slen);
        if (n < 1) return 1;
        for (int j = 0; j < n - 1; j++) if (!pl_db_ground(&((pl_cell_t *)d->p)[j])) return 0;
        d = pl_deref(&((pl_cell_t *)d->p)[n - 1]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_no_attv(const pl_cell_t *c) {
    pl_cell_t *d = pl_deref((pl_cell_t *)c);
    for (;;) {
        int n;
        if (pl_attv_is(d)) return 0;
        if ((int)d->v != DT_PLREF || !d->p) return 1;
        n = plc_fid_arity(d->slen);
        if (n < 1) return 1;
        for (int j = 0; j < n - 1; j++) if (!pl_db_no_attv(&((pl_cell_t *)d->p)[j])) return 0;
        d = pl_deref(&((pl_cell_t *)d->p)[n - 1]);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_data_fact(const pl_db_t *db, int i) {
    pl_cell_t *pair = pl_deref((pl_cell_t *)&db->s[i].cl), *body, *head;
    if ((int)pair->v != DT_PLREF || !pair->p) return 0;
    body = pl_deref(&((pl_cell_t *)pair->p)[1]);
    head = pl_deref(&((pl_cell_t *)pair->p)[0]);
    if ((int)body->v != DT_PLATOM || (int)body->i != g_runtime.atoms.atom_true || (int)head->v != DT_PLREF || plc_fid_arity(head->slen) < 1) return 0;
    return pl_db_no_attv(head);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_define_data(void *root, const char *nm, int arity, const char *skey) {
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * pl_runtime_define_pred(const char *, const void *, int, void *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *hargs = (pl_cell_t *)PL_CELL_ALLOC((size_t)arity * sizeof(pl_cell_t));
    pl_cell_t head, th[1], body, kids[2], pair;
    void *raw, *cl, *ch;
    if (!hargs) return 0;
    pl_tr_ctx_t rcx = { 0 };
    rcx.r14 = (uint64_t)(uintptr_t)root;
    for (int i = 0; i < arity; i++) pl_init_var(&hargs[i], &rcx);
    head = pl_make_compound(prolog_atom_intern(nm), arity, hargs);
    th[0] = head;
    body = pl_make_compound(prolog_atom_intern("$db_data"), 1, (void *)th);
    kids[0] = head;
    kids[1] = body;
    pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
    raw = rt_pl_clause_tree((void *)&pair);
    cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
    if (!cl) return 0;
    ch = rt_pl_choice_new(skey);
    rt_pl_choice_add(ch, cl);
    return pl_runtime_define_pred(skey, ch, arity, root) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_materialize(void *root, pl_db_t *db) {
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern void * pl_runtime_define_static(const char *, const char *, const void *, int, void *);
    int ar = 0, first = -1, facts = 1;
    const char *nm;
    if (db->whole != 1 && db->whole != 3) return db->whole == 2;
    for (int j = db->head_idx; j >= 0; j = db->s[j].next_idx) if (!db->s[j].erased) { if (first < 0) first = j; if (!pl_db_data_fact(db, j)) facts = 0; }
    if (first < 0) return 0;
    nm = pl_db_head_name((void *)&db->s[first].cl, &ar);
    if (!nm) { pl_db_dynamize(root, db); return 0; }
    if (facts) { db->data_off = 0; db->data_on = 0; if (pl_db_data_enter(root, db, first)) { db->data_on = 1; db->whole = 2; return 1; } }
    {
        PL_DB_KEY_VLA(key, nm, ar);
        char wkey[fmt_len("%s@whole", key)];
        void *ch;
        snprintf(wkey, sizeof wkey, "%s@whole", key);
        ch = rt_pl_choice_new(wkey);
        for (int j = db->head_idx; j >= 0; j = db->s[j].next_idx) {
            void *raw, *cl;
            if (db->s[j].erased) continue;
            raw = rt_pl_clause_tree((void *)&db->s[j].cl);
            cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
            if (!cl) { pl_db_dynamize(root, db); return 0; }
            rt_pl_choice_add(ch, cl);
        }
        if (pl_runtime_define_static(key, wkey, ch, ar, root)) { db->data_on = 0; db->data_off = 1; db->whole = 2; return 1; }
        pl_db_dynamize(root, db);
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_consult(void *root, void *db_v, void *clause_term, int declared, int pend) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db) return 0;
    if (db->whole == 2) pl_db_dynamize(root, db);
    if (db->whole == 0 && (declared || db->n > 0 || db->data_on || db->data_off)) return pl_db_store(root, db_v, clause_term, 0, 1, 1);
    if (!pl_db_store(root, db_v, clause_term, 0, 0, 0)) return 0;
    if (!db->whole) db->whole = pend;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_materialize_key(void *root, const char *key) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = r ? pl_db_reg_find(r, key) : (pl_db_key_t *)0;
    pl_db_t *db;
    if (!e) return 0;
    db = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db;
    return (db && (db->whole == 1 || db->whole == 3)) ? pl_db_materialize(root, db) : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_materialize_all(void *root, int which) {
    extern void pl_runtime_register_key(const char *, int);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    int n = 0;
    if (!r) return 0;
    for (int i = 0; i < r->n; i++) {
        pl_db_t *db = (r->e[i].k >= 0) ? pl_db_cell_peek(root, r->e[i].k) : r->e[i].db;
        const char *sl = strrchr(r->e[i].key, '/');
        if (db && db->whole == which && sl) pl_runtime_register_key(r->e[i].key, atoi(sl + 1));
    }
    for (int i = 0; i < r->n; i++) { pl_db_t *db = (r->e[i].k >= 0) ? pl_db_cell_peek(root, r->e[i].k) : r->e[i].db; if (db && db->whole == which && pl_db_materialize(root, db)) n++; }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rr_mark(pl_db_rr_t *r, const char *p, unsigned char bit) {
    const char *b = (const char *)bb_pool_base();
    if (!p || !b || p < b || p >= b + bb_pool_used()) return;
    for (int i = 0; i < r->db->n; i++) { int f = r->db->s[i].frag; if (f && p >= pl_db_frag_lo(f) && p < pl_db_frag_hi(f)) { r->mk[i] |= bit; return; } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rr_frame(const void *map, const char *base, void *a) {
    pl_db_rr_t *r = (pl_db_rr_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    r->frames++;
    pl_db_rr_mark(r, (const char *)map, 1);
    if (!m || !(m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT))) return;
    {
        const uint64_t *t = (const uint64_t *)(m + 1);
        for (long i = 0; i < (long)t[0]; i++) {
            uint64_t q = t[1 + i];
            if (GC_LAY_KIND(q) != GC_LAY_PTR_CODE) continue;
            for (int o = 0; o + 8 <= GC_LAY_SIZE(q); o += 8) pl_db_rr_mark(r, *(const char * const *)(base + GC_LAY_OFF(q) + o), 1);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_reclaim_report(pl_db_t *db) {
    extern char *rt_gc_program_stack_top(void);
    const char *e = getenv("SCRIP_PL_RECLAIM_REPORT");
    long every = e ? strtol(e, (char **)0, 10) : 0;
    if (every <= 0 || !db || db->n <= 0 || db->erased_n % every) return;
    {
        unsigned char mk[db->n];
        pl_db_rr_t r = { db, mk, 0 };
        int ar = 0, live = 0, recount = 0, unrec = 0, vis = 0, raw = 0, only = 0, ok, comp = 0;
        long pages = 0, gone = 0;
        const char *nm = (const char *)0, *top = rt_gc_program_stack_top();
        char probe = 0;
        memset(mk, 0, sizeof mk);
        ok = rt_gc_frames_visit(pl_db_rr_frame, (void *)&r);
        for (const char *w = (const char *)(((uintptr_t)&probe + 7) & ~(uintptr_t)7); top && w + 8 <= top; w += 8) pl_db_rr_mark(&r, *(const char * const *)w, 2);
        for (int i = 0; i < db->n; i++) {
            if (!nm) nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
            if (db->s[i].ridx >= 0) comp++;
            if (!db->s[i].erased) { live++; continue; }
            recount++;
            if (!PL_DB_FRAG_REC(db->s[i].frag)) unrec++;
            else if (db->s[i].frag & PL_DB_FRAG_GONE) gone += PL_DB_FRAG_PAGES(db->s[i].frag);
            else pages += PL_DB_FRAG_PAGES(db->s[i].frag);
            if (mk[i] & 1) vis++;
            if (mk[i] & 2) raw++;
            if ((mk[i] & 2) && !(mk[i] & 1)) only++;
        }
        fprintf(stderr,
            "[PL-RECLAIM] key=%s/%d slots=%d live=%d erased=%d recount=%d erased_pages=%ld unrecorded=%d visitor=%s frames=%ld visitor_live=%d raw_live=%d raw_only=%d compiled=%d pool_used=%zu slot_"
            "bytes=%ld" " reclaimed=%d reclaimed_pages=%ld\n", nm ? nm : "?", ar, db->n, live, db->erased_n, recount, pages, unrec, ok ? "whole" : "refused", r.frames, vis, raw, only, comp,
            bb_pool_used(), (long)db->cap * (long)sizeof(pl_db_slot_t), db->recl_n, gone);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_rcix_cmp(const void *a, const void *b) { const char *x = ((const pl_db_rcix_t *)a)->lo, *y = ((const pl_db_rcix_t *)b)->lo; return (x > y) - (x < y); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_rc_index(pl_db_rc_t *r) {
    extern void *rt_heap_alloc_c(size_t);
    const pl_db_t *db = r->db;
    int n = 0, k = 0, asc = 1;
    pl_db_rcix_t *ix;
    for (int i = 0; i < db->n; i++) if (PL_DB_FRAG_REC(db->s[i].frag) && !(db->s[i].frag & PL_DB_FRAG_GONE)) n++;
    ix = (pl_db_rcix_t *)rt_heap_alloc_c((size_t)(n > 0 ? n : 1) * sizeof(pl_db_rcix_t));
    if (!ix) return 0;
    for (int i = 0; i < db->n; i++) {
        if (!PL_DB_FRAG_REC(db->s[i].frag) || (db->s[i].frag & PL_DB_FRAG_GONE)) continue;
        ix[k].lo = pl_db_frag_lo(db->s[i].frag);
        ix[k].slot = i;
        if (k && ix[k - 1].lo > ix[k].lo) asc = 0;
        k++;
    }
    if (!asc) qsort(ix, (size_t)n, sizeof(pl_db_rcix_t), pl_db_rcix_cmp);
    r->ix = ix;
    r->nix = n;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_frag_find(const pl_db_rc_t *r, const char *p) {
    int lo = 0, hi = r->nix - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2, m = r->ix[mid].slot;
        if (p < pl_db_frag_lo(r->db->s[m].frag)) hi = mid - 1;
        else if (p >= pl_db_frag_hi(r->db->s[m].frag)) lo = mid + 1;
        else return m;
    }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rc_hold(pl_db_rc_t *r, const char *p) {
    int j;
    if (!p || p < r->lo || p >= r->hi) return;
    if (bb_pool_in_hole(p)) rt_bomb("the assertz reclaim: a frame holds a reclaimed run-time Prolog fragment -- a pass reclaimed code a live activation still reaches (unification.c pl_db_rc_hold)");
    if ((j = pl_db_frag_find(r, p)) < 0) return;
    if (r->db->s[j].ref + 1 < r->min_g) r->min_g = r->db->s[j].ref + 1;
    if (j >= r->c_lo && j <= r->c_hi) r->db->s[j].frag |= PL_DB_FRAG_HELD;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_rc_frame(const void *map, const char *base, void *a) {
    pl_db_rc_t *r = (pl_db_rc_t *)a;
    const gc_frame_map_t *m = (const gc_frame_map_t *)map;
    pl_db_rc_hold(r, (const char *)map);
    if (!m || !(m->flags & (GC_FRAME_MAP_BLOB | GC_FRAME_MAP_LAYOUT))) return;
    {
        const uint64_t *t = (const uint64_t *)(m + 1);
        for (long i = 0; i < (long)t[0]; i++) {
            uint64_t q = t[1 + i];
            if (GC_LAY_KIND(q) != GC_LAY_PTR_CODE) continue;
            for (int o = 0; o + 8 <= GC_LAY_SIZE(q); o += 8) pl_db_rc_hold(r, *(const char * const *)(base + GC_LAY_OFF(q) + o));
        }
    }
}
static void pl_db_rc_seg(const char *lo, const char *hi, void *a) {
    pl_db_rc_t *r = (pl_db_rc_t *)a;
    for (const char *w = (const char *)(((uintptr_t)lo + 7) & ~(uintptr_t)7); w + 8 <= hi; w += 8) {
        uint64_t x = *(const uint64_t *)w;
        const char *p = (const char *)(uintptr_t)(x >> 8);
        int j;
        if ((x & 255u) != (uint64_t)DT_RAW || p < r->lo || p >= r->hi || (j = pl_db_frag_find(r, p)) < 0) continue;
        pl_db_rc_hold(r, p);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_reclaim_one(pl_db_t *db, int i) {
    extern void rt_gc_frame_maps_drop_range(const void *, const void *);
    extern void rt_gc_frame_sites_drop_range(const void *, const void *);
    extern void rt_proc_set_fn_at(int, bb_box_fn);
    extern void rt_proc_set_dcfn(const char *, void *);
    extern void bb_ab_seal_alpha(const char *, void *);
    int f = db->s[i].frag, ar = 0;
    char *lo = (char *)pl_db_frag_lo(f), *hi = (char *)pl_db_frag_hi(f);
    const char *nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!lo || !nm || db->s[i].ridx < 0 || mprotect(lo, (size_t)(hi - lo), PROT_NONE) != 0) return 0;
    madvise(lo, (size_t)(hi - lo), MADV_DONTNEED);
    rt_gc_frame_maps_drop_range(lo, hi);
    rt_gc_frame_sites_drop_range(lo, hi);
    {
        PL_DB_KEY_VLA(key, nm, ar);
        char fkey[fmt_len("%s@%d", key, i)];
        snprintf(fkey, sizeof fkey, "%s@%d", key, i);
        rt_proc_set_fn_at(db->s[i].ridx, (bb_box_fn)rt_pl_db_reclaimed_entry);
        rt_proc_set_dcfn(fkey, (void *)rt_pl_db_reclaimed_entry);
        bb_ab_seal_alpha(fkey, (void *)rt_pl_db_reclaimed_entry);
    }
    db->s[i].chain_off = 0;
    db->s[i].frag = (f & ~PL_DB_FRAG_HELD) | PL_DB_FRAG_GONE;
    db->recl_n++;
    bb_pool_reclaim(lo, hi);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_reclaim_cand(const pl_db_t *db, int i) { int f = db->s[i].frag; return db->s[i].erased && PL_DB_FRAG_REC(f) && !(f & PL_DB_FRAG_GONE) && i != db->head_idx && i != db->entry_idx; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_reclaim(pl_db_t *db) {
    extern long rt_gc_runs_count(void);
    const char *b = (const char *)bb_pool_base();
    int runs, lo = -1, hi = -1, next_lo;
    if (!db || db->frag_top < 0 || !b || db->recl_since <= 0 || db->tx_pins > 0) return;
    runs = (int)rt_gc_runs_count();
    if (runs == db->recl_gc && db->recl_since < 1024) return;
    db->recl_gc = runs;
    db->recl_since = 0;
    for (int i = db->recl_lo; i < db->n; i++) if (pl_db_reclaim_cand(db, i)) { if (lo < 0) lo = i; hi = i; }
    if (lo < 0) { db->recl_lo = db->n; return; }
    {
        pl_db_rc_t r = { db, b, b + bb_pool_used(), INT_MAX, lo, hi, (const pl_db_rcix_t *)0, 0 };
        int ok = pl_db_rc_index(&r);
        if (ok) ok = rt_gc_frames_visit(pl_db_rc_frame, (void *)&r);
        if (ok) rt_gc_segments_visit(pl_db_rc_seg, (void *)&r);
        next_lo = db->n;
        for (int i = lo; i <= hi; i++) {
            int held = db->s[i].frag & PL_DB_FRAG_HELD;
            db->s[i].frag &= ~PL_DB_FRAG_HELD;
            if (!pl_db_reclaim_cand(db, i)) continue;
            if (!ok || held || db->s[i].erased >= r.min_g || !pl_db_reclaim_one(db, i)) { if (i < next_lo) next_lo = i; }
        }
        db->recl_lo = next_lo;
    }
}
int rt_pl_mod_ext(void *root, void *mc, void *gc, void *ec, void *out, pl_tr_ctx_t *cx) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    extern int rt_proc_is_registered(const char *);
    pl_cell_t *m = pl_deref((pl_cell_t *)mc), *g = pl_deref((pl_cell_t *)gc), *e = pl_deref((pl_cell_t *)ec), r = *g;
    if ((int)m->v == DT_PLATOM && (int)e->v == DT_I && ((int)g->v == DT_PLATOM || ((int)g->v == DT_PLREF && g->p))) {
        const char *mn = prolog_atom_name((int)m->i);
        int ar = (int)g->v == DT_PLREF ? plc_fid_arity(g->slen) : 0;
        const char *gn = (int)g->v == DT_PLREF ? prolog_atom_name(plc_fid_name(g->slen)) : prolog_atom_name((int)g->i);
        if (mn && gn && strcmp(mn, "user")) {
            char key[strlen(mn) + strlen(gn) + 24];
            snprintf(key, sizeof key, "%s:%s/%d", mn, gn, ar + (int)e->i);
            if (rt_proc_is_registered(key) || rt_pl_db_key_cell(root, key) >= 0) {
                char qn[strlen(mn) + strlen(gn) + 2];
                int qid;
                snprintf(qn, sizeof qn, "%s:%s", mn, gn);
                qid = prolog_atom_intern(qn);
                r = (int)g->v == DT_PLREF ? pl_make_compound(qid, ar, g->p) : pl_make_atom(qid);
            }
        }
    }
    return plc_unify_cells_cx((pl_cell_t *)out, &r, cx) ? 1 : 0;
}
int rt_pl_tx_db(void *root, int op, void *recs) {
    extern int prolog_atom_intern(const char *);
    int txdb = prolog_atom_intern("$txdb");
    pl_cell_t *l = pl_deref((pl_cell_t *)recs);
    while ((int)l->v == DT_PLREF && l->p && plc_fid_arity(l->slen) == 2 && plc_fid_name(l->slen) == g_runtime.atoms.atom_dot) {
        pl_cell_t *cons = (pl_cell_t *)l->p, *d = pl_deref(&cons[0]);
        if ((int)d->v == DT_PLREF && d->p && plc_fid_arity(d->slen) == 5 && plc_fid_name(d->slen) == txdb) {
            pl_cell_t *a = (pl_cell_t *)d->p, *k = pl_deref(&a[0]), *c = pl_deref(&a[1]), *s = pl_deref(&a[2]);
            pl_db_t *db = ((int)k->v == DT_I && (int)c->v == DT_I && (int)s->v == DT_I) ? (pl_db_t *)rt_pl_db_get(root, c->i) : (pl_db_t *)0;
            int i = (int)s->i;
            if (db && i >= 0 && i < db->n) {
                if (k->i == 1) {
                    if (db->tx_pins > 0) db->tx_pins--;
                    if (op == 0 && db->s[i].erased) { db->s[i].erased = 0; if (db->erased_n > 0) db->erased_n--; pl_db_note_live(root, db, i); }
                } else if (op == 0 && !db->s[i].erased) pl_db_note_erase(root, db, i, db->next_ref++);
            }
        }
        l = pl_deref(&cons[1]);
    }
    return 1;
}
