#include "rt/rt_arena.h"
#include "ct_arena.h"
#include "core.h"
#include "coerce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
/*====================================================================================================================================================================================================*/
typedef long icnx_word_t;
typedef struct { icnx_word_t dword, vword; } icnx_descr_t;
typedef struct { icnx_word_t title; double rval; } icnx_realblock_t;
#define ICNX_F_Nqual    ((icnx_word_t)0x8000000000000000L)
#define ICNX_F_Ptr      ((icnx_word_t)0x1000000000000000L)
#define ICNX_F_Typecode ((icnx_word_t)0x2000000000000000L)
#define ICNX_D_Typecode (ICNX_F_Nqual | ICNX_F_Typecode)
#define ICNX_D_Null     ((icnx_word_t)0 | ICNX_D_Typecode)
#define ICNX_D_Integer  ((icnx_word_t)1 | ICNX_D_Typecode)
#define ICNX_D_Lrgint   ((icnx_word_t)2 | ICNX_D_Typecode | ICNX_F_Ptr)
#define ICNX_D_External ((icnx_word_t)19 | ICNX_D_Typecode | ICNX_F_Ptr)
typedef int (*icnx_fn_t)(int, icnx_descr_t *);
typedef struct icnx_extlfuns { icnx_fn_t extlcmp, extlcopy, extlname, extlimage; } icnx_extlfuns_t;
typedef struct icnx_extblk { icnx_word_t title, blksize, id; icnx_extlfuns_t *funcs; icnx_word_t data[]; } icnx_extblk_t;
#define ICNX_D_Real     ((icnx_word_t)3 | ICNX_D_Typecode | ICNX_F_Ptr)
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icnx_is_string(const icnx_descr_t *d) { return d->dword >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static char *icnx_lrgint_str(const icnx_descr_t *d) { extern DESCR_t rt_big_from_icnx_block(const void *); extern char *rt_big_str(DESCR_t); return rt_big_str(rt_big_from_icnx_block((const void *)d->vword)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icnx_to_double(const icnx_descr_t *s, double *out) {
    if (icnx_is_string(s)) { icnx_word_t n = s->dword; if (n < 0) return 0; char buf[n + 1]; memcpy(buf, (const char *)s->vword, (size_t)n); buf[n] = 0;
        char *end = buf; double v = strtod(buf, &end); while (*end == ' ' || *end == '\t') end++; if (end == buf || *end) return 0; *out = v; return 1; }
    if (s->dword == ICNX_D_Integer) { *out = (double)s->vword; return 1; }
    if (s->dword == ICNX_D_Real) { *out = ((const icnx_realblock_t *)(const void *)s->vword)->rval; return 1; }
    if (s->dword == ICNX_D_Lrgint) { char *t = icnx_lrgint_str(s); if (!t) return 0; *out = strtod(t, (char **)0); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
icnx_descr_t nulldesc;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static void icn_extfn_unprovided(const char *what)
{
    char b[fmt_len("external function facility not provided by this runtime: %s", what)];
    snprintf(b, sizeof b, "external function facility not provided by this runtime: %s", what); core_runtime_error(216, b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *alcfile(void *fp, int stat, icnx_descr_t *name) { (void)fp; (void)stat; (void)name; icn_extfn_unprovided("alcfile"); return (void *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void *alcexternal(long nbytes, void *f, void *data) {
    extern void *rt_wsb_alloc(size_t); extern long rt_agg_serial_external(void);
    if (nbytes == 0) nbytes = (long)sizeof(icnx_extblk_t);
    long datasize = nbytes - (long)offsetof(icnx_extblk_t, data);
    if (datasize < 0) { core_runtime_error(301, "alcexternal: invalid size"); return (void *)0; }
    nbytes = (nbytes + (long)sizeof(icnx_word_t) - 1) & ~((long)sizeof(icnx_word_t) - 1);
    icnx_extblk_t *b = (icnx_extblk_t *)rt_wsb_alloc((size_t)nbytes); if (!b) return (void *)0;
    memset(b, 0, (size_t)nbytes); b->title = 19; b->blksize = nbytes; b->id = rt_agg_serial_external(); b->funcs = (icnx_extlfuns_t *)f;
    if (data && datasize > 0) memcpy(b->data, data, (size_t)datasize);
    return (void *)b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int palnum(void) { icn_extfn_unprovided("palnum"); return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rgbkey(void) { icn_extfn_unprovided("rgbkey"); return 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *alcstr(char *s, icnx_word_t len) {
    extern char *rt_str_alloc(long n);
    char *p = rt_str_alloc((long)len + 1); if (!p) return (char *)0; if (s && len > 0) memcpy(p, s, (size_t)len); p[len] = 0; return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
icnx_realblock_t *alcreal(double v) {
    extern void *rt_wsb_alloc(size_t);
    icnx_realblock_t *b = (icnx_realblock_t *)rt_wsb_alloc(sizeof *b); if (!b) return b; b->title = ICNX_D_Real; b->rval = v; return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
double getdbl(icnx_descr_t *d) { double v = 0.0; if (d) icnx_to_double(d, &v); return v; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cnv_int(icnx_descr_t *s, icnx_descr_t *d) {
    if (!s || !d) return 0;
    if (!icnx_is_string(s) && s->dword == ICNX_D_Integer) { d->dword = ICNX_D_Integer; d->vword = s->vword; return 1; }
    double v; if (!icnx_to_double(s, &v)) return 0;
    if (v != (double)(icnx_word_t)v) return 0;
    d->dword = ICNX_D_Integer; d->vword = (icnx_word_t)v; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cnv_real(icnx_descr_t *s, icnx_descr_t *d) {
    if (!s || !d) return 0;
    double v; if (!icnx_to_double(s, &v)) return 0;
    icnx_realblock_t *b = alcreal(v); if (!b) return 0;
    d->dword = ICNX_D_Real; d->vword = (icnx_word_t)(void *)b; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cnv_str(icnx_descr_t *s, icnx_descr_t *d) {
    if (!s || !d) return 0;
    if (icnx_is_string(s)) { *d = *s; return 1; }
    if (s->dword == ICNX_D_Lrgint) { char *t = icnx_lrgint_str(s); if (!t) return 0; icnx_word_t n = (icnx_word_t)strlen(t); char *p = alcstr(t, n); if (!p) return 0; d->dword = n; d->vword = (icnx_word_t)(void *)p; return 1; }
    char buf[64];
    if (s->dword == ICNX_D_Integer) snprintf(buf, sizeof buf, "%ld", (long)s->vword);
    else if (s->dword == ICNX_D_Real) { double r = ((const icnx_realblock_t *)(const void *)s->vword)->rval; snprintf(buf, sizeof buf, "%g", r); }
    else if (s->dword == ICNX_D_Null) buf[0] = 0;
    else return 0;
    icnx_word_t n = (icnx_word_t)strlen(buf); char *p = alcstr(buf, n); if (!p) return 0; d->dword = n; d->vword = (icnx_word_t)(void *)p; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int cnv_c_str(icnx_descr_t *s, icnx_descr_t *d) {
    if (!cnv_str(s, d)) return 0;
    char *p = alcstr((char *)(void *)d->vword, d->dword); if (!p) return 0; d->vword = (icnx_word_t)(void *)p; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static icnx_descr_t icn_extfn_marshal_in(DESCR_t v) {
    icnx_descr_t d; d.dword = ICNX_D_Null; d.vword = 0;
    if (v.v == DT_I) { d.dword = ICNX_D_Integer; d.vword = (icnx_word_t)v.i; return d; }
    if (v.v == DT_EXTL) { d.dword = ICNX_D_External; d.vword = (icnx_word_t)v.p; return d; }
    if (v.v == DT_BIG) { extern void *rt_big_icnx_block(DESCR_t); void *b = rt_big_icnx_block(v); if (b) { d.dword = ICNX_D_Lrgint; d.vword = (icnx_word_t)b; } return d; }
    if (v.v == DT_R) { icnx_realblock_t *b = alcreal(v.r); if (b) { d.dword = ICNX_D_Real; d.vword = (icnx_word_t)(void *)b; } return d; }
    if (v.v == DT_S) { const char *s = v.s ? v.s : ""; d.dword = (icnx_word_t)descr_slen(v); d.vword = (icnx_word_t)(void *)s; return d; }
    if (v.v == DT_SNUL) { d.dword = 0; d.vword = (icnx_word_t)(void *)""; return d; }
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t icn_extfn_marshal_out(const icnx_descr_t *d) {
    if (icnx_is_string(d)) { char *p = alcstr((char *)(void *)d->vword, d->dword); if (!p) return FAILDESCR; return (DESCR_t){ .v = DT_S, .slen = (uint32_t)d->dword, .s = p }; }
    if (d->dword == ICNX_D_Integer) return INTVAL((int64_t)d->vword);
    if (d->dword == ICNX_D_Real) return REALVAL(((const icnx_realblock_t *)(const void *)d->vword)->rval);
    if (d->dword == ICNX_D_Lrgint) { extern DESCR_t rt_big_from_icnx_block(const void *); return rt_big_from_icnx_block((const void *)d->vword); }
    if (d->dword == ICNX_D_External) return (DESCR_t){ .v = DT_EXTL, .p = (void *)d->vword };
    return (DESCR_t){ .v = DT_SNUL, .i = 0 };
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static icnx_extlfuns_t *icnx_extl_funcs(DESCR_t v) { icnx_extblk_t *b = (icnx_extblk_t *)v.p; return b ? b->funcs : (icnx_extlfuns_t *)0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icnx_extl_call(icnx_fn_t fn, DESCR_t a, DESCR_t b, int n, icnx_descr_t *res) {
    icnx_descr_t *st = (icnx_descr_t *)rt_heap_alloc_c(3 * sizeof *st); if (!st) return 0; st[0].dword = ICNX_D_Null; st[0].vword = 0; st[1] = icn_extfn_marshal_in(a); st[2] = (n == 2) ? icn_extfn_marshal_in(b) : st[0];
    if (fn(n, st) != 0) { core_runtime_error(301, "external value helper function did not succeed"); return 0; }
    *res = st[0]; return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_extl_name(DESCR_t v) {
    icnx_extlfuns_t *f = icnx_extl_funcs(v); icnx_descr_t r;
    if (f && f->extlname && icnx_extl_call(f->extlname, v, v, 1, &r) && icnx_is_string(&r)) return icn_extfn_marshal_out(&r);
    return STRVAL("external");
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_extl_image(DESCR_t v) {
    icnx_extlfuns_t *f = icnx_extl_funcs(v); icnx_extblk_t *b = (icnx_extblk_t *)v.p; icnx_descr_t r;
    if (f && f->extlimage && icnx_extl_call(f->extlimage, v, v, 1, &r) && icnx_is_string(&r)) return icn_extfn_marshal_out(&r);
    DESCR_t nm = rt_extl_name(v); long nw = b ? (long)((b->blksize - (icnx_word_t)offsetof(icnx_extblk_t, data)) / (icnx_word_t)sizeof(icnx_word_t)) : 0;
    size_t cap = (size_t)nm.slen + 48; char *p = (char *)rt_heap_alloc_c(cap); if (!p) return FAILDESCR;
    snprintf(p, cap, "%.*s_%ld(%ld)", (int)nm.slen, nm.s ? nm.s : "", b ? (long)b->id : 0L, nw);
    return STRVAL(p);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_extl_copy(DESCR_t v) {
    icnx_extlfuns_t *f = icnx_extl_funcs(v); icnx_descr_t r;
    if (f && f->extlcopy && icnx_extl_call(f->extlcopy, v, v, 1, &r)) { if (r.dword != ICNX_D_External) { core_runtime_error(301, "extlcopy: not an external"); return FAILDESCR; } return icn_extfn_marshal_out(&r); }
    return v;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_extl_cmp(DESCR_t a, DESCR_t b) {
    icnx_extblk_t *ba = (icnx_extblk_t *)a.p, *bb = (icnx_extblk_t *)b.p; icnx_extlfuns_t *f = icnx_extl_funcs(a); icnx_descr_t r;
    if (ba && bb && ba->funcs == bb->funcs && f && f->extlcmp && icnx_extl_call(f->extlcmp, a, b, 2, &r) && r.dword == ICNX_D_Integer) return (r.vword > 0) - (r.vword < 0);
    DESCR_t na = rt_extl_name(a), nb = rt_extl_name(b); size_t la = na.slen, lb = nb.slen, m = la < lb ? la : lb;
    int c = m ? memcmp(na.s, nb.s, m) : 0; if (c) return c < 0 ? -1 : 1; if (la != lb) return la < lb ? -1 : 1;
    long ia = ba ? (long)ba->id : 0, ib = bb ? (long)bb->id : 0; return (ia > ib) - (ia < ib);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_extfn_mint(const char *name, void *fn) {
    size_t n = name ? strlen(name) : 0;
    extern void *rt_wsb_alloc(size_t);
    EXTFN_t *b = (EXTFN_t *)rt_wsb_alloc(sizeof(EXTFN_t) + n + 1); if (!b) return FAILDESCR;
    b->fn = fn; if (n) memcpy(b->name, name, n); b->name[n] = 0;
    return PROCVAL_EXTERNAL(b);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_extfn_invoke(DESCR_t callee, DESCR_t *argv, int n) {
    int (*fn)(int, icnx_descr_t *) = (int (*)(int, icnx_descr_t *))PROCVAL_EXT_FN(callee);
    if (!fn) return FAILDESCR;
    if (n < 0) n = 0;
    if (n > 62) { core_runtime_error(301, "too many arguments to an external function"); return FAILDESCR; }
    icnx_descr_t slots[64];
    slots[0].dword = ICNX_D_Null; slots[0].vword = 0;
    for (int k = 0; k < n; k++) slots[k + 1] = icn_extfn_marshal_in(argv[k]);
    int rc = fn(n, slots);
    if (rc == -1) return FAILDESCR;
    if (rc > 0) { extern int core_icn_error(int, DESCR_t); core_icn_error(rc, icn_extfn_marshal_out(&slots[0])); return FAILDESCR; }
    return icn_extfn_marshal_out(&slots[0]);
}
