#include "runtime/lib/bn_head.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include <ctype.h>
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_37_pas.h"
double pas_xr_todouble(DESCR_t d);
static void pas_currency_str(double r, char *buf, int bufsz);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_fmt_round(unsigned char *buf, int *ncur, int nmax) {
    int n = *ncur, dr = buf[nmax];
    *ncur = nmax;
    if (dr == 4 && nmax < n - 3 && buf[n - 2] >= 8) { int i = n - 2; do i--; while (i != nmax && buf[i] == 9); if (i == nmax) dr = 9; }
    if (dr < 5) return 0;
    while (nmax > 0) { nmax--; buf[nmax]++; if (buf[nmax] < 10) { *ncur = nmax + 1; return 0; } }
    buf[0] = 1;
    *ncur = 1;
    return 1;
}
static int pas_fmt_digits(double v, unsigned char *buf, int *dot) {
    *dot = 1;
    if (v == 0.0) return 0;
    int cap = 48, n = 0;
    char t[cap];
    snprintf(t, (size_t)cap, "%.20e", fabs(v));
    const char *ep = strchr(t, 'e');
    for (const char *q = t; q < ep; q++) if (*q >= '0' && *q <= '9') buf[n++] = (unsigned char)(*q - '0');
    *dot = atoi(ep + 1) + 1;
    while (n > 1 && buf[n - 1] == 0) n--;
    return n;
}
static int pas_fmt_fixed(int minus, const unsigned char *dg, int n, int dot, int min_width, int frac, char *o) {
    unsigned char t[n + 1];
    if (n > 0) memcpy(t, dg, (size_t)n);
    int cut = dot + frac;
    if (cut < 0) n = 0;
    else if (cut < n) dot += pas_fmt_round(t, &n, cut);
    int nb = dot, nbp = 0;
    if (dot <= 0 || n == 0) { nb = 0; nbp = 1; } else if (dot > n) { nb = n; nbp = dot - n; }
    int nap = dot < 0 ? -dot : 0;
    if (nap > frac) nap = frac;
    int na = n - nb, ntp = frac - na - nap, len = minus + nb + nbp;
    if (frac > 0) len += nap + na + ntp + 1;
    if (len > 255) return 0;
    int sp = min_width - len;
    if (sp > 255 - len) sp = 255 - len;
    char *p = o;
    for (int i = 0; i < sp; i++) *p++ = ' ';
    if (minus) *p++ = '-';
    int j = 0;
    for (int i = 0; i < nb; i++) *p++ = (char)('0' + t[j++]);
    for (int i = 0; i < nbp; i++) *p++ = '0';
    if (frac != 0) { *p++ = '.'; for (int i = 0; i < nap; i++) *p++ = '0'; for (int i = 0; i < na; i++) *p++ = (char)('0' + t[j++]); for (int i = 0; i < ntp; i++) *p++ = '0'; }
    *p = '\0';
    return 1;
}
static void pas_fmt_exp(int minus, const unsigned char *dg, int n, int nreq, int dexp, int min_width, char *o) {
    int em = dexp < 0, ne = 1;
    if (em) dexp = -dexp;
    for (int t = dexp; t >= 10; t /= 10) ne++;
    int len = (ne <= 4 ? 4 : ne) + 1 + nreq + 2 + (nreq > 1), sp = min_width - len;
    if (sp > 255 - len) sp = 255 - len;
    char *p = o;
    for (int i = 0; i < sp; i++) *p++ = ' ';
    *p++ = minus ? '-' : ' ';
    *p++ = n > 0 ? (char)('0' + dg[0]) : '0';
    if (nreq > 1) *p++ = '.';
    int j = 1;
    while (j < n && j < nreq) *p++ = (char)('0' + dg[j++]);
    for (int k = nreq - j; k > 0; k--) *p++ = '0';
    *p++ = 'E';
    *p++ = em ? '-' : '+';
    for (int k = 4 - ne; k > 0; k--) *p++ = '0';
    sprintf(p, "%d", dexp);
}
static void pas_fmt_str_real(double v, int min_width, int frac, char *o) {
    if (!isfinite(v)) { sprintf(o, "%*s", min_width, isnan(v) ? "Nan" : v < 0 ? "-Inf" : "+Inf"); return; }
    int dot, minus = signbit(v) != 0, nsci = min_width < 0 ? 21 : min_width - 8;
    if (nsci < 2) nsci = 2;
    if (nsci > 21) nsci = 21;
    int dcap = 24;
    unsigned char dg[dcap];
    int n = pas_fmt_digits(v, dg, &dot);
    if (frac >= 0 && pas_fmt_fixed(minus, dg, n, dot, min_width, frac, o)) return;
    if (n > nsci) dot += pas_fmt_round(dg, &n, nsci);
    pas_fmt_exp(minus, dg, n, nsci, n > 0 ? dot - 1 : 0, min_width, o);
}
void pas_fmt_fixed_double(double v, int min_width, int frac, char *o) {
    if (!isfinite(v) || frac < 0) { pas_fmt_str_real(v, min_width, frac, o); return; }
    int dot, minus = signbit(v) != 0;
    unsigned char dg[24];
    int n = pas_fmt_digits(v, dg, &dot);
    if (n > 17) dot += pas_fmt_round(dg, &n, 17);
    if (!pas_fmt_fixed(minus, dg, n, dot, min_width, frac, o)) pas_fmt_str_real(v, min_width, frac, o);
}
static void pas_fmt_del(char *s, int i, int cnt) { memmove(s + i, s + i + cnt, strlen(s + i + cnt) + 1); }
static void pas_fmt_ins(char *s, int i, const char *x, int cnt) { memmove(s + i + cnt, s + i, strlen(s + i) + 1); memcpy(s + i, x, (size_t)cnt); }
static void pas_fmt_float_shape(char kind, char *s) {
    if (kind == 'E') {
        char *e = strchr(s, 'E');
        if (!e) return;
        int p = (int)(e - s) + 2, dg = (int)strlen(s) - p - 3;
        if (dg < 0) pas_fmt_ins(s, p, "0000", -dg);
        else while (dg > 0 && s[p] == '0') { pas_fmt_del(s, p, 1); if (p >= (int)strlen(s)) { pas_fmt_del(s, p - 2, 2); break; } dg--; }
        return;
    }
    if (kind == 'N') { char *d = strchr(s, '.'); for (int p = (d ? (int)(d - s) + 1 : (int)strlen(s) + 1) - 3; p > 1; p -= 3) if (s[p - 2] != '-') pas_fmt_ins(s, p - 1, ",", 1); return; }
    if (kind != 'G') return;
    char *d = strchr(s, '.'), *e = strchr(s, 'E');
    if (!d || !e) return;
    int P = (int)(d - s), PE = (int)(e - s), ex = atoi(s + PE + 2), len;
    if (s[PE + 1] == '-') ex = -ex;
    if (P + ex < PE && ex > -6) {
        s[PE] = '\0';
        len = PE;
        if (ex >= 0) {
            for (int q = 0; q < ex; q++) { s[P] = s[P + 1]; P++; }
            s[P] = '.';
            P = s[0] == '-';
            while (s[P] == '0' && P < len - 1 && s[P + 1] != '.') { pas_fmt_del(s, P, 1); len--; }
        } else {
            pas_fmt_ins(s, P - 1, "00000", -ex);
            s[P - ex] = s[P - ex - 1];
            s[P] = '.';
            if (ex != -1) s[P - ex - 1] = '0';
            len = (int)strlen(s);
        }
        int q = len - 1;
        while (q >= 0 && s[q] == '0') q--;
        if (q >= 0 && s[q] == '.') q--;
        if (q < 0 || (q == 0 && s[0] == '-')) strcpy(s, "0");
        else s[q + 1] = '\0';
        return;
    }
    while (PE > 0 && s[PE - 1] == '0') { pas_fmt_del(s, PE - 1, 1); PE--; }
    if (PE > 0 && s[PE - 1] == '.') { pas_fmt_del(s, PE - 1, 1); PE--; }
    if (s[PE + 1] == '+') pas_fmt_del(s, PE + 1, 1);
    else PE++;
    while (s[PE + 1] == '0' && s[PE + 2]) pas_fmt_del(s, PE + 1, 1);
}
static void pas_fmt_float(double v, char kind, int prec, char *s) {
    if (kind == 'E' || kind == 'G') {
        if (prec < 0 || prec > 17) prec = 17;
        pas_fmt_str_real(v, prec + 8, -1, s);
        while (s[0] == ' ') pas_fmt_del(s, 0, 1);
    } else {
        int d = prec < 0 ? 2 : prec > (kind == 'N' ? 17 : 18) ? (kind == 'N' ? 17 : 18) : prec;
        pas_fmt_str_real(v, 0, d, s);
        if (s[0] == ' ') pas_fmt_del(s, 0, 1);
    }
    pas_fmt_float_shape(kind, s);
    if (s[0] == '-' && s[1] && s[strspn(s + 1, "0.E+,") + 1] == '\0') pas_fmt_del(s, 0, 1);
}
static void pas_fmt_fail(const char *msg, const char *fmt) { fflush(NULL); fprintf(stderr, "An unhandled exception occurred at $0000000000000000:\nEConvertError: %s \"%s\"\n", msg, fmt); exit(217); }
size_t pas_format(const char *fmt, const DESCR_t *a, int na, char *out, size_t cap) {
    int len = (int)strlen(fmt), ch = 0, old = 0, argpos = 0, nbcap = 640;
    size_t w = 0;
    char nb[nbcap];
    const char *add = "";
    while (ch < len) {
        while (ch < len && fmt[ch] != '%') ch++;
        for (int i = old; i < ch; i++) { if (w < cap) out[w] = fmt[i]; w++; }
        if (ch < len - 1) {
            int index = -1, width = -1, prec = -1, value = -1, left = 0, doarg;
            char fc;
            ch++;
            if (fmt[ch] == '%') fc = '%';
            else {
                for (int stage = 0; stage < 3; stage++) {
                    if (stage == 1) { if (fmt[ch] == '-') { left = 1; ch++; } }
                    if (stage == 2 && fmt[ch] == '.') { ch++; value = -1; } else if (stage == 2) break;
                    if (stage == 0 && fmt[ch] == ':') value = 0;
                    else if (value == -1) {
                        int o0 = ch;
                        while (ch < len && fmt[ch] >= '0' && fmt[ch] <= '9') ch++;
                        if (ch >= len) pas_fmt_fail("Invalid format specifier :", fmt);
                        if (fmt[ch] == '*') {
                            int an = index == -1 ? argpos : index++;
                            if (ch > o0 || an >= na || !IS_INT_fn(a[an])) pas_fmt_fail("Invalid format specifier :", fmt);
                            argpos = an + 1;
                            value = (int)a[an].i;
                            ch++;
                        } else value = ch > o0 ? atoi(fmt + o0) : -1;
                    }
                    if (stage == 0 && fmt[ch] == ':') { if (value == -1) pas_fmt_fail("Missing argument in format", fmt); index = value; value = -1; ch++; }
                    if (stage == 1 && value != -1) { width = value; value = -1; }
                    if (stage == 2) prec = value == -1 ? 0 : value;
                }
                fc = (char)toupper((unsigned char)fmt[ch]);
            }
            if (fc != '%' && strchr("DUEFGNMSPX", fc)) {
                doarg = index == -1 ? argpos : index;
                argpos = doarg + 1;
                if (doarg >= na) pas_fmt_fail("Invalid argument index in format", fmt);
            } else doarg = -1;
            const DESCR_t *d = doarg >= 0 ? &a[doarg] : NULL;
            int isint = d && IS_INT_fn(*d), isreal = d && d->v == DT_R;
            long long iv = isint ? (long long)d->i : isreal ? (long long)d->r : 0;
            switch (fc) {
                case 'D':
                case 'U':
                {
                    if (fc == 'U') snprintf(nb, (size_t)nbcap, "%llu", (iv >= INT32_MIN && iv <= INT32_MAX) ? (unsigned long long)(uint32_t)iv : (unsigned long long)iv);
                    else snprintf(nb, (size_t)nbcap, "%lld", iv);
                    int l = (int)strlen(nb), z = prec - l + (nb[0] == '-');
                    for (int k = 0; k < z && l + k + 1 < nbcap; k++) pas_fmt_ins(nb, nb[0] == '-', "0", 1);
                    add = nb;
                    break;
                }
                case 'E':
                case 'F':
                case 'G':
                case 'N':
                case 'M':
                if (d && d->v == DT_XR && (fc == 'F' || fc == 'M')) {
                    extern void pas_xr_fixed(DESCR_t, int, char *, size_t);
                    pas_xr_fixed(*d, prec < 0 ? 2 : prec, nb, (size_t)nbcap);
                    add = nb;
                    break;
                }
                if (d && d->v == DT_XR && (fc == 'E' || fc == 'G')) {
                    extern DESCR_t pas_xr_str_d(DESCR_t, long long, long long);
                    DESCR_t xs = pas_xr_str_d(*d, (prec < 0 || prec > 17 ? 17 : prec) + 8, -1);
                    size_t xl = xs.slen < (size_t)nbcap - 1 ? xs.slen : (size_t)nbcap - 1;
                    memcpy(nb, xs.s, xl);
                    nb[xl] = '\0';
                    while (nb[0] == ' ') pas_fmt_del(nb, 0, 1);
                    pas_fmt_float_shape(fc, nb);
                    if (nb[0] == '-' && nb[1] && nb[strspn(nb + 1, "0.E+,") + 1] == '\0') pas_fmt_del(nb, 0, 1);
                    add = nb;
                    break;
                }
                pas_fmt_float(isreal ? d->r : d && d->v == DT_XR ? pas_xr_todouble(*d) : (double)iv, fc == 'M' ? 'F' : fc, prec, nb);
                add = nb;
                break;
                case 'S':
                {
                    const char *sv = isint ? (snprintf(nb, (size_t)nbcap, "%lld", iv), nb) : isreal ? (pas_fmt_float(d->r, 'G', -1, nb), nb) : VARVAL_fn(*d);
                    if (!sv) sv = "";
                    int l = (int)strlen(sv);
                    if (prec != -1 && l > prec) { if (sv != nb) { memcpy(nb, sv, (size_t)(prec < nbcap ? prec : nbcap - 1)); sv = nb; } nb[prec < nbcap ? prec : nbcap - 1] = '\0'; }
                    add = sv;
                    break;
                }
                case 'P':
                snprintf(nb, (size_t)nbcap, "%016llX", (unsigned long long)iv);
                add = nb;
                break;
                case 'X':
                {
                    int is32 = iv >= INT32_MIN && iv <= INT32_MAX, mx = is32 ? 16 : 31;
                    unsigned long long vq = is32 ? (unsigned long long)(uint32_t)iv : (unsigned long long)iv;
                    int nd = 1;
                    if (prec > mx) nd = mx;
                    else { while (nd < 16 && (1ULL << (nd * 4)) <= vq) nd++; if (nd < prec) nd = prec; }
                    for (int k = 0; k < nd && k < nbcap - 1; k++) nb[k] = "0123456789ABCDEF"[k < nd - 16 ? 0 : (vq >> (4 * (nd - 1 - k))) & 15];
                    nb[nd < nbcap - 1 ? nd : nbcap - 1] = '\0';
                    add = nb;
                    break;
                }
                case '%':
                add = "%";
                break;
                default:
                break;
            }
            int al = (int)strlen(add), pad = width != -1 && al < width ? width - al : 0;
            if (!left) for (int k = 0; k < pad; k++) { if (w < cap) out[w] = ' '; w++; }
            for (int k = 0; k < al; k++) { if (w < cap) out[w] = add[k]; w++; }
            if (left) for (int k = 0; k < pad; k++) { if (w < cap) out[w] = ' '; w++; }
        }
        ch++;
        old = ch;
    }
    return w;
}
DESCR_t c_rt_pas_sqr(DESCR_t *args, int nargs) {
    (void) nargs;
    if (IS_REAL_fn(args[0])) return REALVAL(args[0].r * args[0].r);
    long v = IS_INT_fn(args[0]) ? args[0].i : 0;
    if (v > 46340 || v < -46340) pas_file_err("6.6.6.2", "sqr(x) of an integer whose square exceeds maxint, so no such value exists", NULL);
    return INTVAL(v * v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_itoa(char *b, long long v, int uns) {
    char t[24];
    int n = 0, k = 0;
    unsigned long long u = (uns || v >= 0) ? (unsigned long long)v : 0ULL - (unsigned long long)v;
    do { t[n++] = (char)('0' + (int)(u % 10)); u /= 10; } while (u);
    if (!uns && v < 0) b[k++] = '-';
    while (n) b[k++] = t[--n];
    b[k] = '\0';
    return k;
}
static void pas_wput(pas_wsink_t *k, const char *s, size_t n) {
    if (k->fp) { fwrite(s, 1, n, k->fp); return; }
    if (k->n + n + 1 > k->cap) { size_t c = (k->n + n + 1) * 2 + 32; k->b = (char *)rt_wsb_realloc(k->b, c); k->cap = c; }
    if (n) memcpy(k->b + k->n, s, n);
    k->n += n;
    k->b[k->n] = '\0';
}
static void pas_wpad(pas_wsink_t *k, long n) { for (long i = 0; i < n; i++) pas_wput(k, " ", 1); }
static void pas_put_rjust(pas_wsink_t *k, const char *s, int len, int width) { pas_wpad(k, (long)width - (long)len); pas_wput(k, s, (size_t)len); }
void pas_write_items(pas_wsink_t *k, DESCR_t *args, int nargs, int start) {
    for (int _pi = start; _pi + 1 < nargs; _pi += 2) {
        DESCR_t av = args[_pi];
        DESCR_t aw = args[_pi + 1];
        int w = IS_INT_fn(aw) ?
            (aw.i == -3 ? -3 : (aw.i == -4 ? -4 : (aw.i == -5 ? -5 : (aw.i == -6 ? -6 : (aw.i == -7 ? -7 : (aw.i == -8 ? -8 : (aw.i == -12 ? -12 : (aw.i >= 0 ? (int)aw.i : -1)))))))) : -1;
        int _upper = 0, _uns = 0;
        if (IS_INT_fn(aw) && aw.i == -11 && _pi + 3 < nargs) { _uns = 1; w = (IS_INT_fn(args[_pi + 2]) && args[_pi + 2].i >= 0) ? (int)args[_pi + 2].i : -1; _pi += 2; }
        if (IS_INT_fn(aw) && aw.i == -10 && _pi + 3 < nargs) { _upper = 1; w = (IS_INT_fn(args[_pi + 2]) && args[_pi + 2].i >= 0) ? (int)args[_pi + 2].i : -1; _pi += 2; }
        if (w == -6) {
            double _cv = IS_REAL_fn(av) ? av.r : (IS_INT_fn(av) ? (double)av.i : 0.0);
            int _cn = 32;
            char _cb[_cn];
            pas_currency_str(_cv, _cb, _cn);
            pas_wput(k, _cb, strlen(_cb));
            continue;
        }
        if (w == -3) {
            double _rv = IS_REAL_fn(av) ? av.r : (IS_INT_fn(av) ? (double)av.i : 0.0);
            long _fw2 = 0, _fp2 = 0;
            if (_pi + 3 < nargs) { if (IS_INT_fn(args[_pi+2])) _fw2 = (long)args[_pi+2].i; if (IS_INT_fn(args[_pi+3])) _fp2 = (long)args[_pi+3].i; }
            if (_fw2 < 0) _fw2 = 0;
            if (_fp2 < 0) _fp2 = 0;
            if (_fp2 > 255) _fp2 = 255;
            char _fxb[(size_t)(_fw2 > 255 ? _fw2 : 255) + 64];
            pas_fmt_fixed_double(_rv, (int)_fw2, (int)_fp2, _fxb);
            pas_wput(k, _fxb, strlen(_fxb));
            _pi += 2;
            continue;
        }
        if (IS_INT_fn(av)) {
            int _pbn = 32;
            char _pbq[_pbn];
            int _pfmtlen = pas_itoa(_pbq, (long long)av.i, w == -7 || w == -8 || _uns);
            if (w < 0 && w != -4 && w != -8 && _pfmtlen > 11) { _pbq[11] = '\0'; _pfmtlen = 11; }
            int _fw = (w == -4 || w == -8) ? _pfmtlen : (w < 0) ? 11 : (w > _pfmtlen ? w : _pfmtlen);
            pas_put_rjust(k, _pbq, _pfmtlen, _fw);
        } else if (IS_REAL_fn(av)) {
            char _rb[64];
            int _sgl = w == -5 || w == -12;
            int _prec = _sgl ? 9 : (w < 0) ? 16 : (w - 8 < 1 ? 1 : (w - 8 > 16 ? 16 : w - 8));
            pas_real_str_exp(_sgl ? (double)(float)av.r : av.r, _rb, sizeof _rb, _prec, _sgl ? 2 : 3, _upper || w == -4 || w == -5);
            int _pfmtlen = (int)strlen(_rb);
            int _signpad = (_rb[0] == '-') ? 0 : 1;
            int _basew = _sgl ? 0 : (w < 0) ? 20 : w;
            int _fw = (_pfmtlen + _signpad > _basew) ? _pfmtlen + _signpad : _basew;
            pas_put_rjust(k, _rb, _pfmtlen, _fw);
        } else {
            const char *_ps = VARVAL_fn(av);
            if (_ps) {
                if (w == -2) pas_wput(k, _ps, 1);
                else if (w >= 0) { size_t _l = pas_slen(av, _ps); pas_wpad(k, (long)w - (long)_l); pas_wput(k, _ps, _l); } else pas_wput(k, _ps, pas_slen(av, _ps));
            }
        }
    }
}
DESCR_t pas_writestr(DESCR_t *args, int nargs) { pas_wsink_t k = { NULL, NULL, 0, 0 }; pas_wput(&k, "", 0); pas_write_items(&k, args, nargs, 0); return BSTRVAL(k.b, k.n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void pas_currency_str(double r, char *buf, int bufsz) {
    long long n = llround(r * 10000.0);
    int neg = n < 0;
    unsigned long long u = neg ? (unsigned long long)(-n) : (unsigned long long)n;
    int L = 1;
    for (unsigned long long t = u; t >= 10; t /= 10) L++;
    int e = (u == 0) ? 0 : L - 1 - 4;
    unsigned long long p = 1;
    for (int i = 1; i < L; i++) p *= 10;
    if (bufsz < 28) { if (bufsz > 0) buf[0] = '\0'; return; }
    char *o = buf;
    *o++ = neg ? '-' : ' ';
    for (int i = 0; i <= 18; i++) { if (i == 1) *o++ = '.'; if (i < L) { *o++ = (char)('0' + (int)((u / p) % 10ULL)); p /= 10; } else *o++ = '0'; }
    *o++ = 'E';
    *o++ = e < 0 ? '-' : '+';
    if (e < 0) e = -e;
    *o++ = (char)('0' + e / 10);
    *o++ = (char)('0' + e % 10);
    *o = '\0';
}
void pas_real_str_exp(double r, char *buf, int bufsz, int prec, int expdig, int upper) {
    if (prec < 1) prec = 1;
    if (prec > 16) prec = 16;
    char tmp[64];
    snprintf(tmp, sizeof tmp, "%.*e", prec, r);
    char *ep = strchr(tmp, 'e');
    if (!ep) { snprintf(buf, bufsz, "%s", tmp); return; }
    char sign = ep[1];
    const char *digits = ep + 2;
    int ndig = (int)strlen(digits);
    int ml = (int)(ep - tmp);
    char mant[ml + 1];
    memcpy(mant, tmp, ml);
    mant[ml] = '\0';
    if (ndig < expdig) snprintf(buf, bufsz, "%s%c%c%0*d", mant, upper ? 'E' : 'e', sign, expdig, atoi(digits));
    else { snprintf(buf, bufsz, "%s", tmp); if (upper) { char *q = strchr(buf, 'e'); if (q) *q = 'E'; } }
}
