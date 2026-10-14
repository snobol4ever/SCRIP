#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "descr.h"
#include "core.h"
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen);
DESCR_t rt_pas_resundef(DESCR_t *, int);
DESCR_t rt_pas_sin(DESCR_t *, int);
DESCR_t rt_pas_cos(DESCR_t *, int);
DESCR_t rt_pas_exp(DESCR_t *, int);
DESCR_t rt_pas_sqrt(DESCR_t *, int);
DESCR_t rt_pas_ln(DESCR_t *, int);
DESCR_t rt_pas_arctan(DESCR_t *, int);
DESCR_t rt_pas_sqr(DESCR_t *, int);
DESCR_t rt_rk_arr_at(DESCR_t *, int);
DESCR_t rt_rk_arr_set(DESCR_t *, int);
DESCR_t rt_rk_elem_put(DESCR_t *, int);
static int fails = 0, ncase = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int same(DESCR_t a, DESCR_t b);
static int same(DESCR_t a, DESCR_t b) {
    if (a.v != b.v) return 0;
    if (a.v == DT_SNUL) return a.slen == b.slen;
    if (a.v == DT_S && a.s && b.s) return a.slen == b.slen && !memcmp(a.s, b.s, a.slen);
    if (a.v == DT_A && a.arr && b.arr) {
        ARBLK_t *x = a.arr, *y = b.arr;
        if (x == y) return 1;
        if (x->lo != y->lo || x->hi != y->hi || x->ndim != y->ndim) return 0;
        for (int i = 0; i <= x->hi - x->lo; i++) if (!same(x->data[i], y->data[i])) return 0;
        return 1;
    }
    if (a.v != DT_I && a.v != DT_R && a.v != DT_S && a.v != DT_SNUL && a.v == b.v && a.slen == b.slen) return 1;
    return memcmp(&a, &b, sizeof a) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cmp1(const char *fn, DESCR_t (*direct)(DESCR_t *, int), DESCR_t x) {
    DESCR_t a1[1] = { x }, a2[1] = { x };
    DESCR_t d = direct(a1, 1), r = rt_call_arr_bl(fn, a2, 1, -1);
    ncase++;
    if (!same(d, r)) { fails++; printf("  FAIL %s: direct %02x/%016llx by-name %02x/%016llx\n", fn, d.v & 0xff, (unsigned long long)d.i, r.v & 0xff, (unsigned long long)r.i); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ARBLK_t *mk(int n, int base) { ARBLK_t *a = array_new(0, n - 1); for (int i = 0; i < n; i++) a->data[i] = INTVAL(base + i); return a; }
static DESCR_t darr(ARBLK_t *a) { DESCR_t d = { .v = DT_A }; d.arr = a; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    core_lib_init();
    DESCR_t xs[] = { INTVAL(0), INTVAL(1), INTVAL(-3), INTVAL(7), INTVAL(46340), INTVAL(-46340), REALVAL(0.0), REALVAL(-0.0), REALVAL(0.5), REALVAL(-2.5), REALVAL(1e10), NULVCL, STRVAL("12") };
    int nx = (int)(sizeof xs / sizeof xs[0]);
    for (int i = 0; i < nx; i++) {
        cmp1("__pas_resundef", rt_pas_resundef, xs[i]);
        cmp1("__pas_sin", rt_pas_sin, xs[i]);
        cmp1("__pas_cos", rt_pas_cos, xs[i]);
        cmp1("__pas_exp", rt_pas_exp, xs[i]);
        cmp1("__pas_arctan", rt_pas_arctan, xs[i]);
        cmp1("__pas_sqr", rt_pas_sqr, xs[i]);
        if (xs[i].v == DT_I ? xs[i].i >= 0 : (xs[i].v == DT_R ? xs[i].r >= 0 : 1)) cmp1("__pas_sqrt", rt_pas_sqrt, xs[i]);
        if (xs[i].v == DT_I ? xs[i].i > 0 : (xs[i].v == DT_R ? xs[i].r > 0 : 0)) cmp1("__pas_ln", rt_pas_ln, xs[i]);
    }
    DESCR_t idx[] = { INTVAL(0), INTVAL(3), INTVAL(7), INTVAL(8), INTVAL(20), INTVAL(-1), REALVAL(2.0), STRVAL("4"), NULVCL };
    int ni = (int)(sizeof idx / sizeof idx[0]);
    for (int k = 0; k < ni; k++) {
        ARBLK_t *a1 = mk(8, 100), *a2 = mk(8, 100);
        DESCR_t p[2] = { darr(a1), idx[k] }, q[2] = { darr(a2), idx[k] };
        DESCR_t d = rt_rk_arr_at(p, 2), r = rt_call_arr_bl("__rk_arr_at", q, 2, -1);
        ncase++;
        if (!same(d, r)) { fails++; printf("  FAIL __rk_arr_at idx %d\n", k); }
        ARBLK_t *b1 = mk(8, 100), *b2 = mk(8, 100);
        DESCR_t s1[3] = { darr(b1), idx[k], INTVAL(-9) }, s2[3] = { darr(b2), idx[k], INTVAL(-9) };
        d = rt_rk_arr_set(s1, 3);
        r = rt_call_arr_bl("__rk_arr_set", s2, 3, -1);
        ncase++;
        if (!same(d, r) || !same(darr(b1), darr(b2))) { fails++; printf("  FAIL __rk_arr_set idx %d\n", k); }
        ARBLK_t *c1 = mk(8, 100), *c2 = mk(8, 100);
        DESCR_t e1[4] = { STRVAL("a"), darr(c1), idx[k], INTVAL(-7) }, e2[4] = { STRVAL("a"), darr(c2), idx[k], INTVAL(-7) };
        d = rt_rk_elem_put(e1, 4);
        r = rt_call_arr_bl("__rk_elem_put", e2, 4, -1);
        ncase++;
        if (!same(d, r) || !same(darr(c1), darr(c2))) { fails++; printf("  FAIL __rk_elem_put idx %d\n", k); }
    }
    printf("RTX DIRECT: %d cases, %d mismatches (the direct builtin entries against the by-name road)\n", ncase, fails);
    printf("RTX DIRECT UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
