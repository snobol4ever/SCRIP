#include "runtime/lib/bn_head.h"
#include "runtime/core/trace.h"
#include "runtime/rt/rt_arena.h"
#include "driver/driver_data.h"
#include "runtime/builtin_leaf.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_15_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_20_pl.h"
const char *rk_param_sigil(const char *pn) {
    size_t n = pn ? strlen(pn) : 0;
    return (n && (pn[0] == '@' || pn[0] == '%' || pn[0] == '&')) ? (pn[0] == '@' ? "@" : pn[0] == '%' ? "%" : "&") : (n > 6 && !strcmp(pn + n - 6, "__code")) ? "&" : "$";
}
DESCR_t rk_mk_metaval(const char *proto, const char *a, const char *b) {
    DESCR_t el[2] = { STRVAL(rt_heap_strdup_c(a)), STRVAL(rt_heap_strdup_c(b ? b : "")) };
    DESCR_t d = rk_mk_arr(el, b ? 2 : 1);
    ((ARBLK_t *) d.arr)->proto = proto;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_dat_index(const char *name) {
    extern int dat_type_count(void);
    extern const char *dat_type_name(int i);
    int n = dat_type_count();
    for (int i = 0; name && i < n; i++) { const char *t = dat_type_name(i); if (t && !strcmp(t, name)) return i; }
    return -1;
}
static int rk_field_inherited(const char **mro, int mn, int k, const char *fn) {
    extern int dat_type_nfields(int i);
    extern const char *dat_type_field(int i, int j);
    for (int q = k + 1; fn && q < mn; q++) { int ti = rk_dat_index(mro[q]); for (int j = 0; ti >= 0 && j < dat_type_nfields(ti); j++) if (!strcmp(dat_type_field(ti, j), fn)) return 1; }
    return 0;
}
DESCR_t rk_meta_list(const char *cn, char what, int local) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_type_nmethods(int i), dat_type_nfields(int i), dat_type_nroles(int i), dat_type_field_sigil(int i, int j);
    extern const char *dat_type_method_at(int i, int j), *dat_type_field(int i, int j), *dat_type_role_at(int i, int j);
    const char *mro[bnd_dat_need(cn, 0)];
    int mn = dat_mro(cn, mro, (int)(sizeof mro / sizeof mro[0]));
    if (mn == 0) { mro[0] = cn; mn = 1; }
    int mnall = mn;
    if (local) mn = 1;
    int cap = 1 + mn;
    for (int k = 0; k < mn; k++) {
        int ti = rk_dat_index(mro[k]);
        if (ti < 0) continue;
        cap += dat_type_nmethods(ti) + dat_type_nfields(ti) + 3;
        for (int r = 0; r < dat_type_nroles(ti); r++) { int ri = rk_dat_index(dat_type_role_at(ti, r)); if (ri >= 0) cap += dat_type_nmethods(ri); }
    }
    DESCR_t el[cap];
    int n = 0;
    for (int k = (what == 'p') ? 1 : 0; k < mn; k++) {
        if (what == 'p') { if (strcmp(mro[k], "Any") && strcmp(mro[k], "Mu")) el[n++] = rk_typeobj(mro[k]); continue; }
        int ti = rk_dat_index(mro[k]);
        if (ti < 0) continue;
        if (what == 'a') {
            for (int j = 0; j < dat_type_nfields(ti); j++) {
                int sg = dat_type_field_sigil(ti, j);
                const char *fn = dat_type_field(ti, j);
                if (rk_field_inherited(mro, mnall, k, fn)) continue;
                char nm[strlen(fn ? fn : "") + 3];
                snprintf(nm, sizeof nm, "%c!%s", sg == '@' || sg == '%' ? sg : '$', fn ? fn : "");
                el[n++] = rk_mk_metaval(rk_proto_attrval, sg == '@' ? "Positional" : sg == '%' ? "Associative" : "Mu", nm);
            }
            continue;
        }
        for (int j = 0; j < dat_type_nmethods(ti); j++) el[n++] = rk_mk_metaval(rk_proto_methval, dat_type_method_at(ti, j), NULL);
        for (int j = 0; j < dat_type_nfields(ti); j++) {
            extern int dat_type_field_priv(int i, int j);
            const char *fn = dat_type_field(ti, j);
            int own = 0;
            for (int q = 0; fn && q < dat_type_nmethods(ti); q++) if (!strcmp(dat_type_method_at(ti, q), fn)) own = 1;
            if (fn && !own && !dat_type_field_priv(ti, j) && !rk_field_inherited(mro, mnall, k, fn)) el[n++] = rk_mk_metaval(rk_proto_methval, fn, NULL);
        }
        for (int r = 0; r < dat_type_nroles(ti); r++) {
            int ri = rk_dat_index(dat_type_role_at(ti, r));
            for (int j = 0; ri >= 0 && j < dat_type_nmethods(ri); j++) el[n++] = rk_mk_metaval(rk_proto_methval, dat_type_method_at(ri, j), NULL);
        }
        el[n++] = rk_mk_metaval(rk_proto_methval, "POPULATE", NULL);
        if (k > 0) el[n++] = rk_mk_metaval(rk_proto_methval, "POPULATE", NULL);
    }
    return rk_mk_arr(el, n);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int pas_move_side(DESCR_t a, DESCR_t i, DESCR_t **base, long long *k, long long *len) {
    if (a.v == DT_A && a.arr) {
        ARBLK_t *b = (ARBLK_t *) a.arr;
        if (b->ndim != 1 || !b->data) return 0;
        *base = b->data;
        *k = (IS_INT_fn(i) ? (long long) i.i : b->lo) - b->lo;
        *len = (long long) b->hi - b->lo + 1;
        return *k >= 0;
    }
    if (a.v == DT_PCELL && a.ptr && !a.src_node0) { *base = (DESCR_t *) a.ptr; *k = (int32_t) a.slen; *len = -1; return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int bnd_dat_need(const char *c, int what) {
    DatType *t = c ? dat_find_type(c) : (DatType *)0;
    int n = 0;
    if (t && what == 0) n = t->mro_len;
    else if (t && what == 1) n = t->nroles;
    else if (t && what == 2) n = t->nbuild_keys;
    else if (t) for (int i = 0; i < (t->mro_len > 0 ? t->mro_len : 1); i++) {
        DatType *k = dat_find_type(i ? t->mro[i] : t->name);
        if (!k) continue;
        n += what == 4 ? k->nfields : k->nmethods;
        if (what == 3) for (int r = 0; r < k->nroles; r++) { DatType *rl = dat_find_type(k->roles[r]); if (rl) n += rl->nmethods; }
    }
    return n > 1 ? n : 1;
}
long rt_tweak_open(DESCR_t *out, const char *cname, trace_pend_t *pend) {
    if (!out || !cname || !*cname || !pend || !IS_DATA_INST_fn(*out)) return 0;
    pend->args[0] = STRVAL(rt_heap_fmt("%s__TWEAK", cname));
    pend->args[1] = *out;
    pend->args[2] = NULVCL;
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_build(const char *cname, DESCR_t self, DESCR_t *named, int nnamed) {
    extern int dat_mro(const char *name, const char **out, int max);
    extern int dat_class_has_build(const char *cls);
    extern int dat_build_keys(const char *cls, const char **out, int max);
    extern int rt_proc_has_native_fn(const char *name);
    if (!cname || !*cname) return;
    const char *chain[bnd_dat_need(cname, 0)];
    int n = dat_mro(cname, chain, (int)(sizeof chain / sizeof chain[0]));
    if (n == 0) { chain[0] = cname; n = 1; }
    for (int i = n - 1; i >= 0; i--) {
        if (!dat_class_has_build(chain[i])) continue;
        char proc[fmt_len("%s__BUILD", chain[i])];
        snprintf(proc, sizeof proc, "%s__BUILD", chain[i]);
        if (!meth_is_user_proc(proc)) continue;
        const char *keys[bnd_dat_need(chain[i], 2)];
        int nk = dat_build_keys(chain[i], keys, (int)(sizeof keys / sizeof keys[0]));
        DESCR_t callargs[nk + 1];
        callargs[0] = self;
        for (int k = 0; k < nk; k++) {
            DESCR_t v = NULVCL;
            for (int ci = 0; ci + 1 < nnamed; ci += 2) { const char *kn = VARVAL_fn(named[ci]); if (kn && keys[k] && !strcmp(kn, keys[k])) { v = named[ci + 1]; break; } }
            callargs[1 + k] = v;
        }
        int total = 1 + nk;
        int pi;
        for (pi = 0; pi < g_stage2.proc_count; pi++) if (g_stage2.proc_table[pi].name && !strcmp(g_stage2.proc_table[pi].name, proc)) break;
        if (pi >= g_stage2.proc_count || rt_proc_has_native_fn(proc)) {
            extern DESCR_t rt_c2bb_bomb(const char *site, const char *name);
            rt_c2bb_bomb("BUILD from C (a class the lowerer's .new helper sub did not see: the box calls BUILD, C never does)", proc);
        } else {
            extern DESCR_t ir_call_proc(int pix, DESCR_t *a, int na);
            ir_call_proc(pi, callargs, total);
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_exc_isa(DESCR_t d, const char *target) {
    if (!(IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name)) return 0;
    extern int dat_mro(const char *name, const char **out, int max);
    const char *cn = d.u->type->name;
    const char *mro[bnd_dat_need(cn, 0)];
    int mn = dat_mro(cn, mro, (int)(sizeof mro / sizeof mro[0]));
    if (mn == 0) { mro[0] = cn; mn = 1; }
    for (int i = 0; i < mn; i++) if (mro[i] && !strcmp(mro[i], target)) return 1;
    return 0;
}
DESCR_t bt___rk_when_match(DESCR_t *args, int nargs) {
    if (nargs >= 2) {
        const char * tn = VARVAL_fn(args[1]);
        if (tn && *tn && dat_find_type(tn)) {
            const char * cn = NULL;
            if (IS_DATA_INST_fn(args[0]) && args[0].u) {
                DATINST_t * di = (DATINST_t *)args[0].u;
                cn = (di && di->type) ? rk_dat_tname(di->type->name) : NULL;
            } else {
                const char * s0 = VARVAL_fn(args[0]);
                if (s0 && dat_find_type(s0)) cn = s0;
            }
            if (!cn) { return INTVAL(0); }
            extern int dat_mro(const char *name, const char **out, int max);
            extern int dat_roles(const char *name, const char **out, int max);
            int hit = 0;
            const char * mro[bnd_dat_need(cn, 0)];
            int mn = dat_mro(cn, mro, (int)(sizeof mro / sizeof mro[0]));
            if (mn == 0) { mro[0] = cn; mn = 1; }
            for (int i = 0; i < mn && !hit; i++) if (mro[i] && !strcmp(mro[i], tn)) hit = 1;
            for (int i = 0; i < mn && !hit; i++) {
                const char * rls[bnd_dat_need(mro[i], 1)];
                int rn = dat_roles(mro[i], rls, (int)(sizeof rls / sizeof rls[0]));
                for (int j = 0; j < rn && !hit; j++) if (rls[j] && !strcmp(rls[j], tn)) hit = 1;
            }
            return INTVAL(hit ? 1 : 0);
        }
        if (IS_INT_fn(args[0]) && IS_INT_fn(args[1])) { return INTVAL(args[0].i == args[1].i ? 1 : 0); }
        if ((IS_INT_fn(args[0]) || IS_REAL_fn(args[0])) && (IS_INT_fn(args[1]) || IS_REAL_fn(args[1]))) { return INTVAL(to_real(args[0]) == to_real(args[1]) ? 1 : 0); }
        { const char * a0 = VARVAL_fn(args[0]); const char * a1 = VARVAL_fn(args[1]); return INTVAL((a0 && a1 && !strcmp(a0, a1)) ? 1 : 0); }
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
