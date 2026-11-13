#include "runtime/lib/bn_head.h"
#include "ir/rk_opname.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/rt/prolog_atom.h"
#include "driver/driver_globals.h"
#include "runtime/rx.h"
#include "runtime/core/utf8.h"
#include "runtime/builtin_leaf.h"
#include <ctype.h>
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_22_rk.h"
#include "runtime/lib/bn_34_pl.h"
#include "runtime/rk_case_table.inc"
extern void *rt_ws_alloc_descr(size_t);
extern FILE *fh_get(int);
const char *rk_typeobj_name(DESCR_t v) { return (IS_STR_fn(v) && v.s && v.s[0] == RK_TY) ? v.s + 1 : NULL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *itos(long long v, char *buf, size_t cap) {
    if (cap < 2) { buf[0] = '\0'; return buf; }
    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    char tmp[24];
    int n = 0;
    int neg = (v < 0);
    unsigned long long u = neg ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
    while (u) { tmp[n++] = (char)('0' + (int)(u % 10)); u /= 10; }
    if (neg) tmp[n++] = '-';
    size_t len = (size_t)n;
    if (len >= cap) len = cap - 1;
    for (size_t i = 0; i < len; i++) buf[i] = tmp[len - 1 - i];
    buf[len] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *to_cstring(DESCR_t v, char *scratch, size_t scap) {
    if (v.v == DT_DATA) { extern int rk_rat_is(DESCR_t); extern const char *rk_rat_str(DESCR_t, int); if (rk_rat_is(v)) return rk_rat_str(v, 0); }
    if (v.v == DT_BOOL) { return v.i ? "True" : "False"; }
    if (v.v == DT_ORDER) { return v.i < 0 ? "Less" : (v.i > 0 ? "More" : "Same"); }
    if (IS_INT_fn(v)) { return itos((long long)v.i, scratch, scap); }
    if (IS_REAL_fn(v)) { return rtos(v.r, scratch, scap); }
    const char *s = VARVAL_fn(v);
    return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
double rk_atanh(double x) { return 0.5 * log((1.0 + x) / (1.0 - x)); }
unsigned rk_utf8_cp(const unsigned char *p, size_t n, size_t *len) {
    if (p[0] < 0xC0 || n < 2) { *len = 1; return p[0]; }
    size_t k = p[0] >= 0xF0 ? 4 : p[0] >= 0xE0 ? 3 : 2;
    if (k > n) { *len = 1; return p[0]; }
    unsigned c = p[0] & (0x7Fu >> k);
    for (size_t i = 1; i < k; i++) c = (c << 6) | (p[i] & 0x3Fu);
    *len = k;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t rk_utf8_put(unsigned cp, char *o) {
    if (cp < 0x80) { o[0] = (char)cp; return 1; }
    if (cp < 0x800) { o[0] = (char)(0xC0 | (cp >> 6)); o[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char)(0xE0 | (cp >> 12)); o[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char)(0xF0 | ((cp >> 18) & 0x07));
    o[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    o[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}
static int rk_case_cmp(const void *k, const void *e) { unsigned a = *(const unsigned *)k, b = ((const RkCase *)e)->cp; return a < b ? -1 : a > b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *rk_case_one(unsigned cp, int m) {
    const RkCase *e = (const RkCase *)bsearch(&cp, rk_case, sizeof rk_case / sizeof *rk_case, sizeof *rk_case, rk_case_cmp);
    if (!e) return NULL;
    return rk_case_at(m == 1 ? e->uc : m == 2 ? e->lc : m == 3 ? e->tc : e->fc);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_wordish(unsigned cp) {
    if (cp < 0x80) return isalnum((int)cp) || cp == '_';
    if (cp >= 0x2000 && cp <= 0x206F) return 0;
    if (cp >= 0x3000 && cp <= 0x303F) return 0;
    return cp >= 0xC0 && cp != 0xD7 && cp != 0xF7;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rk_case_str(const char *s, size_t n, int mfirst, int mrest, int words) {
    size_t need = 0;
    for (int pass = 0; pass < 2; pass++) {
        char *r = pass ? (char *)rt_str_alloc(need) : NULL;
        size_t o = 0;
        int inw = 0;
        for (size_t i = 0; i < n; ) {
            size_t L;
            unsigned cp = rk_utf8_cp((const unsigned char *)s + i, n - i, &L);
            int m;
            if (words) {
                size_t L2 = 0;
                unsigned nx = i + L < n ? rk_utf8_cp((const unsigned char *)s + i + L, n - i - L, &L2) : 0;
                int letter = rk_wordish(cp) && !(cp < 0x80 && isdigit((int)cp)) && cp != '_';
                if (!inw && letter) { m = 3; inw = 1; } else if (inw && (rk_wordish(cp) || ((cp == '\'' || cp == '-') && nx && rk_wordish(nx)))) m = 2;
                else { m = 0; inw = 0; }
            } else m = i == 0 ? mfirst : mrest;
            const char *rep = m ? rk_case_one(cp, m) : NULL;
            size_t rl = rep ? strlen(rep) : L;
            if (pass) memcpy(r + o, rep ? rep : s + i, rl);
            o += rl;
            i += L;
        }
        if (pass) { r[o] = '\0'; return r; }
        need = o;
    }
    return NULL;
}
int rk_trans_items(DESCR_t side, const char **o, int cap) {
    int n = 0;
    if (side.v == DT_A && side.arr && !rk_is_pair(side)) { rk_av_t a = rk_av(side); for (int i = 0; i < a.n; i++) { if (o && n < cap) o[n] = rk_av_text(a, i); n++; } return n; }
    const char *t = rk_cstr(side);
    size_t tl = strlen(t);
    for (size_t i = 0, l; i < tl; i += l) {
        unsigned cp = rk_utf8_cp((const unsigned char *) t + i, tl - i, &l);
        if (!l) l = 1;
        size_t l2 = 0, l3 = 0, l4 = 0;
        unsigned c2 = i + l < tl ? rk_utf8_cp((const unsigned char *) t + i + l, tl - i - l, &l2) : 0;
        unsigned c3 = (c2 == '.' && i + l + l2 < tl) ? rk_utf8_cp((const unsigned char *) t + i + l + l2, tl - i - l - l2, &l3) : 0;
        if (c2 == '.' && c3 == '.' && i + l + l2 + l3 < tl) {
            unsigned hi = rk_utf8_cp((const unsigned char *) t + i + l + l2 + l3, tl - i - l - l2 - l3, &l4);
            for (unsigned c = cp; c <= hi; c++) { if (o && n < cap) { char *b = rt_wsb_alloc(8); size_t k = rk_utf8_put(c, b); b[k] = 0; o[n] = b; } n++; }
            l += l2 + l3 + (l4 ? l4 : 1);
            continue;
        }
        if (o && n < cap) { char *b = rt_wsb_alloc(l + 1); memcpy(b, t + i, l); b[l] = 0; o[n] = b; }
        n++;
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tr_cp(const char *s, int *i) {
    unsigned char c = (unsigned char) s[*i];
    if (c < 0x80) { (*i)++; return c; }
    int k = c >= 0xF0 ? 4 : c >= 0xE0 ? 3 : 2, v = c & (0x7F >> k);
    (*i)++;
    for (int j = 1; j < k && s[*i]; j++, (*i)++) v = (v << 6) | (s[*i] & 0x3F);
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tr_atom(const char *s, int *i) {
    if (s[*i] != '\\') return rk_tr_cp(s, i);
    (*i)++;
    char e = s[*i];
    if (e == 'x' || e == 'X') {
        (*i)++;
        int br = s[*i] == '[';
        if (br) (*i)++;
        int v = 0;
        while (isxdigit((unsigned char) s[*i])) { v = v * 16 + (isdigit((unsigned char) s[*i]) ? s[*i] - '0' : (tolower((unsigned char) s[*i]) - 'a' + 10)); (*i)++; }
        if (br && s[*i] == ']') (*i)++;
        return v;
    }
    (*i)++;
    switch (e) { case 'n': return 10; case 't': return 9; case 'r': return 13; case 'e': return 27; case '0': return 0; case 'f': return 12; case 'a': return 7; default: return (unsigned char) e; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_tr_expand(const char *s, unsigned *out) {
    int i = 0, n = 0;
    while (s[i]) {
        unsigned a = (unsigned) rk_tr_atom(s, &i);
        if (s[i] == '.' && s[i + 1] == '.' && s[i + 2]) {
            i += 2;
            unsigned b = (unsigned) rk_tr_atom(s, &i);
            for (unsigned c = a; c <= b; c++) { if (out) out[n] = c; n++; }
        } else {
            if (out) out[n] = a;
            n++;
        }
    }
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_trans_run(const char *subj, const char *search, const char *repl, long flags) {
    int ns = rk_tr_expand(search, NULL), nr = rk_tr_expand(repl, NULL);
    unsigned sa[ns + 1], ra[nr + 1];
    rk_tr_expand(search, sa);
    rk_tr_expand(repl, ra);
    int complement = (flags & 1) != 0, del = (flags & 2) != 0 || nr == 0, squash = (flags & 4) != 0;
    size_t n = strlen(subj);
    char *o = (char *) rt_wsb_alloc(n * 4 + 8);
    size_t k = 0;
    int last_out = -1, last_mapped = 0;
    for (int i = 0; subj[i];) {
        int cp = rk_tr_cp(subj, &i), idx = -1;
        for (int j = 0; j < ns; j++) if ((int) sa[j] == cp) { idx = j; break; }
        int hit = complement ? idx < 0 : idx >= 0;
        if (!hit) { k += rk_utf8_put((unsigned) cp, o + k); last_out = -1; last_mapped = 0; continue; }
        int rc;
        if (complement) rc = nr ? (int) ra[nr - 1] : -1;
        else rc = idx < nr ? (int) ra[idx] : (del ? -1 : (int) ra[nr - 1]);
        if (rc < 0) { last_mapped = 0; continue; }
        if (squash && last_mapped && last_out == rc) continue;
        k += rk_utf8_put((unsigned) rc, o + k);
        last_out = rc;
        last_mapped = 1;
    }
    o[k] = 0;
    return STRVAL(rt_heap_strdup_c(o));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_txt_elem(const char *t, size_t L) {
    if (!L) return STRVAL(rt_heap_strdup_c(""));
    DESCR_t d = elem_to_descr(t, L);
    if (IS_INT_fn(d) || IS_REAL_fn(d)) { const char *b = rk_cstr(d); if (strlen(b) == L && !memcmp(b, t, L)) return d; char *e = rt_wsb_alloc(L + 1); memcpy(e, t, L); e[L] = '\0'; return STRVAL(e); }
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_arr_of_texts(const char *const *tx, const size_t *ln, int n) {
    DESCR_t *r = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
    for (int i = 0; i < n; i++) r[i] = rk_txt_elem(tx[i], ln[i]);
    return rk_mk_arr(r, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t rk_lines_arr(const char *s, size_t n) {
    size_t cap = 1;
    for (size_t i = 0; i < n; i++) if (s[i] == '\n') cap++;
    DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr(cap);
    char *e = rt_wsb_alloc(n + 1);
    int c = 0;
    size_t i = 0;
    while (i < n) { size_t L = 0; while (i < n && s[i] != '\n') { if (s[i] != '\r') e[L++] = s[i]; i++; } e[L] = '\0'; if (i < n) i++; r[c++] = rk_txt_elem(e, L); }
    return rk_mk_arr(r, c);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rk_av_joined(rk_av_t a, const char *sep, const char *open, const char *close) {
    size_t sl = strlen(sep), ol = strlen(open), cl = strlen(close), tot = ol + cl + 1;
    const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
    for (int i = 0; i < a.n; i++) { tx[i] = rk_typeobj_name(a.el[i]) ? "" : rk_av_text(a, i); tot += strlen(tx[i]) + (i ? sl : 0); }
    char *buf = rt_wsb_alloc(tot);
    size_t p = 0;
    memcpy(buf + p, open, ol);
    p += ol;
    for (int i = 0; i < a.n; i++) { if (i) { memcpy(buf + p, sep, sl); p += sl; } size_t L = strlen(tx[i]); memcpy(buf + p, tx[i], L); p += L; }
    memcpy(buf + p, close, cl);
    p += cl;
    buf[p] = '\0';
    return buf;
}
const char *rk_uni_str(const unsigned *c, int n) { char *s = rt_wsb_alloc((size_t) n * 4 + 1); size_t k = 0; for (int i = 0; i < n; i++) k += rk_utf8_put(c[i], s + k); s[k] = '\0'; return s; }
int rk_seq_stop(DESCR_t v, DESCR_t ev, int kind, int dir, int gen_code) {
    if (rk_typeobj_name(ev) != NULL || (IS_REAL_fn(ev) && isinf(ev.r)) || ev.v == DT_SNUL) return 0;
    if (rk_is_code(ev)) return 4;
    double dE = 0;
    int iE = 0;
    if (rk_range_num(ev, &dE, &iE)) {
        double dv;
        int iv;
        if (rk_range_num(v, &dv, &iv)) {
            double tol = 1e-9 * (fabs(dE) > 1 ? fabs(dE) : 1);
            if (fabs(dv - dE) <= (iv && iE ? 0 : tol)) return iv ? 1 : 3;
            if (!gen_code && ((dir > 0 && dv > dE) || (dir < 0 && dv < dE))) return 2;
        }
        return 0;
    }
    { const char *vs = rk_cstr(v), *es = rk_cstr(ev); if (!strcmp(vs, es)) return 1; if (kind == 3 && !gen_code && strlen(vs) > strlen(es)) return 2; }
    return 0;
}
DESCR_t bt_m_chars(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return INTVAL((long)utf8_strlen(s)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_uc(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 1, 1, 0)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_lc(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 2, 2, 0)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_fc(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 4, 4, 0)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_tc(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 3, 0, 0)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_tclc(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 3, 2, 0)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_flip(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        char *r = (char *)rt_str_alloc(n);
        size_t o = n;
        for (size_t i = 0; i < n; ) { size_t L; rk_utf8_cp((const unsigned char *)s + i, n - i, &L); o -= L; memcpy(r + o, s + i, L); i += L; }
        r[n] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_trim(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        size_t a = 0, b = n;
        while (a < b && isspace((unsigned char)s[a])) a++;
        while (b > a && isspace((unsigned char)s[b - 1])) b--;
        char *r = (char *)rt_str_alloc(b - a);
        memcpy(r, s + a, b - a);
        r[b - a] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_chop(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        long cnt = 1;
        if ((nargs - 1) >= 1 && (args + 1)) {
            DESCR_t c = args[1 + (0)];
            if (IS_INT_fn(c)) cnt = (long)c.i;
            else if (IS_REAL_fn(c)) cnt = (long)c.r;
            else if (c.v == DT_BIG) cnt = LONG_MAX;
            else { const char *t = to_cstring(c, sb, sizeof sb); cnt = (t && strspn(t, "0123456789") > 18) ? LONG_MAX : t ? atol(t) : 1; }
        }
        size_t b = n;
        for (long k = 0; k < cnt && b > 0; k++) { b--; while (b > 0 && ((unsigned char)s[b] & 0xC0) == 0x80) b--; }
        char *r = (char *)rt_str_alloc(b);
        memcpy(r, s, b);
        r[b] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_wordcase(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return STRVAL(rk_case_str(s, n, 0, 0, 1)); }
    {
        char *r = (char *)rt_str_alloc(n);
        memcpy(r, s, n + 1);
        int start = 1;
        for (size_t i = 0; i < n; i++) { if (isspace((unsigned char)r[i])) start = 1; else { if (start) r[i] = (char)toupper((unsigned char)r[i]); start = 0; } }
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_contains(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1) { char nb[64]; const char *nd = to_cstring(args[1 + (0)], nb, sizeof nb); if (!nd) nd = ""; return INTVAL(strstr(s, nd) ? 1 : 0); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_startsx2Dwith(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1) {
        char nb[128];
        const char *nd = to_cstring(args[1 + (0)], nb, sizeof nb);
        if (!nd) nd = "";
        size_t ln = strlen(nd);
        return INTVAL((ln <= n && !strncmp(s, nd, ln)) ? 1 : 0);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_endsx2Dwith(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1) {
        char nb[128];
        const char *nd = to_cstring(args[1 + (0)], nb, sizeof nb);
        if (!nd) nd = "";
        size_t ln = strlen(nd);
        return INTVAL((ln <= n && !strncmp(s + (n - ln), nd, ln)) ? 1 : 0);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_subst(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1 && rk_rx_pat(args[1 + (0)]) && ((nargs - 1) < 2 || !rk_is_code(args[1 + (1)]))) {
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", rk_rx_pat(args[1 + (0)]), 0);
        if (!prog) { return FAILDESCR; }
        char rb2[256];
        const char *rep = (nargs - 1) >= 2 ? to_cstring(args[1 + (1)], rb2, sizeof rb2) : "";
        if (!rep) rep = "";
        size_t rl = strlen(rep);
        int glob = 0;
        for (int i = 2; i < (nargs - 1); i++) if (args[1 + (i)].v == DT_BOOL && args[1 + (i)].i) glob = 1;
        size_t cap = n + 1, olen = 0, pos = 0;
        char *o = (char *) rt_wsb_alloc(cap);
        int done = 0;
        while (pos <= n && !done) {
            RxMatch rm;
            if (!rx_exec(prog, s, (int) n, (int) pos, 0, &rm)) break;
            size_t ms = (size_t) rm.from, me = (size_t) rm.to;
            size_t need = olen + (ms - pos) + rl + (n - me) + 8;
            if (need > cap) { size_t nc = need * 2; char *no = (char *) rt_wsb_alloc(nc); memcpy(no, o, olen); o = no; cap = nc; }
            memcpy(o + olen, s + pos, ms - pos);
            olen += ms - pos;
            memcpy(o + olen, rep, rl);
            olen += rl;
            if (me == ms) { if (ms < n) o[olen++] = s[ms]; pos = ms + 1; } else pos = me;
            if (!glob) done = 1;
        }
        if (pos <= n) {
            size_t need = olen + (n - pos) + 8;
            if (need > cap) { char *no = (char *) rt_wsb_alloc(need); memcpy(no, o, olen); o = no; }
            memcpy(o + olen, s + pos, n - pos);
            olen += n - pos;
        }
        o[olen] = '\0';
        return STRVAL(rt_heap_strdup_c(o));
    }
    if ((nargs - 1) >= 1) {
        char nb[256];
        const char *needle = to_cstring(args[1 + (0)], nb, sizeof nb);
        if (!needle) needle = "";
        char rb[256];
        const char *repl = ((nargs - 1) >= 2) ? to_cstring(args[1 + (1)], rb, sizeof rb) : "";
        if (!repl) repl = "";
        size_t nl = strlen(needle), rl = strlen(repl);
        int glob = 0;
        for (int i = 2; i < (nargs - 1); i++) if (args[1 + (i)].v == DT_BOOL && args[1 + (i)].i) glob = 1;
        const char *hit = (nl > 0) ? strstr(s, needle) : NULL;
        if (!hit) { char *o = (char *)rt_str_alloc(n); memcpy(o, s, n + 1); return STRVAL(o); }
        size_t hits = 1;
        for (const char *h = hit + nl; glob && (h = strstr(h, needle)); h += nl) hits++;
        char *o = (char *)rt_str_alloc(n - hits * nl + hits * rl);
        size_t olen = 0;
        const char *from = s;
        for (size_t k = 0; k < hits; k++) {
            memcpy(o + olen, from, (size_t)(hit - from));
            olen += (size_t)(hit - from);
            memcpy(o + olen, repl, rl);
            olen += rl;
            from = hit + nl;
            hit = strstr(from, needle);
        }
        memcpy(o + olen, from, (size_t)(s + n - from));
        olen += (size_t)(s + n - from);
        o[olen] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_substr(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1) {
        long from = IS_INT_fn(args[1 + (0)]) ? (long)args[1 + (0)].i : atol(to_cstring(args[1 + (0)], sb, sizeof sb));
        long ln = ((nargs - 1) >= 2) ? (IS_INT_fn(args[1 + (1)]) ? (long)args[1 + (1)].i : atol(to_cstring(args[1 + (1)], sb, sizeof sb))) : (long)utf8_strlen(s) - from;
        if (from < 0) from = 0;
        if (ln < 0) ln = 0;
        long avail = (long)utf8_strlen(s) - from;
        if (avail < 0) avail = 0;
        if (ln > avail) ln = avail;
        if (ln == 0) { return STRVAL(rt_heap_strdup_c("")); }
        return SUBSTR_fn(args[0], INTVAL(from + 1), INTVAL(ln));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_chr(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { long cp = IS_INT_fn(args[0]) ? (long)args[0].i : (long)atoll(s); char *r = (char *)rt_str_alloc(4); size_t rl = rk_utf8_put((unsigned)cp, r); r[rl] = '\0'; return BSTRVAL(r, rl); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_ord(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { size_t on = IS_STR_fn(args[0]) ? descr_slen(args[0]) : n; if (!s || !on) { return FAILDESCR; } size_t ln; return INTVAL(rk_utf8_cp((const unsigned char *)s, on, &ln)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_parsex2Dbase(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) == 1 && IS_INT_fn(args[1 + (0)])) { return INTVAL(strtoll(s, NULL, (int) args[1 + (0)].i)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_ords(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) == 0) {
        size_t cnt = 0;
        { size_t ln; for (size_t i = 0; i < n; i += ln) { rk_utf8_cp((const unsigned char *) s + i, n - i, &ln); if (!ln) ln = 1; cnt++; } }
        DESCR_t *r = cnt ? (DESCR_t *) rt_ws_alloc_descr(cnt) : NULL;
        size_t k = 0, ln;
        for (size_t i = 0; i < n && k < cnt; i += ln) { unsigned cp = rk_utf8_cp((const unsigned char *) s + i, n - i, &ln); if (!ln) ln = 1; r[k++] = INTVAL(cp); }
        return rk_mk_arr(r, (int) cnt);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_sqrt(DESCR_t *args, int nargs) { { return REALVAL(sqrt(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_m_log(DESCR_t *args, int nargs) {
    { double lx = log(to_real(args[0])); if ((nargs - 1) >= 1) lx /= log(to_real(args[1 + (0)])); return REALVAL(lx); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_exp(DESCR_t *args, int nargs) { { return REALVAL(exp(to_real(args[0]))); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rand(DESCR_t *args, int nargs) { { return REALVAL((double)rand() / RAND_MAX * to_real(args[0])); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_words(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        const char **tx = (const char **) rt_pvec_alloc(n / 2 + 2);
        size_t *ln = (size_t *) rt_wsb_alloc((n / 2 + 2) * sizeof(size_t));
        int c = 0;
        size_t i = 0;
        while (i < n) { while (i < n && isspace((unsigned char)s[i])) i++; if (i >= n) break; size_t b = i; while (i < n && !isspace((unsigned char)s[i])) i++; tx[c] = s + b; ln[c] = i - b; c++; }
        return rk_arr_of_texts(tx, ln, c);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_codes(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) == 0) { return INTVAL((long) utf8_strlen(s)); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static const char *rk_cp_map(unsigned cp, int m, char *buf) {
    if (cp < 0x80) { buf[0] = (char) (m == 1 ? toupper((int) cp) : tolower((int) cp)); buf[1] = '\0'; return buf; }
    return rk_case_one(cp, m);
}
static int rk_cp_changes(unsigned cp, int m) { char b[16], e[8]; const char *x = rk_cp_map(cp, m, b); e[rk_utf8_put(cp, e)] = '\0'; return x && strcmp(x, e) != 0; }
DESCR_t bt_samecase(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) == 1) {
        char pb[256];
        const char *pat = to_cstring(args[1 + (0)], pb, sizeof pb);
        if (!pat) pat = "";
        size_t pn = strlen(pat);
        char *r = (char *) rt_str_alloc(n * 3 + 1);
        size_t o = 0, pi = 0, pl = 0, L = 0;
        unsigned pcp = 0;
        for (size_t i = 0; i < n; i += L) {
            unsigned cp = rk_utf8_cp((const unsigned char *) s + i, n - i, &L);
            char mb[16];
            if (pi < pn) { pcp = rk_utf8_cp((const unsigned char *) pat + pi, pn - pi, &pl); pi += pl; }
            const char *rep = rk_cp_changes(pcp, 2) ? rk_cp_map(cp, 1, mb) : rk_cp_changes(pcp, 1) ? rk_cp_map(cp, 2, mb) : NULL;
            size_t rl = rep ? strlen(rep) : L;
            memcpy(r + o, rep ? rep : s + i, rl);
            o += rl;
        }
        r[o] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_chomp(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        size_t b = n;
        if (b > 1 && s[b - 2] == '\r' && s[b - 1] == '\n') b -= 2;
        else if (b > 0 && (s[b - 1] == '\n' || s[b - 1] == '\r')) b--;
        char *r = (char *)rt_str_alloc(b);
        memcpy(r, s, b);
        r[b] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_lines(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { return rk_lines_arr(s, n); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_fmt(DESCR_t *args, int nargs) {
    {
        char fb[256];
        const char *fmt = ((nargs - 1) >= 1) ? to_cstring(args[1 + (0)], fb, sizeof fb) : "%s";
        if (!fmt) fmt = "%s";
        DESCR_t fa[2];
        fa[0] = (args + 1) ? args[1 + (0)] : NULVCL;
        fa[1] = args[0];
        char *r = NULL;
        size_t rl = 0;
        rk_rsprintf_legacy(fmt, fa, 2, 1, &r, &rl);
        return BSTRVAL(r, rl);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_split(DESCR_t *args, int nargs, int _bop) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1 && rk_rx_pat(args[1 + (0)])) {
        RxEnv renv;
        RxProg *prog = rk_rx_compile(&renv, "", rk_rx_pat(args[1 + (0)]), 0);
        if (!prog) { return FAILDESCR; }
        int split = (_bop == 0);
        const char **tx = (const char **) rt_pvec_alloc(n + 2);
        size_t *ln = (size_t *) rt_wsb_alloc((n + 2) * sizeof(size_t));
        size_t seg = 0, pos = 0;
        int nel = 0;
        while (pos <= n) {
            RxMatch rm;
            if (!rx_exec(prog, s, (int) n, (int) pos, 0, &rm)) break;
            size_t ms = (size_t) rm.from, me = (size_t) rm.to;
            if (me == ms) { pos = ms + 1; continue; }
            if (split) { tx[nel] = s + seg; ln[nel] = ms - seg; seg = me; } else { tx[nel] = s + ms; ln[nel] = me - ms; }
            nel++;
            pos = me;
        }
        if (split) { tx[nel] = s + seg; ln[nel] = n - seg; nel++; }
        return rk_arr_of_texts(tx, ln, nel);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_comb(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { const DESCR_t _r = bta_split(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; }
    if ((nargs - 1) == 1 && IS_INT_fn(args[1 + (0)]) && args[1 + (0)].i > 0) {
        long cn = (long) args[1 + (0)].i;
        const char **tx = (const char **) rt_pvec_alloc(n + 1);
        size_t *ln = (size_t *) rt_wsb_alloc((n + 1) * sizeof(size_t));
        int c = 0;
        for (size_t i = 0; i < n; ) {
            size_t b = i;
            for (long k = 0; k < cn && i < n; k++) { int cl = utf8_seqlen((unsigned char) s[i]); for (int q = 0; q < cl && i < n; q++) i++; }
            tx[c] = s + b;
            ln[c] = i - b;
            c++;
        }
        return rk_arr_of_texts(tx, ln, c);
    }
    {
        const char **tx = (const char **) rt_pvec_alloc(n + 1);
        size_t *ln = (size_t *) rt_wsb_alloc((n + 1) * sizeof(size_t));
        int c = 0;
        for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); size_t b = i; for (int k = 0; k < cl && i < n; k++) i++; tx[c] = s + b; ln[c] = i - b; c++; }
        return rk_arr_of_texts(tx, ln, c);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_split(DESCR_t *args, int nargs) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    { const DESCR_t _r = bta_split(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; }
    if ((nargs - 1) >= 1) {
        char sept[64];
        const char *sep = to_cstring(args[1 + (0)], sept, sizeof sept);
        if (!sep) sep = "";
        size_t sl = strlen(sep);
        const char **tx = (const char **) rt_pvec_alloc(n + 2);
        size_t *ln = (size_t *) rt_wsb_alloc((n + 2) * sizeof(size_t));
        int c = 0;
        if (sl == 0) {
            for (size_t i = 0; i < n; ) { int cl = utf8_seqlen((unsigned char)s[i]); size_t b = i; for (int k = 0; k < cl && i < n; k++) i++; tx[c] = s + b; ln[c] = i - b; c++; }
        } else {
            const char *p = s, *hit;
            while ((hit = strstr(p, sep)) != NULL) { tx[c] = p; ln[c] = (size_t)(hit - p); c++; p = hit + sl; }
            tx[c] = p;
            ln[c] = strlen(p);
            c++;
        }
        return rk_arr_of_texts(tx, ln, c);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_index(DESCR_t *args, int nargs, int _bop) {
    char sb[64];
    const char *s = args[0].v == DT_A ? rk_av_joined(rk_av(args[0]), " ", "", "") : IS_REAL_fn(args[0]) ? rk_real_str(args[0].r, sb, (int)sizeof sb) : to_cstring(args[0], sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    if ((nargs - 1) >= 1) {
        char nb[64];
        const char *nd = to_cstring(args[1 + (0)], nb, sizeof nb);
        if (!nd) nd = "";
        int rev = (_bop == 1);
        size_t nl = strlen(nd);
        long nch = (long)utf8_strlen(s);
        long pos = (nargs - 1) >= 2 ? (IS_INT_fn(args[1 + (1)]) ? (long)args[1 + (1)].i : IS_REAL_fn(args[1 + (1)]) ? (long)args[1 + (1)].r : atol(to_cstring(args[1 + (1)], sb, sizeof sb))) :
            (rev ? nch : 0);
        long found = -1;
        size_t b = 0;
        for (long c = 0; c <= nch; c++) {
            if (b + nl <= n && memcmp(s + b, nd, nl) == 0) { if (rev) { if (c <= pos) found = c; } else if (c >= pos) { found = c; break; } }
            if (b >= n) break;
            b++;
            while (b < n && ((unsigned char)s[b] & 0xC0) == 0x80) b++;
        }
        return found >= 0 ? INTVAL(found) : NULVCL;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_m_rindex(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_index(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_m_index(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_index(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_arr_hole(DESCR_t e) { return e.v == DT_SNUL || (rk_typeobj_name(e) && !strcmp(rk_typeobj_name(e), "Any")); }
DESCR_t bt___rk_unany(DESCR_t *args, int nargs) { if (nargs == 1) { return rk_typeobj_name(args[0]) ? NULVCL : args[0]; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_unset(DESCR_t *args, int nargs) {
    if (nargs == 1) { DESCR_t a = args[0]; int u = a.v == DT_SNUL || rk_typeobj_name(a) || (IS_STR_fn(a) && (!a.s || !a.s[0])); return (DESCR_t){ .v = DT_BOOL, .i = u }; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_lines(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) {
        extern void fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL;
        int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *s = VARVAL_fn(args[0]);
            size_t n = s ? strlen(s) : 0;
            return rk_lines_arr(s ? s : "", n);
        }
        if (!fp) { return rk_mk_arr(NULL, 0); }
        ARBLK_t *la = array_new(0, -1);
        char *line = (char *)0;
        size_t lcap = 0;
        for (;;) {
            size_t ll = 0;
            int ch, any = 0;
            while ((ch = fgetc(fp)) != EOF) { any = 1; if (!pl_rb_room(&line, &lcap, ll + 2)) break; line[ll++] = (char)ch; if (ch == '\n') break; }
            if (!any || !line) break;
            while (ll > 0 && (line[ll-1] == '\n' || line[ll-1] == '\r')) ll--;
            line[ll] = '\0';
            rk_arr_append(la, rk_txt_elem(line, ll));
        }
        if (need_close) fclose(fp);
        { DESCR_t d = {0}; d.v = DT_A; d.slen = 0; d.arr = la; *out = rk_mark_list(d); }
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_re_trans(DESCR_t *args, int nargs) {
    if (nargs == 4) {
        char sb[64], pb[512], rb[512];
        const char *sj = to_cstring(args[0], sb, sizeof sb), *se = to_cstring(args[1], pb, sizeof pb), *rp = to_cstring(args[2], rb, sizeof rb);
        return rk_trans_run(sj ? sj : "", se ? se : "", rp ? rp : "", args[3].i);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
