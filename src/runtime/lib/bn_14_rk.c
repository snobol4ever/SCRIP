#include "runtime/lib/bn_head.h"
#include "ir/rk_opname.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "runtime/builtin_leaf.h"
#include "runtime/rt/rt_list_view.h"
#include <ctype.h>
#include "runtime/lib/bn_02_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_18_rk.h"
#include "runtime/lib/bn_19_rk.h"
extern void *rt_ws_alloc_descr(size_t);
int rk_is_seq(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_seq; }
int rk_seq_is_lazy(DESCR_t v) { return rk_is_seq(v) && (((ARBLK_t *) v.arr)->proto_bare & RK_SEQ_LAZY); }
int rk_is_range(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_range; }
int rk_is_list(DESCR_t v) {
    return v.v == DT_A && v.arr && (((ARBLK_t *) v.arr)->proto == rk_proto_list || ((ARBLK_t *) v.arr)->proto == rk_proto_range || ((ARBLK_t *) v.arr)->proto == rk_proto_seq);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_mark_list(DESCR_t d) { if (d.v == DT_A && d.arr) ((ARBLK_t *) d.arr)->proto = rk_proto_list; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char rk_proto_methval[] = "Method";
const char rk_proto_attrval[] = "Attribute";
int rk_is_methval(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_methval; }
int rk_is_attrval(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_attrval; }
const char rk_proto_sigval[] = "Signature";
const char rk_proto_paramval[] = "Parameter";
int rk_is_sigval(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_sigval; }
int rk_is_paramval(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_paramval; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_unmark(DESCR_t d) { if (d.v == DT_A && d.arr) ((ARBLK_t *) d.arr)->proto = (const char *) 0; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_arr_put(DESCR_t a, long idx, DESCR_t v, int through) {
    if (a.v != DT_A || !a.arr) return 0;
    ARBLK_t *b = (ARBLK_t *) a.arr;
    if (b->proto || b->ndim != 1 || b->lo != 0) return 0;
    long n = b->hi + 1 > 0 ? b->hi + 1 : 0;
    if (through && (b->proto_bare & RK_ARR_BOUND) && idx >= 0 && idx < n && rk_is_cell(b->data[idx])) { *(DESCR_t *) b->data[idx].ptr = v; return 1; }
    if (idx >= n) { DESCR_t *d = (DESCR_t *) rt_ws_alloc_descr((size_t) (idx + 1)); for (long i = 0; i < idx; i++) d[i] = i < n ? b->data[i] : rk_typeobj("Any"); b->data = d; b->hi = (int) idx; }
    b->data[idx] = v;
    return 1;
}
static int rk_arr_store_in_place(DESCR_t a, long idx, DESCR_t v) { return rk_arr_put(a, idx, v, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char rk_proto_pair[] = "Pair";
const char rk_proto_jct_any[] = "any";
const char rk_proto_jct_all[] = "all";
const char rk_proto_jct_one[] = "one";
const char rk_proto_jct_none[] = "none";
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_pair(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_pair; }
int rk_hy_seq(DESCR_t v) { return v.v == DT_A && v.arr && !rk_is_pair(v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_jct(DESCR_t v) {
    if (!(v.v == DT_A && v.arr)) return 0;
    const char *p = ((ARBLK_t *) v.arr)->proto;
    return p == rk_proto_jct_any || p == rk_proto_jct_all || p == rk_proto_jct_one || p == rk_proto_jct_none;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_reify_deep(DESCR_t v, int depth) {
    if (v.v != DT_A || !v.arr || depth > 64 || rk_seq_is_lazy(v)) return;
    rk_cbh_t H = { g_rk_cbh_cur, { v, NULVCL } };
    g_rk_cbh_cur = &H;
    if (rk_is_seq(H.d[0])) rk_seq_reify(&H.d[0], -1L);
    for (long i = 0; ; i++) {
        DESCR_t a = rk_is_seq(H.d[0]) ? ((ARBLK_t *) H.d[0].arr)->data[3] : H.d[0];
        if (a.v != DT_A || !a.arr || rk_is_jct(a) || rk_is_range(a)) break;
        ARBLK_t *b = (ARBLK_t *) a.arr;
        if (i > (long) b->hi - b->lo) break;
        DESCR_t e = b->data[i];
        if (e.v == DT_A && e.arr) rk_reify_deep(e, depth + 1);
    }
    g_rk_cbh_cur = H.prev;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_arr_text(DESCR_t v) { rk_reify_deep(v, 0); if (rk_is_pair(v)) return rk_pair_text(v); if (rk_is_jct(v)) return rk_jct_text(v); return rk_av_joined(rk_av(v), " ", "", ""); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_paren(const char *s) { size_t n = strlen(s); char *r = rt_wsb_alloc(n + 3); r[0] = '('; memcpy(r + 1, s, n); r[n + 1] = ')'; r[n + 2] = '\0'; return r; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_gist_str(DESCR_t v) {
    if (rk_seq_is_lazy(v)) return "(...)";
    rk_reify_deep(v, 0);
    if (rk_is_methval(v)) return rk_cstr(rk_av(v).el[0]);
    if (rk_is_attrval(v)) { rk_av_t a = rk_av(v); const char *t = rk_cstr(a.el[0]), *nm = rk_cstr(a.el[1]); char *r = rt_wsb_alloc(strlen(t) + strlen(nm) + 2); sprintf(r, "%s %s", t, nm); return r; }
    { const char *tn = rk_typeobj_name(v); if (tn) { char *r = rt_wsb_alloc(strlen(tn) + 3); snprintf(r, strlen(tn) + 3, "(%s)", tn); return r; } }
    if (v.v == DT_T && v.tbl) {
        TBBLK_t *t = v.tbl;
        const char **tx = t->size ? (const char **) rt_pvec_alloc((size_t) t->size) : NULL;
        int c = 0;
        for (unsigned i = 0; i < t->ord_len; i++) {
            DESCR_t key = t->ord[i];
            if (key.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(t, key);
            if (!e) continue;
            const char *kt = rk_cstr(key), *vt = rk_gist_str(e->val);
            size_t kl = strlen(kt), vl = strlen(vt);
            char *ln = rt_wsb_alloc(kl + vl + 5);
            memcpy(ln, kt, kl);
            memcpy(ln + kl, " => ", 4);
            memcpy(ln + kl + 4, vt, vl);
            ln[kl + 4 + vl] = '\0';
            tx[c++] = ln;
        }
        return rk_join_gists(tx, c, ", ", "{", "}");
    }
    if (rk_is_pair(v)) {
        rk_av_t a = rk_av(v);
        const char *kt = rk_is_pair(a.el[0]) ? rk_paren(rk_gist_str(a.el[0])) : rk_av_text(a, 0), *vt = rk_gist_str(a.el[1]);
        size_t kl = strlen(kt), vl = strlen(vt);
        char *ln = rt_wsb_alloc(kl + vl + 5);
        memcpy(ln, kt, kl);
        memcpy(ln + kl, " => ", 4);
        memcpy(ln + kl + 4, vt, vl);
        ln[kl + 4 + vl] = '\0';
        return ln;
    }
    if (rk_is_jct(v)) return rk_jct_text(v);
    if (rk_is_range(v)) return rk_range_text(v);
    if (v.v == DT_A && v.arr) {
        rk_av_t a = rk_av(v);
        const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) tx[i] = rk_gist_str(a.el[i]);
        int lst = rk_is_list(v) || rk_is_slip(v);
        return rk_join_gists(tx, a.n, " ", lst ? "(" : "[", lst ? ")" : "]");
    }
    if (v.v == DT_DATA) { extern const char *rk_obj_stringify(DESCR_t d, int use_gist); return rk_obj_stringify(v, 1); }
    return rk_cstr(v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_range_bounds(DESCR_t v, DESCR_t *lo, DESCR_t *hi, int *xl, int *xh) {
    ARBLK_t *b = (ARBLK_t *) v.arr;
    int fl = b->proto_bare;
    *xl = fl & 1;
    *xh = (fl >> 1) & 1;
    if (fl & 8) { int n = b->hi - b->lo + 1; *lo = b->data[n]; *hi = b->data[n + 1]; return; }
    rk_av_t a = rk_av(v);
    *lo = a.n ? a.el[0] : NULVCL;
    *hi = a.n ? a.el[a.n - 1] : NULVCL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_range_text(DESCR_t v) {
    DESCR_t lo, hi;
    int xl, xh;
    rk_range_bounds(v, &lo, &hi, &xl, &xh);
    const char *ls = rk_raku_str(lo, 0), *hs = rk_raku_str(hi, 0);
    char *r = (char *) rt_wsb_alloc(strlen(ls) + strlen(hs) + 8);
    if (IS_INT_fn(lo) && IS_INT_fn(hi) && !lo.i && !xl && xh) sprintf(r, "^%s", hs);
    else sprintf(r, "%s%s..%s%s", ls, xl ? "^" : "", xh ? "^" : "", hs);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_typeobj(const char *nm) { size_t l = strlen(nm ? nm : ""); char *r = (char *)rt_str_alloc(l + 1); r[0] = RK_TY; memcpy(r + 1, nm ? nm : "", l); r[l + 1] = '\0'; return STRVAL(r); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_range_num(DESCR_t v, double *d, int *isint) {
    if (IS_INT_fn(v)) { *d = (double) v.i; *isint = 1; return 1; }
    if (IS_REAL_fn(v)) { *d = v.r; *isint = 0; return 1; }
    if (v.v == DT_BIG) { *d = strtod(rk_cstr(v), NULL); *isint = 1; return 1; }
    if (v.v == DT_DATA) { extern int rk_rat_to_real(DESCR_t, double *); if (!rk_rat_to_real(v, d)) return 0; *isint = 0; return 1; }
    if (!IS_STR_fn(v) || rk_typeobj_name(v)) return 0;
    const char *t = rk_cstr(v);
    char *e;
    if (!*t || isspace((unsigned char) *t)) return 0;
    long long iv = strtoll(t, &e, 10);
    if (!*e) { *d = (double) iv; *isint = 1; return 1; }
    double dv = strtod(t, &e);
    if (!*e) { *d = dv; *isint = 0; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_range_end(DESCR_t v) { return v.v == DT_A && v.arr && !rk_is_range(v) ? INTVAL(rk_av(v).n) : v; }
DESCR_t rk_range_arr_make_fl(DESCR_t a, DESCR_t b, int fl) {
    double dl, dh;
    int il, ih, xl = fl & 1, xh = (fl >> 1) & 1;
    a = rk_range_end(a);
    b = rk_range_end(b);
    if (rk_range_num(a, &dl, &il) && rk_range_num(b, &dh, &ih)) {
        if (dh != dh || dh > 4e18 || dl != dl || dl < -4e18) return rk_mk_arr(NULL, 0);
        double st = dl + xl;
        if (dh < st || (xh && dh == st)) return rk_mk_arr(NULL, 0);
        long long cnt = (long long) floor(dh - st) + 1;
        if (xh && st + (double) (cnt - 1) == dh) cnt--;
        ARBLK_t *ra = array_new(0, (int) (cnt - 1));
        for (long long i = 0; i < cnt; i++) ra->data[i] = il ? INTVAL((long long) st + i) : REALVAL(st + (double) i);
        DESCR_t d = {0};
        d.v = DT_A;
        d.slen = 0;
        d.arr = ra;
        return d;
    }
    const char *lo = rk_cstr(a), *hi = rk_cstr(b);
    size_t hl = strlen(hi), ll = strlen(lo);
    if (strcmp(lo, hi) > 0) return rk_mk_arr(NULL, 0);
    char cur[(ll > hl ? ll : hl) + 4], nxt[(ll > hl ? ll : hl) + 4];
    long long cnt = 0;
    strcpy(cur, lo);
    for (;;) { cnt++; if (!strcmp(cur, hi) || cnt > 50000000) break; rk_str_succ_buf(cur, nxt); if (strlen(nxt) > hl) break; strcpy(cur, nxt); }
    ARBLK_t *ra = array_new(0, (int) (cnt - 1));
    {
        const char *prev = lo;
        for (long long i = 0; i < cnt; i++) { char *e = i ? rk_str_succ_buf(prev, (char *)rt_heap_alloc_c(strlen(prev) + 2)) : rt_heap_strdup_c(lo); ra->data[i] = STRVAL(e); prev = e; }
    }
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = ra;
    rk_av_t v = rk_av(d);
    int from = xl && v.n && !strcmp(rk_cstr(v.el[0]), lo) ? 1 : 0, to = v.n;
    if (xh && to > from && !strcmp(rk_cstr(v.el[to - 1]), hi)) to--;
    return from || to < v.n ? rk_mk_arr(v.el + from, to - from) : d;
}
static DESCR_t rk_range_arr_make(DESCR_t a, DESCR_t b) { return rk_range_arr_make_fl(a, b, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_range_value(DESCR_t lo, DESCR_t hi, int fl) {
    lo = rk_range_end(lo);
    hi = rk_range_end(hi);
    rk_av_t a = rk_av(rk_range_arr_make_fl(lo, hi, fl));
    ARBLK_t *ra = array_new(0, a.n + 1);
    ra->hi = a.n - 1;
    for (int i = 0; i < a.n; i++) ra->data[i] = a.el[i];
    ra->data[a.n] = lo;
    ra->data[a.n + 1] = hi;
    ra->proto = rk_proto_range;
    ra->proto_bare = (fl & 3) | 8;
    DESCR_t d = {0};
    d.v = DT_A;
    d.arr = ra;
    return d;
}
static int rk_range_accepts_range(DESCR_t lo, DESCR_t hi, int xl, int xh, DESCR_t topic) {
    DESCR_t tl, th;
    int txl, txh;
    double a, b, c, d;
    rk_range_bounds(topic, &tl, &th, &txl, &txh);
    if (rk_num_dbl(lo, &a) && rk_num_dbl(hi, &b)) {
        int ic, id;
        if (!rk_range_num(tl, &c, &ic) || !rk_range_num(th, &d, &id)) return 0;
        return (c > a || (c == a && !(!txl && xl))) && (d < b || (d == b && !(!txh && xh)));
    }
    int cl = strcmp(rk_cstr(tl), rk_cstr(lo)), ch = strcmp(rk_cstr(th), rk_cstr(hi));
    return (cl > 0 || (!cl && !(!txl && xl))) && (ch < 0 || (!ch && !(!txh && xh)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_range_method(const char *m, DESCR_t v, int nmargs, DESCR_t *out) {
    DESCR_t lo, hi;
    int xl, xh;
    rk_range_bounds(v, &lo, &hi, &xl, &xh);
    if (nmargs == 0 && !strcmp(m, "min")) { *out = lo; return 1; }
    if (nmargs == 0 && !strcmp(m, "max")) { *out = hi; return 1; }
    if (nmargs == 0 && !strcmp(m, "excludes-min")) { *out = (DESCR_t){ .v = DT_BOOL, .i = xl }; return 1; }
    if (nmargs == 0 && !strcmp(m, "excludes-max")) { *out = (DESCR_t){ .v = DT_BOOL, .i = xh }; return 1; }
    if (nmargs == 0 && !strcmp(m, "bounds")) { DESCR_t b2[2] = { lo, hi }; *out = rk_mk_arr(b2, 2); return 1; }
    if (nmargs == 0 && !strcmp(m, "minmax")) { DESCR_t b2[2] = { lo, hi }; *out = rk_mk_arr(b2, 2); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_in_range_value(DESCR_t x, DESCR_t r) {
    DESCR_t lo, hi;
    int xl, xh;
    rk_range_bounds(r, &lo, &hi, &xl, &xh);
    if (rk_is_range(x)) return rk_range_accepts_range(lo, hi, xl, xh, x);
    double dx, dl, dh;
    int ix, il, ih;
    if (rk_range_num(x, &dx, &ix) && rk_range_num(lo, &dl, &il) && rk_range_num(hi, &dh, &ih)) return (xl ? dx > dl : dx >= dl) && (xh ? dx < dh : dx <= dh);
    const char *sx = rk_cstr(x), *sl = rk_cstr(lo), *sh = rk_cstr(hi);
    int c1 = strcmp(sx, sl), c2 = strcmp(sx, sh);
    return (xl ? c1 > 0 : c1 >= 0) && (xh ? c2 < 0 : c2 <= 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_in_range(DESCR_t x, DESCR_t lo, DESCR_t hi, int fl) {
    double dx, dl, dh;
    int ix, il, ih, xl = fl & 1, xh = (fl >> 1) & 1;
    if (rk_is_range(x)) return rk_range_accepts_range(lo, hi, xl, xh, x);
    if (rk_range_num(x, &dx, &ix) && rk_range_num(lo, &dl, &il) && rk_range_num(hi, &dh, &ih)) return (xl ? dx > dl : dx >= dl) && (xh ? dx < dh : dx <= dh);
    int c1 = strcmp(rk_cstr(x), rk_cstr(lo)), c2 = strcmp(rk_cstr(x), rk_cstr(hi));
    return (xl ? c1 > 0 : c1 >= 0) && (xh ? c2 < 0 : c2 <= 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
double rk_cstr_num(DESCR_t v) { double d; int i; return rk_range_num(v, &d, &i) ? d : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt_item(DESCR_t *args, int nargs) {
    if ((nargs - 1) != 0) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    DESCR_t v = args[0];
    if (!(v.v == DT_A && v.arr) || rk_is_pair(v) || rk_is_jct(v) || rk_is_seq(v) || rk_is_range(v) || rk_is_item(v)) return v;
    rk_av_t a = rk_av(v);
    DESCR_t c = rk_mk_arr(NULL, 0);
    for (int i = 0; i < a.n; i++) rk_arr_append((ARBLK_t *) c.arr, a.el[i]);
    ((ARBLK_t *) c.arr)->proto = ((ARBLK_t *) v.arr)->proto;
    ((ARBLK_t *) c.arr)->proto_bare |= RK_ARR_ITEM;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_key_tmp(DESCR_t k) { const char *t = rk_cstr(k); return STRVAL((char *) t); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_key_own(DESCR_t k) { const char *t = rk_cstr(k); return STRVAL(rt_heap_strdup_c(t)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_hash_store(DESCR_t h, DESCR_t k, DESCR_t v) {
    TBBLK_t *t = (h.v == DT_T && h.tbl) ? h.tbl : table_new();
    DESCR_t val = rk_one(v);
    TBPAIR_t *e = table_find_pair_d(t, rk_key_tmp(k));
    if (e) e->val = val;
    else table_set_descr_d(t, rk_key_own(k), val);
    return TABLE_VAL(t);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_is(DESCR_t v) { return rk_is_jct(v); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_item_agg(DESCR_t *args, int nargs) {
    int cap = 0;
    for (int i = 0; i < nargs; i++) cap += rk_item_count(args[i]);
    DESCR_t *r = cap > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) cap) : NULL;
    int k = 0;
    for (int i = 0; i < nargs; i++) k = rk_item(r, k, args[i]);
    return rk_mk_arr(r, k);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_plus_agg(DESCR_t *args, int nargs, int as_list) {
    DESCR_t r;
    if (nargs == 1 && args[0].v == DT_A && args[0].arr) { rk_av_t a = rk_av(args[0]); r = rt_make_item_agg(a.el, a.n); } else if (nargs == 1 && !as_list) r = rt_make_flat_agg(args, nargs);
    else r = rt_make_item_agg(args, nargs);
    return as_list ? rk_mark_list(r) : r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_adv_parse(DESCR_t *args, int nargs, rk_adv_t *a) {
    memset(a, 0, sizeof *a);
    for (int i = 0; i < nargs; i++) {
        if (!rk_is_pair(args[i])) continue;
        rk_av_t p = rk_av(args[i]);
        const char *k = rk_cstr(p.el[0]);
        int v = p.n > 1 ? rk_is_truthy(p.el[1]) : 1;
        if (!strcmp(k, "delete")) {
            a->dh = 1;
            a->dv = v;
        } else if (!strcmp(k, "exists")) {
            a->eh = 1;
            a->ev = v;
        } else if (!strcmp(k, "kv")) {
            a->kvh = 1;
            a->kvv = v;
            a->nmode++;
        } else if (!strcmp(k, "p")) {
            a->ph = 1;
            a->pv = v;
            a->nmode++;
        } else if (!strcmp(k, "k")) {
            a->kh = 1;
            a->kv2 = v;
            a->nmode++;
        } else if (!strcmp(k, "v")) {
            a->vh = 1;
            a->vv = v;
            a->nmode++;
        } else return 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_is_arr(DESCR_t c) { return c.v == DT_A && c.arr && !rk_is_pair(c) && !rk_is_jct(c) && ((ARBLK_t *) c.arr)->ndim == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_idx(DESCR_t key, long long *i) {
    extern void rt_script_die_surface(const char *msg);
    const char *s = rk_cstr(key);
    char *ep;
    if (IS_INT_fn(key)) *i = key.i;
    else if (s && *s) { *i = strtoll(s, &ep, 10); if (*ep) return 0; } else return 0;
    if (*i < 0) { char m[96]; snprintf(m, sizeof m, "Index out of range. Is: %lld, should be in 0..^Inf", *i); rt_script_die_surface(m); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_has(DESCR_t c, DESCR_t k) {
    long long i;
    if (c.v == DT_T && c.tbl) return table_has_d(c.tbl, rk_key_tmp(k)) ? 1 : 0;
    if (!rk_sub_is_arr(c) || !rk_sub_idx(k, &i)) return 0;
    ARBLK_t *b = (ARBLK_t *) c.arr;
    return i < b->hi - b->lo + 1 && !rk_arr_hole(b->data[i]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_sub_get(DESCR_t c, DESCR_t k) {
    long long i;
    int found = 0;
    if (c.v == DT_T && c.tbl) { DESCR_t v = table_get_found_d(c.tbl, rk_key_tmp(k), &found); return found ? v : rk_typeobj("Any"); }
    if (!rk_sub_is_arr(c) || !rk_sub_idx(k, &i)) return rk_typeobj("Any");
    ARBLK_t *b = (ARBLK_t *) c.arr;
    return i < b->hi - b->lo + 1 ? b->data[i] : rk_typeobj("Any");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_sub_del(DESCR_t c, DESCR_t k) {
    long long i;
    int found = 0;
    if (c.v == DT_T && c.tbl) { DESCR_t dk = rk_key_tmp(k), v = table_get_found_d(c.tbl, dk, &found); if (found) table_delete_d(c.tbl, dk); return found ? v : rk_typeobj("Any"); }
    if (!rk_sub_is_arr(c) || !rk_sub_idx(k, &i)) return rk_typeobj("Any");
    ARBLK_t *b = (ARBLK_t *) c.arr;
    long long n = b->hi - b->lo + 1;
    if (i >= n) return rk_typeobj("Any");
    DESCR_t old = b->data[i];
    b->data[i] = rk_typeobj("Any");
    while (n > 0 && rk_arr_hole(b->data[n - 1]) && i >= n - 1) { b->hi--; n--; }
    return old;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_bool(int v) { return (DESCR_t){ .v = DT_BOOL, .i = v ? 1 : 0 }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_adv_mode(const rk_adv_t *a, int *flag, int hashy) {
    int vm = hashy ? a->vh : a->vh && a->vv;
    if ((a->kvh ? 1 : 0) + (a->ph ? 1 : 0) + (a->kh ? 1 : 0) + (vm ? 1 : 0) > 1) return -1;
    if (a->kvh) { *flag = a->kvv; return 'w'; }
    if (a->ph) { *flag = a->pv; return 'p'; }
    if (a->kh) { *flag = a->kv2; return 'k'; }
    *flag = a->vv;
    return vm ? 'v' : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_out(int m, DESCR_t pos, DESCR_t v, DESCR_t *r) {
    if (m == 'w') { r[0] = pos; r[1] = v; return 2; }
    if (m == 'p') { r[0] = rk_mk_pair(pos, v); return 1; }
    r[0] = m == 'k' ? pos : v;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_arr(DESCR_t c, DESCR_t pos, const rk_adv_t *a, DESCR_t *r) {
    int fl, m = rk_adv_mode(a, &fl, 0), d = a->dh && a->dv, w = rk_sub_has(c, pos);
    if (m < 0) return -1;
    if (a->eh) {
        int t = a->ev ? 1 : 0, val = w ? t : !t;
        if (m == 'k' || m == 'v') return -1;
        if (d && w) (void) rk_sub_del(c, pos);
        if (!m) { r[0] = rk_bool(val); return 1; }
        if (fl && !w) return 0;
        return rk_sub_out(m, pos, rk_bool(fl ? t : val), r);
    }
    if (m == 'k' && d) { if (fl && !w) return 0; if (w) (void) rk_sub_del(c, pos); r[0] = pos; return 1; }
    if (m == 'k') { if (fl && !w) return 0; r[0] = pos; return 1; }
    if ((m == 'w' || m == 'p' || m == 'v') && fl && !w) return 0;
    if (!m) { r[0] = d ? rk_sub_del(c, pos) : rk_sub_get(c, pos); return 1; }
    return rk_sub_out(m, pos, d ? rk_sub_del(c, pos) : rk_sub_get(c, pos), r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_hash1(DESCR_t c, DESCR_t key, const rk_adv_t *a, DESCR_t *r) {
    int fl, m = rk_adv_mode(a, &fl, 1), ex;
    if (m < 0 || (a->eh && (m == 'k' || m == 'v'))) return -1;
    ex = rk_sub_has(c, key);
    if (a->eh) {
        int rr = (a->ev ? 1 : 0) == ex;
        if (a->dh) { if (a->dv && ex) (void) rk_sub_del(c, key); if (!m) { r[0] = rk_bool(rr); return 1; } return ex || !fl ? rk_sub_out(m, key, rk_bool(rr), r) : 0; }
        if (!m) { r[0] = rk_bool(rr); return 1; }
        if (!rr) return 0;
        if (!fl) { r[0] = INTVAL(1); return 1; }
        return rk_sub_out(m, key, rk_bool(rr), r);
    }
    if (m && fl && !ex) return 0;
    if (m == 'k' && a->dh && a->dv && ex) (void) rk_sub_del(c, key);
    if (m == 'k') { r[0] = key; return 1; }
    if (!m) { r[0] = a->dh && a->dv ? rk_sub_del(c, key) : rk_sub_get(c, key); return 1; }
    return rk_sub_out(m, key, a->dh && a->dv ? rk_sub_del(c, key) : rk_sub_get(c, key), r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_sub_hashn(DESCR_t c, DESCR_t one, const rk_adv_t *a, DESCR_t *r) {
    int fl, m = rk_adv_mode(a, &fl, 1), d = a->dh && a->dv, was = rk_sub_has(c, one);
    if (m < 0 || (a->eh && (m == 'k' || m == 'v'))) return -1;
    if (a->eh) { int rr = (a->ev ? 1 : 0) == was; if (d) (void) rk_sub_del(c, one); if (!m) { r[0] = rk_bool(rr); return 1; } if (fl && !was) return 0; return rk_sub_out(m, one, rk_bool(rr), r); }
    if (m && fl && !was) return 0;
    if (m == 'k') { if (d && was) (void) rk_sub_del(c, one); r[0] = one; return 1; }
    return rk_sub_out(m, one, d ? rk_sub_del(c, one) : rk_sub_get(c, one), r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_subscr_adv(DESCR_t *args, int nargs, DESCR_t *out) {
    extern void rt_script_die_surface(const char *msg);
    rk_adv_t a;
    DESCR_t c = args[0], key = args[1];
    if (!rk_adv_parse(args + 2, nargs - 2, &a)) { rt_script_die_surface("Unexpected named argument passed to a subscript"); *out = FAILDESCR; return 1; }
    if (rk_match_is(c)) c = FIELD_GET_fn(c, "named");
    int isarr = rk_sub_is_arr(c), whatever = rk_typeobj_name(key) && !strcmp(rk_typeobj_name(key), "Whatever");
    int (*one)(DESCR_t, DESCR_t, const rk_adv_t *, DESCR_t *) = isarr ? rk_sub_arr : rk_sub_hashn;
    if (!whatever && !(key.v == DT_A && key.arr && !rk_is_pair(key) && !rk_is_jct(key))) {
        DESCR_t r[2];
        int n = (isarr ? rk_sub_arr : rk_sub_hash1)(c, key, &a, r);
        if (n < 0) { rt_script_die_surface("Unsupported combination of named arguments to a subscript"); *out = FAILDESCR; return 1; }
        *out = n == 0 ? rk_mark_list(rk_mk_arr(NULL, 0)) : n == 1 ? r[0] : rk_mark_list(rk_mk_arr(r, 2));
        return 1;
    }
    rk_av_t ks;
    if (whatever && isarr) {
        ARBLK_t *ab = (ARBLK_t *) c.arr;
        int an = ab->hi - ab->lo + 1;
        DESCR_t *ix = (DESCR_t *) rt_ws_alloc_descr((size_t) (an > 0 ? an : 1));
        for (int i = 0; i < an; i++) ix[i] = INTVAL(i);
        ks = rk_av(rk_mk_arr(ix, an > 0 ? an : 0));
    } else if (whatever) ks = rk_av(rk_hash_list(c, 'k'));
    else ks = rk_av(key);
    DESCR_t *buf = (DESCR_t *) rt_ws_alloc_descr((size_t) (2 * ks.n + 1));
    int k = 0;
    for (int i = 0; i < ks.n; i++) {
        int n = one(c, rk_av_elem(ks, i), &a, buf + k);
        if (n < 0) { rt_script_die_surface("Unsupported combination of named arguments to a subscript"); *out = FAILDESCR; return 1; }
        k += n;
    }
    *out = rk_mark_list(rk_mk_arr(buf, k));
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_arr_set_impl(DESCR_t *args, DESCR_t *out) {
    rk_av_t a = rk_av(args[0]);
    char ib[64];
    long idx = IS_INT_fn(args[1]) ? (long)args[1].i : IS_REAL_fn(args[1]) ? (long)args[1].r : strtol(to_cstring(args[1], ib, sizeof ib), NULL, 10);
    if (idx < 0) { *out = FAILDESCR; return; }
    if (rk_arr_store_in_place(args[0], idx, args[2])) { *out = args[0]; return; }
    long n = idx + 1 > a.n ? idx + 1 : a.n;
    DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
    for (long i = 0; i < n; i++) r[i] = (i == idx) ? args[2] : (i < a.n ? rk_av_elem(a, (int) i) : rk_typeobj("Any"));
    *out = rk_unmark(rk_mk_arr(r, (int) n));
}
DESCR_t c_rt_rk_arr_set(DESCR_t *args, int nargs) { DESCR_t out = NULVCL; (void) nargs; rk_arr_set_impl(args, &out); return out; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_elem_put_impl(DESCR_t *args, DESCR_t *out) {
    const char *kd = VARVAL_fn(args[0]);
    DESCR_t cur = args[1];
    if (kd && kd[0] == 'h') { *out = rk_hash_store((cur.v == DT_T && cur.tbl) ? cur : TABLE_VAL(table_new()), args[2], args[3]); return; }
    rk_av_t a = (cur.v == DT_A && cur.arr) ? rk_av(cur) : (rk_av_t){ 0 };
    char ib[64];
    long idx = IS_INT_fn(args[2]) ? (long)args[2].i : IS_REAL_fn(args[2]) ? (long)args[2].r : strtol(to_cstring(args[2], ib, sizeof ib), NULL, 10);
    if (idx < 0) { *out = FAILDESCR; return; }
    if (rk_arr_store_in_place(cur, idx, args[3])) { *out = cur; return; }
    long n = idx + 1 > a.n ? idx + 1 : a.n;
    DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
    for (long i = 0; i < n; i++) r[i] = (i == idx) ? args[3] : (i < a.n ? rk_av_elem(a, (int) i) : rk_typeobj("Any"));
    *out = rk_unmark(rk_mk_arr(r, (int) n));
}
DESCR_t rt_rk_elem_put(DESCR_t *args, int nargs) { DESCR_t out = NULVCL; (void) nargs; rk_elem_put_impl(args, &out); return out; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_arr_at_impl(DESCR_t *args, DESCR_t *out) {
    if (rk_failure_is(args[0])) { *out = args[0]; return; }
    if (rk_is_seq(args[0])) {
        long need = -1L;
        if (IS_INT_fn(args[1])) need = args[1].i >= 0 ? (long) args[1].i + 1 : -1L;
        else if (rk_is_range(args[1])) {
            DESCR_t l, h;
            int xl, xh;
            rk_range_bounds(args[1], &l, &h, &xl, &xh);
            if (IS_INT_fn(h)) need = (long) h.i + 1 - xh;
        } else if (args[1].v == DT_A && args[1].arr && !rk_is_pair(args[1])) {
            rk_av_t ix = rk_av(args[1]);
            long mx = -1;
            for (int k = 0; k < ix.n; k++) if (IS_INT_fn(ix.el[k]) && ix.el[k].i > mx) mx = (long) ix.el[k].i;
            need = mx + 1;
        }
        rk_seq_reify(&args[0], need);
        args[0] = ((ARBLK_t *) args[0].arr)->data[3];
    }
    if (rk_match_is(args[0])) args[0] = FIELD_GET_fn(args[0], "positional");
    if (args[0].v == DT_A && args[0].arr && args[1].v == DT_A && args[1].arr && !rk_is_pair(args[1]) && !rk_is_jct(args[1])) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        rk_av_t ix = rk_av(args[1]);
        DESCR_t *r = ix.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) ix.n) : NULL;
        for (int k = 0; k < ix.n; k++) { long i = rk_ival(ix.el[k]); r[k] = (i < b->lo || i > b->hi) ? rk_typeobj("Any") : rk_slot(b, i - b->lo); }
        *out = rk_mk_arr(r, ix.n);
        return;
    }
    if (args[0].v == DT_A && args[0].arr) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        long i = rk_ival(args[1]);
        if (i < 0) {
            rk_exc_init();
            *out =
                rk_failure_new(DATCON_fn("X::OutOfRange", STRVAL(rt_heap_fmt("Index out of range. Is: %ld, should be in 0..^Inf", i)), STRVAL(rt_heap_strdup_c("Index")), INTVAL(i),
                STRVAL(rt_heap_strdup_c("0..^Inf"))));
            return;
        }
        if (i < b->lo || i > b->hi) { *out = rk_typeobj("Any"); return; }
        *out = rk_slot(b, i - b->lo);
        return;
    }
    rk_av_t sc = rk_av(args[0]);
    long idx = rk_ival(args[1]);
    *out = (idx >= 0 && idx < sc.n) ? sc.el[idx] : rk_typeobj("Any");
}
DESCR_t bt___rk_elem_bind(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        DESCR_t c = args[0], r = args[2];
        long idx = IS_INT_fn(args[1]) ? (long) args[1].i : IS_REAL_fn(args[1]) ? (long) args[1].r : -1L;
        if (idx < 0) { return c; }
        if (!rk_arr_put(c, idx, r, 0)) {
            ARBLK_t *b0 = (c.v == DT_A && c.arr && !rk_is_seq(c)) ? (ARBLK_t *) c.arr : NULL;
            long n0 = (b0 && !b0->proto && b0->hi >= b0->lo) ? b0->hi - b0->lo + 1 : 0;
            long n = idx + 1 > n0 ? idx + 1 : n0;
            DESCR_t *d = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
            for (long i = 0; i < n; i++) d[i] = i == idx ? r : i < n0 ? b0->data[i] : rk_typeobj("Any");
            c = rk_unmark(rk_mk_arr(d, (int) n));
        }
        if (rk_is_cell(r)) ((ARBLK_t *) c.arr)->proto_bare |= RK_ARR_BOUND;
        return c;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_seq_new(DESCR_t *args, int nargs) {
    if ((nargs == 1 || nargs == 2)) {
        DESCR_t sq = rk_mk_arr(NULL, 0);
        rk_arr_append((ARBLK_t *) sq.arr, args[0]);
        rk_arr_append((ARBLK_t *) sq.arr, (DESCR_t){ .v = DT_RAW, .ptr = (void *) 0 });
        rk_arr_append((ARBLK_t *) sq.arr, INTVAL(0));
        rk_arr_append((ARBLK_t *) sq.arr, rk_mark_list(rk_mk_arr(NULL, 0)));
        ((ARBLK_t *) sq.arr)->proto = rk_proto_seq;
        if (nargs == 2 && IS_INT_fn(args[1]) && args[1].i) ((ARBLK_t *) sq.arr)->proto_bare |= RK_SEQ_LAZY;
        return sq;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_iter_has(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        long i = (long) rk_ival(args[1]);
        if (rk_is_seq(args[0])) {
            rk_seq_reify(&args[0], i + 1);
            ARBLK_t *c = (ARBLK_t *) ((ARBLK_t *) args[0].arr)->data[3].arr;
            return (DESCR_t){ .v = DT_BOOL, .i = (long) (c->hi - c->lo + 1) > i };
        }
        { rk_av_t a = rk_av(args[0]); return (DESCR_t){ .v = DT_BOOL, .i = i < a.n }; }
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_arr(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 0) {
        DESCR_t ag = rt_make_item_agg(args, nargs);
        int as_list = (_bop == 0);
        if (!as_list) { ARBLK_t * ab = (ARBLK_t *) ag.arr; for (int i = 0; i <= ab->hi - ab->lo; i++) if (ab->data[i].v == DT_SNUL) ab->data[i] = rk_typeobj("Any"); }
        return as_list ? rk_mark_list(ag) : rk_unmark(ag);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_lit(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_spread(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        DESCR_t v = args[0];
        int spread = (rk_is_seq(v) || (v.v == DT_A && v.arr)) && !rk_is_range(v) && !rk_is_pair(v) && !rk_is_jct(v);
        rk_av_t a = spread ? rk_av(v) : (rk_av_t) { 1, args };
        return rk_unmark(rt_make_item_agg((DESCR_t *) a.el, a.n));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_materialize(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) { DESCR_t *a = 0; int n = 0; if (!rt_lv_is_list(args[0], &a, &n)) return (DESCR_t){ .v = RTX_NOT_HANDLED }; return rk_mark_list(rt_make_item_agg(a, n)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_range_val(DESCR_t *args, int nargs) { if (nargs == 3) { return rk_range_value(args[0], args[1], IS_INT_fn(args[2]) ? (int) args[2].i : 0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_in_range(DESCR_t *args, int nargs) {
    if ((nargs == 3 || nargs == 4)) { return (DESCR_t){ .v = DT_BOOL, .i = rk_in_range(args[0], args[1], args[2], nargs == 4 ? (int) args[3].i : 0) }; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_typeobj(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_typeobj(VARVAL_fn(args[0])); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_item1(DESCR_t *args, int nargs) { if (nargs == 1) { return args[0]; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_hash(DESCR_t *args, int nargs) {
    { DESCR_t hh = TABLE_VAL(table_new()); for (int i = 0; i + 1 < nargs; i += 2) hh = rk_hash_store(hh, args[i], args[i + 1]); return hh; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_hkeys(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) { if (args[0].v == DT_A && args[0].arr && !rk_is_pair(args[0])) { rk_av_t a = rk_av(args[0]); *out = rk_mark_list(rk_mk_arr(a.el, a.n)); } else *out = args[0]; return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_sleep(DESCR_t *args, int nargs) {
    if (nargs >= 1) {
        double sec = IS_REAL_fn(args[0]) ? args[0].r : IS_INT_fn(args[0]) ? (double) args[0].i : strtod(rk_cstr(args[0]), NULL);
        if (sec > 0) { fflush(stdout); usleep((useconds_t) (sec * 1e6)); }
        return rk_typeobj("Nil");
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_numify(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) {
        *out = rk_is_range(args[0]) ? rk_range_numeric(args[0]) : args[0];
        if (args[0].v == DT_A && args[0].arr && !rk_is_range(args[0]) && !rk_is_pair(args[0]) && !rk_is_jct(args[0]) && !rk_is_seq(args[0])) *out = INTVAL(rk_av(args[0]).n);
        if (args[0].v == DT_T && args[0].tbl) *out = INTVAL(args[0].tbl->size);
        if (IS_DATA_INST_fn(args[0]) && FIELD_GET_fn(args[0], "text").v != 0) {
            char nb[64];
            const char *t = to_cstring(FIELD_GET_fn(args[0], "text"), nb, sizeof nb);
            char *e;
            long long iv = t ? strtoll(t, &e, 10) : 0;
            if (t && *t && !*e) *out = INTVAL(iv);
            else *out = (DESCR_t){ .v = DT_R, .r = t ? strtod(t, NULL) : 0.0 };
        }
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_hyper_done(DESCR_t *args, int nargs) {
    if (nargs == 2) { return (args[1].v == DT_A && args[1].arr && !((ARBLK_t *) args[1].arr)->proto) ? rk_unmark(args[0]) : args[0]; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_hash_set_pure(DESCR_t *args, int nargs) {
    if (nargs >= 3 && rk_is_list(args[1]) && !rk_is_pair(args[1]) && !rk_is_seq(args[1])) {
        rk_av_t ks = rk_av(args[1]);
        int vl = args[2].v == DT_A && args[2].arr && !rk_is_pair(args[2]);
        rk_av_t vs = vl ? rk_av(args[2]) : (rk_av_t){ 1, &args[2], 0 };
        DESCR_t h = args[0];
        for (int i = 0; i < ks.n; i++) { DESCR_t v = i < vs.n ? vs.el[i] : rk_typeobj("Any"); if (rk_qh_is(h)) h = rk_qh_put(h, ks.el[i], v); else h = rk_hash_store(h, ks.el[i], v); }
        return h;
    }
    if (nargs >= 3 && rk_qh_is(args[0])) { return rk_qh_put(args[0], args[1], args[2]); }
    if (nargs >= 3) { return rk_hash_store(args[0], args[1], args[2]); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_unpack_named(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 2) {
        if (args[0].v == DT_T && args[0].tbl) { int found = 0; DESCR_t v = table_get_found_d(args[0].tbl, rk_key_tmp(args[1]), &found); return found ? v : rk_typeobj("Any"); }
        DESCR_t ma[2] = { args[0], args[1] };
        if (script_try_call_builtin_by_name("meth_call", ma, 2, out) && !IS_FAIL_fn(*out)) return _bo;
        return rk_typeobj("Any");
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bt_hash_each(DESCR_t *args, DESCR_t (*one)(DESCR_t *, int), int as_bool) {
    rk_av_t ks = rk_av(args[1]);
    DESCR_t *r = ks.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) ks.n) : NULL;
    for (int i = 0; i < ks.n; i++) {
        DESCR_t sub[2] = { args[0], ks.el[i] }, got = one(sub, 2);
        r[i] = IS_FAIL_fn(got) ? rk_typeobj("Any") : got;
        if (as_bool) r[i] = (DESCR_t){ .v = DT_BOOL, .i = rk_is_truthy(r[i]) ? 1 : 0 };
    }
    return rk_mark_list(rk_mk_arr(r, ks.n));
}
DESCR_t bt_hash_get(DESCR_t *args, int nargs) {
    if (nargs < 2) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    if (rk_is_list(args[1]) && !rk_is_pair(args[1]) && !rk_is_seq(args[1])) return bt_hash_each(args, bt_hash_get, 0);
    if (rk_qh_is(args[0])) return rk_qh_get(args[0], args[1]);
    if (rk_match_is(args[0])) args[0] = FIELD_GET_fn(args[0], "named");
    if (rk_sub_is_arr(args[0]) || rk_is_range(args[0])) {
        rk_exc_throw(rk_exc_adhoc(STRVAL(rt_heap_fmt("Type %s does not support associative indexing.", rt_mc_type_name(args[0])))));
        return FAILDESCR;
    }
    int found = 0;
    DESCR_t v = (args[0].v == DT_T && args[0].tbl) ? table_get_found_d(args[0].tbl, rk_key_tmp(args[1]), &found) : NULVCL;
    return found ? v : rk_typeobj("Any");
}
DESCR_t bt_hash_exists(DESCR_t *args, int nargs) {
    if (nargs < 2) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    if (rk_is_list(args[1]) && !rk_is_pair(args[1]) && !rk_is_seq(args[1])) return bt_hash_each(args, bt_hash_exists, 1);
    if (rk_qh_is(args[0])) return INTVAL(rk_qh_get(args[0], args[1]).i ? 1 : 0);
    return INTVAL((args[0].v == DT_T && args[0].tbl && table_has_d(args[0].tbl, rk_key_tmp(args[1]))) ? 1 : 0);
}
DESCR_t bt_hash_delete(DESCR_t *args, int nargs) {
    if (nargs < 2) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    if (rk_is_list(args[1]) && !rk_is_pair(args[1]) && !rk_is_seq(args[1])) return bt_hash_each(args, bt_hash_delete, 0);
    if (rk_qh_is(args[0])) { DESCR_t ov; return rk_qh_del(args[0], args[1], &ov) ? ov : NULVCL; }
    DESCR_t dk = rk_key_tmp(args[1]), dv = NULVCL;
    int found = 0;
    if (args[0].v == DT_T && args[0].tbl) { dv = table_get_found_d(args[0].tbl, dk, &found); if (found) table_delete_d(args[0].tbl, dk); }
    return found ? dv : NULVCL;
}
DESCR_t bt___rk_subscr_adv(DESCR_t *args, int nargs) { DESCR_t o = FAILDESCR; if (nargs >= 2 && rk_subscr_adv(args, nargs, &o)) return o; return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
