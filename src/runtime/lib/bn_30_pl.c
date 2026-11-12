#include "runtime/lib/bn_head.h"
#include <math.h>
#include "runtime/rt/prolog_atom.h"
#include "runtime/core/utf8.h"
#include "runtime/pl_rat.h"
#include <errno.h>
#include "runtime/rt_pl_reader.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_06_pl.h"
#include "runtime/lib/bn_26_sh.h"
#include "runtime/lib/bn_27_pl.h"
#include "runtime/lib/bn_30_pl.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_33_pl.h"
int rt_pl_term_variables_cell(void *, void *, void *, pl_tr_ctx_t *);
int rt_pl_numbervars_cell(void *, void *, void *, pl_tr_ctx_t *);
extern int rt_plco_park(int kind, DESCR_t r, char **tr, long *gen, long gen_in, char *b);
extern DESCR_t rt_plco_after_start(void *h, DESCR_t r, char **tr, long *gen_out, int *st, DESCR_t *hd);
extern DESCR_t rt_plco_resume(DESCR_t h, long cmd, long gen, char **tr, long *gen_out, int *st);
extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
extern char * pl_tr_base_of(const char *);
extern int rt_pl_db_decl(void *, const char *, int64_t, int64_t);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *pl_atom_str_n(DESCR_t v, size_t *n) {
    extern const char *prolog_atom_name(int);
    extern size_t prolog_atom_len(int);
    const char *nm;
    *n = 0;
    if (v.v == DT_S) { *n = v.s ? v.slen : 0; return v.s ? v.s : ""; }
    if (v.v != (DTYPE_t)DT_PLATOM) return NULL;
    nm = prolog_atom_name((int)v.i);
    if (nm) *n = prolog_atom_len((int)v.i);
    return nm ? nm : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_is_nil(DESCR_t v) {
    if (v.v == DT_S || v.v == DT_SNUL) return v.s && !strcmp(v.s, "[]");
    if (v.v == (DTYPE_t)DT_PLATOM) { extern const char *prolog_atom_name(int); const char *nm = prolog_atom_name((int)v.i); return nm && !strcmp(nm, "[]"); }
    return 0;
}
int pl_cell_text_n(DESCR_t v, char *buf, size_t n, const char **out, size_t *len) {
    DESCR_t d = rt_pl_deref_val(v);
    const char *s = pl_atom_str_n(d, len);
    if (s) { *out = s; return 1; }
    if (d.v == DT_I) { snprintf(buf, n, "%lld", (long long)d.i); *out = buf; *len = strlen(buf); return 1; }
    if (d.v == DT_BIG) { extern char *rt_big_str(DESCR_t); *out = rt_big_str(d); if (*out) *len = strlen(*out); return *out != (const char *)0; }
    if (d.v == DT_R) { *out = pl_real_iso_str(d.r, buf, (int)n); *len = strlen(*out); return 1; }
    if (pl_rat_is(d)) {
        extern void *rt_wsb_alloc(size_t);
        size_t m = pl_rat_text(d, (char *)0, 0) + 1;
        char *rb = (char *)rt_wsb_alloc(m);
        if (!rb) return 0;
        (void)pl_rat_text(d, rb, m);
        *out = rb;
        *len = m - 1;
        return 1;
    }
    if (pl_is_cons(d) || pl_is_nil(d)) { const char *t = pl_list_text_heap(d); if (!t) return 0; *out = t; *len = strlen(t); return 1; }
    return 0;
}
static int pl_parse_number_radix(const char *s, DESCR_t *out) {
    char *e = 0;
    int base = 0;
    if (s[0] == '0' && s[1] == '\'') {
        if (s[2] == '\'' && ((s[3] == '\'' && !s[4]) || !s[3])) { *out = INTVAL(39); return 1; }
        if (s[2] == '\\' && s[3] && !s[4]) {
            int c = s[3];
            c = (c == 'n') ? 10 : (c == 't') ? 9 : (c == 'r') ? 13 : (c == 'a') ? 7 : (c == 'b') ? 8 : (c == 'f') ? 12 : (c == 'v') ? 11 : (c == '0') ? 0 : (c == '\\') ? 92 : (c == '\'') ? 39 :
                (c == '"') ? 34 : (c == '`') ? 96 : -1;
            if (c < 0) return 0;
            *out = INTVAL(c);
            return 1;
        }
        if (s[2]) { int adv = 0; int cp = rt_pl_u8_get(s + 2, &adv); if (adv > 0 && !s[2 + adv]) { *out = INTVAL(cp); return 1; } }
        return 0;
    }
    if (s[0] == '0' && s[1] == 'x') base = 16;
    else if (s[0] == '0' && s[1] == 'o') base = 8;
    else if (s[0] == '0' && s[1] == 'b') base = 2;
    if (!base) return 0;
    {
        long long v;
        errno = 0;
        v = strtoll(s + 2, &e, base);
        if (e && e != s + 2 && !*e) {
            if (errno == ERANGE) { extern DESCR_t rt_big_from_str_base(const char *, int); DESCR_t bg = rt_big_from_str_base(s + 2, base); if (bg.v == DT_FAIL) return 0; *out = bg; return 1; }
            *out = INTVAL(v);
            return 1;
        }
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char * pl_skip_layout(const char *s) { if (!s) return s; while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r' || *s == '\f' || *s == '\v') s++; return s; }
int rt_pl_parse_number(const char *s, DESCR_t *out) { return pl_parse_number(s, out); }
static int pl_parse_rational(const char *s, DESCR_t *out) {
    const char *p = pl_skip_layout(s), *q = p + (*p == '-');
    char b[strlen(p) + 1];
    size_t k = 0;
    int r = 0, dig = 0;
    if (*p == '-') b[k++] = '-';
    for (; *q; q++) {
        if (*q >= '0' && *q <= '9') { b[k++] = *q; dig = 1; continue; }
        if (*q == '_' && dig && q[1] >= '0' && q[1] <= '9') continue;
        if (*q == 'r' && dig && !r && q[1] >= '0' && q[1] <= '9') { b[k++] = 'r'; r = 1; dig = 0; continue; }
        return 0;
    }
    if (!r || !dig) return 0;
    b[k] = 0;
    { DESCR_t v = pl_rat_parse(b); if (v.v == DT_FAIL) return 0; *out = v; return 1; }
}
static int pl_nd_zero(unsigned c) { extern int prolog_u_nd_zero(unsigned); return prolog_u_nd_zero(c); }
static int pl_digits_to_ascii(const char *s, char *b) {
    const unsigned char *q = (const unsigned char *)s;
    int zero = -1, ascii = 0, wide = 0;
    size_t k = 0;
    while (*q) {
        unsigned c = *q;
        int n = c < 0x80 ? 1 : c < 0xE0 ? 2 : c < 0xF0 ? 3 : 4;
        if (n > 1) {
            c &= n == 2 ? 0x1F : n == 3 ? 0x0F : 0x07;
            for (int i = 1; i < n; i++) { if ((q[i] & 0xC0) != 0x80) return 0; c = (c << 6) | (q[i] & 0x3F); }
            int z = pl_nd_zero(c);
            if (z < 0 || (zero >= 0 && z != zero)) return 0;
            zero = z;
            wide = 1;
            b[k++] = (char)('0' + (c - (unsigned)z));
        } else {
            if (c >= '0' && c <= '9') ascii = 1;
            if (!strchr("0123456789._eEr+-", (int)c)) return 0;
            b[k++] = (char)c;
        }
        q += n;
    }
    b[k] = 0;
    return wide && !ascii;
}
static int pl_dg_val(char c) { return (c >= '0' && c <= '9') ? c - '0' : (c >= 'a' && c <= 'z') ? c - 'a' + 10 : (c >= 'A' && c <= 'Z') ? c - 'A' + 10 : 99; }
static int pl_dg_layout(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
static int pl_digit_groups(const char *s, char *b) {
    const char *q = s;
    size_t k = 0;
    int groups = 0, radix = 10;
    while (pl_dg_layout(*q)) q++;
    if (*q == '-' || *q == '+') b[k++] = *q++;
    if (*q < '0' || *q > '9') return 0;
    if (q[0] == '0' && (q[1] == 'x' || q[1] == 'o' || q[1] == 'b')) {
        radix = q[1] == 'x' ? 16 : q[1] == 'o' ? 8 : 2;
        b[k++] = *q++;
        b[k++] = *q++;
    } else {
        const char *r = q;
        int rv = 0;
        while (*r >= '0' && *r <= '9' && rv <= 36) rv = rv * 10 + (*r++ - '0');
        if (*r == '\'' && r[1] && rv >= 2 && rv <= 36 && pl_dg_val(r[1]) < rv) { radix = rv; while (q <= r) b[k++] = *q++; }
    }
    if (pl_dg_val(*q) >= radix) return 0;
    for (; *q; q++) {
        if (pl_dg_val(*q) < radix) { b[k++] = *q; continue; }
        if (*q == '_' && pl_dg_val(q[-1]) < radix) { const char *r = q + 1; while (pl_dg_layout(*r)) r++; if (pl_dg_val(*r) >= radix) return 0; q = r - 1; groups = 1; continue; }
        if (*q == ' ' && radix <= 10 && pl_dg_val(q[-1]) < radix && pl_dg_val(q[1]) < radix) { groups = 1; continue; }
        return 0;
    }
    b[k] = 0;
    return groups;
}
int pl_parse_number(const char *s, DESCR_t *out) {
    char *e = 0;
    if (!s) return 0;
    if ((strchr(s, '_') || strchr(s, ' ')) && !pl_gnu_dialect()) { char b[strlen(s) + 1]; if (pl_digit_groups(s, b)) return pl_parse_number(b, out); }
    const char *q0 = pl_skip_layout(s);
    for (const char *w = s; *w && !(q0[0] == '0' && q0[1] == '\''); w++) {
        if (!((unsigned char)*w & 0x80)) continue;
        char b[strlen(s) + 1];
        return pl_digits_to_ascii(s, b) && pl_parse_number(b, out);
    }
    if (pl_parse_rational(s, out)) return 1;
    {
        const char *p = pl_skip_layout(s);
        if (*p && pl_parse_number_radix(p, out)) return 1;
        if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X' || p[1] == 'o' || p[1] == 'O' || p[1] == 'b' || p[1] == 'B')) return 0;
    }
    {
        long long iv;
        errno = 0;
        iv = strtoll(s, &e, 10);
        if (e && e != s && !*e) {
            if (errno == ERANGE) { extern DESCR_t rt_big_from_str(const char *); DESCR_t bg = rt_big_from_str(pl_skip_layout(s)); if (bg.v == DT_FAIL) return 0; *out = bg; return 1; }
            *out = INTVAL(iv);
            return 1;
        }
    }
    {
        double dv = strtod(s, &e);
        if (e && e != s && !*e) {
            const char *b = pl_skip_layout(s);
            const char *d = strchr(b, '.');
            extern int rt_pl_iso_mode(void);
            if (!d && rt_pl_iso_mode()) return 0;
            if (d && (d[1] < '0' || d[1] > '9')) return 0;
            if (d && (d == b || d[-1] < '0' || d[-1] > '9')) return 0;
            *out = REALVAL(dv);
            return 1;
        }
    }
    return 0;
}
static DESCR_t pl_co_answer(DESCR_t *args, int ir, DESCR_t r, long gen, int st, pl_tr_ctx_t *cx) {
    char *tr0 = cx->tr;
    void *pend = st > 0 ? (void *)0 : *pl_tr_ball_slot(pl_tr_base_of(cx->tr));
    if (pend) { cx->ball = pend; rt_pl_tr_gc_sync(cx->tr); return FAILDESCR; }
    int ok = st > 0 && plw_unify_vals(args[ir], st == 2 ? pl_mk_cmp1("$det", r) : r, cx) && plw_unify_vals(args[ir + 1], INTVAL(gen), cx);
    if (!ok) cx->tr = rt_pl_tr_unwind_to(cx->tr, tr0);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_co_start_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t g, r, hd = {0};
    void *h = (void *)0, *ball = (void *)0;
    char *tr = cx->tr;
    long gen = 0;
    int st = 0;
    if (nargs != 4) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    g = rt_pl_deref_val(args[0]);
    r = rt_pl_goal_gen_h_c(pl_mk_atom_dup("$co_body", 8), &g, 1, &h, &ball, cx);
    if (ball) { cx->ball = ball; return FAILDESCR; }
    r = rt_plco_after_start(h, r, &tr, &gen, &st, &hd);
    cx->tr = tr;
    if (st > 0 && !plw_unify_vals(args[1], hd, cx)) st = 0;
    return pl_co_answer(args, 2, r, gen, st, cx);
}
DESCR_t rt_pl_dop_co_resume_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t h, cm, gn, r;
    char *tr = cx->tr;
    long gen = 0;
    int st = 0;
    if (nargs != 5) return FAILDESCR;
    rt_pl_tr_gc_sync(cx->tr);
    h = rt_pl_deref_val(args[0]);
    cm = rt_pl_deref_val(args[1]);
    gn = rt_pl_deref_val(args[2]);
    if (cm.v != DT_I || gn.v != DT_I) return FAILDESCR;
    r = rt_plco_resume(h, (long)cm.i, (long)gn.i, &tr, &gen, &st);
    if (st < 0) { cx->ball = rt_pl_ball_permission3("resume", "continuation", h); return FAILDESCR; }
    cx->tr = tr;
    return pl_co_answer(args, 3, r, gen, st, cx);
}
static DESCR_t pl_co_park_leaf(DESCR_t *args, int kind, long gen_in, int bind_gen, pl_tr_ctx_t *cx) {
    DESCR_t r = rt_pl_deref_val(args[kind >= 4 ? 1 : 0]);
    char *tr = cx->tr;
    long gen = 0;
    int cmd, ok;
    rt_pl_tr_gc_sync(cx->tr);
    cmd = rt_plco_park(kind, r, &tr, &gen, gen_in, cx->b);
    if (cmd < 0) { cx->ball = rt_pl_ball_permission3("backtrack", "continuation", r); return FAILDESCR; }
    cx->tr = tr;
    ok = cmd > 0 && bind_gen && plw_unify_vals(args[1], INTVAL(gen), cx);
    rt_pl_tr_gc_sync(cx->tr);
    return ok ? pl_ok() : FAILDESCR;
}
DESCR_t rt_pl_dop_co_yield_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) { return nargs == 2 ? pl_co_park_leaf(args, 2, 0, 1, cx) : FAILDESCR; }
DESCR_t rt_pl_dop_co_unwound_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t gn = nargs == 2 ? rt_pl_deref_val(args[1]) : FAILDESCR;
    return gn.v == DT_I ? pl_co_park_leaf(args, 3, (long)gn.i, 0, cx) : FAILDESCR;
}
DESCR_t rt_pl_dop_co_finish_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    DESCR_t cd = nargs == 2 ? rt_pl_deref_val(args[0]) : FAILDESCR;
    return cd.v == DT_I ? pl_co_park_leaf(args, cd.i ? 5 : 4, 0, 0, cx) : FAILDESCR;
}
PL_CX_LEAF_HEAD(term_variables, 2)
    {
        extern void *rt_pl_dop_list_guard_c(DESCR_t *, int);
        void *b = rt_pl_dop_list_guard_c(&args[1], 1);
        if (b) { cx->ball = b; ok = 0; } else ok = rt_pl_term_variables_cell(&args[0], &args[1], (void *)0, cx);
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(numbervars3, 3)
    {
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t s = rt_pl_deref_val(args[1]), e = rt_pl_deref_val(args[2]);
        ok = 0;
        if (pl_val_unbound(s)) cx->ball = rt_pl_ball_instantiation();
        else if (s.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", s);
        else if (!pl_val_unbound(e) && e.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", e);
        else ok = rt_pl_numbervars_cell(&args[0], &args[1], &args[2], cx);
    }
PL_CX_LEAF_TAIL
    PL_ATOM_OP_LEAF(string_chars, 2) static int pl_split_text(DESCR_t *args, pl_tr_ctx_t *cx) {
    char sb[256], tb[8192];
    const char *sep, *txt;
    gv_t el;
    int n = 0;
    size_t sl;
    if (!pl_cell_text(args[1], sb, sizeof sb, &sep) || !sep[0] || !pl_cell_text(args[2], tb, sizeof tb, &txt)) return 0;
    sl = strlen(sep);
    el.p = 0;
    el.len = 0;
    el.cap = 0;
    el.esz = 0;
    el.kind = 0;
    el.pad = 0;
    for (const char *p = txt;;) {
        const char *q = strstr(p, sep);
        size_t L = q ? (size_t)(q - p) : strlen(p);
        DESCR_t a = pl_mk_atom_dup(p, L);
        *(DESCR_t *)gv_push(&el, (uint16_t)HB_DVEC, (uint32_t)sizeof(DESCR_t), "pl_split_text") = a;
        n++;
        if (!q) break;
        p = q + sl;
    }
    return plw_unify_vals(args[0], pl_list_from_arr((DESCR_t *)el.p, n), cx);
}
static int pl_alc_splits(DESCR_t *args) {
    char sb[256];
    const char *sep;
    DESCR_t l = rt_pl_deref_val(args[0]), end;
    long n = 0;
    int cyc = 0;
    if (pl_val_unbound(l)) return 1;
    if (pl_val_unbound(rt_pl_deref_val(args[2])) || !pl_cell_text(args[1], sb, sizeof sb, &sep) || !sep[0]) return 0;
    end = pl_spine_end(l, &n, &cyc);
    if (cyc) return 0;
    if (pl_val_unbound(end)) return 1;
    for (DESCR_t cur = l; n-- > 0; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) if (pl_val_unbound(rt_pl_deref_val(((DESCR_t *)cur.p)[0]))) return 1;
    return 0;
}
DESCR_t rt_pl_dop_atomic_list_concat_c(DESCR_t *args, int nargs, pl_tr_ctx_t *cx) {
    int ok;
    if (nargs != 2 && nargs != 3) return FAILDESCR;
    pl_atoms_ready();
    rt_pl_tr_gc_sync(cx->tr);
    if (nargs == 3 && pl_alc_splits(args)) ok = pl_split_text(args, cx);
    else ok = rt_pl_atom_op_cell("atomic_list_concat", &args[0], &args[1], nargs == 3 ? (void *)&args[2] : (void *)0, cx);
PL_CX_LEAF_TAIL
static int pl_op_spec_ok(const char *t) { static const char *const k[] = { "xfx", "xfy", "yfx", "fy", "fx", "yf", "xf", 0 }; for (int i = 0; k[i]; i++) if (!strcmp(t, k[i])) return 1; return 0; }
static const char *pl_op_name(DESCR_t d) { return pl_is_nil(d) ? "[]" : pl_atom_str(d); }
static DESCR_t pl_op_unqual(DESCR_t d) {
    extern const char *prolog_atom_name(int);
    while (pl_swi_dialect() && d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 2 && !strcmp(prolog_atom_name(plc_fid_name(d.slen)), ":")) d = rt_pl_deref_val(((DESCR_t *)d.p)[1]);
    return d;
}
static int pl_op_foreign(DESCR_t d) {
    extern const char *prolog_atom_name(int);
    const char *m = (const char *)0;
    while (pl_swi_dialect() && d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) == 2 && !strcmp(prolog_atom_name(plc_fid_name(d.slen)), ":")) {
        DESCR_t q = rt_pl_deref_val(((DESCR_t *)d.p)[0]);
        m = pl_val_unbound(q) ? (const char *)0 : pl_atom_str(q);
        d = rt_pl_deref_val(((DESCR_t *)d.p)[1]);
    }
    return m && strcmp(m, "user") && strcmp(m, "system");
}
PL_CX_LEAF_HEAD(op, 3)
    {
        extern int prolog_op_table_add(const char *, int, const char *);
        extern int prolog_op_permission(const char *, int, const char *);
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        extern void *rt_pl_ball_instantiation(void);
        extern void *rt_pl_ball_permission3(const char *, const char *, DESCR_t);
        DESCR_t p = rt_pl_deref_val(args[0]), t = rt_pl_deref_val(args[1]), o = pl_op_unqual(rt_pl_deref_val(args[2])), cur, e;
        const char *ty = pl_atom_str(t), *nm;
        int pm;
        ok = 0;
        for (cur = o; pl_is_cons(cur); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) if (pl_val_unbound(rt_pl_deref_val(((DESCR_t *)cur.p)[0]))) break;
        if (pl_val_unbound(p) || pl_val_unbound(t) || pl_val_unbound(o) || pl_val_unbound(cur) || pl_is_cons(cur)) cx->ball = rt_pl_ball_instantiation();
        else if (p.v != DT_I) cx->ball = rt_pl_ball_kind2("type_error", "integer", p);
        else if (!ty || pl_is_nil(t)) cx->ball = rt_pl_ball_kind2("type_error", "atom", t);
        else if (!pl_is_cons(o) && !pl_op_name(o)) cx->ball = rt_pl_ball_kind2("type_error", "list", o);
        else if (pl_is_cons(o) && !pl_is_nil(cur)) cx->ball = rt_pl_ball_kind2("type_error", "list", o);
        else if (p.i < 0 || p.i > 1200) cx->ball = rt_pl_ball_kind2("domain_error", "operator_priority", p);
        else if (!pl_op_spec_ok(ty)) cx->ball = rt_pl_ball_kind2("domain_error", "operator_specifier", t);
        if (cx->ball) return FAILDESCR;
        ty = ct_strdup(ty);
        for (cur = o; ; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
            e = pl_is_cons(cur) ? pl_op_unqual(rt_pl_deref_val(((DESCR_t *)cur.p)[0])) : cur;
            if (pl_is_cons(o) && pl_is_nil(cur)) break;
            if (!(nm = pl_op_name(e))) { cx->ball = rt_pl_ball_kind2("type_error", "atom", e); return FAILDESCR; }
            if ((pm = prolog_op_permission(nm, (int)p.i, ty)) && !(pl_swi_dialect() && (!strcmp(nm, "{}") || !strcmp(nm, "[]")))) {
                cx->ball = rt_pl_ball_permission3(pm == 1 ? "modify" : "create", "operator", e);
                return FAILDESCR;
            }
            if (!pl_is_cons(o)) break;
        }
        for (cur = o; ; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
            e = pl_is_cons(cur) ? pl_op_unqual(rt_pl_deref_val(((DESCR_t *)cur.p)[0])) : cur;
            if (pl_is_cons(o) && pl_is_nil(cur)) break;
            if (!pl_op_foreign(rt_pl_deref_val(args[2])) && !pl_op_foreign(pl_is_cons(cur) ? rt_pl_deref_val(((DESCR_t *)cur.p)[0]) : cur)) prolog_op_table_add(ct_strdup(pl_op_name(e)), (int)p.i, ty);
            if (!pl_is_cons(o)) break;
        }
        ok = 1;
    }
PL_CX_LEAF_TAIL
int rt_pl_cap_refuses(unsigned long long n, unsigned long long b) {
    extern unsigned long long rt_gcheap_cap_bytes(void);
    pl_flag_t *sl = pl_flag_find("stack_limit");
    unsigned long long cap = rt_gcheap_cap_bytes(), soft = sl && sl->val ? strtoull(sl->val, (char **)0, 10) : 0ULL;
    if (soft && (!cap || soft < cap)) cap = soft;
    return cap && n && b && n > cap / b;
}
PL_CX_LEAF_HEAD(heap_guard, 2)
    {
        extern void *rt_pl_cap_error(void);
        DESCR_t u = rt_pl_deref_val(args[0]), b = rt_pl_deref_val(args[1]);
        ok = 1;
        if (u.v == (DTYPE_t)DT_I && b.v == (DTYPE_t)DT_I && u.i > 0 && b.i > 0 && rt_pl_cap_refuses((unsigned long long)u.i, (unsigned long long)b.i)) { cx->ball = rt_pl_cap_error(); ok = 0; }
    }
PL_CX_LEAF_TAIL
PL_CX_LEAF_HEAD(pl_op_check, 3)
    {
        extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
        DESCR_t p = rt_pl_deref_val(args[0]), t = rt_pl_deref_val(args[1]), o = rt_pl_deref_val(args[2]);
        ok = 1;
        if (!pl_val_unbound(p) && p.v != DT_I && pl_swi_dialect()) cx->ball = rt_pl_ball_kind2("type_error", "integer", p);
        else if (!pl_val_unbound(p) && (p.v != DT_I || p.i < 0 || p.i > 1200)) cx->ball = rt_pl_ball_kind2("domain_error", "operator_priority", p);
        else if (!pl_val_unbound(t) && !pl_atom_str(t) && !pl_is_nil(t) && pl_swi_dialect()) cx->ball = rt_pl_ball_kind2("type_error", "atom", t);
        else if (!pl_val_unbound(t) && (!pl_atom_str(t) || pl_is_nil(t) || !pl_op_spec_ok(pl_atom_str(t)))) cx->ball = rt_pl_ball_kind2("domain_error", "operator_specifier", t);
        else if (!pl_val_unbound(o) && !pl_op_name(o)) cx->ball = rt_pl_ball_kind2("type_error", "atom", o);
        if (cx->ball) ok = 0;
    }
PL_CX_LEAF_TAIL
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pl_iso_unbound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLVAR || d.v == DT_SNUL || d.v == DT_FAIL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_char_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    char b[64];
    const char *s;
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t v = rt_pl_deref_val(args[0]);
        if (pl_iso_unbound(v)) return rt_pl_ball_instantiation();
        if (pl_cell_text(args[0], b, sizeof b, &s) && s && s[0] && !s[1]) return (void *)0;
        return rt_pl_ball_kind2("type_error", "character", v);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_goal_conv_scan(DESCR_t t, int depth) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    extern const char *prolog_atom_name(int);
    DESCR_t v = rt_pl_deref_val(t);
    if (pl_iso_unbound(v)) return depth ? 0 : 1;
    if ((int)v.v == DT_PLREF) {
        int ar = plc_fid_arity(v.slen);
        const char *fn = prolog_atom_name(plc_fid_name(v.slen));
        if (ar == 2 && fn && depth < 64 && (!strcmp(fn, ",") || !strcmp(fn, ";") || !strcmp(fn, "->") || !strcmp(fn, "*->") || !strcmp(fn, "|"))) {
            DESCR_t *kids = (DESCR_t *)v.p;
            int a = pl_goal_conv_scan(kids[0], depth + 1);
            if (a) return a;
            return pl_goal_conv_scan(kids[1], depth + 1);
        }
        if (ar == 1 && fn && depth < 64 && !strcmp(fn, "\\+") && rt_pl_dialect_swi()) return pl_goal_conv_scan(((DESCR_t *)v.p)[0], depth + 1);
        return 0;
    }
    if ((int)v.v == DT_PLATOM || v.v == DT_S) return 0;
    return 2;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_goal_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t g = rt_pl_deref_val(args[0]);
        int k = pl_goal_conv_scan(args[0], 0);
        if (k == 1) return rt_pl_ball_instantiation();
        if (k == 2) return rt_pl_ball_kind2("type_error", "callable", g);
        return (void *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_list_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    if (nargs != 1) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t orig = rt_pl_deref_val(args[0]);
        DESCR_t cur = orig;
        void *ck = (void *)0;
        long st = 0, pw = 1;
        while (cur.v == (DTYPE_t)DT_PLREF && plc_fid_arity(cur.slen) == 2 && cur.p) {
            if (cur.p == ck) return rt_pl_ball_kind2("type_error", "list", orig);
            if (++st == pw) { ck = cur.p; pw <<= 1; st = 0; }
            cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]);
        }
        if (pl_iso_unbound(cur) || pl_is_nil(cur)) return (void *)0;
        return rt_pl_ball_kind2("type_error", "list", orig);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_pl_dop_pl_declared_c(DESCR_t *args, int nargs, void *root) {
    extern int rt_pl_db_key_is_declared(void *, const char *);
    char nb[264];
    const char *nm;
    if (nargs != 2) return FAILDESCR;
    pl_atoms_ready();
    {
        DESCR_t a = rt_pl_deref_val(args[1]);
        if (a.v != DT_I || !pl_cell_text(args[0], nb, sizeof nb, &nm) || !nm) return FAILDESCR;
        char key[fmt_len("%s/%d", nm, (int)a.i)];
        snprintf(key, sizeof key, "%s/%d", nm, (int)a.i);
        return rt_pl_db_key_is_declared(root, key) ? pl_ok() : FAILDESCR;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_dynamic_one(DESCR_t s, void *root, const char *mod) {
    extern int prolog_atom_intern(const char *);
    extern int rt_pl_db_decl(void *, const char *, int64_t, int64_t);
    s = rt_pl_deref_val(s);
    if (pl_is_nil(s)) return 1;
    if (s.v == (DTYPE_t)DT_PLREF && plc_fid_arity(s.slen) == 2 && plc_fid_name(s.slen) == prolog_atom_intern("/")) {
        DESCR_t n = rt_pl_deref_val(((DESCR_t *)s.p)[0]);
        DESCR_t a = rt_pl_deref_val(((DESCR_t *)s.p)[1]);
        const char *nm;
        if (n.v == (DTYPE_t)DT_PLREF && plc_fid_arity(n.slen) == 2 && plc_fid_name(n.slen) == prolog_atom_intern(":")) {
            const char *m = pl_atom_str(rt_pl_deref_val(((DESCR_t *)n.p)[0]));
            n = rt_pl_deref_val(((DESCR_t *)n.p)[1]);
            if (m) mod = strcmp(m, "user") ? m : (const char *)0;
        }
        nm = pl_atom_str(n);
        if (!nm || a.v != DT_I || (long long)a.i < 0) return 0;
        if (mod) { char qn[strlen(mod) + strlen(nm) + 2]; snprintf(qn, sizeof qn, "%s:%s", mod, nm); return rt_pl_db_decl(root, qn, a.i, 2); }
        return rt_pl_db_decl(root, nm, a.i, 2);
    }
    if (pl_is_cons(s) || (s.v == (DTYPE_t)DT_PLREF && plc_fid_arity(s.slen) == 2 && plc_fid_name(s.slen) == prolog_atom_intern(","))) return pl_dynamic_one(((DESCR_t *)s.p)[0], root, mod) &&
        pl_dynamic_one(((DESCR_t *)s.p)[1], root, mod);
    if (s.v == (DTYPE_t)DT_PLREF && plc_fid_arity(s.slen) == 2 && plc_fid_name(s.slen) == prolog_atom_intern(":")) {
        const char *m = pl_atom_str(rt_pl_deref_val(((DESCR_t *)s.p)[0]));
        if (!m) return 0;
        return pl_dynamic_one(((DESCR_t *)s.p)[1], root, strcmp(m, "user") ? m : (const char *)0);
    }
    return 0;
}
DESCR_t rt_pl_dop_pl_dynamic_c(DESCR_t *args, int nargs, void *root) {
    if (nargs != 1) return FAILDESCR;
    pl_atoms_ready();
    return pl_dynamic_one(args[0], root, (const char *)0) ? pl_ok() : FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void * rt_pl_dop_between_guard_c(DESCR_t *args, int nargs) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    if (nargs != 3) return (void *)0;
    pl_atoms_ready();
    {
        DESCR_t lo = rt_pl_deref_val(args[0]);
        DESCR_t hi = rt_pl_deref_val(args[1]);
        DESCR_t xv = rt_pl_deref_val(args[2]);
        if (pl_iso_unbound(lo)) return rt_pl_ball_instantiation();
        if (lo.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", lo);
        if (pl_iso_unbound(hi)) return rt_pl_ball_instantiation();
        if (hi.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", hi);
        if (!pl_iso_unbound(xv) && xv.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", xv);
        return (void *)0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_anum_list_kind(DESCR_t d) {
    DESCR_t cur = rt_pl_deref_val(d);
    for (;;) { if (pl_is_nil(cur)) return 1; if (pl_iso_unbound(cur)) return 0; if (!pl_is_cons(cur)) return -1; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); }
}
static int pl_anum_is_num(DESCR_t d) { return d.v == DT_I || d.v == DT_R || d.v == DT_BIG || pl_rat_is(d); }
int pl_anum_is_compound(DESCR_t d) { return d.v == (DTYPE_t)DT_PLREF && plc_fid_arity(d.slen) > 0; }
static int pl_anum_code_ok(long c) { return c >= 0 && c <= 0x10FFFF; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_anum_one_char(const char *es) { int adv; if (!es || !es[0]) return 0; (void)rt_pl_u8_get(es, &adv); return es[adv] == '\0'; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_code_range_ball(DESCR_t c) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    return pl_swi_dialect() ? rt_pl_ball_kind2("type_error", "character_code", c) : rt_pl_ball_kind1("representation_error", "character_code");
}
static void * pl_anum_elems(DESCR_t lst, int codes, int allow_var) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    DESCR_t cur = rt_pl_deref_val(lst);
    while (pl_is_cons(cur)) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (pl_iso_unbound(e)) { if (!allow_var) return rt_pl_ball_instantiation(); cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]); continue; }
        if (codes) {
            if (e.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", e);
            if (!pl_anum_code_ok((long)e.i)) return pl_code_range_ball(e);
        } else {
            const char *es = pl_atom_str(e);
            if (!pl_anum_one_char(es)) return rt_pl_ball_kind2("type_error", "character", e);
        }
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]);
    }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_number_syntax(DESCR_t lst, int codes) {
    extern void *rt_pl_ball_kind1(const char *, const char *);
    size_t need = 1;
    for (DESCR_t m = rt_pl_deref_val(lst); pl_is_cons(m); m = rt_pl_deref_val(((DESCR_t *)m.p)[1])) {
        const char *es = codes ? (const char *)0 : pl_atom_str(rt_pl_deref_val(((DESCR_t *)m.p)[0]));
        need += codes ? 4 : (es ? strlen(es) : 0);
    }
    char buf[need];
    size_t k = 0;
    DESCR_t cur = rt_pl_deref_val(lst);
    DESCR_t num;
    while (pl_is_cons(cur)) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (codes) {
            if (e.v != DT_I) return (void *)0;
            k += (size_t)rt_pl_u8_put(buf + k, (int)e.i);
        } else {
            const char *es = pl_atom_str(e);
            if (!pl_anum_one_char(es)) return (void *)0;
            { size_t cl = strlen(es); memcpy(buf + k, es, cl); k += cl; }
        }
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]);
    }
    buf[k] = '\0';
    {
        extern const char *pl_skip_layout(const char *);
        const char *p = pl_skip_layout(buf);
        if (*p && pl_parse_number(p, &num)) {
            if (num.v == DT_R && isinf(num.r) && !strstr(p, "Inf") && !strstr(p, "inf") && !pl_float_flag("float_overflow", "infinity")) return rt_pl_ball_kind1("syntax_error", "float_overflow");
            return (void *)0;
        }
        return rt_pl_ball_kind1("syntax_error", "illegal_number");
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_swi_list(DESCR_t a, DESCR_t l, const char *atom_type) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    int mode = -1;
    DESCR_t m = rt_pl_deref_val(l);
    for (; pl_is_cons(m); m = rt_pl_deref_val(((DESCR_t *)m.p)[1])) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)m.p)[0]);
        int is_code = e.v == DT_I && pl_anum_code_ok((long)e.i), is_char = pl_anum_one_char(pl_atom_str(e));
        if (pl_iso_unbound(e)) return pl_iso_unbound(a) ? rt_pl_ball_instantiation() : (void *)0;
        if (mode < 0) mode = is_code ? 1 : is_char ? 0 : 1;
        if (mode == 1 && !is_code) return rt_pl_ball_kind2("type_error", "character_code", e);
        if (mode == 0 && !is_char) return rt_pl_ball_kind2("type_error", "character", e);
    }
    if (pl_iso_unbound(m)) return pl_iso_unbound(a) ? rt_pl_ball_instantiation() : (void *)0;
    if (!pl_is_nil(m)) return rt_pl_ball_kind2("type_error", "list", l);
    return strcmp(atom_type, "number") ? (void *)0 : pl_anum_number_syntax(l, mode == 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_text_list_pair(DESCR_t a, DESCR_t l, int codes, const char *atom_type) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    int lk = pl_anum_list_kind(l);
    if (!pl_iso_unbound(a)) {
        if (codes >= 0 && !strcmp(atom_type, "atom") && !pl_anum_is_text(a) && !(pl_swi_dialect() && pl_anum_is_num(a))) return rt_pl_ball_kind2("type_error", "atom", a);
        if (!strcmp(atom_type, "number") && !pl_anum_is_num(a)) return rt_pl_ball_kind2("type_error", "number", a);
    }
    if (l.v == DT_S) {
        extern void *rt_pl_ball_kind1(const char *, const char *);
        extern const char *pl_skip_layout(const char *);
        DESCR_t num;
        const char *p = pl_skip_layout(l.s ? l.s : "");
        return (strcmp(atom_type, "number") || pl_swi_string_zero_quote(p) || (*p && pl_parse_number(p, &num))) ? (void *)0 : rt_pl_ball_kind1("syntax_error", "illegal_number");
    }
    if (pl_swi_dialect()) return pl_anum_swi_list(a, l, atom_type);
    if (pl_iso_unbound(a)) {
        for (DESCR_t m = rt_pl_deref_val(l); pl_is_cons(m); m = rt_pl_deref_val(((DESCR_t *)m.p)[1])) if (pl_iso_unbound(rt_pl_deref_val(((DESCR_t *)m.p)[0]))) return rt_pl_ball_instantiation();
        if (lk == 0 && pl_swi_dialect()) return rt_pl_ball_instantiation();
    }
    if (lk == -1) return rt_pl_ball_kind2("type_error", "list", l);
    { void *b = pl_anum_elems(l, codes, !pl_iso_unbound(a)); if (b) return b; }
    if (pl_iso_unbound(a) && lk == 0) return rt_pl_ball_instantiation();
    if (lk == 1 && !strcmp(atom_type, "number")) { void *b = pl_anum_number_syntax(l, codes); if (b) return b; }
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_anum_atomics_in_list(DESCR_t lst) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    DESCR_t cur = rt_pl_deref_val(lst);
    while (pl_is_cons(cur)) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        if (!pl_is_nil(e)) { if (pl_iso_unbound(e)) return rt_pl_ball_instantiation(); if (!pl_anum_is_text(e) && !pl_anum_is_num(e)) return rt_pl_ball_kind2("type_error", "atomic", e); }
        cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1]);
    }
    if (!pl_is_nil(cur) && pl_iso_unbound(cur)) return rt_pl_ball_instantiation();
    return (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pl_anum_big_sign(DESCR_t b) { extern int rt_big_cmp(DESCR_t, DESCR_t); return rt_big_cmp(b, INTVAL(0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void * pl_swi_list_text_ball(DESCR_t x) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    long k = 0;
    int cyc = 0, mode = 0;
    DESCR_t end = pl_spine_end(x, &k, &cyc), cur = x;
    if (!pl_is_cons(x) || cyc) return rt_pl_ball_kind2("type_error", "text", x);
    for (; k-- > 0; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
        DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
        size_t en = 0;
        const char *es = e.v == DT_I ? (const char *)0 : pl_atom_str_n(e, &en);
        int code = e.v == DT_I && e.i >= 0 && e.i <= 0x10FFFF, ch = es && en > 0 && (size_t)utf8_seqlen((unsigned char)es[0]) == en;
        if (pl_iso_unbound(e)) return rt_pl_ball_instantiation();
        if (!mode) mode = code ? 1 : ch ? 2 : 0;
        if (mode != 2 && !code) return rt_pl_ball_kind2("type_error", "character_code", e);
        if (mode == 2 && !ch) return rt_pl_ball_kind2("type_error", "character", e);
    }
    if (pl_iso_unbound(end)) return rt_pl_ball_instantiation();
    return pl_is_nil(end) ? (void *)0 : rt_pl_ball_kind2("type_error", "text", x);
}
void * pl_anum_check(const char *nm, DESCR_t *a, int n) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    extern void *rt_pl_ball_instantiation(void);
    if (!nm) return (void *)0;
    if (!strcmp(nm, "atom_length") && n == 2) {
        DESCR_t x = rt_pl_deref_val(a[0]), l = rt_pl_deref_val(a[1]);
        if (pl_iso_unbound(x)) return rt_pl_ball_instantiation();
        if (pl_swi_dialect() && x.v == (DTYPE_t)DT_PLREF) {
            void *tb = pl_swi_list_text_ball(x);
            if (tb) return tb;
        } else if (!pl_anum_is_text(x) && !(pl_swi_dialect() && pl_anum_is_num(x))) return rt_pl_ball_kind2("type_error", "atom", x);
        if (!pl_iso_unbound(l) && l.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", l);
        if (!pl_iso_unbound(l) && l.i < 0 && !pl_swi_dialect()) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", l);
        return (void *)0;
    }
    if (!strcmp(nm, "number_string") && n == 2) {
        DESCR_t x = rt_pl_deref_val(a[0]), s = rt_pl_deref_val(a[1]);
        if (!pl_iso_unbound(x) && !pl_anum_is_num(x)) return rt_pl_ball_kind2("type_error", "number", x);
        if (pl_iso_unbound(x) && pl_iso_unbound(s)) return rt_pl_ball_instantiation();
        return (void *)0;
    }
    if (!strcmp(nm, "atomic_list_concat") && (n == 2 || n == 3)) {
        DESCR_t l = rt_pl_deref_val(a[0]), sep = rt_pl_deref_val(a[1]), r = rt_pl_deref_val(a[n - 1]), end, cur;
        int swi = pl_swi_dialect(), cyc = 0, partial;
        long k = 0;
        if (n == 3 && !pl_iso_unbound(sep) && !pl_is_nil(sep) && !pl_anum_is_text(sep) && !pl_anum_is_num(sep)) return rt_pl_ball_kind2("type_error", swi ? "text" : "atomic", sep);
        if (!swi && !pl_iso_unbound(r) && !pl_anum_is_text(r)) return rt_pl_ball_kind2("type_error", "atom", r);
        end = pl_spine_end(l, &k, &cyc);
        if (!cyc && !pl_iso_unbound(end) && !pl_is_nil(end)) return rt_pl_ball_kind2("type_error", "list", end);
        partial = !cyc && pl_iso_unbound(end);
        for (cur = l; !cyc && k-- > 0; cur = rt_pl_deref_val(((DESCR_t *)cur.p)[1])) {
            DESCR_t e = rt_pl_deref_val(((DESCR_t *)cur.p)[0]);
            if (pl_iso_unbound(e)) partial = 1;
            else if (swi && !pl_is_nil(e) && !pl_anum_is_text(e) && !pl_anum_is_num(e)) return rt_pl_ball_kind2("type_error", "text", e);
        }
        if (swi && !pl_iso_unbound(r) && !pl_anum_is_text(r) && !pl_anum_is_num(r)) return partial ? rt_pl_ball_kind2("type_error", "atomic", r) : (void *)0;
        if (partial && (pl_iso_unbound(r) || (n == 3 && pl_iso_unbound(sep)))) return rt_pl_ball_instantiation();
        if (n == 3 && !pl_is_nil(l) && pl_iso_unbound(l) && !pl_iso_unbound(r) && pl_atom_str(sep) && !pl_atom_str(sep)[0]) return rt_pl_ball_kind2("domain_error", "non_empty_atom", sep);
        if (n == 3 && !pl_is_nil(l) && pl_iso_unbound(l) && !pl_iso_unbound(r)) return (void *)0;
        if (n == 3 && pl_iso_unbound(sep)) return rt_pl_ball_instantiation();
        if (n == 3 && pl_alc_splits(a)) return (void *)0;
        { void *b = pl_anum_atomics_in_list(l); if (b) return b; }
        return (void *)0;
    }
    if (!strcmp(nm, "atomic_concat") && n == 3) {
        DESCR_t x = rt_pl_deref_val(a[0]), y = rt_pl_deref_val(a[1]), z = rt_pl_deref_val(a[2]);
        if (!pl_iso_unbound(z) && !pl_anum_is_text(z)) return rt_pl_ball_kind2("type_error", "atom", z);
        if (!pl_iso_unbound(x) && !pl_anum_is_text(x) && !pl_anum_is_num(x)) return rt_pl_ball_kind2("type_error", "atomic", x);
        if (!pl_iso_unbound(y) && !pl_anum_is_text(y) && !pl_anum_is_num(y)) return rt_pl_ball_kind2("type_error", "atomic", y);
        if (pl_iso_unbound(x) || pl_iso_unbound(y)) return rt_pl_ball_instantiation();
        return (void *)0;
    }
    if (!strcmp(nm, "atom_concat") && n == 3) {
        DESCR_t x = rt_pl_deref_val(a[0]), y = rt_pl_deref_val(a[1]), z = rt_pl_deref_val(a[2]);
        int swi = pl_swi_dialect();
        if (!pl_iso_unbound(z) && !pl_anum_is_text(z) && !(swi && pl_anum_is_num(z))) return rt_pl_ball_kind2("type_error", swi ? "atomic" : "atom", z);
        if (!pl_iso_unbound(x) && !pl_anum_is_text(x) && !(swi && pl_anum_is_num(x))) return rt_pl_ball_kind2("type_error", swi ? "atomic" : "atom", x);
        if (!pl_iso_unbound(y) && !pl_anum_is_text(y) && !(swi && pl_anum_is_num(y))) return rt_pl_ball_kind2("type_error", swi ? "atomic" : "atom", y);
        if (pl_iso_unbound(z) && (pl_iso_unbound(x) || pl_iso_unbound(y))) return rt_pl_ball_instantiation();
        return (void *)0;
    }
    if ((!strcmp(nm, "atom_chars") || !strcmp(nm, "atom_codes")) && n == 2) return pl_anum_text_list_pair(rt_pl_deref_val(a[0]), rt_pl_deref_val(a[1]), !strcmp(nm, "atom_codes"), "atom");
    if ((!strcmp(nm, "number_chars") || !strcmp(nm, "number_codes")) && n == 2) return pl_anum_text_list_pair(rt_pl_deref_val(a[0]), rt_pl_deref_val(a[1]), !strcmp(nm, "number_codes"), "number");
    if (!strcmp(nm, "char_code") && n == 2) {
        DESCR_t c = rt_pl_deref_val(a[0]), k = rt_pl_deref_val(a[1]);
        if (!pl_iso_unbound(c)) {
            const char *cs = c.v == (DTYPE_t)DT_PLATOM ? pl_atom_str(c) : NULL;
            if (!cs || !cs[0] || cs[utf8_seqlen((unsigned char)cs[0])]) return rt_pl_ball_kind2("type_error", "character", c);
        }
        if (!pl_iso_unbound(k)) { if (k.v != DT_I) return rt_pl_ball_kind2("type_error", "integer", k); if (!pl_anum_code_ok((long)k.i)) return pl_code_range_ball(k); }
        if (pl_iso_unbound(c) && pl_iso_unbound(k)) return rt_pl_ball_instantiation();
        return (void *)0;
    }
    if (!strcmp(nm, "sub_atom") && n == 5) {
        DESCR_t x = rt_pl_deref_val(a[0]), s = rt_pl_deref_val(a[4]);
        if (pl_iso_unbound(x)) return rt_pl_ball_instantiation();
        if (!pl_anum_is_text(x) && !(pl_swi_dialect() && pl_anum_is_num(x))) return rt_pl_ball_kind2("type_error", "atom", x);
        for (int i = 1; i <= 3; i++) { DESCR_t v = rt_pl_deref_val(a[i]); if (!pl_iso_unbound(v) && v.v != DT_I && v.v != DT_BIG) return rt_pl_ball_kind2("type_error", "integer", v); }
        if (!pl_iso_unbound(s) && !pl_anum_is_text(s) && !(pl_swi_dialect() && pl_anum_is_num(s))) return rt_pl_ball_kind2("type_error", "atom", s);
        for (int i = 1; i <= 3; i++) {
            DESCR_t v = rt_pl_deref_val(a[i]);
            if (v.v == DT_I ? v.i < 0 : v.v == DT_BIG && pl_anum_big_sign(v) < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", v);
        }
        for (int i = 1; i <= 3; i++) {
            extern int rt_big_limbs(DESCR_t);
            DESCR_t v = rt_pl_deref_val(a[i]);
            if (v.v == DT_BIG && rt_big_limbs(v) > 2) { extern void *rt_pl_ball_kind1(const char *, const char *); return rt_pl_ball_kind1("representation_error", "size_t"); }
        }
        return (void *)0;
    }
    if (!strcmp(nm, "arg") && n == 3) {
        DESCR_t k = rt_pl_deref_val(a[0]), t = rt_pl_deref_val(a[1]);
        if (pl_iso_unbound(k) || pl_iso_unbound(t)) return rt_pl_ball_instantiation();
        if (k.v != DT_I && k.v != DT_BIG) return rt_pl_ball_kind2("type_error", "integer", k);
        if (!pl_anum_is_compound(t)) return rt_pl_ball_kind2("type_error", "compound", t);
        if (k.v == DT_BIG ? pl_anum_big_sign(k) < 0 : (long long)k.i < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", k);
        return (void *)0;
    }
    if (!strcmp(nm, "functor") && n == 3) {
        DESCR_t t = rt_pl_deref_val(a[0]), f = rt_pl_deref_val(a[1]), r = rt_pl_deref_val(a[2]);
        if (!pl_iso_unbound(t)) return (void *)0;
        if (pl_iso_unbound(f) || pl_iso_unbound(r)) return rt_pl_ball_instantiation();
        if (r.v != DT_I && r.v != DT_BIG) return rt_pl_ball_kind2("type_error", "integer", r);
        if (pl_anum_is_compound(f)) return rt_pl_ball_kind2("type_error", "atomic", f);
        if (r.v == DT_BIG ? pl_anum_big_sign(r) < 0 : (long long)r.i < 0) return rt_pl_ball_kind2("domain_error", "not_less_than_zero", r);
        if ((r.v == DT_BIG || (long long)r.i > 0) && !pl_anum_is_text(f)) return rt_pl_ball_kind2("type_error", "atom", f);
        if (r.v == DT_BIG || (long long)r.i > PROLOG_MAX_ARITY) { extern void *rt_pl_ball_kind1(const char *, const char *); return rt_pl_ball_kind1("representation_error", "max_arity"); }
        return (void *)0;
    }
    return (void *)0;
}
