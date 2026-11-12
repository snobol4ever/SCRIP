#include "runtime/unification/un_head.h"
#include "runtime/unification/un_01_pl.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_05_pl.h"
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val);
extern int plw_unify_cell_val(DESCR_t *, DESCR_t, pl_tr_ctx_t *);
extern int rt_pl_dialect_swi(void);
extern const char *prolog_atom_name(int id);
static const char *plc_atom_op_list_text_heap(pl_cell_t *d, size_t *len);
static const char *plc_atom_op_text_n(pl_cell_t *t, char *buf, size_t bufsz, size_t *len);
static int plc_unify_into_cell_cx(pl_cell_t *dst, pl_cell_t val, pl_tr_ctx_t *cx) { return cx ? plw_unify_cell_val(dst, val, cx) : plc_unify_into_cell(dst, val); }
const char *plc_vname(plc_vmap *m, pl_cell_t *d) {
    const char *r = (const char *)0;
    if (m->vnc <= 0 || !m->vnl) return r;
    for (pl_cell_t *lst = pl_deref(m->vnl); (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2 && lst->p; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
        pl_cell_t *o = pl_deref(&((pl_cell_t *)lst->p)[0]);
        const char *on = (int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1 && o->p ? prolog_atom_name(plc_fid_name(o->slen)) : (const char *)0;
        if (!on || strcmp(on, "variable_names")) continue;
        for (pl_cell_t *vl = pl_deref(&((pl_cell_t *)o->p)[0]); (int)vl->v == DT_PLREF && plc_fid_arity(vl->slen) == 2 && vl->p; vl = pl_deref(&((pl_cell_t *)vl->p)[1])) {
            pl_cell_t *e = pl_deref(&((pl_cell_t *)vl->p)[0]);
            if (!((int)e->v == DT_PLREF && plc_fid_arity(e->slen) == 2 && e->p)) continue;
            pl_cell_t *nm = pl_deref(&((pl_cell_t *)e->p)[0]), *vr = pl_deref(&((pl_cell_t *)e->p)[1]);
            if (vr == d && pl_cell_unbound(vr) && ((int)nm->v == DT_PLATOM || (int)nm->v == DT_S)) { r = plc_atom_text(nm); break; }
        }
    }
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_vindex(plc_vmap *m, pl_cell_t *d) { for (int i = 0; i < m->n; i++) if (m->seen[i] == d) return i; if (m->n < 1024) { m->seen[m->n] = d; return m->n++; } return m->n - 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *plc_atom_text(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->s : ""; const char *n = prolog_atom_name((int)d->i); return n ? n : "?"; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t plc_atom_len(pl_cell_t *d) { if ((int)d->v == DT_S) return d->s ? d->slen : 0; return prolog_atom_name((int)d->i) ? prolog_atom_len((int)d->i) : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_opt_is_true(pl_cell_t *o) {
    if (!o) return 0;
    o = pl_deref(o);
    if ((int)o->v == DT_PLREF && plc_fid_arity(o->slen) == 1) {
        pl_cell_t *a = pl_deref((pl_cell_t *)o->p);
        return ((int)a->v == DT_PLATOM || (int)a->v == DT_S) && strcmp(plc_atom_text(a), "true") == 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_sp_ball(pl_tr_ctx_t *cx, void *b) { if (cx && !cx->ball) cx->ball = b; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_int_arg(pl_cell_t *d, pl_tr_ctx_t *cx, int *bound) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    *bound = 0;
    if (!d || pl_cell_unbound(d)) return 1;
    if ((int)d->v != DT_I && (int)d->v != DT_BIG) { plc_sp_ball(cx, rt_pl_ball_kind2("type_error", "integer", *d)); return 0; }
    *bound = 1;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_sp_not_negative(pl_cell_t *d, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern int rt_big_sign(DESCR_t);
    if (d && ((int)d->v == DT_I || (int)d->v == DT_BIG) && rt_big_sign(*d) < 0) { plc_sp_ball(cx, rt_pl_ball_kind2("domain_error", "not_less_than_zero", *d)); return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_sp_addsub(pl_cell_t a, pl_cell_t b, int sub) {
    extern DESCR_t rt_big_add(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    long long r;
    if ((int)a.v == DT_I && (int)b.v == DT_I && !(sub ? __builtin_sub_overflow(a.i, b.i, &r) : __builtin_add_overflow(a.i, b.i, &r))) return pl_make_int(r);
    return sub ? rt_big_sub(a, b) : rt_big_add(a, b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_wall_clock_cell(int ms, void *a_cell, pl_tr_ctx_t *cx) {
    struct timespec _ts;
    clock_gettime(ms == 2 ? CLOCK_PROCESS_CPUTIME_ID : CLOCK_MONOTONIC, &_ts);
    long long us = (long long)_ts.tv_sec * 1000000LL + (long long)_ts.tv_nsec / 1000LL;
    pl_cell_t v = pl_make_int(ms ? us / 1000LL : us);
    return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_succ_plus_cell(long arity, void *a_cell, void *b_cell, void *c_cell, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_instantiation(void);
    pl_cell_t *da = a_cell ? pl_deref((pl_cell_t *)a_cell) : (pl_cell_t *)0;
    pl_cell_t *db = b_cell ? pl_deref((pl_cell_t *)b_cell) : (pl_cell_t *)0;
    pl_cell_t *dc = c_cell ? pl_deref((pl_cell_t *)c_cell) : (pl_cell_t *)0;
    if (arity == 2) {
        int va, vb;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb)) return 0;
        if (!va && !vb) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (!plc_sp_not_negative(da, cx) || !plc_sp_not_negative(db, cx)) return 0;
        if (va) { pl_cell_t v = plc_sp_addsub(*da, pl_make_int(1), 0); return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        if ((int)db->v == DT_I && db->i == 0) return 0;
        { pl_cell_t v = plc_sp_addsub(*db, pl_make_int(1), 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    if (arity == 3) {
        int va, vb, vc;
        if (!plc_sp_int_arg(da, cx, &va) || !plc_sp_int_arg(db, cx, &vb) || !plc_sp_int_arg(dc, cx, &vc)) return 0;
        if (va + vb + vc < 2) { plc_sp_ball(cx, rt_pl_ball_instantiation()); return 0; }
        if (va && vb) { pl_cell_t v = plc_sp_addsub(*da, *db, 0); return plc_unify_cells_cx((pl_cell_t *)c_cell, &v, cx) ? 1 : 0; }
        if (va && vc) { pl_cell_t v = plc_sp_addsub(*dc, *da, 1); return plc_unify_cells_cx((pl_cell_t *)b_cell, &v, cx) ? 1 : 0; }
        { pl_cell_t v = plc_sp_addsub(*dc, *db, 1); return plc_unify_cells_cx((pl_cell_t *)a_cell, &v, cx) ? 1 : 0; }
    }
    return 0;
}
static const char *plc_text_nil_empty(pl_cell_t *t, char *buf, size_t bufsz, size_t *len) {
    if (t && rt_pl_dialect_swi() && plc_is_nil(pl_deref(t))) { *len = 0; return ""; }
    return plc_atom_op_text_n(t, buf, bufsz, len);
}
static const char *plc_atom_op_text_n(pl_cell_t *t, char *buf, size_t bufsz, size_t *len) {
    const char *r = (const char *)0;
    *len = 0;
    if (!t) return r;
    pl_cell_t *d = pl_deref(t);
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) { *len = plc_atom_len(d); return plc_atom_text(d); }
    if ((int)d->v == DT_PLREF) return plc_atom_op_list_text_heap(d, len);
    if ((int)d->v == DT_I) {
        snprintf(buf, bufsz, "%ld", (long)d->i);
        r = buf;
    } else if ((int)d->v == DT_BIG) {
        extern char *rt_big_str(DESCR_t);
        r = rt_big_str(*d);
    } else if (pl_rat_is(*d)) {
        size_t n = pl_rat_text(*d, (char *)0, 0) + 1;
        char *o = (char *)rt_wsb_alloc(n);
        (void)pl_rat_text(*d, o, n);
        r = o;
    } else if ((int)d->v == DT_R) {
        extern const char *pl_real_iso_str(double, char *, int);
        r = pl_real_iso_str(d->r, buf, (int)bufsz);
    }
    if (r) *len = strlen(r);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *plc_atom_op_text(pl_cell_t *t, char *buf, size_t bufsz) { size_t n; return plc_atom_op_text_n(t, buf, bufsz, &n); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_num_of_text(const char *s, pl_cell_t *out) {
    extern int rt_pl_parse_number(const char *, DESCR_t *);
    extern int rt_pl_flag_is(const char *, const char *);
    if (!s || !rt_pl_parse_number(s, out)) return 0;
    return !((int)out->v == DT_R && isinf(out->r) && !strstr(s, "Inf") && !strstr(s, "inf") && !rt_pl_flag_is("float_overflow", "infinity"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t plc_make_atom_cell(const char *name) { int id = prolog_atom_intern(name ? name : ""); const char *nm = prolog_atom_name(id); (void)nm; return pl_make_atom(id); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_make_atom_cell_n(const char *s, size_t n) { return pl_make_atom(prolog_atom_intern_n(s ? s : "", s ? n : 0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_unify_into_cell(pl_cell_t *dst, pl_cell_t val) { pl_cell_t tmp = val; return pl_unify(dst, &tmp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_make_string_cell_n(const char *s, size_t n) {
    pl_cell_t c = {0};
    c.v = (DTYPE_t)DT_S;
    c.s = (char *)rt_heap_alloc_c(n + 1);
    if (n) memcpy(c.s, s, n);
    c.s[n] = 0;
    c.slen = (uint32_t)n;
    return c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t rt_pl_make_string_cell(const char *s) { if (!s) s = ""; return rt_pl_make_string_cell_n(s, strlen(s)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_out_n(void *cell, const char *s, size_t n, int as_str, pl_tr_ctx_t *cx) {
    pl_cell_t *d = cell ? pl_deref((pl_cell_t *)cell) : (pl_cell_t *)0;
    if (d && !pl_cell_unbound(d) && ((int)d->v == DT_S || (int)d->v == DT_PLATOM)) return plc_atom_len(d) == n && (!n || !memcmp(plc_atom_text(d), s, n));
    return plc_unify_into_cell_cx((pl_cell_t *)cell, as_str ? rt_pl_make_string_cell_n(s, n) : plc_make_atom_cell_n(s, n), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_text_out(void *cell, const char *s, int as_str, pl_tr_ctx_t *cx) { return plc_text_out_n(cell, s, s ? strlen(s) : 0, as_str, cx); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *plc_text_carve(size_t n, int as_str, pl_tr_ctx_t *cx) {
    extern int rt_pl_cap_refuses(unsigned long long, unsigned long long);
    extern void *rt_pl_cap_error(void);
    char *b = (char *)0;
    if (!as_str) b = prolog_atom_carve(n);
    else if (!rt_pl_cap_refuses((unsigned long long)n + 1ULL, 1ULL)) b = (char *)rt_heap_alloc_c(n + 1);
    if (!b && cx && !cx->ball) cx->ball = rt_pl_cap_error();
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t plc_atom_built(char *b, size_t n, size_t k) { return pl_make_atom(prolog_atom_carved(b, n, k)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_text_built(void *cell, char *b, size_t n, size_t k, int as_str, pl_tr_ctx_t *cx) {
    pl_cell_t *d = cell ? pl_deref((pl_cell_t *)cell) : (pl_cell_t *)0;
    if (d && !pl_cell_unbound(d) && ((int)d->v == DT_S || (int)d->v == DT_PLATOM)) {
        int eq = plc_atom_len(d) == k && (!k || !memcmp(plc_atom_text(d), b, k));
        if (!as_str) prolog_atom_uncarve(b, n);
        return eq;
    }
    if (as_str) { pl_cell_t c = {0}; b[k] = 0; c.v = (DTYPE_t)DT_S; c.s = b; c.slen = (uint32_t)k; return plc_unify_into_cell_cx((pl_cell_t *)cell, c, cx); }
    return plc_unify_into_cell_cx((pl_cell_t *)cell, plc_atom_built(b, n, k), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_get(const char *s, int *adv) {
    const unsigned char *u = (const unsigned char *)s;
    unsigned c = u[0];
    if (c < 0x80) { *adv = 1; return (int)c; }
    if ((c & 0xE0) == 0xC0 && (u[1] & 0xC0) == 0x80) { *adv = 2; return (int)(((c & 0x1Fu) << 6) | (u[1] & 0x3Fu)); }
    if ((c & 0xF0) == 0xE0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80) { *adv = 3; return (int)(((c & 0x0Fu) << 12) | ((u[1] & 0x3Fu) << 6) | (u[2] & 0x3Fu)); }
    if ((c & 0xF8) == 0xF0 && (u[1] & 0xC0) == 0x80 && (u[2] & 0xC0) == 0x80 && (u[3] & 0xC0) == 0x80) {
        *adv = 4;
        return (int)(((c & 0x07u) << 18) | ((u[1] & 0x3Fu) << 12) | ((u[2] & 0x3Fu) << 6) | (u[3] & 0x3Fu));
    }
    *adv = 1;
    return (int)c;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
size_t pl_u8_count_n(const char *s, size_t len) { size_t n = 0; int a; for (const char *e = s + len; s < e; s += a) { (void)rt_pl_u8_get(s, &a); n++; } return n; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_u8_put(char *o, int cp) {
    if (cp < 0) return 0;
    if (cp < 0x80) { o[0] = (char)cp; return 1; }
    if (cp < 0x800) { o[0] = (char)(0xC0 | (cp >> 6)); o[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) { o[0] = (char)(0xE0 | (cp >> 12)); o[1] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[2] = (char)(0x80 | (cp & 0x3F)); return 3; }
    o[0] = (char)(0xF0 | (cp >> 18));
    o[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char)(0x80 | ((cp >> 6) & 0x3F));
    o[3] = (char)(0x80 | (cp & 0x3F));
    return 4;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *plc_atom_op_list_text_heap(pl_cell_t *d, size_t *len) {
    extern void *rt_wsb_alloc(size_t);
    extern void *rt_wsb_realloc(void *, size_t);
    size_t oi = 0, cap = 64;
    char *buf = (char *)rt_wsb_alloc(cap);
    while (d && (int)d->v == DT_PLREF && plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(d->slen) == 2 && d->p) {
        pl_cell_t *pr = (pl_cell_t *)d->p;
        pl_cell_t *el = pl_deref(&pr[0]);
        if ((int)el->v == DT_I) {
            if (oi + 8 >= cap) { cap *= 2; buf = (char *)rt_wsb_realloc(buf, cap); }
            oi += (size_t)rt_pl_u8_put(buf + oi, (int)el->i);
        } else if (plc_is_atomlike(el)) {
            const char *cn = plc_atom_text(el);
            size_t cl = cn ? plc_atom_len(el) : 0;
            if (!cn || !cl) return (const char *)0;
            while (oi + cl + 1 > cap) cap *= 2;
            buf = (char *)rt_wsb_realloc(buf, cap);
            memcpy(buf + oi, cn, cl);
            oi += cl;
        } else return (const char *)0;
        d = pl_deref(&pr[1]);
    }
    if (!(d && plc_is_atomlike(d) && plc_atom_id_of(d) == g_runtime.atoms.atom_nil)) return (const char *)0;
    buf[oi] = '\0';
    *len = oi;
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_atom_op_cell(const char *fn, void *a0_cell, void *a1_cell, void *a2_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *t0 = a0_cell ? pl_deref((pl_cell_t *)a0_cell) : (pl_cell_t *)0;
    pl_cell_t *t1 = a1_cell ? pl_deref((pl_cell_t *)a1_cell) : (pl_cell_t *)0;
    pl_cell_t *t2 = a2_cell ? pl_deref((pl_cell_t *)a2_cell) : (pl_cell_t *)0;
    char buf0[512], buf1[512];
    if (!strcmp(fn, "atom_length")) {
        size_t sn;
        const char *s;
        if (rt_pl_dialect_swi() && (int)t0->v == DT_PLREF && plc_fid_name(t0->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(t0->slen) == 2) {
            int64_t k = 0;
            for (pl_cell_t *c = t0; (int)c->v == DT_PLREF && plc_fid_name(c->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(c->slen) == 2; c = pl_deref(&((pl_cell_t *)c->p)[1])) k++;
            return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int(k), cx) ? 1 : 0;
        }
        s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count_n(s, sn)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "atomic_concat")) {
        size_t l0, l1;
        const char *s0 = plc_atom_op_text_n(t0, buf0, sizeof buf0, &l0);
        const char *s1 = plc_atom_op_text_n(t1, buf1, sizeof buf1, &l1);
        if (!s0 || !s1) { return 0; }
        {
            char *cat = plc_text_carve(l0 + l1, 0, cx);
            if (!cat) { return 0; }
            memcpy(cat, s0, l0);
            memcpy(cat + l0, s1, l1);
            return plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_atom_built(cat, l0 + l1, l0 + l1), cx) ? 1 : 0;
        }
    }
    if (!strcmp(fn, "atom_concat")) {
        size_t l0, l1;
        const char *s0 = plc_atom_op_text_n(t0, buf0, sizeof buf0, &l0);
        const char *s1 = plc_atom_op_text_n(t1, buf1, sizeof buf1, &l1);
        if (!s0 || !s1) {
            char bufz[512];
            size_t lz;
            const char *sz = plc_atom_op_text_n(t2, bufz, sizeof bufz, &lz);
            if (!sz) { return 0; }
            {
                if (s0 && !s1) { if (l0 > lz || memcmp(sz, s0, l0)) { return 0; } return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_make_atom_cell_n(sz + l0, lz - l0), cx) ? 1 : 0; }
                if (!s0 && s1) { if (l1 > lz || memcmp(sz + (lz - l1), s1, l1)) { return 0; } return plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell_n(sz, lz - l1), cx) ? 1 : 0; }
            }
            return 0;
        }
        char *cat = plc_text_carve(l0 + l1, 0, cx);
        if (!cat) { return 0; }
        memcpy(cat, s0, l0);
        memcpy(cat + l0, s1, l1);
        if (!plc_unify_into_cell_cx((pl_cell_t *)a2_cell, plc_atom_built(cat, l0 + l1, l0 + l1), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "upcase_atom") || !strcmp(fn, "downcase_atom")) {
        size_t n;
        const char *s = plc_atom_op_text_n(t0, buf0, sizeof buf0, &n);
        if (!s) { return 0; }
        char *out = plc_text_carve(n, 0, cx);
        int up = (!strcmp(fn, "upcase_atom"));
        if (!out) { return 0; }
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, plc_atom_built(out, n, n), cx)) { return 0; }
        return 1;
    }
    int as_codes = (!strcmp(fn, "atom_codes") || !strcmp(fn, "string_codes"));
    if (!strcmp(fn, "atom_chars") || !strcmp(fn, "string_chars") || as_codes) {
        int swi = !strncmp(fn, "string_", 7);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        if (t0 && !pl_cell_unbound(t0)) {
            size_t sn;
            const char *s = swi ? plc_text_nil_empty(t0, buf0, sizeof buf0, &sn) : plc_atom_op_text_n(t0, buf0, sizeof buf0, &sn);
            if (!s) { if (swi && cx && !cx->ball) cx->ball = rt_pl_ball_kind2("type_error", "text", *t0); return 0; }
            if (t1 && (int)t1->v == DT_S) return plc_atom_len(t1) == sn && (!sn || !memcmp(plc_atom_text(t1), s, sn));
            if (rt_pl_dialect_swi() && t1 && (int)t1->v == DT_PLREF && plc_fid_name(t1->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(t1->slen) == 2 && t1->p) {
                pl_cell_t *c = t1, *e0 = pl_deref(&((pl_cell_t *)t1->p)[0]);
                while (c && (int)c->v == DT_PLREF && plc_fid_name(c->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(c->slen) == 2 && c->p) {
                    if (pl_cell_unbound(pl_deref(&((pl_cell_t *)c->p)[0]))) break;
                    c = pl_deref(&((pl_cell_t *)c->p)[1]);
                }
                if (c && plc_is_atomlike(c) && plc_atom_id_of(c) == g_runtime.atoms.atom_nil) as_codes = (int)e0->v == DT_I;
            }
            int *cps = (int *)rt_wsb_alloc((sn + 1) * sizeof(int));
            int cn = 0, adv;
            for (const char *q = s; q < s + sn; q += adv) { cps[cn] = rt_pl_u8_get(q, &adv); cn++; }
            pl_cell_t lst = plc_make_atom_cell("[]");
            for (int i = cn; i > 0; i--) {
                pl_cell_t el;
                if (as_codes) el = pl_make_int((int64_t)cps[i - 1]);
                else { char cs[8]; int bl = rt_pl_u8_put(cs, cps[i - 1]); cs[bl] = '\0'; el = plc_make_atom_cell_n(cs, (size_t)bl); }
                pl_cell_t *pair = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
                pair[0] = el;
                pair[1] = lst;
                lst = pl_make_compound(g_runtime.atoms.atom_dot, 2, pair);
            }
            if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, lst, cx)) { return 0; }
            return 1;
        }
        if (t1 && (int)t1->v == DT_S) return plc_text_out_n(a0_cell, plc_atom_text(t1), plc_atom_len(t1), fn[0] == 's', cx);
        if (swi || rt_pl_dialect_swi()) {
            int mode = -1;
            size_t cap = 64, oi = 0;
            char *o = (char *)rt_wsb_alloc(cap);
            void *b = (void *)0;
            for (pl_cell_t *c = t1; !b; c = pl_deref(&((pl_cell_t *)c->p)[1])) {
                if (!c || pl_cell_unbound(c)) { b = rt_pl_ball_instantiation(); break; }
                if (plc_is_atomlike(c) && plc_atom_id_of(c) == g_runtime.atoms.atom_nil) break;
                if (!((int)c->v == DT_PLREF && plc_fid_name(c->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(c->slen) == 2 && c->p)) { b = rt_pl_ball_kind2("type_error", "list", *t1); break; }
                pl_cell_t *el = pl_deref(&((pl_cell_t *)c->p)[0]);
                int cp = 0, adv = 0;
                const char *cn = ((int)el->v == DT_PLATOM || (int)el->v == DT_S) ? plc_atom_text(el) : (const char *)0;
                size_t cnl = cn ? plc_atom_len(el) : 0;
                if (pl_cell_unbound(el)) { b = rt_pl_ball_instantiation(); break; }
                if (mode < 0) mode = (int)el->v == DT_I ? 0 : 1;
                if (mode == 0 && (int)el->v != DT_I) { b = rt_pl_ball_kind2("type_error", "character_code", *el); break; }
                if (mode == 1 && (!cn || !cnl || ((cp = rt_pl_u8_get(cn, &adv)), (size_t)adv != cnl))) { b = rt_pl_ball_kind2("type_error", "character", *el); break; }
                if (mode == 0) cp = (int)el->i;
                if (oi + 8 >= cap) { cap *= 2; o = (char *)rt_wsb_realloc(o, cap); }
                oi += (size_t)rt_pl_u8_put(o + oi, cp);
            }
            if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
            o[oi] = '\0';
            return plc_text_out_n(a0_cell, o, oi, swi, cx);
        }
        pl_cell_t *cur = t1;
        size_t ocap = 256;
        char *out = (char *)rt_wsb_alloc(ocap);
        size_t oi = 0;
        while (cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == g_runtime.atoms.atom_dot && plc_fid_arity(cur->slen) == 2) {
            pl_cell_t *pr = (pl_cell_t *)cur->p;
            pl_cell_t *el = pl_deref(&pr[0]);
            if (as_codes) {
                if ((int)el->v != DT_I) { return 0; }
                if (oi + 8 >= ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); }
                oi += (size_t)rt_pl_u8_put(out + oi, (int)el->i);
            } else {
                if ((int)el->v != DT_PLATOM && (int)el->v != DT_S) { return 0; }
                const char *cn = plc_atom_text(el);
                if (!cn) {
                    if (oi + 2 > ocap) { ocap *= 2; out = (char *)rt_wsb_realloc(out, ocap); }
                    out[oi++] = '?';
                } else {
                    size_t cl = plc_atom_len(el);
                    while (oi + cl + 1 > ocap) ocap *= 2;
                    out = (char *)rt_wsb_realloc(out, ocap);
                    memcpy(out + oi, cn, cl);
                    oi += cl;
                }
            }
            cur = pl_deref(&pr[1]);
        }
        out[oi] = '\0';
        return plc_text_out_n(a0_cell, out, oi, fn[0] == 's', cx);
    }
    if (!strcmp(fn, "string_length")) {
        size_t sn;
        const char *s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a1_cell, pl_make_int((int64_t)pl_u8_count_n(s, sn)), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_upper") || !strcmp(fn, "string_lower")) {
        const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
        if (!s) { return 0; }
        size_t n = strlen(s);
        char *out = plc_text_carve(n, 1, cx);
        int up = (!strcmp(fn, "string_upper"));
        if (!out) { return 0; }
        for (size_t i = 0; i < n; i++) out[i] = up ? (char)toupper((unsigned char)s[i]) : (char)tolower((unsigned char)s[i]);
        return plc_text_built(a1_cell, out, n, n, 1, cx);
    }
    if (!strcmp(fn, "atom_string") || !strcmp(fn, "string_to_atom")) {
        int a0_str = !strcmp(fn, "string_to_atom");
        size_t sn;
        if (t0 && !pl_cell_unbound(t0)) { const char *s = plc_text_nil_empty(t0, buf0, sizeof buf0, &sn); if (!s) { return 0; } return plc_text_out_n(a1_cell, s, sn, !a0_str, cx); }
        const char *s = plc_atom_op_text_n(t1, buf1, sizeof buf1, &sn);
        if (!s) { return 0; }
        return plc_text_out_n(a0_cell, s, sn, a0_str, cx);
    }
    if (!strcmp(fn, "number_string")) {
        if (t0 && !pl_cell_unbound(t0) && (!t1 || pl_cell_unbound(t1))) { const char *s = plc_atom_op_text(t0, buf0, sizeof buf0); if (!s) { return 0; } return plc_text_out(a1_cell, s, 1, cx); }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        { pl_cell_t nv; if (!plc_num_of_text(s, &nv)) return 0; return plc_unify_into_cell_cx((pl_cell_t *)a0_cell, nv, cx) ? 1 : 0; }
    }
    if (!strcmp(fn, "atom_number")) {
        if (t0 && !pl_cell_unbound(t0)) {
            const char *s = plc_atom_op_text(t0, buf0, sizeof buf0);
            if (!s) { return 0; }
            { pl_cell_t nv; if (!plc_num_of_text(s, &nv)) return 0; return plc_unify_into_cell_cx((pl_cell_t *)a1_cell, nv, cx) ? 1 : 0; }
        }
        const char *s = plc_atom_op_text(t1, buf1, sizeof buf1);
        if (!s) { return 0; }
        if (!plc_unify_into_cell_cx((pl_cell_t *)a0_cell, plc_make_atom_cell(s), cx)) { return 0; }
        return 1;
    }
    if (!strcmp(fn, "string_concat")) {
        const char *s0 = plc_atom_op_text(t0, buf0, sizeof buf0);
        const char *s1 = plc_atom_op_text(t1, buf1, sizeof buf1);
        int u0 = !t0 || pl_cell_unbound(t0), u1 = !t1 || pl_cell_unbound(t1);
        if (u0 != u1 && t2 && !pl_cell_unbound(t2)) {
            char buf2[512];
            const char *s2 = plc_atom_op_text(t2, buf2, sizeof buf2);
            if (!s2 || (u0 ? !s1 : !s0)) { return 0; }
            size_t l2 = strlen(s2);
            if (!u0) { size_t l0 = strlen(s0); return (l0 <= l2 && !memcmp(s2, s0, l0)) ? plc_text_out(a1_cell, s2 + l0, 1, cx) : 0; }
            {
                size_t l1 = strlen(s1);
                char *pre;
                if (l1 > l2 || memcmp(s2 + l2 - l1, s1, l1)) { return 0; }
                pre = plc_text_carve(l2 - l1, 1, cx);
                if (!pre) { return 0; }
                memcpy(pre, s2, l2 - l1);
                return plc_text_built(a0_cell, pre, l2 - l1, l2 - l1, 1, cx);
            }
        }
        if (!s0 || !s1) { return 0; }
        size_t l0 = strlen(s0), l1 = strlen(s1);
        char *cat = plc_text_carve(l0 + l1, 1, cx);
        if (!cat) { return 0; }
        memcpy(cat, s0, l0);
        memcpy(cat + l0, s1, l1);
        return plc_text_built(a2_cell, cat, l0 + l1, l0 + l1, 1, cx);
    }
    if (!strcmp(fn, "atomic_list_concat") || !strcmp(fn, "concat_atom")) {
        const char *sep = (t2 && t1 && ((int)t1->v == DT_PLATOM || (int)t1->v == DT_S)) ? plc_atom_text(t1) : "";
        void *result_cell = t2 ? a2_cell : a1_cell;
        size_t sl = (sep && sep[0]) ? strlen(sep) : 0, need = 1;
        long ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0);
            if (!es) { return 0; }
            need += strlen(es) + (ne++ ? sl : 0);
        }
        char *out = plc_text_carve(need - 1, 0, cx);
        size_t oi = 0;
        if (!out) { return 0; }
        ne = 0;
        for (pl_cell_t *lst = t0; lst && (int)lst->v == DT_PLREF && plc_fid_arity(lst->slen) == 2; lst = pl_deref(&((pl_cell_t *)lst->p)[1])) {
            const char *es = plc_atom_op_text(pl_deref(&((pl_cell_t *)lst->p)[0]), buf0, sizeof buf0);
            size_t el_len = strlen(es);
            if (ne++ && sl) { memcpy(out + oi, sep, sl); oi += sl; }
            memcpy(out + oi, es, el_len);
            oi += el_len;
        }
        if (!plc_unify_into_cell_cx((pl_cell_t *)result_cell, plc_atom_built(out, need - 1, oi), cx)) { return 0; }
        return 1;
    }
    (void)t2;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_fmt_is_atom(pl_cell_t *d) { return d && ((int)d->v == DT_PLATOM || (int)d->v == DT_S) && !pl_cell_unbound(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_format_cell(const char *fmt, void *list_cell) { (void)rt_pl_format_run(fmt, list_cell, (pl_tr_ctx_t *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_latin(int c) {
    static const char sy_hi[] = "\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAB\xAC\xAE\xAF\xB0\xB1\xB4\xB6\xB7\xB8\xBB\xBF\xD7\xF7";
    if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7)) return 'U';
    if ((c >= 'a' && c <= 'z') || c == 0xAA || c == 0xB5 || c == 0xBA || (c >= 0xDF && c <= 0xFF && c != 0xF7)) return 'L';
    if (c >= '0' && c <= '9') return 'D';
    if (c > 0 && c < 0x80 && strchr("#$&*+-./:<=>?@\\^~", c)) return 'S';
    if (c >= 0x80 && c <= 0xFF && memchr(sy_hi, c, sizeof sy_hi - 1)) return 'S';
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_mark(int c) {
    static const int mk[][2] = { { 0x300, 0x36F }, { 0x483, 0x489 }, { 0x591, 0x5BD }, { 0x5BF, 0x5BF }, { 0x5C1, 0x5C2 }, { 0x5C4, 0x5C5 }, { 0x5C7, 0x5C7 }, { 0x610, 0x61A }, { 0x64B, 0x65F },
        { 0x670, 0x670 }, { 0x6D6, 0x6DC }, { 0x6DF, 0x6E4 }, { 0x6E7, 0x6E8 }, { 0x6EA, 0x6ED }, { 0x1AB0, 0x1AFF }, { 0x1DC0, 0x1DFF }, { 0x20D0, 0x20F0 }, { 0xFE20, 0xFE2F } };
    for (size_t k = 0; k < sizeof mk / sizeof *mk; k++) if (c >= mk[k][0] && c <= mk[k][1]) return (c >= 0x488 && c <= 0x489) || (c >= 0x20DD && c <= 0x20E0) || (c >= 0x20E2 && c <= 0x20E4) ? 2 : 1;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_class0(const char *ty, int cp, locale_t l) {
    extern int prolog_u_nd_zero(unsigned);
    wint_t w = (wint_t)cp;
    int lt = cp <= 0xFF ? pl_ct_latin(cp) : 0;
    if (!strcmp(ty, "alnum")) return iswalnum_l(w, l) != 0;
    if (!strcmp(ty, "alpha")) return iswalpha_l(w, l) != 0;
    if (!strcmp(ty, "csym")) return iswalnum_l(w, l) || cp == '_';
    if (!strcmp(ty, "csymf")) return iswalpha_l(w, l) || cp == '_';
    if (!strcmp(ty, "prolog_var_start")) return cp <= 0xFF ? lt == 'U' : iswupper_l(w, l) != 0;
    if (!strcmp(ty, "prolog_atom_start")) return cp <= 0xFF ? lt == 'L' : (iswalpha_l(w, l) && !iswupper_l(w, l));
    if (!strcmp(ty, "prolog_identifier_continue")) return cp <= 0xFF ? (lt == 'U' || lt == 'L' || lt == 'D') : (iswalnum_l(w, l) || pl_ct_mark(cp) == 1);
    if (!strcmp(ty, "decimal")) return cp <= 0xFF ? lt == 'D' : prolog_u_nd_zero((unsigned)cp) >= 0;
    if (!strcmp(ty, "prolog_symbol")) return cp <= 0xFF ? lt == 'S' : (iswpunct_l(w, l) && !pl_ct_mark(cp));
    if (!strcmp(ty, "ascii")) return cp < 0x80;
    if (!strcmp(ty, "white")) return cp == ' ' || cp == '\t';
    if (!strcmp(ty, "cntrl")) return iswcntrl_l(w, l) != 0;
    if (!strcmp(ty, "digit")) return iswdigit_l(w, l) != 0;
    if (!strcmp(ty, "print")) return iswprint_l(w, l) != 0;
    if (!strcmp(ty, "graph")) return iswgraph_l(w, l) != 0;
    if (!strcmp(ty, "lower")) return iswlower_l(w, l) != 0;
    if (!strcmp(ty, "upper")) return iswupper_l(w, l) != 0;
    if (!strcmp(ty, "punct")) return iswpunct_l(w, l) != 0;
    if (!strcmp(ty, "space")) return iswspace_l(w, l) != 0;
    if (!strcmp(ty, "end_of_file")) return 0;
    if (!strcmp(ty, "end_of_line")) return cp >= 10 && cp <= 13;
    if (!strcmp(ty, "newline")) return cp == 10;
    if (!strcmp(ty, "period")) return cp && strchr(".?!", (char)cp) != (char *)0;
    if (!strcmp(ty, "quote")) return cp && strchr("'`\"", (char)cp) != (char *)0;
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_class1(const char *ty, int cp, int rev, int *chr, locale_t l) {
    extern int prolog_u_nd_zero(unsigned);
    wint_t w = (wint_t)cp;
    *chr = 1;
    if (!strcmp(ty, "lower")) return rev ? (iswupper_l(w, l) ? (int)towlower_l(w, l) : -1) : (iswlower_l(w, l) ? (int)towupper_l(w, l) : -1);
    if (!strcmp(ty, "upper")) return rev ? (iswlower_l(w, l) ? (int)towupper_l(w, l) : -1) : (iswupper_l(w, l) ? (int)towlower_l(w, l) : -1);
    if (!strcmp(ty, "to_lower")) return rev ? (int)towlower_l(w, l) : (int)towupper_l(w, l);
    if (!strcmp(ty, "to_upper")) return rev ? (int)towupper_l(w, l) : (int)towlower_l(w, l);
    if (!strcmp(ty, "paren")) {
        static const char op[] = "({[", cl[] = ")}]";
        const char *f = cp > 0 && cp < 0x80 ? strchr(rev ? cl : op, cp) : (const char *)0;
        return f ? (int)(unsigned char)(rev ? op : cl)[f - (rev ? cl : op)] : -1;
    }
    *chr = 0;
    if (!strcmp(ty, "digit")) return rev ? -2 : (cp >= '0' && cp <= '9' ? cp - '0' : -1);
    if (!strcmp(ty, "xdigit")) return rev ? -2 : (cp >= '0' && cp <= '9' ? cp - '0' : cp >= 'a' && cp <= 'f' ? cp - 'a' + 10 : cp >= 'A' && cp <= 'F' ? cp - 'A' + 10 : -1);
    if (!strcmp(ty, "decimal")) return rev ? -2 : (cp >= '0' && cp <= '9' ? cp - '0' : cp > 0xFF && prolog_u_nd_zero((unsigned)cp) >= 0 ? cp - prolog_u_nd_zero((unsigned)cp) : -1);
    if (!strcmp(ty, "code")) return rev ? -2 : cp;
    return -3;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_code(pl_cell_t *c, int *as_char) {
    char b[16];
    const char *s;
    int adv, cp;
    if ((int)c->v == DT_I) { *as_char = 0; return (c->i >= 0 && c->i <= 0x10FFFF) ? (int)c->i : -2; }
    *as_char = 1;
    s = plc_fmt_is_atom(c) ? plc_atom_op_text(c, b, sizeof b) : (const char *)0;
    if (!s || !s[0]) return -2;
    cp = rt_pl_u8_get(s, &adv);
    return (cp >= 0 && !s[adv]) ? cp : -2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static pl_cell_t pl_ct_out(int v, int chr, int as_char) { char b[8]; if (!chr || !as_char) return pl_make_int((int64_t)v); b[rt_pl_u8_put(b, v)] = 0; return plc_make_atom_cell(b); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_ct_scan(pl_cell_t *tc, pl_cell_t *td, pl_tr_ctx_t *cx) {
    pl_cell_t *a = (pl_cell_t *)pl_compound_heap(td), *t = pl_deref(&a[0]), *h = pl_deref(&a[1]), *arg = (pl_cell_t *)0;
    const char *hs = plc_fmt_is_atom(h) ? plc_atom_text(h) : (const char *)0, *ty;
    int code_mode = hs && !strcmp(hs, "code"), ia = 1, ac = -2, c, hit = -1, bind = 0, br = 0, bchr = 0;
    locale_t l;
    if ((int)tc->v != DT_I || tc->i < 0) return 0;
    if ((int)t->v == DT_PLREF && pl_arity(t) == 1) {
        ty = prolog_atom_name(plc_functor(t));
        arg = &((pl_cell_t *)pl_compound_heap(t))[0];
        if (!pl_cell_unbound(pl_deref(arg))) ac = pl_ct_code(pl_deref(arg), &ia);
    } else if ((int)t->v == DT_PLATOM) ty = prolog_atom_name(pl_atom_id(t));
    else return 0;
    if (!ty) return 0;
    l = newlocale(LC_CTYPE_MASK, "C.UTF-8", (locale_t)0);
    if (!l) return 0;
    for (c = (int)tc->i; c <= 0x10FFFF && hit < 0; c++) {
        int chr, r, k;
        if (!arg) { k = pl_ct_class0(ty, c, l); if (k < 0) break; if (k) hit = c; continue; }
        r = pl_ct_class1(ty, c, 0, &chr, l);
        if (r == -3) break;
        if (r < 0) continue;
        if (ac == -2) { hit = c; bind = 1; br = r; bchr = chr; continue; }
        if (ac == r || (chr && ac >= 0 && pl_ct_class1(ty, ac, 1, &k, l) == c)) hit = c;
    }
    freelocale(l);
    if (hit < 0 || !plc_unify_into_cell_cx(&a[2], pl_make_int((int64_t)hit), cx)) return 0;
    return !bind || plc_unify_into_cell_cx(arg, pl_ct_out(br, bchr, !code_mode), cx);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_char_type_ball(void *char_cell, void *type_cell) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    pl_cell_t *tc = pl_deref((pl_cell_t *)char_cell), *td = pl_deref((pl_cell_t *)type_cell);
    const char *ty;
    int as_char, chr, known;
    locale_t l;
    if ((int)td->v == DT_PLREF && pl_arity(td) == 3 && !strcmp(prolog_atom_name(plc_functor(td)), "$ct_from")) return (void *)0;
    if (pl_cell_unbound(td)) return (void *)0;
    ty = (int)td->v == DT_PLATOM ? prolog_atom_name(pl_atom_id(td)) : ((int)td->v == DT_PLREF && pl_arity(td) == 1) ? prolog_atom_name(plc_functor(td)) : (const char *)0;
    l = ty ? newlocale(LC_CTYPE_MASK, "C.UTF-8", (locale_t)0) : (locale_t)0;
    known = l && ((int)td->v == DT_PLATOM ? pl_ct_class0(ty, 'a', l) >= 0 : pl_ct_class1(ty, 'a', 0, &chr, l) != -3);
    if (l) freelocale(l);
    if (!known) return rt_pl_ball_kind2("type_error", "char_type", *td);
    if (!pl_cell_unbound(tc) && pl_ct_code(tc, &as_char) < 0 && !((int)tc->v == DT_I && tc->i == -1)) return rt_pl_ball_kind2("type_error", "character", *tc);
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_char_type_cell(void *char_cell, void *type_cell, void *val_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *tc = char_cell ? pl_deref((pl_cell_t *)char_cell) : (pl_cell_t *)0, *td, *inner;
    const char *ty;
    int as_char = 1, cp, r, chr, ok;
    locale_t l;
    if (!tc || !type_cell) return 0;
    td = pl_deref((pl_cell_t *)type_cell);
    if ((int)td->v == DT_PLREF && pl_arity(td) == 3 && !strcmp(prolog_atom_name(plc_functor(td)), "$ct_from")) return pl_ct_scan(tc, td, cx);
    if ((int)td->v == DT_PLREF && pl_arity(td) == 1) {
        ty = prolog_atom_name(plc_functor(td));
        inner = val_cell ? (pl_cell_t *)val_cell : &((pl_cell_t *)pl_compound_heap(td))[0];
        if (!ty) return 0;
        l = newlocale(LC_CTYPE_MASK, "C.UTF-8", (locale_t)0);
        if (!l) return 0;
        if (pl_cell_unbound(tc)) {
            int ia = 1, ac = pl_cell_unbound(pl_deref(inner)) ? -2 : pl_ct_code(pl_deref(inner), &ia);
            r = ac >= 0 ? pl_ct_class1(ty, ac, 1, &chr, l) : -1;
            freelocale(l);
            return r >= 0 && plc_unify_into_cell_cx(tc, pl_ct_out(r, 1, ia), cx);
        }
        cp = pl_ct_code(tc, &as_char);
        r = cp >= 0 ? pl_ct_class1(ty, cp, 0, &chr, l) : -1;
        if (r < 0) { freelocale(l); return 0; }
        if (!pl_cell_unbound(pl_deref(inner)) && chr) {
            int ia = 1, ac = pl_ct_code(pl_deref(inner), &ia), rv;
            if (ac == r) { freelocale(l); return 1; }
            rv = ac >= 0 ? pl_ct_class1(ty, ac, 1, &chr, l) : -1;
            freelocale(l);
            return rv >= 0 && rv == cp;
        }
        freelocale(l);
        return plc_unify_into_cell_cx(inner, pl_ct_out(r, chr, as_char), cx) ? 1 : 0;
    }
    if ((int)td->v != DT_PLATOM && (int)td->v != DT_S) return 0;
    ty = ((int)td->v == DT_S) ? (td->s ? td->s : "") : prolog_atom_name(pl_atom_id(td));
    cp = ty ? pl_ct_code(tc, &as_char) : -2;
    if (cp < 0) return 0;
    l = newlocale(LC_CTYPE_MASK, "C.UTF-8", (locale_t)0);
    if (!l) return 0;
    ok = pl_ct_class0(ty, cp, l);
    freelocale(l);
    return ok > 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_lower_upper_cell(void *lower_cell, void *upper_cell, pl_tr_ctx_t *cx) {
    if (!lower_cell || !upper_cell) return 0;
    pl_cell_t *lo = pl_deref((pl_cell_t *)lower_cell);
    pl_cell_t *up = pl_deref((pl_cell_t *)upper_cell);
    int lo_bound = !pl_cell_unbound(lo), up_bound = !pl_cell_unbound(up);
    if (!lo_bound && !up_bound) { extern void rt_pl_iso_throw_instantiation(void); rt_pl_iso_throw_instantiation(); return 0; }
    char b0[256];
    if (lo_bound) {
        const char *cs = plc_atom_op_text(lo, b0, sizeof b0);
        if (!cs || !cs[0] || cs[1]) return 0;
        char c2[2] = { (char)toupper((unsigned char)cs[0]), 0 };
        if (!plc_unify_into_cell_cx(up, plc_make_atom_cell(c2), cx)) { return 0; }
        return 1;
    }
    const char *cs = plc_atom_op_text(up, b0, sizeof b0);
    if (!cs || !cs[0] || cs[1]) return 0;
    char c2[2] = { (char)tolower((unsigned char)cs[0]), 0 };
    if (!plc_unify_into_cell_cx(lo, plc_make_atom_cell(c2), cx)) { return 0; }
    return 1;
}
void *rt_pl_cap_error(void) { return rt_pl_ball_kind1("resource_error", "stack"); }
void *rt_pl_cap_ball(void) {
    extern void rt_gcheap_cap_raised(void);
    void *b = rt_pl_cap_error();
    if (!b) { fprintf(stderr, "[ZHP] REFUSED: the resource error at the hard cap could not be built from the error reserve\n"); exit(2); }
    rt_gcheap_cap_raised();
    return b;
}
void *rt_pl_alarm_ball(void) { pl_cell_t *b = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t)); if (b) *b = pl_make_atom(prolog_atom_intern("time_limit_exceeded")); return (void *)b; }
