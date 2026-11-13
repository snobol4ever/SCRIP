#include "runtime/unification/un_head.h"
#include <math.h>
#include "runtime/pl_rat.h"
#include "runtime/core/core.h"
#include "runtime/rt/prolog_atom.h"
#include "runtime/unification/un_01_pl.h"
#include "runtime/unification/un_02_pl.h"
#include "runtime/unification/un_03_pl.h"
#include "runtime/unification/un_04_pl.h"
#include "runtime/unification/un_05_pl.h"
#include "runtime/unification/un_06_pl.h"
#include "runtime/unification/un_08_pl.h"
#include "runtime/core/g_runtime_atoms.h"
extern int rt_pl_eng_inst_is(DESCR_t);
extern int64_t rt_pl_eng_inst_id(DESCR_t);
extern int rt_pl_dialect_swi(void);
extern int rt_pl_dialect_xsb(void);
extern int rt_pl_dialect_gnu(void);
extern const char *prolog_atom_name(int id);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_unify_var_var(int lslot, int rslot) { (void)lslot; (void)rslot; return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_cells_init(void **cells, int n) { char *base = (char *)cells; for (int i = 0; i < n; i++) pl_init_var((pl_cell_t *)(base + (size_t)16 * (size_t)i), (const pl_tr_ctx_t *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_pl_gz_init(void *frame, int nslots) {
    prolog_atom_init();
    char *base = (char *)frame;
    for (int i = 0; i < nslots; i++) pl_init_var((pl_cell_t *)(base + 8 + (size_t)16 * (size_t)i), (const pl_tr_ctx_t *)0);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_compound_cell(const char *functor_name, int arity, void *arg_words) {
    pl_cell_t *src = (pl_cell_t *)arg_words;
    pl_cell_t *blk = (pl_cell_t *)PL_CELL_ALLOC((size_t)(arity > 0 ? arity : 1) * sizeof(pl_cell_t));
    for (int i = 0; i < arity; i++) blk[i] = src[i];
    if (arity == 0) blk[0] = pl_make_int(0);
    int fid = prolog_atom_intern(functor_name ? functor_name : "[]");
    pl_cell_t *out = (pl_cell_t *)PL_CELL_ALLOC(sizeof(pl_cell_t));
    *out = pl_make_compound(fid, arity, blk);
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_put_float(plc_vmap *m, pl_cell_t *d) {
    int prec = m->fprec;
    char spec = m->fspec, fmt[16];
    if (prec < 0) { plc_wt_num(m->fp, d); return; }
    if (prec == 0) prec = 17;
    snprintf(fmt, sizeof fmt, "%%.%d%c", prec, spec ? spec : 'g');
    char buf[snprintf((char *)0, 0, fmt, d->r) + 3];
    snprintf(buf, sizeof buf, fmt, d->r);
    if (!spec || spec == 'g') {
        char *e = strchr(buf, 'e');
        if (!strchr(buf, '.') && !strchr(buf, 'n') && !strchr(buf, 'i')) { if (e) { memmove(e + 2, e, strlen(e) + 1); e[0] = '.'; e[1] = '0'; } else strcat(buf, ".0"); }
    }
    fputs(buf, m->fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void plc_put_int(plc_vmap *m, long v) {
    FILE *fp = m->fp;
    unsigned long u, b = (unsigned long)m->ibase;
    char buf[72];
    int n = 0;
    if (m->ibase < 2) { fprintf(fp, "%ld", v); return; }
    u = v < 0 ? 0UL - (unsigned long)v : (unsigned long)v;
    do { int dg = (int)(u % b); buf[n++] = (char)(dg < 10 ? '0' + dg : 'a' + dg - 10); u /= b; } while (u);
    if (v < 0) fputc('-', fp);
    if (m->ipfx) { if (m->inamed) fputs(m->ibase == 16 ? "0x" : "0o", fp); else fprintf(fp, "%d'", m->ibase); }
    while (n) fputc(buf[--n], fp);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_xsb_codes(plc_vmap *m, pl_cell_t *d) {
    FILE *fp = m->fp;
    pl_cell_t *cur = d;
    long n = 0;
    for (;;) {
        pl_cell_t *ca = (pl_cell_t *)cur->p;
        pl_cell_t *hd = pl_deref(&ca[0]), *tl;
        long c;
        if ((int)hd->v != DT_I) return 0;
        c = (long)hd->i;
        if (!(c == 9 || c == 10 || (c >= 32 && c <= 126))) return 0;
        if (++n > 1000000) return 0;
        tl = pl_deref(&ca[1]);
        if ((int)tl->v == DT_PLREF && plc_fid_name(tl->slen) == g_rt_atoms.atom_dot && plc_fid_arity(tl->slen) == 2 && tl->p) { cur = tl; continue; }
        if (!plc_is_nil(tl) && !pl_cell_unbound(tl)) return 0;
        break;
    }
    fputc('"', fp);
    cur = d;
    for (;;) {
        pl_cell_t *ca = (pl_cell_t *)cur->p;
        long c = (long)pl_deref(&ca[0])->i;
        pl_cell_t *tl = pl_deref(&ca[1]);
        if (c == '"' || c == '\\') fputc((int)c, fp);
        fputc((int)c, fp);
        if ((int)tl->v != DT_PLREF || !tl->p) break;
        cur = tl;
    }
    fputc('"', fp);
    return 1;
}
pl_cell_t plc_fz_var(plc_fz_t *z, long k) {
    if (z->named) {
        char nb[32];
        pl_cell_t *a = (pl_cell_t *)rt_ws_alloc_descr(1);
        snprintf(nb, sizeof nb, "S_%ld", k);
        *a = pl_make_atom(prolog_atom_intern(nb));
        return pl_make_compound(prolog_atom_intern("$VAR"), 1, a);
    }
    { pl_cell_t *v = (pl_cell_t *)rt_ws_alloc_descr(1); pl_init_var(v, (const pl_tr_ctx_t *)0); return pl_make_ref(v, -1); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_atom_id_of(pl_cell_t *d) {
    if ((int)d->v == DT_PLATOM) return (int)d->i;
    if ((int)d->v == DT_S) { extern int prolog_atom_intern(const char *); return prolog_atom_intern(d->s ? d->s : ""); }
    return -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_is_atomlike(pl_cell_t *d) { return (int)d->v == DT_PLATOM || (int)d->v == DT_S; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pl_cell_t plc_atom_id_cell(int id) { return pl_make_atom(id); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_ground_visit(pl_cell_t *d, void *ctx) {
    (void)ctx;
    if (pl_cell_unbound(d)) return PLR_V_STOP;
    return ((int)d->v == DT_PLREF && plc_fid_arity(d->slen) > 0 && !d->p) ? PLR_V_STOP : PLR_V_GO;
}
PLR_WALK1(plc_ground_walk, plc_ground_visit, pl_deref) static int plc_cell_is_ground(pl_cell_t *c) { return c && !plc_ground_walk(c, (void *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int plc_cell_is_proper_list(pl_cell_t *c) {
    pl_cell_t *d = c ? pl_deref(c) : (pl_cell_t *)0;
    pl_cell_t *tort = d;
    unsigned long n = 0;
    while (d) {
        if (plc_is_atomlike(d) && plc_atom_id_of(d) == g_rt_atoms.atom_nil) return 1;
        if ((int)d->v == DT_PLREF && plc_fid_name(d->slen) == g_rt_atoms.atom_dot && plc_fid_arity(d->slen) == 2 && d->p) {
            d = pl_deref(&((pl_cell_t *)d->p)[1]);
            n++;
            if (d == tort) return 0;
            if ((n & (n - 1)) == 0) tort = d;
            continue;
        }
        return 0;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_type_test_cell(void *cell_term, const char *fn) {
    pl_cell_t *d = cell_term ? pl_deref((pl_cell_t *)cell_term) : (pl_cell_t *)0;
    int isvar = (!d || pl_cell_unbound(d));
    int isatom = (d && !isvar && (int)d->v == DT_PLATOM);
    int isstr = (d && !isvar && (int)d->v == DT_S);
    int isint = (d && !isvar && ((int)d->v == DT_I || (int)d->v == DT_BIG));
    int isfloat = (d && !isvar && (int)d->v == DT_R);
    int iscomp = (d && !isvar && (int)d->v == DT_PLREF);
    int israt = (d && !isvar && pl_rat_is(*d));
    int isblob = (d && !isvar && rt_pl_eng_inst_is(*d));
    if (!fn) return 0;
    if (strcmp(fn, "var") == 0) return isvar ? 1 : 0;
    if (strcmp(fn, "nonvar") == 0) return !isvar ? 1 : 0;
    if (strcmp(fn, "atom") == 0) return isatom ? 1 : 0;
    if (strcmp(fn, "integer") == 0) return isint ? 1 : 0;
    if (strcmp(fn, "float") == 0) return isfloat ? 1 : 0;
    if (strcmp(fn, "number") == 0) return (isint || isfloat || israt) ? 1 : 0;
    if (strcmp(fn, "rational") == 0) return (isint || israt) ? 1 : 0;
    if (strcmp(fn, "atomic") == 0) return (isatom || isstr || isint || isfloat || israt || isblob) ? 1 : 0;
    if (strcmp(fn, "string") == 0) return isstr ? 1 : 0;
    if (strcmp(fn, "compound") == 0) return iscomp ? 1 : 0;
    if (strcmp(fn, "callable") == 0) return (isatom || iscomp) ? 1 : 0;
    if (strcmp(fn, "ground") == 0) return plc_cell_is_ground((pl_cell_t *)cell_term) ? 1 : 0;
    if (strcmp(fn, "is_list") == 0) return plc_cell_is_proper_list((pl_cell_t *)cell_term) ? 1 : 0;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int plc_swi_list_name(int name, int arity) { extern int rt_pl_dialect_swi(void); return (arity == 2 && name == g_rt_atoms.atom_dot && rt_pl_dialect_swi()) ? prolog_atom_intern("[|]") : name; }
int plc_swi_list_build(int name, int arity) { extern int rt_pl_dialect_swi(void); return (arity == 2 && rt_pl_dialect_swi() && name == prolog_atom_intern("[|]")) ? g_rt_atoms.atom_dot : name; }
int rt_pl_functor_cell(void *t0_cell, void *name_cell, void *arity_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    pl_cell_t *d1 = name_cell ? pl_deref((pl_cell_t *)name_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t nameV, arityV;
        if ((int)d0->v == DT_PLREF && plc_fid_arity(d0->slen) == 0 && !rt_pl_dialect_xsb()) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (cx) cx->ball = rt_pl_ball_kind2("domain_error", "compound_non_zero_arity", *d0);
            return 0;
        }
        if ((int)d0->v == DT_PLREF) {
            nameV = plc_atom_id_cell(plc_swi_list_name(plc_fid_name(d0->slen), plc_fid_arity(d0->slen)));
            arityV = pl_make_int((int64_t)plc_fid_arity(d0->slen));
        } else if ((int)d0->v == DT_S) {
            nameV = *d0;
            arityV = pl_make_int(0);
        } else if (plc_is_atomlike(d0)) {
            nameV = plc_atom_id_cell(plc_atom_id_of(d0));
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_I) {
            nameV = pl_make_int(d0->i);
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_R) {
            nameV = pl_make_float(d0->r);
            arityV = pl_make_int(0);
        } else if ((int)d0->v == DT_BIG || pl_rat_is(*d0)) {
            nameV = *d0;
            arityV = pl_make_int(0);
        } else {
            return 0;
        }
        if (!plc_unify_cells_cx((pl_cell_t *)name_cell, &nameV, cx) || !plc_unify_cells_cx((pl_cell_t *)arity_cell, &arityV, cx)) { return 0; }
        return 1;
    }
    pl_cell_t *d2 = arity_cell ? pl_deref((pl_cell_t *)arity_cell) : (pl_cell_t *)0;
    if (!d2 || (int)d2->v != DT_I) { return 0; }
    long ar = (long)d2->i;
    pl_cell_t built;
    if (ar == 0) {
        built = d1 ? *d1 : plc_atom_id_cell(prolog_atom_intern("[]"));
    } else {
        if (!d1 || !plc_is_atomlike(d1)) { return 0; }
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)ar * sizeof(pl_cell_t));
        for (long i = 0; i < ar; i++) pl_init_var(&args[i], cx);
        built = pl_make_compound(plc_swi_list_build(plc_atom_id_of(d1), (int)ar), (int)ar, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
int rt_pl_univ_cell(void *t0_cell, void *list_cell, pl_tr_ctx_t *cx) {
    pl_cell_t *d0 = t0_cell ? pl_deref((pl_cell_t *)t0_cell) : (pl_cell_t *)0;
    if (d0 && !pl_cell_unbound(d0)) {
        pl_cell_t lst = plc_atom_id_cell(prolog_atom_intern("[]"));
        if ((int)d0->v == DT_PLREF && plc_fid_arity(d0->slen) == 0 && !rt_pl_dialect_xsb()) {
            extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
            if (cx) cx->ball = rt_pl_ball_kind2("domain_error", "compound_non_zero_arity", *d0);
            return 0;
        }
        if ((int)d0->v == DT_PLREF) {
            int ar = plc_fid_arity(d0->slen);
            pl_cell_t *aa = (pl_cell_t *)d0->p;
            for (int i = ar - 1; i >= 0; i--) { pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t)); c[0] = aa[i]; c[1] = lst; lst = pl_make_compound(g_rt_atoms.atom_dot, 2, c); }
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = plc_atom_id_cell(plc_swi_list_name(plc_fid_name(d0->slen), plc_fid_arity(d0->slen)));
            c[1] = lst;
            lst = pl_make_compound(g_rt_atoms.atom_dot, 2, c);
        } else {
            pl_cell_t *c = (pl_cell_t *)PL_CELL_ALLOC(2 * sizeof(pl_cell_t));
            c[0] = *d0;
            c[1] = lst;
            lst = pl_make_compound(g_rt_atoms.atom_dot, 2, c);
        }
        if (!plc_unify_cells_cx((pl_cell_t *)list_cell, &lst, cx)) { return 0; }
        return 1;
    }
    extern void *rt_pl_ball_instantiation(void);
    extern void *rt_pl_ball_kind1(const char *, const char *);
    extern void *rt_pl_ball_kind2(const char *, const char *, DESCR_t);
    pl_cell_t *l0 = list_cell ? pl_deref((pl_cell_t *)list_cell) : (pl_cell_t *)0, *cur = l0;
    void *b = (void *)0;
    int ne = 0;
    long nl = plc_list_cells(list_cell, g_rt_atoms.atom_dot);
    while (nl >= 0 && cur && (int)cur->v == DT_PLREF && plc_fid_name(cur->slen) == g_rt_atoms.atom_dot && plc_fid_arity(cur->slen) == 2 && cur->p) { ne++; cur = pl_deref(&((pl_cell_t *)cur->p)[1]); }
    if (nl < 0) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (!cur || pl_cell_unbound(cur)) b = rt_pl_ball_instantiation();
    else if (!(plc_is_atomlike(cur) && plc_atom_id_of(cur) == g_rt_atoms.atom_nil)) b = rt_pl_ball_kind2("type_error", "list", *l0);
    else if (ne == 0) b = rt_pl_ball_kind2("domain_error", "non_empty_list", *l0);
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t *h = pl_deref(&((pl_cell_t *)l0->p)[0]);
    if (pl_cell_unbound(h)) b = rt_pl_ball_instantiation();
    else if (ne == 1 && (int)h->v == DT_PLREF) b = rt_pl_ball_kind2("type_error", "atomic", *h);
    else if (ne > 1 && !plc_is_atomlike(h)) b = rt_pl_ball_kind2("type_error", "atom", *h);
    else if ((long)ne - 1 > PROLOG_MAX_ARITY) b = rt_pl_ball_kind1("representation_error", "max_arity");
    if (b) { if (cx && !cx->ball) cx->ball = b; return 0; }
    pl_cell_t built;
    if (ne == 1) {
        built = *h;
    } else {
        pl_cell_t *args = (pl_cell_t *)PL_CELL_ALLOC((size_t)(ne - 1) * sizeof(pl_cell_t));
        int i = 0;
        for (cur = pl_deref(&((pl_cell_t *)l0->p)[1]); i < ne - 1; cur = pl_deref(&((pl_cell_t *)cur->p)[1])) args[i++] = *pl_deref(&((pl_cell_t *)cur->p)[0]);
        built = pl_make_compound(plc_swi_list_build(plc_atom_id_of(h), ne - 1), ne - 1, args);
    }
    if (!plc_unify_cells_cx((pl_cell_t *)t0_cell, &built, cx)) { return 0; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_text_eq(const DESCR_t *A, const DESCR_t *B) {
    if ((int)A->v == DT_PLATOM && (int)B->v == DT_PLATOM) return A->i == B->i;
    if ((int)A->v != DT_S || (int)B->v != DT_S) return 0;
    return A->slen == B->slen && (!A->slen || !memcmp(A->s, B->s, A->slen));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_pl_cell_class(pl_cell_t *d) {
    int t = (int)d->v;
    if (pl_cell_unbound(d)) return 0;
    if (t == DT_I || t == DT_R || t == DT_BIG || pl_rat_is(*d)) return 1;
    if (t == DT_S) return 2;
    if (t == DT_PLATOM) return 3;
    if (t == DT_PLREF) return 4;
    if (rt_pl_eng_inst_is(*d)) return 3;
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static double plc_big_real(pl_cell_t *d) { extern char *rt_big_str(DESCR_t); char *t = rt_big_str(*d); return t ? strtod(t, (char **)0) : 0.0; }
int rt_pl_cell_compare_leaf(pl_cell_t *a, pl_cell_t *b, void *ctx) {
    if ((int)a->v == DT_PLREF && (int)b->v == DT_PLREF && a->slen == b->slen) return PLR_DESCEND;
    if ((int)a->v == DT_I && (int)b->v == DT_I) return a->i < b->i ? -1 : (a->i > b->i ? 1 : 0);
    int cla = rt_pl_cell_class(a), clb = rt_pl_cell_class(b);
    if (cla != clb) return cla < clb ? -1 : 1;
    if (cla == 0) { uint64_t sa = pl_var_serial(a), sb = pl_var_serial(b); if (a == b) return 0; if (sa != sb) return sa < sb ? -1 : 1; return (uintptr_t)a < (uintptr_t)b ? -1 : 1; }
    if (cla == 1 && (pl_rat_is(*a) || pl_rat_is(*b))) {
        if ((int)a->v == DT_R || (int)b->v == DT_R) {
            double x = (int)a->v == DT_R ? a->r : pl_rat_real(*a), y = (int)b->v == DT_R ? b->r : pl_rat_real(*b);
            if (x != y) return x < y ? -1 : 1;
            return (int)a->v == DT_R ? -1 : 1;
        }
        { int c = pl_rat_cmp(*a, *b); return c < 0 ? -1 : (c > 0 ? 1 : 0); }
    }
    if (cla == 1 && ((int)a->v == DT_R) != ((int)b->v == DT_R) && (rt_pl_dialect_gnu() || rt_pl_dialect_xsb())) return ((int)a->v == DT_R) ? -1 : 1;
    if (cla == 1 && ((int)a->v == DT_BIG || (int)b->v == DT_BIG) && (int)a->v != DT_R && (int)b->v != DT_R) {
        extern int rt_big_cmp(DESCR_t, DESCR_t);
        int rc = rt_big_cmp(*a, *b);
        return rc < 0 ? -1 : (rc > 0 ? 1 : 0);
    }
    if (cla == 1) {
        double x = ((int)a->v == DT_I) ? (double)a->i : (int)a->v == DT_BIG ? plc_big_real(a) : a->r, y = ((int)b->v == DT_I) ? (double)b->i : (int)b->v == DT_BIG ? plc_big_real(b) : b->r;
        if (isnan(x) || isnan(y)) return isnan(x) && isnan(y) ? 0 : isnan(x) ? -1 : 1;
        if (x < y) return -1;
        if (x > y) return 1;
        if ((int)a->v == DT_R && (int)b->v == DT_R && signbit(x) != signbit(y)) return signbit(x) ? -1 : 1;
        if ((int)a->v == (int)b->v) return 0;
        return ((int)a->v == DT_R) ? -1 : 1;
    }
    if (cla == 2) {
        uint32_t n = a->slen < b->slen ? a->slen : b->slen;
        int c = n ? memcmp(a->s, b->s, n) : 0;
        if (!c) c = a->slen < b->slen ? -1 : a->slen > b->slen ? 1 : 0;
        return c < 0 ? -1 : (c > 0 ? 1 : 0);
    }
    if (cla == 3 && ((int)a->v == DT_DATA || (int)b->v == DT_DATA)) {
        if ((int)a->v == DT_DATA && (int)b->v == DT_DATA) { int64_t x = rt_pl_eng_inst_id(*a), y = rt_pl_eng_inst_id(*b); return x < y ? -1 : x > y ? 1 : 0; }
        return (int)a->v == DT_DATA ? 1 : -1;
    }
    if (cla == 3) {
        if (a->i == b->i) return 0;
        if (rt_pl_dialect_swi() && plc_is_nil(a) != plc_is_nil(b)) return plc_is_nil(a) ? -1 : 1;
        size_t la = plc_atom_len(a), lb = plc_atom_len(b);
        int c = memcmp(rt_pl_cell_name(a), rt_pl_cell_name(b), la < lb ? la : lb);
        if (!c) c = la < lb ? -1 : la > lb ? 1 : 0;
        return c < 0 ? -1 : (c > 0 ? 1 : 0);
    }
    int ara = plc_fid_arity(a->slen), arb = plc_fid_arity(b->slen);
    if (ara != arb) return ara < arb ? -1 : 1;
    if ((plc_fid_name(a->slen)) != (plc_fid_name(b->slen))) {
        const char *na = prolog_atom_name(plc_fid_name(a->slen)), *nb = prolog_atom_name(plc_fid_name(b->slen));
        size_t la = na ? prolog_atom_len(plc_fid_name(a->slen)) : 0, lb = nb ? prolog_atom_len(plc_fid_name(b->slen)) : 0;
        int c = memcmp(na ? na : "", nb ? nb : "", la < lb ? la : lb);
        if (!c) c = la < lb ? -1 : la > lb ? 1 : 0;
        if (c) return c < 0 ? -1 : 1;
    }
    (void)ctx;
    return PLR_DESCEND;
}
static const void *plc_cmp_find(const void **mk, const void **mv, const void *p) {
    for (int g = 0; g < PLC_CMP_MAP; g++) {
        size_t h = ((uintptr_t)p >> 4) * 0x9E3779B97F4A7C15ull >> 50;
        while (mk[h] && mk[h] != p) h = (h + 1) & (PLC_CMP_MAP - 1);
        if (!mk[h]) return p;
        p = mv[h];
    }
    return p;
}
int plc_cmp_bounded(pl_cell_t *ca, pl_cell_t *cb, int use_map, int *ok) {
    int cap = 64, top = 0, used = 0;
    plc_cmp_fr_t *fr = (plc_cmp_fr_t *)alloca(sizeof *fr * (size_t)cap);
    long steps = 0;
    const void **mk = (const void **)0, **mv = (const void **)0;
    if (use_map) { mk = (const void **)alloca(sizeof(void *) * PLC_CMP_MAP); mv = (const void **)alloca(sizeof(void *) * PLC_CMP_MAP); memset(mk, 0, sizeof(void *) * PLC_CMP_MAP); }
    *ok = 1;
    for (;;) {
        pl_cell_t *A = pl_deref(ca), *B = pl_deref(cb);
        if (A != B) {
            int r = rt_pl_cell_compare_leaf(A, B, (void *)0);
            if (r == PLR_DESCEND) {
                int ar = plc_fid_arity(A->slen);
                int same = 0;
                if (use_map) {
                    const void *ra = plc_cmp_find(mk, mv, A->p), *rb = plc_cmp_find(mk, mv, B->p);
                    if (ra == rb) same = 1;
                    else {
                        if (used * 4 >= PLC_CMP_MAP * 3) { *ok = 0; return 0; }
                        size_t h = ((uintptr_t)ra >> 4) * 0x9E3779B97F4A7C15ull >> 50;
                        while (mk[h]) h = (h + 1) & (PLC_CMP_MAP - 1);
                        mk[h] = ra;
                        mv[h] = rb;
                        used++;
                    }
                } else if (A->p == B->p) same = 1;
                if (!same && ar > 0) {
                    while (top > 0 && fr[top - 1].i >= fr[top - 1].n) top--;
                    if (!use_map && ++steps > (1L << 16)) { *ok = 0; return 0; }
                    if (top >= cap) { plc_cmp_fr_t *nf = (plc_cmp_fr_t *)alloca(sizeof *nf * (size_t)cap * 2u); memcpy(nf, fr, sizeof *fr * (size_t)top); fr = nf; cap *= 2; }
                    fr[top].a = (pl_cell_t *)A->p;
                    fr[top].b = (pl_cell_t *)B->p;
                    fr[top].i = 0;
                    fr[top].n = ar;
                    top++;
                }
            } else if (r) return r;
        }
        while (top > 0 && fr[top - 1].i >= fr[top - 1].n) top--;
        if (top == 0) return 0;
        ca = &fr[top - 1].a[fr[top - 1].i];
        cb = &fr[top - 1].b[fr[top - 1].i];
        fr[top - 1].i++;
    }
}
__attribute__((noinline)) int rt_pl_bind_vv(pl_cell_t *A, pl_cell_t *B) {
    char probe;
    int as = (char *)A > &probe, bs = (char *)B > &probe;
    pl_cell_t *from = as && bs ? (A < B ? A : B) : as ? A : bs ? B : (A > B ? A : B);
    pl_cell_t *to = from == A ? B : A;
    pl_cell_t r = pl_make_ref(to, 0);
    if ((int)to->v != DT_PLVAR || to->p != (void *)to) pl_init_var(to, (const pl_tr_ctx_t *)0);
    *from = r;
    return 1;
}
static int plc_unify_leaf(pl_cell_t *A, pl_cell_t *B, void *ctx) {
    int av = pl_cell_unbound(A), bv = pl_cell_unbound(B);
    (void)ctx;
    if (av && bv && pl_attv_is(A) != pl_attv_is(B)) { pl_cell_t r = pl_make_ref(pl_attv_is(A) ? A : B, 0); pl_bind(pl_attv_is(A) ? B : A, r); return 1; }
    if (av && bv) return rt_pl_bind_vv(A, B);
    if (av) { pl_bind(A, *B); return 1; }
    if (bv) { pl_bind(B, *A); return 1; }
    if ((int)A->v == DT_PLATOM && (int)B->v == DT_PLATOM) return A->i == B->i;
    if (((int)A->v == DT_S || (int)A->v == DT_PLATOM) && ((int)B->v == DT_S || (int)B->v == DT_PLATOM)) return rt_pl_text_eq(A, B);
    if (A->v != B->v) return 0;
    if ((int)A->v == DT_I) return A->i == B->i;
    if ((int)A->v == DT_R) return (isnan(A->r) && isnan(B->r)) || !memcmp(&A->r, &B->r, sizeof A->r);
    if ((int)A->v == DT_PLREF) return A->slen == B->slen ? PLR_DESCEND : 0;
    return 0;
}
PLR_WALK2(plc_unify_walk, plc_unify_leaf, pl_deref) int rt_pl_unify_cyc_plain(pl_cell_t *a, pl_cell_t *b) { return plc_unify_walk(a, b, (void *)0, 1); }
static plc_cc_t *plc_cc_at(plc_cp_t *w, pl_cell_t *aa) { return &w->cc[((uintptr_t)aa >> 4) & (PLC_CC - 1)]; }
static pl_cell_t *plc_cc_get(plc_cp_t *w, pl_cell_t *aa, uint32_t sl) { plc_cc_t *e = plc_cc_at(w, aa); return (e->aa == aa && e->sl == sl) ? e->na : (pl_cell_t *)0; }
static void plc_cc_put(plc_cp_t *w, pl_cell_t *aa, uint32_t sl, pl_cell_t *na) { plc_cc_t *e = plc_cc_at(w, aa); e->aa = aa; e->sl = sl; e->na = na; }
int plc_copy_run(void *wv) {
    plc_cp_t *w = (plc_cp_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        if (d->v == PLR_COPY) *w->d = pl_make_compound(plc_fid_name(d->slen), plc_fid_arity(d->slen), d->p);
        else if (d->v == PLR_VCOPY) {
            pl_cell_t *f = (pl_cell_t *)d->p;
            *w->d = pl_make_ref(f, (int)f->slen);
        } else if (pl_cell_unbound(d)) {
            pl_cell_t *fresh;
            uint64_t w0;
            int at = w->atts && pl_attv_is(d);
            if (!plr_room(&w->lg) || (at && !plr_room(&w->ag))) { if (plr_refuses(&w->ag, &w->lg)) break; return plr_more(&w->ag, &w->lg, plc_copy_run, w); }
            fresh = (pl_cell_t *)rt_ws_alloc_descr(at ? 2 : 1);
            if (!fresh) *w->d = *d;
            else {
                if (at) pl_var_make(fresh, w->cx, 1);
                else pl_init_var(fresh, w->cx);
                if (at && pl_tr_residue_on(w->cx)) { memset(&fresh[1], 0, sizeof fresh[1]); pl_tr_push_residue(w->cx, fresh + 1); }
                memcpy(&w0, d, 8);
                plr_push(&w->lg, d, d->p, w0);
                d->v = PLR_VCOPY;
                d->p = (void *)fresh;
                *w->d = pl_make_ref(fresh, (int)fresh->slen);
                if (at) plr_push(&w->ag, d + 1, fresh + 1, 1);
            }
        } else if ((int)d->v == DT_PLREF) {
            int fn = plc_fid_name(d->slen), ar = plc_fid_arity(d->slen);
            pl_cell_t *aa = (pl_cell_t *)d->p, *na, *hit = ar > 0 ? plc_cc_get(w, aa, d->slen) : (pl_cell_t *)0;
            uint64_t w0;
            if (hit) *w->d = pl_make_compound(fn, ar, hit);
            else if (!plr_room(&w->lg) || (ar > 0 && !plr_room(&w->ag))) {
                if (plr_refuses(&w->ag, &w->lg)) break;
                return plr_more(&w->ag, &w->lg, plc_copy_run, w);
            } else if (!(na = (pl_cell_t *)rt_ws_alloc_descr((size_t)(ar > 0 ? ar : 1)))) *w->d = *d;
            else {
                if (ar > 0) plc_cc_put(w, aa, d->slen, na);
                memcpy(&w0, d, 8);
                *w->d = pl_make_compound(fn, ar, na);
                plr_push(&w->lg, d, (void *)aa, w0);
                d->v = PLR_COPY;
                d->p = (void *)na;
                if (ar > 0) plr_push(&w->ag, aa, na, (uint64_t)ar);
            }
        } else *w->d = *d;
        if (plr_empty(&w->ag)) break;
        { plr_ent_t *t = plr_peek(&w->ag); w->s = (pl_cell_t *)t->x; w->d = (pl_cell_t *)t->y; t->x = (void *)(w->s + 1); t->y = (void *)(w->d + 1); if (!--t->n) plr_drop(&w->ag); }
    }
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
static int plc_sh_room2(const plr_stk_t *s) { return s->top && (s->top->cap - s->top->n >= 2 || s->top->next); }
static void plc_sh_changed(plc_cp_t *w) { if (!plr_empty(&w->ag)) plr_peek(&w->ag)->n |= PLC_SH_CHG; }
static int plc_sh_more(plc_cp_t *w) {
    long na = plc_sh_room2(&w->ag) ? 0 : w->ag.want, nb = plr_room(&w->lg) ? 0 : w->lg.want;
    plr_ent_t ea[na > 0 ? na : 1], eb[nb > 0 ? nb : 1];
    plr_seg_t sa, sb;
    if (na) plr_link(&w->ag, &sa, ea, na);
    if (nb) plr_link(&w->lg, &sb, eb, nb);
    return plc_copy_share_run(w);
}
int plc_copy_share_run(void *wv) {
    plc_cp_t *w = (plc_cp_t *)wv;
    for (;;) {
        pl_cell_t *d = plr_resolve(pl_deref(w->s));
        if (d->v == PLR_COPY) {
            *w->d = pl_make_compound(plc_fid_name(d->slen), plc_fid_arity(d->slen), d->p);
            plc_sh_changed(w);
        } else if (d->v == PLR_VCOPY) {
            pl_cell_t *f = (pl_cell_t *)d->p;
            *w->d = pl_make_ref(f, (int)f->slen);
            if (f != d) plc_sh_changed(w);
        } else if (pl_cell_unbound(d)) {
            pl_cell_t *fresh;
            uint64_t w0;
            int at = w->atts && pl_attv_is(d);
            if (!plr_room(&w->lg) || (at && !plr_room(&w->ag))) { if (plr_refuses(&w->ag, &w->lg)) goto done; return plc_sh_more(w); }
            fresh = (pl_cell_t *)rt_ws_alloc_descr(at ? 2 : 1);
            if (at) pl_var_make(fresh, w->cx, 1);
            else pl_init_var(fresh, w->cx);
            if (at && pl_tr_residue_on(w->cx)) { memset(&fresh[1], 0, sizeof fresh[1]); pl_tr_push_residue(w->cx, fresh + 1); }
            memcpy(&w0, d, 8);
            plr_push(&w->lg, d, d->p, w0);
            d->v = PLR_VCOPY;
            d->p = (void *)fresh;
            *w->d = pl_make_ref(fresh, (int)fresh->slen);
            plc_sh_changed(w);
            if (at) plr_push(&w->ag, d + 1, fresh + 1, 1 | PLC_SH_ATT);
        } else if ((int)d->v == DT_PLREF) {
            int ar = plc_fid_arity(d->slen);
            pl_cell_t *aa = (pl_cell_t *)d->p, *na;
            uint64_t w0;
            if (ar <= 0) *w->d = *d;
            else if ((na = plc_cc_get(w, aa, d->slen))) {
                *w->d = pl_make_compound(plc_fid_name(d->slen), ar, na);
                plc_sh_changed(w);
            } else {
                if (!plr_room(&w->lg) || !plc_sh_room2(&w->ag)) { if (plr_refuses(&w->ag, &w->lg)) goto done; return plc_sh_more(w); }
                na = (pl_cell_t *)rt_ws_alloc_descr((size_t)ar);
                plc_cc_put(w, aa, d->slen, na);
                memcpy(&w0, d, 8);
                *w->d = pl_make_compound(plc_fid_name(d->slen), ar, na);
                plr_push(&w->lg, d, (void *)aa, w0);
                plr_push(&w->ag, (void *)w->d, (void *)aa, PLC_SH_FRAME | (uint64_t)d->slen);
                w->nfr++;
                plr_push(&w->ag, (void *)aa, (void *)na, (uint64_t)ar);
                d->v = PLR_COPY;
                d->p = (void *)na;
            }
        } else *w->d = *d;
        for (;;) {
            plr_ent_t *t;
            if (plr_empty(&w->ag)) goto done;
            t = plr_peek(&w->ag);
            if (t->n & PLC_SH_CNT) {
                w->s = (pl_cell_t *)t->x;
                w->d = (pl_cell_t *)t->y;
                t->x = (void *)(w->s + 1);
                t->y = (void *)(w->d + 1);
                if (--t->n & PLC_SH_CNT || (!(t->n & (PLC_SH_CHG | PLC_SH_ATT)) && w->nfr <= PLC_SH_BUDGET)) break;
                { uint64_t f = t->n; plr_drop(&w->ag); if (!(f & PLC_SH_ATT)) { plr_drop(&w->ag); w->nfr--; plc_sh_changed(w); } }
                break;
            }
            {
                uint64_t f = t->n;
                plr_drop(&w->ag);
                if (f & PLC_SH_ATT) continue;
                t = plr_peek(&w->ag);
                {
                    pl_cell_t *dst = (pl_cell_t *)t->x, r;
                    memset(&r, 0, sizeof r);
                    r.v = (DTYPE_t)DT_PLREF;
                    r.slen = (uint32_t)(t->n & PLC_SH_CNT);
                    r.p = t->y;
                    plr_drop(&w->ag);
                    w->nfr--;
                    if (!(f & PLC_SH_CHG)) *dst = r;
                    else plc_sh_changed(w);
                }
            }
        }
    }
    done:
    for (plr_seg_t *g = w->lg.top; g; g = g->prev) for (long i = g->n; i-- > 0; ) { pl_cell_t *c = (pl_cell_t *)g->e[i].x; uint64_t w0 = g->e[i].n; memcpy(c, &w0, 8); c->p = g->e[i].y; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_evaluable(const char *name, int arity) {
    extern void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar);
    return rt_pl_ball_type_pi("type_error", "evaluable", name, arity);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_type_pi(const char *kind, const char *what, const char *nm, int ar) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t te[2];
    pl_cell_t *tec;
    pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    te[0] = pl_make_atom(prolog_atom_intern(what ? what : "evaluable"));
    te[1] = *pic;
    tec = (pl_cell_t *)rt_pl_compound_cell(kind ? kind : "type_error", 2, (void *)te);
    if (!tec) return (void *)0;
    er[0] = *tec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_eval_error(const char *kind, const char *op, int arity) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t ee[1];
    pl_cell_t *eec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t ct[2];
    pl_cell_t *ctc;
    pl_cell_t er[2];
    ee[0] = pl_make_atom(prolog_atom_intern(kind ? kind : "zero_divisor"));
    eec = (pl_cell_t *)rt_pl_compound_cell("evaluation_error", 1, (void *)ee);
    if (!eec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(op ? op : "is"));
    pi[1] = pl_make_int(arity);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    ct[0] = *pic;
    ct[1] = rt_pl_fresh_var_ref(0);
    ctc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ct);
    if (!ctc) return (void *)0;
    er[0] = *eec;
    er[1] = *ctc;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind1_system_ctx(const char *kind, const char *arg0_atom, const char *nm, int ar, const char *msg) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t sy[2];
    pl_cell_t *syc;
    pl_cell_t ct[2];
    pl_cell_t *ctc;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    sy[0] = pl_make_atom(prolog_atom_intern("system"));
    sy[1] = *pic;
    syc = (pl_cell_t *)rt_pl_compound_cell(":", 2, (void *)sy);
    if (!syc) return (void *)0;
    ct[0] = *syc;
    ct[1] = msg ? pl_make_atom(prolog_atom_intern(msg)) : rt_pl_fresh_var_ref(0);
    ctc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ct);
    if (!ctc) return (void *)0;
    er[0] = *fec;
    er[1] = *ctc;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2(const char *kind, const char *arg0_atom, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t fe[2];
    pl_cell_t *fec;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_occurs(DESCR_t *var, DESCR_t *term) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t oc[2];
    pl_cell_t *occ;
    pl_cell_t er[2];
    oc[0].v = (DTYPE_t)DT_PLVAR;
    oc[0].slen = 0;
    oc[0].p = (void *)var;
    oc[1] = *term;
    occ = (pl_cell_t *)rt_pl_compound_cell("occurs_check", 2, (void *)oc);
    if (!occ) return (void *)0;
    er[0] = rt_pl_cell_snapshot(occ);
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind2_pi(const char *kind, const char *arg0_atom, DESCR_t culprit, const char *nm, int ar) {
    pl_cell_t fe[2];
    pl_cell_t *fec;
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t er[2];
    fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fe[1] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 2, (void *)fe);
    if (!fec) return (void *)0;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    er[0] = *fec;
    er[1] = *pic;
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_set_pi(void *ball, const char *nm, int ar) {
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t *b = (pl_cell_t *)ball;
    if (!b || (int)b->v != DT_PLREF || !b->p) return ball;
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (pic) ((pl_cell_t *)b->p)[1] = *pic;
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_pl_ball_xsb_arith_kind(void *ball) {
    pl_cell_t *b = (pl_cell_t *)ball, *f, *w;
    if (!b || (int)b->v != DT_PLREF || !b->p || !pl_cell_unbound(pl_deref(&((pl_cell_t *)b->p)[1]))) return 0;
    f = pl_deref(&((pl_cell_t *)b->p)[0]);
    if ((int)f->v == DT_PLATOM || (int)f->v == DT_S) return !strcmp(plc_atom_text(f), "instantiation_error") ? 1 : 0;
    if ((int)f->v != DT_PLREF || !f->p || plc_fid_arity(f->slen) != 2 || strcmp(prolog_atom_name(plc_fid_name(f->slen)), "type_error")) return 0;
    w = pl_deref(&((pl_cell_t *)f->p)[0]);
    if ((int)w->v != DT_PLATOM && (int)w->v != DT_S) return 0;
    return !strcmp(plc_atom_text(w), "evaluable") ? 2 : !strcmp(plc_atom_text(w), "integer") ? 3 : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_set_xsb_arith_msg(void *ball, const char *msg, const char *goal) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t in[2], ou[2], *inc, *ouc, *b = (pl_cell_t *)ball;
    if (!b || (int)b->v != DT_PLREF || !b->p) return ball;
    in[0] = pl_make_atom(prolog_atom_intern(msg ? msg : ""));
    in[1] = pl_make_atom(prolog_atom_intern(goal ? goal : "NULL"));
    inc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)in);
    if (!inc) return ball;
    ou[0] = *inc;
    ou[1] = rt_pl_fresh_var_ref(0);
    ouc = (pl_cell_t *)rt_pl_compound_cell("context", 2, (void *)ou);
    if (ouc) ((pl_cell_t *)b->p)[1] = *ouc;
    return ball;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rt_pl_writeq_text(DESCR_t d, int ignore_ops, size_t *len) {
    extern FILE *fh_memsink_open(char **, size_t *);
    char *bp = (char *)0;
    size_t bn = 0;
    FILE *ms = fh_memsink_open(&bp, &bn);
    plc_vmap m;
    *len = 0;
    if (!ms) return "";
    plc_atoms_ready();
    m.n = 0;
    m.vnc = 0;
    m.ibase = 0;
    m.inamed = 0;
    m.ipfx = 1;
    m.xstr = 0;
    m.fprec = -1;
    m.fspec = 0;
    m.xopf = 0;
    m.fp = ms;
    m.portray = (int (*)(pl_cell_t *, plc_vmap *))0;
    m.pthrown = (void *)0;
    m.cx = (pl_tr_ctx_t *)0;
    m.pheld = 0;
    m.pgoal = (pl_cell_t *)0;
    m.popts = (pl_cell_t *)0;
    m.space_args = 0;
    m.next_arg = 0;
    if (ignore_ops) plc_wt((pl_cell_t *)&d, 1, 1, 1, -1, 0, 1200, &m);
    else plc_writeq((pl_cell_t *)&d, &m);
    fclose(ms);
    if (!bp) return "";
    *len = bn;
    return bp;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_culprit1(const char *kind, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t er[2];
    fe[0] = rt_pl_cell_snapshot(&culprit);
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_kind1(const char *kind, const char *arg0_atom) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t fe[1];
    pl_cell_t *fec;
    pl_cell_t er[2];
    const char *sep = arg0_atom ? strchr(arg0_atom, 1) : (const char *)0;
    if (sep) {
        size_t k = (size_t)(sep - arg0_atom);
        char nm[k + 1];
        pl_cell_t ia[1];
        pl_cell_t *ic;
        memcpy(nm, arg0_atom, k);
        nm[k] = 0;
        ia[0] = pl_make_atom(prolog_atom_intern(sep + 1));
        ic = (pl_cell_t *)rt_pl_compound_cell(nm, 1, (void *)ia);
        if (!ic) return (void *)0;
        fe[0] = *ic;
    } else fe[0] = pl_make_atom(prolog_atom_intern(arg0_atom ? arg0_atom : "term"));
    fec = (pl_cell_t *)rt_pl_compound_cell(kind, 1, (void *)fe);
    if (!fec) return (void *)0;
    er[0] = *fec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_instantiation(void) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t er[2];
    er[0] = pl_make_atom(prolog_atom_intern("instantiation_error"));
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission_pi(const char *op, const char *type, const char *nm, int ar) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t pi[2];
    pl_cell_t *pic;
    pl_cell_t pe[3];
    pl_cell_t *pec;
    pl_cell_t er[2];
    pi[0] = pl_make_atom(prolog_atom_intern(nm ? nm : "?"));
    pi[1] = pl_make_int(ar);
    pic = (pl_cell_t *)rt_pl_compound_cell("/", 2, (void *)pi);
    if (!pic) return (void *)0;
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "modify"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "static_procedure"));
    pe[2] = *pic;
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *rt_pl_ball_permission3(const char *op, const char *type, DESCR_t culprit) {
    extern DESCR_t rt_pl_fresh_var_ref(uint64_t);
    pl_cell_t pe[3];
    pl_cell_t *pec;
    pl_cell_t er[2];
    pe[0] = pl_make_atom(prolog_atom_intern(op ? op : "access"));
    pe[1] = pl_make_atom(prolog_atom_intern(type ? type : "stream"));
    pe[2] = rt_pl_cell_snapshot(&culprit);
    pec = (pl_cell_t *)rt_pl_compound_cell("permission_error", 3, (void *)pe);
    if (!pec) return (void *)0;
    er[0] = *pec;
    er[1] = rt_pl_fresh_var_ref(0);
    return rt_pl_compound_cell("error", 2, (void *)er);
}
pl_cell_t pl_trie_key_term(pl_trie_node_t *n) {
    int d = n->depth, nv = 0, sp = 0;
    pl_trie_node_t *path[d > 0 ? d : 1];
    pl_cell_t *slots[d > 0 ? d : 1];
    pl_cell_t out;
    memset(&out, 0, sizeof out);
    for (pl_trie_node_t *c = n; c && c->kind; c = c->parent) { path[c->depth - 1] = c; if (c->kind == 3 && (int)c->tok.i + 1 > nv) nv = (int)c->tok.i + 1; }
    {
        pl_cell_t *vars[nv > 0 ? nv : 1];
        for (int i = 0; i < nv; i++) vars[i] = (pl_cell_t *)0;
        slots[sp++] = &out;
        for (int i = 0; i < d && sp > 0; i++) {
            pl_trie_node_t *c = path[i];
            pl_cell_t *dst = slots[--sp];
            if (c->kind == 2) {
                int ar = plc_fid_arity((uint32_t)c->tok.i);
                pl_cell_t *aa = ar > 0 ? (pl_cell_t *)rt_ws_alloc_descr((size_t)ar) : (pl_cell_t *)0;
                dst->v = (DTYPE_t)DT_PLREF;
                dst->slen = (uint32_t)c->tok.i;
                dst->p = (void *)aa;
                for (int k = ar - 1; k >= 0; k--) slots[sp++] = &aa[k];
            } else if (c->kind == 3) {
                int k = (int)c->tok.i;
                if (!vars[k]) { vars[k] = (pl_cell_t *)rt_ws_alloc_descr(1); pl_init_var(vars[k], (const pl_tr_ctx_t *)0); }
                *dst = pl_make_ref(vars[k], -1);
            } else *dst = c->tok;
        }
    }
    return out;
}
int rt_pl_trie_entry_key(void *root, int64_t id, int64_t ent, void *out_cell, pl_tr_ctx_t *cx) {
    pl_trie_t *t = pl_trie_at(root, id);
    pl_trie_node_t *n;
    if (!t || ent < 0 || ent >= t->nents) return 0;
    n = (pl_trie_node_t *)t->ents[ent];
    if (!n || !n->has_val) return 0;
    return pl_trie_give(pl_trie_key_term(n), out_cell, cx);
}
int rt_pl_db_key_is_declared(void *root, const char *key) { pl_db_key_t *e = pl_db_reg_find(pl_db_registry(root, 0), key); return (e && e->decl) ? 1 : 0; }
