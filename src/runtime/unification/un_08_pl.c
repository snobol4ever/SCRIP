#include "runtime/unification/un_head.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_07_pl.h"
#include "runtime/unification/un_08_pl.h"
#include "runtime/unification/un_09_pl.h"
extern const char *prolog_atom_name(int id);
static int pl_db_kmod_get(void *root, int64_t k);
static int pl_db_src_grow(pl_db_t *db, int n, int cap);
extern pl_db_key_t * rt_pl_db_reg_find_na(pl_db_reg_t *r, const char *nm, int arity);
int rt_pl_db_key_is_dynamic(void *root, const char *key);
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind);
int rt_pl_db_cells_base(void *root, int64_t n);
static void pl_db_tx_note(void *root, pl_db_t *db, int kind, int slot);
void rt_pl_root_omega(void) { extern void *rt_pl_ball_take(void); void *ball = rt_pl_ball_take(); if (!ball) exit(0); rt_pl_ball_report(ball); exit(2); }
void * rt_pl_db_get(void *root, int64_t k) {
    if (!root || k < 0) return (void *)0;
    {
        pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return (void *)0;
        if (!*cell) {
            pl_db_t *d = (pl_db_t *)rt_pl_struct_alloc(HB_PLDB, sizeof *d);
            if (!d) return (void *)0;
            d->cap = 8;
            d->n = 0;
            d->killed = 2;
            d->next_ref = 1;
            d->head_idx = -1;
            d->tail_idx = -1;
            d->cell_k = (int)k;
            d->erased_n = 0;
            d->recl_n = 0;
            d->recl_lo = 0;
            d->recl_gc = -1;
            d->recl_since = 0;
            d->frag_top = -1;
            d->tx_pins = 0;
            d->ix = (void *)0;
            d->ix_off = 0;
            d->data_on = 0;
            d->data_off = 0;
            d->entry_idx = -1;
            d->mod = pl_db_kmod_get(root, k);
            d->s = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)d->cap * sizeof(pl_db_slot_t));
            d->src = (int *)0;
            if (!d->s || !pl_db_src_grow(d, 0, d->cap)) { d->cap = 0; }
            *cell = d;
        }
        return (void *)*cell;
    }
}
int plc_gvar_absent(const pl_cell_t *b) { return b && (int)b->v == DT_I && b->slen == PLC_GVAR_ABSENT; }
int rt_pl_nb_set(void *root, int64_t k, void *val) {
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return 0;
        pl_cell_t *t = pl_deref((pl_cell_t *)val);
        pl_cell_t stored = plc_copy_x(t, 1);
        pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(2);
        if (!box) return 0;
        box[1].v = (DTYPE_t)DT_RAW;
        *box = stored;
        *cell = box;
        if ((int)stored.v == DT_PLREF || (int)stored.v == DT_PLVAR) rt_pl_tr_freeze();
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_b_set(void *root, int64_t k, void *val, pl_tr_ctx_t *cx) {
    extern void *rt_ws_alloc_descr(size_t);
    if (!root || k < 0 || !val || !cx) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 1);
        if (!cell) return 0;
        pl_cell_t *j = (pl_cell_t *)rt_ws_alloc_descr(1);
        if (!j) return 0;
        pl_init_var(j, cx);
        if (!plc_unify_cells_cx(j, (pl_cell_t *)val, cx)) return 0;
        pl_cell_t stored = pl_make_ref(j, 0);
        if (*cell) {
            pl_cell_t *box = *cell;
            char probe;
            char *floor_ = &probe;
            if (pl_tr_needs_log(cx, box, floor_)) {
                int64_t mark = (int64_t)(intptr_t) *(const char * const *) cx->b;
                if (box[1].i != mark) { pl_tr_push_keep(cx, box); pl_tr_push_keep(cx, &box[1]); box[1].i = mark; }
            }
            *box = stored;
        } else {
            pl_cell_t *box = (pl_cell_t *)rt_ws_alloc_descr(2);
            if (!box) return 0;
            box[1].v = (DTYPE_t)DT_RAW;
            *box = pl_make_int(0);
            box->slen = PLC_GVAR_ABSENT;
            *cell = box;
            if (cx->b) pl_tr_push_keep(cx, box);
            *box = stored;
        }
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_get_cell(void *root, int64_t k, void *out_cell, pl_tr_ctx_t *cx) {
    if (!root || k < 0 || !out_cell) return 0;
    {
        pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0);
        pl_cell_t *box = cell ? *cell : (pl_cell_t *)0;
        if (!box || plc_gvar_absent(box)) return 0;
        return plc_unify_cells_cx((pl_cell_t *)out_cell, box, cx) ? 1 : 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_nb_is_set(void *root, int64_t k) {
    if (!root || k < 0) return 0;
    { pl_cell_t **cell = (pl_cell_t **)pl_db_cell_addr(root, k, 0); return cell && *cell != (pl_cell_t *)0 && !plc_gvar_absent(*cell); }
}
void rt_pl_nb_unset(void *root, int64_t k) { pl_cell_t **cell = root && k >= 0 ? (pl_cell_t **)pl_db_cell_addr(root, k, 0) : (pl_cell_t **)0; if (cell) *cell = (pl_cell_t *)0; }
pl_db_reg_t * pl_db_registry(void *root, int create) {
    if (!root) return (pl_db_reg_t *)0;
    {
        pl_db_reg_t **cell = (pl_db_reg_t **)((char *)root - PL_DB_CELL0 - 8 * (size_t)PL_DB_REGISTRY_CELL);
        if (!*cell && create) {
            pl_db_reg_t *r = (pl_db_reg_t *)rt_pl_struct_alloc(HB_PLDBR, sizeof *r);
            if (!r) return (pl_db_reg_t *)0;
            r->cap = 32;
            r->n = 0;
            r->next_cell = 1;
            r->ovf_cap = 0;
            r->ovf = (void **)0;
            r->tries = (void **)0;
            r->ntries = 0;
            r->tcap = 0;
            r->tbl_vt = -1;
            r->hix = (int *)0;
            r->hcap = 0;
            r->mtok = (long *)0;
            r->mtn = 0;
            r->mtcap = 0;
            r->e = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)r->cap * sizeof(pl_db_key_t));
            if (!r->e) { r->cap = 0; }
            *cell = r;
        }
        return *cell;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void ** pl_db_cell_addr(void *root, int64_t k, int create) {
    extern void *rt_pvec_realloc(void *, size_t);
    if (!root || k < 0) return (void **)0;
    if (k < PL_DB_FRAME_CELLS) return (void **)((char *)root - PL_DB_CELL0 - 8 * (size_t)k);
    {
        pl_db_reg_t *r = pl_db_registry(root, create);
        int64_t i = k - PL_DB_FRAME_CELLS;
        if (!r) return (void **)0;
        if (i >= r->ovf_cap) {
            int64_t nc = r->ovf_cap > 0 ? r->ovf_cap : 16;
            void **v;
            if (!create) return (void **)0;
            while (nc <= i) nc *= 2;
            v = (void **)rt_pvec_realloc(r->ovf, (size_t)nc);
            if (!v) return (void **)0;
            r->ovf = v;
            r->ovf_cap = (int)nc;
        }
        return &r->ovf[i];
    }
}
static uint64_t pl_db_hash_bytes(uint64_t h, const char *s, size_t n) { for (size_t i = 0; i < n; i++) h = (h ^ (unsigned char)s[i]) * 0x100000001B3ull; return h; }
static void pl_db_hix_put(pl_db_reg_t *r, int idx) {
    const char *k = r->e[idx].key;
    int m = r->hcap - 1, i = (int)(pl_db_hash_bytes(0xCBF29CE484222325ull, k, strlen(k)) & (uint64_t)m);
    while (r->hix[i]) i = (i + 1) & m;
    r->hix[i] = idx + 1;
}
static void pl_db_hix_index(pl_db_reg_t *r) {
    if (r->hix && 2 * r->n <= r->hcap) { pl_db_hix_put(r, r->n - 1); return; }
    {
        int nc = r->hcap > 0 ? 2 * r->hcap : 64;
        int *nh = (int *)rt_str_alloc((long)nc * (long)sizeof(int));
        r->hix = nh;
        r->hcap = nh ? nc : 0;
        if (!nh) return;
        memset(nh, 0, (size_t)nc * sizeof(int));
        for (int i = 0; i < r->n; i++) pl_db_hix_put(r, i);
    }
}
pl_db_key_t * pl_db_reg_find(pl_db_reg_t *r, const char *key) {
    if (!r || !key) return (pl_db_key_t *)0;
    if (r->hix) {
        int m = r->hcap - 1;
        for (int i = (int)(pl_db_hash_bytes(0xCBF29CE484222325ull, key, strlen(key)) & (uint64_t)m), x; (x = r->hix[i]) != 0; i = (i + 1) & m) if (!strcmp(r->e[x - 1].key, key)) return &r->e[x - 1];
        return (pl_db_key_t *)0;
    }
    for (int i = 0; i < r->n; i++) if (!strcmp(r->e[i].key, key)) return &r->e[i];
    return (pl_db_key_t *)0;
}
static int pl_db_kmod_get(void *root, int64_t k) { pl_db_reg_t *r = pl_db_registry(root, 0); return (r && r->kmod && k >= 0 && k < r->kmcap) ? r->kmod[k] : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_db_shape_cells(const pl_cell_t *t) {
    int64_t c = 0;
    for (;;) {
        t = pl_deref((pl_cell_t *)t);
        c++;
        if ((int)t->v != DT_PLREF || !t->p) return c;
        { int ar = plc_fid_arity(t->slen); const pl_cell_t *a = (const pl_cell_t *)t->p; if (ar <= 0) return c; for (int i = 0; i + 1 < ar; i++) c += pl_db_shape_cells(&a[i]); t = &a[ar - 1]; }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_db_clause_bytes(const pl_cell_t *cl) { return (int64_t)sizeof(pl_db_slot_t) + 16 * pl_db_shape_cells(cl); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int64_t pl_db_term_bytes(const pl_cell_t *term) {
    extern int prolog_atom_intern(const char *);
    const pl_cell_t *t = pl_deref((pl_cell_t *)term);
    if ((int)t->v == DT_PLREF && plc_fid_arity(t->slen) == 2 && plc_functor((pl_cell_t *)t) == prolog_atom_intern(":-")) return pl_db_clause_bytes(t);
    return (int64_t)sizeof(pl_db_slot_t) + 16 * (pl_db_shape_cells(t) + 1 + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_db_mod_t * pl_db_mod_at(void *root, int idx) {
    extern int prolog_atom_intern(const char *);
    pl_db_reg_t *r = pl_db_registry(root, 1);
    if (!r) return (pl_db_mod_t *)0;
    if (r->nmods == 0) pl_db_mod_find(r, prolog_atom_intern("user"), 1);
    if (idx < 0 || idx >= r->nmods) idx = 0;
    return (pl_db_mod_t *)r->mods + idx;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_mod_charge(void *root, pl_db_t *db, int64_t d) { pl_db_mod_t *m = pl_db_mod_at(root, db->mod); if (m) m->size += d; }
void pl_db_note_live(void *root, pl_db_t *db, int i) { pl_db_mod_charge(root, db, pl_db_clause_bytes(&db->s[i].cl)); }
static pl_db_key_t * pl_db_reg_add(pl_db_reg_t *r, const char *key) {
    if (!r || !key) return (pl_db_key_t *)0;
    if (r->n >= r->cap) {
        int nc = r->cap > 0 ? r->cap * 2 : 32;
        pl_db_key_t *ne = (pl_db_key_t *)rt_pl_struct_alloc(HB_PLDBK, (size_t)nc * sizeof(pl_db_key_t));
        if (!ne) return (pl_db_key_t *)0;
        for (int i = 0; i < r->n; i++) ne[i] = r->e[i];
        r->e = ne;
        r->cap = nc;
    }
    {
        char *ks = rt_str_dup(key);
        if (!ks) return (pl_db_key_t *)0;
        pl_db_key_t *e = &r->e[r->n++];
        e->key = ks;
        e->k = -1;
        e->meta = 0;
        e->db = (pl_db_t *)0;
        e->stat = 0;
        e->decl = 0;
        e->file = -1;
        e->line = 0;
        e->mf = 0;
        e->lib = 0;
        { int mi = pl_db_mod_of_key(r, key, 1); if (mi >= 0) ((pl_db_mod_t *)r->mods)[mi].size += (int64_t)sizeof(pl_db_key_t) + (int64_t)strlen(key) + 1; }
        pl_db_hix_index(r);
        return &r->e[r->n - 1];
    }
}
int rt_pl_db_bind(void *root, int64_t k, const char *name, int64_t arity) {
    if (!root || !name || k <= PL_DB_REGISTRY_CELL) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = pl_db_reg_find(r, key);
        if (!e) e = pl_db_reg_add(r, key);
        if (!e || !pl_db_cell_addr(root, k, 1)) return 0;
        e->k = (int)k;
        pl_db_kmod_set(r, e->k, pl_db_mod_of_key(r, e->key, 1));
        if ((int)k + 1 > r->next_cell) r->next_cell = (int)k + 1;
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_key_cell(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->k >= 0) ? e->k : -1; }
int rt_pl_db_cell_for(void *root, const char *name, int arity) {
    { pl_db_reg_t *r = name ? pl_db_registry(root, 0) : (pl_db_reg_t *)0; pl_db_key_t *e = (r && r->hix) ? rt_pl_db_reg_find_na(r, name, arity) : (pl_db_key_t *)0; if (e && e->k >= 0) return e->k; }
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = r ? pl_db_reg_find(r, key) : (pl_db_key_t *)0;
        if (e && e->k >= 0) return e->k;
        if (!r) return -1;
        if (r->next_cell <= PL_DB_REGISTRY_CELL) r->next_cell = PL_DB_REGISTRY_CELL + 1;
        if (!e) e = pl_db_reg_add(r, key);
        if (!e || !pl_db_cell_addr(root, r->next_cell, 1)) return -1;
        e->k = r->next_cell++;
        pl_db_kmod_set(r, e->k, pl_db_mod_of_key(r, e->key, 1));
        return e->k;
    }
}
int rt_pl_db_key_is_dynamic_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_is_dynamic(root, key); }
int rt_pl_db_key_cell_na(void *root, const char *name, int arity) { PL_DB_KEY_VLA(key, name, arity); return rt_pl_db_key_cell(root, key); }
int rt_pl_tbl_dl(void *root, int set, void *val, pl_tr_ctx_t *cx) {
    int64_t k = rt_pl_db_cell_for(root, "$tbl_dl", -1);
    return k < 0 ? 0 : set ? rt_pl_b_set(root, k, val, cx) : rt_pl_nb_get_cell(root, k, val, cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_term_cell(void *root, void *term_cell) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell);
    pl_cell_t *h = t;
    const char *nm;
    int ar = 0;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); ar = pl_arity(h); } else if ((int)h->v == DT_PLATOM) nm = prolog_atom_name((int)h->i);
    else if ((int)h->v == DT_S) nm = h->s;
    else return -1;
    if (!nm) return -1;
    { PL_DB_KEY_VLA(key, nm, ar); return rt_pl_db_key_cell(root, key); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_frame_cells(void) { return PL_DB_FRAME_CELLS; }
static void pl_db_key_set_file(void *root, const char *name, int64_t arity, int64_t file) {
    PL_DB_KEY_VLA(key, name, arity);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    if (e) e->file = (int)file;
}
static void pl_db_key_set_meta(void *root, const char *name, int64_t arity, int64_t kind, int64_t v) {
    PL_DB_KEY_VLA(key, name, arity);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    if (e && kind == 5) e->line = (int)v;
    if (e && kind == 6) e->mf = 1;
}
static void pl_db_key_put_meta(void *root, const char *key, const long *ids, int n) {
    extern void *ct_grow(void *, size_t);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (!e || n <= 0) return;
    if (r->mtn + n > r->mtcap) { int nc = r->mtcap ? r->mtcap : 64; while (nc < r->mtn + n) nc *= 2; r->mtok = (long *)ct_grow((void *)r->mtok, (size_t)nc * sizeof(long)); r->mtcap = nc; }
    e = pl_db_reg_find(r, key);
    for (int i = 0; i < n; i++) r->mtok[r->mtn + i] = ids[i];
    e->meta = r->mtn + 1;
    r->mtn += n;
}
int rt_pl_db_key_meta(void *root, const char *key, const long **ids) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    const char *sl = strrchr(key, '/');
    if (!e || e->meta <= 0 || !sl) return 0;
    *ids = r->mtok + e->meta - 1;
    return (int)strtol(sl + 1, (char **)0, 10);
}
void rt_pl_db_meta_decl(void *root, const char *name, int64_t arity, const char *const *toks) {
    extern int prolog_atom_intern(const char *);
    long ids[arity > 0 ? arity : 1];
    PL_DB_KEY_VLA(key, name, arity);
    if (!root || !name || arity <= 0) return;
    if (!pl_db_reg_find(pl_db_registry(root, 1), key)) (void)rt_pl_db_decl(root, name, arity, 7);
    for (int64_t i = 0; i < arity; i++) ids[i] = prolog_atom_intern(toks[i]);
    pl_db_key_put_meta(root, key, ids, (int)arity);
}
int rt_pl_db_key_line(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return e ? e->line : 0; }
int rt_pl_db_key_mf(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return e ? e->mf : 0; }
static void rt_pl_mkground_at(const long long *tab, long e, DESCR_t *out) {
    for (;;) {
        uint64_t w0 = (uint64_t)tab[2 * e], w1 = (uint64_t)tab[2 * e + 1];
        long n, a;
        DESCR_t *blk;
        if ((w0 & 0xFF) != DT_PLREF) { memcpy(out, &w0, 8); memcpy((char *)out + 8, &w1, 8); return; }
        n = (long)(w1 >> 32);
        a = (long)(uint32_t)w1;
        blk = (DESCR_t *)rt_gcheap_alloc(HB_DVEC, (uint64_t)(16 * n));
        memcpy(out, &w0, 8);
        out->p = blk;
        for (long i = 0; i + 1 < n; i++) rt_pl_mkground_at(tab, a + i, blk + i);
        out = blk + n - 1;
        e = a + n - 1;
    }
}
void rt_pl_mkground(const long long *tab, DESCR_t *out) { rt_pl_mkground_at(tab, 0, out); }
void rt_pl_db_decls_install(const long *tab, void *root) {
    extern const char *prolog_atom_name(int);
    long cells;
    long n;
    const long *e;
    if (!tab || !root) return;
    n = tab[0] / 4;
    cells = tab[1];
    e = tab + 2;
    rt_pl_db_cells_base(root, cells);
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        if (nm && e[4 * i + 2] != 4 && e[4 * i + 2] < 5 && e[4 * i + 3] >= 0) rt_pl_db_bind(root, e[4 * i + 3], nm, e[4 * i + 1]);
    }
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        if (nm && e[4 * i + 2] == 4) pl_db_key_set_file(root, nm, e[4 * i + 1], e[4 * i + 3]);
        else if (nm && e[4 * i + 2] == 8) continue;
        else if (nm && e[4 * i + 2] == 9) pl_db_key_set_ncl(root, nm, e[4 * i + 1], e[4 * i + 3]);
        else if (nm && e[4 * i + 2] >= 5 && e[4 * i + 2] != 7) pl_db_key_set_meta(root, nm, e[4 * i + 1], e[4 * i + 2], e[4 * i + 3]);
        else if (nm && e[4 * i + 2] > 0) rt_pl_db_decl(root, nm, e[4 * i + 1], e[4 * i + 2]);
    }
    for (long i = 0; i < n; i++) {
        const char *nm = prolog_atom_name((int)e[4 * i]);
        long ar = e[4 * i + 1], j = 0;
        if (!nm || e[4 * i + 2] != 8 || ar <= 0 || i + ar > n) continue;
        {
            long ids[ar];
            PL_DB_KEY_VLA(key, nm, ar);
            for (j = 0; j < ar && e[4 * (i + j) + 2] == 8 && e[4 * (i + j)] == e[4 * i] && e[4 * (i + j) + 1] == ar; j++) ids[j] = e[4 * (i + j) + 3];
            if (j == ar) { if (!pl_db_reg_find(pl_db_registry(root, 1), key)) (void)rt_pl_db_decl(root, nm, ar, 7); pl_db_key_put_meta(root, key, ids, (int)ar); }
            i += j - 1;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_cells_base(void *root, int64_t n) { pl_db_reg_t *r = pl_db_registry(root, 1); if (!r) return 0; if ((int)n > r->next_cell) r->next_cell = (int)n; return 1; }
int rt_pl_db_key_is_dynamic(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && !e->stat && !e->lib) ? 1 : 0; }
int rt_pl_db_key_is_static(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->stat) ? 1 : 0; }
int rt_pl_db_key_lib_take(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); if (!e || !e->lib) return 0; e->lib = 0; return 1; }
const char *rt_pl_db_key_file(void *root, const char *key) {
    extern const char *prolog_atom_name(int);
    pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key);
    return (e && e->file >= 0 && pl_db_key_current(root, e)) ? prolog_atom_name(e->file) : (const char *)0;
}
int rt_pl_db_key_kind(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); if (!e || !pl_db_key_current(root, e)) return 0; return e->stat ? 1 : 2; }
int rt_pl_db_decl(void *root, const char *name, int64_t arity, int64_t kind) {
    if (!root || !name) return 0;
    PL_DB_KEY_VLA(key, name, arity);
    {
        pl_db_reg_t *r = pl_db_registry(root, 1);
        pl_db_key_t *e = pl_db_reg_find(r, key);
        if (!e) e = pl_db_reg_add(r, key);
        if (!e) return 0;
        if (kind == 1) { if (e->k < 0 && !e->db) e->stat = 1; } else if (kind == 7) e->lib = 1;
        else if (kind == 3) e->stat = 1;
        else e->decl = 1;
        if (kind == 2) { pl_db_t *d = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db; if (d && d->whole) pl_db_dynamize(root, d); }
        return 1;
    }
}
pl_db_t * pl_db_cell_peek(void *root, int k) { if (!root || k < 0) return (pl_db_t *)0; { pl_db_t **cell = (pl_db_t **)pl_db_cell_addr(root, k, 0); return cell ? *cell : (pl_db_t *)0; } }
int pl_db_key_current(void *root, pl_db_key_t *e) {
    pl_db_t *db;
    if (!strncmp(e->key, "$pl_", 4) || !strncmp(e->key, "$db_", 4)) return 0;
    if (e->lib) return 0;
    if (e->stat) return 1;
    db = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db;
    if (db) return db->killed ? 0 : 1;
    return e->decl ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_db_space_ball(void *root, const char *key, void *clause_term) {
    extern void *rt_pl_ball_kind1(const char *, const char *);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_mod_t *m;
    int mi;
    int64_t need;
    if (!r || !key || !clause_term || r->nmods == 0) return (void *)0;
    mi = pl_db_mod_of_key(r, key, 0);
    if (mi < 0) return (void *)0;
    m = (pl_db_mod_t *)r->mods + mi;
    if (!m->limit) return (void *)0;
    need = pl_db_term_bytes((const pl_cell_t *)clause_term);
    if (!pl_db_reg_find(r, key)) need += (int64_t)sizeof(pl_db_key_t) + (int64_t)strlen(key) + 1;
    return (m->size + need > m->limit) ? rt_pl_ball_kind1("resource_error", "program_space") : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_db_store_refusal(void *root, void *db_v, void *clause_term) {
    extern void *rt_pl_ball_kind1(const char *, const char *);
    pl_db_t *db = (pl_db_t *)db_v;
    pl_db_mod_t *pm;
    if (!db || !clause_term) return (void *)0;
    pm = pl_db_mod_at(root, db->mod);
    if (!pm || !pm->limit) return (void *)0;
    return (pm->size + pl_db_term_bytes(pl_deref((pl_cell_t *)clause_term)) > pm->limit) ? rt_pl_ball_kind1("resource_error", "program_space") : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_space_limited(void *root) { pl_db_reg_t *r = pl_db_registry(root, 0); if (!r) return 0; for (int i = 0; i < r->nmods; i++) if (((pl_db_mod_t *)r->mods)[i].limit) return 1; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_cp_count(void *root) { pl_db_reg_t *r = pl_db_registry(root, 0); int c = 0; if (!r) return 0; for (int i = 0; i < r->n; i++) if (pl_db_key_current(root, &r->e[i])) c++; return c; }
const char * rt_pl_db_cp_nth(void *root, int64_t i, int *arity) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    int c = 0;
    if (!r || i < 1) return (const char *)0;
    for (int j = 0; j < r->n; j++) if (pl_db_key_current(root, &r->e[j]) && ++c == (int)i) { const char *sl = strrchr(r->e[j].key, '/'); *arity = sl ? atoi(sl + 1) : 0; return r->e[j].key; }
    return (const char *)0;
}
void * rt_pl_db_get_by_key(void *root, const char *key, int create) {
    pl_db_reg_t *r = pl_db_registry(root, create);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (e && e->k >= 0) return rt_pl_db_get(root, e->k);
    if (!create) return (void *)0;
    if (!e) e = pl_db_reg_add(r, key);
    if (!e) return (void *)0;
    e->k = r->next_cell++;
    pl_db_kmod_set(r, e->k, pl_db_mod_of_key(r, e->key, 1));
    return rt_pl_db_get(root, e->k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_fragment(void *root, pl_db_t *db, int i) {
    extern void * pl_runtime_define_fragment(const char *, void *, int, int, int, int *, int *, void *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    int ar = 0;
    int ridx = -1;
    int off = 0;
    const char *nm;
    if (!db || i < 0 || i >= db->n) return 0;
    nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!nm) return 0;
    {
        PL_DB_KEY_VLA(key, nm, ar);
        {
            char fkey[fmt_len("%s@%d", key, i)];
            void *fn;
            snprintf(fkey, sizeof fkey, "%s@%d", key, i);
            fn = pl_runtime_define_fragment(fkey, (void *)&db->s[i].cl, ar, db->cell_k, i, &ridx, &off, root);
            if (!fn) return 0;
            pl_db_frag_record(db, i, fn);
        }
        db->s[i].ridx = ridx;
        db->s[i].chain_off = off;
        if (db->head_idx >= 0) {
            char hkey[fmt_len("%s@%d", key, db->head_idx)];
            snprintf(hkey, sizeof hkey, "%s@%d", key, db->head_idx);
            if (pl_runtime_install_packet_entry(key, ar, hkey)) db->entry_idx = db->head_idx;
        }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *pl_db_head_name(void *pair_cell, int *ar) {
    extern const char *prolog_atom_name(int);
    pl_cell_t *p = pl_deref((pl_cell_t *)pair_cell);
    if ((int)p->v != DT_PLREF || pl_arity(p) != 2) return (const char *)0;
    {
        pl_cell_t *h = pl_deref(&((pl_cell_t *)p->p)[0]);
        if ((int)h->v == DT_PLREF) { *ar = pl_arity(h); return prolog_atom_name(plc_functor(h)); }
        if ((int)h->v == DT_PLATOM) { *ar = 0; return prolog_atom_name((int)h->i); }
        if ((int)h->v == DT_S) { *ar = 0; return h->s; }
        return (const char *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_head_key(void *pair_cell, char *out, size_t n, int *ar) { const char *nm = pl_db_head_name(pair_cell, ar); if (!nm) return 0; snprintf(out, n, "%s/%d", nm, *ar); return 1; }
int rt_pl_db_assert(void *root, void *db_v, void *clause_term, int prepend) {
    if (db_v && ((pl_db_t *)db_v)->whole) pl_db_dynamize(root, (pl_db_t *)db_v);
    return pl_db_store(root, db_v, clause_term, prepend, 1, 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_assert_ref(void *root, void *db_v, void *clause_term, int prepend) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (db && db->whole) pl_db_dynamize(root, db);
    if (!pl_db_store(root, db_v, clause_term, prepend, 1, 1)) return 0;
    return db->s[db->n - 1].ref;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_src_set_last(void *db_v, int file, int line) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || db->n < 1 || !db->src) return 0;
    db->src[2 * (db->n - 1)] = file;
    db->src[2 * (db->n - 1) + 1] = line;
    return 1;
}
int rt_pl_db_src_of_ref(void *db_v, int ref, int *file, int *line) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || ref < 1 || !db->src) return 0;
    for (int i = 0; i < db->n; i++) if (db->s[i].ref == ref) { *file = db->src[2 * i]; *line = db->src[2 * i + 1]; return 1; }
    return 0;
}
int rt_pl_db_slot_of_ref(void *db_v, int ref) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || ref < 1) return -1;
    for (int i = 0; i < db->n; i++) if (!db->s[i].erased && db->s[i].ref == ref) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_ref_at(void *db_v, int i) { pl_db_t *db = (pl_db_t *)db_v; if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0; return db->s[i].ref; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_seed(void *root, void *db_v, void *clause_term) { return pl_db_store(root, db_v, clause_term, 0, 1, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_data_enter(void *root, pl_db_t *db, int i) {
    extern int rt_proc_is_registered(const char *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    int ar = 0;
    const char *nm = pl_db_head_name((void *)&db->s[i].cl, &ar);
    if (!nm || ar < 1) return 0;
    {
        PL_DB_KEY_VLA(key, nm, ar);
        char skey[fmt_len("%s@data", key)];
        snprintf(skey, sizeof skey, "%s@data", key);
        if (!rt_proc_is_registered(skey) && !pl_db_define_data(root, nm, ar, skey)) return 0;
        return pl_runtime_install_packet_entry(key, ar, skey);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_data_leave(pl_db_t *db) { db->data_off = 1; db->data_on = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_install_first(void *root, pl_db_t *db) {
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    int ar = 0, first = -1;
    const char *nm;
    for (int j = db->head_idx; j >= 0 && first < 0; j = db->s[j].next_idx) if (!db->s[j].erased) first = j;
    if (first >= 0 && db->s[first].ridx < 0 && !pl_db_fragment(root, db, first)) return;
    nm = first >= 0 ? pl_db_head_name((void *)&db->s[first].cl, &ar) : (const char *)0;
    if (!nm) return;
    {
        PL_DB_KEY_VLA(key, nm, ar);
        char hkey[fmt_len("%s@%d", key, first)];
        snprintf(hkey, sizeof hkey, "%s@%d", key, first);
        if (pl_runtime_install_packet_entry(key, ar, hkey)) db->entry_idx = first;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_place(void *root, pl_db_t *db, int i) {
    int leaving;
    if (!db->data_off && pl_db_data_fact(db, i)) { if (!db->data_on && pl_db_data_enter(root, db, i)) db->data_on = 1; if (db->data_on) return; }
    leaving = !db->data_off;
    if (leaving) pl_db_data_leave(db);
    pl_db_fragment(root, db, i);
    if (leaving) pl_db_install_first(root, db);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_dynamize(void *root, pl_db_t *db) {
    int w = db->whole;
    if (!w) return;
    db->whole = 0;
    if (w == 1 || w == 3) { for (int j = db->head_idx; j >= 0; j = db->s[j].next_idx) if (!db->s[j].erased) pl_db_place(root, db, j); return; }
    if (db->data_on) return;
    db->data_off = 1;
    pl_db_install_first(root, db);
}
int rt_pl_db_slot_ensure(void *root, void *db_v, int i) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || i < 0 || i >= db->n) return 0;
    if (db->s[i].ridx >= 0) return 1;
    return pl_db_fragment(root, db, i);
}
static int pl_db_src_grow(pl_db_t *db, int n, int cap) {
    extern void *rt_heap_alloc_c(size_t);
    int *ns = (int *)rt_heap_alloc_c((size_t)(cap > 0 ? cap : 1) * 2 * sizeof(int));
    if (!ns) return 0;
    for (int i = 0; i < cap; i++) { ns[2 * i] = i < n && db->src ? db->src[2 * i] : -1; ns[2 * i + 1] = i < n && db->src ? db->src[2 * i + 1] : 0; }
    db->src = ns;
    return 1;
}
int pl_db_store(void *root, void *db_v, void *clause_term, int prepend, int recompile, int tx) {
    extern int prolog_atom_intern(const char *);
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !clause_term) return 0;
    pl_db_reclaim(db);
    db->killed = 0;
    if (db->n >= db->cap) {
        int nc = db->cap > 0 ? db->cap * 2 : 8;
        if (!pl_db_src_grow(db, db->n, nc)) return 0;
        pl_db_slot_t *ns = (pl_db_slot_t *)rt_pl_struct_alloc(HB_PLDBS, (size_t)nc * sizeof(pl_db_slot_t));
        if (!ns) return 0;
        for (int i = 0; i < db->n; i++) ns[i] = db->s[i];
        db->s = ns;
        db->cap = nc;
    }
    {
        pl_cell_t *t = pl_deref((pl_cell_t *)clause_term);
        pl_cell_t kids[2];
        pl_cell_t pair;
        { pl_db_mod_t *pm = pl_db_mod_at(root, db->mod); if (pm && pm->limit && pm->size + pl_db_term_bytes(t) > pm->limit) return 0; }
        if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) {
            pl_cell_t *aa = (pl_cell_t *)t->p;
            kids[0] = aa[0];
            kids[1] = aa[1];
        } else {
            kids[0] = *t;
            kids[1] = pl_make_atom(prolog_atom_intern("true"));
        }
        pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
        {
            pl_cell_t stored = plc_copy(&pair);
            int i = db->n;
            if (db->next_ref < 1) db->next_ref = 1;
            db->s[i].cl = stored;
            db->src[2 * i] = -1;
            db->src[2 * i + 1] = 0;
            db->s[i].erased = 0;
            db->s[i].ref = db->next_ref++;
            db->s[i].ridx = -1;
            db->s[i].chain_off = 0;
            db->s[i].next_idx = -1;
            db->s[i].frag = 0;
            if (prepend) {
                db->s[i].next_idx = db->head_idx;
                if (db->head_idx >= 0 && db->head_idx < db->recl_lo) db->recl_lo = db->head_idx;
                db->head_idx = i;
                if (db->tail_idx < 0) db->tail_idx = i;
            } else {
                if (db->tail_idx >= 0) db->s[db->tail_idx].next_idx = i;
                else db->head_idx = i;
                db->tail_idx = i;
            }
            db->n++;
            pl_db_note_live(root, db, i);
            pl_ix_note(db, i, prepend);
            if (tx) pl_db_tx_note(root, db, prepend ? 2 : 0, i);
            if (recompile) pl_db_place(root, db, i);
        }
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_note_erase(void *root, pl_db_t *db, int i, int gen) {
    db->s[i].erased = gen;
    db->erased_n++;
    db->recl_since++;
    if (i < db->recl_lo) db->recl_lo = i;
    pl_db_mod_charge(root, db, -pl_db_clause_bytes(&db->s[i].cl));
}
static pl_cell_t * pl_db_tx_log(void *root) {
    extern int prolog_atom_intern(const char *);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = (r && r->hix) ? rt_pl_db_reg_find_na(r, "$tx_cur", -1) : (pl_db_key_t *)0;
    pl_cell_t **cell, *box, *t;
    if (!e || e->k < 0) return (pl_cell_t *)0;
    cell = (pl_cell_t **)pl_db_cell_addr(root, e->k, 0);
    box = cell ? *cell : (pl_cell_t *)0;
    if (!box || plc_gvar_absent(box)) return (pl_cell_t *)0;
    t = pl_deref(box);
    if ((int)t->v != DT_PLREF || !t->p || plc_fid_arity(t->slen) != 1 || plc_fid_name(t->slen) != prolog_atom_intern("tx")) return (pl_cell_t *)0;
    return &((pl_cell_t *)t->p)[0];
}
static void pl_db_tx_note(void *root, pl_db_t *db, int kind, int slot) {
    extern int prolog_atom_intern(const char *);
    pl_cell_t *lst = pl_db_tx_log(root);
    const char *nm;
    int ar = 0;
    if (!lst || !db || slot < 0 || slot >= db->n) return;
    nm = pl_db_head_name((void *)&db->s[slot].cl, &ar);
    if (!nm || nm[0] == '$') return;
    {
        PL_DB_KEY_VLA(key, nm, ar);
        pl_cell_t *da = (pl_cell_t *)rt_ws_alloc_descr(5), *cons = (pl_cell_t *)rt_ws_alloc_descr(2), *rk = (pl_cell_t *)rt_ws_alloc_descr(2);
        rk[0] = pl_make_atom(prolog_atom_intern(key));
        rk[1] = pl_make_int(db->s[slot].ref);
        da[0] = pl_make_int(kind);
        da[1] = pl_make_int(db->cell_k);
        da[2] = pl_make_int(slot);
        da[3] = pl_make_compound(prolog_atom_intern("$clref"), 2, (void *)rk);
        da[4] = kind == 1 ? plc_copy(&db->s[slot].cl) : pl_make_atom(g_runtime.atoms.atom_nil);
        cons[0] = pl_make_compound(prolog_atom_intern("$txdb"), 5, (void *)da);
        cons[1] = *lst;
        *lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, (void *)cons);
        if (kind == 1) db->tx_pins++;
    }
}
static void pl_db_erase_slot(void *root, pl_db_t *db, int i, int gen) { pl_db_note_erase(root, db, i, gen); pl_db_tx_note(root, db, 1, i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_erase(void *root, void *db_v, int i) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (db && db->whole) pl_db_dynamize(root, db);
    if (!db || i < 0 || i >= db->n || db->s[i].erased) return 0;
    pl_db_erase_slot(root, db, i, db->next_ref++);
    pl_db_reclaim_report(db);
    pl_db_reclaim(db);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_unload_src(void *root, int file) {
    pl_db_reg_t *r = pl_db_registry(root, 0);
    int n = 0;
    if (!r || file < 0) return 0;
    for (int k = 0; k < r->n; k++) {
        pl_db_t *db;
        if (r->e[k].stat || !pl_db_key_current(root, &r->e[k])) continue;
        db = (r->e[k].k >= 0) ? pl_db_cell_peek(root, r->e[k].k) : r->e[k].db;
        if (!db || !db->src) continue;
        for (int i = db->n - 1; i >= 0; i--) if (i < db->n && !db->s[i].erased && db->src[2 * i] == file && rt_pl_db_erase(root, db, i)) n++;
    }
    return n;
}
int rt_pl_db_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->n : 0; }
int rt_pl_db_slot_erased(void *db_v, int i) { pl_db_t *db = (pl_db_t *)db_v; return db && i >= 0 && i < db->n && db->s[i].erased; }
int rt_pl_db_abolish(void *root, void *db_v) {
    pl_db_t *db = (pl_db_t *)db_v;
    int ar = 0;
    int hi = -1;
    if (!db) return 0;
    if (db->whole) pl_db_dynamize(root, db);
    for (int i = 0; i < db->n && hi < 0; i++) if (pl_db_head_name((void *)&db->s[i].cl, &ar)) hi = i;
    { int st = db->next_ref++; for (int i = 0; i < db->n; i++) if (!db->s[i].erased) pl_db_erase_slot(root, db, i, st); }
    pl_db_reclaim_report(db);
    db->killed = 1;
    if (hi >= 0) { const char *nm = pl_db_head_name((void *)&db->s[hi].cl, &ar); PL_DB_KEY_VLA(key, nm, ar); pl_db_define_absent(root, key, ar); }
    db->data_on = 0;
    pl_db_reclaim(db);
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_match_erase(void *root, void *db_v, void *goal_term) {
    pl_db_t *db = (pl_db_t *)db_v;
    int hit = 0;
    int ar = 0;
    int key_i = -1;
    if (!db || !goal_term) return 0;
    if (db->whole) pl_db_dynamize(root, db);
    int64_t kk = db->ix ? pl_ix_head_key(db, goal_term) : 0;
    pl_ix_ent_t *ke = kk ? pl_ix_find((pl_ix_t *)db->ix, kk) : (pl_ix_ent_t *)0;
    for (int i = kk ? (ke ? ke->first : -1) : 0; kk ? i >= 0 : i < db->n; i = pl_ix_step(db, kk, i)) {
        if (db->s[i].erased) continue;
        {
            pl_cell_t pair = plc_copy(&db->s[i].cl);
            pl_cell_t g = plc_copy((pl_cell_t *)goal_term);
            pl_cell_t *h = (pl_cell_t *)pl_deref(&pair)->p;
            if (h && pl_unify(&h[0], &g)) { if (key_i < 0 && pl_db_head_name((void *)&db->s[i].cl, &ar)) key_i = i; pl_db_erase_slot(root, db, i, db->next_ref++); hit++; }
        }
    }
    (void)key_i;
    (void)ar;
    if (hit) pl_db_reclaim_report(db);
    if (hit) pl_db_reclaim(db);
    return hit;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_killed(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->killed : 0; }
void rt_pl_db_define(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; if (db && db->killed == 2) db->killed = 0; }
int rt_pl_db_born_undef(void *root, void *db_v, const char *pi) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || db->killed != 2 || !pi || pi[0] == '$') return 0;
    {
        const char *sl = strrchr(pi, '/');
        if (!sl) return 0;
        {
            size_t nl = (size_t)(sl - pi);
            char nm[nl + 1];
            memcpy(nm, pi, nl);
            nm[nl] = 0;
            PL_DB_KEY_VLA(key, nm, atol(sl + 1));
            { pl_db_reg_t *r = pl_db_registry(root, 0); pl_db_key_t *e = r ? pl_db_reg_find(r, key) : (pl_db_key_t *)0; return !(e && e->decl); }
        }
    }
}
