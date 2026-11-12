#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_26_sh.h"
int core_icn_error(int code, DESCR_t val);
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern void *rt_ws_alloc_descr(size_t);
extern const char *icon_real_str(double r, char *buf, int bufsz);
extern FILE *fh_get(int);
int rt_lv_unraw(DESCR_t *slot);
extern unsigned rt_dtax_gen;
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char *icn_pad_str(DESCR_t d, char *buf, int bufsz) { if (IS_REAL_fn(d)) return icon_real_str(d.r, buf, bufsz); return VARVAL_fn(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_true_len(DESCR_t d, const char *materialized) { if (d.v == DT_S) return (int)descr_slen(d); return materialized ? (int)strlen(materialized) : 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void pas_tf_write(FILE *fp, DESCR_t v) {
    if (v.v == DT_I) { unsigned char tg = 'I'; int64_t x = (int64_t)v.i; fwrite(&tg, 1, 1, fp); fwrite(&x, 8, 1, fp); return; }
    if (v.v == DT_R) { unsigned char tg = 'R'; double x = v.r; fwrite(&tg, 1, 1, fp); fwrite(&x, 8, 1, fp); return; }
    if (v.v == DT_A && v.arr) {
        ARBLK_t *b = (ARBLK_t *)v.arr;
        unsigned char tg = 'A';
        int32_t lo = b->lo, hi = b->hi;
        fwrite(&tg, 1, 1, fp);
        fwrite(&lo, 4, 1, fp);
        fwrite(&hi, 4, 1, fp);
        for (int k = 0; k <= b->hi - b->lo; k++) pas_tf_write(fp, b->data[k]);
        return;
    }
    if (v.v == DT_S || v.v == DT_SNUL) {
        const char *sv = VARVAL_fn(v);
        uint32_t n = (v.v == DT_S && v.slen && v.slen != 0xFFFFFFFFu) ? v.slen : (sv ? (uint32_t)strlen(sv) : 0u);
        unsigned char tg = 'S';
        fwrite(&tg, 1, 1, fp);
        fwrite(&n, 4, 1, fp);
        if (n) fwrite(sv, 1, n, fp);
        return;
    }
    unsigned char tg = 'N';
    fwrite(&tg, 1, 1, fp);
}
int pas_tf_read(FILE *fp, DESCR_t *o) {
    unsigned char tg;
    if (fread(&tg, 1, 1, fp) != 1) { *o = INTVAL(0); return 0; }
    if (tg == 'I') { int64_t x = 0; if (fread(&x, 8, 1, fp) != 1) return 0; *o = INTVAL(x); return 1; }
    if (tg == 'R') { double x = 0; if (fread(&x, 8, 1, fp) != 1) return 0; *o = REALVAL(x); return 1; }
    if (tg == 'S') {
        uint32_t n = 0;
        if (fread(&n, 4, 1, fp) != 1) return 0;
        char *sv = (char *)rt_wsb_alloc((size_t)n + 1);
        if (n && fread(sv, 1, n, fp) != n) return 0;
        sv[n] = 0;
        *o = BSTRVAL(sv, n);
        return 1;
    }
    if (tg == 'A') {
        int32_t lo = 0, hi = -1;
        if (fread(&lo, 4, 1, fp) != 1 || fread(&hi, 4, 1, fp) != 1) return 0;
        long long n = (long long)hi - lo + 1;
        if (n < 1) n = 1;
        ARBLK_t *b = (ARBLK_t *)rt_gcheap_alloc(HB_ARR, sizeof(ARBLK_t));
        b->id = rt_agg_serial_list();
        b->dumpno = rt_sno_dumpno_next();
        b->lo = lo;
        b->hi = hi;
        b->ndim = 1;
        b->lo2 = 0;
        b->hi2 = 0;
        b->proto_bare = 0;
        b->data = (DESCR_t *)rt_ws_alloc_descr((size_t)(n));
        for (long long k = 0; k < n; k++) b->data[k] = INTVAL(0);
        for (int k = 0; k <= hi - lo; k++) if (!pas_tf_read(fp, &b->data[k])) return 0;
        DESCR_t d = {0};
        d.v = DT_A;
        d.slen = 0;
        d.arr = b;
        *o = d;
        return 1;
    }
    *o = NULVCL;
    return 1;
}
static DESCR_t rk_rest_pack(int kind, DESCR_t *tail, int rest) {
    extern DESCR_t rt_make_list(DESCR_t *args, int nargs);
    extern DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
    extern DESCR_t rt_make_item_agg(DESCR_t *args, int nargs);
    extern DESCR_t rt_make_plus_agg(DESCR_t *args, int nargs, int as_list);
    if (kind == 2) return rt_make_item_agg(tail, rest);
    if (kind >= 3) return rt_make_plus_agg(tail, rest, kind == 4);
    return kind == 1 ? rt_make_flat_agg(tail, rest) : rt_make_list(tail, rest);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t bt___rk_slurp(DESCR_t *args, int nargs) {
    if (nargs >= 1 && IS_INT_fn(args[0])) return rk_rest_pack((int) args[0].i, nargs > 1 ? &args[1] : (DESCR_t *)0, nargs - 1);
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
int rt_builtin_tail_may_open(const char *fn) {
    return fn &&
        (!strcmp(fn, "__blk_invoke") || !strcmp(fn, "__multi_call") || !strcmp(fn, "obj_new") || !strcmp(fn, "__rk_named_call") || !strcmp(fn, "__rk_meth_nc") || !strcmp(fn, "meth_call") ||
        !strcmp(fn, "__rk_str") || !strcmp(fn, "__rk_gist_pre") || !strcmp(fn, "__rk_str_pre") || !strcmp(fn, "__rk_eval"));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_lv_unraw(DESCR_t *slot) {
    if (!slot || rt_lv_is_list(*slot, (DESCR_t **)0, (int *)0) != 2) return 0;
    int n = (int)slot->u->fields[1].i;
    uint32_t k = slot->u->fields[0].slen;
    DESCR_t *mem = (DESCR_t *)rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
    DATINST_t *in = slot->u;
    const int64_t *ri = (const int64_t *)in->fields[0].ptr;
    const double *rr = (const double *)in->fields[0].ptr;
    for (int i = 0; i < n; i++) mem[i] = k == DATA_RVEC_R_SLEN ? REALVAL(rr[i]) : INTVAL(ri[i]);
    in->fields[0].ptr = (void *)mem;
    in->fields[0].slen = DATA_ELEMS_SLEN;
    return 1;
}
DESCR_t rt_make_list(DESCR_t *args, int nargs) {
    DESCR_t *elems = (DESCR_t *)0;
    if (nargs > 0) { elems = rt_ws_alloc_descr((size_t)nargs); for (int _j=0;_j<nargs;_j++) elems[_j]=args[_j]; }
    DESCR_t eptr = {0};
    eptr.v=DT_DATA;
    eptr.slen=DATA_ELEMS_SLEN;
    eptr.ptr=(void*)elems;
    return DATCON_fn("list", eptr, INTVAL(nargs), STRVAL("list"), INTVAL(nargs), NULVCL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_list_own(DESCR_t *elems, int nargs) {
    DESCR_t eptr = {0};
    eptr.v=DT_DATA;
    eptr.slen=DATA_ELEMS_SLEN;
    eptr.ptr=(void*)(nargs > 0 ? elems : (DESCR_t *)0);
    return DATCON_fn("list", eptr, INTVAL(nargs), STRVAL("list"), INTVAL(nargs), NULVCL);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_args_list_from(char **v, int n) {
    if (n < 0 || !v) n = 0;
    DESCR_t *tmp = rt_ws_alloc_descr((size_t)(n>0?n:1));
    for (int _i=0;_i<n;_i++) tmp[_i] = STRVAL(v[_i]);
    return rt_make_list(tmp, n);
}
void rt_main_args_stage(char **v, int n) { if (n < 0 || !v) n = 0; g_runtime.bn.main_args_v = v; g_runtime.bn.main_args_n = n; }
int pas_main_argc(void) { return g_runtime.bn.main_args_n > 0 ? g_runtime.bn.main_args_n : 0; }
const char *pas_main_arg(int k) {
    return (k >= 1 && k <= g_runtime.bn.main_args_n && g_runtime.bn.main_args_v && g_runtime.bn.main_args_v[k - 1] && g_runtime.bn.main_args_v[k - 1][0]) ? g_runtime.bn.main_args_v[k - 1] :
        (const char *)0;
}
void rt_main_args_stage_argv(char **v, int n) {
    extern int rt_cmdline_prog_first(void);
    int skip = rt_cmdline_prog_first() - 1;
    if (n < 0 || !v) { rt_main_args_stage(v, 0); return; }
    if (skip < 0) skip = 0;
    if (skip > n) skip = n;
    rt_main_args_stage(v + skip, n - skip);
}
void rt_main_args_bind(void) {
    if (g_runtime.bn.main_args_descr.v == DT_DATA) return;
    if (g_runtime.bn.main_args_n < 0) rt_main_args_stage((char **)0, 0);
    g_runtime.bn.main_args_descr = rt_args_list_from(g_runtime.bn.main_args_v, g_runtime.bn.main_args_n);
    if (!getenv("SCRIP_NO_MAIN_ARGS")) rt_call_args_need(2);
    CALL_ARGS[0] = g_runtime.bn.main_args_descr;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_bi_string_of_int(DESCR_t av, DESCR_t *out) {
    char tmp[24], *e = tmp + sizeof tmp;
    unsigned long long u = av.i < 0 ? 0ull - (unsigned long long)av.i : (unsigned long long)av.i;
    do { *--e = (char)('0' + u % 10ull); u /= 10ull; } while (u);
    if (av.i < 0) *--e = '-';
    size_t n = (size_t)(tmp + sizeof tmp - e);
    char *ib = rt_wsb_alloc(n + 1);
    memcpy(ib, e, n);
    ib[n] = 0;
    *out = (DESCR_t){ .v = DT_S, .slen = (uint32_t)n, .s = ib };
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_icn_trim(DESCR_t *args, int nargs, DESCR_t *out) {
    const char *s=VARVAL_fn(args[0]);
    if(!s)s="";
    const char *cset = " ";
    int cl = 1;
    if (nargs >= 2) { DESCR_t cv = args[1]; if (cv.v != DT_SNUL) { const char *cp; int cn; if (cset_resolve(cv, &cp, &cn)) { cset = cp; cl = cn; } } }
    int sl=icn_true_len(args[0], s);
    while (sl > 0 && cl > 0 && memchr(cset, (unsigned char)s[sl-1], (size_t)cl)) sl--;
    char *buf=rt_wsb_alloc(sl+1);
    memcpy(buf,s,sl);
    buf[sl]='\0';
    *out = BSTRVAL(buf, sl);
    return 1;
}
int bn_icn_put(DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t ld = args[0];
    if (ld.v != DT_DATA) return 0;
    {
        DESCR_t *_a=0;
        int _n=0;
        if (rt_lv_is_list(ld,&_a,&_n)) {
            int _nv2 = (nargs > 1) ? nargs - 1 : 1;
            for (int _pi = 0; _pi < _nv2; _pi++) {
                DESCR_t _vd = (nargs > 1) ? args[1 + _pi] : NULVCL;
                if (ld.u->type->nfields >= 5 && !(ld.u->fields[4].v == DT_I && ld.u->fields[4].slen == 0)) rt_lvb_put(ld,_n);
                long _c = rt_lv_cap(ld);
                if(_a && _c>=0 && _n<_c){ _a[_n]=_vd; _n++; rt_lv_set_size(ld,_n); continue; }
                long _ncap=(_n>0)?(long)_n*2:8;
                if(_ncap<_n+1)_ncap=_n+1;
                DESCR_t *_nb=rt_ws_alloc_descr((size_t)_ncap);
                if(_a&&_n>0) memcpy(_nb,_a,_n*sizeof(DESCR_t));
                _nb[_n]=_vd;
                _a=_nb;
                _n++;
                rt_lv_set_elems(ld,DATA_ELEMS(_nb));
                rt_lv_set_size(ld,_n);
                rt_lv_set_cap(ld,_ncap);
            }
            *out = ld;
            return 1;
        }
    }
    return 0;
}
int bn_icn_get(DESCR_t ld, DESCR_t *out) {
    if (ld.v != DT_DATA) return 0;
    {
        DESCR_t *_a=0;
        int _n=0;
        if (rt_lv_is_list(ld,&_a,&_n)) {
            if(!_a||_n<=0) { *out=FAILDESCR; return 1; }
            rt_lvb_get(ld,_n);
            DESCR_t _r=_a[0];
            rt_lv_set_elems(ld,DATA_ELEMS(_a+1));
            rt_lv_set_size(ld,_n-1);
            { long _c=rt_lv_cap(ld); if(_c>0) rt_lv_set_cap(ld,_c-1); }
            *out=_r;
            return 1;
        }
    }
    return 0;
}
int bn_icn_read_fh(DESCR_t a0, DESCR_t *out) {
    int _fi = (a0.v == DT_SNUL) ? 0 : (IS_FH_fn(a0) || IS_INT_fn(a0)) ? (int)a0.i : -1;
    if (_fi >= 0 && _fi < FH_N && g_fh[_fi].closed) { core_icn_error(212, a0.v == DT_SNUL ? FHVAL(0) : a0); *out = FAILDESCR; return 1; }
    FILE *fp = (_fi >= 0) ? fh_get(_fi) : NULL;
    if (!fp) { *out = FAILDESCR; return 1; }
    extern int fh_is_untranslated(int);
    int _un = fh_is_untranslated(_fi);
    {
        const char *rp = fp->_IO_read_ptr, *re = fp->_IO_read_end;
        const char *nl = (rp && re > rp) ? (const char *)memchr(rp, '\n', (size_t)(re - rp)) : (const char *)0;
        if (nl && (_un || !memchr(rp, '\r', (size_t)(nl - rp)))) {
            size_t fl = (size_t)(nl - rp);
            char *fr = rt_wsb_alloc(fl + 1);
            memcpy(fr, rp, fl);
            fr[fl] = '\0';
            fp->_IO_read_ptr = (char *)nl + 1;
            DESCR_t fs = {0};
            fs.v = DT_S;
            fs.slen = (uint32_t)fl;
            fs.s = fr;
            *out = fs;
            return 1;
        }
    }
    size_t cap = 128, len = 0;
    char *ln = (char *)ct_alloc(cap);
    if (!ln) { *out = FAILDESCR; return 1; }
    int c, any = 0;
    while ((c = getc_unlocked(fp)) != EOF) {
        any = 1;
        if (c == '\n') break;
        if (!_un && c == '\r') { int d = getc_unlocked(fp); if (d != '\n' && d != EOF) ungetc(d, fp); break; }
        if (len + 1 >= cap) { cap *= 2; char *nb = (char *)ct_grow(ln, cap); if (!nb) { ct_drop(ln); *out = FAILDESCR; return 1; } ln = nb; }
        ln[len++] = (char)c;
    }
    if (!any) { ct_drop(ln); *out = FAILDESCR; return 1; }
    char *r = rt_wsb_alloc(len + 1);
    memcpy(r, ln, len);
    r[len] = '\0';
    ct_drop(ln);
    DESCR_t rs = {0};
    rs.v = DT_S;
    rs.slen = (uint32_t)len;
    rs.s = r;
    *out = rs;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_icn_right(DESCR_t *args, int nargs, DESCR_t *out) {
    char _pb[64];
    const char *s=icn_pad_str(args[0],_pb,sizeof _pb);
    if(!s)s="";
    int sl=icn_true_len(args[0], s);
    int n = 1;
    if (nargs >= 2) { DESCR_t nv = args[1]; if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv); }
    if (n < 0) n = 0;
    const char *fill=" ";
    int fl=1;
    if (nargs >= 3) {
        DESCR_t fd = args[2];
        if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) { const char *fs = VARVAL_fn(fd); { int _fl = fs ? icn_true_len(fd, fs) : 0; if (_fl > 0) { fill = fs; fl = _fl; } } }
    }
    char *buf=rt_wsb_alloc(n+1);
    int pad = n - sl;
    if (pad < 0) pad = 0;
    for (int i = 0; i < pad; i++) buf[i] = fill[i % fl];
    int srcoff = (sl > n) ? (sl - n) : 0;
    int copy = sl - srcoff;
    if (pad + copy > n) copy = n - pad;
    for (int i = 0; i < copy; i++) buf[pad + i] = s[srcoff + i];
    buf[n]='\0';
    *out = BSTRVAL(buf, n);
    return 1;
}
int bn_icn_left(DESCR_t *args, int nargs, DESCR_t *out) {
    char _pb[64];
    const char *s=icn_pad_str(args[0],_pb,sizeof _pb);
    if(!s)s="";
    int sl=icn_true_len(args[0], s);
    int n = 1;
    if (nargs >= 2) { DESCR_t nv = args[1]; if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv); }
    if (n < 0) n = 0;
    const char *fill=" ";
    int fl=1;
    if (nargs >= 3) {
        DESCR_t fd = args[2];
        if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) { const char *fs = VARVAL_fn(fd); { int _fl = fs ? icn_true_len(fd, fs) : 0; if (_fl > 0) { fill = fs; fl = _fl; } } }
    }
    char *buf=rt_wsb_alloc(n+1);
    int copy = sl < n ? sl : n;
    for (int i = 0; i < copy; i++) buf[i] = s[i];
    int rpad = n - copy;
    for (int k = 0; k < rpad; k++) { int idx = ((k + fl - rpad) % fl + fl) % fl; buf[copy + k] = fill[idx]; }
    buf[n]='\0';
    *out = BSTRVAL(buf, n);
    return 1;
}
int bn_icn_center(DESCR_t *args, int nargs, DESCR_t *out) {
    char _pb[64];
    const char *s=icn_pad_str(args[0],_pb,sizeof _pb);
    if(!s)s="";
    int sl=icn_true_len(args[0], s);
    int n = 1;
    if (nargs >= 2) { DESCR_t nv = args[1]; if (!IS_FAIL_fn(nv) && nv.v != DT_SNUL) n = (int)to_int(nv); }
    if (n < 0) n = 0;
    const char *fill=" ";
    int fl=1;
    if (nargs >= 3) {
        DESCR_t fd = args[2];
        if (!IS_FAIL_fn(fd) && fd.v != DT_SNUL) { const char *fs = VARVAL_fn(fd); { int _fl = fs ? icn_true_len(fd, fs) : 0; if (_fl > 0) { fill = fs; fl = _fl; } } }
    }
    char *buf=rt_wsb_alloc(n+1);
    int lpad = (n - sl) / 2;
    if (lpad < 0) lpad = 0;
    int srcoff = (sl > n) ? (sl - n + 1) / 2 : 0;
    int copy = sl - srcoff;
    if (lpad + copy > n) copy = n - lpad;
    int rpad = n - lpad - copy;
    for (int i = 0; i < lpad; i++) buf[i] = fill[i % fl];
    for (int i = 0; i < copy; i++) buf[lpad + i] = s[srcoff + i];
    for (int k = 0; k < rpad; k++) { int idx = ((k + fl - rpad) % fl + fl) % fl; buf[lpad + copy + k] = fill[idx]; }
    buf[n]='\0';
    *out = BSTRVAL(buf, n);
    return 1;
}
static int icn_bi_direct_list_io(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    extern int rt_g_want_name;
    if (g_runtime.bn.dtax_bid[bid].kind != 0 && g_runtime.bn.dtax_bid[bid].kind != 3) return 0;
    if (bid == BID_trim) {
        if (nargs < 1 || nargs > 2 || args[0].v != DT_S || IS_CSET_fn(args[0])) return 0;
        if (nargs == 2 && args[1].v != DT_SNUL && args[1].v != DT_S) return 0;
        core_icn_op_ctx_clear();
        return bn_icn_trim(args, nargs, out);
    }
    if (bid == BID_put) { if (nargs < 1 || !rt_lv_is_list(args[0], (DESCR_t **)0, (int *)0)) return 0; core_icn_op_ctx_clear(); return bn_icn_put(args, nargs, out); }
    if (bid == BID_get) { if (nargs != 1 || rt_g_want_name || !rt_lv_is_list(args[0], (DESCR_t **)0, (int *)0)) return 0; core_icn_op_ctx_clear(); return bn_icn_get(args[0], out); }
    if (nargs != 1) return 0;
    const long rfi = args[0].v == DT_SNUL ? 0 : IS_FH_fn(args[0]) ? (long)args[0].i : -1;
    if (rfi < 0 || rfi >= FH_N || g_fh[rfi].closed || !fh_get((int)rfi)) return 0;
    core_icn_op_ctx_clear();
    return bn_icn_read_fh(args[0], out);
}
static int icn_bi_direct_scalar(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    extern int rt_g_want_name;
    if (g_runtime.bn.dtax_bid[bid].kind != 0 && g_runtime.bn.dtax_bid[bid].kind != 3) return 0;
    if (bid == BID_type) { if (nargs != 1 || rt_g_want_name) return 0; core_icn_op_ctx_clear(); return bn_type_datatype("type", args, nargs, out); }
    if (bid == BID_integer) {
        if (nargs != 1) return 0;
        if (args[0].v == DT_I) { core_icn_op_ctx_clear(); *out = args[0]; return 1; }
        if (args[0].v == DT_R && fabs(args[0].r) < 9223372036854775807.0) { core_icn_op_ctx_clear(); *out = INTVAL((long long)args[0].r); return 1; }
        return 0;
    }
    if (bid == BID_abs) {
        if (nargs != 1) return 0;
        if (args[0].v == DT_R) { core_icn_op_ctx_clear(); *out = REALVAL(fabs(args[0].r)); return 1; }
        if (args[0].v == DT_I && args[0].i != (-9223372036854775807LL - 1)) { core_icn_op_ctx_clear(); *out = INTVAL(args[0].i < 0 ? -args[0].i : args[0].i); return 1; }
        return 0;
    }
    if (bid == BID_atan) {
        if (nargs < 1 || nargs > 2 || (args[0].v != DT_I && args[0].v != DT_R) || (nargs == 2 && args[1].v != DT_SNUL && args[1].v != DT_I && args[1].v != DT_R)) return 0;
        core_icn_op_ctx_clear();
        double v = args[0].v == DT_R ? args[0].r : (double)args[0].i;
        if (nargs == 2 && args[1].v != DT_SNUL) { double v2 = args[1].v == DT_R ? args[1].r : (double)args[1].i; *out = REALVAL(atan2(v, v2)); } else *out = REALVAL(atan(v));
        return 1;
    }
    return 0;
}
static int icn_bi_direct_pad(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    if (g_runtime.bn.dtax_bid[bid].kind != 0 && g_runtime.bn.dtax_bid[bid].kind != 3) return 0;
    if (nargs < 2 || nargs > 3 || (args[0].v != DT_I && (args[0].v != DT_S || IS_CSET_fn(args[0]))) || args[1].v != DT_I || args[1].i < 0) return 0;
    if (nargs == 3 && args[2].v != DT_SNUL && (args[2].v != DT_S || IS_CSET_fn(args[2]))) return 0;
    core_icn_op_ctx_clear();
    return bid == BID_left ? bn_icn_left(args, nargs, out) : bid == BID_center ? bn_icn_center(args, nargs, out) : bn_icn_right(args, nargs, out);
}
static int icn_bi_direct_write(int bid, DESCR_t *args, int nargs, DESCR_t *out) {
    if (g_runtime.bn.dtax_bid[bid].kind != 0 && g_runtime.bn.dtax_bid[bid].kind != 3) return 0;
    if (nargs < 1 || g_fh[1].closed) return 0;
    for (int i = 0; i < nargs; i++) if (!((args[i].v == DT_S && !IS_CSET_fn(args[i])) || args[i].v == DT_I)) return 0;
    core_icn_op_ctx_clear();
    for (int i = 0; i < nargs; i++) {
        if (args[i].v == DT_I) { out_write_descr(stdout, args[i], bid == BID_write); continue; }
        const char *bs = VARVAL_fn(args[i]);
        uint32_t bn = args[i].slen ? args[i].slen : (bs ? (uint32_t)strlen(bs) : 0u);
        if (bs && bn) fwrite(bs, 1, bn, stdout);
    }
    if (bid == BID_write) fputc('\n', stdout);
    *out = args[nargs - 1];
    return 1;
}
int icn_bi_direct(int bidlen, DESCR_t *args, int nargs, DESCR_t *out) {
    const int bid = bidlen & BID_BAKE_MASK;
    const unsigned fnlen = (unsigned)bidlen >> 16;
    for (int i = 0; i < nargs; i++) if (args[i].v == DT_DATA) rt_lv_unraw(&args[i]);
    switch (bid) {
        case BID_string:
        case BID_IDENTICAL:
        case BID_trim:
        case BID_put:
        case BID_get:
        case BID_read:
        case BID_type:
        case BID_integer:
        case BID_abs:
        case BID_atan:
        case BID_right:
        case BID_write:
        case BID_writes:
        case BID_any:
        case BID_many:
        case BID_left:
        case BID_center:
        case BID_tab:
        case BID_move:
        break;
        default:
        return 0;
    }
    if (!fnlen || fnlen >= 14 || dtax_off() || g_runtime.bn.dtax_bid[bid].gen != rt_dtax_gen) return 0;
    switch (bid) {
        case BID_write:
        case BID_writes:
        return icn_bi_direct_write(bid, args, nargs, out);
        case BID_any:
        case BID_many:
        return icn_bi_direct_scan(bid, args, nargs, out);
        case BID_tab:
        case BID_move:
        return icn_bi_direct_scanpos(bid, args, nargs, out);
        case BID_type:
        case BID_integer:
        case BID_abs:
        case BID_atan:
        return icn_bi_direct_scalar(bid, args, nargs, out);
        case BID_right:
        case BID_left:
        case BID_center:
        return icn_bi_direct_pad(bid, args, nargs, out);
        case BID_trim:
        case BID_put:
        case BID_get:
        case BID_read:
        return icn_bi_direct_list_io(bid, args, nargs, out);
        case BID_IDENTICAL:
        { if (nargs != 2 || (g_runtime.bn.dtax_bid[bid].kind != 0 && g_runtime.bn.dtax_bid[bid].kind != 3)) return 0; *out = bn_identical(args[0], args[1]) ? args[1] : FAILDESCR; return 1; }
        default:
        break;
    }
    if (nargs != 1 || g_runtime.bn.dtax_bid[bid].kind != 0) return 0;
    if (args[0].v == DT_I) return icn_bi_string_of_int(args[0], out);
    if (args[0].v == DT_S && !IS_CSET_fn(args[0])) { *out = args[0]; return 1; }
    return 0;
}
void dtx4(dtax_ent_t *e, unsigned char l, const char *nm, void *h) {
    if (!e || !l) return;
    e->gen = rt_dtax_gen;
    e->len = l;
    e->kind = 4;
    e->nf = 0;
    memcpy(e->dxn, nm, l);
    e->ctor = h;
    e->syn = 0;
}
int bn_uni_list_insert(DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t ld = args[0];
    DESCR_t *a = 0;
    int n = 0;
    if (!rt_lv_is_list(ld, &a, &n)) return 0;
    long p = nargs > 1 ? to_int(args[1]) : 0;
    if (p <= 0) p = (long) n + 1 + p;
    if (p < 1 || p > (long) n + 1) { *out = FAILDESCR; return 1; }
    DESCR_t pa[2] = { ld, nargs > 2 ? args[2] : NULVCL };
    DESCR_t ignore;
    bn_icn_put(pa, 2, &ignore);
    if (!rt_lv_is_list(ld, &a, &n)) return 0;
    if (p <= (long) n - 1) { memmove(&a[p], &a[p - 1], (size_t)(n - p) * sizeof(DESCR_t)); a[p - 1] = pa[1]; }
    *out = ld;
    return 1;
}
int bn_uni_list_delete(DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t ld = args[0];
    DESCR_t *a = 0;
    int n = 0;
    if (!rt_lv_is_list(ld, &a, &n)) return 0;
    long p = nargs > 1 ? to_int(args[1]) : 0;
    if (p <= 0) p = (long) n + 1 + p;
    if (a && p >= 1 && p <= (long) n) { memmove(&a[p - 1], &a[p], (size_t)(n - p) * sizeof(DESCR_t)); rt_lv_set_size(ld, n - 1); }
    *out = ld;
    return 1;
}
int bn_uni_list_reverse(DESCR_t *args, int nargs, DESCR_t *out) {
    DESCR_t *a = 0;
    int n = 0;
    (void) nargs;
    if (!rt_lv_is_list(args[0], &a, &n)) return 0;
    DESCR_t *r = rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
    for (int i = 0; i < n; i++) r[i] = a[n - 1 - i];
    *out = rt_make_list(r, n);
    return 1;
}
DESCR_t rt_sno_mkexpr_of(DESCR_t a) {
    extern void *bb_dstar_rec_intern(const char *, uint32_t);
    sno_dstar_rec_t *rec = (a.v == DT_I) ? (sno_dstar_rec_t *)(uintptr_t)a.i : (sno_dstar_rec_t *)0;
    if (!rec) {
        const char *nm = VARVAL_fn(a);
        if (!nm) nm = "";
        size_t nl = strlen(nm);
        char key[nl + 2];
        key[0] = '*';
        memcpy(key + 1, nm, nl + 1);
        rec = (sno_dstar_rec_t *)bb_dstar_rec_intern(key, 0u);
    }
    DESCR_t xd = {0};
    xd.v = DT_X;
    xd.slen = 0;
    xd.p = rec;
    return xd;
}
DESCR_t bt_ICNx24ULIST(DESCR_t *args, int nargs) {
    extern DESCR_t rt_make_list(DESCR_t *, int);
    DESCR_t * el = NULL;
    int n = 0;
    if (nargs == 1 && args[0].v == DT_T && args[0].tbl && args[0].tbl->is_set) {
        DESCR_t * mem;
        TBPAIR_t * e;
        int k;
        extern int table_icn_nth(TBBLK_t *, int64_t, TBPAIR_t **);
        while (table_icn_nth(args[0].tbl, n, &e)) n++;
        mem = rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
        k = n;
        for (int i = 0; i < n; i++) if (table_icn_nth(args[0].tbl, i, &e)) mem[--k] = e->key_descr;
        return rt_make_list(mem, n);
    }
    if (nargs == 1 && args[0].v == DT_DATA && rt_lv_is_list(args[0], &el, &n)) {
        int ki = 1, kr = 1;
        for (int i = 0; i < n; i++) { ki &= IS_INT_fn(el[i]) ? 1 : 0; kr &= IS_REAL_fn(el[i]) ? 1 : 0; }
        return n > 0 && (ki || kr) ? rt_make_list(el, n) : args[0];
    }
    { DESCR_t out = FAILDESCR; if (icn_argtype_gate(BID_list, args, nargs, &out, 1)) return out; }
    if (nargs > 2) return FAILDESCR;
    if (nargs >= 1 && !IS_FAIL_fn(args[0]) && args[0].v != DT_SNUL) {
        if (IS_INT_fn(args[0])) n = (int) args[0].i;
        else if (IS_REAL_fn(args[0])) n = (int) args[0].r;
        else if (icn_cvt_int_ok(args[0])) n = (int) to_int(args[0]);
        else return FAILDESCR;
    }
    if (n < 0) return FAILDESCR;
    { DESCR_t * mem = rt_ws_alloc_descr((size_t)(n > 0 ? n : 1)); for (int i = 0; i < n; i++) mem[i] = (nargs >= 2 && !IS_FAIL_fn(args[1])) ? args[1] : NULVCL; return rt_make_list_own(mem, n); }
}
