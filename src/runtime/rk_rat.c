#include "core.h"
#include "rt/rt.h"
#include "rt/gc_heap.h"
#include "builtins/gen.h"
#include "rt/rt_arena.h"
#include "dtp.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef __int128 i128;
typedef unsigned long long u64;
#define RK_RAT_NAME "\x01" "Rat"
#define RK_FATRAT_NAME "\x01" "FatRat"
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
extern void rt_script_die_surface(const char *msg);
DESCR_t rk_rat_new(DESCR_t n, DESCR_t d, int fat);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int bi_is(DESCR_t a) { return a.v == DT_I || a.v == DT_BIG; }
static int bi_sgn(DESCR_t a) { return rt_big_sign(a); }
static int bi_zero(DESCR_t a) { return a.v == DT_I && a.i == 0; }
static int bi_one(DESCR_t a) { return a.v == DT_I && a.i == 1; }
static DESCR_t bi_add(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_add_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_add(a, b); }
static DESCR_t bi_sub(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_sub_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_sub(a, b); }
static DESCR_t bi_mul(DESCR_t a, DESCR_t b) { int64_t z; if (a.v == DT_I && b.v == DT_I && !__builtin_mul_overflow(a.i, b.i, &z)) return INTVAL(z); return rt_big_mul(a, b); }
static DESCR_t bi_neg(DESCR_t a) { if (a.v == DT_I && a.i != INT64_MIN) return INTVAL(-a.i); return rt_big_neg(a); }
static DESCR_t bi_abs(DESCR_t a) { return bi_sgn(a) < 0 ? bi_neg(a) : a; }
static int bi_cmp(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I) return (a.i > b.i) - (a.i < b.i); return rt_big_cmp(a, b); }
static DESCR_t bi_divt(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I && !(a.i == INT64_MIN && b.i == -1)) return INTVAL(a.i / b.i); return rt_big_div(a, b); }
static DESCR_t bi_modt(DESCR_t a, DESCR_t b) { if (a.v == DT_I && b.v == DT_I && b.i != -1) return INTVAL(a.i % b.i); if (a.v == DT_I && b.v == DT_I) return INTVAL(0); return rt_big_mod(a, b); }
static DESCR_t bi_floordiv(DESCR_t a, DESCR_t b) { DESCR_t q = bi_divt(a, b), r = bi_modt(a, b); if (!bi_zero(r) && bi_sgn(r) != bi_sgn(b)) q = bi_sub(q, INTVAL(1)); return q; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static u64 gcd64(u64 a, u64 b) { while (b) { u64 t = a % b; a = b; b = t; } return a; }
static DESCR_t bi_gcd(DESCR_t a, DESCR_t b) {
    if (a.v == DT_I && b.v == DT_I) {
        u64 x = a.i < 0 ? (u64) 0 - (u64) a.i : (u64) a.i, y = b.i < 0 ? (u64) 0 - (u64) b.i : (u64) b.i;
        u64 g = gcd64(x, y);
        if (g <= (u64) INT64_MAX) return INTVAL((int64_t) g);
    }
    a = bi_abs(a);
    b = bi_abs(b);
    while (!bi_zero(b)) { DESCR_t t = bi_modt(a, b); a = b; b = t; }
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bi_from_i128(i128 v) {
    if (v >= (i128) INT64_MIN && v <= (i128) INT64_MAX) return INTVAL((int64_t) v);
    char buf[48];
    int k = 0, neg = v < 0;
    unsigned __int128 u = neg ? (unsigned __int128) 0 - (unsigned __int128) v : (unsigned __int128) v;
    char rev[48];
    int m = 0;
    while (u) { rev[m++] = (char) ('0' + (int) (u % 10)); u /= 10; }
    if (neg) buf[k++] = '-';
    while (m) buf[k++] = rev[--m];
    buf[k] = 0;
    return rt_big_from_str(buf);
}
static DESCR_t bi_pow(DESCR_t a, int64_t e) {
    if (e == 0) return INTVAL(1);
    if (a.v == DT_I) {
        int64_t acc = 1, base = a.i;
        int64_t k = e;
        int ovf = 0;
        while (k > 0 && !ovf) { if (k & 1) ovf |= __builtin_mul_overflow(acc, base, &acc); k >>= 1; if (k) ovf |= __builtin_mul_overflow(base, base, &base); }
        if (!ovf) return INTVAL(acc);
    }
    return rt_big_pow(a, e);
}
static int bi_bits(DESCR_t a) { if (a.v == DT_I) { u64 v = a.i < 0 ? (u64) 0 - (u64) a.i : (u64) a.i; return v ? 64 - __builtin_clzll(v) : 0; } return (int) rt_big_bits(a); }
static int bi_lt_2p64(DESCR_t d) { return d.v == DT_I ? d.i >= 0 : (bi_sgn(d) > 0 && bi_bits(d) <= 64); }
static double bi_dbl(DESCR_t a) { if (a.v == DT_I) return (double) a.i; return strtod(rt_big_str(a), NULL); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double rq_dbl(DESCR_t n, DESCR_t d) {
    if (bi_zero(d)) return bi_zero(n) ? NAN : (bi_sgn(n) > 0 ? INFINITY : -INFINITY);
    if (bi_zero(n)) return 0.0;
    if (n.v == DT_I && d.v == DT_I) return (double) n.i / (double) d.i;
    int neg = (bi_sgn(n) < 0) != (bi_sgn(d) < 0);
    DESCR_t an = bi_abs(n), ad = bi_abs(d);
    int s = 55 - (bi_bits(an) - bi_bits(ad)) + 1;
    if (s > 0) an = bi_mul(an, bi_pow(INTVAL(2), s));
    else if (s < 0) ad = bi_mul(ad, bi_pow(INTVAL(2), -s));
    DESCR_t q = bi_divt(an, ad), r = bi_modt(an, ad);
    u64 qv = (u64) q.i;
    int nb = 64 - __builtin_clzll(qv), drop = nb - 53;
    u64 mant = qv >> drop;
    u64 guard = (qv >> (drop - 1)) & 1u;
    u64 rest = (qv & ((1ull << (drop - 1)) - 1)) | (bi_zero(r) ? 0u : 1u);
    if (guard && (rest || (mant & 1u))) mant++;
    double v = ldexp((double) mant, drop - s);
    return neg ? -v : v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rq_kind(DESCR_t v) {
    if (v.v != DT_DATA || !v.u || !IS_DATA_INST_fn(v) || !v.u->type || !v.u->type->name || v.u->type->name[0] != 1) return 0;
    const char *nm = v.u->type->name + 1;
    if (!strcmp(nm, "Rat")) return 1;
    if (!strcmp(nm, "FatRat")) return 2;
    return 0;
}
int rk_rat_is(DESCR_t d) { return rq_kind(d) != 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rq_new(DESCR_t n, DESCR_t d, int fat) {
    const char *nm = fat ? RK_FATRAT_NAME : RK_RAT_NAME;
    DESCR_t r = DATCON_fn(nm, n, d);
    if (r.v == DT_DATA) return r;
    DEFDAT_fn(fat ? RK_FATRAT_NAME "(numerator,denominator)" : RK_RAT_NAME "(numerator,denominator)");
    return DATCON_fn(nm, n, d);
}
static DESCR_t rq_make(DESCR_t n, DESCR_t d, int fat) {
    if (bi_sgn(d) < 0) { n = bi_neg(n); d = bi_neg(d); }
    if (!bi_one(d)) { DESCR_t g = bi_gcd(n, d); if (!bi_one(g) && !bi_zero(g)) { n = bi_divt(n, g); d = bi_divt(d, g); } }
    if (!fat && !bi_lt_2p64(d)) return REALVAL(rq_dbl(n, d));
    return rq_new(n, d, fat);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rq_get(DESCR_t v, DESCR_t *n, DESCR_t *d, int *fat) {
    if (v.v == DT_I || v.v == DT_BIG) { *n = v; *d = INTVAL(1); return 1; }
    if (v.v == DT_BOOL) { *n = INTVAL(v.i != 0); *d = INTVAL(1); return 1; }
    int k = rq_kind(v);
    if (!k) return 0;
    *n = v.u->fields[0];
    *d = v.u->fields[1];
    if (k == 2) *fat = 1;
    return 1;
}
int rk_rat_to_real(DESCR_t v, double *out) { if (!rq_kind(v)) return 0; *out = rq_dbl(v.u->fields[0], v.u->fields[1]); return 1; }
int rk_rat_to_int(DESCR_t v, long long *out) { if (!rq_kind(v)) return 0; DESCR_t q = bi_divt(v.u->fields[0], v.u->fields[1]); *out = q.v == DT_I ? (long long) q.i : (long long) bi_dbl(q); return 1; }
int rk_rat_truthy(DESCR_t v) { return !bi_zero(v.u->fields[0]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rq_bool(int b) { return (DESCR_t) { .v = DT_BOOL, .i = b != 0 }; }
static void rq_dz(const char *op) { char msg[96]; snprintf(msg, sizeof msg, "Attempt to divide by zero using %s", op); rt_script_die_surface(msg); }
static DESCR_t rq_pow(DESCR_t n, DESCR_t d, DESCR_t e, int fat) {
    if (e.v != DT_I) return REALVAL(pow(rq_dbl(n, d), bi_dbl(e)));
    int64_t k = e.i;
    int neg = k < 0;
    if (neg) { if (k == INT64_MIN) return REALVAL(pow(rq_dbl(n, d), (double) k)); k = -k; }
    DESCR_t pn = bi_pow(n, k), pd = bi_pow(d, k);
    if (neg) { if (bi_zero(pn)) { rq_dz("infix:<**>"); return FAILDESCR; } return rq_make(pd, pn, fat); }
    return rq_make(pn, pd, fat);
}
int rk_rat_binop(DESCR_t a, DESCR_t b, int op, DESCR_t *out) {
    if (!rq_kind(a) && !rq_kind(b)) return 0;
    DESCR_t an, ad, bn, bd;
    int fat = 0;
    int ae = rq_get(a, &an, &ad, &fat), be = rq_get(b, &bn, &bd, &fat);
    if (ae && be) {
        switch (op) {
            case BINOP_ADD:
            if (bi_cmp(ad, bd) == 0) *out = rq_make(bi_add(an, bn), ad, fat);
            else *out = rq_make(bi_add(bi_mul(an, bd), bi_mul(bn, ad)), bi_mul(ad, bd), fat);
            return 1;
            case BINOP_SUB:
            if (bi_cmp(ad, bd) == 0) *out = rq_make(bi_sub(an, bn), ad, fat);
            else *out = rq_make(bi_sub(bi_mul(an, bd), bi_mul(bn, ad)), bi_mul(ad, bd), fat);
            return 1;
            case BINOP_MUL:
            *out = rq_make(bi_mul(an, bn), bi_mul(ad, bd), fat);
            return 1;
            case BINOP_DIV:
            if (bi_zero(bn)) { rq_dz("infix:</>"); *out = FAILDESCR; return 1; }
            *out = rq_make(bi_mul(an, bd), bi_mul(ad, bn), fat);
            return 1;
            case BINOP_MOD:
            {
                if (bi_zero(bn)) { rq_dz("infix:<%>"); *out = FAILDESCR; return 1; }
                DESCR_t qn = bi_mul(an, bd), qd = bi_mul(ad, bn);
                DESCR_t fl = bi_floordiv(qn, qd);
                *out = rq_make(bi_sub(qn, bi_mul(bi_mul(fl, bn), ad)), bi_mul(ad, bd), fat);
                return 1;
            }
            case BINOP_POW:
            if (bi_one(bd)) { *out = rq_pow(an, ad, bn, fat); return 1; }
            break;
            case BINOP_LT:
            case BINOP_LE:
            case BINOP_GT:
            case BINOP_GE:
            case BINOP_EQ:
            case BINOP_NE:
            {
                int c = bi_cmp(bi_mul(an, bd), bi_mul(bn, ad));
                int r = op == BINOP_LT ? c < 0 : op == BINOP_LE ? c <= 0 : op == BINOP_GT ? c > 0 : op == BINOP_GE ? c >= 0 : op == BINOP_EQ ? c == 0 : c != 0;
                *out = rq_bool(r);
                return 1;
            }
            default:
            break;
        }
    }
    double x, y;
    if (ae) x = rq_dbl(an, ad);
    else if (IS_REAL_fn(a)) x = a.r;
    else return 0;
    if (be) y = rq_dbl(bn, bd);
    else if (IS_REAL_fn(b)) y = b.r;
    else return 0;
    switch (op) {
        case BINOP_ADD:
        *out = REALVAL(x + y);
        return 1;
        case BINOP_SUB:
        *out = REALVAL(x - y);
        return 1;
        case BINOP_MUL:
        *out = REALVAL(x * y);
        return 1;
        case BINOP_DIV:
        if (y == 0.0) { rq_dz("infix:</>"); *out = FAILDESCR; return 1; }
        *out = REALVAL(x / y);
        return 1;
        case BINOP_MOD:
        if (y == 0.0) { rq_dz("infix:<%>"); *out = FAILDESCR; return 1; }
        *out = REALVAL(x - y * floor(x / y));
        return 1;
        case BINOP_POW:
        *out = REALVAL(pow(x, y));
        return 1;
        case BINOP_LT:
        *out = rq_bool(x < y);
        return 1;
        case BINOP_LE:
        *out = rq_bool(x <= y);
        return 1;
        case BINOP_GT:
        *out = rq_bool(x > y);
        return 1;
        case BINOP_GE:
        *out = rq_bool(x >= y);
        return 1;
        case BINOP_EQ:
        *out = rq_bool(x == y);
        return 1;
        case BINOP_NE:
        *out = rq_bool(x != y);
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_cmp(DESCR_t a, DESCR_t b, int *c) {
    DESCR_t an, ad, bn, bd;
    int fat = 0;
    if (!rq_kind(a) && !rq_kind(b)) return 0;
    if (rq_get(a, &an, &ad, &fat) && rq_get(b, &bn, &bd, &fat)) { *c = bi_cmp(bi_mul(an, bd), bi_mul(bn, ad)); return 1; }
    double x, y;
    if (rq_get(a, &an, &ad, &fat)) x = rq_dbl(an, ad);
    else if (IS_REAL_fn(a)) x = a.r;
    else return 0;
    if (rq_get(b, &bn, &bd, &fat)) y = rq_dbl(bn, bd);
    else if (IS_REAL_fn(b)) y = b.r;
    else return 0;
    *c = x < y ? -1 : x > y ? 1 : 0;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_rat_div(DESCR_t a, DESCR_t b) {
    DESCR_t an, ad, bn, bd;
    int fat = 0;
    if (!rq_get(a, &an, &ad, &fat) || !rq_get(b, &bn, &bd, &fat)) return FAILDESCR;
    if (bi_zero(bn)) { rq_dz("infix:</>"); return FAILDESCR; }
    return rq_make(bi_mul(an, bd), bi_mul(ad, bn), fat);
}
DESCR_t rk_rat_pow(DESCR_t a, DESCR_t b) {
    DESCR_t an, ad, bn, bd;
    int fat = 0;
    if (!rq_get(a, &an, &ad, &fat) || !rq_get(b, &bn, &bd, &fat) || !bi_one(bd)) return FAILDESCR;
    int aint = !rq_kind(a);
    if (bi_zero(an) && bi_sgn(bn) < 0) { rq_dz("infix:<**>"); return FAILDESCR; }
    if (bn.v != DT_I) {
        if (aint && bi_sgn(bn) > 0) { if (bi_zero(an) || bi_one(an)) return an; if (bi_cmp(an, INTVAL(-1)) == 0) return INTVAL(rt_big_sign(rt_big_mod(bn, INTVAL(2))) != 0 ? -1 : 1); }
        return REALVAL(pow(rq_dbl(an, ad), bi_dbl(bn)));
    }
    if (aint && bn.i >= 0) return bi_pow(an, bn.i);
    return rq_pow(an, ad, bn, fat);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_rat_lit(const char *txt) {
    const char *sl = strchr(txt, '/');
    if (sl) {
        char nb[strlen(txt) + 1], db[strlen(txt) + 1];
        int nn = 0, dn = 0;
        for (const char *p = txt; p < sl; p++) if ((*p >= '0' && *p <= '9') || *p == '-') nb[nn++] = *p;
        for (const char *p = sl + 1; *p; p++) if (*p >= '0' && *p <= '9') db[dn++] = *p;
        nb[nn] = 0;
        db[dn] = 0;
        if (!nn || !dn) return INTVAL(0);
        return rk_rat_new(rt_big_from_str(nb), rt_big_from_str(db), 0);
    }
    char dig[strlen(txt) + 1];
    int nd = 0, frac = 0, seen = 0, neg = 0;
    for (const char *p = txt; *p; p++) { if (*p == '-' && nd == 0 && !seen) neg = 1; else if (*p == '.') seen = 1; else if (*p >= '0' && *p <= '9') { dig[nd++] = *p; if (seen) frac++; } }
    dig[nd] = 0;
    if (!nd) return INTVAL(0);
    DESCR_t n = rt_big_from_str(dig);
    if (neg) n = bi_neg(n);
    return rq_make(n, bi_pow(INTVAL(10), frac), 0);
}
DESCR_t rk_rat_new(DESCR_t n, DESCR_t d, int fat) { if (!bi_is(n) || !bi_is(d)) return FAILDESCR; if (bi_zero(d)) return rq_new(INTVAL(bi_sgn(n)), INTVAL(0), fat); return rq_make(n, d, fat); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rq_pow10_cmp_den(DESCR_t d) { return bi_cmp(d, INTVAL(100000)) < 0; }
static int rq_chars(DESCR_t a) { if (a.v == DT_I) { char b[32]; return snprintf(b, sizeof b, "%lld", (long long) a.i); } return (int) strlen(rt_big_str(a)); }
static char *rq_dup(const char *s) { char *o = rt_wsb_alloc(strlen(s) + 1); strcpy(o, s); return o; }
const char *rk_rat_str(DESCR_t v, int raku) {
    DESCR_t n, d;
    int fat = 0;
    if (!rq_get(v, &n, &d, &fat)) return "";
    int neg = bi_sgn(n) < 0;
    if (bi_zero(d)) {
        if (raku) { const char *ns = rt_big_str(n); char *b = rt_wsb_alloc(strlen(ns) + 8); sprintf(b, "<%s/0>", ns); return b; }
        rt_script_die_surface("Attempt to divide by zero when coercing Rational to Str");
        return "";
    }
    DESCR_t an = bi_abs(n);
    DESCR_t whole = bi_divt(an, d), fn = bi_modt(an, d);
    const char *ws = rt_big_str(whole);
    if (raku && !fat) {
        DESCR_t t = d;
        while (!bi_one(t) && bi_zero(bi_modt(t, INTVAL(5)))) t = bi_divt(t, INTVAL(5));
        while (!bi_one(t) && bi_zero(bi_modt(t, INTVAL(2)))) t = bi_divt(t, INTVAL(2));
        if (bi_one(d)) { char *b = rt_wsb_alloc(strlen(ws) + 8); sprintf(b, "%s%s.0", neg ? "-" : "", ws); return b; }
        if (!bi_one(t)) { const char *ns = rt_big_str(n), *ds = rt_big_str(d); char *b = rt_wsb_alloc(strlen(ns) + strlen(ds) + 8); sprintf(b, "<%s/%s>", ns, ds); return b; }
        int k2 = 0, k5 = 0;
        DESCR_t u = d;
        while (bi_zero(bi_modt(u, INTVAL(2)))) { u = bi_divt(u, INTVAL(2)); k2++; }
        while (bi_zero(bi_modt(u, INTVAL(5)))) { u = bi_divt(u, INTVAL(5)); k5++; }
        int k = k2 > k5 ? k2 : k5;
        DESCR_t sc = bi_floordiv(bi_mul(fn, bi_pow(INTVAL(10), k)), d);
        const char *fs = rt_big_str(sc);
        size_t fl = strlen(fs);
        char *b = rt_wsb_alloc(strlen(ws) + (size_t) k + fl + 8);
        size_t o = 0;
        if (neg) b[o++] = '-';
        o += (size_t) sprintf(b + o, "%s.", ws);
        for (int i = 0; i < k - (int) fl; i++) b[o++] = '0';
        o += (size_t) sprintf(b + o, "%s", fs);
        return b;
    }
    if (raku && fat) { const char *ns = rt_big_str(n), *ds = rt_big_str(d); char *b = rt_wsb_alloc(strlen(ns) + strlen(ds) + 24); sprintf(b, "FatRat.new(%s, %s)", ns, ds); return b; }
    if (bi_zero(fn)) { char *b = rt_wsb_alloc(strlen(ws) + 4); sprintf(b, "%s%s", neg ? "-" : "", ws); return b; }
    int digits;
    if (!fat) digits = rq_pow10_cmp_den(d) ? 6 : rq_chars(d) + 1;
    else digits = rq_pow10_cmp_den(d) ? 6 : rq_chars(d) + rq_chars(whole) + 5;
    DESCR_t p10 = bi_pow(INTVAL(10), digits);
    DESCR_t sc = bi_floordiv(bi_add(bi_mul(bi_mul(fn, p10), INTVAL(2)), d), bi_mul(d, INTVAL(2)));
    if (!fat && fn.v == DT_I && d.v == DT_I && (double) fn.i / (double) d.i == 1.0) sc = p10;
    if (bi_cmp(sc, p10) >= 0) { whole = bi_add(whole, INTVAL(1)); ws = rt_big_str(whole); char *b = rt_wsb_alloc(strlen(ws) + 4); sprintf(b, "%s%s", neg ? "-" : "", ws); return b; }
    const char *fs = rt_big_str(sc);
    int fl = (int) strlen(fs);
    char *b = rt_wsb_alloc(strlen(ws) + (size_t) digits + 8);
    size_t o = 0;
    if (neg) b[o++] = '-';
    o += (size_t) sprintf(b + o, "%s.", ws);
    char *fr = b + o;
    for (int i = 0; i < digits - fl; i++) *fr++ = '0';
    memcpy(fr, fs, (size_t) fl);
    fr += fl;
    while (fr > b + o && fr[-1] == '0') fr--;
    if (fr == b + o) fr--;
    *fr = 0;
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rq_floor(DESCR_t n, DESCR_t d) { return bi_floordiv(n, d); }
int rk_rat_method(const char *m, DESCR_t self, DESCR_t *arg, DESCR_t *out) {
    DESCR_t n, d;
    int fat = 0;
    if (!rq_get(self, &n, &d, &fat) || !rq_kind(self)) return 0;
    if (!strcmp(m, "numerator")) { *out = n; return 1; }
    if (!strcmp(m, "denominator")) { *out = d; return 1; }
    if (bi_zero(d) && (!strcmp(m, "Int") || !strcmp(m, "truncate") || !strcmp(m, "floor") || !strcmp(m, "ceiling") || !strcmp(m, "round") || !strcmp(m, "succ") || !strcmp(m, "pred"))) {
        char msg[64];
        snprintf(msg, sizeof msg, "Attempt to divide by zero when calling .%s on Rational", m);
        rt_script_die_surface(msg);
        *out = FAILDESCR;
        return 1;
    }
    if (!strcmp(m, "Num") || !strcmp(m, "Real") || !strcmp(m, "Numeric") || !strcmp(m, "Bridge")) {
        if (!strcmp(m, "Num") || !strcmp(m, "Bridge")) { *out = REALVAL(rq_dbl(n, d)); return 1; }
        *out = self;
        return 1;
    }
    if (!strcmp(m, "Int") || !strcmp(m, "truncate")) { *out = bi_divt(n, d); return 1; }
    if (!strcmp(m, "floor")) { *out = rq_floor(n, d); return 1; }
    if (!strcmp(m, "ceiling")) { *out = bi_neg(rq_floor(bi_neg(n), d)); return 1; }
    if (!strcmp(m, "round")) {
        if (arg) {
            DESCR_t sn, sd;
            int f2 = 0;
            if (!rq_get(*arg, &sn, &sd, &f2) || bi_zero(sn)) return 0;
            DESCR_t qn = bi_mul(n, sd), qd = bi_mul(d, sn);
            DESCR_t r = bi_floordiv(bi_add(bi_mul(qn, INTVAL(2)), qd), bi_mul(qd, INTVAL(2)));
            if (bi_sgn(qd) < 0) r = bi_floordiv(bi_add(bi_mul(bi_neg(qn), INTVAL(2)), bi_neg(qd)), bi_mul(bi_neg(qd), INTVAL(2)));
            *out = rq_make(bi_mul(r, sn), sd, fat || f2);
            return 1;
        }
        *out = bi_floordiv(bi_add(bi_mul(n, INTVAL(2)), d), bi_mul(d, INTVAL(2)));
        return 1;
    }
    if (!strcmp(m, "abs")) { *out = rq_make(bi_abs(n), d, fat); return 1; }
    if (!strcmp(m, "sign")) { *out = INTVAL(bi_sgn(n)); return 1; }
    if (!strcmp(m, "narrow")) { *out = bi_one(d) ? n : self; return 1; }
    if (!strcmp(m, "Rat")) { if (!fat) { *out = self; return 1; } *out = rq_make(n, d, 0); return 1; }
    if (!strcmp(m, "FatRat")) { *out = rq_new(n, d, 1); return 1; }
    if (!strcmp(m, "Str") || !strcmp(m, "gist")) { *out = STRVAL((char *) rk_rat_str(self, 0)); return 1; }
    if (!strcmp(m, "raku") || !strcmp(m, "perl")) { *out = STRVAL((char *) rk_rat_str(self, 1)); return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so")) { *out = rq_bool(!bi_zero(n)); return 1; }
    if (!strcmp(m, "not")) { *out = rq_bool(bi_zero(n)); return 1; }
    if (!strcmp(m, "succ")) { *out = rq_make(bi_add(n, d), d, fat); return 1; }
    if (!strcmp(m, "pred")) { *out = rq_make(bi_sub(n, d), d, fat); return 1; }
    if (!strcmp(m, "isNaN")) { *out = rq_bool(0); return 1; }
    if (!strcmp(m, "defined")) { *out = rq_bool(1); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_int_method(const char *m, DESCR_t self, DESCR_t *out) {
    if (!bi_is(self)) return 0;
    if (!strcmp(m, "numerator")) { *out = self; return 1; }
    if (!strcmp(m, "denominator")) { *out = INTVAL(1); return 1; }
    if (!strcmp(m, "Rat")) { *out = rq_new(self, INTVAL(1), 0); return 1; }
    if (!strcmp(m, "FatRat")) { *out = rq_new(self, INTVAL(1), 1); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rq_from_dbl_int(double x) { if (fabs(x) < 9.0e18) return INTVAL((int64_t) x); char buf[400]; snprintf(buf, sizeof buf, "%.0f", x); return rt_big_from_str(buf); }
DESCR_t rk_rat_from_num(double x, double eps, int fat) {
    if (x != x) return rq_new(INTVAL(0), INTVAL(0), fat);
    if (isinf(x)) return rq_new(INTVAL(x > 0 ? 1 : -1), INTVAL(0), fat);
    int neg = x < 0;
    double num = neg ? -x : x;
    double r = num - floor(num);
    if (r == 0.0) return rq_make(rq_from_dbl_int(neg ? -num : num), INTVAL(1), fat);
    DESCR_t a = INTVAL(1), b = rq_from_dbl_int(floor(num)), c = INTVAL(0), d = INTVAL(1);
    while (r != 0.0 && fabs(num - rq_dbl(b, d)) > eps) {
        double m = 1.0 / r;
        DESCR_t q = rq_from_dbl_int(floor(m));
        r = m - floor(m);
        DESCR_t ob = b, od = d;
        b = bi_add(bi_mul(q, b), a);
        a = ob;
        d = bi_add(bi_mul(q, d), c);
        c = od;
    }
    return rq_make(neg ? bi_neg(b) : b, d, fat);
}
int rk_rat_real_method(const char *m, DESCR_t self, DESCR_t *arg, DESCR_t *out) {
    if (self.v != DT_R) return 0;
    if (strcmp(m, "Rat") && strcmp(m, "FatRat")) return 0;
    double eps = 1.0e-6;
    if (arg) { double e; if (rk_rat_to_real(*arg, &e)) eps = e; else if (arg->v == DT_R) eps = arg->r; else if (arg->v == DT_I) eps = (double) arg->i; }
    *out = rk_rat_from_num(self.r, eps, m[0] == 'F');
    return 1;
}
int rk_rat_str_method(const char *m, DESCR_t self, DESCR_t *out) {
    if ((self.v != DT_S && self.v != DT_SNUL) || (strcmp(m, "Rat") && strcmp(m, "FatRat"))) return 0;
    const char *t = self.s ? self.s : "";
    while (*t == ' ') t++;
    char *end;
    const char *slash = strchr(t, '/');
    if (slash) {
        char nb[64], db[64];
        size_t ln = (size_t) (slash - t), ld = strlen(slash + 1);
        if (ln >= sizeof nb || ld >= sizeof db) return 0;
        memcpy(nb, t, ln);
        nb[ln] = 0;
        memcpy(db, slash + 1, ld + 1);
        DESCR_t n = rt_big_from_str(nb), d = rt_big_from_str(db);
        if (bi_zero(d)) return 0;
        *out = rq_make(n, d, m[0] == 'F');
        return 1;
    }
    strtod(t, &end);
    if (end == t || strpbrk(t, "eE")) { if (end == t) return 0; *out = rk_rat_from_num(strtod(t, NULL), 1.0e-6, m[0] == 'F'); return 1; }
    char buf[128];
    snprintf(buf, sizeof buf, "%.*s", (int) (end - t) < 127 ? (int) (end - t) : 127, t);
    DESCR_t r = rk_rat_lit(buf);
    if (m[0] == 'F' && rq_kind(r) == 1) r = rq_new(r.u->fields[0], r.u->fields[1], 1);
    *out = r;
    return 1;
}
