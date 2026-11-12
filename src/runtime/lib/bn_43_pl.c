#include "runtime/lib/bn_head.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/lib/bn_05_pl.h"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_store_k_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_term_cell(void *, void *);
    int k;
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    k = rt_pl_db_term_cell(root, (void *)&args[0]);
    return k >= 0 ? INTVAL(k) : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_copy_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_copy(void *, int, void *);
    if (nargs != 2) return FAILDESCR;
    {
        void *db = pl_db_cell_of(args, root);
        DESCR_t iv = rt_pl_deref_val(args[1]);
        DESCR_t out;
        if (!db || iv.v != DT_I) return FAILDESCR;
        return rt_pl_db_copy(db, (int)iv.i, (void *)&out) ? out : FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_db_data_unify_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *root) {
    extern int rt_pl_db_data_unify(void *, int, void *, pl_tr_ctx_t *);
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    {
        char *tr0 = cx->tr;
        void *db = pl_db_cell_of(args, root);
        DESCR_t iv = rt_pl_deref_val(args[1]);
        int ok = db && iv.v == DT_I && rt_pl_db_data_unify(db, (int)iv.i, (void *)&args[2], cx);
        if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
        rt_pl_tr_gc_sync(cx->tr);
        return ok ? pl_ok() : FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_dl_push_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *fb) {
    int64_t *w = (int64_t *)fb;
    DESCR_t lv;
    (void)cx;
    if (nargs != 1) return FAILDESCR;
    lv = rt_pl_deref_val(args[0]);
    if (lv.v != DT_I) return FAILDESCR;
    w[2] = w[0] + lv.i;
    w[3] = w[0];
    w[1] = (int64_t)(uintptr_t)&w[2];
    return pl_ok();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_dl_reached_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *fb) {
    int64_t *w = (int64_t *)fb, d = w[3] - w[0];
    DESCR_t v;
    if (nargs != 1) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    v = INTVAL(d < 1 ? 1 : d);
    { char *tr0 = cx->tr; int ok = plw_unify_vals(args[0], v, cx); if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_dl_reset_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *fb) { int64_t *w = (int64_t *)fb; (void)args; (void)nargs; (void)cx; w[3] = w[0] + 1; return pl_ok(); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_dl_tick_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *fb) {
    int64_t *w = (int64_t *)fb, *pair = (int64_t *)(uintptr_t)(w[1] & ~(int64_t)1), lvl = w[0] + 1;
    DESCR_t kv;
    (void)cx;
    if (nargs == 1) { kv = rt_pl_deref_val(args[0]); if (kv.v == DT_I && kv.i > 0) lvl += kv.i; }
    if (!pair) return pl_ok();
    if (lvl > pair[1]) pair[1] = lvl;
    return lvl > pair[0] ? FAILDESCR : pl_ok();
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_dl_false_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx, void *fb) {
    extern int prolog_atom_intern(const char *);
    int64_t *w = (int64_t *)fb;
    DESCR_t v;
    if (nargs != 1) return FAILDESCR;
    if (w[3] <= w[2]) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    v = pl_make_atom(prolog_atom_intern("depth_limit_exceeded"));
    { char *tr0 = cx->tr; int ok = plw_unify_vals(args[0], v, cx); if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0); rt_pl_tr_gc_sync(cx->tr); return ok ? pl_ok() : FAILDESCR; }
}
