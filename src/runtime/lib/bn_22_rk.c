#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_22_rk.h"
static void rk_sb_put(rk_sb_t *s, const char *p, size_t n) {
    if (s->n + n + 1 > s->cap) { size_t nc = (s->cap ? s->cap * 2 : 128) + n; char *nb = (char *) rt_wsb_alloc(nc); if (s->n) memcpy(nb, s->b, s->n); s->b = nb; s->cap = nc; }
    memcpy(s->b + s->n, p, n);
    s->n += n;
}
static void rk_sb_rep(rk_sb_t *s, char c, long n) { for (long i = 0; i < n; i++) rk_sb_put(s, &c, 1); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t rk_u8len(const char *s) { size_t n = 0; for (; *s; s++) if (((unsigned char) *s & 0xC0) != 0x80) n++; return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rsp_arg_num(DESCR_t a, long long *iv, double *dv, int *isint, const char **big) {
    *big = NULL;
    *isint = 1;
    *iv = 0;
    *dv = 0;
    if (a.v == DT_BIG) { *big = rk_cstr(a); return; }
    if (IS_INT_fn(a) || a.v == DT_BOOL) { *iv = a.i; *dv = (double) a.i; return; }
    if (IS_REAL_fn(a)) { *isint = 0; *dv = a.r; *iv = (long long) a.r; return; }
    extern int rk_rat_is(DESCR_t);
    extern int rk_rat_method(const char *, DESCR_t, DESCR_t *, DESCR_t *);
    extern int rk_rat_to_real(DESCR_t, double *);
    if (rk_rat_is(a)) { DESCR_t t; rk_rat_to_real(a, dv); if (rk_rat_method("truncate", a, NULL, &t)) { if (t.v == DT_BIG) { *big = rk_cstr(t); return; } if (t.v == DT_I) { *iv = t.i; return; } } }
    double d;
    int ii;
    if (rk_range_num(a, &d, &ii)) { *dv = d; *iv = (long long) d; *isint = ii; return; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rsp_pad(rk_sb_t *o, const char *body, size_t blen, size_t chars, long width, int left, int zero, size_t zpos) {
    long pad = width > (long) chars ? width - (long) chars : 0;
    if (left) { rk_sb_put(o, body, blen); rk_sb_rep(o, ' ', pad); return; }
    if (zero) { rk_sb_put(o, body, zpos); rk_sb_rep(o, '0', pad); rk_sb_put(o, body + zpos, blen - zpos); return; }
    rk_sb_rep(o, ' ', pad);
    rk_sb_put(o, body, blen);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rsprintf(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen) {
    rk_sb_t o = { 0, 0, 0 };
    int ai = from;
    for (const char *p = fmt; *p; ) {
        if (*p != '%') { const char *q = strchr(p, '%'); size_t n = q ? (size_t) (q - p) : strlen(p); rk_sb_put(&o, p, n); p += n; continue; }
        const char *start = p++;
        if (*p == '%') { rk_sb_put(&o, "%", 1); p++; continue; }
        int idx = -1;
        { const char *q = p; long n = 0; while (*q >= '0' && *q <= '9') n = n * 10 + (*q++ - '0'); if (q > p && *q == '$') { idx = (int) n; p = q + 1; } }
        int left = 0, plus = 0, space = 0, zero = 0, alt = 0;
        for (;; p++) { if (*p == '-') left = 1; else if (*p == '+') plus = 1; else if (*p == ' ') space = 1; else if (*p == '0') zero = 1; else if (*p == '#') alt = 1; else break; }
        long width = 0;
        if (*p == '*') {
            long long w = ai < nargs ? (IS_INT_fn(args[ai]) ? args[ai].i : (long long) rk_cstr_num(args[ai])) : 0;
            ai++;
            if (w < 0) { left = 1; w = -w; }
            width = (long) w;
            p++;
        } else while (*p >= '0' && *p <= '9') width = width * 10 + (*p++ - '0');
        long prec = -1;
        if (*p == '.') {
            p++;
            prec = 0;
            if (*p == '*') {
                long long w = ai < nargs ? (IS_INT_fn(args[ai]) ? args[ai].i : (long long) rk_cstr_num(args[ai])) : 0;
                ai++;
                prec = w < 0 ? -1 : (long) w;
                p++;
            } else while (*p >= '0' && *p <= '9') prec = prec * 10 + (*p++ - '0');
        }
        while (*p == 'l' || *p == 'h' || *p == 'q' || *p == 'L' || *p == 'V' || *p == 'z' || *p == 't' || *p == 'j') p++;
        char conv = *p;
        if (!conv) { rk_sb_put(&o, start, (size_t) (p - start)); break; }
        p++;
        DESCR_t a = NULVCL;
        if (idx > 0) { if (from - 1 + idx < nargs) a = args[from - 1 + idx]; } else { if (ai < nargs) a = args[ai]; ai++; }
        long long iv;
        double dv;
        int isint;
        const char *big;
        char sb[64];
        if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'b' || conv == 'B' || conv == 'o' || conv == 'x' || conv == 'X') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            int neg = big ? big[0] == '-' : (isint ? iv < 0 : dv < 0);
            char digits[big ? strlen(big) + 2 : 72];
            if (big) strcpy(digits, big[0] == '-' ? big + 1 : big);
            else {
                unsigned long long mag = neg ? (unsigned long long) (-(iv + 1)) + 1ULL : (unsigned long long) iv;
                if (!isint) { double ad = dv < 0 ? -dv : dv; mag = (unsigned long long) ad; }
                int base = (conv == 'd' || conv == 'i' || conv == 'u') ? 10 : (conv == 'b' || conv == 'B') ? 2 : conv == 'o' ? 8 : 16;
                const char *dg = conv == 'X' ? "0123456789ABCDEF" : "0123456789abcdef";
                int nb = 2;
                for (unsigned long long m = mag; m > 1; m >>= 1) nb++;
                char tmp[nb + 2];
                int tn = 0;
                if (mag == 0) tmp[tn++] = '0';
                while (mag) { tmp[tn++] = dg[mag % (unsigned) base]; mag /= (unsigned) base; }
                int dn = 0;
                while (tn) digits[dn++] = tmp[--tn];
                digits[dn] = 0;
            }
            int is0 = digits[0] == '0' && !digits[1];
            int signed_conv = conv == 'd' || conv == 'i';
            if (prec == 0 && is0) digits[0] = 0;
            size_t dl = strlen(digits);
            char dpad[(prec > (long) dl ? (size_t) prec : dl) + 2];
            size_t k = 0;
            for (long z = (long) dl; z < prec; z++) dpad[k++] = '0';
            memcpy(dpad + k, digits, dl + 1);
            const char *pre = "";
            if (alt && !is0) pre = conv == 'b' ? "0b" : conv == 'B' ? "0B" : conv == 'x' ? "0x" : conv == 'X' ? "0X" : conv == 'o' ? (dpad[0] == '0' ? "" : "0") : "";
            const char *sg = neg ? "-" : (signed_conv && plus) ? "+" : (signed_conv && space) ? " " : "";
            char body[strlen(sg) + strlen(pre) + strlen(dpad) + 2];
            snprintf(body, sizeof body, "%s%s%s", sg, pre, dpad);
            rk_rsp_pad(&o, body, strlen(body), strlen(body), width, left, zero && prec < 0, strlen(sg) + strlen(pre));
        } else if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'F' || conv == 'g' || conv == 'G') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            if (big) dv = strtod(big, NULL);
            char cs[32];
            int cl = 0;
            cs[cl++] = '%';
            if (left && !zero) cs[cl++] = '-';
            if (plus) cs[cl++] = '+';
            if (space) cs[cl++] = ' ';
            if (zero) cs[cl++] = '0';
            if (alt) cs[cl++] = '#';
            cl += snprintf(cs + cl, sizeof cs - (size_t) cl, "%ld", width);
            if (prec >= 0) cl += snprintf(cs + cl, sizeof cs - (size_t) cl, ".%ld", prec);
            cs[cl++] = conv;
            cs[cl] = 0;
            size_t need = (size_t) snprintf(NULL, 0, cs, dv) + 1;
            char fb[need];
            snprintf(fb, need, cs, dv);
            rk_sb_put(&o, fb, strlen(fb));
        } else if (conv == 's') {
            const char *sv = a.v == DT_BOOL ? (a.i ? "True" : "False") : rk_cstr(a);
            if (!sv) sv = "";
            size_t chars = rk_u8len(sv), blen = strlen(sv);
            if (prec >= 0 && (size_t) prec < chars) {
                size_t q = 0, cc = 0;
                while (sv[q] && cc < (size_t) prec) { q++; while (((unsigned char) sv[q] & 0xC0) == 0x80) q++; cc++; }
                blen = q;
                chars = (size_t) prec;
            }
            rk_rsp_pad(&o, sv, blen, chars, width, left, zero, 0);
        } else if (conv == 'c') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            char ch[8];
            size_t cn = rk_utf8_put((unsigned) iv, ch);
            rk_rsp_pad(&o, ch, cn, 1, width, left, zero, 0);
        } else rk_sb_put(&o, start, (size_t) (p - start));
        (void) sb;
    }
    if (!o.b) o.b = (char *) rt_wsb_alloc(1);
    o.b[o.n] = 0;
    *outp = o.b;
    if (outlen) *outlen = o.n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static size_t rk_dec_cap(double r) { return (size_t) snprintf(NULL, 0, "%.17e", r) + 2; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_dec_digits(double r, char *digs, int *e) {
    int nd = 0;
    *e = 0;
    digs[0] = '0';
    digs[1] = 0;
    if (r == 0) return;
    for (int prec = 1; prec <= 17; prec++) {
        char b[(size_t) snprintf(NULL, 0, "%.*e", prec - 1, r) + 1];
        snprintf(b, sizeof b, "%.*e", prec - 1, r);
        if (strtod(b, NULL) != r && prec < 17) continue;
        char *ep = strchr(b, 'e');
        *e = ep ? atoi(ep + 1) : 0;
        digs[nd++] = b[0];
        for (char *q = b + 2; q < ep; q++) digs[nd++] = *q;
        while (nd > 1 && digs[nd - 1] == '0') nd--;
        digs[nd] = 0;
        return;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_dec_round(char *num, size_t cap, long prec, const char *digs, int e) {
    size_t nd = strlen(digs);
    long ilen = e >= 0 ? e + 1 : 1;
    char lhs[nd + (size_t) (ilen > 0 ? ilen : 1) + 8], frac[nd + 8 + (size_t) (e < 0 ? -e : 0)];
    size_t li = 0, fi = 0;
    if (e >= 0) {
        for (long i = 0; i < ilen; i++) lhs[li++] = (size_t) i < nd ? digs[i] : '0';
        for (size_t i = (size_t) ilen; i < nd; i++) frac[fi++] = digs[i];
    } else {
        lhs[li++] = '0';
        for (long i = 0; i < -e - 1; i++) frac[fi++] = '0';
        for (size_t i = 0; i < nd; i++) frac[fi++] = digs[i];
    }
    lhs[li] = 0;
    frac[fi] = 0;
    int carry = (long) fi > prec && frac[prec] >= '5';
    char out[li + (size_t) (prec > 0 ? prec : 0) + 4];
    size_t oi = 0;
    for (size_t i = 0; i < li; i++) out[oi++] = lhs[i];
    for (long i = 0; i < prec; i++) out[oi++] = (size_t) i < fi ? frac[i] : '0';
    if (carry) { long k = (long) oi - 1; while (k >= 0 && out[k] == '9') { out[k] = '0'; k--; } if (k >= 0) out[k]++; else { memmove(out + 1, out, oi); out[0] = '1'; oi++; } }
    out[oi] = 0;
    size_t il = oi - (size_t) prec;
    if (prec > 0) snprintf(num, cap, "%.*s.%s", (int) il, out, out + il);
    else snprintf(num, cap, "%s", out);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void rk_rsp_legacy_pad(rk_sb_t *o, const char *made, size_t mlen, long size, int minus, char padc) {
    long chars = (long) rk_u8len(made);
    (void) mlen;
    long npad = (size < 0 ? -size : size) - chars;
    if (minus) { rk_sb_put(o, made, mlen); rk_sb_rep(o, padc, npad); } else { rk_sb_rep(o, padc, npad); rk_sb_put(o, made, mlen); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_rsprintf_legacy(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen) {
    rk_sb_t o = { 0, 0, 0 };
    int ai = from, used = 0, explicit_idx = 0;
    for (const char *p = fmt; *p; ) {
        if (*p != '%') { const char *q = strchr(p, '%'); size_t n = q ? (size_t) (q - p) : strlen(p); rk_sb_put(&o, p, n); p += n; continue; }
        const char *start = p++;
        int idx = -1;
        { const char *q = p; long n = 0; while (*q >= '0' && *q <= '9') n = n * 10 + (*q++ - '0'); if (q > p && *q == '$') { idx = (int) n; p = q + 1; explicit_idx = 1; } }
        int minus = 0, plus = 0, space = 0, zero = 0, hash = 0;
        for (;; p++) { if (*p == '-') minus = 1; else if (*p == '+') plus = 1; else if (*p == ' ') space = 1; else if (*p == '0') zero = 1; else if (*p == '#') hash = 1; else break; }
        long size = 0;
        int has_size = 0;
        if (*p == '*') {
            long long w = ai < nargs ? (IS_INT_fn(args[ai]) ? args[ai].i : (long long) rk_cstr_num(args[ai])) : 0;
            ai++;
            used++;
            size = (long) w;
            has_size = 1;
            p++;
        } else if (*p >= '0' && *p <= '9') {
            has_size = 1;
            while (*p >= '0' && *p <= '9') size = size * 10 + (*p++ - '0');
        }
        long prec = -1;
        if (*p == '.') {
            p++;
            prec = 0;
            if (*p == '*') {
                long long w = ai < nargs ? (IS_INT_fn(args[ai]) ? args[ai].i : (long long) rk_cstr_num(args[ai])) : 0;
                ai++;
                used++;
                prec = (long) w;
                p++;
            } else while (*p >= '0' && *p <= '9') prec = prec * 10 + (*p++ - '0');
        }
        char conv = *p;
        if (!conv) { rk_sb_put(&o, start, (size_t) (p - start)); break; }
        p++;
        if (conv == '%') { rk_sb_put(&o, "%", 1); continue; }
        DESCR_t a = NULVCL;
        if (idx > 0) { if (from - 1 + idx < nargs) a = args[from - 1 + idx]; } else { if (ai < nargs) a = args[ai]; ai++; used++; }
        int isminus = minus || size < 0;
        char padc = (zero && ((prec < 0 && !isminus) || strchr("eEfFgG", conv))) ? '0' : ' ';
        long asz = size < 0 ? -size : size;
        long long iv;
        double dv;
        int isint;
        const char *big;
        char *made = NULL;
        if (conv == 'd' || conv == 'i' || conv == 'u' || conv == 'b' || conv == 'B' || conv == 'o' || conv == 'x' || conv == 'X') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            if (!isint && !big) { iv = dv > 0 ? (long long) floor(dv) : (long long) ceil(dv); }
            int neg = big ? big[0] == '-' : iv < 0;
            char dec[big ? strlen(big) + 2 : 40];
            if (big) strcpy(dec, big[0] == '-' ? big + 1 : big);
            else snprintf(dec, sizeof dec, "%llu", neg ? (unsigned long long) (-(iv + 1)) + 1ULL : (unsigned long long) iv);
            const char *sign = neg ? "-" : plus ? "+" : space ? " " : "";
            if (conv == 'u') {
                snprintf(dec, sizeof dec, "%llu", neg ? 0ULL : (unsigned long long) iv);
                made = (char *) rt_wsb_alloc(strlen(dec) + 1);
                strcpy(made, dec);
            } else if (conv == 'd' || conv == 'i') {
                char d[strlen(dec) + (prec > 0 ? (size_t) prec : 0) + 8];
                strcpy(d, dec);
                if (prec >= 0) {
                    if (prec != 0) {
                        long rep = prec - (long) (strlen(dec) + (neg ? 1 : 0));
                        if (rep > 0) { char t[sizeof d]; memset(t, '0', (size_t) rep); strcpy(t + rep, d); strcpy(d, t); }
                    } else if (!strcmp(dec, "0")) d[0] = 0;
                }
                char *mb = (char *) rt_wsb_alloc(strlen(d) + (size_t) (asz > 0 ? asz : 0) + 8);
                if (padc != ' ' && has_size) {
                    long z = asz - (long) strlen(d) - 1;
                    sprintf(mb, "%s", sign);
                    size_t k = strlen(mb);
                    for (long i = 0; i < z; i++) mb[k++] = padc;
                    strcpy(mb + k, d);
                } else sprintf(mb, "%s%s", sign, d);
                made = mb;
            } else {
                int base = (conv == 'b' || conv == 'B') ? 2 : conv == 'o' ? 8 : 16;
                const char *dg = "0123456789ABCDEF";
                unsigned long long mag = neg ? (unsigned long long) (-(iv + 1)) + 1ULL : (unsigned long long) iv;
                int nb = 2;
                for (unsigned long long m = mag; m > 1; m >>= 1) nb++;
                char tmp[nb + 2], digits[nb + 2];
                int tn = 0, dn = 0;
                if (mag == 0) tmp[tn++] = '0';
                while (mag) { tmp[tn++] = dg[mag % (unsigned) base]; mag /= (unsigned) base; }
                while (tn) digits[dn++] = tmp[--tn];
                digits[dn] = 0;
                int is0 = !strcmp(digits, "0");
                char *mb = (char *) rt_wsb_alloc((size_t) (asz > prec ? asz : prec) + 96);
                if (conv == 'b' || conv == 'B') {
                    const char *pre = (!is0 && hash) ? (conv == 'b' ? "0b" : "0B") : "";
                    if (prec >= 0) {
                        if (prec == 0 && is0) mb[0] = 0;
                        else { sprintf(mb, "%s%s", sign, pre); size_t k = strlen(mb); for (long i = 0; i < prec - dn; i++) mb[k++] = '0'; strcpy(mb + k, digits); }
                    } else if (padc != ' ' && has_size) {
                        sprintf(mb, "%s%s", sign, pre);
                        size_t k = strlen(mb);
                        long z = asz - (long) strlen(sign) - (long) strlen(pre) - dn;
                        for (long i = 0; i < z; i++) mb[k++] = padc;
                        strcpy(mb + k, digits);
                    } else sprintf(mb, "%s%s%s", sign, pre, digits);
                } else {
                    char ib[strlen(digits) + 2];
                    snprintf(ib, sizeof ib, "%s%s", neg ? "-" : "", digits);
                    const char *pre = (!is0 && hash) ? (conv == 'o' ? "0" : "0X") : "";
                    char body[sizeof ib + (size_t) (prec > 0 ? prec : 0) + 8];
                    if (prec >= 0) {
                        if (prec == 0 && is0) ib[0] = 0;
                        if (conv == 'o' && prec > (long) strlen(ib)) pre = "";
                        size_t k = 0;
                        for (const char *q = pre; *q; q++) body[k++] = *q;
                        for (long i = 0; i < prec - (long) strlen(ib); i++) body[k++] = '0';
                        strcpy(body + k, ib);
                    } else snprintf(body, sizeof body, "%s%s", pre, ib);
                    if (conv == 'x') for (char *q = body; *q; q++) *q = (char) tolower((unsigned char) *q);
                    strcpy(mb, body);
                }
                made = mb;
            }
        } else if (conv == 'e' || conv == 'E' || conv == 'f' || conv == 'F' || conv == 'g' || conv == 'G') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            if (big) dv = strtod(big, NULL);
            long pr = prec >= 0 ? prec : 6;
            int neg = dv < 0 || (dv == 0 && signbit(dv));
            const char *sign = neg ? "-" : plus ? "+" : space ? " " : "";
            double ad = fabs(dv);
            char num[400 + (size_t) pr];
            if (isnan(ad) || isinf(ad)) snprintf(num, sizeof num, "%s", isnan(ad) ? "NaN" : "Inf");
            else if (conv == 'f' || conv == 'F') {
                char digs[rk_dec_cap(ad)];
                int e;
                rk_dec_digits(ad, digs, &e);
                rk_dec_round(num, sizeof num, pr, digs, e);
            } else if (conv == 'e' || conv == 'E') {
                char digs[rk_dec_cap(ad)], m[400 + (size_t) pr];
                int e;
                rk_dec_digits(ad, digs, &e);
                rk_dec_round(m, sizeof m, pr, digs, 0);
                snprintf(num, sizeof num, "%s%c%c%02d", m, conv, e < 0 ? '-' : '+', e < 0 ? -e : e);
            } else {
                char cs[(size_t) snprintf(NULL, 0, "%%.%ldg", pr) + 1];
                snprintf(cs, sizeof cs, "%%.%ldg", pr);
                snprintf(num, sizeof num, cs, ad);
                if (conv == 'G') for (char *q = num; *q; q++) *q = (char) toupper((unsigned char) *q);
            }
            char *mb = (char *) rt_wsb_alloc(strlen(num) + (size_t) (asz > 0 ? asz : 0) + 8);
            if (padc != ' ' && has_size) {
                long z = asz - (long) strlen(num) - 1;
                sprintf(mb, "%s", sign);
                size_t k = strlen(mb);
                for (long i = 0; i < z; i++) mb[k++] = padc;
                strcpy(mb + k, num);
            } else sprintf(mb, "%s%s", sign, num);
            made = mb;
        } else if (conv == 's') {
            const char *sv = a.v == DT_BOOL ? (a.i ? "True" : "False") : rk_cstr(a);
            if (!sv) sv = "";
            size_t blen = strlen(sv);
            if (prec >= 0 && (size_t) prec < rk_u8len(sv)) { size_t q = 0, cc = 0; while (sv[q] && cc < (size_t) prec) { q++; while (((unsigned char) sv[q] & 0xC0) == 0x80) q++; cc++; } blen = q; }
            made = (char *) rt_wsb_alloc(blen + 1);
            memcpy(made, sv, blen);
            made[blen] = 0;
        } else if (conv == 'c') {
            rk_rsp_arg_num(a, &iv, &dv, &isint, &big);
            char ch[8];
            size_t cn = rk_utf8_put((unsigned) iv, ch);
            made = (char *) rt_wsb_alloc(cn + 1);
            memcpy(made, ch, cn);
            made[cn] = 0;
        } else {
            rk_sb_put(&o, start, (size_t) (p - start));
            continue;
        }
        if (has_size) rk_rsp_legacy_pad(&o, made, strlen(made), size, isminus, padc);
        else rk_sb_put(&o, made, strlen(made));
    }
    (void) explicit_idx;
    if (!o.b) o.b = (char *) rt_wsb_alloc(1);
    o.b[o.n] = 0;
    *outp = o.b;
    if (outlen) *outlen = o.n;
}
