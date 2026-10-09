#include "core.h"
#include "rt/rt.h"
#include "rt/gc_heap.h"
#include "builtins/gen.h"
#include "rt/rt_arena.h"
#include "dtp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef __int128 i128;
#define RK_RAT_NAME "\x01Rat"
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_is(DESCR_t d) { return IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && !strcmp(d.u->type->name, RK_RAT_NAME); }
static i128 rk_gcd(i128 a, i128 b) { if (a < 0) a = -a; if (b < 0) b = -b; while (b) { i128 t = a % b; a = b; b = t; } return a; }
static int rk_fits(i128 v) { return v >= (i128) INT64_MIN + 1 && v <= (i128) INT64_MAX; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rat_cons(long long n, long long d) {
    static int reg = 0;
    if (!reg) { DEFDAT_fn(RK_RAT_NAME "(numerator,denominator)"); reg = 1; }
    return DATCON_fn(RK_RAT_NAME, INTVAL(n), INTVAL(d));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_rat_make(i128 n, i128 d) {
    if (d == 0) return REALVAL(n == 0 ? NAN : (n > 0 ? INFINITY : -INFINITY));
    if (d < 0) { n = -n; d = -d; }
    i128 g = rk_gcd(n, d);
    if (g > 1) { n /= g; d /= g; }
    if (!rk_fits(n) || !rk_fits(d)) return REALVAL((double) n / (double) d);
    return rk_rat_cons((long long) n, (long long) d);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_exact(DESCR_t v, i128 *n, i128 *d) {
    if (IS_INT_fn(v)) { *n = v.i; *d = 1; return 1; }
    if (v.v == DT_BOOL) { *n = v.i != 0; *d = 1; return 1; }
    if (rk_rat_is(v)) { *n = FIELD_GET_fn(v, "numerator").i; *d = FIELD_GET_fn(v, "denominator").i; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_to_real(DESCR_t v, double *out) { i128 n, d; if (!rk_rat_is(v) || !rk_exact(v, &n, &d)) return 0; *out = (double) n / (double) d; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_to_int(DESCR_t v, long long *out) { i128 n, d; if (!rk_rat_is(v) || !rk_exact(v, &n, &d)) return 0; *out = (long long) (n / d); return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static i128 rk_floor_div(i128 n, i128 d) { i128 q = n / d; if ((n % d != 0) && ((n < 0) != (d < 0))) q--; return q; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_rat_pow(i128 n, i128 d, long long e) {
    int neg = e < 0;
    if (neg) e = -e;
    i128 rn = 1, rd = 1;
    for (long long i = 0; i < e; i++) { rn *= n; rd *= d; if (!rk_fits(rn) || !rk_fits(rd)) { double r = pow((double) n / (double) d, neg ? -(double) e : (double) e); return REALVAL(r); } }
    return neg ? rk_rat_make(rd, rn) : rk_rat_make(rn, rd);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_binop(DESCR_t a, DESCR_t b, int op, DESCR_t *out) {
    if (!rk_rat_is(a) && !rk_rat_is(b)) return 0;
    i128 an, ad, bn, bd;
    int ae = rk_exact(a, &an, &ad), be = rk_exact(b, &bn, &bd);
    if (ae && be) {
        switch (op) {
            case BINOP_ADD:
            *out = rk_rat_make(an * bd + bn * ad, ad * bd);
            return 1;
            case BINOP_SUB:
            *out = rk_rat_make(an * bd - bn * ad, ad * bd);
            return 1;
            case BINOP_MUL:
            *out = rk_rat_make(an * bn, ad * bd);
            return 1;
            case BINOP_DIV:
            if (bn == 0) { *out = FAILDESCR; return 1; }
            *out = rk_rat_make(an * bd, ad * bn);
            return 1;
            case BINOP_MOD:
            { if (bn == 0) { *out = FAILDESCR; return 1; } i128 qn = an * bd, qd = ad * bn; i128 fl = rk_floor_div(qn, qd); *out = rk_rat_make(an * bd - fl * bn * ad, ad * bd); return 1; }
            case BINOP_POW:
            if (bd == 1) { *out = rk_rat_pow(an, ad, (long long) bn); return 1; }
            break;
            case BINOP_LT:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd < bn * ad };
            return 1;
            case BINOP_LE:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd <= bn * ad };
            return 1;
            case BINOP_GT:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd > bn * ad };
            return 1;
            case BINOP_GE:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd >= bn * ad };
            return 1;
            case BINOP_EQ:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd == bn * ad };
            return 1;
            case BINOP_NE:
            *out = (DESCR_t){ .v = DT_BOOL, .i = an * bd != bn * ad };
            return 1;
            default:
            break;
        }
    }
    double x, y;
    if (ae) x = (double) an / (double) ad;
    else if (IS_REAL_fn(a)) x = a.r;
    else return 0;
    if (be) y = (double) bn / (double) bd;
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
        if (y == 0.0) { *out = FAILDESCR; return 1; }
        *out = REALVAL(x / y);
        return 1;
        case BINOP_POW:
        *out = REALVAL(pow(x, y));
        return 1;
        case BINOP_LT:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x < y };
        return 1;
        case BINOP_LE:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x <= y };
        return 1;
        case BINOP_GT:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x > y };
        return 1;
        case BINOP_GE:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x >= y };
        return 1;
        case BINOP_EQ:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x == y };
        return 1;
        case BINOP_NE:
        *out = (DESCR_t){ .v = DT_BOOL, .i = x != y };
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_rat_div(DESCR_t a, DESCR_t b) { i128 an, ad, bn, bd; if (!rk_exact(a, &an, &ad) || !rk_exact(b, &bn, &bd) || bn == 0) return FAILDESCR; return rk_rat_make(an * bd, ad * bn); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_terminating(i128 d) { while (d % 2 == 0) d /= 2; while (d % 5 == 0) d /= 5; return d == 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_rat_str(DESCR_t v, int raku) {
    i128 n, d;
    if (!rk_exact(v, &n, &d)) return "";
    char buf[512];
    int k = 0, neg = n < 0;
    if (neg) n = -n;
    if (raku && d != 1 && !rk_terminating(d)) {
        snprintf(buf, sizeof buf, "<%s%lld/%lld>", neg ? "-" : "", (long long) n, (long long) d);
        char *o = rt_wsb_alloc(strlen(buf) + 1);
        strcpy(o, buf);
        return o;
    }
    i128 whole = n / d, rem = n % d;
    if (neg) buf[k++] = '-';
    if (!rem) {
        if (raku) { k += snprintf(buf + k, sizeof buf - (size_t) k, "%lld.0", (long long) whole); } else k += snprintf(buf + k, sizeof buf - (size_t) k, "%lld", (long long) whole);
        char *o = rt_wsb_alloc((size_t) k + 1);
        memcpy(o, buf, (size_t) k + 1);
        return o;
    }
    if (rk_terminating(d)) {
        k += snprintf(buf + k, sizeof buf - (size_t) k, "%lld.", (long long) whole);
        for (int i = 0; i < 100 && rem; i++) { rem *= 10; buf[k++] = (char) ('0' + (int) (rem / d)); rem %= d; }
    } else {
        i128 scaled = (rem * 1000000 * 2 + d) / (2 * d);
        if (scaled >= 1000000) { whole++; scaled -= 1000000; }
        k += snprintf(buf + k, sizeof buf - (size_t) k, "%lld.%06lld", (long long) whole, (long long) scaled);
        while (buf[k - 1] == '0') k--;
        if (buf[k - 1] == '.') k--;
    }
    buf[k] = 0;
    char *o = rt_wsb_alloc((size_t) k + 1);
    memcpy(o, buf, (size_t) k + 1);
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_rat_method(const char *m, DESCR_t self, DESCR_t *out) {
    i128 n, d;
    if (!rk_exact(self, &n, &d)) return 0;
    if (!strcmp(m, "numerator")) { *out = INTVAL((long long) n); return 1; }
    if (!strcmp(m, "denominator")) { *out = INTVAL((long long) d); return 1; }
    if (!strcmp(m, "nude")) { DESCR_t a[2] = { INTVAL((long long) n), INTVAL((long long) d) }; extern DESCR_t rt_make_list(DESCR_t *args, int nargs); *out = rt_make_list(a, 2); return 1; }
    if (!strcmp(m, "Num") || !strcmp(m, "Real")) { *out = REALVAL((double) n / (double) d); return 1; }
    if (!strcmp(m, "Int") || !strcmp(m, "truncate")) { *out = INTVAL((long long) (n / d)); return 1; }
    if (!strcmp(m, "floor")) { *out = INTVAL((long long) rk_floor_div(n, d)); return 1; }
    if (!strcmp(m, "ceiling")) { *out = INTVAL((long long) -rk_floor_div(-n, d)); return 1; }
    if (!strcmp(m, "round")) { *out = INTVAL((long long) rk_floor_div(2 * n + d, 2 * d)); return 1; }
    if (!strcmp(m, "abs")) { *out = rk_rat_make(n < 0 ? -n : n, d); return 1; }
    if (!strcmp(m, "sign")) { *out = INTVAL(n > 0 ? 1 : n < 0 ? -1 : 0); return 1; }
    if (!strcmp(m, "Rat") || !strcmp(m, "narrow")) { *out = self; return 1; }
    if (!strcmp(m, "Str") || !strcmp(m, "gist")) { *out = STRVAL((char *) rk_rat_str(self, 0)); return 1; }
    if (!strcmp(m, "raku") || !strcmp(m, "perl")) { *out = STRVAL((char *) rk_rat_str(self, 1)); return 1; }
    if (!strcmp(m, "Bool") || !strcmp(m, "so")) { *out = (DESCR_t){ .v = DT_BOOL, .i = n != 0 }; return 1; }
    if (!strcmp(m, "not")) { *out = (DESCR_t){ .v = DT_BOOL, .i = n == 0 }; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_rat_from_real(double x) {
    if (x != x || isinf(x)) return REALVAL(x);
    int e;
    double fr = frexp(x, &e);
    i128 n = (i128) (fr * 9007199254740992.0), d = 1;
    e -= 53;
    if (e > 0) { while (e-- > 0 && n < ((i128) 1 << 100)) n *= 2; } else { while (e++ < 0 && d < ((i128) 1 << 100)) d *= 2; }
    return rk_rat_make(n, d);
}
