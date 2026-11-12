#include "runtime/lib/bn_head.h"
#include "ir/dtp.h"
#include "runtime/by_name_dispatch.h"
#include "ir/rk_opname.h"
#include "runtime/core/core_error.h"
#include "runtime/core/icn_builtins.h"
#include "runtime/core/nv.h"
#include "runtime/snobol4_system_fns.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/builtins/gen_value.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_ids.h"
#include <ctype.h>
#include "runtime/core/core_errjmp.h"
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_05_pl.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_10_pas.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_26_sh.h"
#include "runtime/lib/bn_31_pl.h"
#include "runtime/lib/bn_41_sh.h"
int core_icn_error(int code, DESCR_t val);
extern int c_rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op);
extern gv_t g_call_args;
void rt_call_args_need(int n);
extern DESCR_t dat_construct_rq(DatType *, DESCR_t *, int, long *);
extern void *rt_ws_alloc_descr(size_t);
extern int rt_proc_is_registered(const char *name);
extern DESCR_t rt_deref(DESCR_t d);
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen, int strict, int sn4, long *rq);
DESCR_t c_rt_str_coerce(DESCR_t d);
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op, int blank0, int num_text);
extern unsigned rt_dtax_gen;
rt_call_next_t rt_apply_open(DESCR_t *args, int nargs) {
    extern rt_call_next_t rt_call_open_by_name(const char *, int);
    extern int rt_proc_is_registered(const char *);
    extern void rt_c2bb_hit(const char *site, const char *name);
    rt_call_next_t none = (rt_call_next_t){ 0, 0 };
    if (!args || nargs < 1) return none;
    {
        const char *pn = (args[0].v == DT_N && args[0].slen == 0) ? args[0].s : VARVAL_fn(args[0]);
        if (!pn || !*pn) { extern void core_runtime_error(int, const char *); core_runtime_error(60, "apply first arg is not natural variable name"); return none; }
        {
            int na = nargs - 1;
            if ((uint64_t)(na > 8 ? na : 8) > g_call_args.cap) rt_call_args_need(na);
            for (int k = 0; k < na; k++) CALL_ARGS[k] = args[k + 1];
#if RT_DIAG
            rt_c2bb_hit("apply.open", pn);
#endif
            if (na == 1 && CALL_ARGS[0].v == DT_X && pn[0] == 'E' && !strcmp(pn, "EVAL")) {
                extern int rt_dtx_open_tail(sno_dstar_rec_t *, long *);
                long rq[2] = { 0, 0 };
                if (rt_dtx_open_tail(SNO_DTX_REC(CALL_ARGS[0]), rq) && rq[0]) return (rt_call_next_t){ rq[0], rq[1] };
            }
            { extern rt_call_next_t rt_call_open_found(const char *, int, int *); int reg = 0; rt_call_next_t n = rt_call_open_found(pn, na, &reg); if (!(reg && n.fn)) return none; return n; }
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_apply_stno_land(const long *sv) {
    if (g_runtime.abi.stno != sv[0] || g_runtime.abi.line != sv[1]) {
        g_runtime.abi.lastno = g_runtime.abi.stno;
        g_runtime.abi.lastline = g_runtime.abi.line;
        g_runtime.abi.stno = sv[0];
        g_runtime.abi.line = sv[1];
    }
}
DESCR_t rt_apply_land_γ(DESCR_t frame0, long word, const long *sv) { extern DESCR_t rt_call_land_γ(DESCR_t, long); rt_apply_stno_land(sv); return rt_call_land_γ(frame0, word); }
void rt_fire_buildplan_tweak_rq(const char *cname, DESCR_t self, long *rq) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    extern int dat_type_gen(void);
    if (!cname || !*cname) return;
    {
        long gen = (long)g_runtime.rt.rt_gen_proc_count + (long)g_stage2.proc_count * 65536L + (long)dat_type_gen() * 4294967296L + g_runtime.bn.tweak_gen_bump * 1099511627776L;
        unsigned h = (unsigned)(((uintptr_t)cname >> 3) & (RT_TWEAK_IC_N - 1));
        if (g_runtime.bn.tweak_none[h].cname == cname && g_runtime.bn.tweak_none[h].gen == gen) return;
        const char *chain[bnd_dat_need(cname, 0)];
        int n = dat_mro(cname, chain, (int)(sizeof chain / sizeof chain[0]));
        int any = 0;
        if (n == 0) { chain[0] = cname; n = 1; }
        for (int i = n - 1; i >= 0; i--) {
            char proc[fmt_len("%s__TWEAK", chain[i])];
            snprintf(proc, sizeof proc, "%s__TWEAK", chain[i]);
            if (!meth_is_user_proc(proc)) continue;
            any = 1;
            int pi;
            for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
            if ((pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) && rq && !rq[0]) {
                rq[0] = 2;
                rq[1] = (long)(uintptr_t)chain[i];
            } else if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
                extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
                rt_c2bb_bomb("TWEAK from C (a Raku class the lowerer's .new helper sub did not see, or a SNOBOL4 DATA type's <type>__TWEAK hook: the box calls TWEAK, C never does)", proc);
            } else {
                extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na);
                DESCR_t a0 = self;
                ir_call_proc(pi, &a0, 1);
            }
        }
        if (!any) { g_runtime.bn.tweak_none[h].cname = cname; g_runtime.bn.tweak_none[h].gen = gen; }
    }
}
static void *pl_var_cell_ptr(DESCR_t v) { extern DESCR_t rt_pl_deref_val(DESCR_t); DESCR_t d = rt_pl_deref_val(v); return (d.v == (DTYPE_t)DT_PLVAR) ? d.p : (void *)0; }
static void pl_count_var_occ(DESCR_t t, void *target, int *cnt) {
    extern DESCR_t rt_pl_deref_val(DESCR_t);
    DESCR_t d = rt_pl_deref_val(t);
    if (d.v == (DTYPE_t)DT_PLVAR) { if (d.p == target) (*cnt)++; return; }
    if (d.v == (DTYPE_t)DT_PLREF) { int ar = plc_fid_arity(d.slen); DESCR_t *kids = (DESCR_t *)d.p; for (int i = 0; i < ar; i++) pl_count_var_occ(kids[i], target, cnt); }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t pl_sink_build(int kind, const char *s, size_t n) {
    if (kind == 1 || kind == 2) return pl_mk_atom_dup(s, n);
    DESCR_t *elems = (DESCR_t *)rt_ws_alloc_descr((size_t)(n > 0 ? n : 1));
    for (size_t i = 0; i < n; i++) {
        if (kind == 3) {
            elems[i].v = (DTYPE_t)DT_I;
            elems[i].slen = 0;
            elems[i].i = (unsigned char)s[i];
        } else {
            char *o = (char *)rt_wsb_alloc(2);
            o[0] = s[i];
            o[1] = 0;
            elems[i] = pl_mk_atom_dup(o, 1);
        }
    }
    return pl_list_from_arr(elems, (int)n);
}
DESCR_t dop_nl(DESCR_t *args, int nargs) {
    extern FILE *fh_cur_out_fp(void);
    (void)args;
    (void)nargs;
    fputc('\n', fh_cur_out_fp());
    { DESCR_t r = {0}; r.v = (DTYPE_t)DT_I; r.slen = 0; r.i = 1; return r; }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int junction_collapse(DESCR_t scalar, DESCR_t jct, int op, int numeric) {
    if (!rk_is_jct(jct)) return 0;
    const char *fl = ((ARBLK_t *) jct.arr)->proto;
    rk_av_t a = rk_av(jct);
    double sx = numeric ? to_real(scalar) : 0.0;
    const char *sstr = numeric ? NULL : VARVAL_fn(scalar);
    if (!sstr) sstr = "";
    int total = 0, hits = 0;
    for (int i = 0; i < a.n; i++) {
        DESCR_t e = a.el[i];
        int hit;
        if (rk_is_jct(e)) hit = junction_collapse(scalar, e, op, numeric);
        else {
            const char *t = rk_cstr(e);
            size_t cl = strlen(t);
            char mb[cl + 1];
            memcpy(mb, t, cl);
            mb[cl] = '\0';
            hit = numeric ? jct_one_cmp_num(sx, strtod(mb, NULL), op) : jct_one_cmp_str(sstr, mb, op);
        }
        total++;
        if (hit) hits++;
    }
    if (fl == rk_proto_jct_any) return hits >= 1;
    if (fl == rk_proto_jct_all) return total > 0 && hits == total;
    if (fl == rk_proto_jct_one) return hits == 1;
    return hits == 0;
}
DESCR_t rt_call_arr_strict(const char *fn, DESCR_t *args, int nargs) { return RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, -1, 1, 0, (long *)0)); }
static long rt_ctor_gen(void) {
    extern int dat_type_gen(void);
    extern unsigned rt_dtax_gen;
    return (long)g_runtime.rt.rt_gen_proc_count + (long)g_stage2.proc_count * 65536L + (long)dat_type_gen() * 4294967296L + (long)rt_dtax_gen * 1099511627776L;
}
void rt_ctor_ic_note(const char *fn, void *dt) {
    const char *site = g_runtime.bn.ctor_ic_site;
    if (!site || (site != fn && strcmp(site, fn) != 0)) return;
    {
        unsigned h = rt_ctor_ic_h(site);
        if (g_runtime.bn.ctor_ic[h].fn != site && g_runtime.bn.ctor_ic[h].fn && (!g_runtime.bn.ctor_ic[h ^ 1u].fn || g_runtime.bn.ctor_ic[h ^ 1u].fn == site)) h ^= 1u;
        g_runtime.bn.ctor_ic[h].fn = site;
        g_runtime.bn.ctor_ic[h].dt = dt;
        g_runtime.bn.ctor_ic[h].gen = rt_ctor_gen();
    }
}
DESCR_t rt_call_arr_bl_sn4_rq(const char *fn, DESCR_t *args, int nargs, int bidlen, long *rq) {
    if (g_runtime.abi.error == 0 && g_runtime.abi.kw_errlimit == 0 && fn) {
        int h = rt_ctor_ic_find(fn, rt_ctor_gen());
        if (h >= 0) {
            extern DESCR_t dat_construct(DatType *, DESCR_t *, int);
            DatType *_udt = (DatType *)g_runtime.bn.ctor_ic[h].dt;
            if (nargs <= _udt->nfields) return dat_construct_rq(_udt, args, nargs, rq);
        }
    }
    {
        const char *outer = g_runtime.bn.ctor_ic_site;
        DESCR_t r;
        g_runtime.bn.ctor_ic_site = fn;
        r = RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, bidlen, 0, 1, rq));
        g_runtime.bn.ctor_ic_site = outer;
        return r;
    }
}
DESCR_t rt_call_arr_bl_strict(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    extern int rt_g_want_name;
    extern DESCR_t dat_construct_byref(void *, DESCR_t *, int);
    extern int dat_nfields_byref(void *);
    DESCR_t out;
    if (bidlen >= 0 && icn_bi_direct(bidlen, args, nargs, &out)) return out;
    if (fn && g_runtime.abi.error == 0 && g_runtime.abi.kw_errlimit == 0 && !rt_g_want_name) {
        int h = rt_ctor_ic_find(fn, rt_ctor_gen());
        if (h >= 0 && nargs <= dat_nfields_byref(g_runtime.bn.ctor_ic[h].dt)) return dat_construct_byref(g_runtime.bn.ctor_ic[h].dt, args, nargs);
    }
    {
        const char *outer = g_runtime.bn.ctor_ic_site;
        DESCR_t r;
        g_runtime.bn.ctor_ic_site = fn;
        r = RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, bidlen, 1, 0, (long *)0));
        g_runtime.bn.ctor_ic_site = outer;
        return r;
    }
}
int rt_field_index_cached(const char *fn, const DATBLK_t *idb) { return rt_field_index_cached_i(fn, idb); }
static void rt_crec_fill(sno_callee_rec_t *r) {
    extern int dat_type_count(void);
    extern int dat_type_nfields(int);
    extern const char *dat_type_field(int, int);
    extern unsigned rt_dtax_gen;
    r->types.len = 0;
    int fi = -1, same = 1;
    for (int c = 0; c < dat_type_count(); c++) for (int f = 0; f < dat_type_nfields(c); f++) {
        const char *fn2 = dat_type_field(c, f);
        if (fn2 && r->name && !strcmp(fn2, r->name)) { if (fi < 0) fi = f; else if (fi != f) same = 0; CV_PUSH(r->types, int32_t) = (int32_t)c; }
    }
    r->fi = same ? fi : -1;
    r->gen1 = rt_dtax_gen + 1u;
}
DESCR_t rt_call_fld_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r) {
    extern unsigned rt_dtax_gen;
    extern const void *dat_type_blk(int);
    if (r->gen1 != rt_dtax_gen + 1u) rt_crec_fill(r);
    if (nargs == 1 && r->fi >= 0 && IS_DATA_INST_fn(args[0]) && args[0].u && args[0].u->type && args[0].u->fields) {
        const void *tb = (const void *)args[0].u->type;
        const int32_t *ti = (const int32_t *)r->types.p;
        for (uint32_t k = 0; k < r->types.len; k++) if (dat_type_blk(ti[k]) == tb) return args[0].u->fields[r->fi];
    }
    return rt_call_name_sn4(r->name, args, nargs, -1);
}
rt_call_next_t rt_call_arr_bl_try(const char *fn, DESCR_t *args, long nb, DESCR_t *out) {
    long rq[2] = { 0, 0 };
    int nargs = (int)(uint32_t)nb, bidlen = (int)(nb >> 32);
    { DESCR_t v = RT_GC_CALLBACK(rt_call_arr_bl_s(fn, args, nargs, bidlen, 0, 0, rq)); if (!rq[0] || rq[0] == 2) *out = v; return (rt_call_next_t){ rq[0], rq[1] }; }
}
rt_call_next_t rt_call_arr_bl_sn4_try(const char *fn, DESCR_t *args, long nb, DESCR_t *out) {
    long rq[2] = { 0, RT_RQ_EVAL_ARM };
    int nargs = (int)(uint32_t)nb, bidlen = (int)(nb >> 32);
    { DESCR_t v = RT_GC_CALLBACK(rt_call_arr_bl_sn4_rq(fn, args, nargs, bidlen, rq)); if (!rq[0] || rq[0] == 2 || rq[0] == RT_TRY_EVAL_OPEN) *out = v; return (rt_call_next_t){ rq[0], rq[1] }; }
}
rt_call_next_t rt_call_callee_try_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r, DESCR_t *out) {
    extern DESCR_t dat_construct(DatType *, DESCR_t *, int);
    long rq[2] = { 0, RT_RQ_EVAL_ARM };
    if (r->ctor && r->cgen == rt_ctor_gen() && nargs <= ((DatType *)r->ctor)->nfields && !(nargs == 1 && IS_DATA_INST_fn(args[0]))) {
        *out = dat_construct_rq((DatType *)r->ctor, args, nargs, rq);
        return (rt_call_next_t){ rq[0], rq[1] };
    }
    {
        DESCR_t v = rt_call_name_sn4_rq(r->name, args, nargs, -1, rq);
        int h = rt_ctor_ic_find(r->name, rt_ctor_gen());
        if (h >= 0) { r->ctor = g_runtime.bn.ctor_ic[h].dt; r->cgen = g_runtime.bn.ctor_ic[h].gen; }
        if (!rq[0] || rq[0] == 2 || rq[0] == RT_TRY_EVAL_OPEN) *out = v;
        return (rt_call_next_t){ rq[0], rq[1] };
    }
}
DESCR_t rt_call_callee_sn4(DESCR_t *args, int nargs, sno_callee_rec_t *r) {
    extern DESCR_t dat_construct(DatType *, DESCR_t *, int);
    if (r->ctor && r->cgen == rt_ctor_gen() && nargs <= ((DatType *)r->ctor)->nfields && !(nargs == 1 && IS_DATA_INST_fn(args[0]))) return dat_construct((DatType *)r->ctor, args, nargs);
    {
        DESCR_t v = rt_call_name_sn4(r->name, args, nargs, -1);
        int h = rt_ctor_ic_find(r->name, rt_ctor_gen());
        if (h >= 0) { r->ctor = g_runtime.bn.ctor_ic[h].dt; r->cgen = g_runtime.bn.ctor_ic[h].gen; }
        return v;
    }
}
DESCR_t rt_call_name_sn4_rq(const char *fn, DESCR_t *args, int nargs, int bidlen, long *rq) {
    if (nargs == 1 && fn && IS_DATA_INST_fn(args[0]) && args[0].u && args[0].u->type) {
        DATBLK_t *idb = args[0].u->type;
        int fi = rt_field_index_cached_i(fn, idb);
        if (fi >= 0 && args[0].u->fields) {
            extern int rt_g_want_name;
            extern DESCR_t rt_field_var(const char *field, DESCR_t obj);
            if (rt_g_want_name) { rt_g_want_name = 0; return rt_field_var(fn, args[0]); }
            return args[0].u->fields[fi];
        }
    }
    if (fn) {
        int h = rt_ctor_ic_find(fn, rt_ctor_gen());
        if (h >= 0) {
            extern DESCR_t dat_construct(DatType *, DESCR_t *, int);
            DatType *_udt = (DatType *)g_runtime.bn.ctor_ic[h].dt;
            if (nargs <= _udt->nfields) return dat_construct_rq(_udt, args, nargs, rq);
        }
    }
    {
        const char *outer = g_runtime.bn.ctor_ic_site;
        DESCR_t r;
        g_runtime.bn.ctor_ic_site = fn;
        r = RT_GC_CALLBACK(rt_call_arr_bl_sn4_rq(fn, args, nargs, bidlen, rq));
        g_runtime.bn.ctor_ic_site = outer;
        return r;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_arr_bl_s(const char *fn, DESCR_t *args, int nargs, int bidlen, int strict, int sn4, long *rq) {
    extern int g_core_errjmp_n;
#if RT_DIAG
    {
        static long _rspc = -1;
        if (_rspc == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _rspc = (ev && *ev && *ev != '0') ? 0 : -2; }
        if (_rspc >= 0) {
            void *rsp_now;
            __asm__ volatile ("mov %%rsp, %0" : "=r"(rsp_now));
            _rspc++;
            fprintf(stderr, "[RSP] %ld fn='%s' rsp=%p\n", _rspc, fn ? fn : "(null)", rsp_now);
            fflush(stderr);
        }
    }
#endif
    core_errjmp_t ej;
    int my = g_core_errjmp_n;
    void * volatile bimark = core_icn_bi_mark();
    rk_cbh_t * volatile cbh0 = g_rk_cbh_cur;
    if (setjmp(ej.jb)) { core_errjmp_pop(&ej, my); core_icn_bi_reset(bimark); g_rk_cbh_cur = cbh0; return FAILDESCR; }
    core_errjmp_push(&ej);
    g_core_errjmp_n = my + 1;
    DESCR_t r = RT_GC_CALLBACK(rt_call_arr_impl(fn, args, nargs, bidlen, strict, sn4, rq));
    core_errjmp_pop(&ej, my);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_name_is_identifier(const char *fn) { return fn && fn[0] && ((fn[0] >= 'a' && fn[0] <= 'z') || (fn[0] >= 'A' && fn[0] <= 'Z') || fn[0] == '_' || fn[0] == '&'); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int sn4_call_in_scope(const char *fn) {
    extern int rt_proc_is_registered(const char *);
    extern int FNCEX_fn(const char *);
    extern int rt_dat_field_of_any_live(const char *);
    extern int dat_type_count(void);
    extern int dat_type_live(int);
    extern const char *dat_type_name(int);
    if (!fn || !fn[0]) return 0;
    if (!sn4_name_is_identifier(fn)) return 1;
    if (strchr(fn, '$')) return 1;
    if (rt_proc_is_registered(fn) || FNCEX_fn(fn)) return 1;
    if (sn4_is_system_fn(fn)) return 1;
    if (rt_dat_field_of_any_live(fn)) return 1;
    for (int c = 0; c < dat_type_count(); c++) { if (!dat_type_live(c)) continue; const char *tn = dat_type_name(c); if (tn && !strcmp(tn, fn)) return 1; }
    return 0;
}
static DESCR_t rt_call_arr_impl(const char *fn, DESCR_t *args, int nargs, int bidlen, int strict, int sn4, long *rq) {
    extern DESCR_t APPLY_fn_rq(const char *, DESCR_t *, int, long *);
    DESCR_t out = FAILDESCR;
#if RT_DIAG
    {
        static long _cac = -1;
        if (_cac == -1) { const char *ev = getenv("SCRIP_CALLARR_TRACE"); _cac = (ev && *ev && *ev != '0') ? 0 : -2; }
        if (_cac >= 0) { extern int g_core_errjmp_n; _cac++; fprintf(stderr, "[CAC] %ld fn='%s' nargs=%d errjmp_n=%d\n", _cac, fn ? fn : "(null)", nargs, g_core_errjmp_n); fflush(stderr); }
    }
#endif
    if (!fn) return out;
    if (fn[0] == 'S' && fn[1] == 'N' && !strcmp(fn, "SNO$NOFAIL")) { extern void rt_nofail_abort(void); rt_nofail_abort(); return out; }
    if (fn[0] == '$' && fn[1]) { if (script_try_call_builtin_by_name(fn, args, nargs, &out)) return out; out = FAILDESCR; }
    if (fn[0] && !sn4_name_is_identifier(fn)) {
        extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
        extern DESCR_t rt_num_arith_strict(DESCR_t, DESCR_t, int);
        DESCR_t (*na)(DESCR_t, DESCR_t, int) = strict ? rt_num_arith_strict : rt_num_arith;
        if (nargs == 1) {
            extern DESCR_t rt_random_var(DESCR_t);
            extern DESCR_t rt_random_var_strict(DESCR_t);
            extern DESCR_t rt_deref(DESCR_t);
            DESCR_t a = args[0];
            if (!strcmp(fn, "-")) return na(INTVAL(0), a, BINOP_SUB);
            if (!strcmp(fn, "+")) return na(INTVAL(0), a, BINOP_ADD);
            if (!strcmp(fn, "*")) { extern DESCR_t rt_call_arr(const char *, DESCR_t *, int); DESCR_t _a = a; return try_call_builtin_by_name("*", &_a, 1, &out) ? out : FAILDESCR; }
            if (!strcmp(fn, "\\")) return (a.v == DT_SNUL || a.v == 0) ? FAILDESCR : a;
            if (!strcmp(fn, "?")) return rt_deref((strict ? rt_random_var_strict : rt_random_var)(a));
            if (!strcmp(fn, "/") || !strcmp(fn, "%") || !strcmp(fn, "#") || !strcmp(fn, "|") || !strcmp(fn, "!") || !strcmp(fn, "=") || !strcmp(fn, "^")) {
                extern int core_call_registered_fn(const char *, DESCR_t *, int, DESCR_t *);
                extern int FNCEX_fn(const char *);
                extern const char *sn4_unary_op_key(const char *);
                const char *uk = sn4_unary_op_key(fn);
                if (core_call_registered_fn(uk, args, nargs, &out)) return out;
                if (FNCEX_fn(uk)) return RT_GC_CALLBACK(APPLY_fn_rq(uk, args, nargs, rq));
                core_runtime_error(29, "undefined operator referenced");
                return FAILDESCR;
            }
        }
        DESCR_t a = (nargs > 0) ? args[0] : NULVCL, b = (nargs > 1) ? args[1] : NULVCL;
        if (nargs == 3 && !strcmp(fn, "[:]")) {
            extern DESCR_t rt_section_var(DESCR_t, DESCR_t, DESCR_t);
            extern DESCR_t rt_section_var_strict(DESCR_t, DESCR_t, DESCR_t);
            extern DESCR_t rt_deref(DESCR_t);
            extern DESCR_t rt_var_ref_cell(DESCR_t *);
            extern void *rt_agg_alloc(int, size_t);
            DESCR_t sb = a;
            if (!IS_VARREF_fn(sb)) { DESCR_t *cell = (DESCR_t *)rt_agg_alloc(0, sizeof(DESCR_t)); if (!cell) return FAILDESCR; *cell = a; sb = rt_var_ref_cell(cell); }
            DESCR_t v = (strict ? rt_section_var_strict : rt_section_var)(sb, b, args[2]);
            if (IS_FAIL_fn(v)) return FAILDESCR;
            return rt_deref(v);
        }
        if (!strcmp(fn, "[]")) {
            extern DESCR_t c_rt_subscript_var(DESCR_t, DESCR_t);
            extern DESCR_t c_rt_subscript_var_strict(DESCR_t, DESCR_t);
            extern DESCR_t rt_deref(DESCR_t);
            DESCR_t v = (strict ? c_rt_subscript_var_strict : c_rt_subscript_var)(a, b);
            if (IS_FAIL_fn(v)) return FAILDESCR;
            return rt_deref(v);
        }
        if (!strcmp(fn, "++")) return na(a, b, BINOP_CUNION);
        if (!strcmp(fn, "--")) return na(a, b, BINOP_CDIFF);
        if (!strcmp(fn, "**")) return na(a, b, BINOP_CINTER);
        if (!strcmp(fn, "+")) return na(a, b, BINOP_ADD);
        if (!strcmp(fn, "-")) return na(a, b, BINOP_SUB);
        if (!strcmp(fn, "*")) return na(a, b, BINOP_MUL);
        if (!strcmp(fn, "/")) return na(a, b, BINOP_DIV);
        if (!strcmp(fn, "%")) {
            extern int FNCEX_fn(const char *);
            if (FNCEX_fn(fn)) return RT_GC_CALLBACK(APPLY_fn_rq(fn, args, nargs, rq));
            if (sn4) { core_runtime_error(29, "undefined operator referenced"); return FAILDESCR; }
            return na(a, b, BINOP_MOD);
        }
        if (!strcmp(fn, "^")) return na(a, b, BINOP_POW);
        if (!strcmp(fn, "|||")) { extern DESCR_t rt_icn_lconcat_d(DESCR_t, DESCR_t); return rt_icn_lconcat_d(a, b); }
        if (!strcmp(fn, "||")) {
            const char *x = VARVAL_fn(a), *y = VARVAL_fn(b);
            if (!x) x = "";
            if (!y) y = "";
            size_t lx = strlen(x), ly = strlen(y);
            char *o = rt_str_alloc((int)(lx + ly));
            memcpy(o, x, lx);
            memcpy(o + lx, y, ly);
            o[lx + ly] = 0;
            return STRVAL(o);
        }
        {
            DESCR_t c_rt_str_coerce(DESCR_t);
            void c_rt_relop_val_coerce(DESCR_t, DESCR_t, DESCR_t *);
            int oc = -1;
            if (!strcmp(fn, "=")) oc = BINOP_EQ;
            else if (!strcmp(fn, "~=")) oc = BINOP_NE;
            else if (!strcmp(fn, "<")) oc = BINOP_LT;
            else if (!strcmp(fn, "<=")) oc = BINOP_LE;
            else if (!strcmp(fn, ">")) oc = BINOP_GT;
            else if (!strcmp(fn, ">=")) oc = BINOP_GE;
            else if (!strcmp(fn, "==")) oc = BINOP_SEQ;
            else if (!strcmp(fn, "~==")) oc = BINOP_SNE;
            else if (!strcmp(fn, "<<")) oc = BINOP_SLT;
            else if (!strcmp(fn, "<<=")) oc = BINOP_SLE;
            else if (!strcmp(fn, ">>")) oc = BINOP_SGT;
            else if (!strcmp(fn, ">>=")) oc = BINOP_SGE;
            else if (!strcmp(fn, "===")) oc = BINOP_EQV;
            else if (!strcmp(fn, "~===")) oc = BINOP_NEQV;
            if (oc >= 0) {
                if (!rtx_try_rt_jct_relop(a, b, oc)) return FAILDESCR;
                if (oc >= BINOP_SLT && oc <= BINOP_SNE) return rtx_try_rt_str_coerce(b);
                if (oc == BINOP_EQV || oc == BINOP_NEQV) return b;
                DESCR_t _rv;
                c_rt_relop_val_coerce(a, b, &_rv);
                return _rv;
            }
        }
    }
    {
        int sysfn = (bidlen >= 0) ? ((bidlen & BID_BAKE_SYSFN) ? 1 : 0) : (sn4 ? sn4_is_system_fn(fn) : 0);
        if (!strict && nargs == 1 && IS_DATA_INST_fn(args[0]) && args[0].u && args[0].u->type) {
            DATBLK_t *idb = args[0].u->type;
            const char _f0 = fn[0];
            extern DESCR_t c_dat_field_get(const char *field, DESCR_t obj);
            for (int fi = 0; fi < idb->nfields; fi++) if (idb->fields[fi] && idb->fields[fi][0] == _f0 && !strcmp(idb->fields[fi], fn)) return rtx_try_dat_field_get(fn, args[0]);
        }
        { extern int rt_proc_is_registered(const char *); if (sn4 && !sysfn && rt_proc_is_registered(fn)) { out = RT_GC_CALLBACK(APPLY_fn_rq(fn, args, nargs, rq)); return out; } }
        if (sn4 && !sysfn && !sn4_call_in_scope(fn)) {
            if (sn4_name_is_identifier(fn) && !(fn[0] == '&' && !fn[1])) core_runtime_error(22, "undefined function called");
            else core_runtime_error(29, "undefined operator referenced");
            return FAILDESCR;
        }
        {
            extern int FNCEX_fn(const char *);
            extern int rt_dat_field_of_any(const char *);
            if (sn4 && !sysfn && FNCEX_fn(fn) && icn_builtin_is_known(fn) && !rt_dat_field_of_any(fn) && !dat_find_type(fn)) return RT_GC_CALLBACK(APPLY_fn(fn, args, nargs));
        }
    }
    {
        icn_bi_rec_t bi;
        core_icn_bi_push(&bi, fn, args, nargs);
        if (core_icn_builtin_argcheck(fn, args, nargs, strict)) { core_icn_bi_pop(&bi); return FAILDESCR; }
        long trq[2] = { 0, rq ? rq[1] : 0 };
        int hit = try_call_builtin_by_name_bl_s_rq(fn, args, nargs, &out, bidlen, strict, sn4 ? trq : rq);
        core_icn_bi_pop(&bi);
        if (sn4 && rq && (trq[0] == 2 || trq[0] == RT_TRY_EVAL_OPEN) && !rq[0]) { rq[0] = trq[0]; rq[1] = trq[1]; }
        if (hit) return out;
    }
    if (sn4) {
        extern int core_fn_arity_norm(const char *, int);
        int na = core_fn_arity_norm(fn, nargs);
        if (na != nargs) {
            DESCR_t pad[na > 0 ? na : 1];
            for (int k = 0; k < na; k++) pad[k] = k < nargs ? args[k] : NULVCL;
            return RT_GC_CALLBACK(rt_call_arr_impl(fn, pad, na, bidlen, strict, sn4, rq));
        }
    }
    if (strict && !rt_proc_name_exists(fn) && !icn_builtin_is_known(fn) && icn_builtin_arity(fn) == ICN_ARITY_UNKNOWN) {
        DESCR_t cal = NV_GET_fn(fn);
        if (!IS_PROCVAL_fn(cal)) { icn_bi_rec_t cr; core_icn_bi_push(&cr, (const char *)0, args, nargs); cr.callee = cal; core_icn_error(106, cal); core_icn_bi_pop(&cr); return FAILDESCR; }
    }
    out = RT_GC_CALLBACK(APPLY_fn_rq(fn, args, nargs, rq));
    return out;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int relop_num_coerce(DESCR_t v, DESCR_t *out, int blank0) {
    if (v.v == DT_BIG) { *out = v; return 1; }
    if (v.v == DT_ORDER) { *out = INTVAL((int64_t)v.i); return 1; }
    if (IS_INT_fn(v) || IS_REAL_fn(v)) { *out = v; return 1; }
    const char *s = IS_STR_fn(v) ? v.s : IS_CSET_fn(v) ? v.s : (const char *)0;
    if (!s && blank0 && IS_STR_fn(v)) { *out = INTVAL(0); return 1; }
    if (!s) return 0;
    if (v.v == DT_S && s[0] == RK_TY) { *out = INTVAL(0); return 1; }
    char nb[128];
    if (IS_STR_fn(v) && v.slen != 0 && v.slen != 0xFFFFFFFFu) { char *cp = (v.slen < sizeof nb) ? nb : (char *)rt_wsb_alloc((size_t)v.slen + 1); memcpy(cp, s, v.slen); cp[v.slen] = '\0'; s = cp; }
    const char *t = s;
    while (*t == ' ' || (blank0 && isspace((unsigned char)*t))) t++;
    if (!*t && blank0) { *out = INTVAL(0); return 1; }
    if (!*t) return 0;
    {
        const char *q = t + (*t == '-' || *t == '+');
        long long acc = 0;
        int nd = 0;
        while (nd < 19 && q[nd] >= '0' && q[nd] <= '9') { acc = acc * 10 + (q[nd] - '0'); nd++; }
        const char *qe = q + nd;
        if (nd > 0 && nd <= 18 && (*qe == '\0' || *qe == ' ' || (blank0 && isspace((unsigned char)*qe)))) {
            while (*qe == ' ' || (blank0 && isspace((unsigned char)*qe))) qe++;
            if (*qe != '\0') return 0;
            *out = INTVAL((int64_t)(*t == '-' ? -acc : acc));
            return 1;
        }
    }
    char *endi = 0, *endd = 0;
    long long iv = strtoll(t, &endi, 10);
    double dv = strtod(t, &endd);
    const char *e = (endd > endi) ? endd : endi;
    if (e == t) return 0;
    while (*e == ' ' || (blank0 && isspace((unsigned char)*e))) e++;
    if (*e != '\0') return 0;
    *out = (endd > endi) ? REALVAL(dv) : INTVAL((int64_t)iv);
    return 1;
}
DESCR_t c_rt_str_coerce(DESCR_t d) {
    if (!IS_CSET_fn(d)) {
        if (d.v == DT_I || d.v == DT_R || d.v == DT_BIG) { extern DESCR_t descr_to_str_fracdigit(DESCR_t); DESCR_t sv = descr_to_str_fracdigit(d); if (!IS_FAIL_fn(sv)) return sv; return d; }
        return d;
    }
    const char *cp;
    int cl;
    if (!cset_resolve(d, &cp, &cl) || cl < 0) return d;
    char *b = rt_wsb_alloc((size_t)cl + 1);
    memcpy(b, cp, (size_t)cl);
    b[cl] = 0;
    for (int i = 1; i < cl; i++) { char t = b[i]; int j = i - 1; while (j >= 0 && (unsigned char)b[j] > (unsigned char)t) { b[j+1] = b[j]; j--; } b[j+1] = t; }
    return BSTRVAL(b, cl);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static const char * lexical_text(DESCR_t d, int num_text) {
    extern DESCR_t descr_to_str_fracdigit(DESCR_t);
    if (!IS_REAL_fn(d)) return VARVAL_fn(d);
    return num_text ? rk_cstr(d) : descr_to_str_fracdigit(d).s;
}
int c_rt_jct_relop(DESCR_t lhs, DESCR_t rhs, int op) {
    int fl = op & (BINOP_F_NUMIFY_BLANK | BINOP_F_NUM_TEXT);
    op &= ~fl;
    if (op == BINOP_EQV || op == BINOP_NEQV) return rt_jct_relop_impl(lhs, rhs, op, 0, 0);
    extern int g_core_errjmp_n;
    core_errjmp_t ej;
    int my = g_core_errjmp_n;
    rk_cbh_t * volatile cbh0 = g_rk_cbh_cur;
    if (setjmp(ej.jb)) { core_errjmp_pop(&ej, my); g_rk_cbh_cur = cbh0; return 0; }
    core_errjmp_push(&ej);
    g_core_errjmp_n = my + 1;
    int r = rt_jct_relop_impl(lhs, rhs, op, fl & BINOP_F_NUMIFY_BLANK, fl & BINOP_F_NUM_TEXT);
    core_errjmp_pop(&ej, my);
    return r;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rt_jct_relop_impl(DESCR_t lhs, DESCR_t rhs, int op, int blank0, int num_text) {
    if (IS_FAIL_fn(lhs) || IS_FAIL_fn(rhs)) return 0;
    if (lhs.v == DT_XR || rhs.v == DT_XR) { extern int pas_xr_binop(int, DESCR_t, DESCR_t, DESCR_t *); DESCR_t _xo; if (pas_xr_binop(op, lhs, rhs, &_xo)) return (int)_xo.i; }
    if (op == BINOP_EQV || op == BINOP_NEQV) return (op == BINOP_EQV) ? bn_identical(lhs, rhs) : !bn_identical(lhs, rhs);
    if (rk_match_is(lhs)) lhs = FIELD_GET_fn(lhs, "text");
    if (rk_match_is(rhs)) rhs = FIELD_GET_fn(rhs, "text");
    int lj = junction_is(lhs), rj = junction_is(rhs);
    int num_rel = (op == BINOP_EQ || op == BINOP_NE || op == BINOP_LT || op == BINOP_LE || op == BINOP_GT || op == BINOP_GE);
    int str_rel = (op == BINOP_SEQ || op == BINOP_SNE || op == BINOP_SLT || op == BINOP_SLE || op == BINOP_SGT || op == BINOP_SGE);
    if ((lj || rj) && (num_rel || str_rel)) {
        DESCR_t jct = lj ? lhs : rhs;
        DESCR_t scalar = lj ? rhs : lhs;
        int tt_op = (op == BINOP_EQ || op == BINOP_SEQ) ? TT_EQ : (op == BINOP_NE || op == BINOP_SNE) ? TT_NE : (op == BINOP_LT || op == BINOP_SLT) ? TT_LT : (op == BINOP_LE || op == BINOP_SLE) ?
            TT_LE : (op == BINOP_GT || op == BINOP_SGT) ? TT_GT : TT_GE;
        int numeric = str_rel ? 0 : (IS_INT_fn(scalar) || IS_REAL_fn(scalar));
        if (lj) tt_op = junction_mirror_op(tt_op);
        return junction_collapse(scalar, jct, tt_op, numeric) ? 1 : 0;
    }
    if (num_rel && (lhs.v == DT_DATA || rhs.v == DT_DATA)) {
        extern int rk_rat_cmp(DESCR_t, DESCR_t, int *);
        int rc;
        if (rk_rat_cmp(lhs, rhs, &rc)) {
            switch (op) {
                case BINOP_EQ:
                return rc == 0;
                case BINOP_NE:
                return rc != 0;
                case BINOP_LT:
                return rc < 0;
                case BINOP_LE:
                return rc <= 0;
                case BINOP_GT:
                return rc > 0;
                case BINOP_GE:
                return rc >= 0;
            }
            return 0;
        }
    }
    if (num_rel) {
        DESCR_t L, R;
        int _relop_lok = relop_num_coerce(lhs, &L, blank0) || (!blank0 && relop_radix_coerce(lhs, &L));
        if (_relop_lok && (relop_num_coerce(rhs, &R, blank0) || (!blank0 && relop_radix_coerce(rhs, &R)))) {
            if (IS_REAL_fn(L) || IS_REAL_fn(R)) {
                double a = to_real(L), b = to_real(R);
                switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b; case BINOP_LE: return a<=b; case BINOP_GT: return a>b; case BINOP_GE: return a>=b; }
                return 0;
            }
            if (L.v == DT_BIG || R.v == DT_BIG) {
                extern int rt_big_cmp(DESCR_t, DESCR_t);
                int c = rt_big_cmp(L, R);
                switch (op) { case BINOP_EQ: return c==0; case BINOP_NE: return c!=0; case BINOP_LT: return c<0; case BINOP_LE: return c<=0; case BINOP_GT: return c>0; case BINOP_GE: return c>=0; }
                return 0;
            }
            int64_t a = L.i, b = R.i;
            switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b; case BINOP_LE: return a<=b; case BINOP_GT: return a>b; case BINOP_GE: return a>=b; }
            return 0;
        }
        if (blank0) {
            extern int g_script_try_depth;
            extern const char *core_icn_errmsg(int code);
            extern void rt_script_die_surface(const char *msg);
            if (g_script_try_depth > 0) { rt_script_die_surface(core_icn_errmsg(102)); return -1; }
        }
        core_icn_op_ctx(core_icn_binop_sym(op), 2, _relop_lok ? L : lhs, rhs);
        core_icn_error(102, _relop_lok ? rhs : lhs);
        core_icn_op_ctx_clear();
        return 0;
    }
    if (str_rel && !(core_icn_str_ok(lhs) && core_icn_str_ok(rhs))) {
        DESCR_t sl = lhs;
        if (core_icn_str_ok(lhs) && (lhs.v == DT_I || lhs.v == DT_R || lhs.v == DT_BIG)) { extern DESCR_t c_rt_str_coerce(DESCR_t); sl = rtx_try_rt_str_coerce(lhs); }
        core_icn_op_ctx(core_icn_binop_sym(op), 2, sl, rhs);
        core_icn_error(103, core_icn_str_ok(lhs) ? rhs : lhs);
        core_icn_op_ctx_clear();
        return 0;
    }
    if (num_rel && (IS_REAL_fn(lhs) || IS_REAL_fn(rhs)) && (IS_INT_fn(lhs) || IS_REAL_fn(lhs)) && (IS_INT_fn(rhs) || IS_REAL_fn(rhs))) {
        double a = to_real(lhs), b = to_real(rhs);
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b; case BINOP_LE: return a<=b; case BINOP_GT: return a>b; case BINOP_GE: return a>=b; }
        return 0;
    }
    if (num_rel && IS_INT_fn(lhs) && IS_INT_fn(rhs)) {
        int64_t a = lhs.i, b = rhs.i;
        switch (op) { case BINOP_EQ: return a==b; case BINOP_NE: return a!=b; case BINOP_LT: return a<b; case BINOP_LE: return a<=b; case BINOP_GT: return a>b; case BINOP_GE: return a>=b; }
        return 0;
    }
    {
        const char *a = lexical_text(lhs, num_text), *b = lexical_text(rhs, num_text);
        if (!a) a="";
        if (!b) b="";
        size_t la = IS_CSET_fn(lhs) ? descr_slen(lhs) : (lhs.v == DT_S && lhs.slen != 0xFFFFFFFFu) ? (size_t)lhs.slen : strlen(a);
        size_t lb = IS_CSET_fn(rhs) ? descr_slen(rhs) : (rhs.v == DT_S && rhs.slen != 0xFFFFFFFFu) ? (size_t)rhs.slen : strlen(b);
        size_t lm = la < lb ? la : lb;
        int c = lm ? memcmp(a, b, lm) : 0;
        if (c == 0) c = (la < lb) ? -1 : (la > lb) ? 1 : 0;
        switch (op) {
            case BINOP_EQ:
            case BINOP_SEQ:
            return c==0;
            case BINOP_NE:
            case BINOP_SNE:
            return c!=0;
            case BINOP_LT:
            case BINOP_SLT:
            return c<0;
            case BINOP_LE:
            case BINOP_SLE:
            return c<=0;
            case BINOP_GT:
            case BINOP_SGT:
            return c>0;
            case BINOP_GE:
            case BINOP_SGE:
            return c>=0;
        }
        return 0;
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bn_identical(DESCR_t a, DESCR_t b) {
    if (IS_PROCVAL_fn(a) && IS_PROCVAL_fn(b)) return rt_procval_same(a, b);
    if (a.v != b.v) return 0;
    if (a.slen == b.slen && a.i == b.i) return 1;
    if (a.v == DT_SNUL) return 1;
    if (a.v == DT_I) return a.i == b.i;
    if (a.v == DT_R) return a.r == b.r;
    if (a.v == DT_BIG) { extern int rt_big_eq(DESCR_t, DESCR_t); return rt_big_eq(a, b); }
    if (a.v == DT_XR) return 0;
    if (a.v != DT_S) return a.ptr == b.ptr;
    int acs = (a.slen == 0xFFFFFFFFu), bcs = (b.slen == 0xFFFFFFFFu);
    if (acs || bcs) { const char *ca, *cb; int la, lb; return acs && bcs && cset_resolve(a, &ca, &la) && cset_resolve(b, &cb, &lb) && la == lb && memcmp(ca, cb, (size_t)la) == 0; }
    const char *x = a.s ? a.s : "", *y = b.s ? b.s : "";
    long la = a.slen ? (long)a.slen : (long)strlen(x), lb = b.slen ? (long)b.slen : (long)strlen(y);
    return la == lb && memcmp(x, y, (size_t)la) == 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_dat_field_of_any(const char *name) {
    extern int dat_type_count(void);
    extern int dat_type_nfields(int);
    extern const char *dat_type_field(int, int);
    if (!name || !name[0]) return 0;
    for (int c = 0; c < dat_type_count(); c++) for (int f = 0; f < dat_type_nfields(c); f++) { const char *fn2 = dat_type_field(c, f); if (fn2 && !strcmp(fn2, name)) return 1; }
    return 0;
}
