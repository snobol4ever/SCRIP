#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/icn_builtins.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/rx.h"
#include "runtime/builtin_leaf.h"
#include <fcntl.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_19_rk.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern void *rt_ws_alloc_descr(size_t);
static int rk_qh_same(DESCR_t a, DESCR_t b);
static int rk_qh_kind(DESCR_t d);
extern int rt_proc_is_registered(const char *name);
extern FILE *fh_get(int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_num_dbl(DESCR_t v, double *d) {
    extern int rk_rat_to_real(DESCR_t, double *);
    if (IS_INT_fn(v)) { *d = (double) v.i; return 1; }
    if (IS_REAL_fn(v)) { *d = v.r; return 1; }
    if (v.v == DT_BIG) { *d = strtod(rk_cstr(v), NULL); return 1; }
    return v.v == DT_DATA && rk_rat_to_real(v, d);
}
DESCR_t rk_range_numeric(DESCR_t v) {
    DESCR_t lo, hi;
    int xl, xh;
    double dl, dh;
    rk_range_bounds(v, &lo, &hi, &xl, &xh);
    if (IS_INT_fn(lo) && IS_INT_fn(hi)) { long long n = hi.i - xh - lo.i - xl + 1; return INTVAL(n < 0 ? 0 : n); }
    if (!rk_num_dbl(lo, &dl) || !rk_num_dbl(hi, &dh)) return INTVAL(rk_av(v).n);
    if (dl > dh) return INTVAL(0);
    if (dl == -INFINITY || dh == INFINITY) return REALVAL(INFINITY);
    if (dl != dl || dh != dh) return REALVAL(NAN);
    double diff = dh - dl - xl;
    if (diff < 0) diff = 0;
    double f = floor(diff);
    return INTVAL((long long) f + 1 - (f == diff ? xh : 0));
}
static long long rk_excl_hi(DESCR_t v) {
    double d;
    if (IS_INT_fn(v)) return (long long) v.i - 1;
    if (rk_num_dbl(v, &d) && d == d && fabs(d) < 9.0e18) return (long long) ceil(d) - 1;
    return rk_ival(v) - 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_mk_jct(const char *flavor, DESCR_t *args, int nargs) {
    DESCR_t *src = args;
    int ns = nargs;
    if (nargs == 1 && args[0].v == DT_A && args[0].arr && !rk_is_jct(args[0]) && !rk_is_pair(args[0])) { rk_av_t a = rk_av(args[0]); src = (DESCR_t *) a.el; ns = a.n; }
    DESCR_t *r = ns > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) ns) : NULL;
    for (int i = 0; i < ns; i++) r[i] = rk_one(src[i]);
    DESCR_t j = rk_mk_arr(r, ns);
    ((ARBLK_t *) j.arr)->proto = flavor;
    return j;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *const rk_qh_names[] = { "Set", "SetHash", "Bag", "BagHash", "Mix", "MixHash", NULL };
static void rk_qh_init(void) {
    static int rk_qh_reg = 0;
    if (rk_qh_reg) return;
    rk_qh_reg = 1;
    extern void record_register(const char *spec);
    extern void class_inherit(const char *child, const char *parent);
    static const char *const roles[] = { "QuantHash", "Setty", "Baggy", "Mixy", NULL };
    for (int i = 0; roles[i]; i++) { char spec[strlen(roles[i]) + 24]; snprintf(spec, sizeof spec, "%s(elems,weights,idx)", roles[i]); record_register(spec); }
    class_inherit("Setty", "QuantHash");
    class_inherit("Baggy", "QuantHash");
    class_inherit("Mixy", "Baggy");
    for (int i = 0; rk_qh_names[i]; i++) {
        char spec[strlen(rk_qh_names[i]) + 24];
        snprintf(spec, sizeof spec, "%s(elems,weights,idx)", rk_qh_names[i]);
        record_register(spec);
        class_inherit(rk_qh_names[i], i < 2 ? "Setty" : i < 4 ? "Baggy" : "Mixy");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_kind(DESCR_t d) {
    if (!(IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name)) return -1;
    const char *n = d.u->type->name;
    if (d.u->type->nfields != 3 || strcmp(d.u->type->fields[0], "elems")) return -1;
    for (int i = 0; rk_qh_names[i]; i++) if (!strcmp(n, rk_qh_names[i])) return i;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_own_type(const char *cn) { DatType *dt = dat_find_type(cn); return !dt || (dt->nfields == 3 && !strcmp(dt->fields[0], "elems")); }
int rk_qh_is(DESCR_t d) { return rk_qh_kind(d) >= 0; }
static int rk_qh_mutable(int k) { return k == 1 || k == 3 || k == 5; }
static int rk_qh_baggy(int k) { return k == 2 || k == 3; }
static int rk_qh_mixy(int k) { return k == 4 || k == 5; }
static int rk_qh_setty(int k) { return k == 0 || k == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_new(int kind) { rk_qh_init(); return DATCON_fn(rk_qh_names[kind], rk_mk_arr(NULL, 0), rk_mk_arr(NULL, 0), TABLE_VAL(table_new())); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_qh_key(DESCR_t e) {
    const char *t = rk_cstr(e);
    char tag = IS_INT_fn(e) ? 'I' : IS_REAL_fn(e) ? 'N' : e.v == DT_BOOL ? 'B' : IS_STR_fn(e) ? 'S' : 'O';
    char *o = rt_wsb_alloc(strlen(t) + 3);
    o[0] = tag;
    o[1] = ':';
    strcpy(o + 2, t);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_find(DESCR_t q, DESCR_t e) {
    DESCR_t ix = FIELD_GET_fn(q, "idx");
    if (!(ix.v == DT_T && ix.tbl)) return -1;
    int found = 0;
    DESCR_t v = table_get_found_d(ix.tbl, rk_key_tmp(STRVAL((char *) rk_qh_key(e))), &found);
    return found ? (int) v.i : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_qh_n(DESCR_t q) { DESCR_t a = FIELD_GET_fn(q, "elems"); return (a.v == DT_A && a.arr) ? ((ARBLK_t *) a.arr)->hi - ((ARBLK_t *) a.arr)->lo + 1 : 0; }
static DESCR_t rk_qh_elem(DESCR_t q, int i) { return ((ARBLK_t *) FIELD_GET_fn(q, "elems").arr)->data[i]; }
static DESCR_t rk_qh_wt(DESCR_t q, int i) { return ((ARBLK_t *) FIELD_GET_fn(q, "weights").arr)->data[i]; }
static double rk_qh_w(DESCR_t w) { return IS_REAL_fn(w) ? w.r : IS_INT_fn(w) ? (double) w.i : w.v == DT_BOOL ? (double) w.i : w.v == DT_DATA ? rk_rat_num(w) : 0.0; }
static double rk_qh_wat(DESCR_t q, int i) { return rk_qh_setty(rk_qh_kind(q)) ? 1.0 : rk_qh_w(rk_qh_wt(q, i)); }
static DESCR_t rk_qh_true(void) { return (DESCR_t){ .v = DT_BOOL, .i = 1 }; }
static DESCR_t rk_qh_bool(int b) { return (DESCR_t){ .v = DT_BOOL, .i = b ? 1 : 0 }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_num(double v) { return (v == floor(v) && fabs(v) < 9e15) ? INTVAL((long) v) : REALVAL(v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_qh_reindex(DESCR_t q) {
    TBBLK_t *t = table_new();
    int n = rk_qh_n(q);
    for (int i = 0; i < n; i++) table_set_descr_d(t, rk_key_own(STRVAL((char *) rk_qh_key(rk_qh_elem(q, i)))), INTVAL(i));
    DATINST_t *di = (DATINST_t *) q.u;
    for (int f = 0; f < di->type->nfields; f++) if (!strcmp(di->type->fields[f], "idx")) di->fields[f] = TABLE_VAL(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_qh_drop(DESCR_t q, int pos) {
    ARBLK_t *ea = (ARBLK_t *) FIELD_GET_fn(q, "elems").arr, *wa = (ARBLK_t *) FIELD_GET_fn(q, "weights").arr;
    int n = rk_qh_n(q);
    memmove(ea->data + pos, ea->data + pos + 1, (size_t) (n - pos - 1) * sizeof(DESCR_t));
    memmove(wa->data + pos, wa->data + pos + 1, (size_t) (n - pos - 1) * sizeof(DESCR_t));
    ea->hi--;
    wa->hi--;
    rk_qh_reindex(q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_qh_set(DESCR_t q, DESCR_t e, double w) {
    int kind = rk_qh_kind(q), pos = rk_qh_find(q, e);
    if (rk_qh_setty(kind)) w = w != 0.0 ? 1.0 : 0.0;
    else if (rk_qh_baggy(kind)) w = floor(w);
    if (w == 0.0 || (w < 0.0 && !rk_qh_mixy(kind))) { if (pos >= 0) rk_qh_drop(q, pos); return; }
    DESCR_t wd = rk_qh_num(w);
    if (pos >= 0) { ((ARBLK_t *) FIELD_GET_fn(q, "weights").arr)->data[pos] = wd; return; }
    rk_arr_append((ARBLK_t *) FIELD_GET_fn(q, "elems").arr, e);
    rk_arr_append((ARBLK_t *) FIELD_GET_fn(q, "weights").arr, wd);
    table_set_descr_d(FIELD_GET_fn(q, "idx").tbl, rk_key_own(STRVAL((char *) rk_qh_key(e))), INTVAL(rk_qh_n(q) - 1));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_qh_bump(DESCR_t q, DESCR_t e, double dw) { int pos = rk_qh_find(q, e); double cur = pos >= 0 ? rk_qh_wat(q, pos) : 0.0; rk_qh_set(q, e, rk_qh_setty(rk_qh_kind(q)) ? 1.0 : cur + dw); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_qh_add(DESCR_t q, DESCR_t src, double mult, int weighted) {
    int kind = rk_qh_kind(q);
    if (rk_qh_is(src)) { int n = rk_qh_n(src); for (int i = 0; i < n; i++) rk_qh_bump(q, rk_qh_elem(src, i), rk_qh_wat(src, i) * mult); return; }
    if (src.v == DT_T && src.tbl) {
        TBBLK_t *t = src.tbl;
        for (unsigned i = 0; i < t->ord_len; i++) {
            DESCR_t key = t->ord[i];
            if (key.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(t, key);
            if (!e) continue;
            if (!weighted) rk_qh_bump(q, rk_mk_pair(key, rt_deref(e->val)), mult);
            else if (rk_qh_setty(kind)) { if (rk_is_truthy(rt_deref(e->val))) rk_qh_set(q, key, 1.0); } else { double w = rk_qh_w(rt_deref(e->val)); if (w != 0.0) rk_qh_bump(q, key, w * mult); }
        }
        return;
    }
    if (src.v == DT_A && src.arr && !rk_is_pair(src)) { rk_av_t a = rk_av(src); for (int i = 0; i < a.n; i++) rk_qh_add(q, a.el[i], mult, weighted); return; }
    if (src.v == DT_SNUL || IS_FAIL_fn(src)) return;
    if (weighted && rk_is_pair(src)) {
        rk_av_t p = rk_av(src);
        if (rk_qh_setty(kind)) { if (rk_is_truthy(p.el[1])) rk_qh_set(q, p.el[0], 1.0); } else { double w = rk_qh_w(p.el[1]); if (w != 0.0) rk_qh_bump(q, p.el[0], w * mult); }
        return;
    }
    rk_qh_bump(q, src, mult);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_from(DESCR_t src, int kind, int weighted) { DESCR_t q = rk_qh_new(kind); rk_qh_add(q, src, 1.0, weighted); return q; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_coerce(DESCR_t v, int *kind) { if (rk_qh_is(v)) { *kind = rk_qh_kind(v); return v; } *kind = 0; return rk_qh_from(v, 0, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_binop(const char *op, DESCR_t a, DESCR_t b) {
    int ka, kb;
    DESCR_t qa = rk_qh_coerce(a, &ka), qb = rk_qh_coerce(b, &kb);
    int force = !strcmp(op, "sum") || !strcmp(op, "mul");
    int rk = (rk_qh_mixy(ka) || rk_qh_mixy(kb)) ? 4 : (force || rk_qh_baggy(ka) || rk_qh_baggy(kb)) ? 2 : 0;
    DESCR_t r = rk_qh_new(rk);
    int na = rk_qh_n(qa), nb = rk_qh_n(qb);
    int setres = rk_qh_setty(rk);
    if (!strcmp(op, "uni")) {
        for (int i = 0; i < na; i++) rk_qh_set(r, rk_qh_elem(qa, i), rk_qh_wat(qa, i));
        for (int i = 0; i < nb; i++) { int pos = rk_qh_find(r, rk_qh_elem(qb, i)); double w = rk_qh_wat(qb, i); if (pos < 0 || w > rk_qh_wat(r, pos)) rk_qh_set(r, rk_qh_elem(qb, i), w); }
        return r;
    }
    if (!strcmp(op, "int")) {
        for (int i = 0; i < na; i++) {
            int pos = rk_qh_find(qb, rk_qh_elem(qa, i));
            if (pos < 0) continue;
            double wa = rk_qh_wat(qa, i), wb = rk_qh_wat(qb, pos);
            rk_qh_set(r, rk_qh_elem(qa, i), wa < wb ? wa : wb);
        }
        return r;
    }
    if (!strcmp(op, "dif")) {
        for (int i = 0; i < na; i++) {
            int pos = rk_qh_find(qb, rk_qh_elem(qa, i));
            double wa = rk_qh_wat(qa, i), wb = pos >= 0 ? rk_qh_wat(qb, pos) : 0.0;
            if (setres) { if (pos < 0) rk_qh_set(r, rk_qh_elem(qa, i), 1.0); } else if (wa - wb > 0) rk_qh_set(r, rk_qh_elem(qa, i), wa - wb);
        }
        return r;
    }
    if (!strcmp(op, "sym")) {
        for (int i = 0; i < na; i++) {
            int pos = rk_qh_find(qb, rk_qh_elem(qa, i));
            double d = fabs(rk_qh_wat(qa, i) - (pos >= 0 ? rk_qh_wat(qb, pos) : 0.0));
            if (setres ? pos < 0 : d != 0.0) rk_qh_set(r, rk_qh_elem(qa, i), setres ? 1.0 : d);
        }
        for (int i = 0; i < nb; i++) if (rk_qh_find(qa, rk_qh_elem(qb, i)) < 0) rk_qh_set(r, rk_qh_elem(qb, i), setres ? 1.0 : rk_qh_wat(qb, i));
        return r;
    }
    if (!strcmp(op, "sum")) {
        for (int i = 0; i < na; i++) rk_qh_set(r, rk_qh_elem(qa, i), rk_qh_wat(qa, i));
        for (int i = 0; i < nb; i++) rk_qh_bump(r, rk_qh_elem(qb, i), rk_qh_wat(qb, i));
        return r;
    }
    if (!strcmp(op, "mul")) {
        for (int i = 0; i < na; i++) { int pos = rk_qh_find(qb, rk_qh_elem(qa, i)); if (pos >= 0) rk_qh_set(r, rk_qh_elem(qa, i), rk_qh_wat(qa, i) * rk_qh_wat(qb, pos)); }
        return r;
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_subset(DESCR_t a, DESCR_t b, int baggy) {
    int ka, kb;
    DESCR_t qa = rk_qh_coerce(a, &ka), qb = rk_qh_coerce(b, &kb);
    int na = rk_qh_n(qa);
    for (int i = 0; i < na; i++) { int pos = rk_qh_find(qb, rk_qh_elem(qa, i)); if (pos < 0) return 0; if (baggy && rk_qh_wat(qa, i) > rk_qh_wat(qb, pos)) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_same(DESCR_t a, DESCR_t b) {
    int na = rk_qh_n(a);
    if (na != rk_qh_n(b)) return 0;
    for (int i = 0; i < na; i++) { int pos = rk_qh_find(b, rk_qh_elem(a, i)); if (pos < 0 || rk_qh_wat(a, i) != rk_qh_wat(b, pos)) return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_accepts(DESCR_t q, DESCR_t o) { int st = rk_qh_setty(rk_qh_kind(q)); if (!rk_qh_is(o) || rk_qh_setty(rk_qh_kind(o)) != st) o = rk_qh_from(o, st ? 0 : 2, 1); return rk_qh_same(q, o); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_pairs(DESCR_t q) {
    int n = rk_qh_n(q), kind = rk_qh_kind(q);
    DESCR_t *el = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
    for (int i = 0; i < n; i++) el[i] = rk_mk_pair(rk_qh_elem(q, i), rk_qh_setty(kind) ? rk_qh_true() : rk_qh_wt(q, i));
    return rk_mk_arr(el, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_qh_get(DESCR_t q, DESCR_t k) { int pos = rk_qh_find(q, k); return rk_qh_setty(rk_qh_kind(q)) ? rk_qh_bool(pos >= 0) : pos >= 0 ? rk_qh_wt(q, pos) : INTVAL(0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_qh_put(DESCR_t q, DESCR_t k, DESCR_t v) {
    int kind = rk_qh_kind(q);
    if (!rk_qh_mutable(kind)) {
        if (rk_qh_setty(kind)) rk_qh_refuse("Cannot modify an immutable %s (%s)", rk_qh_names[kind], rk_qh_render(q, 1));
        else rk_qh_refuse("Cannot modify an immutable %s (%s)", "Int", rk_cstr(rk_qh_get(q, k)));
        return q;
    }
    rk_qh_set(q, k, rk_qh_setty(kind) ? (rk_is_truthy(v) ? 1.0 : 0.0) : IS_REAL_fn(v) ? v.r : IS_INT_fn(v) || v.v == DT_BOOL ? (double) v.i : to_real(v));
    return q;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_qh_del(DESCR_t q, DESCR_t k, DESCR_t *old) {
    int kind = rk_qh_kind(q);
    if (!rk_qh_mutable(kind)) { rk_qh_refuse("Cannot call 'DELETE-KEY' on an immutable '%s'%s", rk_qh_names[kind], ""); *old = FAILDESCR; return 1; }
    *old = rk_qh_get(q, k);
    int pos = rk_qh_find(q, k);
    if (pos >= 0) rk_qh_drop(q, pos);
    return pos >= 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_qh_render(DESCR_t q, int gist) {
    int kind = rk_qh_kind(q), n = rk_qh_n(q);
    size_t cap = 64;
    for (int i = 0; i < n; i++) cap += strlen(rk_gist_str(rk_qh_elem(q, i))) + strlen(rk_cstr(rk_qh_wt(q, i))) + 8;
    char *o = rt_wsb_alloc(cap);
    size_t k = 0;
    if (!gist) {
        for (int i = 0; i < n; i++) {
            if (i) o[k++] = ' ';
            k += (size_t) sprintf(o + k, "%s", rk_cstr(rk_qh_elem(q, i)));
            if (!rk_qh_setty(kind) && rk_qh_wat(q, i) != 1.0) k += (size_t) sprintf(o + k, "(%s)", rk_cstr(rk_qh_wt(q, i)));
        }
        o[k] = '\0';
        return o;
    }
    k += (size_t) sprintf(o + k, "%s(", rk_qh_names[kind]);
    for (int i = 0; i < n; i++) {
        if (i) o[k++] = ' ';
        k += (size_t) sprintf(o + k, "%s", rk_gist_str(rk_qh_elem(q, i)));
        if (!rk_qh_setty(kind) && rk_qh_wat(q, i) != 1.0) k += (size_t) sprintf(o + k, "(%s)", rk_cstr(rk_qh_wt(q, i)));
    }
    o[k++] = ')';
    o[k] = '\0';
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_qh_raku(DESCR_t q) {
    int kind = rk_qh_kind(q), n = rk_qh_n(q);
    size_t cap = 64;
    for (int i = 0; i < n; i++) cap += strlen(rk_raku_str(rk_qh_elem(q, i), 0)) + strlen(rk_cstr(rk_qh_wt(q, i))) + 8;
    char *o = rt_wsb_alloc(cap);
    size_t k = 0;
    if (rk_qh_setty(kind)) {
        k += (size_t) sprintf(o + k, "%s.new", rk_qh_names[kind]);
        if (n) o[k++] = '(';
        for (int i = 0; i < n; i++) { if (i) o[k++] = ','; k += (size_t) sprintf(o + k, "%s", rk_raku_str(rk_qh_elem(q, i), 0)); }
        if (n) o[k++] = ')';
        o[k] = '\0';
        return o;
    }
    o[k++] = '(';
    for (int i = 0; i < n; i++) { if (i) o[k++] = ','; k += (size_t) sprintf(o + k, "%s=>%s", rk_raku_str(rk_qh_elem(q, i), 0), rk_cstr(rk_qh_wt(q, i))); }
    k += (size_t) sprintf(o + k, ")%s.%s", n == 0 ? "" : "", rk_qh_names[kind]);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_qh_draw(DESCR_t q, const char *m, DESCR_t *a, int na) {
    int kind = rk_qh_kind(q), n = rk_qh_n(q), roll = m[0] == 'r', grab = m[0] == 'g';
    double w[n + 1], tot = 0;
    for (int i = 0; i < n; i++) { w[i] = rk_qh_wat(q, i); tot += w[i]; }
    long want = na == 0 ? 1 : IS_INT_fn(a[0]) ? (long) a[0].i : (roll ? 64 : (long) (tot + 1));
    long cap = roll ? (want > 1048576 ? 1048576 : want) : (want > (long) tot + 1 ? (long) tot + 1 : want);
    if (cap < 0) cap = 0;
    DESCR_t *el = (DESCR_t *) rt_ws_alloc_descr((size_t) (cap + 1));
    long c = 0;
    while (c < want && c < cap && tot > 0) {
        double r = (double) (random() % 1000003) / 1000003.0 * tot, acc = 0;
        int pick = n - 1;
        for (int i = 0; i < n; i++) { if (w[i] <= 0) continue; acc += w[i]; if (r < acc) { pick = i; break; } }
        el[c++] = rk_qh_elem(q, pick);
        if (!roll) { double take = w[pick] < 1.0 ? w[pick] : 1.0; w[pick] -= take; tot -= take; }
    }
    if (grab) for (long i = 0; i < c; i++) { int pos = rk_qh_find(q, el[i]); if (pos >= 0) rk_qh_set(q, el[i], rk_qh_wat(q, pos) - 1.0); }
    (void) kind;
    if (na == 0) return c ? el[0] : rk_mk_arr(NULL, 0);
    return rk_mk_arr(el, (int) c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t q = args[0];
    int kind = rk_qh_kind(q), n = rk_qh_n(q);
    DESCR_t *a = args + 2;
    int na = nargs - 2;
    if (!strcmp(m, "elems")) { *out = INTVAL(n); return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so")) { *out = rk_qh_bool(n > 0); return 1; }
    if (!strcmp(m, "not")) { *out = rk_qh_bool(n == 0); return 1; }
    if (!strcmp(m, "defined")) { *out = rk_qh_true(); return 1; }
    if (!strcmp(m, "keys")) { DESCR_t *el = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL; for (int i = 0; i < n; i++) el[i] = rk_qh_elem(q, i); *out = rk_mk_arr(el, n); return 1; }
    if (!strcmp(m, "values")) {
        DESCR_t *el = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
        for (int i = 0; i < n; i++) el[i] = rk_qh_setty(kind) ? rk_qh_true() : rk_qh_wt(q, i);
        *out = rk_mk_arr(el, n);
        return 1;
    }
    if (!strcmp(m, "pairs") || !strcmp(m, "list")) { *out = rk_qh_pairs(q); return 1; }
    if (!strcmp(m, "kv")) {
        DESCR_t *el = (DESCR_t *) rt_ws_alloc_descr((size_t) (2 * n + 1));
        for (int i = 0; i < n; i++) { el[2 * i] = rk_qh_elem(q, i); el[2 * i + 1] = rk_qh_setty(kind) ? rk_qh_true() : rk_qh_wt(q, i); }
        *out = rk_mk_arr(el, 2 * n);
        return 1;
    }
    if (!strcmp(m, "antipairs")) {
        DESCR_t *el = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
        for (int i = 0; i < n; i++) el[i] = rk_mk_pair(rk_qh_setty(kind) ? rk_qh_true() : rk_qh_wt(q, i), rk_qh_elem(q, i));
        *out = rk_mk_arr(el, n);
        return 1;
    }
    if (!strcmp(m, "total") || !strcmp(m, "Int") || !strcmp(m, "Numeric") || !strcmp(m, "Real")) { double t = 0; for (int i = 0; i < n; i++) t += rk_qh_wat(q, i); *out = rk_qh_num(t); return 1; }
    if (!strcmp(m, "gist")) { *out = STRVAL(rt_heap_strdup_c(rk_qh_render(q, 1))); return 1; }
    if (!strcmp(m, "Str")) { *out = STRVAL(rt_heap_strdup_c(rk_qh_render(q, 0))); return 1; }
    if (!strcmp(m, "raku") || !strcmp(m, "perl")) { *out = STRVAL(rt_heap_strdup_c(rk_qh_raku(q))); return 1; }
    if (!strcmp(m, "hash") || !strcmp(m, "Hash")) {
        DESCR_t h = TABLE_VAL(table_new());
        for (int i = 0; i < n; i++) h = rk_hash_store(h, rk_qh_elem(q, i), rk_qh_setty(kind) ? rk_qh_true() : rk_qh_wt(q, i));
        *out = h;
        return 1;
    }
    for (int i = 0; rk_qh_names[i]; i++) if (!strcmp(m, rk_qh_names[i])) { *out = (i == kind && !rk_qh_mutable(kind)) ? q : rk_qh_from(q, i, 1); return 1; }
    if (!strcmp(m, "ACCEPTS") && na >= 1) { *out = rk_qh_bool(rk_qh_accepts(q, a[0])); return 1; }
    if (!strcmp(m, "grab") || !strcmp(m, "pick") || !strcmp(m, "roll")) {
        if (m[0] == 'g' && !rk_qh_mutable(kind)) { rk_qh_refuse("Cannot call 'grab' on an immutable '%s'%s", rk_qh_names[kind], ""); *out = FAILDESCR; return 1; }
        *out = rk_qh_draw(q, m, a, na);
        return 1;
    }
    if (!strcmp(m, "push") || !strcmp(m, "append")) { if (!rk_qh_mutable(kind)) return 0; for (int i = 0; i < na; i++) rk_qh_add(q, a[i], 1.0, 0); *out = q; return 1; }
    {
        DESCR_t *fw = (DESCR_t *) rt_ws_alloc_descr((size_t) nargs);
        fw[0] = rk_qh_pairs(q);
        for (int i = 1; i < nargs; i++) fw[i] = args[i];
        if (script_try_call_builtin_by_name("meth_call", fw, nargs, out)) return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_qh_hook(const char *m, DESCR_t *args, int nargs, DESCR_t *out) {
    if (rk_qh_is(args[0])) return rk_qh_method(m, args, nargs, out);
    if (!strncmp(m, "__rk_new_", 9)) { for (int i = 0; rk_qh_names[i]; i++) if (!strcmp(m + 9, rk_qh_names[i])) { rk_qh_init(); *out = rk_qh_from(args[0], i, 0); return 1; } }
    for (int i = 0; rk_qh_names[i]; i++) {
        if (!strcmp(m, rk_qh_names[i]) && nargs == 2 && !rk_typeobj_name(args[0])) { rk_qh_init(); *out = rk_qh_from(args[0], i, 1); return 1; }
        if (!strcmp(m, "new")) {
            const char *cn = rk_typeobj_name(args[0]);
            if (!cn && IS_STR_fn(args[0])) cn = VARVAL_fn(args[0]);
            if (cn && !strcmp(cn, rk_qh_names[i]) && rk_qh_own_type(cn)) { rk_qh_init(); DESCR_t q = rk_qh_new(i); for (int j = 2; j < nargs; j++) rk_qh_add(q, args[j], 1.0, 0); *out = q; return 1; }
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_qh_bg(DESCR_t x, DESCR_t y) { return (rk_qh_is(x) && !rk_qh_setty(rk_qh_kind(x))) || (rk_qh_is(y) && !rk_qh_setty(rk_qh_kind(y))); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_qh_fn(const char *fn, DESCR_t *a, int n, DESCR_t *out) {
    if (!strncmp(fn, "__rk_set_", 9) && n == 2) {
        const char *op = fn + 9;
        if (!strcmp(op, "uni") || !strcmp(op, "int") || !strcmp(op, "dif") || !strcmp(op, "sym") || !strcmp(op, "sum") || !strcmp(op, "mul")) { *out = rk_qh_binop(op, a[0], a[1]); return 1; }
        if (!strcmp(op, "elem") || !strcmp(op, "nelem") || !strcmp(op, "cont") || !strcmp(op, "ncont")) {
            int el = op[0] == 'e' || op[1] == 'e', rev = !el;
            DESCR_t e = rev ? a[1] : a[0], s = rev ? a[0] : a[1];
            int ks;
            DESCR_t qs = rk_qh_coerce(s, &ks);
            int hit = rk_qh_find(qs, e) >= 0;
            if (op[0] == 'n') hit = !hit;
            *out = rk_qh_bool(hit);
            return 1;
        }
        if (!strcmp(op, "sub") || !strcmp(op, "nsub")) { int r = rk_qh_subset(a[0], a[1], rk_qh_bg(a[0], a[1])); *out = rk_qh_bool(op[0] == 'n' ? !r : r); return 1; }
        if (!strcmp(op, "sup") || !strcmp(op, "nsup")) { int r = rk_qh_subset(a[1], a[0], rk_qh_bg(a[0], a[1])); *out = rk_qh_bool(op[0] == 'n' ? !r : r); return 1; }
        if (!strcmp(op, "psub") || !strcmp(op, "npsub")) {
            int ka, kb;
            DESCR_t x = rk_qh_coerce(a[0], &ka), y = rk_qh_coerce(a[1], &kb);
            int r = rk_qh_subset(x, y, rk_qh_bg(a[0], a[1])) && !rk_qh_same(x, y);
            *out = rk_qh_bool(op[0] == 'n' ? !r : r);
            return 1;
        }
        if (!strcmp(op, "psup") || !strcmp(op, "npsup")) {
            int ka, kb;
            DESCR_t x = rk_qh_coerce(a[0], &ka), y = rk_qh_coerce(a[1], &kb);
            int r = rk_qh_subset(y, x, rk_qh_bg(a[0], a[1])) && !rk_qh_same(x, y);
            *out = rk_qh_bool(op[0] == 'n' ? !r : r);
            return 1;
        }
        if (!strcmp(op, "eq") || !strcmp(op, "ne")) {
            int ka, kb;
            DESCR_t x = rk_qh_coerce(a[0], &ka), y = rk_qh_coerce(a[1], &kb);
            int r = rk_qh_same(x, y);
            *out = rk_qh_bool(op[0] == 'n' ? !r : r);
            return 1;
        }
    }
    if (!strncmp(fn, "__rk_bag_", 9) && n == 2) {
        if (!strcmp(fn + 9, "sub")) { *out = rk_qh_bool(rk_qh_subset(a[0], a[1], 1)); return 1; }
        if (!strcmp(fn + 9, "sup")) { *out = rk_qh_bool(rk_qh_subset(a[1], a[0], 1)); return 1; }
    }
    return 0;
}
static DESCR_t bta___rk_mkbool(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        DESCR_t a = args[0];
        long long t;
        extern int rt_is_truthy(DESCR_t v);
        if (a.v == DT_BOOL || IS_INT_fn(a)) t = (a.i != 0);
        else if (IS_REAL_fn(a)) t = (a.r != 0.0);
        else if (IS_STR_fn(a) && !rk_typeobj_name(a)) t = (a.s && a.s[0] != '\0' && a.slen != 0xFFFFFFFFu);
        else if (rk_typeobj_name(a)) t = 0;
        else if (rk_qh_is(a)) t = rk_qh_n(a) > 0;
        else if ((a.v == DT_A && a.arr) || (a.v == DT_T && a.tbl)) t = rk_agg_truth(a);
        else if (a.v == DT_DATA) t = rk_is_truthy(a) ? 1 : 0;
        else if (a.v == DT_FH) t = 1;
        else t = rt_is_truthy(a) ? 1 : 0;
        if ((_bop == 1)) t = !t;
        return (DESCR_t){ .v = DT_BOOL, .i = t };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_mkbool(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_mkbool(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_notbool(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_mkbool(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_jct_any(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) { const char *fl = (_bop == 0) ? rk_proto_jct_any : (_bop == 1) ? rk_proto_jct_all : (_bop == 2) ? rk_proto_jct_one : rk_proto_jct_none; return rk_mk_jct(fl, args, nargs); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_jct_none(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_jct_any(args, nargs, 3); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_jct_any(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_jct_any(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_jct_one(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_jct_any(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_jct_all(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_jct_any(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_slice(DESCR_t *args, int nargs) {
    if ((nargs == 3 || nargs == 4) && rk_is_seq(args[0])) { long long hi = rk_ival(args[2]); rk_seq_reify(&args[0], hi >= 0 ? (long) hi + 1 : -1L); args[0] = ((ARBLK_t *) args[0].arr)->data[3]; }
    if ((nargs == 3 || nargs == 4)) {
        rk_av_t a = rk_av(args[0]);
        int fl = nargs == 4 ? (int) args[3].i : 0;
        long long lo = rk_ival(args[1]) + (fl & 1);
        long long hi = fl & 2 ? rk_excl_hi(args[2]) : rk_ival(args[2]);
        if (lo < 0) lo = 0;
        if (hi >= a.n) hi = a.n - 1;
        return hi < lo ? rk_mk_arr(NULL, 0) : rk_mk_arr(a.el + lo, (int) (hi - lo + 1));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_pick(DESCR_t *args, int nargs) {
    if (nargs >= 2 && rk_is_seq(args[0])) {
        long mx = -1;
        for (int k = 1; k < nargs; k++) if (IS_INT_fn(args[k]) && args[k].i > mx) mx = (long) args[k].i;
        rk_seq_reify(&args[0], mx + 1);
        args[0] = ((ARBLK_t *) args[0].arr)->data[3];
    }
    if (nargs >= 2) {
        rk_av_t a = rk_av(args[0]);
        int nsel = nargs - 1;
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) nsel);
        for (int i = 1; i < nargs; i++) { long long k = rk_ival(args[i]); r[i - 1] = (k >= 0 && k < a.n) ? a.el[k] : rk_typeobj("Any"); }
        return rk_mk_arr(r, nsel);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_xx(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        long cnt = IS_INT_fn(args[1]) ? args[1].i : (IS_REAL_fn(args[1]) ? (long)args[1].r : 0);
        if (cnt < 1) { return rk_mk_arr(NULL, 0); }
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) cnt);
        for (long k = 0; k < cnt; k++) r[k] = args[0];
        return rk_mk_arr(r, (int) cnt);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_smartmatch(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        if (rk_is_jct(args[1])) { int ok = junction_collapse(args[0], args[1], TT_EQ, IS_INT_fn(args[0]) || IS_REAL_fn(args[0])); return (DESCR_t){ .v = DT_BOOL, .i = ok ? 1 : 0 }; }
        if (rk_is_range(args[1])) { return (DESCR_t){ .v = DT_BOOL, .i = rk_in_range_value(args[0], args[1]) }; }
        const char *pat = rk_rx_pat(args[1]);
        if (pat) {
            char sb[64];
            const char *subj = to_cstring(args[0], sb, sizeof sb);
            subj = rt_heap_strdup_c(subj ? subj : "");
            RxEnv renv;
            RxProg *prog = rk_rx_compile(&renv, "", pat, 0);
            if (!prog) { return FAILDESCR; }
            RxMatch rm;
            rx_exec(prog, subj, (int) strlen(subj), 0, 0, &rm);
            rk_rx_to_match(&g_match, prog, &rm);
            g_subject = subj;
            return rm.ok ? INTVAL(1) : FAILDESCR;
        }
        long long hit;
        if (args[1].v == DT_T && args[1].tbl) {
            TBBLK_t * hb = args[1].tbl;
            const char * lp = rk_rx_pat(args[0]);
            int found = 0;
            if (lp) {
                RxEnv henv;
                RxProg * hprog = rk_rx_compile(&henv, "", lp, 0);
                for (unsigned hi = 0; hprog && !found && hi < hb->ord_len; hi++) {
                    DESCR_t hk = hb->ord[hi];
                    if (hk.v == DT_RAW || !table_find_pair_d(hb, hk)) continue;
                    const char * kt = rk_cstr(hk);
                    RxMatch hm;
                    rx_exec(hprog, kt, (int) strlen(kt), 0, 0, &hm);
                    found = hm.ok;
                }
            } else if (args[0].v == DT_A && args[0].arr && !rk_is_range(args[0]) && !rk_is_pair(args[0])) {
                rk_av_t ka = rk_av(args[0]);
                for (int ki = 0; ki < ka.n && !found; ki++) {
                    char kb[64];
                    const char * ks = to_cstring(ka.el[ki], kb, sizeof kb);
                    found = table_find_pair_d(hb, ka.el[ki]) != NULL || table_find_pair_d(hb, STRVAL(rt_heap_strdup_c(ks ? ks : ""))) != NULL;
                }
            } else {
                char kb[64];
                const char * ks = to_cstring(args[0], kb, sizeof kb);
                found = table_find_pair_d(hb, args[0]) != NULL || table_find_pair_d(hb, STRVAL(rt_heap_strdup_c(ks ? ks : ""))) != NULL;
            }
            return (DESCR_t){ .v = DT_BOOL, .i = found };
        }
        if (rk_typeobj_name(args[1])) { return (DESCR_t){ .v = DT_BOOL, .i = rk_type_isa(rk_value_type(args[0]), rk_typeobj_name(args[1])) ? 1 : 0 }; }
        if (rk_qh_is(args[1])) { return rk_qh_bool(rk_qh_accepts(args[1], args[0])); }
        if (rk_is_code(args[1])) {
            rt_call_args_need(2);
            CALL_ARGS[0] = args[0];
            DESCR_t rr = rk_call_block(args[1], 1);
            return (DESCR_t){ .v = DT_BOOL, .i = (!IS_FAIL_fn(rr) && rk_is_truthy(rr)) ? 1 : 0 };
        }
        if (rk_is_pair(args[1])) {
            extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
            rk_av_t pp = rk_av(args[1]);
            DESCR_t got, sm[2];
            rk_cbh_t H = { g_rk_cbh_cur, { pp.n > 1 ? pp.el[1] : NULVCL, NULVCL } };
            int ok = 0;
            g_rk_cbh_cur = &H;
            if (rk_is_pair(args[0])) {
                rk_av_t tp = rk_av(args[0]);
                H.d[1] = tp.n > 1 ? tp.el[1] : NULVCL;
                sm[0] = tp.el[0];
                sm[1] = pp.el[0];
                ok = script_try_call_builtin_by_name("__rk_smartmatch", sm, 2, &got) && !IS_FAIL_fn(got) && rk_is_truthy(got);
                sm[0] = H.d[1];
                sm[1] = H.d[0];
                ok = ok && script_try_call_builtin_by_name("__rk_smartmatch", sm, 2, &got) && !IS_FAIL_fn(got) && rk_is_truthy(got);
                g_rk_cbh_cur = H.prev;
                return (DESCR_t){ .v = DT_BOOL, .i = ok };
            }
            const char * pk = rk_cstr(pp.el[0]);
            char key[strlen(pk) + 1];
            strcpy(key, pk);
            DESCR_t ma[2] = { args[0], STRVAL(key) };
            ok = script_try_call_builtin_by_name("meth_call", ma, 2, &got) && !IS_FAIL_fn(got);
            g_rk_cbh_cur = H.prev;
            return (DESCR_t){ .v = DT_BOOL, .i = ok && (rk_is_truthy(got) != 0) == (rk_is_truthy(H.d[0]) != 0) };
        }
        int sm_c = 0;
        extern int rk_rat_cmp(DESCR_t, DESCR_t, int *);
        double sm_r, sm_l;
        if (rk_is_range(args[0]) && args[1].v != DT_BOOL && rk_num_dbl(args[1], &sm_r) && rk_num_dbl(rk_range_numeric(args[0]), &sm_l)) {
            return (DESCR_t){ .v = DT_BOOL, .i = sm_r != sm_r ? sm_l != sm_l : sm_l == sm_r };
        }
        if (args[1].v == DT_BOOL) hit = args[1].i != 0;
        else if (rk_rat_cmp(args[0], args[1], &sm_c)) hit = sm_c == 0;
        else if ((IS_INT_fn(args[1]) || IS_REAL_fn(args[1])) && rk_is_code(args[0])) hit = 0;
        else if (IS_INT_fn(args[1]) || IS_REAL_fn(args[1])) hit = to_real(args[0]) == to_real(args[1]) || (isnan(to_real(args[0])) && isnan(to_real(args[1])));
        else if (IS_DATA_INST_fn(args[0]) && IS_DATA_INST_fn(args[1])) {
            DESCR_t smi;
            hit = script_try_call_builtin_by_name("__rk_ident", args, 2, &smi) ? (!IS_FAIL_fn(smi) && rk_is_truthy(smi)) : 0;
        } else {
            char s1[64], s2[64];
            const char *x = to_cstring(args[0], s1, sizeof s1), *y = to_cstring(args[1], s2, sizeof s2);
            hit = !strcmp(x ? x : "", y ? y : "");
        }
        return (DESCR_t){ .v = DT_BOOL, .i = hit };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_where(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        extern void fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { return FAILDESCR; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { return FAILDESCR; }
        long pos = ftell(fp);
        if (pos < 0) { return FAILDESCR; }
        return INTVAL(pos + 1);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_rk_seek(DESCR_t *args, int nargs) {
    if ((nargs == 1 || nargs == 2)) {
        extern void fh_ensure_init(void);
        extern FILE *fh_get(int);
        if (!IS_INT_fn(args[0]) && !IS_FH_fn(args[0])) { return FAILDESCR; }
        fh_ensure_init();
        FILE *fp = fh_get((int)args[0].i);
        if (!fp) { return FAILDESCR; }
        long o = 1L;
        if (nargs == 2 && args[1].v != DT_SNUL && args[1].v != 0) { extern int64_t core_icn_to_int_d(DESCR_t); o = IS_INT_fn(args[1]) ? (long)args[1].i : (long)core_icn_to_int_d(args[1]); }
        int rc = (o > 0) ? fseek(fp, o - 1, SEEK_SET) : fseek(fp, o, SEEK_END);
        if (rc != 0) { return FAILDESCR; }
        return args[0];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_ident(DESCR_t *args, int nargs, int _bop) {
    static const char *const _bn[] = { "__rk_ident", "__rk_eqv" };
    const char *const fn = _bn[_bop];
    if (nargs == 2 && (rk_is_jct(args[0]) || rk_is_jct(args[1]))) {
        int j = rk_is_jct(args[0]) ? 0 : 1;
        rk_av_t a = rk_av(args[j]);
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) (a.n ? a.n : 1));
        for (int k = 0; k < a.n; k++) { DESCR_t pa[2] = { args[0], args[1] }; pa[j] = a.el[k]; if (!script_try_call_builtin_by_name(fn, pa, 2, &r[k])) r[k] = (DESCR_t){ .v = DT_BOOL, .i = 0 }; }
        return rk_mk_jct(((ARBLK_t *) args[j].arr)->proto, r, a.n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_ident_2(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2 && rk_qh_is(args[0]) && rk_qh_is(args[1])) {
        int k0 = rk_qh_kind(args[0]), k1 = rk_qh_kind(args[1]);
        return (DESCR_t){ .v = DT_BOOL, .i = (args[0].u == args[1].u) || (k0 == k1 && ((_bop == 1) || k0 % 2 == 0) && rk_qh_same(args[0], args[1])) };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_eqv(DESCR_t *args, int nargs) {
    { const DESCR_t _r = bta___rk_ident(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; }
    { const DESCR_t _r = bta___rk_ident_2(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; }
    if (nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        long long t;
        int an = IS_INT_fn(a) || IS_REAL_fn(a), bn = IS_INT_fn(b) || IS_REAL_fn(b);
        if (an != bn) t = 0;
        else if (an) t = IS_INT_fn(a) && IS_INT_fn(b) ? a.i == b.i : IS_REAL_fn(a) && IS_REAL_fn(b) ? (isnan(a.r) && isnan(b.r)) || (a.r == b.r && signbit(a.r) == signbit(b.r)) : 0;
        else if (a.v == DT_BOOL || b.v == DT_BOOL) t = a.v == b.v && a.i == b.i;
        else if (a.v == DT_A || b.v == DT_A) {
            rk_av_t x = rk_av(a), y = rk_av(b);
            t = x.n == y.n;
            for (int i = 0; t && i < x.n; i++) t = !strcmp(rk_av_text(x, i), rk_av_text(y, i));
        } else {
            const char *x = rk_cstr(a), *y = rk_cstr(b);
            t = !strcmp(x, y);
        }
        return (DESCR_t){ .v = DT_BOOL, .i = t };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_ident(DESCR_t *args, int nargs) {
    { const DESCR_t _r = bta___rk_ident(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; }
    { const DESCR_t _r = bta___rk_ident_2(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; }
    if (nargs == 2 && IS_DATA_INST_fn(args[0]) && IS_DATA_INST_fn(args[1])) {
        extern int rk_rat_is(DESCR_t);
        extern int rk_rat_cmp(DESCR_t, DESCR_t, int *);
        int rc;
        if (rk_rat_is(args[0]) && rk_rat_is(args[1]) && args[0].u->type == args[1].u->type && rk_rat_cmp(args[0], args[1], &rc)) { return (DESCR_t){ .v = DT_BOOL, .i = rc == 0 }; }
        return (DESCR_t){ .v = DT_BOOL, .i = args[0].u == args[1].u };
    }
    if (nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        long long t;
        int an = IS_INT_fn(a) || IS_REAL_fn(a), bn = IS_INT_fn(b) || IS_REAL_fn(b);
        if (an != bn) t = 0;
        else if (an) t = IS_INT_fn(a) && IS_INT_fn(b) ? a.i == b.i : IS_REAL_fn(a) && IS_REAL_fn(b) ? (isnan(a.r) && isnan(b.r)) || (a.r == b.r && signbit(a.r) == signbit(b.r)) : 0;
        else if (a.v == DT_BOOL || b.v == DT_BOOL) t = a.v == b.v && a.i == b.i;
        else if (a.v == DT_A || b.v == DT_A) {
            rk_av_t x = rk_av(a), y = rk_av(b);
            t = x.n == y.n;
            for (int i = 0; t && i < x.n; i++) t = !strcmp(rk_av_text(x, i), rk_av_text(y, i));
        } else {
            const char *x = rk_cstr(a), *y = rk_cstr(b);
            t = !strcmp(x, y);
        }
        return (DESCR_t){ .v = DT_BOOL, .i = t };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_note(DESCR_t *args, int nargs) {
    const char *const fn = "note";
    if (!rt_proc_is_registered(fn)) {
        DESCR_t *tmp = (DESCR_t *)rt_ws_alloc_descr((size_t)(nargs > 0 ? nargs : 1));
        for (int _ri = 0; _ri < nargs; _ri++) { if (IS_REAL_fn(args[_ri])) { char *_rb = rt_wsb_alloc(64); rk_real_str(args[_ri].r, _rb, 64); tmp[_ri] = STRVAL(_rb); } else tmp[_ri] = args[_ri]; }
        for (int _ri = 0; _ri < nargs; _ri++) { char _sb[512]; const char *_cs = to_cstring(tmp[_ri], _sb, sizeof _sb); fputs(_cs ? _cs : "", stderr); }
        fputc('\n', stderr);
        fflush(stderr);
        return INTVAL(1);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
