#include "runtime/unification/un_head.h"
#include "ir/ct_arena.h"
#include "runtime/core/core.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_07_pl.h"
#include "runtime/unification/un_08_pl.h"
extern const char *prolog_atom_name(int id);
int rt_pl_db_copy(void *db_v, int i, void *out_v);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_make(DESCR_t *a, int n) {
    extern void *rt_pl_ball_instantiation(void);
    if (!a || n < 1) return (void *)0;
    if (pl_cell_unbound(pl_deref((pl_cell_t *)&a[0]))) return rt_pl_ball_instantiation();
    pl_cell_t *b = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    if (!b) return (void *)0;
    *b = plc_copy_x((pl_cell_t *)&a[0], 1);
    return (void *)b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pl_ball_key_str(DESCR_t d) { if ((int)d.v == DT_PLATOM) return prolog_atom_name((int)d.i); if ((int)d.v == DT_S || (int)d.v == DT_N) return d.s; return (const char *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence(DESCR_t *a, int n) { extern void *rt_pl_ball_existence_key(const char *key); return rt_pl_ball_existence_key((a && n > 0) ? pl_ball_key_str(a[0]) : (const char *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_existence_key(const char *key) {
    extern int rt_pl_unknown_suppress(const char *);
    int ar = 0;
    if (rt_pl_unknown_suppress(key)) return (void *)0;
    const char *sl = key ? strrchr(key, '/') : (const char *)0;
    int nid = sl ? prolog_atom_intern_n(key, (size_t)(sl - key)) : prolog_atom_intern(key ? key : "?");
    if (sl) ar = atoi(sl + 1);
    pl_cell_t pi[2];
    pi[0] = pl_make_atom(nid);
    pi[1] = pl_make_int(ar);
    pl_cell_t *pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pl_cell_t ee[2];
    ee[0] = pl_make_atom(prolog_atom_intern("procedure"));
    ee[1] = *pic;
    pl_cell_t *eec = (pl_cell_t *)rt_pl_compound_cell("existence_error", 2, (void *)ee);
    if (!eec) return (void *)0;
    pl_cell_t er[2];
    er[0] = *eec;
    er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_ball_report(void *ball) {
    extern void rt_pl_write_cell_fp(void *, FILE *);
    fflush(stdout);
    fprintf(stderr, "Warning: goal raised exception: ");
    if (ball) rt_pl_write_cell_fp(ball, stderr);
    else fprintf(stderr, "unknown");
    fprintf(stderr, "\n");
    fflush(stderr);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cell_atom_id(pl_cell_t *c) {
    pl_cell_t *d = pl_deref(c);
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_mod_find(pl_db_reg_t *r, int atom, int create) {
    extern void *ct_grow(void *, size_t);
    extern int prolog_atom_intern(const char *);
    pl_db_mod_t *m;
    if (!r) return -1;
    if (r->nmods == 0) {
        r->mods = ct_grow((void *)r->mods, 8 * sizeof(pl_db_mod_t));
        r->modcap = 8;
        m = (pl_db_mod_t *)r->mods;
        memset(m, 0, 8 * sizeof(pl_db_mod_t));
        m[0].atom = prolog_atom_intern("user");
        r->nmods = 1;
    }
    m = (pl_db_mod_t *)r->mods;
    for (int i = 0; i < r->nmods; i++) if (m[i].atom == atom) return i;
    if (!create) return -1;
    if (r->nmods == r->modcap) {
        r->mods = ct_grow((void *)r->mods, (size_t)r->modcap * 2 * sizeof(pl_db_mod_t));
        memset((pl_db_mod_t *)r->mods + r->modcap, 0, (size_t)r->modcap * sizeof(pl_db_mod_t));
        r->modcap *= 2;
        m = (pl_db_mod_t *)r->mods;
    }
    m[r->nmods].atom = atom;
    m[r->nmods].size = 0;
    m[r->nmods].limit = 0;
    return r->nmods++;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_mod_of_key(pl_db_reg_t *r, const char *key, int create) {
    extern int prolog_atom_intern_n(const char *, size_t);
    extern int prolog_atom_intern(const char *);
    size_t n = 0;
    while (key[n] && key[n] != ':') {
        unsigned char c = (unsigned char)key[n];
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$')) return pl_db_mod_find(r, prolog_atom_intern("user"), 1);
        n++;
    }
    if (!key[n] || n == 0) return pl_db_mod_find(r, prolog_atom_intern("user"), 1);
    return pl_db_mod_find(r, prolog_atom_intern_n(key, n), create);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pl_db_kmod_set(pl_db_reg_t *r, int k, int mod) {
    extern void *ct_grow(void *, size_t);
    if (!r || k < 0 || mod < 0) return;
    if (k >= r->kmcap) {
        int nc = r->kmcap > 0 ? r->kmcap : 64;
        while (nc <= k) nc *= 2;
        r->kmod = (int *)ct_grow((void *)r->kmod, (size_t)nc * sizeof(int));
        memset(r->kmod + r->kmcap, 0, (size_t)(nc - r->kmcap) * sizeof(int));
        r->kmcap = nc;
    }
    r->kmod[k] = mod;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pl_db_ncl_set(pl_db_reg_t *r, int k, int n) {
    extern void *ct_grow(void *, size_t);
    if (!r || k < 0) return;
    if (k >= r->nclcap) {
        int nc = r->nclcap > 0 ? r->nclcap : 64;
        while (nc <= k) nc *= 2;
        r->ncl = (int *)ct_grow((void *)r->ncl, (size_t)nc * sizeof(int));
        memset(r->ncl + r->nclcap, 0, (size_t)(nc - r->nclcap) * sizeof(int));
        r->nclcap = nc;
    }
    r->ncl[k] = n;
}
void pl_db_key_set_ncl(void *root, const char *name, int64_t arity, int64_t v) {
    PL_DB_KEY_VLA(key, name, arity);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    if (e) pl_db_ncl_set(r, (int)(e - r->e), (int)v);
}
int rt_pl_db_key_nclauses(void *root, const char *key) {
    extern int rt_pl_db_live_count(void *);
    pl_db_reg_t *r = pl_db_registry(root, 0);
    pl_db_key_t *e = pl_db_reg_find(r, key);
    pl_db_t *db;
    int k;
    if (!e || !pl_db_key_current(root, e)) return -1;
    db = (e->k >= 0) ? pl_db_cell_peek(root, e->k) : e->db;
    if (db) return rt_pl_db_live_count(db);
    k = (int)(e - r->e);
    return (r->ncl && k < r->nclcap && r->ncl[k] > 0) ? r->ncl[k] : -1;
}
int rt_pl_db_mod_get(void *root, const char *module, int which, int64_t *out) {
    extern int prolog_atom_intern(const char *);
    pl_db_reg_t *r = pl_db_registry(root, 1);
    int mi;
    pl_db_mod_t *m;
    if (!r || !module) return 0;
    pl_db_mod_at(root, 0);
    mi = pl_db_mod_find(r, prolog_atom_intern(module), 0);
    if (mi < 0) return 0;
    m = (pl_db_mod_t *)r->mods + mi;
    *out = which ? m->limit : m->size;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_mod_set_limit(void *root, const char *module, int64_t n) {
    extern int prolog_atom_intern(const char *);
    pl_db_reg_t *r = pl_db_registry(root, 1);
    int mi;
    pl_db_mod_t *m;
    if (!r || !module) return 1;
    pl_db_mod_at(root, 0);
    mi = pl_db_mod_find(r, prolog_atom_intern(module), 1);
    m = (pl_db_mod_t *)r->mods + mi;
    if (n != 0 && n < m->size) return 1;
    m->limit = n;
    return 0;
}
static pl_cell_t *pl_db_cell_strip(pl_cell_t *t, const char **mod, int *found) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *slow = pl_deref(t);
    int step = 0;
    *mod = (const char *)0;
    *found = 0;
    t = slow;
    while ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":")) {
        pl_cell_t *m = pl_deref(&((pl_cell_t *)t->p)[0]);
        const char *nm;
        if ((int)m->v != DT_PLATOM) break;
        nm = prolog_atom_name((int)m->i);
        *mod = (nm && strcmp(nm, "user")) ? nm : (const char *)0;
        *found = 1;
        t = pl_deref(&((pl_cell_t *)t->p)[1]);
        if (step++ & 1) { slow = pl_deref(&((pl_cell_t *)slow->p)[1]); if (slow == t) return (pl_cell_t *)0; }
    }
    return t;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_mangle_head(pl_cell_t *h, const char *mod, pl_cell_t *out) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    const char *nm;
    int ar;
    if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); ar = pl_arity(h); } else if ((int)h->v == DT_PLATOM) { nm = prolog_atom_name((int)h->i); ar = 0; } else return 0;
    if (!nm) return 0;
    {
        char qn[strlen(mod) + strlen(nm) + 2];
        int fid;
        snprintf(qn, sizeof qn, "%s:%s", mod, nm);
        fid = prolog_atom_intern(qn);
        if (ar == 0) { *out = pl_make_atom(fid); return 1; }
        {
            pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)ar * sizeof(pl_cell_t));
            for (int i = 0; i < ar; i++) blk[i] = pl_make_ref(&((pl_cell_t *)h->p)[i], 0);
            *out = pl_make_compound(fid, ar, blk);
        }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_callable_head(const pl_cell_t *h) { return (int)h->v == DT_PLREF || (int)h->v == DT_PLATOM; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_db_unqual_term(void *slot) {
    extern int prolog_atom_intern(const char *);
    pl_cell_t *a = (pl_cell_t *)slot;
    pl_cell_t *in, *h, *body = (pl_cell_t *)0;
    const char *mod, *mod2;
    int found, found2, clause;
    in = pl_db_cell_strip(a, &mod, &found);
    if (!in) return;
    clause = (int)in->v == DT_PLREF && pl_arity(in) == 2 && plc_functor(in) == prolog_atom_intern(":-");
    h = clause ? pl_deref(&((pl_cell_t *)in->p)[0]) : in;
    if (clause) { h = pl_db_cell_strip(h, &mod2, &found2); if (!h) return; body = &((pl_cell_t *)in->p)[1]; if (found2) { mod = mod2; found = 1; } }
    if (!found || !pl_db_callable_head(h)) return;
    {
        pl_cell_t nh = *h;
        if (mod && !pl_db_mangle_head(h, mod, &nh)) return;
        if (!clause) { *a = nh; return; }
        { pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t)); blk[0] = nh; blk[1] = pl_make_ref(body, 0); *a = pl_make_compound(prolog_atom_intern(":-"), 2, blk); }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_db_unqual_pi(void *slot) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *a = (pl_cell_t *)slot;
    pl_cell_t *in, *nm, *ar;
    const char *mod, *mod2;
    int found, found2, fid;
    in = pl_db_cell_strip(a, &mod, &found);
    if (!in || (int)in->v != DT_PLREF || pl_arity(in) != 2 || plc_functor(in) != prolog_atom_intern("/")) return;
    nm = pl_db_cell_strip(&((pl_cell_t *)in->p)[0], &mod2, &found2);
    ar = &((pl_cell_t *)in->p)[1];
    if (!nm || (int)nm->v != DT_PLATOM) return;
    if (found2) { mod = mod2; found = 1; }
    if (!found) return;
    {
        const char *base = prolog_atom_name((int)nm->i);
        pl_cell_t *blk;
        if (!base) return;
        if (mod) { char qn[strlen(mod) + strlen(base) + 2]; snprintf(qn, sizeof qn, "%s:%s", mod, base); fid = prolog_atom_intern(qn); } else fid = (int)nm->i;
        blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        blk[0] = pl_make_atom(fid);
        blk[1] = pl_make_ref(ar, 0);
        *a = pl_make_compound(prolog_atom_intern("/"), 2, blk);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_term_key(void *term_cell, char *out, size_t n, int *ar) {
    extern const char *prolog_atom_name(int);
    extern int prolog_atom_intern(const char *);
    pl_cell_t *t = pl_deref((pl_cell_t *)term_cell);
    pl_cell_t *h = t;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2 && plc_functor(t) == prolog_atom_intern(":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]);
    {
        const char *nm;
        if ((int)h->v == DT_PLREF) {
            nm = prolog_atom_name(plc_functor(h));
            *ar = pl_arity(h);
        } else if ((int)h->v == DT_PLATOM) {
            nm = prolog_atom_name((int)h->i);
            *ar = 0;
        } else if ((int)h->v == DT_S) {
            nm = h->s;
            *ar = 0;
        } else return 0;
        if (!nm) return 0;
        snprintf(out, n, "%s/%d", nm, *ar);
        return 1;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_gen(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->next_ref : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_db_slot_copy(pl_db_t *db, int i, void *out_v) { *(pl_cell_t *)out_v = plc_copy(&db->s[i].cl); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_as_of(void *db_v, int i, int gen, void *out_v) {
    pl_db_t *db = (pl_db_t *)db_v;
    if (!db || !out_v || i < 0 || i >= db->n || db->s[i].ref >= gen || (db->s[i].erased && db->s[i].erased < gen)) return 0;
    return pl_db_slot_copy(db, i, out_v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_live_count(void *db_v) { pl_db_t *db = (pl_db_t *)db_v; return db ? db->n - db->erased_n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_clause_at(void *db_v, int i, void *out_v) { pl_db_t *db = (pl_db_t *)db_v; if (!db || !out_v || i < 0 || i >= db->n || db->s[i].erased) return 0; return pl_db_slot_copy(db, i, out_v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_db_copy(void *db_v, int i, void *out_v) { pl_db_t *db = (pl_db_t *)db_v; if (!db || !out_v || i < 0 || i >= db->n) return 0; return pl_db_slot_copy(db, i, out_v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_db_define_absent(void *root, const char *key, int arity) {
    extern void * rt_pl_clause_tree(void *);
    extern void * pl_runtime_clause_tree(void *);
    extern void * pl_runtime_define_pred(const char *, const void *, int, void *);
    extern void * rt_pl_choice_new(const char *);
    extern void rt_pl_choice_add(void *, void *);
    extern int prolog_atom_intern(const char *);
    extern int rt_proc_is_registered(const char *);
    extern int pl_runtime_install_packet_entry(const char *, int, const char *);
    const char *sl;
    pl_cell_t *hargs;
    pl_cell_t head;
    pl_cell_t pi[2];
    pl_cell_t pic;
    pl_cell_t ee[2];
    pl_cell_t eec;
    pl_cell_t er[2];
    pl_cell_t erc;
    pl_cell_t th[1];
    pl_cell_t body;
    pl_cell_t kids[2];
    pl_cell_t pair;
    void *raw;
    void *cl;
    void *ch;
    if (!key) return 0;
    sl = strrchr(key, '/');
    if (!sl) return 0;
    char skey[fmt_len("%s@absent", key)];
    snprintf(skey, sizeof skey, "%s@absent", key);
    if (rt_proc_is_registered(skey)) return pl_runtime_install_packet_entry(key, arity, skey);
    int nid = prolog_atom_intern_n(key, (size_t)(sl - key));
    if (arity > 0) {
        hargs = (pl_cell_t *)PL_CELL_ALLOC((size_t)arity * sizeof(pl_cell_t));
        if (!hargs) return 0;
        pl_tr_ctx_t rcx = { 0 };
        rcx.r14 = (uint64_t)(uintptr_t)root;
        for (int i = 0; i < arity; i++) pl_init_var(&hargs[i], &rcx);
        head = pl_make_compound(nid, arity, hargs);
    } else head = pl_make_atom(nid);
    pi[0] = pl_make_atom(nid);
    pi[1] = pl_make_int(arity);
    pic = pl_make_compound(prolog_atom_intern("/"), 2, (void *)pi);
    ee[0] = pl_make_atom(prolog_atom_intern("procedure"));
    ee[1] = pic;
    eec = pl_make_compound(prolog_atom_intern("existence_error"), 2, (void *)ee);
    er[0] = eec;
    er[1] = pic;
    erc = pl_make_compound(prolog_atom_intern("error"), 2, (void *)er);
    th[0] = erc;
    body = pl_make_compound(prolog_atom_intern("throw"), 1, (void *)th);
    kids[0] = head;
    kids[1] = body;
    pair = pl_make_compound(prolog_atom_intern(":-"), 2, (void *)kids);
    raw = rt_pl_clause_tree((void *)&pair);
    cl = raw ? pl_runtime_clause_tree(raw) : (void *)0;
    if (!cl) return 0;
    ch = rt_pl_choice_new(skey);
    rt_pl_choice_add(ch, cl);
    if (!pl_runtime_define_pred(skey, ch, arity, root)) return 0;
    return pl_runtime_install_packet_entry(key, arity, skey);
}
