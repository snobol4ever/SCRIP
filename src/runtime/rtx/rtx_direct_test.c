#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "descr.h"
#include "core.h"
DESCR_t rt_call_arr_bl(const char *fn, DESCR_t *args, int nargs, int bidlen);
DESCR_t rt_pas_resundef(DESCR_t *, int);
DESCR_t rt_pas_sin(DESCR_t *, int);
DESCR_t rt_pas_cos(DESCR_t *, int);
DESCR_t rt_pas_exp(DESCR_t *, int);
DESCR_t rt_pas_sqrt(DESCR_t *, int);
DESCR_t rt_pas_ln(DESCR_t *, int);
DESCR_t rt_pas_arctan(DESCR_t *, int);
DESCR_t rt_pas_sqr(DESCR_t *, int);
DESCR_t rt_rk_arr_at(DESCR_t *, int);
DESCR_t rt_rk_arr_set(DESCR_t *, int);
DESCR_t c_rt_rk_arr_at(DESCR_t *, int);
DESCR_t c_rt_rk_arr_set(DESCR_t *, int);
static int rk_leaf_hits = 0, rk_leaf_declines = 0;
static DESCR_t rk_at(DESCR_t *p) { DESCR_t d = rt_rk_arr_at(p, 2); if (d.v == RTX_NOT_HANDLED) { rk_leaf_declines++; return c_rt_rk_arr_at(p, 2); } rk_leaf_hits++; return d; }
static DESCR_t rk_set(DESCR_t *p) { DESCR_t d = rt_rk_arr_set(p, 3); if (d.v == RTX_NOT_HANDLED) { rk_leaf_declines++; return c_rt_rk_arr_set(p, 3); } rk_leaf_hits++; return d; }
DESCR_t rt_rk_elem_put(DESCR_t *, int);
DESCR_t rt_rk_param_check(DESCR_t *, int);
DESCR_t rt_rk_assign_check(DESCR_t *, int);
DESCR_t rt_rk_bindattr(DESCR_t *, int);
DESCR_t rt_rk_box(DESCR_t *, int);
DESCR_t rt_rk_blk_ref(DESCR_t *, int);
DESCR_t rt_rk_undef(DESCR_t *, int);
DESCR_t rt_rk_range_arr(DESCR_t *, int);
DESCR_t rt_rk_iter_src(DESCR_t *, int);
DESCR_t rt_rk_map_append(DESCR_t *, int);
DESCR_t rt_rk_grep_append(DESCR_t *, int);
DESCR_t rt_rk_byref_assign(DESCR_t *, int);
static int fails = 0, ncase = 0;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int same(DESCR_t a, DESCR_t b);
static int same(DESCR_t a, DESCR_t b) {
    if (a.v != b.v) return 0;
    if (a.v == DT_SNUL) return a.slen == b.slen;
    if (a.v == DT_S && a.s && b.s) return a.slen == b.slen && !memcmp(a.s, b.s, a.slen);
    if (a.v == DT_A && a.arr && b.arr) {
        ARBLK_t *x = a.arr, *y = b.arr;
        if (x == y) return 1;
        if (x->lo != y->lo || x->hi != y->hi || x->ndim != y->ndim) return 0;
        for (int i = 0; i <= x->hi - x->lo; i++) if (!same(x->data[i], y->data[i])) return 0;
        return 1;
    }
    if (a.v == DT_N && a.slen == 1 && b.slen == 1 && a.ptr && b.ptr) return same(*(DESCR_t *) a.ptr, *(DESCR_t *) b.ptr);
    if (a.v == DT_BLK && a.s && b.s) return !strcmp(a.s, b.s);
    if (a.v != DT_I && a.v != DT_R && a.v != DT_S && a.v != DT_SNUL && a.v == b.v && a.slen == b.slen) return 1;
    return memcmp(&a, &b, sizeof a) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cmp1(const char *fn, DESCR_t (*direct)(DESCR_t *, int), DESCR_t x) {
    DESCR_t a1[1] = { x }, a2[1] = { x };
    DESCR_t d = direct(a1, 1), r = rt_call_arr_bl(fn, a2, 1, -1);
    ncase++;
    if (!same(d, r)) { fails++; printf("  FAIL %s: direct %02x/%016llx by-name %02x/%016llx\n", fn, d.v & 0xff, (unsigned long long)d.i, r.v & 0xff, (unsigned long long)r.i); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_lv_is_list(DESCR_t o, DESCR_t **elems, int *n);
static int lv_model(DESCR_t o, DESCR_t **elems, int *n) {
    static DESCR_t nil[1];
    if (!(o.v == DT_DATA && o.slen == DATA_INST_SLEN) || !o.u) return 0;
    DATBLK_t *t = o.u->type;
    if (!t || t->nfields < 3 || !t->fields[0] || strcmp(t->fields[0], "frame_elems") != 0) return 0;
    DESCR_t gt = o.u->fields[2];
    if (gt.v != DT_S || !gt.s || strcmp(gt.s, "list") != 0) return 0;
    DESCR_t ea = o.u->fields[0];
    if (elems) *elems = (ea.v == DT_DATA && ea.slen == DATA_ELEMS_SLEN) ? (ea.ptr ? (DESCR_t *)ea.ptr : nil) : (DESCR_t *)0;
    if (n) *n = (int)o.u->fields[1].i;
    return 1;
}
static int lv_cases = 0;
static void lv_check(const char *what, DESCR_t o) {
    DESCR_t *e1 = (DESCR_t *)1, *e2 = (DESCR_t *)1;
    int n1 = -7, n2 = -7, r1 = rt_lv_is_list(o, &e1, &n1), r2 = lv_model(o, &e2, &n2);
    int nil1 = r1 && e1 && e1 != (DESCR_t *)1 && e1->v == 0 && (o.u ? o.u->fields[0].ptr == 0 : 0), nil2 = r2 && e2 && (o.u ? o.u->fields[0].ptr == 0 : 0);
    lv_cases++;
    ncase++;
    if (r1 != r2 || n1 != n2 || (e1 != e2 && !(nil1 && nil2 && e1 && e2))) { fails++; printf("  FAIL rt_lv_is_list %s: leaf %d/%p/%d model %d/%p/%d\n", what, r1, (void *)e1, n1, r2, (void *)e2, n2); }
    { int r3 = rt_lv_is_list(o, (DESCR_t **)0, (int *)0); if (r3 != r2) { fails++; printf("  FAIL rt_lv_is_list %s with no outputs: %d vs %d\n", what, r3, r2); } }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int pas_cases = 0;
static void pas_model(const char *nm, DESCR_t (*leaf)(DESCR_t *, int), DESCR_t x, int owns, DESCR_t want) {
    DESCR_t a[1] = { x }, d = leaf(a, 1);
    pas_cases++;
    ncase++;
    if (!owns) { if (d.v != RTX_NOT_HANDLED || memcmp(&a[0], &x, sizeof x)) { fails++; printf("  FAIL %s: an error arm answered %02x where it must decline\n", nm, d.v & 0xff); } return; }
    if (d.v != want.v || memcmp((char *)&d + 8, (char *)&want + 8, 8)) {
        fails++;
        printf("  FAIL %s: leaf %02x/%016llx model %02x/%016llx\n", nm, d.v & 0xff, (unsigned long long)d.i, want.v & 0xff, (unsigned long long)want.i);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static ARBLK_t *mk(int n, int base) { ARBLK_t *a = array_new(0, n - 1); for (int i = 0; i < n; i++) a->data[i] = INTVAL(base + i); return a; }
static DESCR_t darr(ARBLK_t *a) { DESCR_t d = { .v = DT_A }; d.arr = a; return d; }
static void cmpn(const char *fn, DESCR_t (*direct)(DESCR_t *, int), const DESCR_t *x, int n) {
    DESCR_t a1[8], a2[8];
    for (int i = 0; i < n; i++) { a1[i] = x[i]; a2[i] = x[i]; }
    DESCR_t d = direct(a1, n), r = rt_call_arr_bl(fn, a2, n, -1);
    ncase++;
    if (!same(d, r)) { fails++; printf("  FAIL %s/%d: direct %02x/%016llx by-name %02x/%016llx\n", fn, n, d.v & 0xff, (unsigned long long) d.i, r.v & 0xff, (unsigned long long) r.i); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void dump(DESCR_t d, int depth) {
    printf("%02x", d.v & 0xff);
    if (d.v == DT_I) printf(":%lld", (long long) d.i);
    else if (d.v == DT_R) printf(":%a", d.r);
    else if (d.v == DT_S && d.s) printf(":%.*s", (int) d.slen, d.s);
    else if (d.v == DT_A && d.arr && depth < 3) {
        ARBLK_t *x = d.arr;
        printf("[");
        for (int i = 0; i <= x->hi - x->lo && i < 64; i++) { dump(x->data[i], depth + 1); printf(","); }
        printf("]");
    } else if (d.v == DT_N && d.slen == 1 && d.ptr && depth < 3) {
        printf("&");
        dump(*(DESCR_t *) d.ptr, depth + 1);
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int run_side(int via_road, const char *fn, DESCR_t (*direct)(DESCR_t *, int), const DESCR_t *x, int n, char *buf, int cap) {
    int fd[2];
    fflush(stdout);
    if (pipe(fd)) return -1;
    pid_t pid = fork();
    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        dup2(fd[1], 2);
        DESCR_t a[8];
        for (int i = 0; i < n; i++) a[i] = x[i];
        DESCR_t r = via_road ? rt_call_arr_bl(fn, a, n, -1) : direct(a, n);
        printf("RESULT ");
        dump(r, 0);
        printf("\n");
        fflush(stdout);
        _exit(0);
    }
    close(fd[1]);
    int len = 0, k;
    while (len < cap - 1 && (k = (int) read(fd[0], buf + len, (size_t) (cap - 1 - len))) > 0) len += k;
    buf[len] = 0;
    close(fd[0]);
    int st = 0;
    waitpid(pid, &st, 0);
    return st;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void cmpfork(const char *fn, DESCR_t (*direct)(DESCR_t *, int), const DESCR_t *x, int n) {
    char b1[4096], b2[4096];
    int s1 = run_side(0, fn, direct, x, n, b1, sizeof b1), s2 = run_side(1, fn, direct, x, n, b2, sizeof b2);
    ncase++;
    if (s1 != s2 || strcmp(b1, b2)) { fails++; printf("  FAIL %s/%d (forked): status %d vs %d\n--- direct\n%s--- by-name\n%s", fn, n, s1, s2, b1, b2); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(void) {
    core_lib_init();
    DESCR_t xs[] = { INTVAL(0), INTVAL(1), INTVAL(-3), INTVAL(7), INTVAL(46340), INTVAL(-46340), REALVAL(0.0), REALVAL(-0.0), REALVAL(0.5), REALVAL(-2.5), REALVAL(1e10), NULVCL, STRVAL("12") };
    int nx = (int)(sizeof xs / sizeof xs[0]);
    for (int i = 0; i < nx; i++) {
        cmp1("__pas_resundef", rt_pas_resundef, xs[i]);
        cmp1("__pas_sin", rt_pas_sin, xs[i]);
        cmp1("__pas_cos", rt_pas_cos, xs[i]);
        cmp1("__pas_exp", rt_pas_exp, xs[i]);
        cmp1("__pas_arctan", rt_pas_arctan, xs[i]);
        cmp1("__pas_sqr", rt_pas_sqr, xs[i]);
        if (xs[i].v == DT_I ? xs[i].i >= 0 : (xs[i].v == DT_R ? xs[i].r >= 0 : 1)) cmp1("__pas_sqrt", rt_pas_sqrt, xs[i]);
        if (xs[i].v == DT_I ? xs[i].i > 0 : (xs[i].v == DT_R ? xs[i].r > 0 : 0)) cmp1("__pas_ln", rt_pas_ln, xs[i]);
    }
    {
        DESCR_t ms[] = { INTVAL(0), INTVAL(1), INTVAL(-3), INTVAL(7), INTVAL(46340), INTVAL(-46340), INTVAL(46341), INTVAL(-46341), INTVAL(9007199254740993LL), REALVAL(0.0), REALVAL(-0.0),
            REALVAL(0.5), REALVAL(-2.5), REALVAL(1e10), REALVAL(1e300), REALVAL(-1e300), REALVAL(INFINITY), REALVAL(-INFINITY), REALVAL(NAN), REALVAL(5e-324), NULVCL, STRVAL("12") };
        int nm = (int)(sizeof ms / sizeof ms[0]), mfail0 = fails;
        for (int i = 0; i < nm; i++) {
            DESCR_t x = ms[i];
            double r = x.v == DT_R ? x.r : (double)(x.v == DT_I ? x.i : 0);
            long iv = x.v == DT_I ? x.i : 0;
            pas_model("rt_pas_resundef", rt_pas_resundef, x, 1, INTVAL(x.v == DT_SNUL ? 1 : 0));
            pas_model("rt_pas_sin", rt_pas_sin, x, 1, REALVAL(sin(r)));
            pas_model("rt_pas_cos", rt_pas_cos, x, 1, REALVAL(cos(r)));
            pas_model("rt_pas_exp", rt_pas_exp, x, 1, REALVAL(exp(r)));
            pas_model("rt_pas_arctan", rt_pas_arctan, x, 1, REALVAL(atan(r)));
            pas_model("rt_pas_sqrt", rt_pas_sqrt, x, !(r < 0), REALVAL(sqrt(r)));
            pas_model("rt_pas_ln", rt_pas_ln, x, r > 0, REALVAL(log(r)));
            pas_model("rt_pas_sqr", rt_pas_sqr, x, x.v == DT_R || (iv <= 46340 && iv >= -46340), x.v == DT_R ? REALVAL(x.r * x.r) : INTVAL(iv * iv));
        }
        printf("RTX DIRECT pascal leaves against the libm model: %d cases, %d mismatches (an error arm declines with its argument untouched)\n", pas_cases, fails - mfail0);
    }
    {
        static char fe[] = "frame_elems", fx[] = "frame_elemz", fl[] = "frame_elemsX", fs[] = "fr", e0[] = "", lst[] = "list", lis[] = "lis", lsts[] = "lists", tbl[] = "table";
        char *f3[] = { fe, e0, e0 }, *f4[] = { fe, e0, e0, e0 }, *fxn[] = { fx, e0, e0 }, *fln[] = { fl, e0, e0 }, *fsn[] = { fs, e0, e0 }, *fnul[] = { 0, e0, e0 };
        DATBLK_t ty3 = { 0 }, ty4 = { 0 }, tyx = { 0 }, tyl = { 0 }, tys = { 0 }, tyn = { 0 }, ty2 = { 0 };
        ty3.nfields = 3;
        ty3.fields = f3;
        ty4.nfields = 4;
        ty4.fields = f4;
        tyx.nfields = 3;
        tyx.fields = fxn;
        tyl.nfields = 3;
        tyl.fields = fln;
        tys.nfields = 3;
        tys.fields = fsn;
        tyn.nfields = 3;
        tyn.fields = fnul;
        ty2.nfields = 2;
        ty2.fields = f3;
        DESCR_t el[3] = { INTVAL(1), INTVAL(2), INTVAL(3) };
        DESCR_t flds[4];
        DATINST_t in = { 0 };
        DESCR_t o = { 0 };
        o.v = DT_DATA;
        o.slen = DATA_INST_SLEN;
        o.u = &in;
        in.fields = flds;
        struct {
            const char *w;
            DATBLK_t *ty;
            int ev;
            uint32_t es;
            void *ep;
            int ok;
            char *tag;
        } cs[] = { { "a list", &ty3, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a list with four fields", &ty4, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a list with no element block", &ty3, DT_DATA,
            DATA_ELEMS_SLEN, 0, 1, lst }, { "a list whose elements are no block", &ty3, DT_I, 0, el, 1, lst }, { "the tag lis", &ty3, DT_DATA, DATA_ELEMS_SLEN, el, 1, lis }, { "the tag lists", &ty3,
            DT_DATA, DATA_ELEMS_SLEN, el, 1, lsts }, { "the tag table", &ty3, DT_DATA, DATA_ELEMS_SLEN, el, 1, tbl }, { "the tag empty", &ty3, DT_DATA, DATA_ELEMS_SLEN, el, 1, e0 },
            { "a type frame_elemz", &tyx, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a type frame_elemsX", &tyl, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a type fr", &tys, DT_DATA, DATA_ELEMS_SLEN,
            el, 1, lst }, { "a type with a NULL first field", &tyn, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a type with two fields", &ty2, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a NULL type",
            0, DT_DATA, DATA_ELEMS_SLEN, el, 1, lst }, { "a tag NULL string", &ty3, DT_DATA, DATA_ELEMS_SLEN, el, 1, 0 }, };
        int ncs = (int)(sizeof cs / sizeof cs[0]), lfail0 = fails;
        for (int i = 0; i < ncs; i++) {
            in.type = cs[i].ty;
            flds[0] = (DESCR_t){ 0 };
            flds[0].v = (uint8_t)cs[i].ev;
            flds[0].slen = cs[i].es;
            flds[0].ptr = cs[i].ep;
            flds[1] = INTVAL(3 + i);
            flds[2] = (DESCR_t){ 0 };
            flds[2].v = DT_S;
            flds[2].s = cs[i].tag;
            flds[2].slen = cs[i].tag ? (uint32_t)strlen(cs[i].tag) : 0;
            flds[3] = INTVAL(8);
            lv_check(cs[i].w, o);
        }
        { DESCR_t x = o; x.slen = DATA_ELEMS_SLEN; in.type = &ty3; lv_check("an element block, not an instance", x); }
        { DESCR_t x = o; x.u = 0; lv_check("a NULL instance", x); }
        { DESCR_t x = INTVAL(5); lv_check("an integer", x); }
        { DESCR_t x = o; flds[2].v = DT_I; lv_check("a tag that is no string", x); flds[2].v = DT_S; }
        printf("RTX DIRECT rt_lv_is_list against the C body: %d cases, %d mismatches\n", lv_cases, fails - lfail0);
    }
    DESCR_t idx[] = { INTVAL(0), INTVAL(3), INTVAL(7), INTVAL(8), INTVAL(20), INTVAL(-1), REALVAL(2.0), STRVAL("4"), NULVCL };
    int ni = (int)(sizeof idx / sizeof idx[0]);
    for (int k = 0; k < ni; k++) {
        ARBLK_t *a1 = mk(8, 100), *a2 = mk(8, 100);
        DESCR_t p[2] = { darr(a1), idx[k] }, q[2] = { darr(a2), idx[k] };
        DESCR_t d = rk_at(p), r = rt_call_arr_bl("__rk_arr_at", q, 2, -1);
        ncase++;
        if (!same(d, r)) { fails++; printf("  FAIL __rk_arr_at idx %d\n", k); }
        ARBLK_t *b1 = mk(8, 100), *b2 = mk(8, 100);
        DESCR_t s1[3] = { darr(b1), idx[k], INTVAL(-9) }, s2[3] = { darr(b2), idx[k], INTVAL(-9) };
        d = rk_set(s1);
        r = rt_call_arr_bl("__rk_arr_set", s2, 3, -1);
        ncase++;
        if (!same(d, r) || !same(darr(b1), darr(b2))) { fails++; printf("  FAIL __rk_arr_set idx %d\n", k); }
        ARBLK_t *c1 = mk(8, 100), *c2 = mk(8, 100);
        DESCR_t e1[4] = { STRVAL("a"), darr(c1), idx[k], INTVAL(-7) }, e2[4] = { STRVAL("a"), darr(c2), idx[k], INTVAL(-7) };
        d = rt_rk_elem_put(e1, 4);
        r = rt_call_arr_bl("__rk_elem_put", e2, 4, -1);
        ncase++;
        if (!same(d, r) || !same(darr(c1), darr(c2))) { fails++; printf("  FAIL __rk_elem_put idx %d\n", k); }
    }
    for (int k = 0; k < ni; k++) {
        DESCR_t st1[8], st2[8];
        ARBLK_t *a1 = mk(8, 100), *a2 = mk(8, 100);
        for (int i = 0; i < 8; i++) {
            st1[i] = INTVAL(300 + i);
            st2[i] = INTVAL(300 + i);
            if (i & 1) { DESCR_t c1 = { .v = DT_N, .slen = 1 }, c2 = c1; c1.ptr = &st1[i]; c2.ptr = &st2[i]; a1->data[i] = c1; a2->data[i] = c2; }
        }
        a1->proto_bare |= 16;
        a2->proto_bare |= 16;
        DESCR_t p[2] = { darr(a1), idx[k] }, q[2] = { darr(a2), idx[k] };
        DESCR_t d = rk_at(p), r = rt_call_arr_bl("__rk_arr_at", q, 2, -1);
        ncase++;
        if (!same(d, r)) { fails++; printf("  FAIL __rk_arr_at bound idx %d\n", k); }
        DESCR_t s1[3] = { darr(a1), idx[k], INTVAL(-11) }, s2[3] = { darr(a2), idx[k], INTVAL(-11) };
        d = rk_set(s1);
        r = rt_call_arr_bl("__rk_arr_set", s2, 3, -1);
        ncase++;
        if (!same(d, r) || !same(darr(a1), darr(a2))) { fails++; printf("  FAIL __rk_arr_set bound idx %d\n", k); }
        for (int i = 0; i < 8; i++) if (!same(st1[i], st2[i])) { fails++; printf("  FAIL __rk_arr_set bound idx %d: cell %d differs\n", k, i); break; }
    }
    printf("RTX DIRECT rk arrays: %d answered by the leaf, %d declined to the twin\n", rk_leaf_hits, rk_leaf_declines);
    if (rk_leaf_hits < 8 || rk_leaf_declines < 4) { fails++; printf("  FAIL rk arrays: the battery saw too few of one verdict\n"); }
    {
        const char *pts[] = { "Any", "Mu", "Cool", "Int", "Int:D", "Num", "Numeric", "Real", "Str", "Str:D" };
        int okt[10][3] = { {1,1,1}, {1,1,1}, {1,1,1}, {1,0,0}, {1,0,0}, {0,1,0}, {1,1,0}, {1,1,0}, {0,0,1}, {0,0,1} };
        DESCR_t vs[] = { INTVAL(7), REALVAL(2.5), STRVAL("x") };
        for (int p = 0; p < 10; p++) for (int v = 0; v < 3; v++) {
            DESCR_t a3[3] = { STRVAL(pts[p]), vs[v], STRVAL("n") }, a2[2] = { STRVAL(pts[p]), vs[v] };
            if (okt[p][v]) {
                cmpn("__param_check", rt_rk_param_check, a3, 3);
                cmpn("__param_check", rt_rk_param_check, a2, 2);
            } else {
                cmpfork("__param_check", rt_rk_param_check, a3, 3);
                cmpfork("__param_check", rt_rk_param_check, a2, 2);
            }
        }
        const char *tys[] = { "Any", "Mu", "Cool", "Int", "Num", "Str", "Real", "Numeric" };
        int oka[8][5] = { {1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}, {1,0,0,1,1}, {0,1,0,1,1}, {0,0,1,1,1}, {1,1,0,1,1}, {1,1,0,1,1} };
        DESCR_t av[] = { INTVAL(7), REALVAL(2.5), STRVAL("x"), NULVCL, FAILDESCR };
        for (int t = 0; t < 8; t++) for (int v = 0; v < 5; v++) for (int el = 0; el < 2; el++) {
            DESCR_t a4[4] = { STRVAL(tys[t]), av[v], STRVAL("nm"), INTVAL(el) };
            if (oka[t][v]) cmpn("__rk_assign_check", rt_rk_assign_check, a4, 4);
            else cmpfork("__rk_assign_check", rt_rk_assign_check, a4, 4);
        }
    }
    {
        DEFDAT_fn("rkpt(x,y)");
        const char *bn[] = { "nqp::bindattr", "nqp::bindattr_n", "nqp::bindattr_i", "nqp::bindattr_s" };
        const char *an[] = { "$!x", "$.y", "x", "$!z", "" };
        DESCR_t bv[] = { INTVAL(5), REALVAL(1.5), STRVAL("s"), NULVCL };
        for (int f = 0; f < 4; f++) for (int a = 0; a < 5; a++) for (int b = 0; b < 4; b++) for (int notinst = 0; notinst < 2; notinst++) {
            DESCR_t i1 = notinst ? INTVAL(3) : DATCON_fn("rkpt", INTVAL(1), INTVAL(2)), i2 = notinst ? INTVAL(3) : DATCON_fn("rkpt", INTVAL(1), INTVAL(2));
            DESCR_t a1[4] = { i1, STRVAL("Pt"), STRVAL(an[a]), bv[b] }, a2[4] = { i2, STRVAL("Pt"), STRVAL(an[a]), bv[b] };
            DESCR_t d = rt_rk_bindattr(a1, 4), r = rt_call_arr_bl(bn[f], a2, 4, -1);
            ncase++;
            int ok = same(d, r) && (notinst || (same(FIELD_GET_fn(i1, "x"), FIELD_GET_fn(i2, "x")) && same(FIELD_GET_fn(i1, "y"), FIELD_GET_fn(i2, "y"))));
            if (!ok) { fails++; printf("  FAIL %s attr '%s' value %d notinst %d\n", bn[f], an[a], b, notinst); }
        }
    }
    {
        for (int i = 0; i < nx; i++) { cmpn("__rk_box", rt_rk_box, &xs[i], 1); cmpn("__rk_undef", rt_rk_undef, &xs[i], 1); cmpn("__rk_iter_src", rt_rk_iter_src, &xs[i], 1); }
        cmpn("__rk_undef", rt_rk_undef, xs, 0);
        cmpn("__rk_iter_src", rt_rk_iter_src, xs, 0);
        DESCR_t bs[] = { STRVAL("blk1"), STRVAL(""), NULVCL };
        for (int i = 0; i < 3; i++) cmpn("__blk_ref", rt_rk_blk_ref, &bs[i], 1);
        DESCR_t rl[] = { INTVAL(1), INTVAL(5), INTVAL(3), INTVAL(3), INTVAL(5), INTVAL(1), INTVAL(0), INTVAL(-2), REALVAL(2.5), STRVAL("a"), STRVAL("e") };
        for (int i = 0; i < 11; i++) for (int j = 0; j < 11; j++) {
            DESCR_t r2[2] = { rl[i], rl[j] }, r3[3] = { rl[i], rl[j], INTVAL(1) };
            cmpfork("__rk_range_arr", rt_rk_range_arr, r2, 2);
            cmpfork("__rk_range_arr", rt_rk_range_arr, r3, 3);
        }
        ARBLK_t *ia = mk(4, 10);
        DESCR_t ar[1] = { darr(ia) };
        cmpn("__rk_iter_src", rt_rk_iter_src, ar, 1);
        const char *an2[] = { "__rk_map_append", "__rk_grep_append" };
        DESCR_t (*af[2])(DESCR_t *, int) = { rt_rk_map_append, rt_rk_grep_append };
        for (int w = 0; w < 2; w++) for (int k = 0; k < nx; k++) {
            ARBLK_t *p1 = mk(3, 1), *p2 = mk(3, 1);
            DESCR_t a1[2] = { darr(p1), xs[k] }, a2[2] = { darr(p2), xs[k] };
            DESCR_t d = af[w](a1, 2), r = rt_call_arr_bl(an2[w], a2, 2, -1);
            ncase++;
            if (!same(d, r) || !same(darr(p1), darr(p2)) || p1->hi != 3) { fails++; printf("  FAIL %s item %d\n", an2[w], k); }
            DESCR_t nb[2] = { xs[k], INTVAL(2) };
            cmpfork(an2[w], af[w], nb, 2);
        }
        DESCR_t z = INTVAL(0);
        DESCR_t bv2[] = { INTVAL(5), REALVAL(1.5), STRVAL("s"), NULVCL };
        for (int b = 0; b < 4; b++) for (int cellv = 0; cellv < 2; cellv++) {
            DESCR_t c1 = cellv ? rt_rk_box(&z, 1) : INTVAL(4), c2 = cellv ? rt_rk_box(&z, 1) : INTVAL(4);
            DESCR_t p[2] = { c1, bv2[b] }, q[2] = { c2, bv2[b] };
            DESCR_t d = rt_rk_byref_assign(p, 2), r = rt_call_arr_bl("__rk_byref_assign", q, 2, -1);
            ncase++;
            if (!same(d, r) || (cellv && !same(*(DESCR_t *) c1.ptr, *(DESCR_t *) c2.ptr))) { fails++; printf("  FAIL __rk_byref_assign value %d cell %d\n", b, cellv); }
        }
    }
    printf("RTX DIRECT: %d cases, %d mismatches (the direct builtin entries against the by-name road)\n", ncase, fails);
    printf("RTX DIRECT UNIT: %s\n", fails ? "FAIL" : "PASS");
    return fails ? 1 : 0;
}
