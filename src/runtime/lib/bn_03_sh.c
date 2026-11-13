#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
#include "runtime/core/nv.h"
#include <math.h>
#include "runtime/rt/rt_arena.h"
#include "runtime/builtins/gen_value.h"
#include "runtime/core/runtime_shim.h"
#include "driver/driver_globals.h"
#include "driver/driver_data.h"
#include "runtime/builtin_ids.h"
#include "runtime/builtin_leaf.h"
#include "runtime/rt/rt_list_view.h"
#include <ctype.h>
#include "runtime/lib/bn_01_sh.h"
#include "runtime/lib/bn_02_sh.h"
#include "runtime/lib/bn_03_sh.h"
#include "runtime/lib/bn_07_rk.h"
#include "runtime/lib/bn_08_sh.h"
#include "runtime/lib/bn_09_rk.h"
#include "runtime/lib/bn_11_pl.h"
#include "runtime/lib/bn_12_rk.h"
#include "runtime/lib/bn_13_rk.h"
#include "runtime/lib/bn_14_rk.h"
#include "runtime/lib/bn_16_rk.h"
#include "runtime/lib/bn_17_pas.h"
#include "runtime/lib/bn_19_rk.h"
#include "runtime/lib/bn_20_pl.h"
#include "runtime/lib/bn_21_sh.h"
#include "runtime/lib/bn_24_sh.h"
#include "runtime/lib/bn_25_sh.h"
#include "runtime/lib/bn_36_sh.h"
#include "runtime/lib/bn_41_sh.h"
extern void *rt_ws_alloc_descr(size_t);
extern const char *icon_real_str(double r, char *buf, int bufsz);
extern int rt_proc_is_registered(const char *name);
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int icn_builtin_is_known(const char *name) { return name && name[0] && bt_row_of(BT_VOC_ICN, name, (unsigned) strlen(name)) >= 0; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rt_builtin_is_known(const char *name) {
    if (!name) return 0;
    if (rt_proc_is_registered(name)) return 0;
    if (builtin_is_generator(name)) return 0;
    static const char known[][24] = { "write", "writes", "stop", "integer", "real", "string", "numeric", "char", "chr", "ord", "cset", "type", "image", "proc", "args", "copy", "abs", "sqrt", "sin",
        "cos", "tan", "exp", "log", "asin", "acos", "atan", "dtor", "rtod", "max", "min", "trim", "reverse", "repl", "map", "left", "center", "right", "detab", "entab", "read", "reads", "sprintf",
        "printf", "iand", "ior", "ixor", "ishift", "icom", "table", "list", "set", "sort", "sortf", "get", "pop", "pull", "member", "insert", "delete", "key", "[]", "__apply__", "MAKELIST",
        "__rk_arr", "__rk_arr_lit", "arr_get", "arr_set_pure", "__rk_arr_set", "arr_init", "arr_last", "array_sort", "array_reverse", "arr_make", "arr_slice", "__rk_arr_xx", "__rk_arr_at",
        "__rk_seq_new", "__rk_iter_has", "__rk_take_dyn", "__rk_seq_plan", "__rk_seq_next", "__rk_seq_stop", "__rk_seq_arity", "__rk_arr_sort", "__rk_arr_min", "__rk_arr_max", "__rk_arr_first",
        "__rk_first_shape", "__rk_unpack_named", "__rk_exc_umsg", "__rk_die_m", "__rk_exc_umeth", "__rk_failure", "__rk_sink", "__rk_shift_val", "__rk_subscr_adv", "__rk_tl_got", "__rk_is_code",
        "__rk_run_child", "__rk_arr_map", "__rk_arr_grep", "__rk_arr_reduce", "__rk_iter_src", "__rk_map_append", "__rk_grep_append", "__rk_iter_done", "__rk_sort_by_keys", "__rk_hyper_meth",
        "__rk_hyper_zip", "__rk_hyper_shape", "__rk_adjacent_pairs", "__rk_all_true", "__rk_regex", "__rk_smartmatch", "__rk_arr_keys", "__rk_hkeys", "__rk_arr_values", "__rk_arr_flat",
        "__rk_arr_slip", "__rk_flat", "__rk_to_array", "__rk_to_hash", "__rk_pre", "__rk_eval", "__rk_rethrow", "re_test", "__rk_io", "__rk_set_uni", "__rk_set_int", "__rk_set_dif", "__rk_set_sym",
        "__rk_set_sum", "__rk_set_mul", "__rk_set_nelem", "__rk_set_ncont", "__rk_set_sub", "__rk_set_nsub", "__rk_set_psub", "__rk_set_npsub", "__rk_set_sup", "__rk_set_nsup", "__rk_set_psup",
        "__rk_set_npsup", "__rk_set_eq", "__rk_set_ne", "__rk_bag_sub", "__rk_bag_sup", "sleep", "val", "__rk_arr_kv", "__rk_range_arr", "__rk_arr_slice", "__rk_arr_pick", "__rk_reduce_add",
        "__rk_reduce_sub", "__rk_reduce_mul", "__rk_reduce_cat", "__rk_reduce_min", "__rk_reduce_max", "__rk_div", "__rk_pow", "__rk_ratlit", "__rk_ratnew", "__rk_exend", "__rk_str", "__rk_cross",
        "__rk_zip", "__rk_min", "__rk_max", "__rk_eqv", "__rk_substr_replace", "__rk_typeobj", "__rk_item1", "__rk_unset", "__rk_unany", "__rk_elem_put", "__rk_hyper_incdec", "__rk_radix_list",
        "__rk_radix_str", "__rk_unbase", "__rk_ident", "__rk_intdiv", "__rk_mod", "__rk_mkbool", "__rk_notbool", "__rk_cmp3", "__rk_cmpg", "__rk_leg", "__rk_when_match", "pick", "__rk_byref_assign",
        "__rk_deref", "__rk_not_smartmatch", "__rk_bor", "__rk_bxor", "__rk_lbor", "__rk_lbxor", "__rk_sbor", "__rk_sband", "__rk_gcd", "__rk_lcm", "__rk_after", "__rk_before", "__rk_approx",
        "__rk_xor", "__rk_coll", "__rk_unicmp", "__rk_set_elem", "__rk_set_cont", "rk_write", "rk_writes", "rk_write_arr", "rk_write_list", "__rk_named_call", "__rk_rep", "__rk_exit", "__pas_ca_pack",
        "__pas_ca_unpack", "__pas_ca_encode", "__pas_stdfile", "__pas_arr_copy", "__pas_arr_of", "__rk_hash", "elems", "push_pure", "unshift_pure", "append_pure", "prepend_pure", "arr_tail",
        "hash_get", "hash_set_pure", "hash_delete", "hash_exists", "hash_keys", "hash_values", "hash_pairs", "hash_kv", "__rk_jct_any", "__rk_jct_all", "__rk_jct_one", "__rk_jct_none", "obj_new",
        "__rk_new_alloc", "__rk_new_check", "__rk_new_defaults", "meth_call", "field_set", "field_set_pub", "field_get_pub", "__rk_say_capture", "__rk_say_named_capture", "__rk_gist_capture",
        "__rk_gist_named_capture", "nqp::create", "nqp::bindattr", "nqp::bindattr_n", "nqp::bindattr_i", "nqp::bindattr_s", "die", "script_die", "srand", "callsame", "nextsame", "callwith",
        "__multi_call", "__param_check", "__blk_ref", "__blk_close", "__blk_invoke", "__rk_box", "__rk_cell_of", "__rk_elem_ref", "__rk_elem_bind", "TIME", "DATE", "IDENTICAL", "getenv", "open",
        "where", "close", "collect", "seek", "LT", "LE", "GT", "GE", "EQ", "NE", "LGT", "LLT", "LGE", "LLE", "LEQ", "LNE", "IDENT", "DIFFER", "SIZE", "TRIM", "DUPL", "REPLACE", "REMDR", "SNO$NAME",
        "SUBSTR", "REVERSE", "LPAD", "RPAD", "INTEGER", "DATATYPE", "ARRAY", "TABLE", "ITEM", "PROTOTYPE", "CONVERT", "DATA", "APPLY", "OPSYN", "VALUE", "SNO$KWSET", "SNO$NRET", "SNO$WANTNM",
        "SNO$BYNAME", "SNO$CHKNM", "SNO$PATCK", "SNO$XNAME", "SNO$XLV", "SNO$XTHUNK", "EVAL", "SNO$MKEXPR", "SNO$MKPAT", "SNO$STMT", "$unify", "$unify_lst", "$ix_g", "__trace_stmt", "__trace_call",
        "__trace_return", "__trace_value", "__trace_tap_off", "" };
    for (int i = 0; known[i][0]; i++) if (!strcmp(known[i], name)) return 1;
    { if (dat_find_type(name)) return 1; }
    { extern int rt_dat_field_of_any(const char *); if (rt_dat_field_of_any(name)) return 1; }
    { extern const char *rt_builtin_synonym(const char *); if (rt_builtin_synonym(name)) return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char *rtos(double r, char *buf, size_t cap) {
    if (isnan(r)) { snprintf(buf, cap, "%s", "NaN"); return buf; }
    if (isinf(r)) { snprintf(buf, cap, "%s", r < 0 ? "-Inf" : "Inf"); return buf; }
    gcvt(r, 14, buf);
    (void)cap;
    return buf;
}
const char *rk_cstr(DESCR_t v) {
    if (IS_REAL_fn(v)) { char *b = rt_wsb_alloc(64); return rk_real_str(v.r, b, 64); }
    if (v.v == DT_A && v.arr) return rk_arr_text(v);
    if (v.v == DT_T && v.tbl) return rk_tbl_text(v);
    if (v.v == DT_DATA && rk_match_is(v)) return rk_match_text(v);
    if (v.v == DT_DATA && rk_qh_is(v)) return rk_qh_render(v, 0);
    if (v.v == DT_DATA && rk_io_is(v)) return rk_io_pathstr(v);
    if (IS_INT_fn(v) || v.v == DT_BOOL || v.v == DT_ORDER) { char *b = rt_wsb_alloc(32); return to_cstring(v, b, 32); }
    const char *s = to_cstring(v, NULL, 0);
    return s ? s : "";
}
const char rk_proto_seq[] = "Seq";
int rk_is_cell(DESCR_t d) { return d.v == DT_N && d.slen == 1 && d.ptr; }
int rk_is_item(DESCR_t v) { return v.v == DT_A && v.arr && (((ARBLK_t *) v.arr)->proto_bare & RK_ARR_ITEM); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_slot(const ARBLK_t *b, long k) { DESCR_t d = b->data[k]; return ((b->proto_bare & RK_ARR_BOUND) && !b->proto && rk_is_cell(d)) ? *(DESCR_t *) d.ptr : d; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
rk_av_t rk_av(DESCR_t v) {
    rk_av_t a = { 0, NULL };
    if (rk_is_seq(v)) { rk_seq_reify(&v, -1L); v = ((ARBLK_t *) v.arr)->data[3]; }
    if (v.v == DT_A && v.arr) {
        ARBLK_t *b = (ARBLK_t *) v.arr;
        a.n = b->hi - b->lo + 1;
        if (a.n < 0) a.n = 0;
        a.el = b->data;
        if (!(b->proto_bare & RK_ARR_BOUND) || b->proto || !a.n) return a;
        DESCR_t *d = (DESCR_t *) rt_ws_alloc_descr((size_t) a.n);
        for (int i = 0; i < a.n; i++) d[i] = rk_is_cell(b->data[i]) ? *(DESCR_t *) b->data[i].ptr : b->data[i];
        a.el = d;
        return a;
    }
    if (v.v == DT_SNUL || v.v == DT_FAIL || (IS_STR_fn(v) && !*rk_cstr(v))) return a;
    DESCR_t *d = (DESCR_t *) rt_ws_alloc_descr(1);
    d[0] = v;
    a.n = 1;
    a.el = d;
    return a;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_av_text(rk_av_t a, int i) { return (i < 0 || i >= a.n) ? "" : rk_cstr(a.el[i]); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_mk_arr(const DESCR_t *el, int n) {
    extern ARBLK_t *array_new(int lo, int hi);
    ARBLK_t *b = array_new(0, n - 1);
    for (int i = 0; i < n; i++) b->data[i] = el[i];
    b->proto = rk_proto_list;
    DESCR_t d = {0};
    d.v = DT_A;
    d.slen = 0;
    d.arr = b;
    return d;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_is_slip(DESCR_t v) { return v.v == DT_A && v.arr && ((ARBLK_t *) v.arr)->proto == rk_proto_slip; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_slip_undef(DESCR_t v) { return rk_is_slip(v) && ((ARBLK_t *) v.arr)->hi < ((ARBLK_t *) v.arr)->lo; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char rk_proto_range[] = "Range";
void rk_seq_reify(DESCR_t *sp, long need) {
    rk_cbh_t H = { g_rk_cbh_cur, { *sp, NULVCL } };
    g_rk_cbh_cur = &H;
    for (;;) { ARBLK_t *c = (ARBLK_t *) ((ARBLK_t *) H.d[0].arr)->data[3].arr; if (need >= 0 && (long) (c->hi - c->lo + 1) >= need) break; if (!rk_seq_pull(&H.d[0])) break; }
    g_rk_cbh_cur = H.prev;
    *sp = H.d[0];
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_jct_text(DESCR_t v) {
    rk_av_t a = rk_av(v);
    const char *fl = ((ARBLK_t *) v.arr)->proto;
    size_t fln = strlen(fl);
    const char **tx = a.n ? (const char **) rt_pvec_alloc((size_t) a.n) : NULL;
    size_t tot = fln + 3;
    for (int i = 0; i < a.n; i++) { tx[i] = rk_av_text(a, i); tot += strlen(tx[i]) + 2; }
    char *buf = rt_wsb_alloc(tot);
    size_t p = 0;
    memcpy(buf, fl, fln);
    p = fln;
    buf[p++] = '(';
    for (int i = 0; i < a.n; i++) { if (i) { buf[p++] = ','; buf[p++] = ' '; } size_t L = strlen(tx[i]); memcpy(buf + p, tx[i], L); p += L; }
    buf[p++] = ')';
    buf[p] = '\0';
    return buf;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_pair_text(DESCR_t v) {
    rk_av_t a = rk_av(v);
    const char *k = rk_av_text(a, 0), *w = rk_av_text(a, 1);
    size_t kl = strlen(k), wl = strlen(w);
    char *b = rt_wsb_alloc(kl + wl + 2);
    memcpy(b, k, kl);
    b[kl] = '\t';
    memcpy(b + kl + 1, w, wl);
    b[kl + 1 + wl] = '\0';
    return b;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_tbl_text(DESCR_t v) {
    TBBLK_t *t = (v.v == DT_T) ? v.tbl : NULL;
    size_t cap = 1;
    const char **tx = (t && t->size) ? (const char **) rt_pvec_alloc((size_t) t->size) : NULL;
    int c = 0;
    for (unsigned i = 0; t && i < t->ord_len; i++) {
        DESCR_t key = t->ord[i];
        if (key.v == DT_RAW) continue;
        TBPAIR_t *e = table_find_pair_d(t, key);
        if (!e) continue;
        const char *kt = rk_cstr(key), *vt = rk_cstr(e->val);
        size_t kl = strlen(kt), vl = strlen(vt);
        char *ln = rt_wsb_alloc(kl + vl + 2);
        memcpy(ln, kt, kl);
        ln[kl] = '\t';
        memcpy(ln + kl + 1, vt, vl);
        ln[kl + 1 + vl] = '\0';
        tx[c++] = ln;
        cap += kl + vl + 2;
    }
    char *buf = rt_wsb_alloc(cap);
    size_t p = 0;
    for (int i = 0; i < c; i++) { if (i) buf[p++] = '\n'; size_t L = strlen(tx[i]); memcpy(buf + p, tx[i], L); p += L; }
    buf[p] = '\0';
    return buf;
}
int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out) {
    if (!meth || !*meth) return 0;
    if ((recv.v == DT_DATA || recv.v == DT_I || recv.v == DT_BIG || recv.v == DT_R || recv.v == DT_S) && (meth[0] == 'R' || meth[0] == 'F' || meth[0] == 'n' || meth[0] == 'd' || recv.v == DT_DATA) &&
        rk_rat_dispatch(meth, &recv, margs, nmargs, out)) return 1;
    if (meth[0] == 'N' && meth[1] == 'F' && rk_str_norm_method(meth, recv, nmargs, out)) return 1;
    {
        int _br = bt_row_of(BT_VOC_RKM, meth, (unsigned)strlen(meth));
        if (_br >= 0 && g_bt_leaf[_br]) {
            DESCR_t _av[1 + nmargs];
            _av[0] = recv;
            for (int _i = 0; _i < nmargs; _i++) _av[1 + _i] = margs[_i];
            const DESCR_t _bv = g_bt_leaf[_br](_av, 1 + nmargs);
            if (_bv.v != RTX_NOT_HANDLED) { *out = _bv; return 1; }
        }
    }
    {
        const char *tn = rk_typeobj_name(recv);
        if (tn) {
            if (!strcmp(meth, "defined") || !strcmp(meth, "Bool") || !strcmp(meth, "so")) { *out = (DESCR_t){ .v = DT_BOOL, .i = 0 }; return 1; }
            if (!strcmp(tn, "Bool") && (!strcmp(meth, "pick") || !strcmp(meth, "roll")) && nmargs <= 1) {
                if (nmargs == 0) { *out = (DESCR_t){ .v = DT_BOOL, .i = rand() & 1 }; return 1; }
                if (!rk_int_or_rat(margs[0])) return 0;
                long cnt = (long) rk_int_arg(margs[0], 0);
                if (cnt < 0) cnt = 0;
                if (meth[0] == 'p' && cnt > 2) cnt = 2;
                DESCR_t *r = cnt > 0 ? (DESCR_t *) rt_ws_alloc_descr((size_t) cnt) : NULL;
                long first = rand() & 1;
                for (long i = 0; i < cnt; i++) r[i] = (DESCR_t){ .v = DT_BOOL, .i = meth[0] == 'p' ? (i ? !first : first) : (rand() & 1) };
                *out = rk_mk_arr(r, (int) cnt);
                return 1;
            }
            if (!strcmp(meth, "gist") && !strcmp(tn, "Nil")) { *out = STRVAL(rt_heap_strdup_c("Nil")); return 1; }
            if (!strcmp(meth, "gist")) {
                const char *sc = strrchr(tn, ':');
                if (sc) tn = sc + 1;
                size_t l = strlen(tn);
                char *r = (char *)rt_str_alloc(l + 2);
                r[0] = '(';
                memcpy(r + 1, tn, l);
                r[l + 1] = ')';
                r[l + 2] = '\0';
                *out = STRVAL(r);
                return 1;
            }
            if (!strcmp(meth, "raku") || !strcmp(meth, "perl") || !strcmp(meth, "^name") || !strcmp(meth, "name")) { *out = STRVAL(rt_heap_strdup_c(tn)); return 1; }
            if (!strcmp(meth, "Str")) { *out = STRVAL(rt_heap_strdup_c("")); return 1; }
            if (!strcmp(meth, "WHAT")) { *out = recv; return 1; }
        }
    }
    if (recv.v == DT_BOOL && nmargs == 0 && (!strcmp(meth, "Int") || !strcmp(meth, "Num") || !strcmp(meth, "Numeric") || !strcmp(meth, "Real") || !strcmp(meth, "abs") || !strcmp(meth, "sum"))) recv =
        INTVAL(recv.i);
    if (nmargs == 0 && (!strcmp(meth, "pairs") || !strcmp(meth, "antipairs")) && recv.v != DT_A && recv.v != DT_T && !rk_typeobj_name(recv) && (IS_INT_fn(recv) || IS_REAL_fn(recv) || IS_STR_fn(recv)))
        {
        DESCR_t p1 = meth[0] == 'p' ? rk_mk_pair(INTVAL(0), recv) : rk_mk_pair(recv, INTVAL(0));
        *out = rk_mk_arr(&p1, 1);
        return 1;
    }
    if (nmargs == 0 && (!strcmp(meth, "Numeric") || !strcmp(meth, "Real")) && (IS_INT_fn(recv) || IS_REAL_fn(recv))) { *out = recv; return 1; }
    if (recv.v == DT_A) {
        if (nmargs == 0 && !rk_is_pair(recv) && !rk_is_jct(recv) && (!strcmp(meth, "Numeric") || !strcmp(meth, "Int") || !strcmp(meth, "Real"))) { *out = INTVAL(rk_av(recv).n); return 1; }
        if (nmargs == 0 && !rk_is_pair(recv) && !rk_is_jct(recv) && !strcmp(meth, "Num")) { *out = (DESCR_t){ .v = DT_R, .r = (double) rk_av(recv).n }; return 1; }
        if (!rk_is_pair(recv) && !rk_is_jct(recv) && rk_list_more_methods(meth, recv, margs, nmargs, out)) return 1;
        if (rk_is_pair(recv)) {
            rk_av_t pa = rk_av(recv);
            if (!strcmp(meth, "key")) { *out = pa.el[0]; return 1; }
            if (!strcmp(meth, "value")) { *out = pa.el[1]; return 1; }
            if (!strcmp(meth, "kv")) { DESCR_t kv2[2]; kv2[0] = pa.el[0]; kv2[1] = pa.el[1]; *out = rk_mark_list(rk_mk_arr(kv2, 2)); return 1; }
        }
        if (!strcmp(meth, "gist")) { *out = STRVAL((char *) rk_gist_str(recv)); return 1; }
        if (!strcmp(meth, "Str")) { *out = STRVAL((char *) rk_arr_text(recv)); return 1; }
        if (!strcmp(meth, "raku") || !strcmp(meth, "perl")) { *out = STRVAL((char *) rk_raku_str(recv, 0)); return 1; }
        if (!strcmp(meth, "elems")) { *out = INTVAL(rk_av(recv).n); return 1; }
        if (nmargs == 0 && !strcmp(meth, "hash")) { rk_av_t ha = rk_av(recv); *out = rk_is_pair(recv) ? rk_hash_from_list(&recv, 1) : rk_hash_from_list(ha.el, ha.n); return 1; }
        if (!rk_is_pair(recv) && (!strcmp(meth, "rotate") || !strcmp(meth, "pairs") || !strcmp(meth, "antipairs") || !strcmp(meth, "batch") || !strcmp(meth, "repeated") || !strcmp(meth, "squish"))) {
            rk_av_t a = rk_av(recv);
            DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
            int k = 0;
            if (!strcmp(meth, "rotate")) {
                long sh = nmargs >= 1 ? (long) rk_int_arg(margs[0], 1) : 1;
                for (int i = 0; i < a.n; i++) r[i] = a.el[(int) ((((i + sh) % a.n) + a.n) % a.n)];
                *out = rk_mk_arr(r, a.n);
                return 1;
            }
            if (!strcmp(meth, "pairs")) { for (int i = 0; i < a.n; i++) r[i] = rk_mk_pair(INTVAL(i), a.el[i]); *out = rk_mk_arr(r, a.n); return 1; }
            if (!strcmp(meth, "antipairs")) { for (int i = 0; i < a.n; i++) r[i] = rk_mk_pair(a.el[i], INTVAL(i)); *out = rk_mk_arr(r, a.n); return 1; }
            if (!strcmp(meth, "batch")) {
                if (nmargs >= 1 && IS_INT_fn(margs[0]) && margs[0].i < 1) {
                    rk_exc_init();
                    rk_exc_throw(DATCON_fn("X::OutOfRange", STRVAL(rt_heap_fmt("Batching sublist length is out of range. Is: %ld, should be in 1..^Inf", (long) margs[0].i)),
                        STRVAL(rt_heap_strdup_c("Batching sublist length")), INTVAL(margs[0].i), STRVAL(rt_heap_strdup_c("1..^Inf"))));
                    return 1;
                }
                long bs = (nmargs >= 1 && IS_INT_fn(margs[0]) && margs[0].i > 0) ? (long) margs[0].i : 1;
                int nb = (int) ((a.n + bs - 1) / bs);
                DESCR_t *rb = nb ? (DESCR_t *) rt_ws_alloc_descr((size_t) nb) : NULL;
                for (int b0 = 0; b0 < nb; b0++) { int lo = (int) (b0 * bs), hi = lo + (int) bs > a.n ? a.n : lo + (int) bs; rb[b0] = rk_mk_arr(a.el + lo, hi - lo); }
                *out = rk_mk_arr(rb, nb);
                return 1;
            }
            if (!strcmp(meth, "squish")) { for (int i = 0; i < a.n; i++) if (i == 0 || strcmp(rk_av_text(a, i), rk_av_text(a, i - 1))) r[k++] = a.el[i]; *out = rk_mk_arr(r, k); return 1; }
            for (int i = 0; i < a.n; i++) { int seen = 0; for (int j = 0; j < i && !seen; j++) if (!strcmp(rk_av_text(a, i), rk_av_text(a, j))) seen = 1; if (seen) r[k++] = a.el[i]; }
            *out = rk_mk_arr(r, k);
            return 1;
        }
        if (!strcmp(meth, "chrs") && nmargs == 0) {
            rk_av_t ca = rk_av(recv);
            char *cr = (char *)rt_str_alloc((size_t)ca.n * 4 + 1);
            size_t cl = 0;
            for (int i = 0; i < ca.n; i++) {
                DESCR_t ce = rk_av_elem(ca, i);
                char eb[64];
                unsigned cp = IS_INT_fn(ce) ? (unsigned)ce.i : (unsigned)atoll(to_cstring(ce, eb, sizeof eb));
                cl += rk_utf8_put(cp, cr + cl);
            }
            cr[cl] = '\0';
            *out = BSTRVAL(cr, cl);
            return 1;
        }
        if (!strcmp(meth, "join")) {
            char jb[64];
            const char *sep = "";
            if (nmargs >= 1) { sep = to_cstring(margs[0], jb, sizeof jb); if (!sep) sep = ""; }
            *out = STRVAL(rk_av_joined(rk_av(recv), sep, "", ""));
            return 1;
        }
    }
    char sb[64];
    const char *s = recv.v == DT_A ? rk_av_joined(rk_av(recv), " ", "", "") : IS_REAL_fn(recv) ? rk_real_str(recv.r, sb, (int)sizeof sb) : to_cstring(recv, sb, sizeof sb);
    if (!s) s = "";
    size_t n = strlen(s);
    {
        int _br = bt_row_of(BT_VOC_RKM, meth, (unsigned)strlen(meth));
        if (_br >= 0 && g_bt_leaf[_br]) {
            DESCR_t _av[1 + nmargs];
            _av[0] = recv;
            for (int _i = 0; _i < nmargs; _i++) _av[1 + _i] = margs[_i];
            const DESCR_t _bv = g_bt_leaf[_br](_av, 1 + nmargs);
            if (_bv.v != RTX_NOT_HANDLED) { *out = _bv; return 1; }
        }
    }
    if (!strcmp(meth, "trim-leading") || !strcmp(meth, "trim-trailing")) {
        size_t a = 0, b = n;
        if (meth[5] == 'l') { while (a < b && isspace((unsigned char)s[a])) a++; } else { while (b > a && isspace((unsigned char)s[b - 1])) b--; }
        char *r = (char *)rt_str_alloc(b - a);
        memcpy(r, s + a, b - a);
        r[b - a] = '\0';
        *out = STRVAL(r);
        return 1;
    }
    if (!strcmp(meth, "Str")) { *out = STRVAL(rt_heap_strdup_c(s)); return 1; }
    if (nmargs == 0 && !strcmp(meth, "gist") && rk_rx_pat(recv)) { *out = STRVAL(rt_heap_fmt("/%s/", rk_rx_pat(recv))); return 1; }
    if (nmargs == 0 && !strcmp(meth, "gist") && (IS_STR_fn(recv) || IS_INT_fn(recv) || IS_REAL_fn(recv) || recv.v == DT_BOOL || recv.v == DT_BIG)) {
        *out = STRVAL(rt_heap_strdup_c(recv.v == DT_SNUL ? "Nil" : s));
        return 1;
    }
    if (!strcmp(meth, "Int")) {
        if (IS_INT_fn(recv)) { *out = recv; return 1; }
        if (IS_REAL_fn(recv) && !isfinite(recv.r)) {
            char *cm = rt_heap_fmt("Cannot convert %s to Int", isnan(recv.r) ? "NaN" : recv.r < 0 ? "-Inf" : "Inf");
            rk_exc_init();
            *out = rk_failure_new(DATCON_fn("X::Numeric::CannotConvert", STRVAL(cm)));
            return 1;
        }
        if (IS_REAL_fn(recv)) { *out = INTVAL((long)recv.r); return 1; }
        *out = INTVAL((long)atoll(s));
        return 1;
    }
    if (!strcmp(meth, "chrs") && nmargs == 0 && recv.v != DT_A) { *out = recv; return 1; }
    if (!strcmp(meth, "abs")) {
        if (recv.v == DT_CPLX) {
            extern double rt_cplx_abs(DESCR_t);
            *out = REALVAL(rt_cplx_abs(recv));
        } else if (IS_INT_fn(recv)) {
            long v = (long)recv.i;
            *out = INTVAL(v < 0 ? -v : v);
        } else {
            *out = REALVAL(fabs(to_real(recv)));
        }
        return 1;
    }
    {
        static const struct {
            const char *nm;
            double (*f)(double);
        } rk_m1[] = { { "asin", asin }, { "acos", acos }, { "atan", atan }, { "sinh", sinh }, { "cosh", cosh }, { "tanh", tanh }, { "asinh", asinh }, { "acosh", acosh }, { "atanh", rk_atanh },
            { "log2", log2 }, { "log10", log10 }, { (const char *) 0, (double (*)(double)) 0 } };
        for (int mi = 0; rk_m1[mi].nm; mi++) if (!strcmp(meth, rk_m1[mi].nm) && nmargs == 0) { *out = REALVAL(rk_m1[mi].f(to_real(recv))); return 1; }
    }
    if (!strcmp(meth, "Num") && nmargs == 0 && (IS_INT_fn(recv) || IS_REAL_fn(recv))) { *out = REALVAL(to_real(recv)); return 1; }
    if (!strcmp(meth, "base") && nmargs == 1 && IS_INT_fn(recv)) {
        long long v = recv.i;
        int bs = (int) rk_int_arg(margs[0], 10);
        char buf[80];
        int k = 79;
        buf[k] = '\0';
        int neg = v < 0;
        unsigned long long u = neg ? -(unsigned long long) v : (unsigned long long) v;
        if (bs < 2 || bs > 36) return 0;
        do { buf[--k] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[u % bs]; u /= bs; } while (u && k > 1);
        if (neg) buf[--k] = '-';
        *out = STRVAL(rt_heap_strdup_c(buf + k));
        return 1;
    }
    if (!strcmp(meth, "polymod") && IS_INT_fn(recv) && nmargs >= 1) {
        long long v = recv.i;
        DESCR_t *r = (DESCR_t *) rt_ws_alloc_descr((size_t) nmargs + 2);
        int k = 0;
        for (int i = 0; i < nmargs && IS_INT_fn(margs[i]) && margs[i].i != 0; i++) { r[k++] = INTVAL(v % margs[i].i); v /= margs[i].i; }
        r[k++] = INTVAL(v);
        *out = rk_mk_arr(r, k);
        return 1;
    }
    if (!strcmp(meth, "polymod") && IS_INT_fn(recv) && nmargs >= 1 && margs) {
        long long v = recv.i;
        rk_av_t dv = rk_av_args((DESCR_t *) margs, 0, nmargs);
        DESCR_t *pr = (DESCR_t *) rt_ws_alloc_descr((size_t) dv.n + 1);
        int q = 0;
        for (int k = 0; k < dv.n; k++) {
            long long d = IS_INT_fn(dv.el[k]) ? (long long) dv.el[k].i : strtoll(rk_av_text(dv, k), NULL, 10);
            if (d == 0) return 0;
            long long r = v % d;
            if (r != 0 && ((r < 0) != (d < 0))) r += d;
            pr[q++] = INTVAL(r);
            v = (v - r) / d;
        }
        pr[q++] = INTVAL(v);
        *out = rk_mk_arr(pr, q);
        return 1;
    }
    if (!strcmp(meth, "Bool") || !strcmp(meth, "so") || !strcmp(meth, "not")) {
        int truthy;
        if (IS_INT_fn(recv)) truthy = (recv.i != 0);
        else if (IS_REAL_fn(recv)) truthy = (recv.r != 0.0);
        else truthy = (n > 0);
        *out = INTVAL(!strcmp(meth, "not") ? (truthy ? 0 : 1) : (truthy ? 1 : 0));
        return 1;
    }
    if (!strcmp(meth, "defined")) { *out = INTVAL(recv.v != DT_SNUL && !rk_slip_undef(recv) ? 1 : 0); return 1; }
    if (!strcmp(meth, "message") || !strcmp(meth, "payload")) { *out = STRVAL(rt_heap_strdup_c(s)); return 1; }
    if (!strcmp(meth, "raku") || !strcmp(meth, "perl")) {
        if (recv.v == DT_SNUL) { *out = STRVAL(rt_heap_strdup_c("Nil")); return 1; }
        if (recv.v == DT_BOOL) { *out = STRVAL(rt_heap_strdup_c(recv.i ? "Bool::True" : "Bool::False")); return 1; }
        if (IS_INT_fn(recv)) { *out = STRVAL(rt_heap_strdup_c(s)); return 1; }
        *out = STRVAL((char *) rk_raku_scalar(recv));
        return 1;
    }
    if ((!strcmp(meth, "Num") || !strcmp(meth, "Numeric")) && nmargs == 0 && recv.v != DT_A && !IS_INT_fn(recv) && !IS_REAL_fn(recv)) {
        char *ep;
        double dv = strtod(s, &ep);
        while (*ep == ' ') ep++;
        if (*s && (*ep == '\0')) { long lv = (long) dv; *out = (!strcmp(meth, "Numeric") && (double) lv == dv && !strpbrk(s, ".eEnN")) ? INTVAL(lv) : REALVAL(dv); return 1; }
    }
    if (!strcmp(meth, "elems")) { *out = INTVAL(s[0] ? 1 : 0); return 1; }
    if (!strcmp(meth, "trans") && nmargs >= 1) {
        if (rk_trans_has_rx(margs, nmargs)) return rk_trans_rx(STRVAL(rt_heap_strdup_c(s)), margs, nmargs, out);
        int np = 0, total = 0, ne = 0, flags = 0;
        for (int i = 0; i < nmargs; i++) {
            if (!rk_is_pair(margs[i])) continue;
            rk_av_t fp = rk_av(margs[i]);
            if (fp.n > 1 && !strcmp(rk_cstr(fp.el[0]), "__trans_flags")) { flags = (int) rk_ival(fp.el[1]); continue; }
            np++;
            total += rk_trans_items(fp.el[0], NULL, 0);
        }
        int fc = flags & 1, fd = flags & 2, fs = flags & 4;
        if (!np || total < 1) { char *r = (char *)rt_str_alloc(n); memcpy(r, s, n + 1); *out = STRVAL(r); return 1; }
        const char **from = (const char **) rt_pvec_alloc((size_t) total), **to = (const char **) rt_pvec_alloc((size_t) total), *clast = "";
        size_t *fromlen = (size_t *) rt_wsb_alloc(sizeof(size_t) * (size_t) total), *tolen = (size_t *) rt_wsb_alloc(sizeof(size_t) * (size_t) total), maxto = 1;
        int cdel = 0;
        for (int i = 0; i < nmargs; i++) {
            if (!rk_is_pair(margs[i])) continue;
            rk_av_t pp = rk_av(margs[i]);
            if (pp.n > 1 && !strcmp(rk_cstr(pp.el[0]), "__trans_flags")) continue;
            int nf = rk_trans_items(pp.el[0], NULL, 0), nt = rk_trans_items(pp.n > 1 ? pp.el[1] : NULVCL, NULL, 0);
            const char **F = (const char **) rt_pvec_alloc((size_t) (nf ? nf : 1)), **T = (const char **) rt_pvec_alloc((size_t) (nt ? nt : 1));
            rk_trans_items(pp.el[0], F, nf);
            rk_trans_items(pp.n > 1 ? pp.el[1] : NULVCL, T, nt);
            clast = nt ? T[nt - 1] : "";
            cdel = fd && nt == 0;
            for (int k = 0; k < nf && ne < total; k++, ne++) {
                from[ne] = F[k];
                to[ne] = k < nt ? T[k] : (fd ? NULL : (nt ? T[nt - 1] : ""));
                fromlen[ne] = strlen(from[ne]);
                tolen[ne] = to[ne] ? strlen(to[ne]) : 0;
                if (tolen[ne] > maxto) maxto = tolen[ne];
            }
        }
        size_t cl = strlen(clast);
        if (cl > maxto) maxto = cl;
        char *r = (char *)rt_str_alloc(n * maxto + n);
        size_t op = 0, i = 0, lastlen = 0;
        const char *lastrep = NULL;
        while (i < n) {
            int matched = -1;
            for (int k = 0; k < ne; k++) { if (fromlen[k] > 0 && i + fromlen[k] <= n && memcmp(s + i, from[k], fromlen[k]) == 0 && (matched < 0 || fromlen[k] > fromlen[matched])) matched = k; }
            size_t step = 1;
            if (matched >= 0) step = fromlen[matched];
            else { size_t ul; rk_utf8_cp((const unsigned char *) s + i, n - i, &ul); step = ul ? ul : 1; }
            const char *rep;
            size_t rl;
            int translated;
            if (fc) {
                translated = matched < 0;
                rep = cdel ? NULL : clast;
                rl = rep ? cl : 0;
            } else {
                translated = matched >= 0;
                rep = translated ? to[matched] : NULL;
                rl = rep ? tolen[matched] : 0;
            }
            if (!translated) { memcpy(r + op, s + i, step); op += step; i += step; lastrep = NULL; continue; }
            if (rep && !(fs && lastrep && lastlen == rl && !memcmp(lastrep, rep, rl))) { memcpy(r + op, rep, rl); op += rl; }
            lastrep = rep ? rep : lastrep;
            lastlen = rl;
            i += step;
        }
        r[op] = '\0';
        *out = STRVAL(r);
        return 1;
    }
    if (!strcmp(meth, "join")) { char *r = (char *)rt_str_alloc(n); memcpy(r, s, n + 1); *out = STRVAL(r); return 1; }
    return 0;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rt_fire_buildplan_tweak(const char *cname, DESCR_t self) { rt_fire_buildplan_tweak_rq(cname, self, (long *)0); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_real_str(double r, char *buf, int bufsz) {
    if (r == 0.0 && signbit(r)) { snprintf(buf, (size_t)bufsz, "-0"); return buf; }
    if (isfinite(r) && r == floor(r) && fabs(r) < 1e15) { snprintf(buf, (size_t)bufsz, "%lld", (long long)r); return buf; }
    if (isnan(r)) { snprintf(buf, (size_t)bufsz, "%s", "NaN"); return buf; }
    if (isinf(r)) { snprintf(buf, (size_t)bufsz, "%s", r < 0 ? "-Inf" : "Inf"); return buf; }
    for (int prec = 15; prec <= 17; prec++) { snprintf(buf, (size_t)bufsz, "%.*g", prec, r); if (strtod(buf, (char **)0) == r) return buf; }
    return buf;
}
DESCR_t rt_sno_wantnm_d(DESCR_t *args, int nargs) { extern int rt_g_want_name; (void)args; (void)nargs; rt_g_want_name = 1; return NULVCL; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_spill_count(DESCR_t v) { DESCR_t *la = 0; int ln = 0; if (rk_is_item(v)) return 1; if (v.v == DT_A && v.arr) return rk_av(v).n; if (rt_lv_is_list(v, &la, &ln)) return ln; return 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
static int rk_spill_scalar(DESCR_t *r, int k, DESCR_t v) { if (v.v == DT_SNUL) { r[k++] = rk_txt_elem("", 0); return k; } r[k++] = v; return k; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_item_count(DESCR_t v) { return rk_is_slip(v) ? rk_av(v).n : 1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_item(DESCR_t *r, int k, DESCR_t v) {
    DESCR_t *la = 0;
    int ln = 0;
    if (rk_is_slip(v)) return rk_spill(r, k, v);
    if (!(v.v == DT_A && v.arr) && rt_lv_is_list(v, &la, &ln)) {
        DESCR_t *e = ln ? (DESCR_t *) rt_ws_alloc_descr((size_t) ln) : NULL;
        int c = 0;
        for (int j = 0; j < ln; j++) c = rk_item(e, c, la[j]);
        r[k++] = rk_mk_arr(e, c);
        return k;
    }
    r[k++] = v;
    return k;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_spill(DESCR_t *r, int k, DESCR_t v) {
    DESCR_t *la = 0;
    int ln = 0;
    if (rk_is_item(v)) { r[k++] = v; return k; }
    if (v.v == DT_A && v.arr) { rk_av_t s = rk_av(v); for (int j = 0; j < s.n; j++) r[k++] = s.el[j]; return k; }
    if (rt_lv_is_list(v, &la, &ln)) { for (int j = 0; j < ln; j++) k = rk_spill_scalar(r, k, la[j]); return k; }
    return rk_spill_scalar(r, k, v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_one(DESCR_t v) { DESCR_t t[1]; if (rk_is_slip(v)) return v; rk_item(t, 0, v); return t[0]; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rk_mk_pair(DESCR_t k, DESCR_t v) {
    DESCR_t kv[2];
    kv[0] = IS_STR_fn(k) && !rk_typeobj_name(k) && !rk_is_slip(k) ? k : rk_one(k);
    kv[1] = IS_STR_fn(v) && !rk_typeobj_name(v) && !rk_is_slip(v) ? v : rk_one(v);
    DESCR_t p = rk_mk_arr(kv, 2);
    ((ARBLK_t *) p.arr)->proto = rk_proto_pair;
    return p;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_flat_count(DESCR_t v) {
    DESCR_t *la = 0;
    int ln = 0, c = 0;
    if (rk_is_item(v)) return 1;
    if (v.v == DT_A && v.arr) { rk_av_t a = rk_av(v); if (rk_is_list(v) || rk_is_slip(v)) { for (int i = 0; i < a.n; i++) c += rk_flat_count(a.el[i]); return c; } return a.n; }
    if (rt_lv_is_list(v, &la, &ln)) { for (int j = 0; j < ln; j++) c += rk_flat_count(la[j]); return c; }
    return 1;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_flat_into(DESCR_t *r, int k, DESCR_t v) {
    DESCR_t *la = 0;
    int ln = 0;
    if (rk_is_item(v)) { r[k++] = v; return k; }
    if (v.v == DT_A && v.arr) {
        rk_av_t a = rk_av(v);
        if (rk_is_list(v) || rk_is_slip(v)) { for (int i = 0; i < a.n; i++) k = rk_flat_into(r, k, a.el[i]); return k; }
        for (int i = 0; i < a.n; i++) r[k++] = a.el[i];
        return k;
    }
    if (rt_lv_is_list(v, &la, &ln)) { for (int j = 0; j < ln; j++) k = rk_flat_into(r, k, la[j]); return k; }
    return rk_spill_scalar(r, k, v);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs) {
    int cap = 0;
    for (int i = 0; i < nargs; i++) cap += rk_spill_count(args[i]);
    DESCR_t *r = cap ? (DESCR_t *) rt_ws_alloc_descr((size_t) cap) : NULL;
    int k = 0;
    for (int i = 0; i < nargs; i++) k = rk_spill(r, k, args[i]);
    return rk_unmark(rk_mk_arr(r, k));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
long pas_ord_of(DESCR_t v) { if (IS_INT_fn(v)) return (long)v.i; if (IS_STR_fn(v)) { const char *s = VARVAL_fn(v); return (s && s[0]) ? (long)(unsigned char)s[0] : -1L; } return -1L; }
void pas_set_bits(DESCR_t v, unsigned char out[PAS_SET_BYTES]) {
    memset(out, 0, PAS_SET_BYTES);
    if (IS_STR_fn(v) && v.slen == PAS_SET_BYTES && v.s) { memcpy(out, v.s, PAS_SET_BYTES); return; }
    if (IS_INT_fn(v)) { long iv = v.i; for (int b = 0; b < 64; b++) if ((iv >> b) & 1L) out[b / 8] |= (unsigned char)(1u << (b % 8)); }
}
const char *pas_cell_cstring(DESCR_t v, char *scratch, size_t scap) { return to_cstring(v, scratch, scap); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_positional_group(int n) { if (n < 0) return -1; for (int g = 0; g < g_match.ngroups; g++) if (!match_group_name(&g_match, g)[0] && n-- == 0) return g; return -1; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_io_is(DESCR_t d) { return IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && !strcmp(d.u->type->name, "IO::Path"); }
const char *rk_io_pathstr(DESCR_t d) { const char *t = VARVAL_fn(FIELD_GET_fn(d, "path")); return t ? t : ""; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_io_arg(DESCR_t d) { return rk_io_is(d) ? rk_io_pathstr(d) : rk_cstr(d); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int rk_io_flag(DESCR_t *kv, int nkv, const char *name, int dflt) { for (int i = 0; i + 1 < nkv; i += 2) if (!strcmp(rk_cstr(kv[i]), name)) return rk_is_truthy(kv[i + 1]); return dflt; }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void rk_io_write_args(FILE *f, DESCR_t *a, int n, int gist, int nl) { for (int i = 0; i < n; i++) out_write_descr(f, a[i], gist); if (nl) fputc('\n', f); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out) { return script_try_call_builtin_by_name_rq(fn, args, nargs, out, (long *)0); }
DESCR_t bt___rk_pair(DESCR_t *args, int nargs) { if (nargs == 2) { return rk_mk_pair(args[0], args[1]); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_hyper_incdec(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        rk_av_t a = rk_av(args[0]);
        long long dl = IS_INT_fn(args[1]) ? (long long) args[1].i : 1;
        DESCR_t *r = a.n ? (DESCR_t *) rt_ws_alloc_descr((size_t) a.n) : NULL;
        for (int i = 0; i < a.n; i++) {
            DESCR_t e = rk_av_elem(a, i);
            r[i] = IS_INT_fn(e) ? INTVAL((long long) e.i + dl) : IS_REAL_fn(e) ? REALVAL(e.r + (double) dl) : (e.v == DT_SNUL || rk_typeobj_name(e)) ? INTVAL(dl) : e;
        }
        return rk_unmark(rk_mk_arr(r, a.n));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_radix_str(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        extern DESCR_t rt_mul_big(DESCR_t, DESCR_t), rt_add_big(DESCR_t, DESCR_t);
        const char *ds = VARVAL_fn(args[1]);
        DESCR_t base = IS_INT_fn(args[0]) ? args[0] : INTVAL(10), acc = INTVAL(0);
        for (const char *q = ds ? ds : ""; *q; q++) {
            int c = (unsigned char) *q, dv = isdigit(c) ? c - '0' : isalpha(c) ? tolower(c) - 'a' + 10 : -1;
            if (dv < 0) continue;
            acc = rt_add_big(rt_mul_big(acc, base), INTVAL(dv));
        }
        return acc;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_radix_list(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        extern DESCR_t rt_mul_big(DESCR_t, DESCR_t), rt_add_big(DESCR_t, DESCR_t);
        rk_av_t a = rk_av(args[1]);
        DESCR_t base = IS_INT_fn(args[0]) ? args[0] : INTVAL(10), acc = INTVAL(0);
        for (int i = 0; i < a.n; i++) {
            DESCR_t d = a.el[i];
            if (IS_STR_fn(d) && d.s && d.s[0] && !d.s[1]) { int c = (unsigned char) d.s[0]; d = INTVAL(isdigit(c) ? c - '0' : isalpha(c) ? tolower(c) - 'a' + 10 : 0); }
            acc = rt_add_big(rt_mul_big(acc, base), IS_INT_fn(d) ? d : INTVAL(0));
        }
        return acc;
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_unbase(DESCR_t *args, int nargs) {
    if ((nargs == 2 || nargs == 3)) {
        extern DESCR_t rk_radix_value(int, const char *, int);
        extern void rt_script_die_surface(const char *msg);
        int rdx = IS_INT_fn(args[0]) ? (int) args[0].i : 10;
        const char *vs = VARVAL_fn(args[1]);
        if (nargs == 2 && !IS_STR_fn(args[1])) {
            char msg[strlen(vs ? vs : "") + 96];
            snprintf(msg, sizeof msg, "This call only converts base-%d strings to numbers; value %s cannot be converted!", rdx, vs ? vs : "");
            rt_script_die_surface(msg);
            return INTVAL(0);
        }
        return rk_radix_value(rdx, vs ? vs : "", nargs == 2);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_regex(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        char sb[64];
        const char *pat = to_cstring(args[0], sb, sizeof sb);
        size_t L = strlen(pat);
        char *r = rt_str_alloc((long)L + 1);
        r[0] = RK_RX;
        memcpy(r + 1, pat, L);
        r[L + 1] = '\0';
        return STRVAL(r);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_not_smartmatch(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        DESCR_t t = FAILDESCR, ar[2] = { args[0], args[1] };
        int hit = script_try_call_builtin_by_name("__rk_smartmatch", ar, 2, &t) && !IS_FAIL_fn(t) && rk_is_truthy(t);
        return (DESCR_t){ .v = DT_BOOL, .i = hit ? 0 : 1 };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta___rk_bor(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        long long x = IS_INT_fn(args[0]) ? args[0].i : (long long) to_real(args[0]), y = IS_INT_fn(args[1]) ? args[1].i : (long long) to_real(args[1]);
        return INTVAL((_bop == 0) ? (x | y) : (x ^ y));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_bor(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_bor(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_bxor(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_bor(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_lbor(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) { int x = rk_is_truthy(args[0]) ? 1 : 0, y = rk_is_truthy(args[1]) ? 1 : 0; return (DESCR_t){ .v = DT_BOOL, .i = (_bop == 0) ? (x | y) : (x ^ y) }; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_lbor(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_lbor(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_lbxor(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_lbor(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_sbor(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        const char *x = rk_cstr(args[0]), *y = rk_cstr(args[1]);
        size_t lx = strlen(x ? x : ""), ly = strlen(y ? y : ""), n = (_bop == 0) ? (lx > ly ? lx : ly) : (lx < ly ? lx : ly);
        int isor = (_bop == 0);
        char *b = (char *) rt_wsb_alloc(n + 1);
        for (size_t i = 0; i < n; i++) { unsigned char cx = i < lx ? (unsigned char) x[i] : 0, cy = i < ly ? (unsigned char) y[i] : 0; b[i] = (char) (isor ? (cx | cy) : (cx & cy)); }
        b[n] = '\0';
        return STRVAL(rt_heap_strdup_c(b));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_sband(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_sbor(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_sbor(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_sbor(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_gcd(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        long long x = IS_INT_fn(args[0]) ? args[0].i : (long long) to_real(args[0]), y = IS_INT_fn(args[1]) ? args[1].i : (long long) to_real(args[1]);
        if (x < 0) x = -x;
        if (y < 0) y = -y;
        long long a1 = x, b1 = y;
        while (b1) { long long t2 = a1 % b1; a1 = b1; b1 = t2; }
        return INTVAL((_bop == 0) ? a1 : (a1 ? x / a1 * y : 0));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_gcd(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_gcd(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_lcm(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_gcd(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_after(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 2) {
        long long c;
        int rc3 = 0;
        extern int rk_rat_cmp(DESCR_t, DESCR_t, int *);
        if (rk_rat_cmp(args[0], args[1], &rc3)) {
            c = rc3;
        } else if ((IS_INT_fn(args[0]) || IS_REAL_fn(args[0])) && (IS_INT_fn(args[1]) || IS_REAL_fn(args[1]))) {
            double x = to_real(args[0]), y = to_real(args[1]);
            c = x < y ? -1 : x > y;
        } else {
            const char *x = rk_cstr(args[0]), *y = rk_cstr(args[1]);
            int d = strcmp(x ? x : "", y ? y : "");
            c = d < 0 ? -1 : d > 0;
        }
        return (DESCR_t){ .v = DT_BOOL, .i = (_bop == 0) ? (c > 0) : (c < 0) };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_before(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_after(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_after(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_after(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_approx(DESCR_t *args, int nargs) {
    if (nargs == 2) {
        double x = to_real(args[0]), y = to_real(args[1]), m = fabs(x) > fabs(y) ? fabs(x) : fabs(y), d = fabs(x - y);
        return (DESCR_t){ .v = DT_BOOL, .i = (d < 1e-15 * m || d == 0.0) ? 1 : 0 };
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_xor(DESCR_t *args, int nargs) {
    if (nargs == 2) { int x = rk_is_truthy(args[0]) ? 1 : 0, y = rk_is_truthy(args[1]) ? 1 : 0; return (x && !y) ? args[0] : (y && !x) ? args[1] : (x && y) ? NULVCL : args[1]; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_coll(DESCR_t *args, int nargs) {
    if (nargs == 2) { const char *x = rk_cstr(args[0]), *y = rk_cstr(args[1]); int c = strcmp(x ? x : "", y ? y : ""); return (DESCR_t){ .v = DT_ORDER, .i = c < 0 ? -1 : c > 0 ? 1 : 0 }; }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_iter_done(DESCR_t *args, int nargs) { if (nargs == 1) { rk_av_t a = rk_av(args[0]); return rk_mk_arr(a.el, a.n); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_reduce_cat(DESCR_t *args, int nargs) { if (nargs >= 1) { return STRVAL(rk_av_joined(rk_av(args[0]), "", "", "")); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
static DESCR_t bta___rk_reduce_min(DESCR_t *args, int nargs, int _bop) { if (nargs >= 1) { return rk_av_minmax(rk_av(args[0]), (_bop == 1)); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_reduce_min(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_reduce_min(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_reduce_max(DESCR_t *args, int nargs) { { const DESCR_t _r = bta___rk_reduce_min(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_produce_init(DESCR_t *args, int nargs) {
    if (nargs == 1) { rk_av_t a = rk_av(args[0]); DESCR_t st[2] = { a.n ? rk_mk_arr(a.el, 1) : rk_mk_arr(NULL, 0), a.n ? a.el[0] : NULVCL }; return rk_mk_arr(st, 2); }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_produce_last(DESCR_t *args, int nargs) { if (nargs == 1) { rk_av_t s = rk_av(args[0]); return s.n == 2 ? s.el[1] : NULVCL; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt___rk_keyed_step(DESCR_t *args, int nargs) {
    if (nargs == 3) {
        rk_av_t s = rk_av(args[0]);
        if (s.n != 4) { return args[0]; }
        long mode = s.el[3].i;
        const char *kt = rk_cstr(args[2]);
        rk_av_t ka = rk_av(s.el[1]);
        int seen = 0;
        if (mode == 1) seen = s.el[2].v != 0 && !strcmp(rk_cstr(s.el[2]), kt);
        else for (int j = 0; j < ka.n && !seen; j++) if (!strcmp(rk_cstr(ka.el[j]), kt)) seen = 1;
        if (mode == 1) ((ARBLK_t *) args[0].arr)->data[2] = STRVAL(rt_heap_strdup_c(kt));
        else if (!seen) rk_arr_append((ARBLK_t *) s.el[1].arr, STRVAL(rt_heap_strdup_c(kt)));
        if (mode == 2 ? seen : !seen) rk_arr_append((ARBLK_t *) s.el[0].arr, args[1]);
        return args[0];
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
static DESCR_t bta_arr_init(DESCR_t *args, int nargs, int _bop) {
    if (nargs == 1) {
        if (args[0].v == DT_A && args[0].arr && !((ARBLK_t *) args[0].arr)->proto && ((ARBLK_t *) args[0].arr)->ndim == 1) {
            ARBLK_t *b = (ARBLK_t *) args[0].arr;
            int n = b->hi - b->lo + 1;
            if (n > 0) { if ((_bop == 1)) memmove(b->data, b->data + 1, (size_t) (n - 1) * sizeof(DESCR_t)); b->data[n - 1] = NULVCL; b->hi--; }
            return args[0];
        }
        rk_av_t a = rk_av(args[0]);
        return rk_unmark(a.n > 1 ? rk_mk_arr(a.el + ((_bop == 1) ? 1 : 0), a.n - 1) : rk_mk_arr(NULL, 0));
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt_arr_tail(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_arr_init(args, nargs, 1); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_arr_init(DESCR_t *args, int nargs) { { const DESCR_t _r = bta_arr_init(args, nargs, 0); if (_r.v != RTX_NOT_HANDLED) return _r; } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t bt_str_trim(DESCR_t *args, int nargs) {
    if (nargs == 1) {
        const char *s = rk_io_is(args[0]) ? rk_io_pathstr(args[0]) : VARVAL_fn(args[0]);
        if (!s) s = "";
        while (*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
        size_t n = strlen(s);
        while (n > 0 && (s[n-1] == ' ' || s[n-1] == '\t' || s[n-1] == '\n' || s[n-1] == '\r')) n--;
        char *o = rt_wsb_alloc(n + 1);
        memcpy(o, s, n);
        o[n] = '\0';
        return STRVAL(o);
    }
    return (DESCR_t){ .v = RTX_NOT_HANDLED };
}
DESCR_t bt___rk_cell_of(DESCR_t *args, int nargs) { if (nargs == 1 && rk_is_cell(args[0])) { return args[0]; } if (nargs == 1) { return rk_box_impl(args); } return (DESCR_t){ .v = RTX_NOT_HANDLED }; }
DESCR_t rt_call_arr_bl_sn4(const char *fn, DESCR_t *args, int nargs, int bidlen) { return RT_GC_CALLBACK(rt_call_arr_bl_sn4_rq(fn, args, nargs, bidlen, (long *)0)); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t rt_call_name_sn4(const char *fn, DESCR_t *args, int nargs, int bidlen) { return rt_call_name_sn4_rq(fn, args, nargs, bidlen, (long *)0); }
DESCR_t c_rt_call_bid_sn4(const char *fn, DESCR_t *args, int nargs, int bidlen) {
    if ((g_runtime.abi.error != 0 && !G_ERROR_IS_EVAL(g_runtime.abi.error)) || g_runtime.abi.kw_errlimit != 0 || bidlen < 0 || !(bidlen & BID_BAKE_LEAF))
        return RT_GC_CALLBACK(rt_call_arr_bl_sn4(fn, args, nargs, bidlen));
    { DESCR_t out = FAILDESCR; if (try_call_builtin_by_name_bl_s(fn, args, nargs, &out, bidlen, 0)) return out; }
    {
        extern int core_fn_arity_norm(const char *, int);
        int na = core_fn_arity_norm(fn, nargs);
        if (na != nargs) { DESCR_t pad[na > 0 ? na : 1]; for (int k = 0; k < na; k++) pad[k] = k < nargs ? args[k] : NULVCL; return c_rt_call_bid_sn4(fn, pad, na, bidlen); }
    }
    return RT_GC_CALLBACK(APPLY_fn(fn, args, nargs));
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
DESCR_t proc_as_value(const char *name) {
    if (!name || name[0] == '&') return FAILDESCR;
    for (int i = 0; i < g_stage2.proc_count; i++) {
        if (g_stage2.proc_table[i].name && strcmp(g_stage2.proc_table[i].name, name) == 0) { DESCR_t pv = {0}; pv.v = DT_E; pv.slen = (uint32_t)i; pv.i = g_stage2.proc_table[i].entry_pc; return pv; }
    }
    static const char builtins[][17] = { "__pas_writeln","__pas_write","__pas_chr","__pas_chrlit","__pas_enum_name","__pas_read_i","__pas_read_c","__pas_readln","__pas_eof","__pas_eoln",
        "__pas_trunc","__pas_abs","__pas_sin", "__pas_read_i_f","__pas_read_c_f","__pas_readln_f","__pas_eof_f","__pas_eoln_f","__pas_getbufch","__pas_getbufch_f", "__pas_ca_pack", "__pas_ca_unpack",
        "__pas_ca_encode","__pas_stdfile","__pas_arr_copy","__pas_arr_of", "__pas_cos","__pas_exp","__pas_sqrt","__pas_ln","__pas_arctan","__pas_fassign","__pas_rewrite", "__pas_reset","__pas_fclose",
        "__pas_std_mute", "__pas_copy", "__pas_upcase", "__pas_paramcount", "__pas_paramstr", "__pas_read_s", "__pas_read_s_f", "write","writes","read","reads","close","open","remove", "flush", "put",
        "get","pull","push","pop","list","image","proc","type","copy", "string","integer", "real","numeric", "ord","char", "reverse","sort","sortf", "find","match","many","any","upto","bal","move",
        "tab","pos", "map","repl","trim","left","right","center","detab","entab", "abs", "sqrt","sin","cos", "tan","asin", "acos","atan","exp","log", "dtor","rtod", "iand","ior","ixor","ishift",
        "icom", "table","key","insert","delete","member","args","level", "collect", "stop","exit","runerr","name", "variable", "seq", "chdir","delay","getch","getche","kbhit","loadfunc", "" };
    for (int i = 0; builtins[i][0]; i++) if (strcmp(builtins[i], name) == 0) return STRVAL(name);
    return FAILDESCR;
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void out_write_str(FILE *dest, const char *s) { if (!s || !*s) return; fputs(s, dest); }
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
const char *rk_obj_stringify(DESCR_t d, int use_gist) {
    { extern int rk_rat_is(DESCR_t); extern const char *rk_rat_str(DESCR_t, int); if (rk_rat_is(d)) return rk_rat_str(d, 0); }
    { const char *ms = rk_match_render(d, use_gist); if (ms) return ms; }
    if (IS_DATA_INST_fn(d) && d.u && d.u->type && rk_uni_is_name(d.u->type->name)) { DESCR_t ur; if (rk_uni_method(use_gist ? "gist" : "Str", d, 0, &ur)) return rk_cstr(ur); }
    if (IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name) {
        const char *mname = use_gist ? "gist" : "Str";
        const char *rmc = resolve_method_chain(d.u->type->name, mname, NULL);
        char proc[fmt_len("%s__%s", rmc, mname)];
        snprintf(proc, sizeof proc, "%s__%s", rmc, mname);
        if (meth_is_user_proc(proc)) { DESCR_t self1 = d; DESCR_t r = invoke_method_proc(proc, &self1, 1); const char *s = VARVAL_fn(r); return s ? s : ""; }
    }
    if (rk_exc_is(d)) return rk_exc_message(d);
    if (rk_qh_is(d)) return rk_qh_render(d, use_gist);
    if (rk_io_is(d)) { if (!use_gist) return rk_io_pathstr(d); char *o = rt_wsb_alloc(strlen(rk_io_pathstr(d)) + 16); sprintf(o, "\"%s\".IO", rk_io_pathstr(d)); return o; }
    if (use_gist && IS_DATA_INST_fn(d) && d.u && d.u->type && d.u->type->name && dat_find_type(d.u->type->name)) return rk_obj_default_raku(d);
    { const char *dt = rk_dyn_text(d, 0); if (dt) return dt; }
    const char *s = VARVAL_fn(d);
    return s ? s : "";
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void out_write_descr(FILE *dest, DESCR_t av, int use_gist) {
    if (IS_INT_fn(av)) { fprintf(dest, "%lld", (long long)av.i); return; }
    if (IS_REAL_fn(av)) { char _rb[64]; fprintf(dest, "%s", icon_real_str(av.r,_rb,sizeof _rb)); return; }
    if (IS_CSET_fn(av)) { if (av.s) fwrite(av.s, 1, descr_slen(av), dest); return; }
    if (av.v == (DTYPE_t)DT_PLREF || av.v == (DTYPE_t)DT_PLVAR) { extern void rt_pl_write_cell_fp(void *, FILE *); DESCR_t _pt = av; fflush(dest); rt_pl_write_cell_fp(plw_entry(&_pt), dest); return; }
    if (av.v == DT_DATA) { const char *s = rk_obj_stringify(av, use_gist); if (s) out_write_str(dest, s); return; }
    if (av.v == DT_A && av.arr) { out_write_str(dest, use_gist ? rk_gist_str(av) : rk_arr_text(av)); return; }
    if (av.v == DT_T && av.tbl) { out_write_str(dest, use_gist ? rk_gist_str(av) : rk_tbl_text(av)); return; }
    const char *s = VARVAL_fn(av);
    if (s) out_write_str(dest, s);
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen) { return try_call_builtin_by_name_bl_s(fn, args, nargs, out, bidlen, 0); }
int bn_sno_name_d(DESCR_t *args, int nargs, DESCR_t *out, int op) { (void)op; return nargs == 1 ? bn_sno_name(args, nargs, out) : 0; }
DESCR_t c_rt_sno_name_dl(DESCR_t *args, int nargs) { RT_HOOK_GUARD("SNO$NAME"); DESCR_t out = FAILDESCR; if (nargs == 1) bn_sno_name(args, nargs, &out); return out; }
const char *rt_sno_target_name(DESCR_t d) { return (d.v == DT_S && d.s && d.slen != 0xFFFFFFFFu && NV_name_needs_key(d.s, (size_t)d.slen)) ? NV_nul_key(d.s, (size_t)d.slen) : rt_cstr_d(d); }
DESCR_t rt_sno_pbk_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_mk_cset(int, const char *);
    extern int pat_cset_code(int);
    extern int pat_cset_arg_ok(int, DESCR_t);
    extern const char *pat_cset_text(DESCR_t);
    RT_HOOK_GUARD("SNO$PBK");
    return nargs == 2 && pat_cset_arg_ok(pat_cset_code((int)to_int(args[0])), args[1]) ? pat_mk_cset((int)to_int(args[0]), pat_cset_text(args[1])) : FAILDESCR;
}
DESCR_t rt_sno_pbn_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_mk_num(int, int64_t);
    extern int pat_num_arg_ok(int, DESCR_t);
    RT_HOOK_GUARD("SNO$PBN");
    return nargs == 2 && pat_num_arg_ok((int)to_int(args[0]), args[1]) ? pat_mk_num((int)to_int(args[0]), to_int(args[1])) : FAILDESCR;
}
DESCR_t rt_sno_pb0_d(DESCR_t *args, int nargs) { extern DESCR_t pat_mk_nil(int); RT_HOOK_GUARD("SNO$PB0"); return nargs == 1 ? pat_mk_nil((int)to_int(args[0])) : FAILDESCR; }
DESCR_t rt_sno_pbc_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_mk_capt(int, const char *, DESCR_t);
    RT_HOOK_GUARD("SNO$PBC");
    return nargs != 3 ? FAILDESCR : pat_mk_capt((int)to_int(args[0]), args[1].v == DT_I ? (const char *)(uintptr_t)args[1].i : rt_sno_target_name(args[1]), args[2]);
}
DESCR_t rt_sno_pcur_d(DESCR_t *args, int nargs) {
    extern DESCR_t pat_mk_cursor(const char *);
    RT_HOOK_GUARD("SNO$PCUR");
    return nargs != 1 ? FAILDESCR : args[0].v == DT_I ? pat_mk_cursor((const char *)(uintptr_t)args[0].i) : pat_mk_cursor(rt_sno_target_name(args[0]));
}
DESCR_t rt_sno_parb_d(DESCR_t *args, int nargs) { extern DESCR_t pat_arbno(DESCR_t); RT_HOOK_GUARD("SNO$PARB"); return nargs == 1 ? pat_arbno(args[0]) : FAILDESCR; }
DESCR_t rt_sno_pfen_d(DESCR_t *args, int nargs) { extern DESCR_t pat_fence_p(DESCR_t); RT_HOOK_GUARD("SNO$PFEN"); return nargs == 1 ? pat_fence_p(args[0]) : FAILDESCR; }
DESCR_t rt_sno_kwset_d(DESCR_t *args, int nargs) {
    extern int rt_keyword_write_snobol4(const char *sval, DESCR_t v);
    RT_HOOK_GUARD("SNO$KWSET");
    if (nargs != 2) return FAILDESCR;
    char kb[64];
    const char *kn = to_cstring(args[0], kb, sizeof kb);
    return rt_keyword_write_snobol4(kn ? kn : "", args[1]) ? args[1] : FAILDESCR;
}
DESCR_t rt_sno_mkexpr_d(DESCR_t *args, int nargs) { if (nargs != 1) return FAILDESCR; return rt_sno_mkexpr_of(args[0]); }
DESCR_t rt_sno_bynmark_d(DESCR_t *args, int nargs) {
    extern char *rt_str_alloc(long n);
    if (nargs != 1) return FAILDESCR;
    DESCR_t a = args[0];
    if (a.v != DT_S || !a.s) return a;
    long n = (a.slen && a.slen != 0xFFFFFFFFu) ? (long)a.slen : (long)strlen(a.s);
    char *o = rt_str_alloc(n + 1);
    o[0] = 1;
    memcpy(o + 1, a.s, (size_t)n);
    o[n + 1] = 0;
    return (DESCR_t){ .v = DT_S, .slen = (uint32_t)(n + 1), .s = o };
}
DESCR_t rt_sno_xthunk_d(DESCR_t *args, int nargs) {
    if (nargs != 1 || !IS_NAMETRAP_fn(args[0])) return FAILDESCR;
    VCELL_t *vc = (VCELL_t *)args[0].p;
    if (!vc || vc->cellp || vc->tbl || vc->key || vc->pos != -2) return FAILDESCR;
    return vc->sv;
}
DESCR_t rt_sno_xlv_d(DESCR_t *args, int nargs) {
    if (nargs != 1 || !IS_NAMETRAP_fn(args[0])) return FAILDESCR;
    VCELL_t *vc = (VCELL_t *)args[0].p;
    if (!vc || vc->cellp || vc->tbl || vc->key || vc->pos != -2 || IS_FAIL_fn(vc->key_d)) return FAILDESCR;
    return vc->key_d;
}
DESCR_t rt_sno_xname_d(DESCR_t *args, int nargs) {
    if (nargs != 1 && nargs != 2) return FAILDESCR;
    VCELL_t *vc = rt_agg_alloc(0, sizeof(VCELL_t));
    vc->cellp = 0;
    vc->tbl = 0;
    vc->key = 0;
    vc->key_d = nargs == 2 ? args[1] : FAILDESCR;
    vc->sv = args[0];
    vc->pos = -2;
    vc->len = 0;
    return NAMETRAP(vc);
}
DESCR_t rt_sno_stmt_d(DESCR_t *args, int nargs) {
    extern void rt_stmt_enter(long stno, long line);
    RT_HOOK_GUARD("SNO$STMT");
    if (nargs < 1 || nargs > 3) return FAILDESCR;
#if RT_DIAG
    if (!IS_INT(args[0]) && nargs == 1) { extern void rt_trace_label_hook(const char *); const char *lb = VARVAL_fn(args[0]); rt_trace_label_hook(lb ? lb : ""); return NULVCL; }
#else
    if (!IS_INT(args[0]) && nargs == 1) return NULVCL;
#endif
    long n = IS_INT(args[0]) ? (long)args[0].i : 0;
    long ln = (nargs >= 2 && IS_INT(args[1])) ? (long)args[1].i : 0;
    if (n >= 0) rt_stmt_enter(n, ln);
    if (nargs == 3) { extern void rt_stmt_file_init(const char *file); const char *fp = VARVAL_fn(args[2]); rt_stmt_file_init(fp ? fp : ""); }
    return NULVCL;
}
DESCR_t rt_sno_nofail_d(DESCR_t *args, int nargs) { extern void rt_nofail_abort(void); RT_HOOK_GUARD("SNO$NOFAIL"); rt_nofail_abort(); return FAILDESCR; }
DESCR_t rt_sno_list_d(DESCR_t *args, int nargs) {
    extern void output_val(DESCR_t v);
    RT_HOOK_GUARD("SNO$LIST");
    if (nargs == 1) {
        const char *sink = getenv("SCRIP_SNO_LIST_SINK");
        FILE *f = (sink && sink[0]) ? fopen(sink, "w") : NULL;
        if (f) { out_write_descr(f, args[0], 1); fputc('\n', f); fclose(f); } else output_val(args[0]);
    }
    return NULVCL;
}
int bn_prototype(DESCR_t *args, int nargs, DESCR_t *out, int op) { extern DESCR_t agg_prototype(DESCR_t); (void)op; *out = agg_prototype(nargs >= 1 ? args[0] : NULVCL); return 1; }
int bn_wantnm(DESCR_t *args, int nargs, DESCR_t *out, int op) { extern int rt_g_want_name; (void)args; (void)nargs; (void)op; rt_g_want_name = 1; *out = NULVCL; return 1; }
int bn_table(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    extern TBBLK_t *table_new_args(int, int);
    (void)op;
    if (nargs > 3) return 0;
    {
        TBBLK_t *tb = table_new_args(nargs >= 1 ? (int)to_int(args[0]) : 0, nargs >= 2 ? (int)to_int(args[1]) : 0);
        tb->null_one = 1;
        if (nargs >= 3) tb->dflt = args[2];
        { DESCR_t d; memset(&d, 0, sizeof d); d.v = DT_T; d.slen = 0; d.tbl = tb; *out = d; return 1; }
    }
}
int bn_convert(DESCR_t *args, int nargs, DESCR_t *out, int op) {
    (void)op;
    if (nargs != 2) return 0;
    {
        char tb[32];
        const char *ts = to_cstring(args[1], tb, sizeof tb);
        if (!ts) ts = "";
        const char * tu = ts;
        {
            DESCR_t a = args[0];
            extern DESCR_t rt_sno_cnv_num(DESCR_t, int);
            if (a.v == DT_S && a.s) a.s = (char *)rt_cstr_d(a);
            if (!strcmp(tu, "INTEGER")) { *out = rt_sno_cnv_num(a, 'I'); return 1; }
            if (!strcmp(tu, "REAL")) { *out = rt_sno_cnv_num(a, 'R'); return 1; }
            if (!strcmp(tu, "STRING")) { if (rt_str_arg_bad(a)) { *out = FAILDESCR; return 1; } const char *sv = VARVAL_fn(a); *out = STRVAL(rt_heap_strdup_c(sv ? sv : "")); return 1; }
            return 0;
        }
    }
}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int try_call_builtin_by_name_bl_s(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen, int strict) {
    return try_call_builtin_by_name_bl_s_rq(fn, args, nargs, out, bidlen, strict, (long *)0);
}
