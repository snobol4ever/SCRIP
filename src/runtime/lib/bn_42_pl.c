#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_05_pl.h"
extern int rt_pl_db_assert(void *, void *, void *, int);
extern int rt_pl_trie_new(void *);
extern int rt_pl_trie_alive(void *, int64_t);
extern int rt_pl_trie_destroy(void *, int64_t);
extern int rt_pl_trie_insert(void *, int64_t, void *, void *, int);
extern int rt_pl_trie_lookup(void *, int64_t, void *, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_delete(void *, int64_t, void *, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_entries(void *, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_entry_key(void *, int64_t, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_slice(void *, int64_t, int64_t, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_count(void *, int64_t);
extern int rt_pl_trie_meta_set(void *, int64_t, int, void *);
extern int rt_pl_trie_meta_get(void *, int64_t, int, void *, pl_tr_ctx_t *);
extern int rt_pl_trie_tbl_vt(void *);
extern int rt_pl_tbh_open(void *, void *, pl_tr_ctx_t *);
extern int rt_pl_tbh_close(void *, pl_tr_ctx_t *);
extern int rt_pl_tbh_leading(void *);
extern int rt_pl_tbh_get(void *, int64_t, void *, pl_tr_ctx_t *);
extern int rt_pl_tbh_put(void *, int64_t, int, void *, pl_tr_ctx_t *);
extern DESCR_t * rt_pl_tbh_current(void *);
extern int rt_pl_tbh_floor_sync(void *, char *);
extern char * pl_tr_base_of(const char *);
extern char ** pl_tr_floor_slot(const char *);
extern uint64_t g_rtccb[32];
extern pl_trie_sl_t * rt_pl_trie_sl(void *, int64_t);
extern int rt_pl_trie_count(void *, int64_t);
extern int rt_pl_trie_nents(void *, int64_t);
extern int64_t rt_pl_tbh_table(void *);
extern int64_t rt_pl_tbl_chain_get(void *);
extern int rt_pl_tbl_chain_set(void *, int64_t);
extern int rt_pl_tbl_abolish_id(void *, int64_t);
extern void rt_pl_pseg_ball(void);
static int pl_trie_leaf_int(DESCR_t a, int64_t *out) { DESCR_t d = rt_pl_deref_val(a); if (d.v != DT_I) return 0; *out = d.i; return 1; }
static int pl_trie_leaf_open(DESCR_t *args, int nargs, int ar, pl_tr_ctx_t *cx, int64_t *id) {
    if (nargs != ar) return 0;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    return id ? pl_trie_leaf_int(args[0], id) : 1;
}
static DESCR_t pl_trie_leaf_close(int ok, pl_tr_ctx_t *cx) { rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
DESCR_t rt_pl_dop_trie_new_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int id;
    if (!pl_trie_leaf_open(args, nargs, 1, cx, (int64_t *)0)) return FAILDESCR;
    id = rt_pl_trie_new(root);
    return pl_trie_leaf_close(id >= 0 && plw_unify_vals(args[0], INTVAL(id), cx), cx);
}
DESCR_t rt_pl_dop_trie_alive_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    if (!pl_trie_leaf_open(args, nargs, 1, cx, &id)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_alive(root, id), cx);
}
DESCR_t rt_pl_dop_trie_destroy_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    if (!pl_trie_leaf_open(args, nargs, 1, cx, &id)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_destroy(root, id), cx);
}
DESCR_t rt_pl_dop_trie_insert_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id, upd;
    int r;
    if (!pl_trie_leaf_open(args, nargs, 5, cx, &id) || !pl_trie_leaf_int(args[1], &upd)) return FAILDESCR;
    r = rt_pl_trie_insert(root, id, (void *)&args[2], (void *)&args[3], (int)upd);
    return pl_trie_leaf_close(r != -1 && plw_unify_vals(args[4], INTVAL(r), cx), cx);
}
DESCR_t rt_pl_dop_trie_lookup_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    if (!pl_trie_leaf_open(args, nargs, 3, cx, &id)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_lookup(root, id, (void *)&args[1], (void *)&args[2], cx), cx);
}
DESCR_t rt_pl_dop_trie_delete_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    if (!pl_trie_leaf_open(args, nargs, 3, cx, &id)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_delete(root, id, (void *)&args[1], (void *)&args[2], cx), cx);
}
DESCR_t rt_pl_dop_trie_entries_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    if (!pl_trie_leaf_open(args, nargs, 2, cx, &id)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_entries(root, id, (void *)&args[1], cx), cx);
}
DESCR_t rt_pl_dop_trie_slice_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id, from, to;
    if (!pl_trie_leaf_open(args, nargs, 4, cx, &id) || !pl_trie_leaf_int(args[1], &from) || !pl_trie_leaf_int(args[2], &to)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_slice(root, id, from, to, (void *)&args[3], cx), cx);
}
DESCR_t rt_pl_dop_trie_entry_key_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id, ent;
    if (!pl_trie_leaf_open(args, nargs, 3, cx, &id) || !pl_trie_leaf_int(args[1], &ent)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_entry_key(root, id, ent, (void *)&args[2], cx), cx);
}
DESCR_t rt_pl_dop_trie_count_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id;
    int n;
    if (!pl_trie_leaf_open(args, nargs, 2, cx, &id)) return FAILDESCR;
    n = rt_pl_trie_count(root, id);
    return pl_trie_leaf_close(n >= 0 && plw_unify_vals(args[1], INTVAL(n), cx), cx);
}
DESCR_t rt_pl_dop_trie_meta_set_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id, which;
    if (!pl_trie_leaf_open(args, nargs, 3, cx, &id) || !pl_trie_leaf_int(args[1], &which)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_meta_set(root, id, (int)which, (void *)&args[2]), cx);
}
DESCR_t rt_pl_dop_trie_meta_get_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t id, which;
    if (!pl_trie_leaf_open(args, nargs, 3, cx, &id) || !pl_trie_leaf_int(args[1], &which)) return FAILDESCR;
    return pl_trie_leaf_close(rt_pl_trie_meta_get(root, id, (int)which, (void *)&args[2], cx), cx);
}
DESCR_t rt_pl_dop_tbl_vt_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int id;
    if (!pl_trie_leaf_open(args, nargs, 1, cx, (int64_t *)0)) return FAILDESCR;
    id = rt_pl_trie_tbl_vt(root);
    return pl_trie_leaf_close(id >= 0 && plw_unify_vals(args[0], INTVAL(id), cx), cx);
}
DESCR_t rt_pl_dop_tbl_open_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { int ok = rt_pl_tbh_open(root, (void *)&args[0], cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
static char * pl_pseg_bytes(pl_pseg_t *b) { return rt_pseg_bytes(b); }
static char * pl_pseg_trail(pl_pseg_t *b) { return rt_pseg_trail(b); }
static DESCR_t * pl_st_arg(DESCR_t st, int k) {
    DESCR_t *t = plw_cell_deref(plw_entry(&st));
    if (t->v != (DTYPE_t)DT_PLREF || !t->p || (int)plc_fid_arity(t->slen) < k) return (DESCR_t *)0;
    return plw_cell_deref(&((DESCR_t *)t->p)[k - 1]);
}
static int64_t pl_st_int(DESCR_t st, int k) { DESCR_t *a = pl_st_arg(st, k); return (a && a->v == (DTYPE_t)DT_I) ? a->i : 0; }
static void pl_st_put(DESCR_t st, int k, int64_t v) { DESCR_t *t = plw_cell_deref(plw_entry(&st)); if (t->v == (DTYPE_t)DT_PLREF && t->p) ((DESCR_t *)t->p)[k - 1] = (DESCR_t){ .v = DT_I, .i = v }; }
static pl_pseg_t * pl_pseg_of(DESCR_t st) { DESCR_t *a = pl_st_arg(st, 1); return (a && a->v == (DTYPE_t)DT_P) ? (pl_pseg_t *)a->p : (pl_pseg_t *)0; }
static pl_pseg_t * pl_sched_pick(pl_psched_t *s) {
    void *root = (void *)(uintptr_t)s->r14;
    pl_pseg_t *nb = (pl_pseg_t *)0;
    DESCR_t nst = {0};
    for (int neg = 0; neg < 2; neg++) for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0; ) {
        pl_trie_sl_t *sl = rt_pl_trie_sl(root, x);
        if (!sl) break;
        {
            int cnt = rt_pl_trie_nents(root, x);
            for (DESCR_t cur = rt_pl_deref_val(neg ? sl->ncons : sl->pcons); cur.v == (DTYPE_t)DT_PLREF && plc_fid_arity(cur.slen) == 2; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
                DESCR_t st = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
                pl_pseg_t *b = pl_pseg_of(st);
                if (!b) continue;
                if (!neg && cnt > pl_st_int(st, 3)) return b;
                if (neg && sl->uncond == 0 && pl_st_int(st, 5) == 0) { nb = b; nst = st; }
            }
        }
        if (x == s->leader) break;
        x = sl->next;
    }
    if (nb) pl_st_put(nst, 5, 1);
    return nb;
}
static void pl_pseg_unrebind(pl_pseg_t *b) {
    char *hi = b->orig_lo + b->clen, *lo_t = (char *)(uintptr_t)b->sched_t;
    for (uint64_t i = b->tn; i-- > 0; ) {
        char *src = pl_pseg_trail(b) + i * 64;
        pl_tr_entry_t *e = (pl_tr_entry_t *)src;
        if (b->tbase + i * PL_TR_ENTRY_BYTES < lo_t || !e->cell || ((char *)e->cell >= b->orig_lo && (char *)e->cell < hi)) continue;
        memcpy((void *)e->cell, src + PL_TR_ENTRY_BYTES + sizeof(DESCR_t), sizeof(DESCR_t));
    }
}
pl_pseg_pair_t rt_pl_pseg_next(pl_psched_t *s) {
    pl_pseg_t *b = pl_sched_pick(s);
    if (!b) {
        pl_trie_sl_t *ls = rt_pl_trie_sl((void *)(uintptr_t)s->r14, s->leader);
        if (ls) ls->sched = 0;
        rt_pl_tbh_floor_sync((void *)(uintptr_t)s->r14, (char *)(uintptr_t)s->r12);
        rt_pl_tr_gc_sync((const char *)(uintptr_t)s->r12);
        for (int k = 0; k < 4; k++) g_rtccb[5 + k] = s->planes[k];
        g_runtime.abi.rt_k_level = (int)s->k_level;
        return (pl_pseg_pair_t){ (void *)0, (void *)s };
    }
    {
        char *now = (char *)(uintptr_t)s->r12, *xg = b->orig_lo + b->len, *rsp_r = (char *)(uintptr_t)(s->entry_rsp + 8);
        char *hi = xg, *lo_t = now > b->tbase ? now : b->tbase, *hp = *(char * const *)(const void *)&g_runtime.abi.hp_fr;
        if (rsp_r < xg) { fprintf(stderr, "FATAL rt_pl_pseg_next: the resumer's rsp %p lies inside the segment it restores (carve top %p)\n", (void *)rsp_r, (void *)xg); * (volatile char *) NULL; }
        b->clen = b->len;
        for (char *p = now; p < b->tbase; p += PL_TR_ENTRY_BYTES) memset(p, 0, PL_TR_ENTRY_BYTES);
        for (uint64_t i = 0; i < b->tn; i++) {
            char *src = pl_pseg_trail(b) + i * 64, *dst = b->tbase + i * PL_TR_ENTRY_BYTES;
            pl_tr_entry_t *e = (pl_tr_entry_t *)src;
            if (dst < lo_t) continue;
            memcpy(dst, src, PL_TR_ENTRY_BYTES);
            if (!e->cell) { ((pl_tr_entry_t *)dst)->pad = (uint64_t)(uintptr_t)hp; continue; }
            if ((char *)e->cell >= b->orig_lo && (char *)e->cell < hi) continue;
            memcpy(src + PL_TR_ENTRY_BYTES + sizeof(DESCR_t), (void *)e->cell, sizeof(DESCR_t));
            memcpy((void *)e->cell, src + PL_TR_ENTRY_BYTES, sizeof(DESCR_t));
        }
        b->sched_t = (uint64_t)(uintptr_t)lo_t;
        rt_pl_tr_gc_sync((const char *)(uintptr_t)b->r12);
        if ((char *)(uintptr_t)b->hdr >= b->orig_lo && (char *)(uintptr_t)b->hdr + 16 * (PL_TBH_CELLS + 5) <= hi) {
            DESCR_t *c = (DESCR_t *)(pl_pseg_bytes(b) + ((char *)(uintptr_t)b->hdr - b->orig_lo));
            c[PL_TBH_CELLS + 2] = (DESCR_t){ .v = DT_I, .i = (int64_t)(s->entry_rsp + 8) };
            *pl_tr_floor_slot(pl_tr_base_of(b->tbase)) = (char *)(uintptr_t)c[PL_TBH_CELLS + 1].i;
            {
                char *ra = (char *)(uintptr_t)c[PL_TBH_CELLS + 3].i;
                if (ra >= b->orig_lo && ra + 24 <= hi) {
                    uint64_t *w = (uint64_t *)(pl_pseg_bytes(b) + (ra - b->orig_lo));
                    w[0] = s->ret_pc;
                    w[1] = (uint64_t)(uintptr_t)&rt_pl_pseg_ball;
                    w[2] = s->rbp;
                }
            }
        }
        b->sched = s;
        for (int k = 0; k < 4; k++) g_rtccb[5 + k] = b->planes[k];
        g_runtime.abi.rt_k_level = (int)b->k_level;
        return (pl_pseg_pair_t){ (void *)b, (void *)s };
    }
}
static pl_pseg_t * pl_sched_resumed(pl_psched_t *s) {
    void *root = (void *)(uintptr_t)s->r14;
    for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0; ) {
        pl_trie_sl_t *sl = rt_pl_trie_sl(root, x);
        if (!sl) break;
        for (int neg = 0; neg < 2; neg++)
            for (DESCR_t cur = rt_pl_deref_val(neg ? sl->ncons : sl->pcons); cur.v == (DTYPE_t)DT_PLREF && plc_fid_arity(cur.slen) == 2; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
            pl_pseg_t *b = pl_pseg_of(rt_pl_deref_val(((DESCR_t *)cur.p)[0]));
            if (b && b->sched == (void *)s) return b;
        }
        if (x == s->leader) break;
        x = sl->next;
    }
    return (pl_pseg_t *)0;
}
void * rt_pl_pseg_ball_c(void *root, char *r12) {
    pl_psched_t *s = (pl_psched_t *)0;
    for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0; ) {
        pl_trie_sl_t *sl = rt_pl_trie_sl(root, x);
        if (!sl) break;
        if (sl->sched) { s = (pl_psched_t *)(uintptr_t)sl->sched; break; }
        x = sl->next;
    }
    if (!s) { fprintf(stderr, "FATAL rt_pl_pseg_ball: a ball left a generator with no resuming leader on the chain\n"); * (volatile char *) NULL; }
    { pl_pseg_t *b = pl_sched_resumed(s); if (b) { rt_pl_tr_unwind_to(r12, (char *)(uintptr_t)b->r12); pl_pseg_unrebind(b); b->sched = (pl_psched_t *)0; } }
    rt_pl_tbh_floor_sync(root, r12);
    for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0; ) {
        pl_trie_sl_t *sl = rt_pl_trie_sl(root, x);
        int64_t nx;
        if (!sl) break;
        nx = sl->next;
        rt_pl_tbl_abolish_id(root, x);
        if (x == s->leader) { rt_pl_tbl_chain_set(root, nx); break; }
        x = nx;
    }
    for (int k = 0; k < 4; k++) g_rtccb[5 + k] = s->planes[k];
    g_runtime.abi.rt_k_level = (int)s->k_level;
    return (void *)s;
}
void * rt_pl_pseg_park(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, char *lrbp) {
    extern void rt_pl_db_pin_cell(void *, int);
    DESCR_t *h = rt_pl_tbh_current((void *)(uintptr_t)cx->r14);
    if (nargs != 1 || !h || (int)h[PL_TBH_CELLS].v != DT_I || (int)h[PL_TBH_CELLS + 1].v != DT_I) {
        fprintf(stderr, "FATAL rt_pl_pseg_park: no current tabling header with a carve top\n");
        * (volatile char *) NULL;
    }
    {
        char *xg = (char *)(uintptr_t)h[PL_TBH_CELLS].i, *tbase = (char *)(uintptr_t)h[PL_TBH_CELLS + 1].i, *orig_lo = lrbp + 8, *top = cx->tr;
        pl_pseg_t *old = pl_pseg_of(args[0]);
        if (xg <= orig_lo || top < tbase) {
            fprintf(stderr, "FATAL rt_pl_pseg_park: carve top %p at or below the park %p, or trail top %p below the base %p\n", (void *)xg, (void *)orig_lo, (void *)top, (void *)tbase);
            * (volatile char *) NULL;
        }
        {
            uint64_t len = (uint64_t)(xg - orig_lo), tn = (uint64_t)(top - tbase) / PL_TR_ENTRY_BYTES;
            pl_pseg_t *b = (pl_pseg_t *)rt_pl_struct_alloc(HB_PSEG, rt_pseg_size(orig_lo, len, tn));
            b->orig_lo = orig_lo;
            b->len = len;
            b->tbase = tbase;
            b->tn = tn;
            b->rbp = *(uint64_t *)lrbp;
            b->rbx = cx->rbx;
            b->r12 = (uint64_t)(uintptr_t)top;
            b->r13 = (uint64_t)(uintptr_t)cx->b;
            b->r14 = cx->r14;
            b->r15 = cx->r15;
            for (int k = 0; k < 4; k++) b->planes[k] = g_rtccb[5 + k];
            b->k_level = g_runtime.abi.rt_k_level;
            b->sched = (pl_psched_t *)0;
            b->link = (void *)0;
            b->hdr = (uint64_t)(uintptr_t)h;
            {
                char *hp = *(char * const *)(const void *)&g_runtime.abi.hp_fr;
                for (char *p = pl_tr_base_of(top) + PL_TR_HEADER_BYTES; p + PL_TR_ENTRY_BYTES <= top; p += PL_TR_ENTRY_BYTES) {
                    pl_tr_entry_t *e = (pl_tr_entry_t *)p;
                    if (!e->cell && e->pad && (char *)(uintptr_t)e->pad < hp) e->pad = (uint64_t)(uintptr_t)hp;
                }
            }
            memcpy(pl_pseg_bytes(b), orig_lo, len);
            for (uint64_t o = 0; o + 8 <= len; o += 8) {
                uint64_t wv = *(const uint64_t *)(const void *)(orig_lo + o);
                if ((wv & 0xFFFFFFFFFFFFull) == (uint64_t)DT_RAW && (wv >> 48)) rt_pl_db_pin_cell((void *)(uintptr_t)cx->r14, (int)(wv >> 48) - 1);
            }
            for (uint64_t i = 0; i < tn; i++) {
                char *src = tbase + i * PL_TR_ENTRY_BYTES, *dst = pl_pseg_trail(b) + i * 64;
                pl_tr_entry_t *e = (pl_tr_entry_t *)src;
                memcpy(dst, src, PL_TR_ENTRY_BYTES);
                if (e->cell) memcpy(dst + PL_TR_ENTRY_BYTES, (void *)e->cell, sizeof(DESCR_t));
                else memset(dst + PL_TR_ENTRY_BYTES, 0, sizeof(DESCR_t));
            }
            { DESCR_t *t = plw_cell_deref(plw_entry(&args[0])); if (t->v == (DTYPE_t)DT_PLREF && t->p) ((DESCR_t *)t->p)[0] = (DESCR_t){ .v = DT_P, .p = (void *)b }; }
            if (old && old->sched) {
                pl_psched_t *s = old->sched;
                old->sched = (pl_psched_t *)0;
                cx->tr = rt_pl_tr_unwind_to(cx->tr, (char *)(uintptr_t)old->r12);
                pl_pseg_unrebind(old);
                rt_pl_tbh_floor_sync((void *)(uintptr_t)cx->r14, cx->tr);
                return (void *)s;
            }
            return (void *)0;
        }
    }
}
pl_pseg_pair_t rt_pl_pseg_resume(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, char *lrbp) {
    int64_t leader;
    if (nargs != 1 || !pl_trie_leaf_int(args[0], &leader)) { fprintf(stderr, "FATAL rt_pl_pseg_resume: the argument is not a leader table\n"); * (volatile char *) NULL; }
    {
        pl_psched_t *s = (pl_psched_t *)rt_str_alloc((long)(sizeof(pl_psched_t) + 16));
        s->entry_rsp = (uint64_t)(uintptr_t)(lrbp + 8);
        s->ret_pc = *(uint64_t *)(lrbp + 8);
        s->rbp = *(uint64_t *)lrbp;
        s->rbx = cx->rbx;
        s->r12 = (uint64_t)(uintptr_t)cx->tr;
        s->r13 = (uint64_t)(uintptr_t)cx->b;
        s->r14 = cx->r14;
        s->r15 = cx->r15;
        for (int k = 0; k < 4; k++) s->planes[k] = g_rtccb[5 + k];
        s->k_level = g_runtime.abi.rt_k_level;
        s->leader = leader;
        { pl_trie_sl_t *ls = rt_pl_trie_sl((void *)(uintptr_t)s->r14, leader); if (ls) ls->sched = (int64_t)(uintptr_t)s; }
        return rt_pl_pseg_next(s);
    }
}
DESCR_t rt_pl_dop_tbl_sl_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t op, t;
    if (nargs != 3 || !pl_trie_leaf_int(args[0], &op) || !pl_trie_leaf_int(args[1], &t)) return FAILDESCR;
    {
        pl_trie_sl_t *sl = rt_pl_trie_sl(root, t);
        if (!sl) return FAILDESCR;
        switch (op) {
            case 1:
            {
                int64_t h = rt_pl_tbl_chain_get(root);
                pl_trie_sl_t *hs = h >= 0 ? rt_pl_trie_sl(root, h) : (pl_trie_sl_t *)0;
                sl->depth = hs ? hs->depth + 1 : 0;
                sl->low = sl->depth;
                sl->next = h;
                sl->done = 0;
                sl->uncond = 0;
                sl->pcons = pl_nil();
                sl->ncons = pl_nil();
                return rt_pl_tbl_chain_set(root, t) ? pl_ok() : FAILDESCR;
            }
            case 2:
            case 3:
            {
                int64_t g = rt_pl_tbh_table(root);
                pl_trie_sl_t *gs = g >= 0 ? rt_pl_trie_sl(root, g) : (pl_trie_sl_t *)0;
                DESCR_t st = rt_pl_deref_val(args[2]);
                if (op == 2) sl->pcons = pl_cons(st, sl->pcons);
                else sl->ncons = pl_cons(st, sl->ncons);
                if (gs && sl->depth < gs->low) gs->low = sl->depth;
                return pl_ok();
            }
            case 7:
            sl->uncond++;
            return pl_ok();
            case 4:
            return sl->low >= sl->depth ? pl_ok() : FAILDESCR;
            case 5:
            {
                int64_t p = rt_pl_tbh_table(root);
                pl_trie_sl_t *ps = p >= 0 ? rt_pl_trie_sl(root, p) : (pl_trie_sl_t *)0;
                if (ps && sl->low < ps->low) ps->low = sl->low;
                rt_pl_tbh_floor_sync(root, cx->tr);
                return pl_ok();
            }
            case 8:
            rt_pl_tbh_floor_sync(root, cx->tr);
            return pl_ok();
            case 6:
            {
                int n = 0, nn = 0;
                for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0; ) { pl_trie_sl_t *xs = rt_pl_trie_sl(root, x); if (!xs) break; nn++; if (x == t) break; x = xs->next; }
                DESCR_t ids[nn > 0 ? nn : 1];
                for (int64_t x = rt_pl_tbl_chain_get(root); x >= 0 && n < nn; ) {
                    pl_trie_sl_t *xs = rt_pl_trie_sl(root, x);
                    if (!xs) break;
                    ids[n++] = INTVAL(x);
                    xs->pcons = pl_nil();
                    xs->ncons = pl_nil();
                    xs->done = 1;
                    if (x == t) { rt_pl_tbl_chain_set(root, xs->next); break; }
                    x = xs->next;
                }
                rt_pl_tbh_floor_sync(root, cx->tr);
                return pl_trie_leaf_close(plw_unify_vals(args[2], pl_list_from_arr(ids, n), cx), cx);
            }
            default:
            return FAILDESCR;
        }
    }
}
DESCR_t rt_pl_dop_ssu_begin_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    uint64_t w;
    (void)root;
    if (nargs != 1 || !cx || !cx->r14) return FAILDESCR;
    w = *(const uint64_t *)(uintptr_t)(cx->r14 - PL_SERIAL_OFF);
    return pl_trie_leaf_close(plw_unify_vals(args[0], INTVAL((int64_t)(w >> PL_SERIAL_SHIFT)), cx), cx);
}
static DESCR_t *pl_ssu_goal_cell(const char *mark, long i, uint64_t s) {
    const pl_tr_entry_t *e = (const pl_tr_entry_t *)(mark + i * PL_TR_ENTRY_BYTES);
    if (!e->cell || (int)e->old.v != DT_PLVAR || pl_var_serial(&e->old) >= s) return (DESCR_t *)0;
    for (long j = 0; j < i; j++) if (((const pl_tr_entry_t *)(mark + j * PL_TR_ENTRY_BYTES))->cell == e->cell) return (DESCR_t *)0;
    return e->cell;
}
DESCR_t rt_pl_dop_ssu_check_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    int64_t s0;
    const char *mark;
    long n;
    (void)root;
    if (nargs != 1 || !pl_trie_leaf_int(args[0], &s0) || !cx || !cx->b) return FAILDESCR;
    mark = *(const char * const *)cx->b;
    if (mark < pl_tr_base_of(cx->tr) + PL_TR_HEADER_BYTES || mark > cx->tr) return FAILDESCR;
    n = (cx->tr - mark) / PL_TR_ENTRY_BYTES;
    for (long i = 0; i < n; i++) {
        DESCR_t *c = pl_ssu_goal_cell(mark, i, (uint64_t)s0), *d;
        if (!c) continue;
        d = plw_cell_deref(c);
        if (!plw_unbound_tag(d)) return FAILDESCR;
        if (d != c && pl_var_serial(d) < (uint64_t)s0) return FAILDESCR;
        for (long j = 0; j < i; j++) { DESCR_t *c2 = pl_ssu_goal_cell(mark, j, (uint64_t)s0); if (c2 && plw_cell_deref(c2) == d) return FAILDESCR; }
    }
    return pl_ok();
}
DESCR_t rt_pl_dop_tbh_close_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    (void)args;
    if (nargs != 0) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { int ok = rt_pl_tbh_close(root, cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_tbh_leading_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    (void)args;
    (void)cx;
    if (nargs != 0) return FAILDESCR;
    pl_atoms_ready();
    return rt_pl_tbh_leading(root) ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_tbh_get_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { DESCR_t f = rt_pl_deref_val(args[0]); int ok = (int)f.v == DT_I && rt_pl_tbh_get(root, f.i, (void *)&args[1], cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
static DESCR_t pl_tbh_put_leaf(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root, int mode) {
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    { DESCR_t f = rt_pl_deref_val(args[0]); int ok = (int)f.v == DT_I && rt_pl_tbh_put(root, f.i, mode, (void *)&args[1], cx); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
DESCR_t rt_pl_dop_tbh_own_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_tbh_put_leaf(args, nargs, cx, root, 0); }
DESCR_t rt_pl_dop_tbh_set_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_tbh_put_leaf(args, nargs, cx, root, 1); }
DESCR_t rt_pl_dop_tbh_bset_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) { return pl_tbh_put_leaf(args, nargs, cx, root, 2); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_db_add(DESCR_t *args, int nargs, void *root, int prepend, void **ball) {
    extern void *rt_pl_db_store_refusal(void *, void *, void *);
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    {
        void *db = pl_db_cell_of(args, root);
        if (!db) return FAILDESCR;
        if (rt_pl_db_assert(root, db, (void *)&args[1], prepend)) return pl_ok();
        *ball = rt_pl_db_store_refusal(root, db, (void *)&args[1]);
        return FAILDESCR;
    }
}
DESCR_t rt_pl_dop_db_assertz_c(DESCR_t *args, int nargs, void *root, void **ball) { return pl_db_add(args, nargs, root, 0, ball); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_asserta_c(DESCR_t *args, int nargs, void *root, void **ball) { return pl_db_add(args, nargs, root, 1, ball); }
