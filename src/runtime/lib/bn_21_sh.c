#include "runtime/lib/bn_head_call_args.h"
#include "runtime/lib/bn_head.h"
#include "runtime/lib/bn_head_rk_av.h"
#include "runtime/rt/rt_arena.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_21_sh.h"
#include "runtime/rk_unicode_norm_tables.inc"
#include "ir/stage2_sm_prog_api.h"
#include "runtime/core/core_rtx_str_shim_api.h"
#include "runtime/rt/gc_heap_etc.h"
#include "runtime/rt/gc_heap.h"
#include "runtime/rt/gc_heap_rt_asm_helpers_api.h"
#include "ir/descr_tags.h"
#include "ir/descr_tags_dt_platom.h"
#include "ir/ct_arena.h"
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern DESCR_t dat_construct_rq(DatType *, DESCR_t *, int, long *);
extern void *rt_ws_alloc_descr(size_t);
extern FILE *fh_get(int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rk_un_ccc_of(unsigned cp) {
    unsigned lo = 0, hi = rk_un_ccc_n;
    while (lo < hi) { unsigned mid = (lo + hi) / 2; if (cp < rk_un_ccc[mid][0]) hi = mid; else if (cp > rk_un_ccc[mid][1]) lo = mid + 1; else return rk_un_ccc[mid][2]; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const unsigned *rk_un_row(const unsigned (*t)[3], unsigned n, unsigned key) {
    unsigned lo = 0, hi = n;
    while (lo < hi) { unsigned mid = (lo + hi) / 2; if (key < t[mid][0]) hi = mid; else if (key > t[mid][0]) lo = mid + 1; else return t[mid]; }
    return NULL;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned rk_un_compose2(unsigned a, unsigned b) {
    if (a >= 0x1100 && a <= 0x1112 && b >= 0x1161 && b <= 0x1175) return 0xAC00 + ((a - 0x1100) * 21 + (b - 0x1161)) * 28;
    if (a >= 0xAC00 && a <= 0xD7A3 && (a - 0xAC00) % 28 == 0 && b >= 0x11A8 && b <= 0x11C2) return a + (b - 0x11A7);
    unsigned lo = 0, hi = rk_un_comp_n;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if (a < rk_un_comp[mid][0] || (a == rk_un_comp[mid][0] && b < rk_un_comp[mid][1])) hi = mid;
        else if (a > rk_un_comp[mid][0] || b > rk_un_comp[mid][1]) lo = mid + 1;
        else return rk_un_comp[mid][2];
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_un_decomp(unsigned cp, int compat, unsigned *out, int n) {
    if (cp >= 0xAC00 && cp <= 0xD7A3) { unsigned si = cp - 0xAC00; out[n++] = 0x1100 + si / 588; out[n++] = 0x1161 + (si % 588) / 28; if (si % 28) out[n++] = 0x11A7 + si % 28; return n; }
    const unsigned *r = rk_un_row(rk_un_canon, rk_un_canon_n, cp);
    if (r) { n = rk_un_decomp(r[1], compat, out, n); if (r[2]) n = rk_un_decomp(r[2], compat, out, n); return n; }
    if (compat && (r = rk_un_row(rk_un_compat, rk_un_compat_n, cp))) { for (unsigned i = 0; i < r[2]; i++) n = rk_un_decomp(rk_un_compat_pool[r[1] + i], compat, out, n); return n; }
    out[n++] = cp;
    return n;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static unsigned *rk_un_normalize(const unsigned *in, int n, int compat, int compose, int *outn) {
    unsigned *o = (unsigned *) rt_wsb_alloc(sizeof(unsigned) * (size_t) (n * 20 + 8));
    int m = 0;
    for (int i = 0; i < n; i++) m = rk_un_decomp(in[i], compat, o, m);
    for (int i = 0; i < m; ) {
        if (!rk_un_ccc_of(o[i])) { i++; continue; }
        int j = i;
        while (j < m && rk_un_ccc_of(o[j])) j++;
        for (int a = i + 1; a < j; a++) { unsigned v = o[a], cv = rk_un_ccc_of(v); int b = a - 1; while (b >= i && rk_un_ccc_of(o[b]) > cv) { o[b + 1] = o[b]; b--; } o[b + 1] = v; }
        i = j;
    }
    if (compose && m > 0) {
        int w = 1, st = rk_un_ccc_of(o[0]) == 0 ? 0 : -1;
        unsigned last = st == 0 ? 0 : 256;
        for (int i = 1; i < m; i++) {
            unsigned c = o[i], cc = rk_un_ccc_of(c), comp = 0;
            if (st >= 0) { int blocked = (w - 1 > st) && (last >= cc || last == 0); if (!blocked) comp = rk_un_compose2(o[st], c); }
            if (comp) { o[st] = comp; continue; }
            if (cc == 0) { st = w; last = 0; } else last = cc;
            o[w++] = c;
        }
        m = w;
    }
    *outn = m;
    return o;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *const g_rk_uni_types[] = { "Uni", "NFC", "NFD", "NFKC", "NFKD", NULL };
static void rk_uni_reg(void) {
    if (dat_find_type("Uni")) return;
    for (int i = 0; g_rk_uni_types[i]; i++) { char spec[strlen(g_rk_uni_types[i]) + 16]; snprintf(spec, sizeof spec, "%s(codes)", g_rk_uni_types[i]); DEFDAT_fn(spec); }
}
static DESCR_t rk_uni_make(const char *tn, const unsigned *cps, int n) {
    rk_uni_reg();
    DESCR_t *el = n ? (DESCR_t *) rt_ws_alloc_descr((size_t) n) : NULL;
    for (int i = 0; i < n; i++) el[i] = INTVAL(cps[i]);
    return DATCON_fn(tn, rk_unmark(rk_mk_arr(el, n)));
}
static int rk_uni_count(DESCR_t d) {
    if (d.v == DT_A && d.arr) { rk_av_t a = rk_av(d); int c = 0; for (int i = 0; i < a.n; i++) c += rk_uni_count(rk_av_elem(a, i)); return c; }
    return IS_INT_fn(d) ? 1 : 0;
}
static int rk_uni_fill(DESCR_t d, unsigned *buf, int n) {
    if (d.v == DT_A && d.arr) { rk_av_t a = rk_av(d); for (int i = 0; i < a.n; i++) n = rk_uni_fill(rk_av_elem(a, i), buf, n); return n; }
    if (IS_INT_fn(d)) buf[n++] = (unsigned) d.i;
    return n;
}
static DESCR_t rk_uni_new(const char *tn, DESCR_t *args, int nargs) {
    int total = 0, n = 0;
    for (int i = 0; i < nargs; i++) total += rk_uni_count(args[i]);
    unsigned *buf = (unsigned *) rt_wsb_alloc(sizeof(unsigned) * (size_t) (total + 1));
    for (int i = 0; i < nargs; i++) n = rk_uni_fill(args[i], buf, n);
    return rk_uni_make(tn, buf, n);
}
static int rk_uni_codes(DESCR_t self, unsigned **out) {
    rk_av_t a = rk_av(FIELD_GET_fn(self, "codes"));
    unsigned *c = (unsigned *) rt_wsb_alloc(sizeof(unsigned) * (size_t) (a.n + 1));
    for (int i = 0; i < a.n; i++) { DESCR_t e = rk_av_elem(a, i); c[i] = IS_INT_fn(e) ? (unsigned) e.i : 0; }
    *out = c;
    return a.n;
}
static int rk_uni_form(const char *m, int *compat, int *compose) {
    if (!strcmp(m, "NFC")) { *compat = 0; *compose = 1; return 1; }
    if (!strcmp(m, "NFD")) { *compat = 0; *compose = 0; return 1; }
    if (!strcmp(m, "NFKC")) { *compat = 1; *compose = 1; return 1; }
    if (!strcmp(m, "NFKD")) { *compat = 1; *compose = 0; return 1; }
    return 0;
}
int rk_uni_method(const char *m, DESCR_t self, int nmargs, DESCR_t *out) {
    if (nmargs || !IS_DATA_INST_fn(self) || !self.u || !self.u->type || !rk_uni_is_name(self.u->type->name)) return 0;
    unsigned *c;
    int n = rk_uni_codes(self, &c), compat, compose;
    if (rk_uni_form(m, &compat, &compose)) { int on; unsigned *r = rk_un_normalize(c, n, compat, compose, &on); *out = rk_uni_make(m, r, on); return 1; }
    if (!strcmp(m, "list") || !strcmp(m, "Seq") || !strcmp(m, "values") || !strcmp(m, "flat")) { *out = rk_mark_list(FIELD_GET_fn(self, "codes")); return 1; }
    if (!strcmp(m, "elems") || !strcmp(m, "codes") || !strcmp(m, "Int") || !strcmp(m, "Numeric")) { *out = INTVAL(n); return 1; }
    if (!strcmp(m, "Str")) { *out = STRVAL((char *) rk_uni_str(c, n)); return 1; }
    if (!strcmp(m, "gist") || !strcmp(m, "raku") || !strcmp(m, "perl")) {
        size_t cap = (size_t) n * 7 + 16 + strlen(self.u->type->name);
        char *r = rt_wsb_alloc(cap);
        size_t k = (size_t) snprintf(r, cap, "%s:0x<", self.u->type->name);
        for (int i = 0; i < n; i++) k += (size_t) snprintf(r + k, cap - k, "%s%X", i ? " " : "", c[i]);
        snprintf(r + k, cap - k, ">");
        *out = STRVAL(r);
        return 1;
    }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_str_norm_method(const char *m, DESCR_t recv, int nmargs, DESCR_t *out) {
    int compat, compose;
    if (nmargs || !IS_STR_fn(recv) || rk_typeobj_name(recv) || !rk_uni_form(m, &compat, &compose)) return 0;
    const char *s = recv.s ? recv.s : "";
    size_t len = strlen(s), cap = len + 1;
    unsigned *c = (unsigned *) rt_wsb_alloc(sizeof(unsigned) * cap);
    int n = 0;
    for (size_t i = 0, ln; i < len; i += ln) { c[n++] = rk_utf8_cp((const unsigned char *) s + i, len - i, &ln); if (!ln) ln = 1; }
    int on;
    unsigned *r = rk_un_normalize(c, n, compat, compose, &on);
    *out = rk_uni_make(m, r, on);
    return 1;
}
DESCR_t bt_spurt(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *path = VARVAL_fn(args[0]);
        if (!path || !*path) { return FAILDESCR; }
        const char *content = VARVAL_fn(args[1]);
        if (!content) content = "";
        FILE *fp = fopen(path, "w");
        if (!fp) { return FAILDESCR; }
        fputs(content, fp);
        fclose(fp);
        return INTVAL(0);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_print_fh(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        extern void fh_ensure_init(void);
        extern FILE *fh_get(int);
        fh_ensure_init();
        int idx = (int)(IS_INT_fn(args[0]) || IS_FH_fn(args[0]) ? args[0].i : 1);
        FILE *fp = fh_get(idx);
        if (!fp) fp = stdout;
        if (fp != stdout) fflush(stdout);
        char scratch[64];
        const char *s = to_cstring(args[1], scratch, sizeof scratch);
        fputs(s, fp);
        if ((_bop == 1)) fputc('\n', fp);
        return INTVAL(0);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_say_fh(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_print_fh(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_print_fh(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_print_fh(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_field_get_pub(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *fname = VARVAL_fn(args[1]);
        if (!fname) fname = "";
        if (IS_DATA_INST_fn(args[0]) && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? rk_dat_tname(di->type->name) : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[fmt_len("Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn)];
                snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m);
                return FAILDESCR;
            }
        }
        extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
        return rtx_try_dat_field_get(fname, args[0]);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_field_set(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]);
        if (!fname) fname = "";
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        return args[2];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_field_set_pub(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        extern DESCR_t *data_field_ptr(const char *fname, DESCR_t inst);
        const char *fname = VARVAL_fn(args[1]);
        if (!fname) fname = "";
        if (IS_DATA_INST_fn(args[0]) && args[0].u) {
            DATINST_t *di = (DATINST_t *)args[0].u;
            const char *cn = (di && di->type) ? rk_dat_tname(di->type->name) : NULL;
            extern int dat_field_is_private(const char *cls, const char *field);
            if (cn && dat_field_is_private(cn, fname)) {
                extern void rt_script_die_surface(const char *msg);
                char _m[fmt_len("Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn)];
                snprintf(_m, sizeof _m, "Attribute '$!%s' not accessible outside of class %s (it is private)", fname, cn);
                rt_script_die_surface(_m);
                return FAILDESCR;
            }
            DatType *dt = cn ? dat_find_type(cn) : NULL;
            if (dt) {
                int isrw = 0, found = 0;
                for (int i = 0; i < dt->nfields; i++) if (!strcmp(dt->fields[i], fname)) { found = 1; isrw = dt->rw[i]; break; }
                if (found && !isrw) {
                    extern void rt_script_die_surface(const char *msg);
                    char _m[fmt_len("Cannot modify an immutable attribute '$.%s' (declare it 'is rw' to allow assignment)", fname)];
                    snprintf(_m, sizeof _m, "Cannot modify an immutable attribute '$.%s' (declare it 'is rw' to allow assignment)", fname);
                    rt_script_die_surface(_m);
                    return FAILDESCR;
                }
            }
        }
        DESCR_t *cell = data_field_ptr(fname, args[0]);
        if (cell) *cell = args[2];
        return args[2];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
int bt_nqpx3Ax3Acreate(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs == 1) {
        const char *cname = NULL;
        if (IS_DATA_INST_fn(args[0]) && args[0].u) { DATINST_t *di = (DATINST_t *)args[0].u; cname = (di && di->type) ? di->type->name : NULL; } else cname = VARVAL_fn(args[0]);
        DatType *dt = cname ? dat_find_type(cname) : NULL;
        if (!dt) { *out = FAILDESCR; return 1; }
        DESCR_t fvals[dt->nfields > 0 ? dt->nfields : 1];
        for (int fi = 0; fi < dt->nfields; fi++) fvals[fi] = NULVCL;
        *out = dat_construct_rq(dt, fvals, dt->nfields, rq);
        return 1;
    }
    return 0;
}
int bt_obj_new(DESCR_t *args, int nargs, DESCR_t *out, long *rq) {
    if (nargs >= 1) {
        extern DESCR_t rk_attr_defaults(DatType *dt, DESCR_t inst);
        { const char *xc = VARVAL_fn(args[0]); if (xc && ((xc[0] == 'X' && xc[1] == ':') || !strcmp(xc, "Exception"))) rk_exc_init(); }
        const char *cname = VARVAL_fn(args[0]);
        if (!cname || !*cname) { *out = FAILDESCR; return 1; }
        const char *rmc = resolve_method_chain(cname, "new", NULL);
        char newproc[fmt_len("%s__new", rmc)];
        snprintf(newproc, sizeof newproc, "%s__new", rmc);
        extern int rt_proc_has_native_fn(const char *name);
        if (meth_is_user_proc(newproc)) {
            int pi;
            for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, newproc)) break;
            if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(newproc)) {
                int total = nargs;
                rt_call_args_need(total);
                for (int k = 0; k < total; k++) CALL_ARGS[k] = args[k];
                *out = rk_proc_descr_rq(newproc, total, rq);
                return 1;
            }
            extern DESCR_t ir_call_proc(int pi, DESCR_t *args, int nargs);
            *out = ir_call_proc(pi, args, nargs);
            return 1;
        }
        if (rk_uni_is_name(cname) && !dat_find_type(cname)) { *out = rk_uni_new(cname, &args[1], nargs - 1); return 1; }
        if (rk_uni_is_name(cname)) { rk_uni_reg(); *out = rk_uni_new(cname, &args[1], nargs - 1); return 1; }
        DatType *dt = dat_find_type(cname);
        if (!dt) { *out = FAILDESCR; return 1; }
        extern int dat_has_build_mro(const char *cls);
        if (dat_has_build_mro(cname)) { extern DESCR_t rt_construct_build(DatType *t, DESCR_t *named, int nnamed); *out = rk_attr_defaults(dt, rt_construct_build(dt, &args[1], nargs - 1)); return 1; }
        DESCR_t fvals[dt->nfields > 0 ? dt->nfields : 1];
        for (int fi = 0; fi < dt->nfields; fi++) fvals[fi] = NULVCL;
        for (int ci = 1; ci + 1 < nargs; ci += 2) {
            const char *kname = VARVAL_fn(args[ci]);
            if (!kname) continue;
            for (int fi = 0; fi < dt->nfields; fi++) { if (strcmp(dt->fields[fi], kname) == 0) { if (!dt->priv[fi]) fvals[fi] = args[ci + 1]; break; } }
        }
        *out = rk_attr_defaults(dt, dat_construct_rq(dt, fvals, dt->nfields, rq));
        return 1;
    }
    return 0;
}
DESCR_t rk_attr_defaults(DatType *dt, DESCR_t inst) {
    DATINST_t *di = (IS_DATA_INST_fn(inst) && inst.u) ? (DATINST_t *) inst.u : NULL;
    if (!di || !dt) return inst;
    for (int i = 0; i < dt->nfields; i++) {
        DESCR_t f0 = di->fields[i];
        int unset = f0.v == DT_SNUL || ((dt->sigil[i] == '@' || dt->sigil[i] == '%') && IS_STR_fn(f0) && (!f0.s || !f0.s[0]));
        if (!unset || dt->has_default[i]) continue;
        di->fields[i] = dt->sigil[i] == '@' ? rk_unmark(rk_mk_arr(NULL, 0)) : dt->sigil[i] == '%' ? TABLE_VAL(table_new()) : rk_typeobj("Any");
    }
    return inst;
}
