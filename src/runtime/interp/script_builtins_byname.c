#include "script_builtins.h"
#include "core.h"
#include "../../driver/interp_private.h"
#include "../../frontend/raku/raku_re.h"
#include "gen_runtime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gc/gc.h>
#define SOH '\x01'
static char *itos(long long v, char *buf, size_t cap) {
    if (cap < 2) { buf[0] = '\0'; return buf; }
    if (v == 0) { buf[0] = '0'; buf[1] = '\0'; return buf; }
    char tmp[24]; int n = 0; int neg = (v < 0);
    unsigned long long u = neg ? (unsigned long long)(-(v + 1)) + 1ULL : (unsigned long long)v;
    while (u) { tmp[n++] = (char)('0' + (int)(u % 10)); u /= 10; }
    if (neg) tmp[n++] = '-';
    size_t len = (size_t)n; if (len >= cap) len = cap - 1;
    for (size_t i = 0; i < len; i++) buf[i] = tmp[len - 1 - i];
    buf[len] = '\0'; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *rtos(double r, char *buf, size_t cap) {
    gcvt(r, 14, buf); (void)cap; return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *to_cstring(DESCR_t v, char *scratch, size_t scap) {
    if (IS_INT_fn(v))  { return itos((long long)v.i, scratch, scap); }
    if (IS_REAL_fn(v)) { return rtos(v.r, scratch, scap); }
    const char *s = VARVAL_fn(v); return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t elem_to_descr(const char *s, size_t slen) {
    char *buf = GC_malloc(slen + 1);
    memcpy(buf, s, slen); buf[slen] = '\0';
    char *ep; long iv = strtol(buf, &ep, 10);
    if (*ep == '\0' && ep > buf) return INTVAL(iv);
    return STRVAL(buf);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_is(DESCR_t v) {
    return (IS_STR_fn(v) || v.v == DT_SNUL) && v.s && v.s[0] == '\x03';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_num(double a, double b, int op) {
    switch (op) { case TT_EQ: return a == b; case TT_NE: return a != b; case TT_LT: return a < b;
                  case TT_LE: return a <= b; case TT_GT: return a > b; case TT_GE: return a >= b;
                  default: return a == b; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int jct_one_cmp_str(const char *a, const char *b, int op) {
    int c = strcmp(a, b);
    switch (op) { case TT_EQ: return c == 0; case TT_NE: return c != 0; case TT_LT: return c < 0;
                  case TT_LE: return c <= 0; case TT_GT: return c > 0; case TT_GE: return c >= 0;
                  default: return c == 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric) {
    const char *s = jct.s; if (!s || s[0] != '\x03' || !s[1]) return 0;
    char flav = s[1];
    double sx = numeric ? to_real(scalar) : 0.0;
    const char *sstr = numeric ? NULL : VARVAL_fn(scalar); if (!sstr) sstr = "";
    int total = 0, hits = 0;
    const char *p = s + 2;
    while (*p == SOH) {
        p++; const char *start = p;
        int hit;
        if (*p == '\x03') {
            int depth = 1; p++;
            while (*p && depth > 0) { if (*p == '\x03') depth++; else if (*p == '\x04') depth--; p++; }
            size_t L = (size_t)(p - start);
            char *mb = GC_malloc(L + 1); memcpy(mb, start, L); mb[L] = '\0';
            hit = junction_collapse(scalar, STRVAL(mb), op, numeric);
        } else {
            while (*p && *p != SOH && *p != '\x04') p++;
            size_t L = (size_t)(p - start);
            char mb[64]; size_t cl = L < 63 ? L : 63; memcpy(mb, start, cl); mb[cl] = '\0';
            hit = numeric ? jct_one_cmp_num(sx, strtod(mb, NULL), op) : jct_one_cmp_str(sstr, mb, op);
        }
        total++; if (hit) hits++;
    }
    switch (flav) { case 'a': return hits >= 1; case 'l': return total > 0 && hits == total;
                    case 'o': return hits == 1; case 'n': return hits == 0; default: return 0; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define RK_GRAM_MAX 128
static struct { const char *qname; const char *body; int flavor; } gram_reg[RK_GRAM_MAX];
static int gram_n = 0;
static void gram_set(const char *qname, const char *body, int flavor) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) { gram_reg[i].body = GC_strdup(body); gram_reg[i].flavor = flavor; return; }
    if (gram_n < RK_GRAM_MAX) { gram_reg[gram_n].qname = GC_strdup(qname); gram_reg[gram_n].body = GC_strdup(body); gram_reg[gram_n].flavor = flavor; gram_n++; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int gram_get_flavor(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].flavor;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *gram_get(const char *qname) {
    for (int i = 0; i < gram_n; i++) if (!strcmp(gram_reg[i].qname, qname)) return gram_reg[i].body;
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void gram_expand(const char *gname, const char *body, int flavor, char *out, int outsz, int depth) {
    int op = 0, n = (int)strlen(body);
    for (int i = 0; i < n && op < outsz - 1; ) {
        char c = body[i];
        if (c == '\\' && i + 1 < n) { out[op++] = c; if (op < outsz - 1) out[op++] = body[i + 1]; i += 2; continue; }
        if (c == '\'' || c == '"') {
            char q = c; out[op++] = c; i++;
            while (i < n && body[i] != q && op < outsz - 1) { if (body[i] == '\\' && i + 1 < n && op < outsz - 2) { out[op++] = body[i]; out[op++] = body[i+1]; i += 2; } else out[op++] = body[i++]; }
            if (i < n && op < outsz - 1) out[op++] = body[i++];
            continue;
        }
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            while (i < n && (body[i]==' '||body[i]=='\t'||body[i]=='\n'||body[i]=='\r')) i++;
            if (flavor == 1 && op < outsz - 3) { out[op++]='\\'; out[op++]='s'; out[op++]='*'; }
            continue;
        }
        if (c == '<' && i + 1 < n) {
            int ns = i + 1;
            if (body[ns] == '.' && ns + 1 < n && isalpha((unsigned char)body[ns + 1])) ns++;
            if (isalpha((unsigned char)body[ns])) {
                int j = ns; char nm[64]; int nl = 0;
                while (j < n && (isalnum((unsigned char)body[j]) || body[j] == '_') && nl < 63) nm[nl++] = body[j++];
                nm[nl] = '\0';
                if (j < n && body[j] == '>' && !(j + 1 < n && body[j + 1] == '(')) {
                    const char *sub = NULL; int subfl = 0;
                    if (depth < 16) { char qn[256]; snprintf(qn, sizeof qn, "%s::%s", gname, nm); sub = gram_get(qn); subfl = gram_get_flavor(qn); }
                    if (sub) {
                        char tmp[4096]; gram_expand(gname, sub, subfl, tmp, sizeof tmp, depth + 1);
                        if (op < outsz - 1) out[op++] = '(';
                        for (int k = 0; tmp[k] && op < outsz - 1; k++) out[op++] = tmp[k];
                        if (op < outsz - 1) out[op++] = ')';
                        i = j + 1; continue;
                    }
                }
            }
        }
        out[op++] = c; i++;
    }
    out[op] = '\0';
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn) return 0;
    if (nargs >= 1 && (!strcmp(fn, "__rk_jct_any") || !strcmp(fn, "__rk_jct_all")
                    || !strcmp(fn, "__rk_jct_one") || !strcmp(fn, "__rk_jct_none"))) {
        char flav = !strcmp(fn, "__rk_jct_any") ? 'a'
                  : !strcmp(fn, "__rk_jct_all") ? 'l'
                  : !strcmp(fn, "__rk_jct_one") ? 'o' : 'n';
        size_t total = 2;
        char **ms = GC_malloc((size_t)nargs * sizeof(char *));
        size_t *mlen = GC_malloc((size_t)nargs * sizeof(size_t));
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            size_t L = strlen(cs);
            char *cp = GC_malloc(L + 1); memcpy(cp, cs, L + 1);
            ms[i] = cp; mlen[i] = L; total += 1 + L;
        }
        char *buf = GC_malloc(total + 2);
        size_t p = 0; buf[p++] = '\x03'; buf[p++] = flav;
        for (int i = 0; i < nargs; i++) { buf[p++] = SOH; memcpy(buf + p, ms[i], mlen[i]); p += mlen[i]; }
        buf[p++] = '\x04'; buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "elems") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = INTVAL(0); return 1; }
        int cnt = 1; for (const char *p = cur; *p; p++) if (*p == SOH) cnt++;
        *out = INTVAL(cnt); return 1;
    }
    if (!strcmp(fn, "__rk_arr") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "reverse") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = nel - 1; i >= 0; i--) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if ((!strcmp(fn, "head") || !strcmp(fn, "tail")) && nargs >= 1) {
        int is_tail = (fn[0] == 't');
        long long n = 1; int listargs = nargs;
        if (nargs >= 2) {
            char nb[64]; const char *ns = to_cstring(args[nargs - 1], nb, sizeof nb);
            char *ep; long long v = strtoll(ns ? ns : "", &ep, 10);
            if (ns && ep > ns) n = v;
            listargs = nargs - 1;
        }
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < listargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                if (nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        if (n < 0) n = 0;
        int lo, hi;
        if (is_tail) { lo = (int)(nel - n); if (lo < 0) lo = 0; hi = nel; }
        else         { lo = 0; hi = (int)(n < nel ? n : nel); }
        size_t total = 0; for (int i = lo; i < hi; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = lo; i < hi; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "unique") && nargs >= 1) {
        const char **els = GC_malloc((size_t)nargs * 64 * sizeof(const char *));
        size_t *lens = GC_malloc((size_t)nargs * 64 * sizeof(size_t));
        int nel = 0, cap = nargs * 64;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                int dup = 0;
                for (int j = 0; j < nel; j++) if (lens[j] == L && memcmp(els[j], seg, L) == 0) { dup = 1; break; }
                if (!dup && nel < cap) { char *cp = GC_malloc(L + 1); memcpy(cp, seg, L); cp[L] = '\0'; els[nel] = cp; lens[nel] = L; nel++; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        size_t total = 0; for (int i = 0; i < nel; i++) total += lens[i] + 1;
        char *buf = GC_malloc(total + 1); size_t p = 0;
        for (int i = 0; i < nel; i++) { if (p > 0) buf[p++] = SOH; memcpy(buf + p, els[i], lens[i]); p += lens[i]; }
        buf[p] = '\0';
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "sum") && nargs >= 1) {
        long long isum = 0; double rsum = 0.0; int any_real = 0;
        for (int i = 0; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                char eb[64]; size_t cl = L < 63 ? L : 63; memcpy(eb, seg, cl); eb[cl] = '\0';
                char *ep; long long iv = strtoll(eb, &ep, 10);
                if (*ep == '\0' && ep > eb) { isum += iv; rsum += (double)iv; }
                else { double dv = strtod(eb, NULL); any_real = 1; rsum += dv; }
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = any_real ? REALVAL(rsum) : INTVAL(isum); return 1;
    }
    if (!strcmp(fn, "join") && nargs >= 1) {
        char sb[64]; const char *sep = to_cstring(args[0], sb, sizeof sb);
        char *sepd = GC_strdup(sep ? sep : ""); size_t seplen = strlen(sepd);
        char *buf = GC_strdup(""); size_t blen = 0; int first = 1;
        for (int i = 1; i < nargs; i++) {
            char scratch[64];
            const char *cs = to_cstring(args[i], scratch, sizeof scratch);
            const char *seg = cs;
            for (;;) {
                const char *nx = strchr(seg, SOH);
                size_t L = nx ? (size_t)(nx - seg) : strlen(seg);
                size_t add = (first ? 0 : seplen) + L;
                char *no = GC_malloc(blen + add + 1);
                memcpy(no, buf, blen); size_t p = blen;
                if (!first) { memcpy(no + p, sepd, seplen); p += seplen; }
                memcpy(no + p, seg, L); p += L; no[p] = '\0';
                buf = no; blen = p; first = 0;
                if (!nx) break;
                seg = nx + 1;
            }
        }
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "arr_get") && nargs == 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (idx < 0 || !*cur) { *out = FAILDESCR; return 1; }
        const char *seg = cur;
        long k = 0;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) {
                size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
                *out = elem_to_descr(seg, elen);
                return 1;
            }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
    }
    if (!strcmp(fn, "str_substr") || (!strcmp(fn, "substr") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        long slen = (long)strlen(s);
        long start = IS_INT_fn(args[1]) ? args[1].i : 0;
        if (start < 0) start = slen + start;
        if (start < 0) start = 0;
        if (start > slen) start = slen;
        long len = slen - start;
        if (nargs >= 3) {
            long ll = IS_INT_fn(args[2]) ? args[2].i : len;
            if (ll < 0) ll = 0;
            if (start + ll > slen) ll = slen - start;
            len = ll;
        }
        char *o = GC_malloc((size_t)len + 1);
        memcpy(o, s + start, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "str_index") || (!strcmp(fn, "index") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : 0;
        if (from < 0) from = 0;
        if (*n == '\0') { *out = INTVAL(from); return 1; }
        const char *p = strstr(s + from, n);
        *out = p ? INTVAL((long)(p - s)) : INTVAL(-1); return 1;
    }
    if (!strcmp(fn, "str_rindex") || (!strcmp(fn, "rindex") && nargs >= 2)) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        const char *n = VARVAL_fn(args[1]); if (!n) n = "";
        long slen = (long)strlen(s);
        long from = (nargs >= 3 && IS_INT_fn(args[2])) ? args[2].i : slen;
        size_t nlen = strlen(n);
        if (nlen == 0) { *out = INTVAL(from < slen ? from : slen); return 1; }
        long best = -1;
        for (long i = 0; i <= slen - (long)nlen && i <= from; i++)
            if (memcmp(s + i, n, nlen) == 0) best = i;
        *out = INTVAL(best); return 1;
    }
    if ((!strcmp(fn, "uc") || !strcmp(fn, "str_uc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = GC_malloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'a' && s[i] <= 'z') ? s[i] - 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "lc") || !strcmp(fn, "str_lc")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        size_t n = strlen(s); char *o = GC_malloc(n + 1);
        for (size_t i = 0; i < n; i++) o[i] = (char)((s[i] >= 'A' && s[i] <= 'Z') ? s[i] + 32 : s[i]);
        o[n] = '\0'; *out = STRVAL(o); return 1;
    }
    if ((!strcmp(fn, "chars") || !strcmp(fn, "length")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        *out = INTVAL((long)strlen(s)); return 1;
    }
    if ((!strcmp(fn, "str_trim") || !strcmp(fn, "trim")) && nargs == 1) {
        const char *s = VARVAL_fn(args[0]); if (!s) s = "";
        while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
        size_t n = strlen(s);
        while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\n' || s[n-1] == '\r')) n--;
        char *o = GC_malloc(n + 1); memcpy(o, s, n); o[n] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "array_sort") && nargs == 1) {
        const char *as = VARVAL_fn(args[0]); if (!as || !*as) { *out = STRVAL(GC_strdup("")); return 1; }
        int cnt = 1; for (const char *p = as; *p; p++) if (*p == SOH) cnt++;
        char **elems = GC_malloc((size_t)cnt * sizeof(char*));
        int idx = 0; const char *seg = as;
        do {
            const char *nx = strchr(seg, SOH);
            size_t elen = nx ? (size_t)(nx - seg) : strlen(seg);
            char *el = GC_malloc(elen + 1); memcpy(el, seg, elen); el[elen] = '\0';
            elems[idx++] = el;
            seg = nx ? nx + 1 : NULL;
        } while (seg && idx < cnt);
        int all_int = 1;
        for (int i = 0; i < cnt && all_int; i++) {
            char *ep; strtol(elems[i], &ep, 10);
            if (*ep) all_int = 0;
        }
        if (all_int) {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; long kv = atol(key); int j = i - 1;
                while (j >= 0 && atol(elems[j]) > kv) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        } else {
            for (int i = 1; i < cnt; i++) {
                char *key = elems[i]; int j = i - 1;
                while (j >= 0 && strcmp(elems[j], key) > 0) { elems[j+1] = elems[j]; j--; }
                elems[j+1] = key;
            }
        }
        size_t total = 0; for (int i = 0; i < cnt; i++) total += strlen(elems[i]) + 1;
        char *o = GC_malloc(total + 1); o[0] = '\0';
        for (int i = 0; i < cnt; i++) {
            if (i) { size_t ol = strlen(o); o[ol] = SOH; o[ol+1] = '\0'; }
            strcat(o, elems[i]);
        }
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "script_die") && nargs >= 1) {
        const char *m = VARVAL_fn(args[0]); if (!m) m = "";
        extern char g_script_exception[512];
        size_t mlen = strlen(m); if (mlen > 511) mlen = 511;
        memcpy(g_script_exception, m, mlen); g_script_exception[mlen] = '\0';
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_clear") && nargs == 0) {
        extern char g_script_exception[512];
        g_script_exception[0] = '\0';
        *out = STRVAL(GC_strdup("")); return 1;
    }
    if (!strcmp(fn, "exc_check") && nargs == 0) {
        extern char g_script_exception[512];
        if (g_script_exception[0] != '\0') { *out = INTVAL(1); return 1; }
        *out = FAILDESCR; return 1;
    }
    if (!strcmp(fn, "exc_get") && nargs == 0) {
        extern char g_script_exception[512];
        *out = STRVAL(GC_strdup(g_script_exception)); return 1;
    }
    if (!strcmp(fn, "fh_capture") && nargs == 1) {
        extern void fh_ensure_init(void);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        *out = FHVAL(idx); return 1;
    }
    if (!strcmp(fn, "open") && (nargs == 1 || nargs == 2)) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *mode = "r";
        if (nargs == 2) {
            const char *ms = VARVAL_fn(args[1]); if (!ms) ms = "";
            if (strstr(ms, ":w") || strstr(ms, "w")) mode = "w";
            else if (strstr(ms, ":a") || strstr(ms, "a")) mode = "a";
        }
        extern void fh_ensure_init(void);
        extern int fh_alloc(FILE *);
        fh_ensure_init();
        FILE *fp = fopen(path, mode);
        if (!fp) { *out = FAILDESCR; return 1; }
        int idx = fh_alloc(fp);
        if (idx < 0) { fclose(fp); *out = FAILDESCR; return 1; }
        *out = INTVAL(idx); return 1;
    }
    if (!strcmp(fn, "close") && nargs == 1) {
        extern FILE *fh_get(int);
        extern void  fh_free(int);
        int idx = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        FILE *fp = fh_get(idx);
        if (fp) { fclose(fp); fh_free(idx); }
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "slurp") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(GC_strdup("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(GC_strdup("")); return 1; }
        fseek(fp, 0, SEEK_END); long sz = ftell(fp); rewind(fp);
        char *buf = GC_malloc(sz + 1);
        size_t nr = fread(buf, 1, (size_t)sz, fp); buf[nr] = '\0';
        if (need_close) fclose(fp);
        *out = STRVAL(buf); return 1;
    }
    if (!strcmp(fn, "lines") && nargs == 1) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        FILE *fp = NULL; int need_close = 0;
        if (IS_INT_fn(args[0]) || IS_FH_fn(args[0])) {
            fh_ensure_init();
            fp = fh_get((int)args[0].i);
        } else {
            const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = STRVAL(GC_strdup("")); return 1; }
            fp = fopen(path, "r"); need_close = 1;
        }
        if (!fp) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_malloc(65536); acc[0] = '\0'; size_t cap = 65536, used = 0; int first = 1;
        char line[4096];
        while (fgets(line, sizeof line, fp)) {
            size_t ll = strlen(line);
            while (ll > 0 && (line[ll-1] == '\n' || line[ll-1] == '\r')) line[--ll] = '\0';
            size_t need = used + ll + 2;
            if (need > cap) { cap = need * 2; char *nb = GC_malloc(cap); memcpy(nb, acc, used); acc = nb; }
            if (!first) { acc[used++] = '\x01'; }
            memcpy(acc + used, line, ll); used += ll; acc[used] = '\0'; first = 0;
        }
        if (need_close) fclose(fp);
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "spurt") && nargs == 2) {
        const char *path = VARVAL_fn(args[0]); if (!path || !*path) { *out = FAILDESCR; return 1; }
        const char *content = VARVAL_fn(args[1]); if (!content) content = "";
        FILE *fp = fopen(path, "w"); if (!fp) { *out = FAILDESCR; return 1; }
        fputs(content, fp); fclose(fp);
        *out = INTVAL(0); return 1;
    }
    if ((!strcmp(fn, "print_fh") || !strcmp(fn, "say_fh")) && nargs == 2) {
        extern void  fh_ensure_init(void);
        extern FILE *fh_get(int);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) || IS_FH_fn(args[0]) ? args[0].i : 1);
        FILE *fp = fh_get(idx); if (!fp) fp = stdout;
        if (fp != stdout) fflush(stdout);
        char scratch[64]; const char *s = to_cstring(args[1], scratch, sizeof scratch);
        fputs(s, fp);
        if (!strcmp(fn, "say_fh")) fputc('\n', fp);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "obj_new") && nargs >= 1) {
        const char *cname = VARVAL_fn(args[0]); if (!cname || !*cname) { *out = FAILDESCR; return 1; }
        DatType *dt = dat_find_type(cname); if (!dt) { *out = FAILDESCR; return 1; }
        DESCR_t fvals[64];
        for (int fi = 0; fi < dt->nfields && fi < 64; fi++) fvals[fi] = NULVCL;
        for (int ci = 1; ci + 1 < nargs; ci += 2) {
            const char *kname = VARVAL_fn(args[ci]); if (!kname) continue;
            for (int fi = 0; fi < dt->nfields && fi < 64; fi++) {
                if (strcmp(dt->fields[fi], kname) == 0) { fvals[fi] = args[ci + 1]; break; }
            }
        }
        *out = dat_construct(dt, fvals, dt->nfields); return 1;
    }
    if (!strcmp(fn, "meth_call") && nargs >= 2) {
        if (args[0].v != DT_DATA || !args[0].u) { *out = FAILDESCR; return 1; }
        DATINST_t *inst = (DATINST_t *)args[0].u;
        const char *cname = (inst && inst->type) ? inst->type->name : NULL;
        if (!cname) { *out = FAILDESCR; return 1; }
        const char *mname = VARVAL_fn(args[1]); if (!mname || !*mname) { *out = FAILDESCR; return 1; }
        char procname[256]; int plen = 0;
        for (; cname[plen] && plen < 127; plen++) procname[plen] = cname[plen];
        procname[plen++] = '_'; procname[plen++] = '_';
        for (int k = 0; mname[k] && plen < 255; k++) procname[plen++] = mname[k];
        procname[plen] = '\0';
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++)
            if (g_stage2.proc_table[pi].name && strcmp(g_stage2.proc_table[pi].name, procname) == 0) break;
        if (pi >= g_stage2.proc_count) { *out = FAILDESCR; return 1; }
        int nextra = nargs - 2;
        int total = 1 + nextra;
        DESCR_t *callargs = GC_malloc((size_t)total * sizeof(DESCR_t));
        callargs[0] = args[0];
        for (int k = 0; k < nextra; k++) callargs[1 + k] = args[2 + k];
        *out = proc_table_call(pi, callargs, total); return 1;
    }
    if (!strcmp(fn, "re_match") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Raku_nfa *nfa = raku_nfa_build(pat);
        if (!nfa) { *out = FAILDESCR; return 1; }
        raku_nfa_exec(nfa, subj, &g_raku_match);
        g_raku_subject = subj;
        int verdict = g_raku_match.matched ? 1 : 0;
        static int nfa_bb = -1;
        if (nfa_bb < 0) { const char *e = getenv("RK_NFA_BB"); nfa_bb = (e && e[0] == '1') ? 1 : 0; }
        if (nfa_bb) { extern int raku_nfa_bb_match(const Raku_nfa *, const char *); verdict = raku_nfa_bb_match(nfa, subj); }
        raku_nfa_free(nfa);
        *out = verdict ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "nfa_accepts") && nargs == 2) {
        DESCR_t topic = args[0], matcher = args[1];
        if (junction_is(matcher)) {
            int numeric = (IS_INT_fn(topic) || IS_REAL_fn(topic));
            int ok = junction_collapse(topic, matcher, TT_EQ, numeric);
            *out = ok ? INTVAL(1) : FAILDESCR; return 1;
        }
        int both_num = (IS_INT_fn(topic) || IS_REAL_fn(topic)) && (IS_INT_fn(matcher) || IS_REAL_fn(matcher));
        int ok;
        if (both_num) {
            ok = (to_real(topic) == to_real(matcher));
        } else {
            const char *ts = VARVAL_fn(topic); if (!ts) ts = "";
            const char *ms = VARVAL_fn(matcher); if (!ms) ms = "";
            ok = (strcmp(ts, ms) == 0);
        }
        *out = ok ? INTVAL(1) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "grammar_register") && nargs == 3) {
        const char *qn   = VARVAL_fn(args[0]);
        const char *body = VARVAL_fn(args[1]);
        const char *flv  = VARVAL_fn(args[2]);
        if (qn && body) gram_set(qn, body, flv ? atoi(flv) : 0);
        *out = INTVAL(1); return 1;
    }
    if (!strcmp(fn, "grammar_parse") && nargs == 2) {
        const char *gname = VARVAL_fn(args[0]); if (!gname) gname = "";
        const char *subj  = VARVAL_fn(args[1]); if (!subj)  subj  = "";
        char qn[256]; snprintf(qn, sizeof qn, "%s::TOP", gname);
        const char *body = gram_get(qn);
        if (!body) { *out = FAILDESCR; return 1; }
        int topflv = gram_get_flavor(qn);
        char pat[4096]; gram_expand(gname, body, topflv, pat, sizeof pat, 0);
        Raku_nfa *nfa = raku_nfa_build(pat);
        if (!nfa) { *out = FAILDESCR; return 1; }
        Raku_match m; raku_nfa_exec(nfa, subj, &m);
        raku_nfa_free(nfa);
        int slen = (int)strlen(subj);
        int ok = m.matched && m.full_start == 0 && m.full_end == slen;
        *out = ok ? STRVAL(GC_strdup(subj)) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "re_match_global") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *pat  = VARVAL_fn(args[1]); if (!pat)  pat  = "";
        Raku_nfa *nfa = raku_nfa_build(pat);
        if (!nfa) { *out = STRVAL(GC_strdup("")); return 1; }
        int slen = (int)strlen(subj);
        char *acc = GC_malloc((size_t)slen * 4 + 4); acc[0] = '\0';
        int pos = 0, count = 0;
        while (pos <= slen) {
            Raku_match m; raku_nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) break;
            int mlen = m.full_end - m.full_start;
            if (count > 0) { int ol = (int)strlen(acc); acc[ol] = SOH; acc[ol + 1] = '\0'; }
            strncat(acc, subj + pos + m.full_start, (size_t)mlen);
            g_raku_match = m;
            g_raku_match.full_start += pos;
            g_raku_match.full_end   += pos;
            for (int g = 0; g < m.ngroups; g++) {
                if (m.group_start[g] >= 0) g_raku_match.group_start[g] += pos;
                if (m.group_end[g]   >= 0) g_raku_match.group_end[g]   += pos;
            }
            g_raku_subject = subj;
            pos += m.full_start + (mlen > 0 ? mlen : 1);
            count++;
        }
        raku_nfa_free(nfa);
        *out = count > 0 ? STRVAL(acc) : FAILDESCR; return 1;
    }
    if (!strcmp(fn, "re_subst") && nargs == 2) {
        const char *subj = VARVAL_fn(args[0]); if (!subj) subj = "";
        const char *tok  = VARVAL_fn(args[1]); if (!tok)  tok  = "";
        const char *sep1 = strchr(tok, SOH);
        if (!sep1) { *out = args[0]; return 1; }
        const char *sep2 = strchr(sep1 + 1, SOH);
        if (!sep2) { *out = args[0]; return 1; }
        int plen = (int)(sep1 - tok);
        int rlen = (int)(sep2 - (sep1 + 1));
        char *pat  = GC_malloc((size_t)plen + 1); memcpy(pat, tok, (size_t)plen); pat[plen] = '\0';
        char *repl = GC_malloc((size_t)rlen + 1); memcpy(repl, sep1 + 1, (size_t)rlen); repl[rlen] = '\0';
        int global = (*(sep2 + 1) == 'g');
        Raku_nfa *nfa = raku_nfa_build(pat);
        if (!nfa) { *out = args[0]; return 1; }
        int slen = (int)strlen(subj);
        char *res = GC_malloc((size_t)slen * 4 + (size_t)rlen * 8 + 4); res[0] = '\0';
        int pos = 0, did_one = 0;
        while (pos <= slen) {
            Raku_match m; raku_nfa_exec(nfa, subj + pos, &m);
            if (!m.matched) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
            strncat(res, subj + pos, (size_t)m.full_start);
            strcat(res, repl);
            g_raku_match = m; g_raku_subject = subj;
            int advance = m.full_start + (m.full_end - m.full_start > 0 ? m.full_end - m.full_start : 1);
            pos += advance; did_one = 1;
            if (!global) { strncat(res, subj + pos, (size_t)(slen - pos)); break; }
        }
        raku_nfa_free(nfa);
        *out = did_one ? STRVAL(res) : args[0]; return 1;
    }
    if (!strcmp(fn, "raku_nfa_compile") && nargs == 1) {
        const char *pat = VARVAL_fn(args[0]); if (!pat) pat = "";
        Raku_nfa *nfa = raku_nfa_build(pat);
        if (!nfa) { printf("NFA:%s:ERROR\n", pat); *out = INTVAL(0); return 1; }
        printf("NFA:%s:states=%d\n", pat, raku_nfa_state_count(nfa));
        raku_nfa_free(nfa);
        *out = INTVAL(0); return 1;
    }
    if (!strcmp(fn, "re_capture") && nargs == 1) {
        int n = (int)(IS_INT_fn(args[0]) ? args[0].i : 0);
        if (!g_raku_match.matched || n < 0 || n >= g_raku_match.ngroups || g_raku_match.group_start[n] < 0) { *out = STRVAL(GC_strdup("")); return 1; }
        int gs = g_raku_match.group_start[n], ge = g_raku_match.group_end[n];
        if (ge < gs) { *out = STRVAL(GC_strdup("")); return 1; }
        int len = ge - gs; char *o = GC_malloc((size_t)len + 1);
        memcpy(o, g_raku_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "re_named_capture") && nargs == 1) {
        const char *name = VARVAL_fn(args[0]); if (!name) name = "";
        if (!g_raku_match.matched) { *out = STRVAL(GC_strdup("")); return 1; }
        int g = -1;
        for (int i = 0; i < g_raku_match.ngroups; i++) if (strcmp(g_raku_match.group_name[i], name) == 0) { g = i; break; }
        if (g < 0 || g_raku_match.group_start[g] < 0) { *out = STRVAL(GC_strdup("")); return 1; }
        int gs = g_raku_match.group_start[g], ge = g_raku_match.group_end[g];
        if (ge < gs) { *out = STRVAL(GC_strdup("")); return 1; }
        int len = ge - gs; char *o = GC_malloc((size_t)len + 1);
        memcpy(o, g_raku_subject + gs, (size_t)len); o[len] = '\0';
        *out = STRVAL(o); return 1;
    }
    /* RK-LOWER-5b: PURE mutating-op variants. A pure variant computes and RETURNS the new container string but
       performs NO NV_SET_fn — the writeback is supplied by the enclosing IR_ASSIGN the lowerer wraps it in
       (`@a = push_pure(@a, x)`), so the container update flows through the same shared four-port store path that
       works in all three modes. Element/pair encoding matches the read-only readers: array = SOH-separated elements
       (docs.raku.org/type/Array), hash = SOH-separated key STX value pairs (docs.raku.org/type/Hash). The hash
       string builders (hash_set_str/hash_delete_str, defined below) already return a fresh string and do no
       NV_SET_fn, so the hash pure variants delegate to them directly. push_pure appends each extra argument to the
       array's end and returns the new array (docs.raku.org/routine/push); arr_set_pure replaces element [idx] and
       returns the new array (docs.raku.org/language/list `@a[$i] = $y`); arr_last/arr_init split the pop semantics
       (docs.raku.org/routine/pop: return+remove the last element) into a read of the last element and a read of the
       array minus its last element, so `$p = pop(@a)` lowers to `$p = arr_last(@a); @a = arr_init(@a)` — two pure
       reads + the existing IR_ASSIGN store, never an exec-side vname peek. */
    if (!strcmp(fn, "push_pure") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = GC_strdup(cur);
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = GC_malloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol + 1 + rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "arr_set_pure") && nargs >= 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        const char *seg = cur;
        long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { tstart = seg; tend = nx; break; }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
        size_t pre = (size_t)(tstart - cur);
        size_t post = tend ? strlen(tend) : 0;
        size_t rvl = strlen(rv);
        size_t total = pre + rvl + post;
        char *o = GC_malloc(total + 1);
        memcpy(o, cur, pre);
        memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post);
        o[total] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "arr_last") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = FAILDESCR; return 1; }
        const char *last = strrchr(cur, SOH);
        const char *pstart = last ? last + 1 : cur;
        *out = elem_to_descr(pstart, strlen(pstart)); return 1;
    }
    if (!strcmp(fn, "arr_init") && nargs == 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = STRVAL(GC_strdup("")); return 1; }
        const char *last = strrchr(cur, SOH);
        if (!last) { *out = STRVAL(GC_strdup("")); return 1; }
        size_t nl = (size_t)(last - cur);
        char *o = GC_malloc(nl + 1); memcpy(o, cur, nl); o[nl] = '\0';
        *out = STRVAL(o); return 1;
    }
    if (!strcmp(fn, "hash_set_pure") && nargs >= 3) {
        extern char *script_hash_set_str(const char *h, const char *key, const char *val);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        char vb[64]; const char *val = to_cstring(args[2], vb, sizeof vb);
        *out = STRVAL(script_hash_set_str(cur, key, val)); return 1;
    }
    if (!strcmp(fn, "hash_delete_pure") && nargs >= 2) {
        extern char *script_hash_delete_str(const char *h, const char *key);
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = STRVAL(script_hash_delete_str(cur, key)); return 1;
    }
    extern int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
    if (script_try_hash_builtin(fn, args, nargs, out)) return 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_mutating_builtin_by_name(const char *fn, const char *vname,
                                      DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || !vname) return 0;
    if (!strcmp(fn, "push") && nargs >= 2) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        char *acc = GC_strdup(cur);
        for (int i = 1; i < nargs; i++) {
            char rb[64]; const char *rv = to_cstring(args[i], rb, sizeof rb);
            size_t ol = strlen(acc), rl = strlen(rv);
            char *no = GC_malloc(ol + rl + 2);
            memcpy(no, acc, ol);
            if (ol > 0) { no[ol] = SOH; memcpy(no + ol + 1, rv, rl); no[ol+1+rl] = '\0'; }
            else        { memcpy(no, rv, rl); no[rl] = '\0'; }
            acc = no;
        }
        DESCR_t nd = STRVAL(acc);
        NV_SET_fn(vname, nd);
        *out = nd; return 1;
    }
    if (!strcmp(fn, "pop") && nargs >= 1) {
        const char *cur = VARVAL_fn(args[0]); if (!cur || !*cur) { *out = FAILDESCR; return 1; }
        const char *last = strrchr(cur, SOH);
        const char *pstart = last ? last + 1 : cur;
        size_t plen = strlen(pstart);
        char *popped = GC_malloc(plen + 1); memcpy(popped, pstart, plen); popped[plen] = '\0';
        char *newarr;
        if (last) { size_t nl = (size_t)(last - cur); newarr = GC_malloc(nl + 1); memcpy(newarr, cur, nl); newarr[nl] = '\0'; }
        else      { newarr = GC_strdup(""); }
        NV_SET_fn(vname, STRVAL(newarr));
        char *ep; long iv = strtol(popped, &ep, 10);
        if (*ep == '\0' && ep > popped) { *out = INTVAL(iv); return 1; }
        *out = STRVAL(popped); return 1;
    }
    if (!strcmp(fn, "arr_set") && nargs >= 3) {
        const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
        long idx = IS_INT_fn(args[1]) ? args[1].i : 0;
        char rb[64]; const char *rv = to_cstring(args[2], rb, sizeof rb);
        const char *seg = cur;
        long k = 0; const char *tstart = NULL; const char *tend = NULL;
        for (;;) {
            const char *nx = strchr(seg, SOH);
            if (k == idx) { tstart = seg; tend = nx; break; }
            if (!nx) { *out = FAILDESCR; return 1; }
            seg = nx + 1; k++;
        }
        size_t pre  = (size_t)(tstart - cur);
        size_t post = tend ? strlen(tend) : 0;
        size_t rvl  = strlen(rv);
        size_t total = pre + rvl + post;
        char *o = GC_malloc(total + 1);
        memcpy(o, cur, pre);
        memcpy(o + pre, rv, rvl);
        if (tend) memcpy(o + pre + rvl, tend, post);
        o[total] = '\0';
        NV_SET_fn(vname, STRVAL(o));
        *out = args[2]; return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#define STX '\x02'
static const char *hash_find(const char *h, const char *key, const char **p_pair_start) {
    if (!h || !key) return NULL;
    size_t klen = strlen(key);
    const char *seg = h;
    while (*seg) {
        const char *nx   = strchr(seg, SOH);
        size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
        const char *stx  = (const char *)memchr(seg, STX, plen);
        if (stx) {
            size_t this_klen = (size_t)(stx - seg);
            if (this_klen == klen && memcmp(seg, key, klen) == 0) {
                if (p_pair_start) *p_pair_start = seg;
                return stx + 1;
            }
        }
        if (!nx) break;
        seg = nx + 1;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_set_str(const char *h, const char *key, const char *val) {
    if (!h) h = "";
    size_t klen = strlen(key), vlen = strlen(val);
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (pair_start) {
        const char *nx = strchr(pair_start, SOH);
        size_t pre  = (size_t)(pair_start - h);
        size_t post = nx ? strlen(nx) : 0;
        size_t total = pre + klen + 1 + vlen + post;
        char *o = GC_malloc(total + 1);
        memcpy(o, h, pre);
        memcpy(o + pre, key, klen);
        o[pre + klen] = STX;
        memcpy(o + pre + klen + 1, val, vlen);
        if (nx) memcpy(o + pre + klen + 1 + vlen, nx, post);
        o[total] = '\0'; return o;
    }
    size_t hlen = strlen(h);
    int need_sep = (hlen > 0);
    size_t total = hlen + (need_sep ? 1 : 0) + klen + 1 + vlen;
    char *o = GC_malloc(total + 1);
    memcpy(o, h, hlen);
    if (need_sep) o[hlen] = SOH;
    memcpy(o + hlen + (need_sep ? 1 : 0), key, klen);
    o[hlen + (need_sep ? 1 : 0) + klen] = STX;
    memcpy(o + hlen + (need_sep ? 1 : 0) + klen + 1, val, vlen);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *script_hash_delete_str(const char *h, const char *key) {
    if (!h || !*h) return GC_strdup("");
    const char *pair_start = NULL;
    hash_find(h, key, &pair_start);
    if (!pair_start) return GC_strdup(h);
    const char *nx = strchr(pair_start, SOH);
    size_t pre = (size_t)(pair_start - h);
    size_t trim_pre = pre;
    if (pre > 0) trim_pre = pre - 1;
    else if (nx) { nx++; }
    size_t post = nx ? strlen(nx) : 0;
    size_t total = trim_pre + post;
    char *o = GC_malloc(total + 1);
    if (pre > 0) { memcpy(o, h, trim_pre); if (nx) memcpy(o + trim_pre, nx, post); }
    else if (nx) memcpy(o, nx, post);
    o[total] = '\0'; return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_hash_mutating_builtin(const char *fn, const char *vname, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || !vname) return 0;
    const char *cur = VARVAL_fn(args[0]); if (!cur) cur = "";
    if (!strcmp(fn, "hash_set") && nargs >= 3) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        char vb[64]; const char *val = to_cstring(args[2], vb, sizeof vb);
        char *nh = script_hash_set_str(cur, key, val);
        NV_SET_fn(vname, STRVAL(nh));
        *out = STRVAL(nh); return 1;
    }
    if (!strcmp(fn, "hash_delete") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        char *nh = script_hash_delete_str(cur, key);
        NV_SET_fn(vname, STRVAL(nh));
        *out = STRVAL(nh); return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_hash_builtin(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) {
    if (!fn || nargs < 1) return 0;
    if (args[0].v != DT_S && args[0].v != DT_SNUL) return 0;
    const char *h = VARVAL_fn(args[0]); if (!h) h = "";
    if (!strcmp(fn, "hash_get") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        const char *vstart = hash_find(h, key, NULL);
        if (!vstart) { *out = STRVAL(GC_strdup("")); return 1; }
        const char *vend = strchr(vstart, SOH);
        size_t vlen = vend ? (size_t)(vend - vstart) : strlen(vstart);
        char *v = GC_malloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
        char *ep; long iv = strtol(v, &ep, 10);
        if (*ep == '\0' && ep > v) { *out = INTVAL(iv); return 1; }
        *out = STRVAL(v); return 1;
    }
    if (!strcmp(fn, "hash_exists") && nargs >= 2) {
        char kb[64]; const char *key = to_cstring(args[1], kb, sizeof kb);
        *out = hash_find(h, key, NULL) ? INTVAL(1) : INTVAL(0); return 1;
    }
    if (!strcmp(fn, "hash_keys") && nargs >= 1) {
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                char *k = GC_malloc(klen + 1); memcpy(k, seg, klen); k[klen] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + klen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, k, klen); na[al + klen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_values") && nargs >= 1) {
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                char *v = GC_malloc(vlen + 1); memcpy(v, vstart, vlen); v[vlen] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + vlen + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, v, vlen); na[al + vlen] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    if (!strcmp(fn, "hash_pairs") && nargs >= 1) {
        if (!*h) { *out = STRVAL(GC_strdup("")); return 1; }
        char *acc = GC_strdup("");
        const char *seg = h;
        while (*seg) {
            const char *nx   = strchr(seg, SOH);
            size_t      plen = nx ? (size_t)(nx - seg) : strlen(seg);
            const char *stx  = (const char *)memchr(seg, STX, plen);
            if (stx) {
                size_t klen = (size_t)(stx - seg);
                const char *vstart = stx + 1;
                size_t vlen = nx ? (size_t)(nx - vstart) : strlen(vstart);
                size_t pl = klen + 1 + vlen;
                char *pair = GC_malloc(pl + 1);
                memcpy(pair, seg, klen); pair[klen] = ':';
                memcpy(pair + klen + 1, vstart, vlen); pair[pl] = '\0';
                size_t al = strlen(acc);
                char *na = GC_malloc(al + (al ? 1 : 0) + pl + 1);
                memcpy(na, acc, al);
                if (al) na[al++] = SOH;
                memcpy(na + al, pair, pl); na[al + pl] = '\0';
                acc = na;
            }
            if (!nx) break;
            seg = nx + 1;
        }
        *out = STRVAL(acc); return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#undef STX
#undef SOH
