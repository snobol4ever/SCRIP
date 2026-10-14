#include "core.h"
#include "rt/rt.h"
#include "pl_rat.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
extern DESCR_t rt_big_mod(DESCR_t, DESCR_t);
extern DESCR_t rt_big_neg(DESCR_t);
extern DESCR_t rt_big_pow(DESCR_t, int64_t);
extern DESCR_t rt_big_from_str(const char *);
extern char *rt_big_str(DESCR_t);
extern long rt_big_bits(DESCR_t);
extern int rt_big_sign(DESCR_t);
extern int rt_big_cmp(DESCR_t, DESCR_t);
extern int rt_big_eq(DESCR_t, DESCR_t);
extern DESCR_t rk_rat_new(DESCR_t, DESCR_t, int);
extern int rk_rat_is(DESCR_t);
extern int rk_rat_to_real(DESCR_t, double *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pq_int(DESCR_t a) { return a.v == DT_I || a.v == DT_BIG; }
static int pq_sgn(DESCR_t a) { return rt_big_sign(a); }
static int pq_zero(DESCR_t a) { return a.v == DT_I && a.i == 0; }
static int pq_one(DESCR_t a) { return a.v == DT_I && a.i == 1; }
static DESCR_t pq_add(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_add_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_add(a, b); }
static DESCR_t pq_sub(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_sub_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_sub(a, b); }
static DESCR_t pq_mul(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_mul_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_mul(a, b); }
static DESCR_t pq_neg(DESCR_t a) { if (a.v == DT_I && a.i != INT64_MIN) return INTVAL(-a.i); return rt_big_neg(a); }
static DESCR_t pq_abs(DESCR_t a) { return pq_sgn(a) < 0 ? pq_neg(a) : a; }
static int pq_same(DESCR_t a, DESCR_t b) { if (a.v == DT_I || b.v == DT_I) return a.v == b.v && a.i == b.i; return rt_big_eq(a, b); }
static int pq_cmp(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I) return (a.i > b.i) - (a.i < b.i); return rt_big_cmp(a, b); }
static DESCR_t pq_divt(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I && !(a.i == INT64_MIN && b.i == -1)) return INTVAL(a.i / b.i); return rt_big_div(a, b); }
static DESCR_t pq_modt(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I) return INTVAL(b.i == -1 ? 0 : a.i % b.i); return rt_big_mod(a, b); }
static DESCR_t pq_floordiv(DESCR_t a, DESCR_t b) { DESCR_t q = pq_divt(a, b), r = pq_modt(a, b); if (!pq_zero(r) && pq_sgn(r) != pq_sgn(b)) q = pq_sub(q, INTVAL(1)); return q; }
static long pq_bits(DESCR_t a) { if (a.v == DT_I) { uint64_t v = a.i < 0 ? (uint64_t)0 - (uint64_t)a.i : (uint64_t)a.i; return v ? 64 - __builtin_clzll(v) : 0; } return rt_big_bits(a); }
static DESCR_t pq_pow(DESCR_t a, int64_t e) {
    int64_t acc = 1, base = a.i, k = e;
    int ovf = 0;
    if (e == 0) return INTVAL(1);
    if (a.v != DT_I) return rt_big_pow(a, e);
    while (k > 0 && !ovf) { if (k & 1) ovf |= __builtin_mul_overflow(acc, base, &acc); k >>= 1; if (k) ovf |= __builtin_mul_overflow(base, base, &base); }
    return ovf ? rt_big_pow(a, e) : INTVAL(acc);
}
static DESCR_t pq_gcd(DESCR_t a, DESCR_t b) {
    a = pq_abs(a);
    b = pq_abs(b);
    if (a.v == DT_I && b.v == DT_I) { uint64_t x = (uint64_t)a.i, y = (uint64_t)b.i; while (y) { uint64_t t = x % y; x = y; y = t; } return INTVAL((int64_t)x); }
    while (!pq_zero(b)) { DESCR_t t = pq_modt(a, b); a = b; b = t; }
    return a;
}
static void pq_parts(DESCR_t a, DESCR_t *n, DESCR_t *d) { if (pl_rat_is(a)) { *n = a.u->fields[0]; *d = a.u->fields[1]; } else { *n = a; *d = INTVAL(1); } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rat_is(DESCR_t a) { return a.v == DT_DATA && rk_rat_is(a); }
int pl_rat_exact(DESCR_t a) { return pq_int(a) || pl_rat_is(a); }
DESCR_t pl_rat_make(DESCR_t n, DESCR_t d) {
    DESCR_t g;
    if (pq_sgn(d) < 0) { n = pq_neg(n); d = pq_neg(d); }
    g = pq_gcd(n, d);
    if (!pq_one(g) && !pq_zero(g)) { n = pq_divt(n, g); d = pq_divt(d, g); }
    if (pq_one(d)) return n;
    return rk_rat_new(n, d, 1);
}
double pl_rat_real(DESCR_t a) {
    double x = 0.0;
    if (a.v == DT_I) return (double)a.i;
    if (a.v == DT_BIG) { const char *s = rt_big_str(a); return s ? strtod(s, (char **)0) : 0.0; }
    if (rk_rat_to_real(a, &x)) return x;
    return 0.0;
}
int pl_rat_cmp(DESCR_t a, DESCR_t b) { DESCR_t an, ad, bn, bd; pq_parts(a, &an, &ad); pq_parts(b, &bn, &bd); return pq_cmp(pq_mul(an, bd), pq_mul(bn, ad)); }
int pl_rat_eq(DESCR_t a, DESCR_t b) { DESCR_t an, ad, bn, bd; pq_parts(a, &an, &ad); pq_parts(b, &bn, &bd); return pq_same(an, bn) && pq_same(ad, bd); }
DESCR_t pl_rat_num(DESCR_t a) { DESCR_t n, d; pq_parts(a, &n, &d); return n; }
DESCR_t pl_rat_den(DESCR_t a) { DESCR_t n, d; pq_parts(a, &n, &d); return d; }
DESCR_t pl_rat_scaled(DESCR_t a, long places, int *neg) {
    DESCR_t n, d, two = INTVAL(2);
    pq_parts(a, &n, &d);
    *neg = pq_sgn(n) < 0;
    n = pq_mul(pq_mul(pq_abs(n), pq_pow(INTVAL(10), places)), two);
    return pq_divt(pq_add(n, d), pq_mul(d, two));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pl_rat_parse(const char *s) {
    const char *r = s ? strchr(s, 'r') : (const char *)0;
    if (!r || r == s || !r[1]) return FAILDESCR;
    {
        char nb[r - s + 1];
        DESCR_t n, d;
        memcpy(nb, s, (size_t)(r - s));
        nb[r - s] = 0;
        n = rt_big_from_str(nb);
        d = rt_big_from_str(r + 1);
        if (n.v == DT_FAIL || d.v == DT_FAIL || pq_zero(d)) return FAILDESCR;
        return pl_rat_make(n, d);
    }
}
static const char *pq_text(DESCR_t a, char *buf, size_t cap) { if (a.v == DT_I) { snprintf(buf, cap, "%lld", (long long)a.i); return buf; } return rt_big_str(a); }
void pl_rat_fput(DESCR_t a, FILE *fp) { char b1[24], b2[24]; DESCR_t n, d; pq_parts(a, &n, &d); fputs(pq_text(n, b1, sizeof b1), fp); fputc('r', fp); fputs(pq_text(d, b2, sizeof b2), fp); }
size_t pl_rat_text(DESCR_t a, char *out, size_t cap) {
    char b1[24], b2[24];
    DESCR_t n, d;
    pq_parts(a, &n, &d);
    return (size_t)snprintf(out, cap, "%sr%s", pq_text(n, b1, sizeof b1), pq_text(d, b2, sizeof b2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rat_from_double(double x, DESCR_t *out) {
    int e;
    double m;
    int64_t mm;
    DESCR_t n;
    if (isnan(x) || isinf(x)) return 0;
    if (x == 0.0) { *out = INTVAL(0); return 1; }
    m = frexp(fabs(x), &e);
    mm = (int64_t)ldexp(m, 53);
    n = INTVAL(x < 0 ? -mm : mm);
    e -= 53;
    *out = e >= 0 ? pq_mul(n, pq_pow(INTVAL(2), e)) : pl_rat_make(n, pq_pow(INTVAL(2), -e));
    return 1;
}
int pl_rat_rationalize(double x, DESCR_t *out) {
    DESCR_t ex, n, d, p1 = INTVAL(1), p2 = INTVAL(0), q1 = INTVAL(0), q2 = INTVAL(1);
    double ax = fabs(x);
    if (!pl_rat_from_double(ax, &ex)) return 0;
    pq_parts(ex, &n, &d);
    for (;;) {
        DESCR_t a = pq_divt(n, d), p = pq_add(pq_mul(a, p1), p2), q = pq_add(pq_mul(a, q1), q2), r = pl_rat_make(p, q), rem = pq_sub(n, pq_mul(a, d));
        if (pl_rat_real(r) == ax || pq_zero(rem)) { *out = x < 0 ? pl_rat_make(pq_neg(p), q) : r; return 1; }
        n = d;
        d = rem;
        p2 = p1;
        p1 = p;
        q2 = q1;
        q1 = q;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pq_iroot(DESCR_t n, int64_t k, int *exact) {
    DESCR_t x, y;
    if (pq_zero(n) || pq_one(n)) { *exact = 1; return n; }
    if (n.v == DT_I && n.i < ((int64_t)1 << 52)) {
        double r0 = floor(pow((double)n.i, 1.0 / (double)k) + 0.5);
        for (double c = r0 - 1 < 0 ? 0 : r0 - 1; c <= r0 + 1; c += 1) { int s = pq_cmp(pq_pow(INTVAL((int64_t)c), k), n); if (s == 0) { *exact = 1; return INTVAL((int64_t)c); } }
        *exact = 0;
        return INTVAL((int64_t)r0);
    }
    x = pq_pow(INTVAL(2), pq_bits(n) / k + 1);
    for (;;) { y = pq_divt(pq_add(pq_mul(INTVAL(k - 1), x), pq_divt(n, pq_pow(x, k - 1))), INTVAL(k)); if (pq_cmp(y, x) >= 0) break; x = y; }
    *exact = pq_cmp(pq_pow(x, k), n) == 0;
    return x;
}
static int pq_ipow(DESCR_t bn, DESCR_t bd, DESCR_t e, DESCR_t *out, int *err) {
    int64_t k;
    if (e.v != DT_I) { if (pq_one(bd) && (pq_zero(bn) || pq_one(bn))) { if (pq_zero(bn) && pq_sgn(e) < 0) { *err = PLQ_ZDIV; return 1; } *out = bn; return 1; } return 0; }
    k = e.i;
    if (k >= 0) { *out = pl_rat_make(pq_pow(bn, k), pq_pow(bd, k)); return 1; }
    if (pq_zero(bn)) { *err = PLQ_ZDIV; return 1; }
    if (k == INT64_MIN) return 0;
    *out = pl_rat_make(pq_pow(bd, -k), pq_pow(bn, -k));
    return 1;
}
int pl_rat_pow(DESCR_t b, DESCR_t e, DESCR_t *out, int *err) {
    DESCR_t bn, bd, p, q, rn, rd;
    int neg, xn = 0, xd = 0;
    pq_parts(b, &bn, &bd);
    if (pq_int(e)) return pq_ipow(bn, bd, e, out, err);
    pq_parts(e, &p, &q);
    if (pq_zero(bn)) { if (pq_sgn(p) > 0) *out = INTVAL(0); else *err = PLQ_ZDIV; return 1; }
    if (pq_one(bn) && pq_one(bd)) { *out = INTVAL(1); return 1; }
    neg = pq_sgn(bn) < 0;
    if (q.v != DT_I) return 0;
    if (neg && !(q.i & 1)) { *err = PLQ_UNDEF; return 1; }
    rn = pq_iroot(pq_abs(bn), q.i, &xn);
    rd = pq_iroot(bd, q.i, &xd);
    if (xn && xd) return pq_ipow(neg ? pq_neg(rn) : rn, rd, p, out, err);
    { double v = pow(pl_rat_real(pq_abs(b)), pl_rat_real(e)); if (neg && p.v == DT_I && (p.i & 1)) v = -v; *out = REALVAL(v); return 1; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_rat_unop(const char *op, DESCR_t a, DESCR_t *out, int *err) {
    DESCR_t n, d;
    if (!pl_rat_exact(a)) {
        if (a.v != DT_R) return 0;
        if (!strcmp(op, "rat")) { if (!pl_rat_from_double(a.r, out)) *err = PLQ_UNDEF; return 1; }
        if (!strcmp(op, "ratz")) { if (!pl_rat_rationalize(a.r, out)) *err = PLQ_UNDEF; return 1; }
        if (!strcmp(op, "numer") || !strcmp(op, "denom")) { *err = PLQ_TYPE_RAT; return 1; }
        return 0;
    }
    pq_parts(a, &n, &d);
    if (!strcmp(op, "rat") || !strcmp(op, "ratz") || !strcmp(op, "pos")) { *out = a; return 1; }
    if (!strcmp(op, "numer")) { *out = n; return 1; }
    if (!strcmp(op, "denom")) { *out = d; return 1; }
    if (!pl_rat_is(a)) return 0;
    if (!strcmp(op, "neg")) { *out = pl_rat_make(pq_neg(n), d); return 1; }
    if (!strcmp(op, "abs")) { *out = pl_rat_make(pq_abs(n), d); return 1; }
    if (!strcmp(op, "sign")) { *out = INTVAL(pq_sgn(n)); return 1; }
    if (!strcmp(op, "trunc")) { *out = pq_divt(n, d); return 1; }
    if (!strcmp(op, "floor")) { *out = pq_floordiv(n, d); return 1; }
    if (!strcmp(op, "ceil")) { *out = pq_neg(pq_floordiv(pq_neg(n), d)); return 1; }
    if (!strcmp(op, "round") || !strcmp(op, "intg")) { DESCR_t r = pq_floordiv(pq_add(pq_mul(INTVAL(2), pq_abs(n)), d), pq_mul(INTVAL(2), d)); *out = pq_sgn(n) < 0 ? pq_neg(r) : r; return 1; }
    if (!strcmp(op, "flt")) { *out = REALVAL(pl_rat_real(a)); return 1; }
    return 0;
}
int pl_rat_binop(const char *op, DESCR_t a, DESCR_t b, DESCR_t *out, int *err, DESCR_t *culprit) {
    DESCR_t an, ad, bn, bd;
    if (!pl_rat_exact(a) || !pl_rat_exact(b)) {
        if (!strcmp(op, "copysign") && pl_rat_is(a) && b.v == DT_R) { pq_parts(a, &an, &ad); *out = pl_rat_make(signbit(b.r) ? pq_neg(pq_abs(an)) : pq_abs(an), ad); return 1; }
        if (!strcmp(op, "rdiv")) { *err = PLQ_TYPE_RAT; *culprit = pl_rat_exact(a) ? b : a; return 1; }
        return 0;
    }
    pq_parts(a, &an, &ad);
    pq_parts(b, &bn, &bd);
    if (!strcmp(op, "add")) { *out = pl_rat_make(pq_add(pq_mul(an, bd), pq_mul(bn, ad)), pq_mul(ad, bd)); return 1; }
    if (!strcmp(op, "sub")) { *out = pl_rat_make(pq_sub(pq_mul(an, bd), pq_mul(bn, ad)), pq_mul(ad, bd)); return 1; }
    if (!strcmp(op, "mul")) { *out = pl_rat_make(pq_mul(an, bn), pq_mul(ad, bd)); return 1; }
    if (!strcmp(op, "div") || !strcmp(op, "rdiv")) { if (pq_zero(bn)) { *err = PLQ_ZDIV; return 1; } *out = pl_rat_make(pq_mul(an, bd), pq_mul(ad, bn)); return 1; }
    if (!strcmp(op, "min")) { *out = pl_rat_cmp(a, b) <= 0 ? a : b; return 1; }
    if (!strcmp(op, "max")) { *out = pl_rat_cmp(a, b) >= 0 ? a : b; return 1; }
    if (!strcmp(op, "fpow") || !strcmp(op, "pow")) return pl_rat_pow(a, b, out, err);
    if (!strcmp(op, "copysign")) { if (!pl_rat_is(a)) return 0; *out = pl_rat_make(pq_sgn(bn) < 0 ? pq_neg(pq_abs(an)) : pq_abs(an), ad); return 1; }
    if (!strcmp(op, "idiv") || !strcmp(op, "divf") || !strcmp(op, "mod") || !strcmp(op, "rem") || !strcmp(op, "gcd") || !strcmp(op, "shr") || !strcmp(op, "shl") || !strcmp(op, "band") ||
        !strcmp(op, "bor") || !strcmp(op, "xor")) {
        *err = PLQ_TYPE_INT;
        *culprit = pl_rat_is(a) ? a : b;
        return 1;
    }
    return 0;
}
