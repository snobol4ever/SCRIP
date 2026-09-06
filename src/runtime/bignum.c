#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "descr.h"
extern void *rt_ws_alloc(size_t);
extern char *rt_ws_strdup_c(const char *);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct BIG_t { int32_t sign; uint32_t n; uint32_t limb[1]; } BIG_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_alloc(uint32_t n) {
    BIG_t *b = (BIG_t *) rt_ws_alloc(sizeof(BIG_t) + (size_t)(n ? n - 1 : 0) * sizeof(uint32_t));
    if (!b) return 0;
    b->sign = 0; b->n = n;
    for (uint32_t i = 0; i < n; i++) b->limb[i] = 0;
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void big_trim(BIG_t *b) { while (b->n > 1 && b->limb[b->n - 1] == 0) b->n--; if (b->n == 1 && b->limb[0] == 0) b->sign = 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int big_ucmp(const BIG_t *a, const BIG_t *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (uint32_t i = a->n; i-- > 0;) if (a->limb[i] != b->limb[i]) return a->limb[i] < b->limb[i] ? -1 : 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_uadd(const BIG_t *a, const BIG_t *b) {
    uint32_t n = (a->n > b->n ? a->n : b->n) + 1; BIG_t *r = big_alloc(n); if (!r) return 0;
    uint64_t c = 0;
    for (uint32_t i = 0; i < n; i++) {
        uint64_t s = c; if (i < a->n) s += a->limb[i]; if (i < b->n) s += b->limb[i];
        r->limb[i] = (uint32_t)s; c = s >> 32;
    }
    big_trim(r); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_usub(const BIG_t *a, const BIG_t *b) {
    BIG_t *r = big_alloc(a->n); if (!r) return 0;
    int64_t br = 0;
    for (uint32_t i = 0; i < a->n; i++) {
        int64_t s = (int64_t)a->limb[i] - br - (int64_t)(i < b->n ? b->limb[i] : 0);
        if (s < 0) { s += ((int64_t)1 << 32); br = 1; } else br = 0;
        r->limb[i] = (uint32_t)s;
    }
    big_trim(r); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_umul(const BIG_t *a, const BIG_t *b) {
    BIG_t *r = big_alloc(a->n + b->n); if (!r) return 0;
    for (uint32_t i = 0; i < a->n; i++) {
        uint64_t c = 0;
        for (uint32_t j = 0; j < b->n; j++) {
            uint64_t t = (uint64_t)a->limb[i] * b->limb[j] + r->limb[i + j] + c;
            r->limb[i + j] = (uint32_t)t; c = t >> 32;
        }
        r->limb[i + b->n] = (uint32_t)c;
    }
    big_trim(r); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_from_i64(int64_t v) {
    BIG_t *b = big_alloc(2); if (!b) return 0;
    uint64_t u = (v < 0) ? (uint64_t)(-(v + 1)) + 1u : (uint64_t)v;
    b->limb[0] = (uint32_t)u; b->limb[1] = (uint32_t)(u >> 32);
    b->sign = (v == 0) ? 0 : (v < 0 ? -1 : 1);
    big_trim(b); return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int big_fits_i64(const BIG_t *b, int64_t *out) {
    if (b->n > 2) return 0;
    uint64_t u = b->limb[0]; if (b->n == 2) u |= ((uint64_t)b->limb[1]) << 32;
    if (b->sign >= 0) { if (u > (uint64_t)INT64_MAX) return 0; *out = (int64_t)u; return 1; }
    if (u > (uint64_t)INT64_MAX + 1u) return 0;
    *out = (u == (uint64_t)INT64_MAX + 1u) ? INT64_MIN : -(int64_t)u;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_norm(void *vb) {
    BIG_t *b = (BIG_t *) vb; int64_t f;
    if (!b) return FAILDESCR;
    big_trim(b);
    if (big_fits_i64(b, &f)) { DESCR_t d; d.v = DT_I; d.mod_op = 0; d.src_node = 0; d.slen = 0; d.i = f; return d; }
    DESCR_t d; d.v = DT_BIG; d.mod_op = 0; d.src_node = 0; d.slen = b->n; d.p = (void *) b; return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_big_is(DESCR_t d) { return d.v == DT_BIG && d.p != 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_of(DESCR_t d) { return (d.v == DT_BIG) ? (BIG_t *) d.p : big_from_i64(d.i); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static BIG_t *big_addsub(const BIG_t *a, const BIG_t *b, int negate_b) {
    int bs = negate_b ? -b->sign : b->sign;
    if (a->sign == 0) { BIG_t *r = big_uadd(b, b); if (!r) return 0; BIG_t *z = big_alloc(b->n); if (!z) return 0; memcpy(z->limb, b->limb, (size_t)b->n * 4); z->n = b->n; z->sign = bs; big_trim(z); return z; }
    if (bs == 0) { BIG_t *z = big_alloc(a->n); if (!z) return 0; memcpy(z->limb, a->limb, (size_t)a->n * 4); z->n = a->n; z->sign = a->sign; big_trim(z); return z; }
    if (a->sign == bs) { BIG_t *r = big_uadd(a, b); if (!r) return 0; r->sign = a->sign; big_trim(r); return r; }
    int c = big_ucmp(a, b);
    if (c == 0) return big_alloc(1);
    BIG_t *r = (c > 0) ? big_usub(a, b) : big_usub(b, a);
    if (!r) return 0;
    r->sign = (c > 0) ? a->sign : bs;
    big_trim(r); return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_add(DESCR_t x, DESCR_t y) { BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return FAILDESCR; return rt_big_norm(big_addsub(a, b, 0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_sub(DESCR_t x, DESCR_t y) { BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return FAILDESCR; return rt_big_norm(big_addsub(a, b, 1)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_mul(DESCR_t x, DESCR_t y) {
    BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return FAILDESCR;
    if (a->sign == 0 || b->sign == 0) return rt_big_norm(big_alloc(1));
    BIG_t *r = big_umul(a, b); if (!r) return FAILDESCR;
    r->sign = (a->sign == b->sign) ? 1 : -1; big_trim(r); return rt_big_norm(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_big_cmp(DESCR_t x, DESCR_t y) {
    BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return 0;
    if (a->sign != b->sign) return a->sign < b->sign ? -1 : 1;
    int c = big_ucmp(a, b);
    return (a->sign < 0) ? -c : c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned long long rt_big_hash(DESCR_t x) {
    BIG_t *b = big_of(x); if (!b) return 0;
    unsigned long long h = 0x9E3779B97F4A7C15ull ^ ((unsigned long long)(uint32_t)b->sign * 0xC2B2AE3D27D4EB4Full);
    for (uint32_t i = 0; i < b->n; i++) { h ^= (unsigned long long)b->limb[i] * 0xFF51AFD7ED558CCDull; h = (h << 13) | (h >> 51); }
    return (h ^ (h >> 31)) * 0xC4CEB9FE1A85EC53ull;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_pow(DESCR_t x, int64_t e) {
    if (e < 0) return FAILDESCR;
    BIG_t *base = big_of(x); if (!base) return FAILDESCR;
    BIG_t *acc = big_from_i64(1); if (!acc) return FAILDESCR;
    while (e > 0) {
        if (e & 1) { BIG_t *t = big_umul(acc, base); if (!t) return FAILDESCR; t->sign = (acc->sign == 0 || base->sign == 0) ? 0 : (acc->sign == base->sign ? 1 : -1); big_trim(t); acc = t; }
        e >>= 1;
        if (e) { BIG_t *s = big_umul(base, base); if (!s) return FAILDESCR; s->sign = (base->sign == 0) ? 0 : 1; big_trim(s); base = s; }
    }
    return rt_big_norm(acc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int big_ucmp_nz(const BIG_t *a, const BIG_t *b) {
    uint32_t an = a->n, bn = b->n;
    while (an > 1 && a->limb[an - 1] == 0) an--;
    while (bn > 1 && b->limb[bn - 1] == 0) bn--;
    if (an != bn) return an < bn ? -1 : 1;
    for (uint32_t i = an; i-- > 0;) if (a->limb[i] != b->limb[i]) return a->limb[i] < b->limb[i] ? -1 : 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void big_usub_into(BIG_t *a, const BIG_t *b) {
    int64_t br = 0;
    for (uint32_t i = 0; i < a->n; i++) {
        int64_t t = (int64_t)a->limb[i] - br - (int64_t)(i < b->n ? b->limb[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->limb[i] = (uint32_t)t;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void big_shl1_fixed(BIG_t *b) {
    uint32_t carry = 0;
    for (uint32_t i = 0; i < b->n; i++) { uint32_t nc = b->limb[i] >> 31; b->limb[i] = (b->limb[i] << 1) | carry; carry = nc; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int big_bit(const BIG_t *b, uint32_t k) { return (k / 32 < b->n) ? (int)((b->limb[k / 32] >> (k % 32)) & 1u) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void big_setbit(BIG_t *b, uint32_t k) { if (k / 32 < b->n) b->limb[k / 32] |= (1u << (k % 32)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int big_udivmod(const BIG_t *u, const BIG_t *v, BIG_t **q_out, BIG_t **r_out) {
    if (v->sign == 0) return 0;
    uint32_t un = u->n ? u->n : 1;
    BIG_t *q = big_alloc(un);     if (!q) return 0;
    BIG_t *r = big_alloc(un + 1); if (!r) return 0;
    for (uint32_t bit = un * 32; bit-- > 0;) {
        big_shl1_fixed(r);
        if (big_bit(u, bit)) r->limb[0] |= 1u;
        if (big_ucmp_nz(r, v) >= 0) { big_usub_into(r, v); big_setbit(q, bit); }
    }
    big_trim(q); big_trim(r);
    q->sign = (q->n == 1 && q->limb[0] == 0) ? 0 : 1;
    r->sign = (r->n == 1 && r->limb[0] == 0) ? 0 : 1;
    *q_out = q; *r_out = r; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_div(DESCR_t x, DESCR_t y) {
    BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return FAILDESCR;
    if (b->sign == 0) return FAILDESCR;
    BIG_t *q = 0, *r = 0; if (!big_udivmod(a, b, &q, &r)) return FAILDESCR;
    if (q->sign) q->sign = (a->sign == b->sign) ? 1 : -1;
    return rt_big_norm(q);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_mod(DESCR_t x, DESCR_t y) {
    BIG_t *a = big_of(x), *b = big_of(y); if (!a || !b) return FAILDESCR;
    if (b->sign == 0) return FAILDESCR;
    BIG_t *q = 0, *r = 0; if (!big_udivmod(a, b, &q, &r)) return FAILDESCR;
    if (r->sign) r->sign = a->sign;
    return rt_big_norm(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_from_str(const char *s) {
    if (!s) return FAILDESCR;
    while (*s == ' ' || *s == '\t') s++;
    int neg = 0; if (*s == '+' || *s == '-') { neg = (*s == '-'); s++; }
    if (*s < '0' || *s > '9') return FAILDESCR;
    BIG_t *acc = big_from_i64(0); if (!acc) return FAILDESCR;
    BIG_t *ten = big_from_i64(10); if (!ten) return FAILDESCR;
    for (; *s >= '0' && *s <= '9'; s++) {
        BIG_t *m = big_umul(acc, ten); if (!m) return FAILDESCR;
        BIG_t *d = big_from_i64((int64_t)(*s - '0')); if (!d) return FAILDESCR;
        BIG_t *t = big_uadd(m, d); if (!t) return FAILDESCR;
        t->sign = 1; big_trim(t); acc = t;
    }
    while (*s == ' ' || *s == '\t') s++;
    if (*s) return FAILDESCR;
    if (neg && acc->sign) acc->sign = -1;
    return rt_big_norm(acc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_big_neg(DESCR_t d) {
    BIG_t *b = big_of(d); if (!b) return FAILDESCR;
    BIG_t *r = big_alloc(b->n); if (!r) return FAILDESCR;
    for (uint32_t i = 0; i < b->n; i++) r->limb[i] = b->limb[i];
    r->n = b->n; r->sign = -b->sign; big_trim(r);
    return rt_big_norm(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_big_str(DESCR_t d);
char *rt_big_image_str(DESCR_t d) {
    BIG_t *b = big_of(d); if (!b) return rt_ws_strdup_c("");
    if (b->sign == 0) return rt_ws_strdup_c("0");
    double dlen = (double)(b->n - 1) * 32.0 * 0.3010299956639812
                + log((double)b->limb[b->n - 1]) * 0.4342944819032518 + 0.5;
    if (dlen >= 30.0) {
        char buf[32]; snprintf(buf, sizeof buf, "integer(~10^%ld)", (long)dlen);
        return rt_ws_strdup_c(buf);
    }
    return rt_big_str(d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rt_big_str(DESCR_t d) {
    BIG_t *b = big_of(d); if (!b) return rt_ws_strdup_c("");
    if (b->sign == 0) return rt_ws_strdup_c("0");
    uint32_t n = b->n; uint32_t *t = (uint32_t *) rt_ws_alloc((size_t)n * 4); if (!t) return rt_ws_strdup_c("");
    memcpy(t, b->limb, (size_t)n * 4);
    size_t cap = (size_t)n * 10 + 4; char *buf = (char *) rt_ws_alloc(cap); if (!buf) return rt_ws_strdup_c("");
    size_t p = cap; buf[--p] = 0;
    while (n > 1 || t[0] != 0) {
        uint64_t rem = 0;
        for (uint32_t i = n; i-- > 0;) { uint64_t cur = (rem << 32) | t[i]; t[i] = (uint32_t)(cur / 1000000000u); rem = cur % 1000000000u; }
        while (n > 1 && t[n - 1] == 0) n--;
        int nine = (n > 1 || t[0] != 0);
        for (int k = 0; k < 9; k++) { if (!nine && rem == 0 && k > 0) break; buf[--p] = (char)('0' + (int)(rem % 10u)); rem /= 10u; }
    }
    if (b->sign < 0) buf[--p] = '-';
    return rt_ws_strdup_c(buf + p);
}
