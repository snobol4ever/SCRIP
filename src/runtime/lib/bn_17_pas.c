#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_pas_view_raw.h"
#include "runtime/lib/bn_head_rt_hook_guard.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include "runtime/builtin_ids.h"
#include <errno.h>
#include <cpuid.h>
#include "runtime/pas_bi_names.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_37_pas.h"
#include "runtime/lib/bn_38_pas.h"
#include "runtime/core/g_runtime_pas.h"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/gc_heap_hb_aggb.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "runtime/rt/gc_heap_rt_gc_callback.h"
#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
extern void *rt_ws_alloc_descr(size_t);
int rt_pas_bi_index(const char *fn);
int pas_xr_dispatch(int fam, DESCR_t *a, int n, DESCR_t *out);
extern FILE *fh_get(int);
DESCR_t pas_slice(DESCR_t *args, int nargs);
DESCR_t pas_ucd(DESCR_t *args, int nargs);
static DESCR_t pas_pcell_at(void *base, long long idx, const char *fn);
static long long pas_raw_off(DESCR_t p);
static DESCR_t pas_raw_at(void *base, long long off, int w, int W, int sgn, const char *fn);
DESCR_t rt_pas_resundef(DESCR_t *args, int nargs);
DESCR_t rt_pas_sin(DESCR_t *args, int nargs);
DESCR_t rt_pas_cos(DESCR_t *args, int nargs);
DESCR_t rt_pas_exp(DESCR_t *args, int nargs);
DESCR_t rt_pas_arctan(DESCR_t *args, int nargs);
long long rk_ival(DESCR_t v) {
    extern int rk_rat_to_int(DESCR_t, long long *);
    long long q;
    return IS_INT_fn(v) ? (long long) v.i : IS_REAL_fn(v) ? (long long) v.r : (v.v == DT_DATA && rk_rat_to_int(v, &q)) ? q : 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
unsigned pas_mt_u32rand(void) {
    g_pasrnd_t *g = &g_rt_pasrnd;
    unsigned li = g->i1 ? g->i1 - 1 : 625, *mt = g->mt;
    g->i1 = li + 2;
    if (g->seed != g->old || li >= 625) {
        mt[0] = g->seed;
        for (unsigned i = 1; i < 624; i++) mt[i] = 1812433253u * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
        g->seed = ~g->seed;
        g->old = g->seed;
        li = 624;
    }
    if (li == 624) {
        for (unsigned i = 0; i < 227; i++) mt[i] = mt[i + 397] ^ ((((mt[i] & 0x80000000u) | (mt[i + 1] & 0x7FFFFFFFu)) >> 1) ^ ((0u - (mt[i + 1] & 1u)) & 0x9908B0DFu));
        for (unsigned i = 227; i < 623; i++) mt[i] = mt[i - 227] ^ ((((mt[i] & 0x80000000u) | (mt[i + 1] & 0x7FFFFFFFu)) >> 1) ^ ((0u - (mt[i + 1] & 1u)) & 0x9908B0DFu));
        mt[623] = mt[396] ^ ((((mt[623] & 0x80000000u) | (mt[0] & 0x7FFFFFFFu)) >> 1) ^ ((0u - (mt[0] & 1u)) & 0x9908B0DFu));
        li = 0;
        g->i1 = 2;
    }
    unsigned r = mt[li];
    r ^= r >> 11;
    r ^= (r << 7) & 0x9D2C5680u;
    r ^= (r << 15) & 0xEFC60000u;
    return r ^ (r >> 18);
}
DESCR_t *pas_heap_ref(DESCR_t p, const char *fn) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    if (p.v == DT_PCELL && (p.src_node2 & PAS_VIEW_STR)) pas_file_err("6.5.4", "a pointer into a string's characters denotes no variable cell", fn);
    if (p.v == DT_PCELL) { if (!p.ptr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", fn); return (DESCR_t *) p.ptr + (int32_t) p.slen; }
    if (!IS_INT_fn(p)) pas_file_err("6.5.4", "the pointer-variable of an identified-variable is undefined", fn);
    if (p.i == 0) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", fn);
    DESCR_t *hc = pas_heap_cell(p.i, 0);
    if (!hc || p.i > g_rt_pas.pas_heap_ctr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable is undefined (it denotes no variable ever created)", fn);
    if (g_rt_pas.pas_heap_dead[p.i] & 1) pas_file_err("6.6.5.3", "the identified-variable was disposed, so no variable stands behind this reference", fn);
    return hc;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *pas_cell_str(DESCR_t *hc) { if (hc->v == DT_S && hc->s) return hc->s; { const char *c = VARVAL_fn(*hc); return c ? c : ""; } }
unsigned char pas_ch_of(DESCR_t v) {
    if (IS_INT_fn(v)) { long cv = v.i; if (cv < 0) cv = 0; if (cv > 255) cv = 255; return (unsigned char)cv; }
    { const char *vs = VARVAL_fn(v); return (unsigned char)((vs && vs[0]) ? vs[0] : ' '); }
}
DESCR_t pas_agg_copy(DESCR_t v) {
    if (v.v != DT_A || !v.arr) return v;
    ARBLK_t *s = (ARBLK_t *) v.arr;
    long long n = (long long)s->hi - s->lo + 1;
    if (s->ndim == 2) n *= (long long)s->hi2 - s->lo2 + 1;
    if (n < 1) n = 1;
    ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    b->id = rt_agg_serial_list();
    b->dumpno = rt_sno_dumpno_next();
    b->lo = s->lo;
    b->hi = s->hi;
    b->ndim = s->ndim;
    b->lo2 = s->lo2;
    b->hi2 = s->hi2;
    b->proto_bare = s->proto_bare;
    b->proto = s->proto;
    b->ewidth = s->ewidth;
    b->esigned = s->esigned;
    if (s->ewidth) {
        b->data = (DESCR_t *) rt_ws_alloc_raw((size_t) (n * s->ewidth));
        memcpy(b->data, s->data, (size_t) (n * s->ewidth));
    } else {
        b->data = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
        for (long long k = 0; k < n; k++) b->data[k] = pas_agg_copy(s->data[k]);
    }
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
static void pas_fill_cell(DESCR_t *e, long long c) {
    if (e->v == DT_A) pas_fill(*e, c);
    else if (e->v == DT_R) *e = REALVAL((double)c);
    else if (e->v == DT_I) *e = INTVAL(c);
    else if (c == 0 && e->v == DT_S) *e = STRVAL("");
    else if (c != 0 && e->v == DT_SNUL) *e = INTVAL(c);
}
static long long pas_fill_count(const ARBLK_t *s) { long long n = (long long)s->hi - s->lo + 1; if (s->ndim == 2) n *= (long long)s->hi2 - s->lo2 + 1; return n; }
void pas_fill(DESCR_t v, long long c) {
    if (v.v != DT_A || !v.arr) return;
    ARBLK_t *s = (ARBLK_t *) v.arr;
    if (s->ewidth) { for (long long k = 0, n = pas_fill_count(s); k < n; k++) memcpy((char *) s->data + k * s->ewidth, &c, (size_t) s->ewidth); return; }
    for (long long k = 0, n = pas_fill_count(s); k < n; k++) pas_fill_cell(&s->data[k], c);
}
void pas_fill_from(DESCR_t v, long long idx, long long bytes, long long esz, long long c) {
    if (v.v != DT_A || !v.arr) return;
    ARBLK_t *s = (ARBLK_t *) v.arr;
    long long n = pas_fill_count(s), k = idx - s->lo, end = esz > 0 ? k + bytes / esz : n;
    if (s->ewidth) { for (k = k < 0 ? 0 : k; k < end && k < n; k++) memcpy((char *) s->data + k * s->ewidth, &c, (size_t) s->ewidth); return; }
    for (k = k < 0 ? 0 : k; k < end && k < n; k++) pas_fill_cell(&s->data[k], c);
}
DESCR_t pas_dyn_block(long long n) {
    ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    b->id = rt_agg_serial_list();
    b->dumpno = rt_sno_dumpno_next();
    b->lo = 0;
    b->hi = (int) (n - 1);
    b->ndim = 1;
    b->lo2 = 0;
    b->hi2 = 0;
    b->proto_bare = 0;
    b->data = (DESCR_t *) rt_ws_alloc_descr((size_t) (n > 0 ? n : 1));
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
DESCR_t pas_dynset(DESCR_t old, DESCR_t proto, int depth, DESCR_t *dims, int nd) {
    long long n = IS_INT_fn(dims[0]) ? dims[0].i : 0;
    if (n < 0) n = 0;
    ARBLK_t *ob = (old.v == DT_A && old.arr) ? (ARBLK_t *) old.arr : NULL;
    long long on = ob ? (long long)ob->hi - ob->lo + 1 : 0;
    int raw = ob && ob->ewidth && nd == 1;
    DESCR_t d = pas_dyn_block(raw ? 0 : n);
    ARBLK_t *b = (ARBLK_t *) d.arr;
    if (raw) {
        long long keep = on < n ? on : n;
        b->hi = (int) (n - 1);
        b->ewidth = ob->ewidth;
        b->esigned = ob->esigned;
        b->data = (DESCR_t *) rt_ws_alloc_raw((size_t) ((n > 0 ? n : 1) * ob->ewidth));
        memset(b->data, 0, (size_t) ((n > 0 ? n : 1) * ob->ewidth));
        if (keep > 0) memcpy(b->data, ob->data, (size_t) (keep * ob->ewidth));
        return d;
    }
    for (long long k = 0; k < n; k++) {
        DESCR_t prev = k < on ? ob->data[k] : NULVCL;
        if (nd > 1) b->data[k] = pas_dynset(prev, proto, depth - 1, dims + 1, nd - 1);
        else if (k < on) b->data[k] = prev;
        else if (depth > 1) b->data[k] = pas_dyn_block(0);
        else b->data[k] = pas_agg_copy(proto);
    }
    return d;
}
DESCR_t pas_chars_of(DESCR_t v, long lo) {
    if (IS_STR_fn(v)) return v;
    if (v.v != DT_A || !v.arr) return STRVAL(rt_heap_strdup_c(""));
    ARBLK_t *b = (ARBLK_t *) v.arr;
    long from = lo > b->lo ? lo : b->lo;
    long n = (long) b->hi - from + 1;
    if (n < 0) n = 0;
    char *o = rt_str_alloc(n);
    long oi = 0;
    for (long k = from; k <= b->hi; k++) { unsigned char c = pas_ch_of(b->data[k - b->lo]); if (!c) break; o[oi++] = (char) c; }
    o[oi] = '\0';
    return STRVAL(o);
}
DESCR_t pas_chars_full(DESCR_t v, long lo) {
    if (IS_STR_fn(v)) return v;
    if (v.v != DT_A || !v.arr) return STRVAL(rt_heap_strdup_c(""));
    ARBLK_t *b = (ARBLK_t *) v.arr;
    long from = lo > b->lo ? lo : b->lo;
    long n = (long) b->hi - from + 1;
    if (n < 0) n = 0;
    char *o = rt_str_alloc(n);
    for (long k = 0; k < n; k++) o[k] = (char) pas_ch_of(b->data[from + k - b->lo]);
    o[n] = '\0';
    return BSTRVAL(o, n);
}
size_t pas_slen(DESCR_t v, const char *s) { return (v.v == DT_S && v.slen != 0 && v.slen != 0xFFFFFFFFu) ? (size_t)v.slen : strlen(s); }
DESCR_t pas_str_setch(DESCR_t v, long eidx, unsigned char ch) {
    char rb[64];
    const char *cur = (v.v == DT_I && v.i == 0) ? "" : pas_cell_cstring(v, rb, sizeof rb);
    if (!cur) cur = "";
    size_t flen = (v.v == DT_S) ? pas_slen(v, cur) : strlen(cur);
    size_t nflen = ((size_t)eidx > flen) ? (size_t)eidx : flen;
    char *o = rt_str_alloc((long)nflen);
    memcpy(o, cur, flen);
    memset(o + flen, ' ', nflen - flen);
    o[eidx - 1] = (char)ch;
    o[nflen] = '\0';
    return BSTRVAL(o, nflen);
}
void pas_file_err(const char *clause, const char *what, const char *proc) {
    fflush(stdout);
    fprintf(stderr, "scrip: pascal runtime error: ISO 7185 %s: %s", clause ? clause : "", what ? what : "");
    if (proc && !strncmp(proc, "__pas_", 6)) fprintf(stderr, " (in %s)", proc + 6);
    fputc('\n', stderr);
    fflush(stderr);
    int rc = 1;
    if (clause && !strcmp(clause, "6.9.1") && what && (!strncmp(what, "end-of-file is true when a number", 33) || !strncmp(what, "the characters read do not form", 31))) rc = 106;
    if (what && strstr(what, "is activated while f is undefined")) rc = 103;
    exit(rc);
}
DESCR_t pas_arr_make(DESCR_t *args, int nargs) {
    long long lo = (nargs == 2 && IS_INT_fn(args[0])) ? args[0].i : 0;
    long long hi = IS_INT_fn(args[nargs - 1]) ? args[nargs - 1].i : 0;
    long long n = hi - lo + 1;
    if (n < 1) n = 1;
    ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    b->id = rt_agg_serial_list();
    b->dumpno = rt_sno_dumpno_next();
    b->lo = (int) lo;
    b->hi = (int) hi;
    b->ndim = 1;
    b->ndim = 1;
    b->lo2 = 0;
    b->hi2 = 0;
    b->proto_bare = 0;
    b->ewidth = 0;
    b->esigned = 0;
    b->data = (DESCR_t *)rt_ws_alloc_descr((size_t)(n));
    for (long long k = 0; k < n; k++) b->data[k] = INTVAL(0);
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
DESCR_t pas_arr_make_raw(DESCR_t *args, int nargs) {
    if (nargs != 4) return FAILDESCR;
    long long lo = IS_INT_fn(args[0]) ? args[0].i : 0;
    long long hi = IS_INT_fn(args[1]) ? args[1].i : 0;
    long long width = IS_INT_fn(args[2]) ? args[2].i : 1;
    long long sgn = IS_INT_fn(args[3]) ? args[3].i : 0;
    long long n = hi - lo + 1;
    if (n < 1) n = 1;
    ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    b->id = rt_agg_serial_list();
    b->dumpno = rt_sno_dumpno_next();
    b->lo = (int) lo;
    b->hi = (int) hi;
    b->ndim = 1;
    b->lo2 = 0;
    b->hi2 = 0;
    b->proto_bare = 0;
    b->ewidth = (int) width;
    b->esigned = (int) sgn;
    b->data = (DESCR_t *)rt_ws_alloc_raw((size_t)(n * width));
    memset(b->data, 0, (size_t)(n * width));
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
DESCR_t pas_arr_get(DESCR_t *args, int nargs) {
    if (args[0].v == DT_A && args[0].arr) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        long i = rk_ival(args[1]);
        if (i < b->lo || i > b->hi) return FAILDESCR;
        if (b->ewidth) {
            unsigned long long u = 0;
            memcpy(&u, (unsigned char *) b->data + (size_t)(i - b->lo) * (size_t) b->ewidth, (size_t) b->ewidth);
            if (b->esigned && b->ewidth < 8) { unsigned long long m = 1ULL << (b->ewidth * 8 - 1); u = (u ^ m) - m; }
            return INTVAL((long long) u);
        }
        return b->data[i - b->lo];
    }
    const char *cur = VARVAL_fn(args[0]);
    if (!cur) cur = "";
    long idx = rk_ival(args[1]);
    size_t clen = pas_slen(args[0], cur);
    if (idx == 0) return INTVAL((long long)(clen & 0xff));
    if ((size_t)idx == clen + 1) return INTVAL(0);
    if (idx < 1 || (size_t)idx > clen) return FAILDESCR;
    return INTVAL((long long)(unsigned char)cur[idx - 1]);
}
DESCR_t pas_arr_set(DESCR_t *args, int nargs) {
    if (args[0].v == DT_A && args[0].arr) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        long i = rk_ival(args[1]);
        if (i < b->lo || i > b->hi) return FAILDESCR;
        if (b->ewidth) { long long v = rk_ival(args[2]); memcpy((unsigned char *) b->data + (size_t)(i - b->lo) * (size_t) b->ewidth, &v, (size_t) b->ewidth); return args[0]; }
        b->data[i - b->lo] = args[2];
        return args[0];
    }
    const char *cur = VARVAL_fn(args[0]);
    if (!cur) cur = "";
    long idx = rk_ival(args[1]);
    char rb[64];
    const char *rv = to_cstring(args[2], rb, sizeof rb);
    size_t rvl = strlen(rv);
    if (idx == 0) { char *e0 = rt_wsb_alloc(rvl + 1); memcpy(e0, rv, rvl); e0[rvl] = '\0'; return STRVAL(e0); }
    if (idx < 1 || ((size_t)idx > pas_slen(args[0], cur) && idx > 255)) return FAILDESCR;
    return pas_str_setch(args[0], idx, pas_ch_of(args[2]));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t pas_elem_ref(DESCR_t *args, int nargs) {
    if (args[0].v == DT_A && args[0].arr && !((ARBLK_t *) args[0].arr)->ewidth) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        long i = rk_ival(args[1]);
        if (i < b->lo || i > b->hi) return FAILDESCR;
        return (DESCR_t){ .v = DT_N, .slen = 1, .ptr = (void *) &b->data[i - b->lo] };
    }
    DESCR_t v = pas_arr_get(args, 2);
    if (v.v == DT_FAIL) return v;
    *(DESCR_t *) args[2].ptr = v;
    return args[2];
}
DESCR_t bt___rk_seq_plan(DESCR_t *args, int nargs) { if (nargs == 3) { return rk_seq_plan(args[0], (int) rk_ival(args[1]), args[2]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_seq_next(DESCR_t *args, int nargs) { if (nargs == 3) { return rk_seq_next((int) rk_ival(args[0]), args[1], args[2]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_seq_stop(DESCR_t *args, int nargs) {
    if (nargs == 5) { return INTVAL(rk_seq_stop(args[0], args[1], (int) rk_ival(args[2]), (int) rk_ival(args[3]), (int) rk_ival(args[4]))); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_keyed_init(DESCR_t *args, int nargs) {
    if (nargs == 1) { DESCR_t st[4] = { rk_mk_arr(NULL, 0), rk_mk_arr(NULL, 0), NULVCL, INTVAL(rk_ival(args[0])) }; return rk_mk_arr(st, 4); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_arr_make(DESCR_t *args, int nargs) { if ((nargs == 1 || nargs == 2)) { return pas_arr_make(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_arr_get(DESCR_t *args, int nargs) { if (nargs == 2) { return pas_arr_get(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_arr_slice(DESCR_t *args, int nargs) { if (nargs == 3) { return pas_slice(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_arr_set(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs >= 3) { rk_arr_set_impl(args, out); return _bo; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_arr_set_pure(DESCR_t *args, int nargs) { if (nargs >= 3) { return pas_arr_set(args, nargs); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t pas_dispose(DESCR_t *args, int nargs) {
    (void) nargs;
    if (!IS_INT_fn(args[0]) || args[0].i <= 0) pas_file_err("6.6.5.3", "dispose of a pointer that is nil or undefined, so it identifies no variable", NULL);
    if (args[0].i < g_rt_pas.pas_heap_cap && (g_rt_pas.pas_heap_dead[args[0].i] & 2))
        pas_file_err("6.6.5.3", "dispose(p) of a variable created with new(p, c1, ..., cn) must name the same variant constants", NULL);
    pas_heap_give(args[0].i);
    return NULVCL;
}
static uint64_t pas_view_mask(int w) { return w >= 8 ? ~0ULL : (1ULL << (8 * w)) - 1; }
static uint64_t pas_view_img(DESCR_t c, int kind, int w) {
    if (kind == 1 && c.v == DT_S) return c.slen > 0 && c.s ? (unsigned char) c.s[0] : 0;
    if (kind == 3) {
        double d = c.v == DT_R ? c.r : IS_INT_fn(c) ? (double) c.i : 0.0;
        if (w == 4) { float f = (float) d; uint32_t u; memcpy(&u, &f, 4); return u; }
        uint64_t u;
        memcpy(&u, &d, 8);
        return u;
    }
    if (IS_INT_fn(c)) return (uint64_t) c.i & pas_view_mask(w);
    return c.v == DT_R ? (uint64_t)(int64_t) c.r & pas_view_mask(w) : 0;
}
static DESCR_t pas_view_val(uint64_t b, int kind, int sg, int w) {
    b &= pas_view_mask(w);
    if (kind == 2) return INTVAL(b != 0 ? 1 : 0);
    if (kind == 3) { if (w == 4) { uint32_t u = (uint32_t) b; float f; memcpy(&f, &u, 4); return REALVAL((double) f); } double d; memcpy(&d, &b, 8); return REALVAL(d); }
    if (sg && w < 8) return INTVAL((int64_t)(b << (64 - 8 * w)) >> (64 - 8 * w));
    return INTVAL((int64_t) b);
}
static DESCR_t *pas_view_at(DESCR_t p, long long *el, int *bo) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    if (!p.ptr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", "__pas_deref");
    long long w = p.src_node0, off = (long long)(int32_t) p.slen, e = off >= 0 ? off / w : -((-off + w - 1) / w);
    *el = e;
    *bo = (int)(off - e * w);
    return (DESCR_t *) p.ptr;
}
static DESCR_t pas_view_load(DESCR_t p) {
    long long e;
    int bo, w = p.src_node0, W = p.src_node1, f = p.src_node2;
    if (f & PAS_VIEW_RAW) {
        uint64_t u = 0;
        if (!p.ptr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", "__pas_deref");
        memcpy(&u, (unsigned char *) p.ptr + pas_raw_off(p), (size_t) W);
        return pas_view_val(u, 0, (f >> 5) & 1, W);
    }
    DESCR_t *b = pas_view_at(p, &e, &bo);
    uint64_t v = 0;
    if (W >= w) for (int k = 0; k < W / w; k++) v |= pas_view_img(b[e + k], f & 3, w) << (8 * w * k);
    else v = pas_view_img(b[e], f & 3, w) >> (8 * bo);
    return pas_view_val(v, (f >> 3) & 3, (f >> 5) & 1, W);
}
static void pas_view_store(DESCR_t p, DESCR_t val) {
    long long e;
    int bo, w = p.src_node0, W = p.src_node1, f = p.src_node2;
    if (f & PAS_VIEW_RAW) {
        uint64_t u = pas_view_img(val, 0, W);
        if (!p.ptr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", "__pas_deref_set");
        memcpy((unsigned char *) p.ptr + pas_raw_off(p), &u, (size_t) W);
        return;
    }
    DESCR_t *b = pas_view_at(p, &e, &bo);
    uint64_t v = pas_view_img(val, (f >> 3) & 3, W);
    if (W >= w) { for (int k = 0; k < W / w; k++) b[e + k] = pas_view_val(v >> (8 * w * k), f & 3, (f >> 2) & 1, w); return; }
    uint64_t o = pas_view_img(b[e], f & 3, w), m = pas_view_mask(W) << (8 * bo);
    b[e] = pas_view_val((o & ~m) | ((v << (8 * bo)) & m), f & 3, (f >> 2) & 1, w);
}
static DESCR_t *pas_strview_at(DESCR_t p, long long *off, long long *n) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    if (!p.ptr) pas_file_err("6.5.4", "the pointer-variable of an identified-variable denotes nil", "__pas_deref");
    DESCR_t *c = (DESCR_t *) p.ptr;
    *off = (long long)(int32_t) p.slen;
    *n = c->v == DT_S && c->s ? (long long) c->slen : 0;
    return c;
}
static DESCR_t pas_strview_load(DESCR_t p) { long long off, n; DESCR_t *c = pas_strview_at(p, &off, &n); return INTVAL(off >= 0 && off < n ? (unsigned char) c->s[off] : 0); }
static void pas_strview_store(DESCR_t p, DESCR_t val) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    long long off, n;
    DESCR_t *c = pas_strview_at(p, &off, &n);
    unsigned char ch = pas_ch_of(val);
    if (off >= 0 && off < n) { c->s[off] = (char) ch; return; }
    if (off == n && ch == 0) return;
    pas_file_err("6.5.4", "a pointer into a string's characters is written outside the string", "__pas_deref_set");
}
DESCR_t pas_deref(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v == DT_PCELL && (args[0].src_node2 & PAS_VIEW_STR)) return pas_strview_load(args[0]);
    if (args[0].v == DT_PCELL && args[0].src_node0) return pas_view_load(args[0]);
    return *pas_heap_ref(args[0], "__pas_deref");
}
DESCR_t pas_deref_set(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v == DT_PCELL && (args[0].src_node2 & PAS_VIEW_STR)) { pas_strview_store(args[0], args[1]); return args[1]; }
    if (args[0].v == DT_PCELL && args[0].src_node0) { pas_view_store(args[0], args[1]); return args[1]; }
    *pas_heap_ref(args[0], "__pas_deref_set") = args[1];
    return args[1];
}
DESCR_t pas_pview(DESCR_t *args, int nargs) {
    (void) nargs;
    DESCR_t p = args[0];
    long long code = rk_ival(args[1]);
    if (p.v != DT_PCELL) return p;
    if (p.src_node2 & PAS_VIEW_RAW) return pas_raw_at(p.ptr, pas_raw_off(p), (int)((code & 255) ? (code & 255) : 1), (int)((code >> 8) & 255), (int)((code >> 16) & 32), "__pas_pview");
    if (!(code & 255)) return p;
    p = pas_pcell_at(p.ptr, (long long)(int32_t) p.slen * (code & 255), "__pas_pview");
    p.src_node0 = (uint8_t)(code & 255);
    p.src_node1 = (uint8_t)((code >> 8) & 255);
    p.src_node2 = (uint8_t)((code >> 16) & 255);
    return p;
}
DESCR_t pas_preview(DESCR_t *args, int nargs) {
    (void) nargs;
    DESCR_t p = args[0];
    long long code = rk_ival(args[1]);
    if (p.v != DT_PCELL || !p.src_node0 || !((code >> 8) & 255)) return p;
    p.src_node1 = (uint8_t)((code >> 8) & 255);
    p.src_node2 = (uint8_t)((p.src_node2 & (7 | PAS_VIEW_STR | PAS_VIEW_RAW)) | ((code >> 16) & 56));
    return p;
}
DESCR_t pas_cpuid(DESCR_t *args, int nargs) {
    (void) nargs;
    unsigned r[4] = { 0, 0, 0, 0 };
    long long reg = rk_ival(args[2]);
    if (reg < 0 || reg > 4) return INTVAL(0);
    if (reg == 4) {
        unsigned lo = 0, hi = 0;
        if (!__get_cpuid_count(1, 0, &r[0], &r[1], &r[2], &r[3]) || !(r[2] & 0x08000000u)) return INTVAL(0);
        __asm__ volatile("xgetbv" : "=a"(lo), "=d"(hi) : "c"(0));
        return INTVAL((long long) lo);
    }
    if (!__get_cpuid_count((unsigned) rk_ival(args[0]), (unsigned) rk_ival(args[1]), &r[0], &r[1], &r[2], &r[3])) return INTVAL(0);
    return INTVAL((long long) r[reg]);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pas_pcell(void *base, long idx) { return (DESCR_t){ .v = DT_PCELL, .slen = (uint32_t)(int32_t) idx, .ptr = base }; }
static DESCR_t pas_pcell_at(void *base, long long idx, const char *fn) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    if (idx < INT32_MIN || idx > INT32_MAX) pas_file_err("6.5.4", "pointer arithmetic leaves the range of the element index", fn);
    return pas_pcell(base, (long) idx);
}
static long long pas_raw_off(DESCR_t p) { return (long long)(int32_t) p.slen * (p.src_node1 ? p.src_node1 : 1) + (p.src_node2 & 7); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_move_raw(DESCR_t d, DESCR_t s, long long bytes) {
    if (d.v != DT_PCELL || s.v != DT_PCELL || !d.ptr || !s.ptr || !(d.src_node2 & PAS_VIEW_RAW) || !(s.src_node2 & PAS_VIEW_RAW)) return 0;
    if (bytes > 0) memmove((unsigned char *) d.ptr + pas_raw_off(d), (unsigned char *) s.ptr + pas_raw_off(s), (size_t) bytes);
    return 1;
}
static DESCR_t pas_raw_at(void *base, long long off, int w, int W, int sgn, const char *fn) {
    long long u = W > 0 ? W : 1, q = off / u, m = off % u;
    if (m < 0) { m += u; q--; }
    DESCR_t r = pas_pcell_at(base, q, fn);
    r.src_node0 = (uint8_t) w;
    r.src_node1 = (uint8_t) W;
    r.src_node2 = (uint8_t) (PAS_VIEW_RAW | (sgn ? 32 : 0) | (int) m);
    return r;
}
DESCR_t pas_addr(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    if (args[0].v == DT_A && args[0].arr) {
        ARBLK_t *b = (ARBLK_t *) args[0].arr;
        if (b->hi < b->lo || !b->data) return INTVAL(0);
        if (b->ewidth) { return pas_raw_at((void *) b->data, (rk_ival(args[1]) - b->lo) * b->ewidth, b->ewidth, b->ewidth, b->esigned, "__pas_addr"); }
        return pas_pcell_at((void *) b->data, rk_ival(args[1]) - b->lo, "__pas_addr");
    }
    if (IS_INT_fn(args[0]) && args[0].i == 0) return INTVAL(0);
    pas_file_err("6.5.4", "the address of an element is taken of a value that is no array or record variable", "__pas_addr");
    return FAILDESCR;
}
DESCR_t pas_addr_var(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    if (args[0].v == DT_N && args[0].slen == 1 && args[0].ptr) return pas_pcell(args[0].ptr, 0);
    pas_file_err("6.5.4", "the address of a variable is taken of a name that denotes no variable cell", "__pas_addr_var");
    return FAILDESCR;
}
DESCR_t pas_ptr_inc(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    if (args[0].v != DT_PCELL || !args[0].ptr) pas_file_err("6.5.4", "pointer arithmetic on a pointer that denotes no cell", "__pas_ptr_inc");
    if (args[0].src_node2 & PAS_VIEW_RAW)
        return pas_raw_at(args[0].ptr, pas_raw_off(args[0]) + rk_ival(args[1]) * args[0].src_node1, args[0].src_node0, args[0].src_node1, args[0].src_node2 & 32, "__pas_ptr_inc");
    DESCR_t r = pas_pcell_at(args[0].ptr, (long long) (int32_t) args[0].slen + rk_ival(args[1]) * (args[0].src_node0 ? args[0].src_node1 : 1), "__pas_ptr_inc");
    r.src_node0 = args[0].src_node0;
    r.src_node1 = args[0].src_node1;
    r.src_node2 = args[0].src_node2;
    return r;
}
DESCR_t pas_slice(DESCR_t *args, int nargs) {
    if (nargs != 3) return FAILDESCR;
    long long lo = rk_ival(args[1]), hi = rk_ival(args[2]), n = hi - lo + 1;
    if (n < 1) return FAILDESCR;
    ARBLK_t *b = (ARBLK_t *) rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
    b->id = rt_agg_serial_list();
    b->dumpno = rt_sno_dumpno_next();
    b->lo = 0;
    b->hi = (int) (n - 1);
    b->ndim = 1;
    b->lo2 = 0;
    b->hi2 = 0;
    b->proto_bare = 0;
    b->ewidth = 0;
    b->esigned = 0;
    b->data = (DESCR_t *) rt_ws_alloc_descr((size_t) n);
    for (long long k = 0; k < n; k++) b->data[k] = INTVAL(0);
    for (long long k = 0; k < n; k++) {
        DESCR_t a2[2] = { args[0], INTVAL(lo + k) };
        DESCR_t e = args[0].v == DT_A ? pas_arr_get(a2, 2) : pas_ptr_inc(a2, 2);
        if (args[0].v != DT_A) e = pas_deref(&e, 1);
        if (e.v == DT_FAIL) return FAILDESCR;
        b->data[k] = e;
    }
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
static uintptr_t pas_ptr_word(DESCR_t p) {
    if (!p.src_node0) return (uintptr_t) p.ptr + 16u * (uintptr_t)(intptr_t)(int32_t) p.slen;
    if (p.src_node2 & PAS_VIEW_RAW) return (uintptr_t) p.ptr + (uintptr_t)(intptr_t) pas_raw_off(p);
    long long off = (long long)(int32_t) p.slen, w = p.src_node0, e = off >= 0 ? off / w : -((-off + w - 1) / w);
    return (uintptr_t) p.ptr + 16u * (uintptr_t)(intptr_t) e + (uintptr_t)(off - e * w);
}
DESCR_t pas_ptr_eq(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v == DT_PCELL || args[1].v == DT_PCELL) return INTVAL(args[0].v == args[1].v && pas_ptr_word(args[0]) == pas_ptr_word(args[1]) ? 1 : 0);
    if (IS_INT_fn(args[0]) && IS_INT_fn(args[1])) return INTVAL(args[0].i == args[1].i ? 1 : 0);
    return INTVAL(args[0].v == args[1].v && args[0].i == args[1].i ? 1 : 0);
}
DESCR_t pas_pdx(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    long long i = rk_ival(args[1]), lo = rk_ival(args[2]), hi = rk_ival(args[3]);
    if (args[0].v == DT_PCELL && (args[0].src_node2 & PAS_VIEW_RAW))
        return pas_raw_at(args[0].ptr, pas_raw_off(args[0]) + (i - lo) * args[0].src_node1, args[0].src_node0, args[0].src_node1, args[0].src_node2 & 32, "__pas_pdx");
    DESCR_t *cell = pas_heap_ref(args[0], "__pas_pdx");
    if (cell->v == DT_A && cell->arr) {
        ARBLK_t *b = (ARBLK_t *) cell->arr;
        if (b->ndim == 1 && b->lo == lo && b->hi == hi && b->data && b->ewidth) { return pas_raw_at((void *) b->data, (i - b->lo) * b->ewidth, b->ewidth, b->ewidth, b->esigned, "__pas_pdx"); }
        if (b->ndim == 1 && b->lo == lo && b->hi == hi && b->data) return pas_pcell_at((void *) b->data, i - b->lo, "__pas_pdx");
    }
    if (args[0].v != DT_PCELL) pas_file_err("6.5.4", "an element is indexed through a pointer that holds no cell kind", "__pas_pdx");
    return pas_pcell_at(args[0].ptr, (long long) (int32_t) args[0].slen + (i - lo), "__pas_pdx");
}
static DESCR_t pas_row_el_get(ARBLK_t *b, long long d) {
    if (!b->ewidth) return b->data[d];
    unsigned long long u = 0;
    memcpy(&u, (unsigned char *) b->data + (size_t) d * (size_t) b->ewidth, (size_t) b->ewidth);
    if (b->esigned && b->ewidth < 8) { unsigned long long m = 1ULL << (b->ewidth * 8 - 1); u = (u ^ m) - m; }
    return INTVAL((long long) u);
}
static void pas_row_el_put(ARBLK_t *b, long long d, DESCR_t v) {
    if (!b->ewidth) { b->data[d] = pas_agg_copy(v); return; }
    long long x = rk_ival(v);
    memcpy((unsigned char *) b->data + (size_t) d * (size_t) b->ewidth, &x, (size_t) b->ewidth);
}
DESCR_t pas_row_get(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    if (args[0].v != DT_A || !args[0].arr) pas_file_err("6.5.3.2", "a row is taken of a value that is no array", "__pas_row_get");
    ARBLK_t *a = (ARBLK_t *) args[0].arr;
    long long i = rk_ival(args[1]), nc = rk_ival(args[2]), l2 = rk_ival(args[3]), h2 = rk_ival(args[4]);
    DESCR_t mk[2] = { INTVAL(l2), INTVAL(h2) };
    DESCR_t row = pas_arr_make(mk, 2);
    ARBLK_t *b = (ARBLK_t *) row.arr;
    for (long long k = l2; k <= h2; k++) {
        long long at = i * nc + (k - l2) - a->lo;
        if (at < 0 || at > (long long) a->hi - a->lo) pas_file_err("6.5.3.2", "an index value is not assignment-compatible with the index-type of the array", "__pas_row_get");
        pas_row_el_put(b, k - l2, pas_row_el_get(a, at));
    }
    return row;
}
DESCR_t pas_row_set(DESCR_t *args, int nargs) {
    void pas_file_err(const char *clause, const char *what, const char *proc);
    (void) nargs;
    if (args[0].v != DT_A || !args[0].arr || args[5].v != DT_A || !args[5].arr) pas_file_err("6.5.3.2", "a row is assigned between values that are no arrays", "__pas_row_set");
    ARBLK_t *a = (ARBLK_t *) args[0].arr, *r = (ARBLK_t *) args[5].arr;
    long long i = rk_ival(args[1]), nc = rk_ival(args[2]), l2 = rk_ival(args[3]), h2 = rk_ival(args[4]);
    for (long long k = l2; k <= h2; k++) {
        long long at = i * nc + (k - l2) - a->lo, from = k - l2;
        if (at < 0 || at > (long long) a->hi - a->lo || from > (long long) r->hi - r->lo)
            pas_file_err("6.5.3.2", "an index value is not assignment-compatible with the index-type of the array", "__pas_row_set");
        pas_row_el_put(a, at, pas_row_el_get(r, from));
    }
    return args[5];
}
static int pas_io_errno_code(int e) { return e == ENOENT ? 2 : e == ENOTDIR ? 3 : e == EMFILE ? 4 : (e == EACCES || e == EPERM || e == EISDIR || e == EROFS) ? 5 : 5; }
int pas_io_fail(int e) { if (!g_rt_pasio.ioflow) return 0; g_rt_pasio.inoutres = pas_io_errno_code(e); return 1; }
const char *pas_io_message(long code) {
    switch (code) {
        case 2:
        return "File not found";
        case 3:
        return "Path not found";
        case 4:
        return "Too many open files";
        case 5:
        return "Access denied";
        case 6:
        return "Invalid file handle";
        case 100:
        return "Disk read error";
        case 101:
        return "Disk write error";
        case 102:
        return "File not assigned";
        case 103:
        return "File not open";
        case 104:
        return "File not open for input";
        case 105:
        return "File not open for output";
        case 106:
        return "Invalid numeric format";
        default:
        return "I/O error";
    }
}
DESCR_t pas_ptr_diff(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v != DT_PCELL || args[1].v != DT_PCELL) return INTVAL(0);
    if ((args[0].src_node2 & PAS_VIEW_RAW) && (args[1].src_node2 & PAS_VIEW_RAW) && args[0].ptr == args[1].ptr && args[0].src_node1)
        return INTVAL((pas_raw_off(args[0]) - pas_raw_off(args[1])) / args[0].src_node1);
    if (args[0].src_node0 && args[0].ptr == args[1].ptr && args[1].src_node0) return INTVAL(((long long)(int32_t) args[0].slen - (int32_t) args[1].slen) / args[0].src_node1);
    return INTVAL((long long)((intptr_t) pas_ptr_word(args[0]) - (intptr_t) pas_ptr_word(args[1])) / 16);
}
static DESCR_t pas_raw_first(ARBLK_t *b) {
    DESCR_t r = pas_pcell((void *) b->data, 0);
    r.src_node0 = r.src_node1 = (uint8_t) b->ewidth;
    r.src_node2 = (uint8_t) (PAS_VIEW_RAW | (b->esigned ? 32 : 0));
    return r;
}
DESCR_t pas_pchar_of(DESCR_t *args, int nargs) {
    (void) nargs;
    if (IS_INT_fn(args[0]) && args[0].i > 0 && args[0].i <= g_rt_pas.pas_heap_ctr) {
        DESCR_t *hc = pas_heap_cell(args[0].i, 0);
        if (hc && hc->v == DT_A && hc->arr && ((ARBLK_t *) hc->arr)->data) return ((ARBLK_t *) hc->arr)->ewidth ? pas_raw_first((ARBLK_t *) hc->arr) :
            pas_pcell((void *) ((ARBLK_t *) hc->arr)->data, 0);
    }
    if (args[0].v == DT_PCELL || IS_INT_fn(args[0])) return args[0];
    if (args[0].v == DT_A && args[0].arr) return ((ARBLK_t *) args[0].arr)->ewidth ? pas_raw_first((ARBLK_t *) args[0].arr) : pas_pcell((void *) ((ARBLK_t *) args[0].arr)->data, 0);
    const char *c = VARVAL_fn(args[0]);
    size_t n = c ? pas_slen(args[0], c) : 0;
    DESCR_t *d = (DESCR_t *) rt_ws_alloc_descr(n + 1);
    for (size_t k = 0; k < n; k++) d[k] = INTVAL((unsigned char) c[k]);
    d[n] = INTVAL(0);
    return pas_pcell((void *) d, 0);
}
DESCR_t pas_pstrview(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v != DT_PCELL || !args[0].ptr || args[0].src_node0) return args[0];
    DESCR_t *c = (DESCR_t *) args[0].ptr + (int32_t) args[0].slen;
    if (c->v != DT_S) { DESCR_t a[2] = { pas_pchar_of(c, 1), args[1] }; return pas_ptr_inc(a, 2); }
    long n = c->s ? (long) c->slen : 0;
    char *o = rt_str_alloc(n);
    if (n) memcpy(o, c->s, (size_t) n);
    o[n] = '\0';
    *c = BSTRVAL(o, n);
    DESCR_t r = pas_pcell_at((void *) c, rk_ival(args[1]), "__pas_pstrview");
    r.src_node0 = 1;
    r.src_node1 = 1;
    r.src_node2 = PAS_VIEW_STR;
    return r;
}
static long pas_strview_len(DESCR_t p) { long long off, n; pas_strview_at(p, &off, &n); long k = 0; DESCR_t *c = (DESCR_t *) p.ptr; while (off + k < n && c->s[off + k]) k++; return k; }
DESCR_t pas_pstrlen(DESCR_t *args, int nargs) {
    (void) nargs;
    if (args[0].v == DT_PCELL && (args[0].src_node2 & PAS_VIEW_STR)) return INTVAL(pas_strview_len(args[0]));
    if (args[0].v != DT_PCELL || !args[0].ptr) return INTVAL(0);
    DESCR_t *c = (DESCR_t *) args[0].ptr + (int32_t) args[0].slen;
    long n = 0;
    while (pas_ch_of(c[n]) != 0 && !(c[n].v == DT_S && c[n].slen == 0)) n++;
    return INTVAL(n);
}
DESCR_t pas_pstr(DESCR_t *args, int nargs) {
    if (args[0].v != DT_PCELL) return IS_INT_fn(args[0]) ? STRVAL(rt_wsb_alloc(1)) : args[0];
    if (args[0].src_node2 & PAS_VIEW_STR) {
        long n = pas_strview_len(args[0]);
        char *o = rt_wsb_alloc((size_t) n + 1);
        if (n) memcpy(o, ((DESCR_t *) args[0].ptr)->s + (int32_t) args[0].slen, (size_t) n);
        o[n] = '\0';
        return BSTRVAL(o, n);
    }
    long n = pas_pstrlen(args, nargs).i;
    DESCR_t *c = (DESCR_t *) args[0].ptr + (int32_t) args[0].slen;
    char *o = rt_wsb_alloc((size_t) n + 1);
    for (long k = 0; k < n; k++) o[k] = (char) pas_ch_of(c[k]);
    o[n] = '\0';
    return BSTRVAL(o, n);
}
DESCR_t pas_field_set(DESCR_t *args, int nargs) {
    DESCR_t *hc = pas_heap_ref(args[0], "__pas_field_set");
    long idx = rk_ival(args[1]);
    (void) nargs;
    if (hc->v == DT_A && hc->arr) {
        ARBLK_t *b = (ARBLK_t *) hc->arr;
        long long rv = rk_ival(args[2]);
        if (b->ewidth && idx >= b->lo && idx <= b->hi) { memcpy((char *) b->data + (idx - b->lo) * b->ewidth, &rv, (size_t) b->ewidth); return args[2]; }
        if (idx >= b->lo && idx <= b->hi) b->data[idx - b->lo] = args[2];
        return args[2];
    }
    *hc = (idx >= 1) ? pas_str_setch(*hc, idx, pas_ch_of(args[2])) : args[2];
    return args[2];
}
static void pas_stdout_flush_at_exit(void) {
    extern FILE *fh_get(int);
    fflush(stdout);
    for (int i = 0; i < FH_N; i++) if (g_fh[i].name && !strcmp(g_fh[i].name, "StdOut")) { FILE *fp = fh_get(i); if (fp) fflush(fp); }
}
int pas_stdout_fh(void) {
    extern FILE *fh_get(int);
    extern int fh_alloc(FILE *);
    for (int i = 0; i < FH_N; i++) if (g_fh[i].name && !strcmp(g_fh[i].name, "StdOut") && fh_get(i)) return i;
    int fd = dup(1);
    FILE *fp = fd >= 0 ? fdopen(fd, "w") : NULL;
    if (!fp) return 1;
    int idx = fh_alloc(fp);
    if (idx < 0) { fclose(fp); return 1; }
    g_fh[idx].name = rt_heap_strdup_c("StdOut");
    atexit(pas_stdout_flush_at_exit);
    return idx;
}
int pas_val_scan(const char *s, int real, long long *iv, double *dv) {
    if (!s) s = "";
    size_t n = strlen(s), p = 0;
    while (p < n && s[p] == ' ') p++;
    size_t st = p;
    int neg = 0;
    if (p < n && (s[p] == '+' || s[p] == '-')) { neg = s[p] == '-'; p++; }
    if (!real) {
        int base = 10;
        if (p < n && s[p] == '$') {
            base = 16;
            p++;
        } else if (p + 1 < n && s[p] == '0' && (s[p + 1] == 'x' || s[p + 1] == 'X')) {
            base = 16;
            p += 2;
        } else if (p < n && s[p] == '%') {
            base = 2;
            p++;
        } else if (p < n && s[p] == '&') {
            base = 8;
            p++;
        }
        size_t d0 = p;
        unsigned long long acc = 0;
        int over = 0;
        for (; p < n; p++) {
            int c = (unsigned char)s[p], dg = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : 99;
            if (dg >= base) break;
            if (acc > (0x7FFFFFFFFFFFFFFFULL - (unsigned)dg) / (unsigned)base) over = 1;
            acc = acc * (unsigned)base + (unsigned)dg;
        }
        if (p < n) return (int)p + 1;
        if (p == d0) return (int)p + 1;
        if (over) return (int)p;
        *iv = neg ? -(long long)acc : (long long)acc;
        return 0;
    }
    size_t d0 = p;
    while (p < n && s[p] >= '0' && s[p] <= '9') p++;
    size_t nd = p - d0;
    if (p < n && s[p] == '.') { p++; size_t f0 = p; while (p < n && s[p] >= '0' && s[p] <= '9') p++; nd += p - f0; }
    if (nd == 0) return (int)p + 1;
    if (p < n && (s[p] == 'e' || s[p] == 'E')) {
        size_t q = p + 1;
        if (q < n && (s[q] == '+' || s[q] == '-')) q++;
        size_t e0 = q;
        while (q < n && s[q] >= '0' && s[q] <= '9') q++;
        if (q == e0) return (int)q + 1;
        p = q;
    }
    if (p < n) return (int)p + 1;
    size_t len = n - st;
    char *tmp = rt_wsb_alloc(len + 1);
    memcpy(tmp, s + st, len);
    tmp[len] = '\0';
    *dv = strtod(tmp, NULL);
    return 0;
}
int pas_bi_chain(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, long *rq) { return pas_bi_chain_id(rt_pas_bi_index(fn), fn, args, nargs, out, rq); }
int rt_pas_bi_index(const char *fn) {
    int lo = 0, hi = (int)PAS_BI_COUNT - 1;
    while (fn && lo <= hi) { int mid = (lo + hi) >> 1, c = strcmp(g_pas_bi_name[mid], fn); if (c == 0) return mid; if (c < 0) lo = mid + 1; else hi = mid - 1; }
    return -1;
}
static inline double pas_real_arg(DESCR_t d) { return IS_REAL_fn(d) ? d.r : (double)(IS_INT_fn(d) ? d.i : 0); }
DESCR_t c_rt_pas_sqrt(DESCR_t *args, int nargs) { double d = pas_real_arg(args[0]); (void) nargs; if (d < 0) pas_file_err("6.6.6.2", "sqrt(x) of a negative x", NULL); return REALVAL(sqrt(d)); }
DESCR_t c_rt_pas_ln(DESCR_t *args, int nargs) {
    double d = pas_real_arg(args[0]);
    (void) nargs;
    if (!(d > 0)) pas_file_err("6.6.6.2", "ln(x) of an x that is not greater than zero", NULL);
    return REALVAL(log(d));
}
int pas_bi_chr(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return 0;
    long long cv = IS_INT_fn(args[0]) ? args[0].i : 0;
    if (cv < 0) cv = 0;
    if (cv > 255) cv = 255;
    char *s = (char *)rt_wsb_alloc(2);
    s[0] = (char)(unsigned char)cv;
    s[1] = '\0';
    *out = (DESCR_t){ .v = DT_S, .s = s, .slen = 1 };
    return 1;
}
int pas_bi_chrlit(DESCR_t *args, int nargs, DESCR_t *out) { if (nargs != 1) return 0; *out = IS_INT_fn(args[0]) ? args[0] : INTVAL(0); return 1; }
int pas_bi_trunc(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1 && nargs != 2) return 0;
    double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
    if (nargs == 2 && IS_INT_fn(args[1]) && args[1].i == 2 && !(d > -9223372036854775809.0 && d < 9223372036854775808.0)) { fflush(NULL); fprintf(stderr, "Runtime error 207 at $0\n"); exit(207); }
    if (nargs == 2 && !(IS_INT_fn(args[1]) && args[1].i == 2) && !(trunc(d) >= -2147483647.0 && trunc(d) <= 2147483647.0))
        pas_file_err("6.6.6.3", "trunc(x) of a value whose integer part is not in -maxint..maxint, so no such integer value exists", NULL);
    *out = INTVAL((long long)d);
    return 1;
}
int pas_bi_abs(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return 0;
    if (IS_REAL_fn(args[0])) { double d = args[0].r; DESCR_t r = {0}; r.v = DT_R; r.r = d < 0 ? -d : d; *out = r; return 1; }
    long long v = IS_INT_fn(args[0]) ? args[0].i : 0;
    *out = INTVAL(v < 0 ? -v : v);
    return 1;
}
int pas_bi_round(DESCR_t *args, int nargs, DESCR_t *out) {
    if (nargs != 1) return 0;
    double d = IS_REAL_fn(args[0]) ? args[0].r : (double)(IS_INT_fn(args[0]) ? args[0].i : 0);
    if (!(round(d) >= -2147483647.0 && round(d) <= 2147483647.0)) pas_file_err("6.6.6.3", "round(x) of a value that rounds outside -maxint..maxint, so no such integer value exists", NULL);
    *out = INTVAL((long long)round(d));
    return 1;
}
static const unsigned short g_pas_bi_bid[PBI_COUNT_] = { [PBI_abs] = BID___pas_abs, [PBI_arctan] = BID___pas_arctan, [PBI_chr] = BID___pas_chr, [PBI_chrlit] = BID___pas_chrlit,
    [PBI_cos] = BID___pas_cos, [PBI_enum_name] = BID___pas_enum_name, [PBI_eof] = BID___pas_eof, [PBI_eof_f] = BID___pas_eof_f, [PBI_eoln] = BID___pas_eoln, [PBI_eoln_f] = BID___pas_eoln_f,
    [PBI_exp] = BID___pas_exp, [PBI_getbufch] = BID___pas_getbufch, [PBI_getbufch_f] = BID___pas_getbufch_f, [PBI_ln] = BID___pas_ln, [PBI_read_c] = BID___pas_read_c,
    [PBI_read_c_f] = BID___pas_read_c_f, [PBI_read_i] = BID___pas_read_i, [PBI_read_i_f] = BID___pas_read_i_f, [PBI_readln] = BID___pas_readln, [PBI_readln_f] = BID___pas_readln_f,
    [PBI_sin] = BID___pas_sin, [PBI_sqrt] = BID___pas_sqrt, [PBI_trunc] = BID___pas_trunc, [PBI_write] = BID___pas_write, [PBI_writeln] = BID___pas_writeln, [PBI_round] = BID___pas_round,
    [PBI_halt] = BID___pas_halt, [PBI_frac] = BID___pas_frac, [PBI_assert] = BID___pas_assert, [PBI_pos] = BID___pas_pos, [PBI_mkdir] = BID___pas_mkdir, [PBI_rmdir] = BID___pas_rmdir,
    [PBI_swapendian] = BID___pas_swapendian, };
DESCR_t rt_call_pas_bi(DESCR_t *args, int nargs, long idx) {
    DESCR_t out = FAILDESCR;
    const char *fn = (idx >= 0 && idx < (long)PAS_BI_COUNT) ? g_pas_bi_name[idx] : "";
    if (g_rt_pasio.ioflow && g_rt_pasio.inoutres &&
        (idx == PBI_write || idx == PBI_writeln || idx == PBI_read_i || idx == PBI_read_i_f || idx == PBI_read_c || idx == PBI_read_c_f || idx == PBI_read_s || idx == PBI_read_s_f ||
        idx == PBI_readln || idx == PBI_readln_f)) return NULVCL;
    if (nargs == 1) {
        if (idx == PBI_resundef) return rt_pas_resundef(args, nargs);
        if (idx == PBI_sin) return rt_pas_sin(args, nargs);
        if (idx == PBI_sqrt) return c_rt_pas_sqrt(args, nargs);
        if (idx == PBI_cos) return rt_pas_cos(args, nargs);
        if (idx == PBI_exp) return rt_pas_exp(args, nargs);
        if (idx == PBI_ln) return c_rt_pas_ln(args, nargs);
        if (idx == PBI_arctan) return rt_pas_arctan(args, nargs);
    }
    switch (idx) {
        case PBI_writestr:
        return pas_writestr(args, nargs);
        case PBI_pview:
        return pas_pview(args, nargs);
        case PBI_preview:
        return pas_preview(args, nargs);
        case PBI_cpuid:
        return pas_cpuid(args, nargs);
        case PBI_ucd:
        return pas_ucd(args, nargs);
        case PBI_pstrview:
        return pas_pstrview(args, nargs);
        case PBI_xr_cvt:
        if (pas_xr_dispatch(0, args, nargs, &out)) return out;
        break;
        case PBI_xr_bin:
        if (pas_xr_dispatch(1, args, nargs, &out)) return out;
        break;
        case PBI_xr_cmp:
        if (pas_xr_dispatch(2, args, nargs, &out)) return out;
        break;
        case PBI_xr_ev:
        if (pas_xr_dispatch(5, args, nargs, &out)) return out;
        break;
        case PBI_xr_un:
        if (pas_xr_dispatch(3, args, nargs, &out)) return out;
        break;
        case PBI_xr_str:
        if (pas_xr_dispatch(4, args, nargs, &out)) return out;
        break;
        case PBI_chr:
        if (pas_bi_chr(args, nargs, &out)) return out;
        break;
        case PBI_chrlit:
        if (pas_bi_chrlit(args, nargs, &out)) return out;
        break;
        case PBI_trunc:
        if (pas_bi_trunc(args, nargs, &out)) return out;
        break;
        case PBI_abs:
        if (pas_bi_abs(args, nargs, &out)) return out;
        break;
        case PBI_round:
        if (pas_bi_round(args, nargs, &out)) return out;
        break;
        default:
        break;
    }
    int bid = (idx >= 0 && idx < (long)PBI_COUNT_) ? g_pas_bi_bid[idx] : 0;
    if (!bid && RT_GC_CALLBACK(pas_bi_chain_id((int)idx, fn, args, nargs, &out, (long *)0))) return out;
    unsigned n = (unsigned)strlen(fn);
    return RT_GC_CALLBACK(rt_call_arr_bl(fn, args, nargs, (int)((n << 16) | (unsigned)bid)));
}
DESCR_t rt_sno_pdef_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_defer_text(const char *, DESCR_t, int);
    RT_HOOK_GUARD("SNO$PDEF");
    return nargs >= 1 && nargs <= 3 ? pat_defer_text(rt_sno_target_name(args[0]), nargs >= 2 ? args[1] : NULVCL, nargs >= 3 ? (int) rk_ival(args[2]) : 1) : FAILDESCR;
}
#include "runtime/pas_ucd_table.inc"
static int pas_ucd_cat_of(unsigned int cp) {
    int lo = 0, hi = PAS_UCD_NCAT - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (cp < pas_ucd_cat[m][0]) hi = m - 1; else if (cp > pas_ucd_cat[m][1]) lo = m + 1; else return (int) pas_ucd_cat[m][2]; }
    return 29;
}
static unsigned int pas_ucd_map_of(const unsigned int (*t)[2], int n, unsigned int cp) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (cp < t[m][0]) hi = m - 1; else if (cp > t[m][0]) lo = m + 1; else return t[m][1]; }
    return 0;
}
static int pas_ucd_ws_of(unsigned int cp) {
    int lo = 0, hi = PAS_UCD_NWS - 1;
    while (lo <= hi) { int m = (lo + hi) / 2; if (cp < pas_ucd_ws[m]) hi = m - 1; else if (cp > pas_ucd_ws[m]) lo = m + 1; else return 1; }
    return 0;
}
static double pas_ucd_num_of(unsigned int cp) {
    int lo = 0, hi = PAS_UCD_NNUM - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (cp < pas_ucd_num[m].cp) hi = m - 1;
        else if (cp > pas_ucd_num[m].cp) lo = m + 1;
        else { double d; memcpy(&d, &pas_ucd_num[m].bits, sizeof d); return d; }
    }
    return 0.0;
}
DESCR_t pas_ucd(DESCR_t *args, int nargs) {
    long long cp, what;
    if (nargs != 2) return FAILDESCR;
    cp = rk_ival(args[0]);
    what = rk_ival(args[1]);
    if (cp < 0 || cp > 0x10FFFF) return REALVAL(what == 0 ? 29.0 : 0.0);
    if (what == 0) return REALVAL((double) pas_ucd_cat_of((unsigned int) cp));
    if (what == 1) return REALVAL((double) pas_ucd_map_of(pas_ucd_upper, PAS_UCD_NUPPER, (unsigned int) cp));
    if (what == 2) return REALVAL((double) pas_ucd_map_of(pas_ucd_lower, PAS_UCD_NLOWER, (unsigned int) cp));
    if (what == 3) return REALVAL((double) pas_ucd_ws_of((unsigned int) cp));
    if (what == 4) return REALVAL(pas_ucd_num_of((unsigned int) cp));
    return FAILDESCR;
}
