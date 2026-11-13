#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "runtime/builtin_leaf.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_19_rk.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern void *rt_ws_alloc_descr(size_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_order_both_numeric(DESCR_t a, DESCR_t b) { extern int rk_rat_is(DESCR_t); return (IS_INT_fn(a) || IS_REAL_fn(a) || rk_rat_is(a)) && (IS_INT_fn(b) || IS_REAL_fn(b) || rk_rat_is(b)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long long rk_order_cmp_num(DESCR_t a, DESCR_t b) {
    extern int rk_rat_cmp(DESCR_t, DESCR_t, int *);
    int rc;
    if ((a.v == DT_DATA || b.v == DT_DATA) && rk_rat_cmp(a, b, &rc)) return rc;
    double x = to_real(a), y = to_real(b);
    return x < y ? -1 : (x > y ? 1 : 0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long long rk_order_cmp_str(DESCR_t a, DESCR_t b) { const char *x = VARVAL_fn(a), *y = VARVAL_fn(b); int c = strcmp(x ? x : "", y ? y : ""); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rk_av_t rk_av_args(DESCR_t *args, int from, int to) {
    int tot = 0, m = to > from ? to - from : 0;
    rk_av_t vs[m ? m : 1];
    for (int i = 0; i < m; i++) { vs[i] = rk_av(args[from + i]); tot += vs[i].n; }
    DESCR_t *all = tot ? (DESCR_t *) rt_ws_alloc_descr((size_t) tot) : NULL;
    int k = 0;
    for (int i = 0; i < m; i++) for (int j = 0; j < vs[i].n; j++) all[k++] = vs[i].el[j];
    rk_av_t a = { tot, all };
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char rk_proto_slip[] = "Slip";
const char rk_proto_tlgot[] = "TLGot";
static int rk_is_tlgot(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_tlgot; }
const char rk_proto_list[] = "List";
static DESCR_t rk_hy_rec(rk_hy_t *h, DESCR_t l, DESCR_t r) {
    int ls = rk_hy_seq(l), rs = rk_hy_seq(r) && !(h->fl & 8);
    if (!ls && !rs) {
        if (h->mode == 0) {
            DESCR_t pr[2] = { l, r };
            if (h->n == h->cap) {
                int nc = h->cap ? h->cap * 2 : 16;
                DESCR_t *nb = (DESCR_t *) rt_ws_alloc_descr((size_t) nc);
                for (int i = 0; i < h->n; i++) nb[i] = h->buf[i];
                h->buf = nb;
                h->cap = nc;
            }
            h->buf[h->n++] = rk_mk_arr(pr, 2);
            return NULVCL;
        }
        return h->vi < h->nv ? h->vals[h->vi++] : NULVCL;
    }
    rk_av_t la = ls ? rk_av(l) : (rk_av_t){ 0 }, ra = rs ? rk_av(r) : (rk_av_t){ 0 };
    int nl = ls ? la.n : 1, nr = rs ? ra.n : 1, ldw = (h->fl >> 1) & 1, rdw = (h->fl >> 2) & 1, n;
    if (!ldw && !rdw) { if (nl != nr) { h->err = 1; return NULVCL; } n = nl; } else if (ldw && !rdw) n = nr;
    else if (!ldw && rdw) n = nl;
    else n = nl > nr ? nl : nr;
    if (nl == 0 || nr == 0) n = 0;
    DESCR_t *res = (h->mode == 1 && n) ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
    for (int i = 0; i < n && !h->err; i++) { DESCR_t a = ls ? rk_av_elem(la, i % nl) : l, b = rs ? rk_av_elem(ra, i % nr) : r; DESCR_t x = rk_hy_rec(h, a, b); if (res) res[i] = x; }
    if (h->mode == 0) return NULVCL;
    DESCR_t out = rk_mk_arr(res, n);
    return rk_is_list(ls ? l : r) || rk_is_slip(ls ? l : r) ? rk_mark_list(out) : rk_unmark(out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_text_int(const char *t, long long *v) { char *ep; *v = strtoll(t, &ep, 10); return *ep == '\0' && ep != t; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_av_minmax(rk_av_t a, int want_max) {
    if (a.n == 0) return REALVAL(want_max ? -INFINITY : INFINITY);
    {
        extern int rk_rat_is(DESCR_t);
        int anyrat = 0, allnum = a.n > 0;
        for (int i = 0; i < a.n && allnum; i++) { if (rk_rat_is(a.el[i])) anyrat = 1; else if (!IS_INT_fn(a.el[i]) && !IS_REAL_fn(a.el[i])) allnum = 0; }
        if (allnum && anyrat) { DESCR_t bst = a.el[0]; for (int i = 1; i < a.n; i++) { long long c = rk_order_cmp_num(a.el[i], bst); if (want_max ? c > 0 : c < 0) bst = a.el[i]; } return bst; }
    }
    const char *best = NULL;
    long long bestn = 0;
    int best_num = 0;
    for (int i = 0; i < a.n; i++) {
        const char *t = rk_av_text(a, i);
        long long v;
        int isn = rk_text_int(t, &v);
        int take;
        if (!best) take = 1;
        else if (isn && best_num) take = want_max ? (v > bestn) : (v < bestn);
        else { int c = strcmp(t, best); take = want_max ? (c > 0) : (c < 0); }
        if (take) { best = t; bestn = v; best_num = isn; }
    }
    if (!best) return NULVCL;
    return best_num ? INTVAL(bestn) : STRVAL(rt_heap_strdup_c(best));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_av_arith(rk_av_t a, int op) {
    {
        extern int rk_rat_is(DESCR_t);
        extern int rk_rat_binop(DESCR_t, DESCR_t, int, DESCR_t *);
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        int anyrat = 0;
        for (int i = 0; i < a.n && !anyrat; i++) anyrat = rk_rat_is(a.el[i]);
        if (anyrat) {
            int bop = op == '+' ? BINOP_ADD : op == '*' ? BINOP_MUL : BINOP_SUB;
            DESCR_t acc = a.el[0];
            for (int i = 1; i < a.n; i++) { DESCR_t r; if (!rk_rat_binop(acc, a.el[i], bop, &r)) r = rt_num_arith(acc, a.el[i], bop); acc = r; }
            return acc;
        }
    }
    long long iacc = 0;
    double racc = 0.0;
    int any_real = 0;
    for (int i = 0; i < a.n; i++) {
        const char *t = rk_av_text(a, i);
        long long iv;
        int isn = rk_text_int(t, &iv);
        double dv = isn ? (double) iv : strtod(t, NULL);
        if (!isn) any_real = 1;
        if (i == 0 && op != '+') { iacc = iv; racc = dv; } else if (op == '+') { iacc += iv; racc += dv; } else if (op == '*') { iacc *= iv; racc *= dv; } else { iacc -= iv; racc -= dv; }
    }
    return any_real ? REALVAL(racc) : INTVAL(iacc);
}
long long rk_int_arg(DESCR_t v, long long d) {
    extern int rk_rat_to_int(DESCR_t, long long *);
    long long q;
    if (IS_STR_fn(v)) { char *e = NULL; const char *t = rk_cstr(v); double dv = strtod(t, &e); return (e && e != t && !*e) ? (long long) dv : d; }
    return IS_INT_fn(v) ? (long long) v.i : IS_REAL_fn(v) ? (long long) v.r : (v.v == DT_DATA && rk_rat_to_int(v, &q)) ? q : d;
}
int rk_int_or_rat(DESCR_t v) { extern int rk_rat_is(DESCR_t); return IS_INT_fn(v) || (v.v == DT_DATA && rk_rat_is(v)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_cmp_val(DESCR_t a, DESCR_t b) {
    double da, db;
    int ia, ib;
    if (rk_range_num(a, &da, &ia) && rk_range_num(b, &db, &ib)) return da < db ? -1 : da > db ? 1 : 0;
    return strcmp(rk_cstr(a), rk_cstr(b));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_next_perm(int *ix, int n) {
    int i = n - 2;
    while (i >= 0 && ix[i] >= ix[i + 1]) i--;
    if (i < 0) return 0;
    int j = n - 1;
    while (ix[j] <= ix[i]) j--;
    int t = ix[i];
    ix[i] = ix[j];
    ix[j] = t;
    for (int a = i + 1, b = n - 1; a < b; a++, b--) { t = ix[a]; ix[a] = ix[b]; ix[b] = t; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_list_more_methods(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    rk_av_t a = rk_av(recv);
    if (nmargs == 0 && !strcmp(meth, "permutations")) {
        if (a.n > 9) return 0;
        DESCR_t res = rk_mk_arr(NULL, 0);
        int ix[a.n > 0 ? a.n : 1];
        for (int i = 0; i < a.n; i++) ix[i] = i;
        do {
            DESCR_t row[a.n > 0 ? a.n : 1];
            for (int i = 0; i < a.n; i++) row[i] = a.el[ix[i]];
            rk_arr_append((ARBLK_t *) res.arr, rk_mk_arr(row, a.n));
            a = rk_av(recv);
        } while (rk_next_perm(ix, a.n));
        *out = res;
        return 1;
    }
    if (!strcmp(meth, "combinations") && nmargs <= 1) {
        int lo = 0, hi = a.n;
        if (nmargs == 1) {
            DESCR_t m = margs[0];
            if (rk_is_range(m)) {
                DESCR_t l, h;
                int xl, xh;
                rk_range_bounds(m, &l, &h, &xl, &xh);
                lo = (int) (IS_INT_fn(l) ? l.i : 0) + xl;
                hi = (int) (IS_INT_fn(h) ? h.i : a.n) - xh;
            } else if (IS_INT_fn(m)) lo = hi = (int) m.i;
            else return 0;
        }
        DESCR_t res = rk_mk_arr(NULL, 0);
        for (int k = lo < 0 ? 0 : lo; k <= hi && k <= a.n; k++) rk_combos_of(a, k, res);
        *out = res;
        return 1;
    }
    if (!strcmp(meth, "rotor") && nmargs >= 1) {
        int partial = 0, nc = 0;
        int cn[nmargs + 8], cg[nmargs + 8];
        for (int i = 0; i < nmargs; i++) {
            DESCR_t m = margs[i];
            if (rk_is_pair(m)) {
                rk_av_t pa = rk_av(m);
                const char *k = rk_cstr(pa.el[0]);
                if (!strcmp(k, "partial")) { partial = rk_is_truthy(pa.el[1]); continue; }
                cn[nc] = (int) (IS_INT_fn(pa.el[0]) ? pa.el[0].i : atoi(k));
                cg[nc] = (int) rk_int_arg(pa.el[1], 0);
                nc++;
            } else if (rk_int_or_rat(m)) {
                cn[nc] = (int) rk_int_arg(m, 0);
                cg[nc] = 0;
                nc++;
            } else return 0;
        }
        if (nc == 0) return 0;
        DESCR_t res = rk_mk_arr(NULL, 0);
        long pos = 0;
        for (int step = 0; pos < a.n && step < 1000000; step++) {
            int n = cn[step % nc], g = cg[step % nc];
            if (n <= 0) break;
            if (pos + n > a.n && !partial) break;
            long e = pos + n > a.n ? a.n : pos + n;
            rk_arr_append((ARBLK_t *) res.arr, rk_mk_arr(a.el + pos, (int) (e - pos)));
            pos += n + g;
            if (pos < 0) pos = 0;
        }
        *out = res;
        return 1;
    }
    if (nmargs == 0 && !strcmp(meth, "pairup")) {
        DESCR_t res = rk_mk_arr(NULL, 0);
        for (int i = 0; i < a.n; ) {
            if (rk_is_pair(a.el[i])) { rk_arr_append((ARBLK_t *) res.arr, a.el[i]); i++; continue; }
            if (i + 1 >= a.n) { extern void rt_script_die_surface(const char *msg); rt_script_die_surface("Odd number of elements found for .pairup()"); *out = FAILDESCR; return 1; }
            rk_arr_append((ARBLK_t *) res.arr, rk_mk_pair(a.el[i], a.el[i + 1]));
            i += 2;
        }
        *out = res;
        return 1;
    }
    if (nmargs == 0 && (!strcmp(meth, "maxpairs") || !strcmp(meth, "minpairs"))) {
        int mx = meth[1] == 'a';
        DESCR_t res = rk_mk_arr(NULL, 0);
        if (a.n) {
            DESCR_t best = a.el[0];
            for (int i = 1; i < a.n; i++) { int c = rk_cmp_val(a.el[i], best); if (mx ? c > 0 : c < 0) best = a.el[i]; }
            for (int i = 0; i < a.n; i++) if (!rk_cmp_val(a.el[i], best)) rk_arr_append((ARBLK_t *) res.arr, rk_mk_pair(INTVAL(i), a.el[i]));
        }
        *out = res;
        return 1;
    }
    if (!strcmp(meth, "skip") && nmargs <= 1) {
        long n = nmargs == 1 ? (long) rk_int_arg(margs[0], 1) : 1;
        if (n < 0) n = 0;
        *out = n >= a.n ? rk_mk_arr(NULL, 0) : rk_mk_arr(a.el + n, a.n - (int) n);
        return 1;
    }
    if (!strcmp(meth, "roll") && nmargs <= 1) {
        if (!a.n) { *out = rk_mk_arr(NULL, 0); return 1; }
        if (nmargs == 0) { *out = a.el[rand() % a.n]; return 1; }
        if (!rk_int_or_rat(margs[0])) return 0;
        long cnt = (long) rk_int_arg(margs[0], 0);
        DESCR_t *r = cnt > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) cnt) : NULL;
        for (long i = 0; i < cnt; i++) r[i] = a.el[rand() % a.n];
        *out = rk_mk_arr(r, (int) (cnt > 0 ? cnt : 0));
        return 1;
    }
    if (nmargs == 0 && (!strcmp(meth, "any") || !strcmp(meth, "all") || !strcmp(meth, "one") || !strcmp(meth, "none"))) {
        const char *fl = meth[0] == 'a' ? (meth[1] == 'n' ? rk_proto_jct_any : rk_proto_jct_all) : meth[0] == 'o' ? rk_proto_jct_one : rk_proto_jct_none;
        DESCR_t *src = (DESCR_t *) a.el;
        *out = rk_mk_jct(fl, src, a.n);
        return 1;
    }
    if (nmargs == 0 && !strcmp(meth, "invert")) {
        DESCR_t res = rk_mk_arr(NULL, 0);
        for (int i = 0; i < a.n; i++) {
            if (!rk_is_pair(a.el[i])) return 0;
            rk_av_t pa = rk_av(a.el[i]);
            DESCR_t k = pa.el[0], v = pa.el[1];
            if (v.v == DT_A && v.arr && !rk_is_pair(v)) {
                rk_av_t va = rk_av(v);
                for (int j = 0; j < va.n; j++) rk_arr_append((ARBLK_t *) res.arr, rk_mk_pair(va.el[j], k));
            } else rk_arr_append((ARBLK_t *) res.arr, rk_mk_pair(v, k));
            a = rk_av(recv);
        }
        *out = rk_seq_done(res);
        return 1;
    }
    if ((!strcmp(meth, "unique") || !strcmp(meth, "squish") || !strcmp(meth, "repeated")) && nmargs == 2 && IS_STR_fn(margs[0]) && !strcmp(rk_cstr(margs[0]), "as") && rk_is_code(margs[1])) {
        int nrf = rk_blk_nref(margs[1]);
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = rk_blk_snap(margs[1], rfs);
        rk_cbh_t H = { g_rk_cbh_cur, { recv, rk_mk_arr(NULL, 0) } };
        g_rk_cbh_cur = &H;
        DESCR_t keys = rk_mk_arr(NULL, 0);
        DESCR_t prev = NULVCL;
        int mode = meth[0] == 'u' ? 0 : meth[0] == 's' ? 1 : 2;
        for (int i = 0; i < a.n; i++) {
            DESCR_t el = rk_av_elem(rk_av(H.d[0]), i);
            rt_call_args_need(2);
            CALL_ARGS[0] = el;
            DESCR_t kd = rk_call_snap(cb, rfs, 1);
            const char *kt = rk_cstr(kd);
            rk_av_t ka = rk_av(keys);
            int seen = 0;
            if (mode == 1) seen = i > 0 && !strcmp(rk_cstr(prev), kt);
            else for (int j = 0; j < ka.n && !seen; j++) if (!strcmp(rk_cstr(ka.el[j]), kt)) seen = 1;
            if (mode == 1) prev = STRVAL(rt_heap_strdup_c(kt));
            else if (!seen) rk_arr_append((ARBLK_t *) keys.arr, STRVAL(rt_heap_strdup_c(kt)));
            if (mode == 2 ? seen : !seen) rk_arr_append((ARBLK_t *) H.d[1].arr, rk_av_elem(rk_av(H.d[0]), i));
        }
        g_rk_cbh_cur = H.prev;
        *out = H.d[1];
        return 1;
    }
    if (nmargs == 1 && !strcmp(meth, "produce") && rk_is_code(margs[0])) {
        int nrf = rk_blk_nref(margs[0]);
        DESCR_t rfs[nrf ? nrf : 1];
        rk_cb_t cb = rk_blk_snap(margs[0], rfs);
        rk_cbh_t H = { g_rk_cbh_cur, { recv, rk_mk_arr(NULL, 0) } };
        g_rk_cbh_cur = &H;
        DESCR_t acc = NULVCL;
        for (int i = 0; i < a.n; i++) {
            DESCR_t el = rk_av_elem(rk_av(H.d[0]), i);
            if (i == 0) acc = el;
            else { rt_call_args_need(3); CALL_ARGS[0] = acc; CALL_ARGS[1] = el; acc = rk_call_snap(cb, rfs, 2); }
            rk_arr_append((ARBLK_t *) H.d[1].arr, acc);
        }
        g_rk_cbh_cur = H.prev;
        *out = H.d[1];
        return 1;
    }
    if (nmargs == 0 && !strcmp(meth, "is-lazy")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
    if (nmargs == 0 && !strcmp(meth, "iterator")) { *out = rk_seq_done(rk_mk_arr(a.el, a.n)); return 1; }
    if (nmargs == 0 && !strcmp(meth, "tail")) { *out = a.n ? a.el[a.n - 1] : NULVCL; return 1; }
    if (nmargs == 0 && !strcmp(meth, "minmax")) {
        if (!a.n) { *out = rk_mk_arr(NULL, 0); return 1; }
        DESCR_t lo = a.el[0], hi = a.el[0];
        for (int i = 1; i < a.n; i++) { if (rk_cmp_val(a.el[i], lo) < 0) lo = a.el[i]; if (rk_cmp_val(a.el[i], hi) > 0) hi = a.el[i]; }
        *out = rk_range_value(lo, hi, 0);
        return 1;
    }
    if (!strcmp(meth, "fmt") && nmargs >= 1) {
        extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
        const char *sep = nmargs >= 2 ? rk_cstr(margs[1]) : " ";
        DESCR_t parts = rk_mk_arr(NULL, 0);
        for (int i = 0; i < a.n; i++) {
            DESCR_t fa[3] = { margs[0], a.el[i], NULVCL }, r;
            int na = 2;
            DESCR_t pair = a.el[i];
            if (rk_is_pair(pair)) { rk_av_t pa = rk_av(pair); fa[1] = pa.el[0]; fa[2] = pa.el[1]; na = 3; }
            if (script_try_call_builtin_by_name("sprintf", fa, na, &r) && !IS_FAIL_fn(r)) rk_arr_append((ARBLK_t *) parts.arr, r);
            a = rk_av(recv);
        }
        rk_av_t pv = rk_av(parts);
        const char **tx = pv.n ? (const char **) rt_pvec_alloc((size_t) pv.n) : NULL;
        for (int i = 0; i < pv.n; i++) tx[i] = rk_cstr(pv.el[i]);
        *out = STRVAL((char *) rk_join_gists(tx, pv.n, sep, "", ""));
        return 1;
    }
    return 0;
}
void pl_vt_init(pl_vtab_t *vt, int occ) {
    unsigned hc = 16;
    vt->cap = occ + 1;
    while (hc < 2u * (unsigned)vt->cap) hc <<= 1;
    vt->nm = (const char **)rt_pvec_alloc((size_t)vt->cap);
    vt->v = (DESCR_t *)rt_ws_alloc_descr((size_t)vt->cap);
    vt->cnt = (int *)rt_wsb_alloc((size_t)vt->cap * sizeof(int));
    vt->hs = (int *)rt_wsb_alloc((size_t)hc * sizeof(int));
    memset(vt->hs, 0xff, (size_t)hc * sizeof(int));
    vt->hmask = hc - 1;
    vt->n = 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_hash_count_of(DESCR_t h) { return (h.v == DT_T && h.tbl) ? h.tbl->size : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_hash_copy(DESCR_t h) {
    TBBLK_t *t = table_new();
    if (h.v == DT_T && h.tbl) {
        TBBLK_t *s = h.tbl;
        for (unsigned i = 0; i < s->ord_len; i++) {
            DESCR_t key = s->ord[i];
            if (key.v == DT_RAW) continue;
            TBPAIR_t *e = table_find_pair_d(s, key);
            if (e) table_set_descr_d(t, key, rt_deref(e->val));
        }
    }
    return TABLE_VAL(t);
}
static DESCR_t bta___rk_cmp3(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 2) {
        DESCR_t a = args[0], b = args[1];
        int numeric;
        if ((_bop == 0)) numeric = 1;
        else if ((_bop == 2)) numeric = 0;
        else numeric = rk_order_both_numeric(a, b);
        if ((_bop == 0) && ((IS_REAL_fn(a) && isnan(a.r)) || (IS_REAL_fn(b) && isnan(b.r)))) { return NULVCL; }
        long long r = numeric ? rk_order_cmp_num(a, b) : rk_order_cmp_str(a, b);
        return (DESCR_t){ .v = DT_ORDER, .i = r };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_cmp3(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cmp3(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_leg(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cmp3(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_cmpg(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cmp3(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_elems(DESCR_t *args, int nargs) { if (nargs == 1) { return INTVAL(args[0].v == DT_T ? rk_hash_count_of(args[0]) : rk_av(args[0]).n); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_substr_replace(DESCR_t *args, int nargs) {
    if ((nargs == 3 || nargs == 4)) {
        const char *str = rk_cstr(args[0]);
        const char *rep = rk_cstr(args[nargs - 1]);
        long from = IS_INT_fn(args[1]) ? (long)args[1].i : 0, len = nargs == 4 && IS_INT_fn(args[2]) ? (long)args[2].i : -1;
        size_t n = strlen(str), a = 0;
        long k = 0;
        while (a < n && k < from) { a++; while (a < n && ((unsigned char)str[a] & 0xC0) == 0x80) a++; k++; }
        size_t e = a;
        k = 0;
        if (len < 0) e = n;
        else while (e < n && k < len) { e++; while (e < n && ((unsigned char)str[e] & 0xC0) == 0x80) e++; k++; }
        size_t rl = strlen(rep);
        char *r = (char *)rt_str_alloc(a + rl + (n - e));
        memcpy(r, str, a);
        memcpy(r + a, rep, rl);
        memcpy(r + a + rl, str + e, n - e);
        r[a + rl + (n - e)] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_min(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        DESCR_t a = args[0], b = args[1];
        int c;
        if (rk_order_both_numeric(a, b) && (a.v == DT_DATA || b.v == DT_DATA)) c = (int) rk_order_cmp_num(a, b);
        else if ((IS_INT_fn(a) || IS_REAL_fn(a)) && (IS_INT_fn(b) || IS_REAL_fn(b))) {
            if (IS_INT_fn(a) && IS_INT_fn(b)) c = a.i < b.i ? -1 : a.i > b.i;
            else { double x = IS_REAL_fn(a) ? a.r : (double)a.i, y = IS_REAL_fn(b) ? b.r : (double)b.i; c = x < y ? -1 : x > y; }
        } else {
            const char *xs = rk_cstr(a), *ys = rk_cstr(b);
            c = strcmp(xs ? xs : "", ys ? ys : "");
        }
        return (1 && (_bop == 0)) ? (c <= 0 ? a : b) : (c >= 0 ? a : b);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_max(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_min(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_min(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_min(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_cross(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        int zip = (_bop == 1);
        long long fin = -1;
        for (int i = 0; zip && i < nargs; i++) if (!rk_is_seq(args[i])) { long long n = rk_av_args(args, i, i + 1).n; fin = (fin < 0 || n < fin) ? n : fin; }
        if (fin >= 0) {
            rk_cbh_t hold[nargs];
            for (int i = 0; i < nargs; i++) { hold[i] = (rk_cbh_t) { g_rk_cbh_cur, { args[i], NULVCL } }; g_rk_cbh_cur = &hold[i]; }
            for (int i = 0; i < nargs; i++) if (rk_is_seq(hold[i].d[0])) rk_seq_reify(&hold[i].d[0], fin);
            g_rk_cbh_cur = hold[0].prev;
            for (int i = 0; i < nargs; i++) args[i] = rk_is_seq(hold[i].d[0]) ? ((ARBLK_t *) hold[i].d[0].arr)->data[3] : hold[i].d[0];
        }
        rk_av_t vs[nargs > 0 ? nargs : 1];
        long long rows = zip ? -1 : 1;
        for (int i = 0; i < nargs; i++) { vs[i] = rk_av_args(args, i, i + 1); if (zip) rows = (rows < 0 || vs[i].n < rows) ? vs[i].n : rows; else rows *= vs[i].n; }
        if (rows <= 0) { return rk_mk_arr(NULL, 0); }
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) rows);
        for (long long row = 0; row < rows; row++) {
            long long rem = row;
            DESCR_t *rv = (DESCR_t *) rt_ws_alloc_descr((size_t) nargs);
            for (int i = nargs - 1; i >= 0; i--) { long long pk; if (zip) pk = row; else { pk = rem % vs[i].n; rem /= vs[i].n; } rv[i] = vs[i].el[pk]; }
            r[row] = rk_mark_list(rk_mk_arr(rv, nargs));
        }
        return rk_mark_list(rk_mk_arr(r, (int) rows));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_cross(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cross(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_zip(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_cross(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_pre(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) { if (!rk_pre_value(rk_cstr(args[0]), out)) *out = NULVCL; return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_to_hash(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) {
        if (args[0].v == DT_T && args[0].tbl) *out = rk_hash_copy(args[0]);
        else if (args[0].v == DT_A && args[0].arr) {
            rk_av_t a = rk_av(args[0]);
            if (a.n == 1 && a.el[0].v == DT_A && a.el[0].arr && !rk_is_pair(a.el[0])) a = rk_av(a.el[0]);
            *out = rk_hash_from_list(a.el, a.n);
        } else if (args[0].v == DT_SNUL) *out = TABLE_VAL(table_new());
        else if (rk_is_cell(args[0])) {
            DESCR_t c = *(DESCR_t *) args[0].ptr;
            if (c.v == DT_SNUL) *out = TABLE_VAL(table_new());
            else if (c.v == DT_T || c.v == DT_A) *out = args[0];
            else *out = rk_hash_from_list(&c, 1);
        } else *out = rk_hash_from_list(&args[0], 1);
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_pick(DESCR_t *args, int nargs) {
    if (nargs >= 1) {
        int listargs = nargs, noarg = 1;
        long long n = 1;
        if (nargs >= 2 && (rk_int_or_rat(args[nargs - 1]) || (rk_typeobj_name(args[nargs - 1]) && !strcmp(rk_typeobj_name(args[nargs - 1]), "Whatever")))) {
            listargs = nargs - 1;
            noarg = 0;
            n = rk_int_or_rat(args[nargs - 1]) ? rk_int_arg(args[nargs - 1], 0) : 0x7fffffff;
        }
        rk_av_t a = rk_av_args(args, 0, listargs);
        int m = a.n;
        DESCR_t *r = m ? (DESCR_t *) rt_ws_alloc_descr((size_t) m) : NULL;
        for (int i = 0; i < m; i++) r[i] = rk_av_elem(a, i);
        if (n > m) n = m;
        if (n < 0) n = 0;
        for (int i = 0; i < n; i++) { int j = i + (int) (random() % (m - i)); DESCR_t t = r[i]; r[i] = r[j]; r[j] = t; }
        if (noarg) { return m ? r[0] : NULVCL; }
        return rk_mk_arr(r, (int) n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_head(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        int is_tail = ((_bop == 1));
        long long n = 1;
        int listargs = nargs;
        if (nargs >= 2) {
            char nb[64];
            const char *ns = to_cstring(args[nargs - 1], nb, sizeof nb);
            char *ep;
            long long v = strtoll(ns ? ns : "", &ep, 10);
            if (ns && ep > ns) n = v;
            if (rk_typeobj_name(args[nargs - 1]) && !strcmp(rk_typeobj_name(args[nargs - 1]), "Whatever")) n = 0x7fffffff;
            listargs = nargs - 1;
        }
        rk_av_t a = rk_av_args(args, 0, listargs);
        if (nargs == 1) return a.n ? a.el[is_tail ? a.n - 1 : 0] : NULVCL;
        if (n < 0) n = 0;
        int lo = is_tail ? (int) (a.n - n < 0 ? 0 : a.n - n) : 0, hi = is_tail ? a.n : (int) (n < a.n ? n : a.n);
        return rk_mk_arr(a.el + lo, hi - lo);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_tail(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_head(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_head(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_head(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_unique(DESCR_t *args, int nargs) {
    if (nargs >= 1) {
        rk_av_t a = rk_av_args(args, 0, nargs);
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
        int k = 0;
        for (int i = 0; i < a.n; i++) { const char *t = rk_av_text(a, i); int dup = 0; for (int j = 0; j < k && !dup; j++) if (!strcmp(tx[j], t)) dup = 1; if (!dup) { tx[k] = t; r[k++] = a.el[i]; } }
        return rk_mk_arr(r, k);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_arr_min(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        int tot = 0;
        rk_av_t vs[nargs > 0 ? nargs : 1];
        for (int i = 0; i < nargs; i++) { vs[i] = rk_av(args[i]); tot += vs[i].n; }
        DESCR_t *all = tot ? (DESCR_t *) rt_ws_alloc_descr((size_t) tot) : NULL;
        int k = 0;
        for (int i = 0; i < nargs; i++) for (int j = 0; j < vs[i].n; j++) all[k++] = vs[i].el[j];
        rk_av_t a = { tot, all, 1 };
        return rk_av_minmax(a, (_bop == 1));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_min(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_min(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_max(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_min(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_hyper_zip(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        rk_hy_t h;
        memset(&h, 0, sizeof h);
        h.fl = (int) args[0].i;
        h.mode = 0;
        rk_hy_rec(&h, args[1], args[2]);
        if (h.err) { extern void rt_script_die_surface(const char *msg); rt_script_die_surface("Lists on either side of non-dwimmy hyperop are not of the same length"); return FAILDESCR; }
        return rk_mark_list(rk_mk_arr(h.buf, h.n));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_hyper_shape(DESCR_t *args, int nargs) {
    if (nargs == 4) {
        DESCR_t mapped = args[1];
        if ((args[2].v == DT_SNUL && args[3].v == DT_SNUL) || !(mapped.v == DT_A && mapped.arr)) { return mapped; }
        rk_av_t ma = rk_av(mapped);
        rk_hy_t h;
        memset(&h, 0, sizeof h);
        h.fl = (int) args[0].i;
        h.mode = 1;
        h.vals = ma.el;
        h.nv = ma.n;
        return rk_hy_rec(&h, args[2], args[3]);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_hyper_meth(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 2) {
        rk_av_t a = rk_av(args[0]);
        DESCR_t *fa = rt_ws_alloc_descr((size_t)nargs);
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) {
            fa[0] = rk_av_elem(a, i);
            for (int k = 1; k < nargs; k++) fa[k] = args[k];
            DESCR_t rd = FAILDESCR;
            if (!script_try_call_builtin_by_name("meth_call", fa, nargs, &rd)) rd = FAILDESCR;
            r[i] = rd;
        }
        *out = rk_mk_arr(r, a.n);
        if (args[0].v == DT_A && args[0].arr && !((ARBLK_t *) args[0].arr)->proto) *out = rk_unmark(*out);
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_sort_by_keys(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        rk_av_t a = rk_av(args[0]), k = rk_av(args[1]);
        int n = a.n < k.n ? a.n : k.n, m = n ? n : 1;
        int ix[m], tm[m];
        DESCR_t r[m];
        for (int i = 0; i < n; i++) ix[i] = i;
        for (int w = 1; w < n; w *= 2) {
            for (int lo = 0; lo < n; lo += 2 * w) {
                int mid = lo + w < n ? lo + w : n, hi = lo + 2 * w < n ? lo + 2 * w : n, i = lo, j = mid, o = lo;
                while (i < mid && j < hi) {
                    DESCR_t x = k.el[ix[j]], y = k.el[ix[i]];
                    long long c = rk_order_both_numeric(x, y) ? rk_order_cmp_num(x, y) : rk_order_cmp_str(x, y);
                    tm[o++] = c < 0 ? ix[j++] : ix[i++];
                }
                while (i < mid) tm[o++] = ix[i++];
                while (j < hi) tm[o++] = ix[j++];
            }
            for (int i = 0; i < n; i++) ix[i] = tm[i];
        }
        for (int i = 0; i < n; i++) r[i] = a.el[ix[i]];
        return rk_mk_arr(r, n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_to_array(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) {
        if (rk_is_seq(args[0])) { return args[0]; }
        if (args[0].v == DT_A && args[0].arr) {
            rk_av_t a = rk_av(args[0]);
            *out = rk_unmark(rk_mk_arr(a.el, a.n));
            ARBLK_t * ab = (ARBLK_t *) out->arr;
            for (int i = 0; i < a.n; i++) if (ab->data[i].v == DT_SNUL) ab->data[i] = rk_typeobj("Any");
        } else *out = args[0].v == DT_SNUL ? rk_unmark(rk_mk_arr(NULL, 0)) : args[0];
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_slip(DESCR_t *args, int nargs) {
    if (nargs >= 1) { rk_av_t a = rk_av(args[0]); DESCR_t s = rk_mk_arr(a.el, a.n); ((ARBLK_t *) s.arr)->proto = rk_proto_slip; return s; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
RT_ENTRY_DECL(rt_arg_stage_d, leaf, 0);
void rt_arg_stage_d(const DESCR_t * cnt, const DESCR_t * val, DESCR_t * out, long spread) {
    extern void rt_arg_stage(int idx, DESCR_t v);
    extern void rt_script_die_surface(const char * msg);
    long k = (cnt && cnt->v == DT_I) ? (long) cnt->i : -1;
    if (k < 0 || !val) { *out = FAILDESCR; return; }
    if (!spread) { rt_arg_stage((int) k, *val); *out = INTVAL(k + 1); return; }
    if (val->v != DT_A || !val->arr || ((ARBLK_t *) val->arr)->proto != rk_proto_slip) { rt_script_die_surface("a | argument is not a reified list"); *out = FAILDESCR; return; }
    rk_av_t a = rk_av(*val);
    for (int i = 0; i < a.n; i++) rt_arg_stage((int) (k + i), a.el[i]);
    *out = INTVAL(k + a.n);
}
static DESCR_t bta___rk_arr_kv(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) {
        rk_av_t a = rk_av(args[0]);
        int kv = 1 && (_bop == 0), m = kv ? 2 * a.n : a.n;
        DESCR_t *r = m ? (DESCR_t *) rt_ws_alloc_descr((size_t) m) : NULL;
        for (int i = 0; i < a.n; i++) { if (kv) { r[2 * i] = INTVAL(i); r[2 * i + 1] = a.el[i]; } else r[i] = INTVAL(i); }
        return rk_mk_arr(r, m);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_arr_keys(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_kv(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_kv(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_arr_kv(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_sum(DESCR_t *args, int nargs) { if (nargs >= 1) { rk_av_t a = rk_av_args(args, 0, nargs); return a.n ? rk_av_arith(a, '+') : INTVAL(0); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_reduce_add(DESCR_t *args, int nargs, int _bop) {
    if (nargs >= 1) { int op = (_bop == 0) ? '+' : (_bop == 2) ? '*' : '-'; rk_av_t a = rk_av(args[0]); return a.n ? rk_av_arith(a, op) : INTVAL(op == '*' ? 1 : 0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_reduce_sub(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_reduce_add(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_reduce_mul(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_reduce_add(args, nargs, 2); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_reduce_add(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_reduce_add(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rk_reverse(DESCR_t *args, int nargs) {
    if (nargs >= 1) {
        rk_av_t a = rk_av_args(args, 0, nargs);
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) r[i] = a.el[a.n - 1 - i];
        return rk_mk_arr(r, a.n);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
