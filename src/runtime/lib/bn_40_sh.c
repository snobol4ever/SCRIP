#include "runtime/lib/bn_head.h"
#include "ir/dtp.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/icn_builtins.h"
#include "runtime/core/nv.h"
#include "runtime/rt/rt_arena.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_ids.h"
#include "runtime/rt/rt_list_view.h"
#include <ctype.h>
#include <termios.h>
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_04_pl.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_39_sh.h"
#include "runtime/lib/bn_40_sh.h"
int core_icn_error(int code, DESCR_t val);
extern int rt_proc_is_registered(const char *name);
extern int rt_proc_nparams(const char *name);
extern DESCR_t rt_deref(DESCR_t d);
extern FILE *fh_get(int);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long rt_record_image_id(void *inst) { return inst ? ((DATINST_t *)inst)->id : 0; }
static int bn_mkpat(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    extern void *rt_proc_get_fn(const char *name);
    extern void *dtp_wrap_fn_sz(void *, int64_t, int32_t);
    extern long rt_fn_frame_bytes_known(void *);
    extern long rt_fn_zstatic_known(void *);
    extern void rt_patv_freeze(void *, const char *, long);
    (void)op;
    if (nargs < 1) return 0;
    {
        const char *nm = VARVAL_fn(args[0]);
        if (!nm) nm = "";
        {
            void *pf = rt_proc_get_fn(nm);
            if (!pf) { fprintf(stderr, "[SNO] SNO$MKPAT: compiled pattern blob '%s' not registered\n", nm); *out = FAILDESCR; return 1; }
            {
                DESCR_t pd = {0};
                pd.v = DT_P;
                pd.slen = 0;
                pd.p = dtp_wrap_fn_sz(pf, (int64_t)rt_fn_frame_bytes_known(pf), (int32_t)rt_fn_zstatic_known(pf));
                if (nargs >= 2) { long _n = 0; if (IS_INT_fn(args[1])) _n = (long)args[1].i; else { const char *cs = VARVAL_fn(args[1]); _n = cs ? atol(cs) : 0; } rt_patv_freeze(pd.p, nm, _n); }
                *out = pd;
                return 1;
            }
        }
    }
}
__attribute__((visibility("hidden"))) const bn_direct_t g_bn_direct[BID_TABSZ] = { [BID_PROTOTYPE] = { bn_prototype, 0, 0 }, [BID_SNOx24NAME] = { bn_sno_name_d, 0, 0 }, [BID_TABLE] = { bn_table, 0,
    0 }, [BID_CONVERT] = { bn_convert, 0, 0 }, [BID_SNOx24MKPAT] = { bn_mkpat, 0, 0 }, [BID_SNOx24WANTNM] = { bn_wantnm, 0, 0 }, [BID_ARRAY] = { bn_array, 0, 0 },
    [BID_SIZE] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_size, 0, 0 }, [BID_SUBSTR] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_substr, 0, 0 },
    [BID_REPLACE] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_replace, 0, 0 }, [BID_TRIM] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_trim, 0, 0 },
    [BID_DUPL] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_dupl, 0, 0 }, [BID_LPAD] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_lpad, 0, 0 },
    [BID_RPAD] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_rpad, 0, 0 }, [BID_REVERSE] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_reverse, 0, 0 },
    [BID_INTEGER] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_integer, 0, 0 }, [BID_REMDR] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_remdr, 0, 0 },
    [BID_DATE] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_date, 0, 0 }, [BID_TIME] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_time, 0, 0 },
    [BID_IDENT] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_identdiffer, 1, 0 }, [BID_DIFFER] = { (int (*)(DESCR_t *, int, DESCR_t *, int))bn_identdiffer, 0, 0 }, };
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *icn_variable_frame_cell(DESCR_t nm, char *stat, size_t statsz) {
    extern int core_icn_act_top(void);
    extern const char *core_icn_act_name(int);
    extern void *core_icn_act_base(int);
    extern int core_icn_act_np(int);
    extern int rt_proc_nparams(const char *);
    extern const char *rt_proc_pname(const char *, int);
    extern const char *rt_proc_loc_pname(const char *, int);
    extern int rt_proc_nlocals(const char *);
    extern const char *rt_proc_lname(const char *, int);
    extern int rt_proc_loff(const char *, int);
    if (stat && statsz) stat[0] = 0;
    if (nm.v != DT_S || !nm.s || nm.slen == 0 || nm.s[0] == 38) return (DESCR_t *)0;
    int lv = core_icn_act_top();
    const char *pn = core_icn_act_name(lv);
    char *base = (char *)core_icn_act_base(lv);
    size_t L = nm.slen;
    if (!pn || !base) return (DESCR_t *)0;
    int nt = core_icn_act_np(lv);
    if (nt <= 0) nt = rt_proc_nparams(pn);
    if (nt < 0) nt = 0;
    for (int k = 0; k < nt; k++) {
        const char *vn = rt_proc_pname(pn, k);
        if (!vn) vn = rt_proc_loc_pname(pn, k);
        if (vn && strlen(vn) == L && !memcmp(vn, nm.s, L)) { extern DESCR_t *core_icn_act_arg(int, int); return core_icn_act_arg(lv, k); }
    }
    for (int k = 0, nl = rt_proc_nlocals(pn); k < nl; k++) {
        const char *vn = rt_proc_lname(pn, k);
        int off = rt_proc_loff(pn, k);
        if (vn && off >= 0 && strlen(vn) == L && !memcmp(vn, nm.s, L)) return (DESCR_t *)(base + off);
    }
    if (stat && snprintf(stat, statsz, "%s__STATIC__%.*s", pn, (int)L, nm.s) < (int)statsz && !NV_EXISTS_fn(stat)) stat[0] = 0;
    return (DESCR_t *)0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *rt_icn_local_cell(const char *name, size_t len) { DESCR_t nm = { .v = DT_S, .slen = (uint32_t) len, .s = (char *) name }; return icn_variable_frame_cell(nm, NULL, 0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t *rt_icn_local_cell_in(const char *name, size_t len, const char *pn, char *base) {
    extern int rt_proc_nlocals(const char *);
    extern const char *rt_proc_lname(const char *, int);
    extern int rt_proc_loff(const char *, int);
    if (!name || !len || name[0] == 38 || !pn || !base) return (DESCR_t *) 0;
    for (int k = 0, nl = rt_proc_nlocals(pn); k < nl; k++) {
        const char * vn = rt_proc_lname(pn, k);
        int off = rt_proc_loff(pn, k);
        if (vn && off >= 0 && strlen(vn) == len && !memcmp(vn, name, len)) return (DESCR_t *) (base + off);
    }
    {
        extern int rt_proc_nparams(const char *);
        extern const char * rt_proc_pname(const char *, int);
        extern long rt_icn_proc_args_off(const char *);
        long ao = rt_icn_proc_args_off(pn);
        for (int k = 0, np = rt_proc_nparams(pn); ao >= 0 && k < np; k++) {
            const char * vn = rt_proc_pname(pn, k);
            if (vn && strlen(vn) == len && !memcmp(vn, name, len)) return (DESCR_t *) (base + ao + 16 * k);
        }
    }
    return (DESCR_t *) 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_rchar(int with_echo) {
    struct termios o, t;
    unsigned char c;
    int tty = tcgetattr(0, &o) == 0;
    if (tty) { t = o; t.c_lflag &= ~(tcflag_t)ICANON; if (with_echo) t.c_lflag |= ECHO; else t.c_lflag &= ~(tcflag_t)ECHO; tcsetattr(0, TCSANOW, &t); }
    ssize_t n = read(0, &c, 1);
    if (tty) tcsetattr(0, TCSANOW, &o);
    return n == 1 ? (int)c : -1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int icn_kbhit(void) {
    struct termios o, t;
    fd_set r;
    struct timeval tv;
    int tty = tcgetattr(0, &o) == 0;
    if (tty) { t = o; t.c_lflag &= ~(tcflag_t)ICANON; tcsetattr(0, TCSANOW, &t); }
    FD_ZERO(&r);
    FD_SET(0, &r);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int rv = select(1, &r, 0, 0, &tv);
    if (tty) tcsetattr(0, TCSANOW, &o);
    return rv > 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_iand_d(DESCR_t *args, int nargs) {
    (void) nargs;
    extern int rt_big_is(DESCR_t);
    extern DESCR_t rt_big_and(DESCR_t, DESCR_t);
    extern DESCR_t rt_bitop_operand(DESCR_t);
    DESCR_t _a0 = rt_bitop_operand(args[0]), _a1 = rt_bitop_operand(args[1]);
    if (rt_big_is(_a0) || rt_big_is(_a1)) { return rt_big_and(_a0, _a1); }
    int64_t a=IS_INT_fn(_a0)?_a0.i:(int64_t)_a0.r, b=IS_INT_fn(_a1)?_a1.i:(int64_t)_a1.r;
    return INTVAL(a&b);
}
DESCR_t rt_ior_d(DESCR_t *args, int nargs) {
    (void) nargs;
    extern int rt_big_is(DESCR_t);
    extern DESCR_t rt_big_or(DESCR_t, DESCR_t);
    extern DESCR_t rt_bitop_operand(DESCR_t);
    DESCR_t _a0 = rt_bitop_operand(args[0]), _a1 = rt_bitop_operand(args[1]);
    if (rt_big_is(_a0) || rt_big_is(_a1)) { return rt_big_or(_a0, _a1); }
    int64_t a=IS_INT_fn(_a0)?_a0.i:(int64_t)_a0.r, b=IS_INT_fn(_a1)?_a1.i:(int64_t)_a1.r;
    return INTVAL(a|b);
}
DESCR_t rt_ixor_d(DESCR_t *args, int nargs) {
    (void) nargs;
    extern int rt_big_is(DESCR_t);
    extern DESCR_t rt_big_xor(DESCR_t, DESCR_t);
    extern DESCR_t rt_bitop_operand(DESCR_t);
    DESCR_t _a0 = rt_bitop_operand(args[0]), _a1 = rt_bitop_operand(args[1]);
    if (rt_big_is(_a0) || rt_big_is(_a1)) { return rt_big_xor(_a0, _a1); }
    int64_t a=IS_INT_fn(_a0)?_a0.i:(int64_t)_a0.r, b=IS_INT_fn(_a1)?_a1.i:(int64_t)_a1.r;
    return INTVAL(a^b);
}
DESCR_t rt_ishift_d(DESCR_t *args, int nargs) {
    (void) nargs;
    extern int rt_big_is(DESCR_t);
    extern DESCR_t rt_big_mul(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_div(DESCR_t, DESCR_t);
    extern DESCR_t rt_big_pow(DESCR_t, int64_t);
    extern DESCR_t rt_big_sub(DESCR_t, DESCR_t);
    extern DESCR_t rt_bitop_operand(DESCR_t);
    args[0] = rt_bitop_operand(args[0]);
    args[1] = rt_bitop_operand(args[1]);
    if (IS_FAIL_fn(args[0]) || IS_FAIL_fn(args[1]) || !(IS_INT_fn(args[1]))) { return FAILDESCR; }
    int64_t b=args[1].i;
    if (rt_big_is(args[0]) ||
        (b > 0 && IS_INT_fn(args[0]) && args[0].i != 0 && (b >= 63 || (args[0].i > 0 ? (args[0].i > (9223372036854775807LL >> b)) : (args[0].i < (-9223372036854775807LL - 1) >> b))))) {
        DESCR_t x = rt_big_is(args[0]) ? args[0] : INTVAL(args[0].i);
        if (b >= 0) { extern DESCR_t rt_big_shl(DESCR_t, int64_t); return rt_big_shl(x, b); }
        {
            DESCR_t p2 = rt_big_pow(INTVAL(2), -b);
            DESCR_t q = rt_big_div(x, p2);
            extern int rt_big_sign(DESCR_t);
            if (rt_big_sign(x) < 0 && !IS_FAIL_fn(rt_big_sub(x, rt_big_mul(q, p2))) && rt_big_sign(rt_big_sub(x, rt_big_mul(q, p2))) != 0) q = rt_big_sub(q, INTVAL(1));
            return q;
        }
    }
    int64_t a=IS_INT_fn(args[0])?args[0].i:(int64_t)args[0].r;
    return INTVAL(b>=0?(b>=64?0:a<<b):(-b>=64?(a<0?-1:0):a>>(-b)));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_unicon_name(DESCR_t a, DESCR_t * out) {
    if (a.v == DT_N && a.slen == 0 && a.s && *a.s) { *out = STRVAL(rt_heap_strdup_c(a.s)); return 1; }
    *out = FAILDESCR;
    if (!IS_NAMETRAP_fn(a) || !a.p) return 1;
    VCELL_t * vc = (VCELL_t *) a.p;
    if (vc->cellp && vc->key && IS_DATA_INST_fn(vc->sv) && vc->sv.u && vc->sv.u->type && vc->sv.u->fields && vc->cellp >= vc->sv.u->fields && vc->cellp < vc->sv.u->fields + vc->sv.u->type->nfields) {
        const char * tn = vc->sv.u->type->name ? vc->sv.u->type->name : "", * fnm = vc->sv.u->type->fields[vc->cellp - vc->sv.u->fields] ? vc->sv.u->type->fields[vc->cellp - vc->sv.u->fields] : "";
        *out = STRVAL(rt_heap_fmt("record %s_%ld.%s", tn, rt_record_image_id(vc->sv.u), fnm));
        return 1;
    }
    if (vc->key && *vc->key) { *out = STRVAL(rt_heap_strdup_c(vc->key)); return 1; }
    if (vc->tbl) {
        DESCR_t kn;
        int r = try_call_builtin_by_name("image", &vc->key_d, 1, &kn);
        *out = STRVAL(rt_heap_fmt("table_%ld[%s]", vc->tbl->id, (r && kn.v == DT_S && kn.s) ? kn.s : ""));
        return 1;
    }
    if (vc->cellp && vc->pos > 0 && vc->sv.v == DT_DATA && vc->sv.u) { *out = STRVAL(rt_heap_fmt("list_%ld[%ld]", rt_record_image_id(vc->sv.u), vc->pos)); return 1; }
    if (vc->cellp && vc->pos < 0 && vc->sv.u && vc->sv.u->type && vc->sv.u->type->fields && -vc->pos <= vc->sv.u->type->nfields) {
        const char * tn = vc->sv.u->type->name ? vc->sv.u->type->name : "", * fnm = vc->sv.u->type->fields[-vc->pos - 1] ? vc->sv.u->type->fields[-vc->pos - 1] : "";
        *out = STRVAL(rt_heap_fmt("record %s_%ld.%s", tn, rt_record_image_id(vc->sv.u), fnm));
        return 1;
    }
    if (vc->sv.v == DT_N && (vc->sv.slen == 0 || vc->sv.slen == 2) && vc->len > 0) {
        DESCR_t bn;
        int r = icn_unicon_name(vc->sv, &bn);
        *out = STRVAL(rt_heap_fmt("%s[%ld:%ld]", (r && bn.v == DT_S && bn.s) ? bn.s : "", vc->pos, vc->pos + vc->len));
    }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static DESCR_t bta_display(DESCR_t *args, int nargs, int _bop) {
    if (1) {
        extern int core_icn_act_top(void);
        extern const char *core_icn_act_name(int);
        extern void *core_icn_act_base(int);
        extern int rt_proc_nparams(const char *);
        extern const char *rt_proc_pname(const char *, int);
        extern void rt_icn_global_note(const char *);
        extern int rt_icn_global_count(void);
        extern const char *rt_icn_global_name(int);
        extern DESCR_t NV_GET_fn(const char *);
        FILE *fp = stderr;
        if (nargs >= 2 && (IS_FH_fn(args[1]) || IS_INT_fn(args[1]))) { FILE *q = fh_get((int)args[1].i); if (q) fp = q; }
        int top = core_icn_act_top();
        int want = top;
        if (nargs >= 1 && !IS_FAIL_fn(args[0]) && args[0].v != DT_SNUL) { long long w = (long long)to_int(args[0]); if (w < 0) { core_icn_error(205, args[0]); return FAILDESCR; } want = (int)w; }
        if (want > top) want = top;
        fflush(stdout);
        {
            extern DESCR_t kw_read(const char *);
            DESCR_t cur = kw_read("current"), im;
            if (try_call_builtin_by_name_bl("image", &cur, 1, &im, -1) && im.v == DT_S && im.s) fwrite(im.s, 1, descr_slen(im), fp);
            fputc('\n', fp);
        }
        fputc('\n', fp);
        for (int lv = top; lv > top - want && lv >= 1; lv--) {
            const char *pn = core_icn_act_name(lv);
            void *base = core_icn_act_base(lv);
            if (!pn) continue;
            fprintf(fp, "%s local identifiers:\n", pn);
            extern int rt_proc_nlocals(const char *);
            extern const char *rt_proc_lname(const char *, int);
            int nt = core_icn_act_np(lv);
            if (nt <= 0) nt = rt_proc_nparams(pn);
            if (nt < 0) nt = 0;
            int nl = rt_proc_nlocals(pn);
            if (nl < 0) nl = 0;
            for (int k = 0; k < nt + nl; k++) {
                extern const char *rt_proc_loc_pname(const char *, int);
                const char *vn = (k < nt) ? (rt_proc_pname(pn, k) ? rt_proc_pname(pn, k) : rt_proc_loc_pname(pn, k)) : rt_proc_lname(pn, k - nt);
                if (!vn) continue;
                if (vn[0] == 38) continue;
                DESCR_t v;
                {
                    extern int rt_proc_loff(const char *, int);
                    int off = (k < nt) ? -1 : rt_proc_loff(pn, k - nt);
                    if (k < nt) {
                        extern DESCR_t *core_icn_act_arg(int, int);
                        DESCR_t *ap = core_icn_act_arg(lv, k);
                        v = ap ? *ap : NULVCL;
                    } else if (base && off >= 0) v = *(DESCR_t *)((char *)base + off);
                    else v = NV_GET_fn(vn);
                }
                fprintf(fp, "   %s = ", vn);
                core_icn_display_image(fp, v);
                fputc('\n', fp);
            }
            {
                int gn2 = rt_icn_global_count();
                size_t pl = strlen(pn);
                for (int gi = 0; gi < gn2; gi++) {
                    const char *gn3 = rt_icn_global_name(gi);
                    if (!gn3) continue;
                    const char *st = strstr(gn3, "__STATIC__");
                    if (!st || (size_t)(st - gn3) != pl || strncmp(gn3, pn, pl)) continue;
                    DESCR_t sv = NV_GET_fn(gn3);
                    fprintf(fp, "   %s = ", st + 10);
                    core_icn_display_image(fp, sv);
                    fputc('\n', fp);
                }
            }
        }
        fputc('\n', fp);
        fputs("global identifiers:\n", fp);
        {
            int gn = rt_icn_global_count();
            int *ord = (int *)rt_wsb_alloc((size_t)(gn > 0 ? gn : 1) * sizeof(int));
            for (int i = 0; i < gn; i++) ord[i] = i;
            for (int i = 1; i < gn; i++) {
                int t = ord[i], j = i - 1;
                const char *tn = rt_icn_global_name(t);
                while (j >= 0 && strcmp(rt_icn_global_name(ord[j]), tn) > 0) { ord[j + 1] = ord[j]; j--; }
                ord[j + 1] = t;
            }
            for (int i = 0; i < gn; i++) {
                const char *gname = rt_icn_global_name(ord[i]);
                if (!gname || gname[0] == '&' || strstr(gname, "__STATIC__") || strstr(gname, "__icn_")) continue;
                DESCR_t v = NV_GET_fn(gname);
                if ((v.v == DT_SNUL || IS_FAIL_fn(v)) && (rt_proc_is_registered(gname) || !strcmp(gname, "main") || dat_find_type(gname) || icn_builtin_arity(gname) != ICN_ARITY_UNKNOWN)) {
                    extern DESCR_t rt_proc_value(const char *);
                    DESCR_t pv = rt_proc_value(gname);
                    if (!IS_FAIL_fn(pv) && pv.v != DT_SNUL) v = pv;
                }
                if (v.v == DT_SNUL || IS_FAIL_fn(v)) continue;
                fprintf(fp, "   %s = ", gname);
                core_icn_display_image(fp, v);
                fputc('\n', fp);
            }
        }
        fflush(fp);
        return NULVCL;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_display(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_display(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_ICNx24UPVAR(DESCR_t *args, int nargs) {
    DESCR_t _bo = FAILDESCR;
    DESCR_t *const out = &_bo;
    if (nargs == 1) {
        extern int rt_icn_scan_proc(const char **, const char **);
        const char * pn, * base;
        *out = FAILDESCR;
        if (args[0].v != DT_S || !args[0].s || args[0].slen == 0) return _bo;
        DESCR_t * cell = rt_icn_scan_proc(&pn, &base) ? rt_icn_local_cell_in(args[0].s, args[0].slen, pn, (char *) base) : (DESCR_t *) 0;
        if (cell) { return rt_deref(*cell); }
        { extern const char * NV_intern_name_n(const char *, size_t); const char * in = NV_intern_name_n(args[0].s, (size_t) args[0].slen); if (in && NV_EXISTS_fn(in)) *out = NV_GET_fn(in); }
        return _bo;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_ICNx24UPCAT(DESCR_t *args, int nargs) { extern DESCR_t pat_cat(DESCR_t, DESCR_t); return nargs == 2 ? pat_cat(args[0], args[1]) : FAILDESCR; }
static TBPAIR_t *utbl_live_at(TBBLK_t *t, unsigned oi) { return t->ord[oi].v == DT_RAW ? (TBPAIR_t *)0 : table_find_pair_d(t, t->ord[oi]); }
static void utbl_build(TBBLK_t *r, TBBLK_t *x, TBBLK_t *y, int keep_in_y) {
    for (unsigned oi = 0; oi < x->ord_len; oi++) { TBPAIR_t *e = utbl_live_at(x, oi); if (e && (table_has_d(y, e->key_descr) != 0) == keep_in_y) table_set_descr_d(r, e->key_descr, e->val); }
}
static DESCR_t utbl_op(const char *op, DESCR_t *args, int nargs) {
    if (nargs != 2) return (DESCR_t){ .v = RTX_NOT_HANDLED };
    if (args[0].v == DT_T && args[0].tbl && !args[0].tbl->is_set && args[1].v == DT_T && args[1].tbl && !args[1].tbl->is_set) {
        TBBLK_t *x = args[0].tbl, *y = args[1].tbl, *r = table_new();
        r->dflt = x->dflt;
        if (op[0] == '+') { utbl_build(r, x, x, 1); utbl_build(r, y, x, 0); } else utbl_build(r, x, y, op[0] == '*');
        return TABLE_VAL(r);
    }
    if (!(args[0].v == DT_T && args[0].tbl && args[0].tbl->is_set && args[1].v == DT_T && args[1].tbl && args[1].tbl->is_set)) {
        extern int icn_cset_operand_ok(DESCR_t v);
        extern int core_icn_error_text(int code, DESCR_t val, const char *msg);
        for (int k = 0; k < 2; k++) if (!icn_cset_operand_ok(args[k])) { core_icn_error_text(120, args[k], "two csets, two sets, or two tables expected"); return FAILDESCR; }
    }
    { extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int); return rt_num_arith(args[0], args[1], op[0] == '+' ? BINOP_CUNION : op[0] == '*' ? BINOP_CINTER : BINOP_CDIFF); }
}
DESCR_t bt_ICNx24UUNION(DESCR_t *args, int nargs) { return utbl_op("++", args, nargs); }
DESCR_t bt_ICNx24UINTER(DESCR_t *args, int nargs) { return utbl_op("**", args, nargs); }
DESCR_t bt_ICNx24UDIFF(DESCR_t *args, int nargs) { return utbl_op("--", args, nargs); }
static int icn_numeric_suffix_power(char c) {
    return (c == 'k' || c == 'K') ? 1 : (c == 'm' || c == 'M') ? 2 : (c == 'g' || c == 'G') ? 3 : (c == 't' || c == 'T') ? 4 : (c == 'p' || c == 'P') ? 5 : 0;
}
DESCR_t bt_ICNx24UNUMERIC(DESCR_t *args, int nargs) {
    if (nargs == 1 && args[0].v == DT_S && args[0].s && args[0].slen != 0xFFFFFFFFu) {
        const char *s = args[0].s;
        size_t n = args[0].slen;
        while (n && isspace((unsigned char) *s)) { s++; n--; }
        while (n && isspace((unsigned char) s[n - 1])) n--;
        const int pw = n >= 2 ? icn_numeric_suffix_power(s[n - 1]) : 0;
        if (pw) {
            size_t i = (s[0] == '+' || s[0] == '-') ? 1 : 0;
            const int neg = s[0] == '-';
            const size_t e = n - 1;
            long long v = 0;
            int dig = i < e, ok = 1;
            for (size_t j = i; dig && j < e; j++) dig = s[j] >= '0' && s[j] <= '9';
            for (; dig && ok && i < e; i++) ok = !__builtin_mul_overflow(v, 10LL, &v) && !__builtin_add_overflow(v, (long long) (s[i] - '0'), &v);
            for (int k = 0; dig && ok && k < pw; k++) ok = !__builtin_mul_overflow(v, 1024LL, &v);
            if (dig && ok) return INTVAL(neg ? -v : v);
            DESCR_t d = FAILDESCR, a0 = { .v = DT_S, .slen = (uint32_t) e, .s = s };
            if (dig && try_call_builtin_by_name_bl_s("numeric", &a0, 1, &d, -1, 0) && d.v != DT_FAIL) {
                extern DESCR_t rt_num_arith(DESCR_t, DESCR_t, int);
                for (int k = 0; k < pw; k++) d = rt_num_arith(d, INTVAL(1024), BINOP_MUL);
                return d;
            }
        }
    }
    DESCR_t out = FAILDESCR;
    try_call_builtin_by_name_bl_s("numeric", args, nargs, &out, -1, 0);
    return out;
}
DESCR_t bt_ICNx24USET(DESCR_t *args, int nargs) {
    TBBLK_t *tbl = set_new();
    DESCR_t *elems = NULL;
    int n = 0;
    if (nargs >= 1 && args[0].v == DT_DATA && rt_lv_is_list(args[0], &elems, &n)) {
        if (elems) for (int _i = 0; _i < n; _i++) table_set_descr_d(tbl, elems[_i], elems[_i]);
    } else for (int _i = 0; _i < nargs; _i++) table_set_descr_d(tbl, args[_i], args[_i]);
    return TABLE_VAL(tbl);
}
DESCR_t bt_ICNx24REFRESH(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        DESCR_t src = args[0];
        extern int core_icn_error(int code, DESCR_t val);
        if (src.v != DT_CO || !src.p) { core_icn_error(118, src); return FAILDESCR; }
        extern void *scrip_co_gc_root(void);
        if ((void *)src.p == scrip_co_gc_root()) { core_icn_error(215, src); return FAILDESCR; }
        extern void *scrip_coexpr_refresh(void *orig);
        DESCR_t d = {0};
        d.v = DT_CO;
        d.p = scrip_coexpr_refresh(src.p);
        return d;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_chdir(DESCR_t *args, int nargs) {
    if (nargs == 1) { const char *_d = VARVAL_fn(args[0]); if (!_d || chdir(_d) != 0) { return FAILDESCR; } return NULVCL; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_delay(DESCR_t *args, int nargs) {
    if (nargs >= 1) { long long _ms = IS_INT_fn(args[0]) ? (long long)args[0].i : 0; if (_ms > 0) usleep((useconds_t)(_ms * 1000)); return NULVCL; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_kbhit(DESCR_t *args, int nargs) { { fflush(stdout); return icn_kbhit() ? NULVCL : FAILDESCR; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_rename(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        const char *_ra = VARVAL_fn(args[0]);
        if (!_ra) _ra = "";
        const char *_rb = VARVAL_fn(args[1]);
        if (!_rb) _rb = "";
        if (rename(_ra, _rb) == 0) { return NULVCL; }
        return FAILDESCR;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
