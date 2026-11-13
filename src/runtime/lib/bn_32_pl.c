#include "runtime/lib/bn_head.h"
#include "runtime/core/g_runtime_misc.h"
#include "runtime/by_name_dispatch.h"
#include <math.h>
#include "runtime/rt/prolog_atom.h"
#include <errno.h>
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_32_pl.h"
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern int rt_pl_db_count(void *);
extern int rt_pl_db_clause_at(void *, int, void *);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
extern void *rt_pl_db_get_by_key(void *, const char *, int);
extern int rt_pl_db_term_key(void *, char *, size_t, int *);
char *pl_atom_carve_cx(size_t n, pl_tr_ctx_t *cx) {
    extern void *rt_pl_cap_error(void);
    extern int rt_gcheap_cap_refused(void);
    char *b = prolog_atom_carve(n);
    if (!b && cx && !cx->ball) cx->ball = rt_pl_cap_error();
    if (!b && !cx) (void)rt_gcheap_cap_refused();
    return b;
}
DESCR_t pl_mk_atom_carved(char *b, size_t n, size_t k) { DESCR_t d = {0}; d.v = DT_PLATOM; d.slen = 0; d.i = prolog_atom_carved(b, n, k); return d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_sink_kind(DESCR_t a) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(a);
    if ((int)v.v != DT_PLREF) return 0;
    int fid = plc_fid_name(v.slen);
    int ar = plc_fid_arity(v.slen);
    if (ar != 1) return 0;
    const char *fnm = prolog_atom_name(fid);
    if (!fnm) return 0;
    if (!strcmp(fnm, "atom")) return 1;
    if (!strcmp(fnm, "string")) return 2;
    if (!strcmp(fnm, "codes")) return 3;
    if (!strcmp(fnm, "chars")) return 4;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_unify_raise_c(DESCR_t *a, DESCR_t *b, pl_tr_ctx_t *cx) {
    char *tr0;
    rt_pl_tr_gc_sync(cx->tr);
    tr0 = cx->tr;
    (void)plw_unify_cells(plw_entry(a), plw_entry(b), cx);
    cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
    rt_pl_tr_gc_sync(cx->tr);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_body_ctl(DESCR_t v, int swi) {
    extern int prolog_atom_intern(const char *);
    uint32_t f = v.slen;
    if (plc_fid_arity(f) == 1) return swi && f == (uint32_t)prolog_functor_intern(prolog_atom_intern("\\+"), 1);
    if (plc_fid_arity(f) != 2) return 0;
    if (f == (uint32_t)prolog_functor_intern(prolog_atom_intern(","), 2) || f == (uint32_t)prolog_functor_intern(prolog_atom_intern(";"), 2)) return 1;
    return f == (uint32_t)prolog_functor_intern(prolog_atom_intern("->"), 2) || (swi && f == (uint32_t)prolog_functor_intern(prolog_atom_intern("*->"), 2));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_body_call_vars(DESCR_t b, int swi) {
    extern int prolog_atom_intern(const char *);
    DESCR_t *bp = plw_cell_deref(plw_entry(&b));
    DESCR_t r = *bp;
    if (plw_unbound_tag(bp)) {
        DESCR_t *k = (DESCR_t *)rt_ws_alloc_descr(1);
        DESCR_t c = {0};
        k[0] = *bp;
        c.v = (DTYPE_t)DT_PLREF;
        c.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern("call"), 1);
        c.p = (void *)k;
        return c;
    }
    if (r.v != (DTYPE_t)DT_PLREF || !pl_body_ctl(r, swi)) return r;
    {
        int ar = plc_fid_arity(r.slen);
        DESCR_t *a = (DESCR_t *)r.p;
        DESCR_t *k = (DESCR_t *)rt_ws_alloc_descr((size_t)ar);
        for (int i = 0; i < ar; i++) k[i] = pl_body_call_vars(a[i], swi);
        r.p = (void *)k;
        return r;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_clause_head_shown(DESCR_t h, void *root) {
    extern int rt_pl_db_mod_get(void *, const char *, int, int64_t *);
    extern void *rt_pl_compound_cell(const char *, int, void *);
    extern const char *prolog_atom_name(int);
    const char *nm = h.v == DT_PLATOM ? pl_atom_str(h) : h.v == (DTYPE_t)DT_PLREF ? prolog_atom_name(plc_fid_name(h.slen)) : (const char *)0;
    const char *c = nm ? strchr(nm, ':') : (const char *)0;
    int64_t sz = 0;
    if (!c || c == nm || !c[1]) return h;
    {
        char mod[(size_t)(c - nm) + 1];
        DESCR_t kids[2];
        memcpy(mod, nm, (size_t)(c - nm));
        mod[c - nm] = 0;
        if (!strcmp(mod, "user") || !rt_pl_db_mod_get(root, mod, 0, &sz)) return h;
        kids[0] = pl_mk_atom(mod);
        kids[1] = h.v == DT_PLATOM ? pl_mk_atom(c + 1) : *(DESCR_t *)rt_pl_compound_cell(c + 1, plc_fid_arity(h.slen), h.p);
        return *(DESCR_t *)rt_pl_compound_cell(":", 2, (void *)kids);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_clause_unify_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    if (nargs != 2) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    {
        char *tr0 = cx->tr;
        DESCR_t out;
        DESCR_t rc = rt_pl_deref_val(args[0]);
        DESCR_t tc = rt_pl_deref_val(args[1]);
        if (rc.v != (DTYPE_t)DT_PLREF || plc_fid_arity(rc.slen) != 2 || tc.v != (DTYPE_t)DT_PLREF || plc_fid_arity(tc.slen) != 2) out = FAILDESCR;
        else {
            DESCR_t *ra = (DESCR_t *)rc.p;
            DESCR_t *ta = (DESCR_t *)tc.p;
            DESCR_t bodyv = pl_body_call_vars(ra[1], !pl_gnu_dialect());
            DESCR_t t0 = rt_pl_deref_val(ta[0]);
            int shown = plw_unbound_tag(plw_cell_deref(plw_entry(&ta[0]))) || (t0.v == (DTYPE_t)DT_PLREF && plc_fid_arity(t0.slen) == 2 && plc_fid_name(t0.slen) == prolog_atom_intern(":"));
            DESCR_t headv = shown ? pl_clause_head_shown(rt_pl_deref_val(ra[0]), (void *)(uintptr_t)cx->r14) : ra[0];
            out = plw_unify_vals(headv, ta[0], cx) && plw_unify_vals(bodyv, ta[1], cx) ? rt_pl_deref_val(args[0]) : FAILDESCR;
        }
        if (out.v == DT_FAIL) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
        rt_pl_tr_gc_sync(cx->tr);
        return out;
    }
}
DESCR_t pl_os_cwd_atom(void) {
    for (size_t cn = 256;; cn *= 2) {
        char *cw = pl_atom_carve_cx(cn, (pl_tr_ctx_t *)0);
        if (!cw) return FAILDESCR;
        if (getcwd(cw, cn + 1)) return pl_mk_atom_carved(cw, cn, strlen(cw));
        prolog_atom_uncarve(cw, cn);
        if (errno != ERANGE) return FAILDESCR;
    }
}
PL_CX_LEAF_HEAD(compound0, 3) {
    extern void *rt_pl_compound_cell(const char *, int, void *);
    extern int plc_swi_list_name(int, int);
    const char *op = pl_atom_str(rt_pl_deref_val(args[0]));
    DESCR_t t = rt_pl_deref_val(args[1]);
    ok = 0;
    if (op && !strcmp(op, "build") && pl_atom_str(t)) ok = plw_unify_vals(args[2], *(DESCR_t *)rt_pl_compound_cell(pl_atom_str(t), 0, (void *)0), cx);
    else if (op && !strcmp(op, "name") && t.v == (DTYPE_t)DT_PLREF) ok = plw_unify_vals(args[2], pl_mk_atom(prolog_atom_name(plc_swi_list_name(plc_fid_name(t.slen), plc_fid_arity(t.slen)))), cx);
    else if (op && !strcmp(op, "arity") && t.v == (DTYPE_t)DT_PLREF) ok = plw_unify_vals(args[2], INTVAL((long long)plc_fid_arity(t.slen)), cx);
}
PL_CX_LEAF_TAIL static int pl_dt_num(DESCR_t a, double *out) { DESCR_t d = rt_pl_deref_val(a); if (d.v == DT_I) { *out = (double)d.i; return 1; } if (d.v == DT_R) { *out = d.r; return 1; } return 0; }
static int pl_dt_int(DESCR_t a, long long *out) { DESCR_t d = rt_pl_deref_val(a); if (d.v != DT_I) return 0; *out = (long long)d.i; return 1; }
static long long pl_dt_days(long long y, long long m, long long d) {
    long long mm = y * 12 + (m - 1), yy = mm >= 0 ? mm / 12 : -((-mm + 11) / 12), mo = mm - yy * 12 + 1, era, yoe, doy;
    yy -= mo <= 2;
    era = (yy >= 0 ? yy : yy - 399) / 400;
    yoe = yy - era * 400;
    doy = (153 * (mo > 2 ? mo - 3 : mo + 9) + 2) / 5;
    return era * 146097 + yoe * 365 + yoe / 4 - yoe / 100 + doy - 719468 + (d - 1);
}
static void *pl_dt_bad(DESCR_t a, const char *ty) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    DESCR_t d = rt_pl_deref_val(a);
    return pl_val_unbound(d) ? rt_pl_ball_instantiation() : rt_pl_ball_kind2("type_error", ty, d);
}
static int pl_dt_stamp(DESCR_t a, double *st, long long *off, const char **zone, int *local, void **ball) {
    DESCR_t d = rt_pl_deref_val(a);
    int ar = d.v == (DTYPE_t)DT_PLREF ? plc_fid_arity(d.slen) : -1;
    const char *fn = ar > 0 ? prolog_atom_name(plc_fid_name(d.slen)) : (const char *)0;
    long long f[5] = { 0, 0, 0, 0, 0 };
    double sec = 0;
    *off = 0;
    *zone = (const char *)0;
    *local = 1;
    if (pl_dt_num(d, st)) return 1;
    if (!fn || strcmp(fn, "date") || (ar != 9 && ar != 3)) { *ball = pl_dt_bad(d, "date"); return 0; }
    {
        DESCR_t *k = (DESCR_t *)d.p;
        for (int i = 0; i < (ar == 9 ? 5 : 3); i++) if (!pl_dt_int(k[i], &f[i])) { *ball = pl_dt_bad(k[i], "integer"); return 0; }
        if (ar == 3) {
            struct tm tm;
            memset(&tm, 0, sizeof tm);
            tm.tm_year = (int)f[0] - 1900;
            tm.tm_mon = (int)f[1] - 1;
            tm.tm_mday = (int)f[2];
            tm.tm_isdst = -1;
            *st = (double)mktime(&tm);
            return 1;
        }
        if (!pl_dt_num(k[5], &sec)) { *ball = pl_dt_bad(k[5], "number"); return 0; }
        if (!pl_dt_int(k[6], off)) { *ball = pl_dt_bad(k[6], "integer"); return 0; }
        *zone = pl_atom_str(rt_pl_deref_val(k[7]));
        *local = 0;
        *st = (double)(pl_dt_days(f[0], f[1], f[2]) * 86400 + f[3] * 3600 + f[4] * 60 + *off) + sec;
        return 1;
    }
}
PL_CX_LEAF_HEAD(date_time_stamp, 2) {
    double st = 0;
    long long off;
    const char *zone;
    int local;
    void *ball = (void *)0;
    ok = 0;
    if (pl_dt_num(args[0], &st)) cx->ball = pl_dt_bad(args[0], "date");
    else if (pl_dt_stamp(args[0], &st, &off, &zone, &local, &ball)) ok = plw_unify_vals(args[1], REALVAL(st), cx);
    else cx->ball = ball;
    pl_gnu_ctx(cx, "date_time_stamp", 2);
}
PL_CX_LEAF_TAIL PL_CX_LEAF_HEAD(stamp_date_time, 3) {
    DESCR_t z = rt_pl_deref_val(args[2]);
    const char *zs = pl_atom_str(z);
    int mode = z.v == DT_I ? 1 : (zs && !strcmp(zs, "UTC")) ? 2 : (zs && !strcmp(zs, "local")) ? 3 : 0;
    double st = 0;
    ok = 0;
    if (!pl_dt_num(args[0], &st)) cx->ball = pl_dt_bad(args[0], "number");
    else if (!mode) cx->ball = pl_dt_bad(z, "atom");
    else {
        double fl = floor(st);
        time_t t = (time_t)fl;
        struct tm tm;
        long long off = 0;
        DESCR_t a[9];
        a[7] = pl_mk_atom_dup("-", 1);
        a[8] = pl_mk_atom_dup("-", 1);
        if (mode == 1) {
            off = (long long)z.i;
            t -= (time_t)off;
            gmtime_r(&t, &tm);
        } else if (mode == 2) {
            gmtime_r(&t, &tm);
            a[7] = pl_mk_atom_dup("UTC", 3);
        } else {
            localtime_r(&t, &tm);
            off = -(long long)tm.tm_gmtoff;
            a[7] = tm.tm_zone ? pl_mk_atom_dup(tm.tm_zone, strlen(tm.tm_zone)) : a[7];
            a[8] = tm.tm_isdst > 0 ? pl_mk_atom_dup("true", 4) : pl_mk_atom_dup("false", 5);
        }
        a[0] = INTVAL(tm.tm_year + 1900);
        a[1] = INTVAL(tm.tm_mon + 1);
        a[2] = INTVAL(tm.tm_mday);
        a[3] = INTVAL(tm.tm_hour);
        a[4] = INTVAL(tm.tm_min);
        a[5] = REALVAL((double)tm.tm_sec + (st - fl));
        a[6] = INTVAL(off);
        ok = plw_unify_vals(args[1], pl_gnu_fn("date", 9, a), cx);
    }
    pl_gnu_ctx(cx, "stamp_date_time", 3);
}
PL_CX_LEAF_TAIL PL_CX_LEAF_HEAD(format_time_text, 3) {
    const char *fm = pl_atom_str(rt_pl_deref_val(args[0]));
    double st = 0;
    long long off;
    const char *zone;
    int local;
    void *ball = (void *)0;
    ok = 0;
    if (!fm) cx->ball = pl_dt_bad(args[0], "text");
    else if (!pl_dt_stamp(args[1], &st, &off, &zone, &local, &ball)) cx->ball = ball;
    else {
        size_t cap = strlen(fm) * 64 + 64, n = 0;
        char *out = pl_atom_carve_cx(cap, cx);
        if (out) {
            double fl = floor(st), frac = fabs(st - trunc(st));
            time_t t = (time_t)fl;
            struct tm tm;
            if (local) localtime_r(&t, &tm);
            else { t -= (time_t)off; gmtime_r(&t, &tm); tm.tm_gmtoff = -(long)off; tm.tm_zone = (zone && strcmp(zone, "-")) ? zone : ""; }
            for (const char *p = fm; *p && n + 64 < cap; p++) {
                int w = 0;
                if (*p != '%') { out[n++] = *p; continue; }
                p++;
                while (*p >= '0' && *p <= '9') w = w * 10 + (*p++ - '0');
                if (*p == 'f') {
                    long long us = llround(frac * 1e6), v = us > 999999 ? 999999 : us;
                    if (!w) w = 6;
                    if (w > 15) w = 15;
                    for (int i = w; i < 6; i++) v /= 10;
                    for (int i = 6; i < w; i++) v *= 10;
                    n += (size_t)snprintf(out + n, cap - n, "%0*lld", w, v);
                } else if (*p == ':' && p[1] == 'z') {
                    long g = tm.tm_gmtoff, a = g < 0 ? -g : g;
                    p++;
                    n += (size_t)snprintf(out + n, cap - n, "%c%02ld:%02ld", g < 0 ? '-' : '+', a / 3600, a / 60 % 60);
                } else if (*p == 's') n += (size_t)snprintf(out + n, cap - n, "%lld", (long long)fl);
                else if (*p == '+') n += strftime(out + n, cap - n, "%a %b %e %H:%M:%S %Y", &tm);
                else if (*p) {
                    char f2[5] = { '%', 0, 0, 0, 0 };
                    int k = 1;
                    if (*p == '-' || *p == '_' || *p == 'E' || *p == 'O') f2[k++] = *p++;
                    f2[k] = *p;
                    if (*p) n += strftime(out + n, cap - n, f2, &tm);
                    else p--;
                } else p--;
            }
            ok = plw_unify_vals(args[2], pl_mk_atom_carved(out, cap, n), cx);
        }
    }
    pl_gnu_ctx(cx, "format_time", 3);
}
PL_CX_LEAF_TAIL static uint32_t pl_gnu_rotl(uint32_t x, int r) { return (x << r) | (x >> (32 - r)); }
static uint32_t pl_gnu_hblock(uint32_t k, uint32_t h) { k *= 0xcc9e2d51u; k = pl_gnu_rotl(k, 15); k *= 0x1b873593u; h ^= k; h = pl_gnu_rotl(h, 13); return h * 5 + 0xe6546b64u; }
static uint32_t pl_gnu_hfinal(uint32_t h, uint32_t len) { h ^= len; h ^= h >> 16; h *= 0x85ebca6bu; h ^= h >> 13; h *= 0xc2b2ae35u; h ^= h >> 16; return h; }
static uint32_t pl_gnu_atom_hash(const char *s) {
    size_t n = strlen(s), i = 0;
    uint32_t h = 1688943522u, k;
    if (n == 0) return 0;
    if (n == 1) return (uint32_t)(unsigned char)s[0];
    if (n == 2 && s[0] == '[' && s[1] == ']') return 256;
    for (; i + 4 <= n; i += 4) { memcpy(&k, s + i, 4); h = pl_gnu_hblock(k, h); }
    k = 0;
    switch (n & 3) {
        case 3:
        k ^= (uint32_t)(unsigned char)s[i + 2] << 16;
        case 2:
        k ^= (uint32_t)(unsigned char)s[i + 1] << 8;
        case 1:
        k ^= (uint32_t)(unsigned char)s[i];
        k *= 0xcc9e2d51u;
        k = pl_gnu_rotl(k, 15);
        k *= 0x1b873593u;
        h ^= k;
    }
    return pl_gnu_hfinal(h, (uint32_t)n);
}
static void pl_gnu_h32(uint32_t *h, uint32_t *len, uint32_t x) { *len += 4; *h = pl_gnu_hblock(x, *h); }
static void pl_gnu_h64(uint32_t *h, uint32_t *len, uint64_t x) { pl_gnu_h32(h, len, (uint32_t)x); pl_gnu_h32(h, len, (uint32_t)(x >> 32)); }
static void pl_gnu_hdbl(uint32_t *h, uint32_t *len, double x) {
    int e;
    uint64_t m;
    x = frexp(x, &e);
    if (x < 0.0) x = -(x + 0.5);
    x *= (double)((uint64_t)1 << 63);
    m = (uint64_t)(int64_t)x;
    m = m + (((uint64_t)1 << 63) / 1024) * (uint64_t)(int64_t)e;
    pl_gnu_h64(h, len, m);
}
static int pl_gnu_hash_walk(DESCR_t *c, uint32_t *h, uint32_t *len, long depth) {
    extern const char *prolog_atom_name(int);
    extern char *rt_big_str(DESCR_t);
    DESCR_t *first = (DESCR_t *)0;
    long nmarked = 0;
    int ret = 1;
    extern int rt_stack_refuses(long bytes);
    if (rt_stack_refuses(256)) return 0;
    for (;;) {
        DESCR_t *d = plw_cell_deref(plw_entry(c));
        const char *s;
        if (d->v == PL_HASH_ON_PATH) { pl_gnu_h32(h, len, 0x9e3779b9u); break; }
        if (plw_unbound_tag(d)) { ret = 0; break; }
        if (d->v == (DTYPE_t)DT_PLREF) {
            int ar = plc_fid_arity(d->slen);
            DESCR_t *kids = (DESCR_t *)d->p;
            if (!pl_is_cons(*d)) { const char *fn = prolog_atom_name(plc_fid_name(d->slen)); pl_gnu_h32(h, len, pl_gnu_atom_hash(fn ? fn : "")); pl_gnu_h32(h, len, (uint32_t)ar); }
            if (--depth == 0) { if (pl_is_cons(*d)) { pl_gnu_h32(h, len, pl_gnu_atom_hash(".")); pl_gnu_h32(h, len, 2u); } break; }
            if (ar == 0) break;
            d->v = PL_HASH_ON_PATH;
            if (!nmarked++) first = d;
            int bad = 0;
            for (int i = 0; i < ar - 1; i++) if (!pl_gnu_hash_walk(&kids[i], h, len, depth)) { bad = 1; break; }
            if (bad) { ret = 0; break; }
            c = &kids[ar - 1];
            continue;
        }
        if ((s = pl_atom_str(*d))) { pl_gnu_h32(h, len, pl_gnu_atom_hash(s)); break; }
        if (d->v == DT_I) { pl_gnu_h64(h, len, (uint64_t)d->i); break; }
        if (d->v == DT_R) { pl_gnu_hdbl(h, len, d->r); break; }
        if (d->v == DT_BIG) { pl_gnu_h32(h, len, pl_gnu_atom_hash(rt_big_str(*d))); break; }
        pl_gnu_h64(h, len, (uint64_t)d->i);
        break;
    }
    for (DESCR_t *d = first; nmarked > 0; nmarked--) { d->v = (DTYPE_t)DT_PLREF; if (nmarked > 1) d = plw_cell_deref(plw_entry(&((DESCR_t *)d->p)[plc_fid_arity(d->slen) - 1])); }
    return ret;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_gnu_hash_top(DESCR_t *c, uint32_t *h, uint32_t *len, long depth, pl_tr_ctx_t *cx) {
    extern void *rt_pl_cap_error(void);
    int arm = !g_rt_misc.stack_refuse_armed, r;
    if (arm) g_rt_misc.stack_refuse_armed = 1;
    r = pl_gnu_hash_walk(c, h, len, depth);
    if (!arm) return r;
    g_rt_misc.stack_refuse_armed = 0;
    if (!g_rt_misc.stack_refused) return r;
    g_rt_misc.stack_refused = 0;
    cx->ball = rt_pl_cap_error();
    return -1;
}
PL_CX_LEAF_HEAD(gnu_term_hash, 2) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t hv = rt_pl_deref_val(args[1]);
    uint32_t h = 1688943522u, len = 0;
    int hr = 1;
    ok = 0;
    if (!pl_val_unbound(hv) && hv.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", hv);
    else if ((hr = pl_gnu_hash_top(&args[0], &h, &len, -1, cx)) < 0) ok = 0;
    else if (!hr) ok = 1;
    else ok = plw_unify_vals(args[1], INTVAL((int64_t)(pl_gnu_hfinal(h, len) % (1u << 28))), cx);
    pl_gnu_ctx(cx, "term_hash", 2);
}
PL_CX_LEAF_TAIL PL_CX_LEAF_HEAD(gnu_term_hash4, 4) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t hv = rt_pl_deref_val(args[3]);
    DESCR_t rv = rt_pl_deref_val(args[2]);
    long dp = (long)rt_pl_deref_val(args[1]).i;
    uint32_t h = 1688943522u, len = 0, m = (rv.i <= 0 || rv.i > ((int64_t)1 << 28)) ? (1u << 28) : (uint32_t)rv.i;
    int hr = 1;
    ok = 0;
    if (!pl_val_unbound(hv) && hv.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", hv);
    else if (dp != 0 && (hr = pl_gnu_hash_top(&args[0], &h, &len, dp, cx)) < 0) ok = 0;
    else if (dp != 0 && !hr) ok = 1;
    else ok = plw_unify_vals(args[3], INTVAL((int64_t)(pl_gnu_hfinal(h, len) % m)), cx);
    pl_gnu_ctx(cx, "term_hash", 4);
}
PL_CX_LEAF_TAIL PL_CX_LEAF_HEAD(gnu_prolog_pid, 1) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t d = rt_pl_deref_val(args[0]);
    ok = 0;
    if (!pl_val_unbound(d) && d.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", d);
    else ok = plw_unify_vals(args[0], INTVAL((int64_t)getpid()), cx);
    pl_gnu_ctx(cx, "prolog_pid", 1);
}
PL_CX_LEAF_TAIL int bnd_db_key_need(void *cell) {
    extern const char *prolog_atom_name(int);
    pl_cell_t *t = pl_deref((pl_cell_t *)cell), *h = t;
    const char *nm = (const char *)0;
    int ar = 0;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 2) { const char *fn = prolog_atom_name(plc_functor(t)); if (fn && !strcmp(fn, ":-")) h = pl_deref(&((pl_cell_t *)t->p)[0]); }
    if ((int)h->v == DT_PLREF) { nm = prolog_atom_name(plc_functor(h)); ar = pl_arity(h); } else if ((int)h->v == DT_PLATOM) nm = prolog_atom_name((int)h->i);
    else if ((int)h->v == DT_S) nm = h->s;
    return nm ? fmt_len("%s/%d", nm, ar) : 1;
}
static int bnd_db_and_key_need(DESCR_t *tgt, DESCR_t refv) {
    int n = bnd_db_key_need((void *)tgt);
    DESCR_t r = rt_pl_deref_val(refv);
    if (r.v == (DTYPE_t)DT_PLREF && plc_fid_arity(r.slen) == 2) { const char *ks = pl_atom_str(rt_pl_deref_val(((DESCR_t *)r.p)[0])); if (ks && (int)strlen(ks) + 1 > n) n = (int)strlen(ks) + 1; }
    return n;
}
static void *pl_db_and_key(DESCR_t *tgt, DESCR_t refv, void *root, char *key, size_t kn) {
    extern int rt_pl_db_term_key(void *, char *, size_t, int *);
    extern void *rt_pl_db_get_by_key(void *, const char *, int);
    int ar = 0;
    {
        DESCR_t r = rt_pl_deref_val(refv);
        if (pl_cell_is_clref(r)) {
            DESCR_t k = rt_pl_deref_val(((DESCR_t *)r.p)[0]);
            const char *ks = pl_atom_str(k);
            if (ks) { snprintf(key, kn, "%s", ks); return rt_pl_db_get_by_key(root, ks, 0); }
        }
    }
    if (rt_pl_db_term_key((void *)tgt, key, kn, &ar)) return rt_pl_db_get_by_key(root, key, 0);
    return (void *)0;
}
DESCR_t rt_pl_dop_db_n_r_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_walk_hi(void *);
    char key[nargs == 2 ? bnd_db_and_key_need(&args[0], args[1]) : 1];
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    { void *db = pl_db_and_key(&args[0], args[1], root, key, sizeof key); return INTVAL(rt_pl_db_walk_hi(db)); }
}
DESCR_t rt_pl_dop_db_at_r_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_clause_at(void *, int, void *);
    char key[nargs == 3 ? bnd_db_and_key_need(&args[0], args[1]) : 1];
    DESCR_t out;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    {
        void *db = pl_db_and_key(&args[0], args[1], root, key, sizeof key);
        DESCR_t iv = rt_pl_deref_val(args[2]);
        if (!db || iv.v != DT_I) return FAILDESCR;
        return rt_pl_db_clause_at(db, (int)iv.i, (void *)&out) ? out : FAILDESCR;
    }
}
DESCR_t rt_pl_dop_db_ref_r_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_ref_at(void *, int);
    extern void *rt_pl_compound_cell(const char *, int, void *);
    size_t kn;
    char *key;
    if (nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    kn = (size_t)bnd_db_and_key_need(&args[0], args[1]);
    key = pl_atom_carve_cx(kn - 1, (pl_tr_ctx_t *)0);
    if (!key) return FAILDESCR;
    {
        void *db = pl_db_and_key(&args[0], args[1], root, key, kn);
        DESCR_t iv = rt_pl_deref_val(args[2]);
        if (!db || iv.v != DT_I) { prolog_atom_uncarve(key, kn - 1); return FAILDESCR; }
        {
            int r = rt_pl_db_ref_at(db, (int)iv.i);
            DESCR_t kids[2];
            DESCR_t *rc;
            if (r < 1) { prolog_atom_uncarve(key, kn - 1); return FAILDESCR; }
            kids[0] = pl_mk_atom_carved(key, kn - 1, strlen(key));
            kids[1] = INTVAL((long long)r);
            rc = (DESCR_t *)rt_pl_compound_cell("$clref", 2, (void *)kids);
            return rc ? *rc : FAILDESCR;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_choice_add(void *choice, void *clause_tree) { if (choice && clause_tree) ast_push((tree_t *)choice, (tree_t *)clause_tree); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_choice_n(void *choice) { return choice ? ((tree_t *)choice)->n : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutcall_ctrl(const char *nm) { return nm && (!strcmp(nm, ",") || !strcmp(nm, ";") || !strcmp(nm, "->") || !strcmp(nm, "*->") || !strcmp(nm, "|")); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_cutcall_walk(DESCR_t *c, char *sk, size_t *len, DESCR_t **leaves, int *nleaf, int *cuts, int depth, tree_t **out) {
    extern const char *prolog_atom_name(int);
    DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)c);
    if (depth > 2048 || *nleaf >= 255) return 0;
    if (!pl_cell_unbound(d) && (int)d->v == DT_PLATOM) {
        const char *s = prolog_atom_name((int)d->i);
        if (s && !strcmp(s, "!")) { if (sk) sk[*len] = '!'; (*len)++; (*cuts)++; if (out) *out = ast_node_new(TT_CUT); return 1; }
    }
    if (!pl_cell_unbound(d) && (int)d->v == DT_PLREF && pl_arity((pl_cell_t *)d) == 2) {
        const char *nm = prolog_atom_name(plc_functor((pl_cell_t *)d));
        if (pl_cutcall_ctrl(nm)) {
            DESCR_t *aa = (DESCR_t *)d->p;
            tree_t *l = (tree_t *)0, *r = (tree_t *)0;
            if (sk) { sk[*len] = '('; sk[*len + 1] = !strcmp(nm, ",") ? ',' : !strcmp(nm, ";") ? ';' : !strcmp(nm, "->") ? '>' : !strcmp(nm, "*->") ? '*' : '|'; }
            *len += 2;
            if (!pl_cutcall_walk(&aa[0], sk, len, leaves, nleaf, cuts, depth + 1, out ? &l : (tree_t **)0)) return 0;
            if (!pl_cutcall_walk(&aa[1], sk, len, leaves, nleaf, cuts, depth + 1, out ? &r : (tree_t **)0)) return 0;
            if (sk) sk[*len] = ')';
            (*len)++;
            if (out) { tree_t *t = ast_node_new(TT_FNC); t->v.sval = pl_tree_name(nm); ast_push(t, l); ast_push(t, r); *out = t; }
            return 1;
        }
    }
    if (!pl_cell_unbound(d) && (int)d->v == DT_PLREF && pl_arity((pl_cell_t *)d) == 1) {
        const char *nm = prolog_atom_name(plc_functor((pl_cell_t *)d));
        int mk = nm && !strcmp(nm, "prolog_current_choice") ? 'C' : nm && !strcmp(nm, "prolog_cut_to") ? 'T' : 0;
        DESCR_t *a = mk ? (DESCR_t *)pl_deref((pl_cell_t *)&((DESCR_t *)d->p)[0]) : (DESCR_t *)0;
        if (a && pl_cell_unbound(a)) {
            int k = *nleaf;
            for (int i = 0; leaves && i < *nleaf; i++) if (leaves[i] == a) { k = i; break; }
            if (k == *nleaf) { if (leaves) leaves[k] = a; (*nleaf)++; }
            if (mk == 'T') (*cuts)++;
            if (sk) { sk[*len] = (char)mk; sk[*len + 1] = (char)('0' + k / 100); sk[*len + 2] = (char)('0' + k / 10 % 10); sk[*len + 3] = (char)('0' + k % 10); }
            *len += 4;
            if (out) {
                int vl = snprintf(NULL, 0, "_L%d", k) + 1;
                char vn[vl];
                tree_t *v = ast_node_new(TT_VAR), *t = ast_node_new(TT_FNC);
                snprintf(vn, (size_t)vl, "_L%d", k);
                v->v.sval = pl_tree_name(vn);
                t->v.sval = pl_tree_name(nm);
                ast_push(t, v);
                *out = t;
            }
            return 1;
        }
    }
    if (sk) sk[*len] = 'V';
    (*len)++;
    if (leaves) leaves[*nleaf] = d;
    if (out) {
        int vl = snprintf(NULL, 0, "_L%d", *nleaf) + 1;
        char vn[vl];
        tree_t *v = ast_node_new(TT_VAR), *t = ast_node_new(TT_FNC);
        snprintf(vn, (size_t)vl, "_L%d", *nleaf);
        v->v.sval = pl_tree_name(vn);
        t->v.sval = pl_tree_name("call");
        ast_push(t, v);
        *out = t;
    }
    (*nleaf)++;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_shc_has(DESCR_t *c, int depth) {
    extern const char *prolog_atom_name(int);
    for (;;) {
        DESCR_t *d = (DESCR_t *)pl_deref((pl_cell_t *)c);
        const char *nm;
        if (pl_cell_unbound(d)) return 0;
        if ((int)d->v == DT_PLATOM) { const char *s = prolog_atom_name((int)d->i); return s && !strcmp(s, "!"); }
        if ((int)d->v != DT_PLREF || pl_arity((pl_cell_t *)d) != 2) return 0;
        nm = prolog_atom_name(plc_functor((pl_cell_t *)d));
        if (!nm || strcmp(nm, ",")) return 0;
        if (depth < 2048 && pl_shc_has(&((DESCR_t *)d->p)[0], depth + 1)) return 1;
        c = &((DESCR_t *)d->p)[1];
    }
}
PL_CX_LEAF_HEAD(shc, 1) ok = pl_shc_has(&args[0], 0);
PL_CX_LEAF_TAIL PL_CX_LEAF_HEAD(cutcall, 2) {
    extern tree_t *pl_runtime_clause_tree(tree_t *);
    extern void *pl_runtime_define_pred(const char *, const void *, int, void *);
    extern int prolog_atom_intern(const char *);
    size_t len = 0;
    int nleaf = 0, cuts = 0;
    ok = 0;
    if (pl_cutcall_walk(&args[0], (char *)0, &len, (DESCR_t **)0, &nleaf, &cuts, 0, (tree_t **)0) && cuts) {
        char *nb = pl_atom_carve_cx(len + 3, cx);
        DESCR_t *leaves[nleaf > 0 ? nleaf : 1];
        size_t l2 = 0;
        int n = 0, c2 = 0;
        if (nb) memcpy(nb, "$cc", 3);
        if (nb && !pl_cutcall_walk(&args[0], nb + 3, &l2, leaves, &n, &c2, 0, (tree_t **)0)) prolog_atom_uncarve(nb, len + 3);
        else if (nb) {
            {
                const char *nm = prolog_atom_name(prolog_atom_carved(nb, len + 3, l2 + 3));
                int kl = snprintf(NULL, 0, "%s/%d", nm, n) + 1;
                char key[kl];
                snprintf(key, (size_t)kl, "%s/%d", nm, n);
                int have = rt_proc_is_registered(key);
                if (!have) {
                    tree_t *body = (tree_t *)0;
                    size_t l3 = 0;
                    int n3 = 0, c3 = 0;
                    DESCR_t *lv3[nleaf > 0 ? nleaf : 1];
                    if (pl_cutcall_walk(&args[0], (char *)0, &l3, lv3, &n3, &c3, 0, &body) && body) {
                        tree_t *hd = ast_node_new(n ? TT_FNC : TT_QLIT), *raw = ast_node_new(TT_FNC), *cl;
                        void *ch;
                        hd->v.sval = pl_tree_name(nm);
                        for (int i = 0; i < n; i++) {
                            int vl = snprintf(NULL, 0, "_L%d", i) + 1;
                            char vn[vl];
                            tree_t *v = ast_node_new(TT_VAR);
                            snprintf(vn, (size_t)vl, "_L%d", i);
                            v->v.sval = pl_tree_name(vn);
                            ast_push(hd, v);
                        }
                        raw->v.sval = pl_tree_name(":-");
                        ast_push(raw, hd);
                        ast_push(raw, body);
                        cl = pl_runtime_clause_tree(raw);
                        ch = cl ? rt_pl_choice_new(key) : (void *)0;
                        if (ch) { rt_pl_choice_add(ch, cl); have = pl_runtime_define_pred(key, ch, n, (void *)(uintptr_t)cx->r14) != (void *)0; }
                    }
                }
                if (have) {
                    DESCR_t g = pl_mk_atom(nm);
                    if (n) {
                        DESCR_t *kids = (DESCR_t *)rt_ws_alloc_descr((size_t)n);
                        for (int i = 0; i < n; i++) kids[i] = pl_cell_unbound(leaves[i]) ? pl_make_ref((pl_cell_t *)leaves[i], 0) : *leaves[i];
                        g.v = (DTYPE_t)DT_PLREF;
                        g.slen = (uint32_t)prolog_functor_intern(prolog_atom_intern(nm), n);
                        g.p = (void *)kids;
                    }
                    ok = plw_unify_vals(args[1], g, cx);
                }
            }
        }
    }
}
PL_CX_LEAF_TAIL
