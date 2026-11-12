#include "runtime/unification/un_head.h"
#include <math.h>
#include "ir/ct_arena.h"
#include "runtime/pl_rat.h"
#include "runtime/core/core.h"
#include "runtime/rt/prolog_atom.h"
#include <ctype.h>
#include "runtime/unification/un_01_pl.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_05_pl.h"
extern int rt_pl_dialect_swi(void);
extern int rt_pl_dialect_xsb(void);
extern int rt_pl_dialect_gnu(void);
extern const char *prolog_atom_name(int id);
plc_tv1a_t rt_pl_tv1_walk_a(pl_cell_t *c, const void *functors, long nfunctors);
const void *rt_pl_functor_entries(void);
int prolog_functor_count(void);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_user_op(const char *fn, int ar, int *prec, int *ra) {
    extern int prolog_op_user_count(void);
    extern int prolog_op_user_get(int, const char **, int *, const char **);
    int n = prolog_op_user_count();
    for (int i = 0; i < n; i++) {
        const char *nm = 0;
        const char *ty = 0;
        int pr = 0;
        if (!prolog_op_user_get(i, &nm, &pr, &ty) || !nm || !ty || strcmp(nm, fn)) continue;
        if (ar == 2 && (!strcmp(ty, "xfx") || !strcmp(ty, "xfy") || !strcmp(ty, "yfx"))) { *prec = pr; *ra = !strcmp(ty, "xfy"); return 1; }
        if (ar == 1 && (!strcmp(ty, "fy") || !strcmp(ty, "fx"))) { *prec = pr; *ra = 0; return 1; }
    }
    return 0;
}
int plc_op_info_id(int nid, int arity, int *prec, int *lmax, int *rmax) {
    extern int prolog_op_col_get(int, int, int *, int *);
    extern int prolog_op_col_user(int, int);
    int p = 0, t = 0;
    if (nid < 0) return 0;
    if (arity == 2) {
        if (!prolog_op_col_get(nid, 1, &p, &t)) return 0;
    } else if (arity == 1) {
        if (prolog_op_col_get(nid, 0, &p, &t) && prolog_op_col_user(nid, 0)) {
        } else if (prolog_op_col_get(nid, 2, &p, &t) && prolog_op_col_user(nid, 2)) {
        } else if (!prolog_op_col_get(nid, 0, &p, &t)) return 0;
    } else return 0;
    *prec = p;
    if (t == 1) {
        *lmax = p - 1;
        *rmax = p - 1;
    } else if (t == 2) {
        *lmax = p - 1;
        *rmax = p;
    } else if (t == 3) {
        *lmax = p;
        *rmax = p - 1;
    } else if (t == 4) {
        *lmax = 0;
        *rmax = p;
    } else if (t == 5) {
        *lmax = 0;
        *rmax = p - 1;
    } else if (t == 6) {
        *lmax = p;
        *rmax = 0;
    } else {
        *lmax = p - 1;
        *rmax = 0;
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_op_info(const char *name, int arity, int *prec, int *lmax, int *rmax) { return name ? plc_op_info_id(prolog_atom_intern(name), arity, prec, lmax, rmax) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_op_is_postfix_id(int nid) {
    extern int prolog_op_col_get(int, int, int *, int *);
    extern int prolog_op_col_user(int, int);
    int p = 0, t = 0;
    if (nid < 0 || !prolog_op_col_get(nid, 2, &p, &t) || !prolog_op_col_user(nid, 2)) return 0;
    { int pp = 0, lm = 0, rm = 0; return !(plc_op_info_id(nid, 1, &pp, &lm, &rm) && rm > 0); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_op_is_postfix(const char *name) { return name ? plc_op_is_postfix_id(prolog_atom_intern(name)) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_is_nil(pl_cell_t *d) { if ((int)d->v == DT_PLATOM) return (int)d->i == g_runtime.atoms.atom_nil; if ((int)d->v == DT_S) return d->s && strcmp(d->s, "[]") == 0; return 0; }
int plc_atom_needs_quoting(const char *name) {
    static const char *const graphic = "#$&*+-./:<=>?@\\^~";
    extern int rt_pl_dialect_xsb(void);
    if (!name || !name[0]) return 1;
    if (rt_pl_dialect_xsb() && (!strcmp(name, "{}") || !strcmp(name, ";"))) return 1;
    if (rt_pl_dialect_xsb()) for (const char *q = name; *q; q++) if ((unsigned char)*q >= 0x80) return 1;
    if (!strcmp(name, "[]") || !strcmp(name, "{}") || !strcmp(name, "!") || !strcmp(name, ";")) return 0;
    if (isupper((unsigned char)name[0]) || name[0] == '_') return 1;
    {
        extern int prolog_u_letter(const char *, int *);
        int adv, k = ((unsigned char)name[0] >= 0x80) ? prolog_u_letter(name, &adv) : 0;
        if (islower((unsigned char)name[0]) || k == 2) {
            for (const char *q = name + (k ? adv : 1); *q; ) {
                if (isalnum((unsigned char)*q) || *q == '_') { q++; continue; }
                if ((unsigned char)*q >= 0x80 && prolog_u_letter(q, &adv)) { q += adv; continue; }
                return 1;
            }
            return 0;
        }
        if (k == 1) return 1;
    }
    {
        extern int prolog_u_letter(const char *, int *);
        extern int rt_pl_dialect_swi(void);
        int all_graphic = 1;
        for (const char *q = name; *q; ) {
            int adv = 1, cp;
            if ((unsigned char)*q < 0x80) { if (!strchr(graphic, *q)) { all_graphic = 0; break; } q++; continue; }
            if (!rt_pl_dialect_swi() || prolog_u_letter(q, &adv) ||
                (cp = rt_pl_u8_get(q, &adv), cp < 0xA0 || cp == 0xA0 || cp == 0x1680 || (cp >= 0x2000 && cp <= 0x200A) || cp == 0x2028 || cp == 0x2029 || cp == 0x202F || cp == 0x205F || cp == 0x3000))
                {
                all_graphic = 0;
                break;
            }
            q += adv;
        }
        if (all_graphic) { if (!strcmp(name, ".")) return 1; if (name[0] == '/' && name[1] == '*') return 1; return 0; }
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_wt_num(FILE *fp, pl_cell_t *d) { extern const char *pl_real_iso_str(double, char *, int); char fb[64]; fputs(pl_real_iso_str(d->r, fb, (int)sizeof fb), fp); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_term_prio(pl_cell_t *c, int ignore_ops) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0, ar;
    if (!d || ignore_ops || (int)d->v != DT_PLREF) return 0;
    ar = plc_fid_arity(d->slen);
    if (plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && ar == 2) return 0;
    if (ar != 1 && ar != 2) return 0;
    { int nid = plc_fid_name(d->slen); const char *fn = prolog_atom_name(nid); if (fn && !strcmp(fn, "{}") && ar == 1) return 0; if (fn && plc_op_info_id(nid, ar, &pr, &lm, &rm)) return pr; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_first_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0;
    if (!d) return '[';
    if (dl <= 0) return '.';
    if (pl_cell_unbound(d)) return '_';
    if ((int)d->v == DT_I) return d->i < 0 ? '-' : '0';
    if ((int)d->v == DT_R) return signbit(d->r) && !isnan(d->r) ? '-' : '0';
    if ((int)d->v == DT_BIG) { extern int rt_big_sign(DESCR_t); return rt_big_sign(*d) < 0 ? '-' : '0'; }
    if (pl_rat_is(*d)) return pl_rat_cmp(*d, pl_make_int(0)) < 0 ? '-' : '0';
    if ((int)d->v == DT_S) return '"';
    if ((int)d->v == DT_PLATOM || (int)d->v == DT_S) {
        const char *n = plc_atom_text(d);
        if (!n || !n[0]) return quoted ? '\'' : ' ';
        return (quoted && plc_atom_needs_quoting(n)) ? '\'' : (unsigned char)n[0];
    }
    if ((int)d->v != DT_PLREF) return '_';
    {
        int ar = plc_fid_arity(d->slen);
        int nid = plc_fid_name(d->slen);
        const char *fn = prolog_atom_name(nid);
        pl_cell_t *aa = (pl_cell_t *)d->p;
        if (!fn) return 'a';
        if (numbervars && ar == 1 && !strcmp(fn, "$VAR")) {
            pl_cell_t *n = pl_deref(&aa[0]);
            if ((int)n->v == DT_I) return 'A';
            if ((int)n->v == DT_PLATOM || (int)n->v == DT_S) { const char *t = plc_atom_text(n); return t ? (unsigned char)*t : 0; }
        }
        if ((!ignore_ops || (ignore_ops & 2)) && plc_fid_name(d->slen) == g_runtime.atoms.atom_dot && ar == 2) return '[';
        if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && !strcmp(fn, "{}")) return '{';
        if (!ignore_ops && (ar == 1 || ar == 2) && plc_op_info_id(nid, ar, &pr, &lm, &rm)) {
            if (pr > 1200) return (unsigned char)fn[0];
            if (ar == 2 && dl > 1 && plc_is_op_atom(pl_deref(&aa[0]))) return '(';
            if (ar == 2 || plc_op_is_postfix_id(nid)) return plc_first_char_d(&aa[0], quoted, ignore_ops, numbervars, dl - 1);
            return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0];
        }
        return (quoted && plc_atom_needs_quoting(fn)) ? '\'' : (unsigned char)fn[0];
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_last_char_d(pl_cell_t *c, int quoted, int ignore_ops, int numbervars, long dl) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    int pr = 0, lm = 0, rm = 0, ar, nid;
    const char *fn, *n;
    pl_cell_t *aa;
    if (!d) return ']';
    if (dl <= 0) return '.';
    if (pl_cell_unbound(d) || (int)d->v == DT_I || (int)d->v == DT_R || (int)d->v == DT_BIG || pl_rat_is(*d)) return '0';
    if ((int)d->v == DT_S) { n = plc_atom_text(d); return quoted ? '"' : (n[0] ? (unsigned char)n[strlen(n) - 1] : ' '); }
    if ((int)d->v == DT_PLATOM) { n = plc_atom_text(d); if (!n[0]) return quoted ? '\'' : ' '; return (quoted && plc_atom_needs_quoting(n)) ? '\'' : (unsigned char)n[strlen(n) - 1]; }
    if ((int)d->v != DT_PLREF) return '0';
    ar = plc_fid_arity(d->slen);
    nid = plc_fid_name(d->slen);
    fn = prolog_atom_name(nid);
    aa = (pl_cell_t *)d->p;
    if (!fn || !fn[0]) return ')';
    if (numbervars && ar == 1 && !strcmp(fn, "$VAR")) return 'a';
    if ((!ignore_ops || (ignore_ops & 2)) && nid == g_runtime.atoms.atom_dot && ar == 2) return ']';
    if ((!ignore_ops || (ignore_ops & 4)) && ar == 1 && !strcmp(fn, "{}")) return '}';
    if (ignore_ops || (ar != 1 && ar != 2) || !plc_op_info_id(nid, ar, &pr, &lm, &rm)) return ')';
    if (ar == 1 && plc_op_is_postfix_id(nid)) return (unsigned char)fn[strlen(fn) - 1];
    aa = ar == 2 ? &aa[1] : &aa[0];
    if (dl <= 1) return '.';
    return (plc_is_op_atom(pl_deref(aa)) || plc_term_prio(aa, ignore_ops) > rm) ? ')' : plc_last_char_d(aa, quoted, ignore_ops, numbervars, dl - 1);
}
static DESCR_t *plc_ws_vec(const pl_tr_ctx_t *cx) { return (cx && cx->tr) ? *(DESCR_t **)pl_tr_wslot_slot(pl_tr_base_of(cx->tr)) : (DESCR_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long plc_ws_mark(const pl_tr_ctx_t *cx) { DESCR_t *v; if (!cx || !cx->tr) return -1; v = plc_ws_vec(cx); return v ? (long)v[0].i : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_ws_end(const pl_tr_ctx_t *cx, long mark) {
    DESCR_t *v = plc_ws_vec(cx);
    long n;
    if (mark < 0 || !v) return;
    n = (long)v[0].i;
    for (long i = mark + 1; i <= n; i++) memset(&v[i], 0, sizeof v[i]);
    v[0].i = mark;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static long plc_ws_push(const pl_tr_ctx_t *cx, DESCR_t x) {
    void **w = pl_tr_wslot_slot(pl_tr_base_of(cx->tr));
    DESCR_t *v = (DESCR_t *)*w;
    long n = v ? (long)v[0].i : 0, cap = v ? (long)v[0].slen : 0;
    if (n + 1 >= cap) {
        long nc = cap ? 2 * cap : 32;
        DESCR_t *nv = (DESCR_t *)rt_ws_alloc_descr((size_t)nc);
        if (v) memcpy(nv, v, sizeof(DESCR_t) * (size_t)(n + 1));
        nv[0].v = (DTYPE_t)DT_I;
        nv[0].slen = (uint32_t)nc;
        nv[0].i = n;
        v = nv;
        *w = (void *)v;
    }
    v[n + 1] = x;
    v[0].i = n + 1;
    return n + 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_wsref_set(const pl_tr_ctx_t *cx, plc_wsref *r, pl_cell_t *c) {
    extern int rt_gc_in_arena(const char *);
    DESCR_t x;
    r->p = c;
    if (!cx || !cx->tr) { r->k = -1; return; }
    memset(&x, 0, sizeof x);
    if (c && rt_gc_in_arena((const char *)c)) { x.v = (DTYPE_t)DT_PLVAR; x.p = (void *)c; }
    if (r->k < 0) r->k = plc_ws_push(cx, x);
    else plc_ws_vec(cx)[r->k] = x;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t *plc_wsref_get(const pl_tr_ctx_t *cx, plc_wsref *r) {
    if (r->k >= 0) { DESCR_t *v = plc_ws_vec(cx); if (v && (int)v[r->k].v == DT_PLVAR) r->p = (pl_cell_t *)v[r->k].p; }
    return r->p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_fb_u8(plc_fb *f, unsigned cp) {
    char u[4];
    if (cp < 0x80u) { u[0] = (char)cp; plc_fb_raw(f, u, 1); return; }
    if (cp < 0x800u) { u[0] = (char)(0xC0u | (cp >> 6)); u[1] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 2); return; }
    if (cp < 0x10000u) { u[0] = (char)(0xE0u | (cp >> 12)); u[1] = (char)(0x80u | ((cp >> 6) & 0x3Fu)); u[2] = (char)(0x80u | (cp & 0x3Fu)); plc_fb_raw(f, u, 3); return; }
    u[0] = (char)(0xF0u | (cp >> 18));
    u[1] = (char)(0x80u | ((cp >> 12) & 0x3Fu));
    u[2] = (char)(0x80u | ((cp >> 6) & 0x3Fu));
    u[3] = (char)(0x80u | (cp & 0x3Fu));
    plc_fb_raw(f, u, 4);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fb_tab(plc_fb *f, int fill) { if (f->nf < 256) { f->fpos[f->nf] = f->n; f->fchr[f->nf] = fill; f->nf++; } }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char plc_fmt_goal_failed_tok = 1;
void *rt_pl_format_failed_token(void) { return (void *)&plc_fmt_goal_failed_tok; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *plc_fb_goal(plc_fb *f, pl_cell_t *g) {
    extern DESCR_t rt_pl_goal_gen_h_c(DESCR_t goal, DESCR_t *argv, int n, void **hslot, void **ball, pl_tr_ctx_t *cx);
    extern FILE *fh_memsink_open(char **, size_t *);
    extern int fh_alloc(FILE *);
    extern void fh_free(int);
    extern int fh_current_output(void);
    extern void fh_set_output(int);
    extern const char *fh_alias_name(int);
    extern void fh_set_alias(int, const char *);
    extern void *rt_pl_ball_take(void);
    extern void rt_proc_drop_frame_h(void **hslot);
    char *bp = (char *)0;
    size_t bn = 0;
    void *h = (void *)0;
    void *b = (void *)0;
    int sv = fh_current_output();
    int slot;
    DESCR_t r;
    FILE *ms = fh_memsink_open(&bp, &bn);
    if (!ms) return (void *)0;
    slot = fh_alloc(ms);
    if (slot < 0) { fclose(ms); return (void *)0; }
    fh_set_alias(slot, fh_alias_name(sv));
    fh_set_output(slot);
    r = rt_pl_goal_gen_h_c(*g, (DESCR_t *)0, 0, &h, &b, f->cx);
    if (!b && IS_FAIL_fn(r)) b = rt_pl_ball_take();
    fflush(ms);
    fh_set_output(sv);
    fh_free(slot);
    if (h) rt_proc_drop_frame_h(&h);
    fclose(ms);
    plc_fb_add(f, bp ? bp : "", bn);
    if (b) return b;
    return IS_FAIL_fn(r) ? (void *)&plc_fmt_goal_failed_tok : (void *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_eval(pl_cell_t *h, DESCR_t *out, void **ball) {
    extern int rt_pl_ax_eval_val(DESCR_t, DESCR_t *, void **);
    if (!h) return 0;
    if (pl_cell_unbound(h)) { extern void *rt_pl_ball_instantiation(void); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    return rt_pl_ax_eval_val(*h, out, ball);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_int(pl_cell_t *h, long *out, void **ball) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    DESCR_t v;
    if (!plc_fmt_eval(h, &v, ball)) return 0;
    if (v.v != DT_I) { if (!*ball) *ball = rt_pl_ball_kind2("type_error", "integer", v); return 0; }
    *out = (long)v.i;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_dbl(pl_cell_t *h, double *out, void **ball) { DESCR_t v; if (!plc_fmt_eval(h, &v, ball)) return 0; *out = (v.v == DT_I) ? (double)v.i : v.r; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_text(pl_cell_t *h, char **out, size_t *len, void **ball) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    size_t cap = 64, n = 0;
    char *b;
    pl_cell_t *lst;
    if (!h) return 0;
    if (pl_cell_unbound(h)) { if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (plc_fmt_is_atom(h)) {
        const char *s = plc_atom_text(h);
        if (!strcmp(s, "[]")) s = "";
        *len = strlen(s);
        *out = (char *)ct_alloc(*len + 1);
        if (!*out) return 0;
        memcpy(*out, s, *len + 1);
        return 1;
    }
    b = (char *)ct_alloc(cap);
    if (!b) return 0;
    lst = pl_deref(h);
    int kind = 0;
    while (lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2) {
        pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst);
        pl_cell_t *e = pl_deref(&aa[0]);
        long ch = -1;
        const char *cs = (const char *)0;
        size_t cl = 0;
        if (pl_cell_unbound(e)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
        if (rt_pl_dialect_xsb()) { int k = pl_is_int(e) ? 1 : 2; if (kind && kind != k) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", *h); return 0; } kind = k; }
        if (pl_is_int(e)) ch = (long)pl_int_val(e);
        else if (plc_fmt_is_atom(e)) { cs = plc_atom_text(e); cl = cs ? strlen(cs) : 0; } else { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", *e); return 0; }
        if (n + cl + 6 > cap) { while (n + cl + 6 > cap) cap *= 2; char *nb = (char *)ct_grow(b, cap); if (!nb) { ct_drop(b); return 0; } b = nb; }
        if (cs) { memcpy(b + n, cs, cl); n += cl; } else if (ch < 0x80) b[n++] = (char)ch;
        else if (ch < 0x800) {
            b[n++] = (char)(0xC0 | (ch >> 6));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        } else if (ch < 0x10000) {
            b[n++] = (char)(0xE0 | (ch >> 12));
            b[n++] = (char)(0x80 | ((ch >> 6) & 0x3F));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        } else {
            b[n++] = (char)(0xF0 | (ch >> 18));
            b[n++] = (char)(0x80 | ((ch >> 12) & 0x3F));
            b[n++] = (char)(0x80 | ((ch >> 6) & 0x3F));
            b[n++] = (char)(0x80 | (ch & 0x3F));
        }
        lst = pl_deref(&aa[1]);
    }
    if (lst && pl_cell_unbound(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_instantiation(); return 0; }
    if (!lst || !plc_is_nil(lst)) { ct_drop(b); if (!*ball) *ball = rt_pl_ball_kind2("type_error", "text", lst ? *lst : *h); return 0; }
    b[n] = '\0';
    *out = b;
    *len = n;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_digits(plc_fb *f, int neg, const char *d, long nd, long places, int group) {
    extern const char *rt_pl_flag_text(const char *);
    long tot = nd > places ? nd : places + 1, ip = tot - places, i;
    const char *dp = ".", *ts = ",", *gs = "r3", *x;
    char mark[ip + 1];
    memset(mark, 0, (size_t)ip + 1);
    if (group) {
        if ((x = rt_pl_flag_text("$loc_dp")) && x[0]) dp = x;
        if ((x = rt_pl_flag_text("$loc_ts"))) ts = x;
        if ((x = rt_pl_flag_text("$loc_grp")) && x[0]) gs = x;
        for (long pos = ip; pos > 0 && *gs; ) {
            char *e;
            long g;
            if (*gs == 'r') { g = strtol(gs + 1, &e, 10); while (g > 0 && (pos -= g) > 0) mark[pos] = 1; break; }
            g = strtol(gs, &e, 10);
            if (g <= 0 || e == gs) break;
            if ((pos -= g) > 0) mark[pos] = 1;
            gs = *e == ',' ? e + 1 : e;
        }
    }
    if (neg) plc_fb_add(f, "-", 1);
    for (i = 0; i < tot; i++) {
        char c = i < tot - nd ? '0' : d[i - (tot - nd)];
        if (i == ip && places > 0) plc_fb_add(f, group ? dp : ".", strlen(group ? dp : "."));
        if (group && i && i < ip && mark[i]) plc_fb_add(f, ts, strlen(ts));
        plc_fb_add(f, &c, 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_dec(plc_fb *f, long long iv, long places, int group) {
    char dig[24];
    int nd = 0, neg = iv < 0;
    unsigned long long u = neg ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv;
    if (!u) dig[nd++] = '0';
    while (u) { dig[nd++] = (char)('0' + (int)(u % 10ull)); u /= 10ull; }
    for (int i = 0; i < nd / 2; i++) { char t = dig[i]; dig[i] = dig[nd - 1 - i]; dig[nd - 1 - i] = t; }
    plc_fmt_digits(f, neg, dig, nd, places, group);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void plc_fmt_radix(plc_fb *f, long long iv, int base, int upper) {
    const char *d = upper ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
    char buf[80];
    int bi = 0;
    unsigned long long u = iv < 0 ? (unsigned long long)(-(iv + 1)) + 1ull : (unsigned long long)iv, ub = (unsigned long long)base;
    if (!u) buf[bi++] = '0';
    while (u) { buf[bi++] = d[(int)(u % ub)]; u /= ub; }
    if (iv < 0) plc_fb_add(f, "-", 1);
    while (bi) plc_fb_add(f, &buf[--bi], 1);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void *plc_fmt_exhausted(pl_cell_t all) {
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    return !rt_pl_dialect_gnu() ? rt_pl_ball_kind2("domain_error", "format_arguments", all) : rt_pl_ball_kind2("domain_error", "non_empty_list", pl_make_atom(g_runtime.atoms.atom_nil));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_fmt_has_stop(const char *p) {
    for (; *p; p++) {
        if (*p != '~') continue;
        p++;
        if (*p == '`' && p[1]) p += 2;
        else if (*p == '*') p++;
        else while (*p >= '0' && *p <= '9') p++;
        if (*p == ':') p++;
        if (*p == '|' || *p == '+' || *p == 'N') return 1;
        if (!*p) return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_format_run(const char *fmt, void *list_cell, pl_tr_ctx_t *cx) {
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    plc_fb f;
    void *ball = (void *)0;
    pl_cell_t *args = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0;
    pl_cell_t all = args ? *args : pl_make_int(0);
    const char *p;
    FILE *fd;
    plc_wsref ra = { -1, (pl_cell_t *)0 }, rall = { -1, (pl_cell_t *)0 };
    long mark = -1;
    pl_cell_t *args0 = args;
    plc_atoms_ready();
    memset(&f, 0, sizeof f);
    f.cx = cx;
    if (!fmt) return (void *)0;
    size_t fl = strlen(fmt);
    char fmtc[fl + 1];
    memcpy(fmtc, fmt, fl + 1);
    fmt = fmtc;
    if (plc_fmt_has_stop(fmt)) {
        extern int fh_query_position(int, long *, long *, long *);
        extern int fh_current_output(void);
        long c = 0, l = 0, lp = 0;
        if (fh_query_position(fh_current_output(), &c, &l, &lp) == 1) f.cadj = f.segadj = lp;
    }
    for (p = fmt; *p && !ball; p++) {
        long nval = 0;
        int have_n = 0, fill = ' ', colon = 0;
        if (*p != '~') { plc_fb_add(&f, p, 1); continue; }
        p++;
        if (*p == '`' && p[1]) {
            fill = (unsigned char)p[1];
            have_n = 1;
            nval = fill;
            p += 2;
        } else if (*p == '*') {
            pl_cell_t *h = plc_fmt_next_arg(&args);
            p++;
            if (!h) { ball = plc_fmt_exhausted(all); break; }
            if (!plc_fmt_int(h, &nval, &ball)) break;
            have_n = 1;
            fill = (int)nval;
        } else {
            while (*p >= '0' && *p <= '9') { nval = nval * 10 + (*p - '0'); have_n = 1; p++; }
            if (have_n) fill = (int)nval;
        }
        if (*p == ':') { colon = 1; p++; }
        if (!*p) { if (rt_pl_dialect_gnu() || rt_pl_dialect_xsb()) ball = rt_pl_ball_kind2("domain_error", "format_control_sequence", plc_make_atom_cell("")); break; }
        if (*p == '~') { plc_fb_add(&f, "~", 1); continue; }
        if (*p == 'n') { long r = have_n ? nval : 1; for (long i = 0; i < r; i++) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 'N') { if (plc_fb_colat(&f, f.n)) plc_fb_add(&f, "\n", 1); continue; }
        if (*p == 't') { plc_fb_tab(&f, have_n ? fill : ' '); continue; }
        if (*p == '|') { plc_fb_stop(&f, have_n ? nval : (long)plc_fb_colat(&f, f.n)); continue; }
        if (*p == '+') { plc_fb_stop(&f, (long)plc_fb_colat(&f, f.seg) - f.segadj + (have_n ? nval : 8)); continue; }
        if (rt_pl_dialect_gnu() && !strchr("acdDeEfgGikpqrRswW", *p)) { char cb[2] = { *p, 0 }; ball = rt_pl_ball_kind2("domain_error", "format_control_sequence", plc_make_atom_cell(cb)); break; }
        {
            pl_cell_t *h = plc_fmt_next_arg(&args);
            if (!h) { ball = plc_fmt_exhausted(all); break; }
            if (rt_pl_dialect_xsb() && (*p == 'w' || *p == 'q')) {
                pl_cell_t *hd = pl_deref(h);
                if ((int)hd->v == DT_PLREF && plc_fid_arity(hd->slen) == 2 && plc_fid_name(hd->slen) == prolog_atom_intern("+")) {
                    pl_cell_t *pa = (pl_cell_t *)hd->p;
                    void *tb = plc_fb_term(&f, &pa[0], *p == 'q' ? 1 : 0, 0, 0);
                    if (!tb) { plc_fb_add(&f, "+", 1); tb = plc_fb_term(&f, &pa[1], *p == 'q' ? 1 : 0, 0, 0); }
                    if (tb) { ball = tb; break; }
                    continue;
                }
            }
            if (*p == 'w') plc_fb_term(&f, h, 0, 0, 0);
            else if (*p == 'p') {
                void *tb;
                if (mark < 0 && cx && cx->tr) { mark = plc_ws_mark(cx); plc_wsref_set(cx, &rall, args0); }
                if (mark >= 0) plc_wsref_set(cx, &ra, args);
                tb = plc_fb_term(&f, h, 4, 0, 0);
                if (mark >= 0) { args = plc_wsref_get(cx, &ra); args0 = plc_wsref_get(cx, &rall); if (args0) all = *args0; }
                if (tb) { ball = tb; break; }
            } else if (*p == '@' && rt_pl_dialect_swi()) {
                void *tb;
                if (pl_cell_unbound(h)) { ball = rt_pl_ball_instantiation(); break; }
                if (mark < 0 && cx && cx->tr) { mark = plc_ws_mark(cx); plc_wsref_set(cx, &rall, args0); }
                if (mark >= 0) plc_wsref_set(cx, &ra, args);
                tb = plc_fb_goal(&f, h);
                if (mark >= 0) { args = plc_wsref_get(cx, &ra); args0 = plc_wsref_get(cx, &rall); if (args0) all = *args0; }
                if (tb) { ball = tb; break; }
            } else if (*p == 'q') plc_fb_term(&f, h, 1, 0, 0);
            else if (*p == 'k') plc_fb_term(&f, h, 2, 0, 0);
            else if (*p == 'i') {
            } else if (*p == 'a') {
                char ab[96];
                const char *as = (const char *)0;
                if (pl_cell_unbound(h)) { ball = rt_pl_ball_instantiation(); break; }
                if (plc_fmt_is_atom(h)) as = plc_atom_text(h);
                else if (rt_pl_dialect_swi() && (int)h->v != DT_PLREF) as = plc_atom_op_text(h, ab, sizeof ab);
                if (!as) { ball = rt_pl_ball_kind2("type_error", "atom", *h); break; }
                plc_fb_add(&f, as, strlen(as));
            } else if (*p == 'c') {
                long cv;
                if (!plc_fmt_int(h, &cv, &ball)) break;
                if (rt_pl_dialect_gnu() && (cv < 0 || cv > 0x10FFFF)) {
                    extern void *rt_pl_ball_kind1(const char *, const char *);
                    ball = rt_pl_ball_kind1("representation_error", "character_code");
                    break;
                }
                {
                    long r = have_n ? nval : 1;
                    char c[8];
                    int cl = (cv >= 0x80 && (rt_pl_dialect_swi() || rt_pl_dialect_xsb())) ? rt_pl_u8_put(c, (int)cv) : (c[0] = (char)cv, 1);
                    for (long i = 0; i < r; i++) plc_fb_add(&f, c, (size_t)cl);
                }
            } else if (*p == 'd' || *p == 'D') {
                long iv;
                if (have_n && nval < 0) { ball = rt_pl_ball_kind2("domain_error", "format_argument", pl_make_int(nval)); break; }
                {
                    DESCR_t bv;
                    if (plc_fmt_eval(h, &bv, &ball) && bv.v == DT_BIG) {
                        extern char *rt_big_str(DESCR_t);
                        char *bs = rt_big_str(bv);
                        int ng = bs && bs[0] == '-';
                        plc_fmt_digits(&f, ng, bs ? bs + ng : "0", bs ? (long)strlen(bs + ng) : 1L, have_n ? nval : 0, *p == 'D' || colon);
                        continue;
                    }
                }
                if (!plc_fmt_int(h, &iv, &ball)) break;
                plc_fmt_dec(&f, (long long)iv, have_n ? nval : 0, *p == 'D' || colon);
            } else if (*p == 'r' || *p == 'R') {
                long iv;
                long base = have_n ? nval : 8;
                if (base < 2 || base > 36) { ball = rt_pl_ball_kind2("domain_error", "radix", pl_make_int(base)); break; }
                if (!plc_fmt_int(h, &iv, &ball)) break;
                plc_fmt_radix(&f, (long long)iv, (int)base, *p == 'R');
            } else if (*p == 'f' || *p == 'e' || *p == 'E' || *p == 'g' || *p == 'G') {
                double dv;
                char spec[8];
                DESCR_t nv;
                int prec = have_n ? (int)nval : 6;
                if (!plc_fmt_eval(h, &nv, &ball)) break;
                if (*p == 'f' && (nv.v == DT_I || nv.v == DT_BIG)) {
                    extern char *rt_big_str(DESCR_t);
                    char ib[24];
                    const char *is = nv.v == DT_BIG ? rt_big_str(nv) : (snprintf(ib, sizeof ib, "%lld", (long long)nv.i), ib);
                    int ng = is && is[0] == '-';
                    size_t il = is ? strlen(is + ng) : 0;
                    char dg[il + (size_t)(prec > 0 ? prec : 0) + 2];
                    memcpy(dg, is ? is + ng : "0", il);
                    for (int k = 0; k < prec; k++) dg[il + (size_t)k] = '0';
                    plc_fmt_digits(&f, ng, dg, (long)il + (prec > 0 ? prec : 0), prec > 0 ? prec : 0, colon);
                    continue;
                }
                if (*p == 'f' && pl_rat_is(nv)) {
                    extern char *rt_big_str(DESCR_t);
                    int ng = 0;
                    long pp = prec > 0 ? prec : 0;
                    DESCR_t qv = pl_rat_scaled(nv, pp, &ng);
                    char ib[24];
                    const char *is = qv.v == DT_BIG ? rt_big_str(qv) : (snprintf(ib, sizeof ib, "%lld", (long long)qv.i), ib);
                    size_t il = strlen(is), z = il <= (size_t)pp ? (size_t)pp + 1 - il : 0;
                    char dg[il + z + 1];
                    memset(dg, '0', z);
                    memcpy(dg + z, is, il);
                    plc_fmt_digits(&f, ng, dg, (long)(il + z), pp, colon);
                    continue;
                }
                if (pl_rat_is(nv)) dv = pl_rat_real(nv);
                else if (nv.v == DT_BIG) { extern char *rt_big_str(DESCR_t); dv = strtod(rt_big_str(nv), (char **)0); } else dv = nv.v == DT_I ? (double)nv.i : nv.r;
                spec[0] = '%';
                spec[1] = '.';
                spec[2] = '*';
                spec[3] = (char)*p;
                spec[4] = '\0';
                {
                    char nb[fmt_len(spec, prec, dv)];
                    snprintf(nb, sizeof nb, spec, prec, dv);
                    if (colon && *p == 'f' && isfinite(dv)) {
                        int ng = nb[0] == '-';
                        size_t nl = strlen(nb), k = 0;
                        char dg[nl + 1];
                        for (size_t i = (size_t)ng; i < nl; i++) if (nb[i] != '.') dg[k++] = nb[i];
                        plc_fmt_digits(&f, ng, dg, (long)k, prec, 1);
                    } else plc_fb_add(&f, nb, strlen(nb));
                }
            } else if (*p == 's') {
                char *tx = (char *)0;
                size_t tn = 0;
                if (!plc_fmt_text(h, &tx, &tn, &ball)) break;
                if (have_n && nval >= 0) {
                    size_t cn = pl_u8_count_n(tx, tn), want = (size_t)nval;
                    if (want < cn) { size_t bi = 0; for (size_t k = 0; k < want; k++) { int a; (void)rt_pl_u8_get(tx + bi, &a); bi += (size_t)a; } tn = bi; cn = want; }
                    plc_fb_add(&f, tx, tn);
                    if (want > cn && !rt_pl_dialect_swi()) for (size_t i = cn; i < want; i++) plc_fb_add(&f, " ", 1);
                } else plc_fb_add(&f, tx, tn);
                ct_drop(tx);
            } else if (*p == 'W') {
                pl_cell_t *o = plc_fmt_next_arg(&args);
                int quoted = 0, iops = 0;
                pl_cell_t *lst;
                if (!o) { ball = plc_fmt_exhausted(all); break; }
                if (pl_cell_unbound(o)) { ball = rt_pl_ball_instantiation(); break; }
                for (lst = pl_deref(o); lst && (int)lst->v == DT_PLREF && pl_arity(lst) == 2; ) {
                    pl_cell_t *aa = (pl_cell_t *)pl_compound_heap(lst);
                    pl_cell_t *e = pl_deref(&aa[0]);
                    if (e && (int)e->v == DT_PLREF && pl_arity(e) == 1) {
                        const char *on = prolog_atom_name(plc_fid_name(e->slen));
                        pl_cell_t *ov = pl_deref(&((pl_cell_t *)pl_compound_heap(e))[0]);
                        int on_true = plc_fmt_is_atom(ov) && !strcmp(plc_atom_text(ov), "true");
                        if (on && !strcmp(on, "quoted")) quoted = on_true;
                        else if (on && !strcmp(on, "ignore_ops")) iops = on_true;
                    }
                    lst = pl_deref(&aa[1]);
                }
                if (rt_pl_dialect_xsb()) {
                    pl_cell_t *hd = pl_deref(h);
                    if (quoted && iops) { plc_fb_term(&f, h, 2, 0, 0); continue; }
                    if (!iops && (int)hd->v == DT_PLREF && plc_fid_arity(hd->slen) == 2 && plc_fid_name(hd->slen) == prolog_atom_intern("+")) {
                        pl_cell_t *pa = (pl_cell_t *)hd->p;
                        void *tb = plc_fb_term(&f, &pa[0], 3, quoted, iops);
                        if (!tb) { plc_fb_add(&f, "+", 1); tb = plc_fb_term(&f, &pa[1], 3, quoted, iops); }
                        if (tb) { ball = tb; break; }
                        continue;
                    }
                }
                plc_fb_term(&f, h, 3, quoted, iops);
            }
        }
    }
    if (!ball && args && !pl_cell_unbound(args) && !plc_is_nil(args)) ball = rt_pl_ball_kind2("domain_error", "format_arguments", all);
    { fd = plc_out(); if (f.n && !(ball && rt_pl_dialect_xsb())) fwrite(f.b, 1, f.n, fd); }
    ct_drop(f.b);
    plc_ws_end(cx, mark);
    return ball;
}
static int plc_tv1_visit(pl_cell_t *d, void *ctx) {
    plc_tv1_t *t = (plc_tv1_t *)ctx;
    if (d->v == PLR_VCOPY) return PLR_V_GO;
    if ((int)d->v == DT_PLVAR) { d->v = PLR_VCOPY; d->p = (void *)t->head; t->head = d; return PLR_V_GO; }
    if (d->v == DT_SNUL || d->v == DT_FAIL) { t->other = 1; return PLR_V_STOP; }
    return PLR_V_GO;
}
PLR_WALK1(plc_tv1_walk, plc_tv1_visit, pl_deref)
int rt_pl_term_variables_cell(void *term_cell, void *vars_cell, void *tail_cell, pl_tr_ctx_t *cx) {
    int dot_id = prolog_atom_intern(".");
    pl_cell_t nil = pl_make_atom(g_runtime.atoms.atom_nil);
    pl_cell_t result = tail_cell ? *(pl_cell_t *)tail_cell : nil;
    plc_tv1_t t;
    t.head = (pl_cell_t *)0;
    t.other = 0;
    if (term_cell) {
        plc_tv1a_t a = rt_pl_tv1_walk_a((pl_cell_t *)term_cell, rt_pl_functor_entries(), (long)prolog_functor_count());
        t.head = a.head;
        t.other = a.st == 1;
        if (a.st == 2) {
            for (pl_cell_t *v = t.head, *nx; v; v = nx) { nx = (pl_cell_t *)v->p; v->v = (DTYPE_t)DT_PLVAR; v->p = (void *)v; }
            t.head = (pl_cell_t *)0;
            (void)plc_tv1_walk((pl_cell_t *)term_cell, (void *)&t);
        }
    }
    if (t.other) {
        for (pl_cell_t *v = t.head, *nx; v; v = nx) { nx = (pl_cell_t *)v->p; v->v = (DTYPE_t)DT_PLVAR; v->p = (void *)v; }
        pl_cell_t *pool[plc_var_occ((pl_cell_t *)term_cell) + 1];
        int pn = 0;
        rt_pl_tv_walk((pl_cell_t *)term_cell, pool, &pn);
        for (int i = pn - 1; i >= 0; i--) {
            pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            blk[0] = pl_make_ref(pool[i], (int)pool[i]->slen);
            blk[1] = result;
            result = pl_make_compound(dot_id, 2, blk);
        }
    } else for (pl_cell_t *v = t.head, *nx; v; v = nx) {
        pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
        nx = (pl_cell_t *)v->p;
        v->v = (DTYPE_t)DT_PLVAR;
        v->p = (void *)v;
        blk[0] = pl_make_ref(v, (int)v->slen);
        blk[1] = result;
        result = pl_make_compound(dot_id, 2, blk);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)vars_cell, &result, cx)) { return 0; }
    return 1;
}
