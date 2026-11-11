#define _GNU_SOURCE
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ir/descr.h"
#include "runtime/builtins/gen.h"
extern void *rt_wsb_alloc(size_t);
typedef union { long double ld; struct { uint64_t m; uint16_t se; } f; } xr_t;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t xr_pack(long double x) { xr_t u = { 0 }; DESCR_t d = { 0 }; u.ld = x; d.v = DT_XR; d.slen = u.f.se; d.i = (int64_t)u.f.m; return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_getp(const DESCR_t *d) {
    xr_t u;
    if (d->v == DT_XR) { u.f.m = (uint64_t)d->i; u.f.se = (uint16_t)d->slen; return u.ld; }
    if (d->v == DT_R) return (long double)d->r;
    if (d->v == DT_I) return (long double)d->i;
    return 0.0L;
}
static long double xr_get(DESCR_t d) {
    xr_t u;
    if (d.v == DT_XR) { u.ld = 0.0L; u.f.m = (uint64_t)d.i; u.f.se = (uint16_t)d.slen; return u.ld; }
    if (d.v == DT_R) return (long double)d.r;
    if (d.v == DT_I) return (long double)d.i;
    return 0.0L;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_sin(long double x) { long double r; __asm__ volatile("fsin" : "=t"(r) : "0"(x)); return r; }
static long double xr_cos(long double x) { long double r; __asm__ volatile("fcos" : "=t"(r) : "0"(x)); return r; }
static long double xr_sqrt(long double x) { long double r; __asm__ volatile("fsqrt" : "=t"(r) : "0"(x)); return r; }
static long double xr_atan(long double x) { long double r; __asm__ volatile("fld1\n\tfpatan" : "=t"(r) : "0"(x) : "st(1)"); return r; }
static long double xr_atan2(long double y, long double x) { long double r; __asm__ volatile("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)"); return r; }
static long double xr_ln(long double x) { long double r; __asm__ volatile("fldln2\n\tfxch\n\tfyl2x" : "=t"(r) : "0"(x) : "st(1)"); return r; }
static long double xr_log2(long double x) { long double r; __asm__ volatile("fld1\n\tfxch\n\tfyl2x" : "=t"(r) : "0"(x) : "st(1)"); return r; }
static long double xr_tan(long double x) { long double r; __asm__ volatile("fptan\n\tfstp %%st(0)" : "=t"(r) : "0"(x)); return r; }
static long double xr_int(long double x) { return truncl(x); }
static long double xr_frac(long double x) { return x - truncl(x); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const double ln2hi = 6.9314718036912382E-001, ln2lo = 1.9082149292705877E-010;
static const float large = 24576.0f, two = 2.0f, half = 0.5f;
static long double xr_exp(long double d) {
    long double r;
    __asm__ volatile( "fldl2e\n\t" "fmul %%st(1), %%st\n\t" "frndint\n\t" "fldl %[hi]\n\t" "fmul %%st(1), %%st\n\t" "fsubrp %%st, %%st(2)\n\t" "fldl %[lo]\n\t" "fmul %%st(1), %%st\n\t"
        "fsubrp %%st, %%st(2)\n\t" "fxch %%st(1)\n\t" "fldl2e\n\t" "fmulp %%st, %%st(1)\n\t" "fld %%st\n\t" "fabs\n\t" "fld1\n\t" "fcomip %%st(1), %%st(0)\n\t" "fstp %%st\n\t" "jp 3f\n\t" "jae 1f\n\t"
        "fld %%st(1)\n\t" "fabs\n\t" "flds %[large]\n\t" "fcomip %%st(1), %%st(0)\n\t" "fstp %%st\n\t" "jb 3f\n\t" "fmuls %[half]\n\t" "f2xm1\n\t" "fld %%st\n\t" "fadds %[two]\n\t"
        "fmulp %%st, %%st(1)\n\t" "jmp 2f\n" "3:\n\t" "fstp %%st\n\t" "fldz\n" "1:\n\t" "f2xm1\n" "2:\n\t" "fld1\n\t" "faddp %%st, %%st(1)\n\t" "fscale\n\t" "fstp %%st(1)" : "=t"(r) : "0"(d),
        [hi] "m"(ln2hi), [lo] "m"(ln2lo), [large] "m"(large), [two] "m"(two), [half] "m"(half) : "st(1)", "st(2)", "st(3)", "cc");
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_intpower(long double base, long long exponent) {
    long double r = 1.0L;
    long long i;
    if (base == 0.0L && exponent == 0) return 1.0L;
    if (exponent < 0) base = 1.0L / base;
    i = exponent < 0 ? -exponent : exponent;
    while (i > 0) { while ((i & 1) == 0) { i >>= 1; base = base * base; } i--; r = r * base; }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_power(long double base, long double e) {
    if (e == 0.0L) return 1.0L;
    if (base == 0.0L && e > 0.0L) return 0.0L;
    if (fabsl(e) <= 2147483647.0L && xr_frac(e) == 0.0L) return xr_intpower(base, (long long)truncl(e));
    return xr_exp(e * xr_ln(base));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_hypot(long double x, long double y) {
    x = fabsl(x);
    y = fabsl(y);
    if (x > y) { long double t = y / x; return x * xr_sqrt(1.0L + t * t); }
    if (x > 0.0L) { long double t = x / y; return y * xr_sqrt(1.0L + t * t); }
    return y;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_un(long long op, long double x) {
    long double t;
    switch (op) {
        case 0:
        return -x;
        case 1:
        return fabsl(x);
        case 2:
        return x * x;
        case 3:
        return xr_sqrt(x);
        case 4:
        return xr_sin(x);
        case 5:
        return xr_cos(x);
        case 6:
        return xr_tan(x);
        case 7:
        return xr_atan(x);
        case 8:
        return xr_ln(x);
        case 9:
        return xr_exp(x);
        case 10:
        return xr_frac(x);
        case 11:
        return xr_int(x);
        case 12:
        return xr_atan2(x, xr_sqrt((1.0L - x) * (1.0L + x)));
        case 13:
        return fabsl(x) == 1.0L ? (x < 0.0L ? M_PIl : 0.0L) : xr_atan2(xr_sqrt((1.0L - x) * (1.0L + x)), x);
        case 14:
        t = xr_exp(x);
        return copysignl(0.5L * (t - 1.0L / t), x);
        case 15:
        t = xr_exp(x);
        return 0.5L * (t + 1.0L / t);
        case 16:
        if (x > 5678.22249441322L) return 1.0L;
        if (x < -5678.22249441322L) return -1.0L;
        t = xr_exp(-2 * x);
        return (1 - t) / (1 + t);
        case 17:
        return xr_ln(x) * 0.43429448190325182765L;
        case 18:
        return xr_log2(x);
        case 19:
        return M_PIl;
        case 20:
        return x * (M_PIl / 180.0L);
        case 21:
        return x * (180.0L / M_PIl);
        default:
        return x;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void xr_digits(long double x, int sig, char *buf, int *dexp) {
    int tn = snprintf(NULL, 0, "%.*Le", sig - 1, x);
    char t[tn + 1];
    snprintf(t, (size_t)tn + 1, "%.*Le", sig - 1, x);
    char *e = strchr(t, 'e');
    int k = 0;
    for (char *q = t; q < e; q++) if (*q >= '0' && *q <= '9') buf[k++] = *q;
    buf[k] = '\0';
    *dexp = atoi(e + 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t xr_wrap(const char *t, int n, long long w) {
    size_t pad = w > n ? (size_t)(w - n) : 0;
    char *r = (char *)rt_wsb_alloc(pad + (size_t)n + 1);
    memset(r, ' ', pad);
    memcpy(r + pad, t, (size_t)n + 1);
    return BSTRVAL(r, pad + (size_t)n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t xr_str(long double x, long long w, long long p) {
    int n, sig = p < 0 && w >= 0 ? (w - 9 < 1 ? 1 : w - 9 > 20 ? 20 : (int)(w - 9)) + 1 : 21, de = 0;
    size_t pad;
    char *r;
    if (!isfinite(x)) { const char *t = isnan(x) ? "Nan" : x < 0 ? "-Inf" : "+Inf"; return xr_wrap(t, (int)strlen(t), w < 0 ? 29 : w); }
    char dg[sig + 1];
    long double ax = fabsl(x);
    if (p >= 0) {
        if (ax != 0.0L) xr_digits(ax, 21, dg, &de);
        if (ax != 0.0L && de + 1 + p >= 21) {
            long long il = de >= 0 ? de + 1 : 1, fl = p;
            long long tot = (signbit(x) ? 1 : 0) + il + (fl > 0 ? 1 + fl : 0);
            char *o = (char *)rt_wsb_alloc((size_t)tot + 1), *q = o;
            if (signbit(x)) *q++ = '-';
            if (de < 0) *q++ = '0';
            else for (int i = 0; i <= de; i++) *q++ = i < 21 ? dg[i] : '0';
            if (fl > 0) { *q++ = '.'; for (long long i = 0; i < fl; i++) { long long di = de >= 0 ? de + 1 + i : i - (-de - 1); *q++ = di >= 0 && di < 21 ? dg[di] : '0'; } }
            *q = '\0';
            return xr_wrap(o, (int)tot, w);
        }
        n = snprintf(NULL, 0, "%.*Lf", (int)p, x);
        pad = w > n ? (size_t)(w - n) : 0;
        r = (char *)rt_wsb_alloc(pad + (size_t)n + 1);
        memset(r, ' ', pad);
        snprintf(r + pad, (size_t)n + 1, "%.*Lf", (int)p, x);
        return BSTRVAL(r, pad + (size_t)n);
    }
    if (ax == 0.0L) { memset(dg, '0', (size_t)sig); dg[sig] = '\0'; } else xr_digits(ax, sig, dg, &de);
    n = snprintf(NULL, 0, "%c%c.%sE%c%04d", signbit(x) ? '-' : ' ', dg[0], dg + 1, de < 0 ? '-' : '+', de < 0 ? -de : de);
    pad = w > n ? (size_t)(w - n) : 0;
    r = (char *)rt_wsb_alloc(pad + (size_t)n + 1);
    memset(r, ' ', pad);
    snprintf(r + pad, (size_t)n + 1, "%c%c.%sE%c%04d", signbit(x) ? '-' : ' ', dg[0], dg + 1, de < 0 ? '-' : '+', de < 0 ? -de : de);
    return BSTRVAL(r, pad + (size_t)n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
extern void pas_file_err(const char *clause, const char *what, const char *proc);
static void xr_chk(long long op, long double x) {
    if (op == 3 && x < 0) pas_file_err("6.6.6.2", "sqrt(x) of a negative x", NULL);
    if (op == 8 && !(x > 0)) pas_file_err("6.6.6.2", "ln(x) of an x that is not greater than zero", NULL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_bin(long long op, long double x, long double y) {
    switch (op) {
        case 0:
        return x + y;
        case 1:
        return x - y;
        case 2:
        return x * y;
        case 3:
        return x / y;
        case 4:
        return xr_atan2(x, y);
        case 5:
        return xr_power(x, y);
        case 6:
        return xr_hypot(x, y);
        default:
        return xr_ln(y) / xr_ln(x);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_lit(DESCR_t m, DESCR_t se) { xr_t u = { 0 }; u.f.m = (uint64_t)m.i; u.f.se = (uint16_t)se.i; return u.ld; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long double xr_ev(DESCR_t *a, int n) {
    const unsigned char *c = (const unsigned char *)a[0].s, *e = c + a[0].slen;
    int depth = 0, li = 1;
    while (c < e && *c != ';') depth = depth * 10 + (*c++ - '0');
    c++;
    long double st[depth + 1], *t = st;
    for (; c < e; c++) {
        switch (*c) {
            case 'L':
            *t++ = li < n ? xr_getp(a + li++) : 0.0L;
            break;
            case 'X':
            *t++ = li + 1 < n ? xr_lit(a[li], a[li + 1]) : 0.0L;
            li += 2;
            break;
            case 'P':
            *t++ = M_PIl;
            break;
            case 'n':
            t[-1] = -t[-1];
            break;
            case 'u':
            c++;
            xr_chk(*c - 'A', t[-1]);
            t[-1] = xr_un(*c - 'A', t[-1]);
            break;
            case 'b':
            c++;
            t[-2] = xr_bin(*c - 'A', t[-2], t[-1]);
            t--;
            break;
            case '+':
            t[-2] += t[-1];
            t--;
            break;
            case '-':
            t[-2] -= t[-1];
            t--;
            break;
            case '*':
            t[-2] *= t[-1];
            t--;
            break;
            case 't':
            case 'r':
            break;
            default:
            t[-2] /= t[-1];
            t--;
            break;
        }
    }
    return t > st ? t[-1] : 0.0L;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_xr_ev_d(DESCR_t *a, int n) {
    long double r = xr_ev(a, n);
    char tl = a[0].slen ? a[0].s[a[0].slen - 1] : 0;
    if (tl == 't') return INTVAL((long long)truncl(r));
    if (tl == 'r') return INTVAL((long long)rintl(r));
    return xr_pack(r);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long double pas_xr_un_ld(long long op, long double x) { return xr_un(op, x); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long double pas_xr_bin_ld(long long op, long double x, long double y) { return xr_bin(op, x, y); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_xr_str_d(DESCR_t d, long long w, long long p) { return xr_str(xr_get(d), w, p); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_xr_dispatch(int fam, DESCR_t *a, int n, DESCR_t *out);
DESCR_t pas_xr_cvt_d(DESCR_t *a, int n) { DESCR_t o = INTVAL(0); pas_xr_dispatch(0, a, n, &o); return o; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_xr_dispatch(int fam, DESCR_t *a, int n, DESCR_t *out) {
    if (fam == 5) { *out = xr_pack(xr_ev(a, n)); return 1; }
    long long op = n > 0 && a[0].v == DT_I ? a[0].i : 0;
    long double x = n > 1 ? xr_get(a[1]) : 0.0L, y = n > 2 ? xr_get(a[2]) : 0.0L;
    if (fam == 0) {
        switch (op) {
            case 0:
            *out = xr_pack((long double)a[1].i);
            return 1;
            case 1:
            *out = xr_pack((long double)(uint64_t)a[1].i);
            return 1;
            case 2:
            *out = xr_pack(xr_get(a[1]));
            return 1;
            case 10:
            *out = xr_pack(xr_lit(a[1], a[2]));
            return 1;
            case 4:
            *out = REALVAL((double)x);
            return 1;
            case 5:
            *out = REALVAL((double)(float)x);
            return 1;
            case 6:
            *out = INTVAL((long long)truncl(x));
            return 1;
            case 7:
            *out = INTVAL((long long)rintl(x));
            return 1;
            case 8:
            *out = INTVAL((long long)floorl(x));
            return 1;
            case 9:
            *out = INTVAL((long long)ceill(x));
            return 1;
            default:
            return 0;
        }
    }
    if (fam == 1) { if (op < 0 || op > 7) return 0; *out = xr_pack(xr_bin(op, x, y)); return 1; }
    if (fam == 2) {
        switch (op) {
            case 0:
            *out = INTVAL(x == y);
            return 1;
            case 1:
            *out = INTVAL(x != y);
            return 1;
            case 2:
            *out = INTVAL(x < y);
            return 1;
            case 3:
            *out = INTVAL(x <= y);
            return 1;
            case 4:
            *out = INTVAL(x > y);
            return 1;
            case 5:
            *out = INTVAL(x >= y);
            return 1;
            default:
            return 0;
        }
    }
    if (fam == 3) { xr_chk(op, x); *out = xr_pack(xr_un(op, x)); return 1; }
    if (fam == 4) { *out = xr_str(x, n > 2 && a[2].v == DT_I ? a[2].i : -1, n > 3 && a[3].v == DT_I ? a[3].i : -1); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_xr_arith(int op, DESCR_t a, DESCR_t b, DESCR_t *out) {
    long double x = xr_get(a), y = xr_get(b);
    switch (op) {
        case 0:
        *out = xr_pack(x + y);
        return 1;
        case 1:
        *out = xr_pack(x - y);
        return 1;
        case 2:
        *out = xr_pack(x * y);
        return 1;
        case 3:
        *out = xr_pack(x / y);
        return 1;
        case 6:
        *out = xr_pack(-x);
        return 1;
        case 8:
        *out = INTVAL(x < y);
        return 1;
        case 9:
        *out = INTVAL(x <= y);
        return 1;
        case 10:
        *out = INTVAL(x > y);
        return 1;
        case 11:
        *out = INTVAL(x >= y);
        return 1;
        case 12:
        *out = INTVAL(x == y);
        return 1;
        case 13:
        *out = INTVAL(x != y);
        return 1;
        default:
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_xr_binop(int code, DESCR_t a, DESCR_t b, DESCR_t *out) {
    int op;
    switch (code) {
        case BINOP_ADD:
        op = 0;
        break;
        case BINOP_SUB:
        op = 1;
        break;
        case BINOP_MUL:
        op = 2;
        break;
        case BINOP_DIV:
        op = 3;
        break;
        case BINOP_LT:
        op = 8;
        break;
        case BINOP_LE:
        op = 9;
        break;
        case BINOP_GT:
        op = 10;
        break;
        case BINOP_GE:
        op = 11;
        break;
        case BINOP_EQ:
        op = 12;
        break;
        case BINOP_NE:
        op = 13;
        break;
        default:
        return 0;
    }
    return pas_xr_arith(op, a, b, out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
double pas_xr_todouble(DESCR_t d) { return (double)xr_get(d); }
void pas_xr_fixed(DESCR_t d, int prec, char *out, size_t cap) {
    long double x = xr_get(d);
    snprintf(out, cap, "%.*Lf", prec > 400 ? 400 : prec, x);
    if (out[0] == '-' && out[strspn(out + 1, "0.") + 1] == '\0') memmove(out, out + 1, strlen(out));
}
