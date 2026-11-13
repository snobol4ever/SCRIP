#ifndef RUNTIME_LIB_BN_03_SH_H
#define RUNTIME_LIB_BN_03_SH_H
#include "runtime/lib/bn_head.h"
#include "runtime/by_name_dispatch.h"
int icn_builtin_is_known(const char *name);
int rt_builtin_is_known(const char *name);
__attribute__((visibility("hidden"))) char *rtos(double r, char *buf, size_t cap);
__attribute__((visibility("hidden"))) int rk_io_is(DESCR_t d);
__attribute__((visibility("hidden"))) const char *rk_io_pathstr(DESCR_t d);
__attribute__((visibility("hidden"))) DESCR_t rk_mk_arr(const DESCR_t *el, int n);
__attribute__((visibility("hidden"))) DESCR_t rk_mk_pair(DESCR_t k, DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_mk_pair_cell(DESCR_t k, DESCR_t cell);
DESCR_t bt___rk_elem_ref(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) const char *rk_real_str(double r, char *buf, int bufsz);
__attribute__((visibility("hidden"))) const char *rk_tbl_text(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_cstr(DESCR_t v);
extern __attribute__((visibility("hidden"))) const char rk_proto_seq[4];
__attribute__((visibility("hidden"))) void rk_seq_reify(DESCR_t *sp, long need);
__attribute__((visibility("hidden"))) int rk_is_cell(DESCR_t d);
__attribute__((visibility("hidden"))) int rk_is_item(DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_slot(const ARBLK_t *b, long k);
__attribute__((visibility("hidden"))) rk_av_t rk_av(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_av_text(rk_av_t a, int i);
__attribute__((visibility("hidden"))) int rk_is_slip(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_slip_undef(DESCR_t v);
extern __attribute__((visibility("hidden"))) const char rk_proto_range[6];
__attribute__((visibility("hidden"))) const char *rk_jct_text(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_pair_text(DESCR_t v);
__attribute__((visibility("hidden"))) const char *rk_real_str(double r, char *buf, int bufsz);
int rt_str_method(const char *meth, DESCR_t recv, const DESCR_t *margs, int nmargs, DESCR_t *out);
__attribute__((visibility("hidden"))) const char *pas_cell_cstring(DESCR_t v, char *scratch, size_t scap);
int try_call_builtin_by_name_bl(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen);
int try_call_builtin_by_name_bl_s(const char *fn, DESCR_t *args, int nargs, DESCR_t *out, int bidlen, int strict);
__attribute__((visibility("hidden"))) void out_write_descr(FILE *dest, DESCR_t av, int use_gist);
__attribute__((visibility("hidden"))) int rk_spill_count(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_spill(DESCR_t *r, int k, DESCR_t v);
__attribute__((visibility("hidden"))) int rk_item_count(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_item(DESCR_t *r, int k, DESCR_t v);
__attribute__((visibility("hidden"))) DESCR_t rk_one(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_flat_count(DESCR_t v);
__attribute__((visibility("hidden"))) int rk_flat_into(DESCR_t *r, int k, DESCR_t v);
DESCR_t rt_make_flat_agg(DESCR_t *args, int nargs);
__attribute__((visibility("hidden"))) long pas_ord_of(DESCR_t v);
__attribute__((visibility("hidden"))) void pas_set_bits(DESCR_t v, unsigned char out[PAS_SET_BYTES]);
__attribute__((visibility("hidden"))) int rk_positional_group(int n);
extern int script_try_call_builtin_by_name(const char *fn, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) const char *rk_io_arg(DESCR_t d);
__attribute__((visibility("hidden"))) int rk_io_flag(DESCR_t *kv, int nkv, const char *name, int dflt);
__attribute__((visibility("hidden"))) void rk_io_write_args(FILE *f, DESCR_t *a, int n, int gist, int nl);
DESCR_t rt_call_name_sn4(const char *fn, DESCR_t *args, int nargs, int bidlen);
DESCR_t c_rt_call_bid_sn4(const char *fn, DESCR_t *args, int nargs, int bidlen);
const char *rk_obj_stringify(DESCR_t d, int use_gist);
__attribute__((visibility("hidden"))) int bn_sno_name_d(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) const char *rt_sno_target_name(DESCR_t d);
__attribute__((visibility("hidden"))) int bn_prototype(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) int bn_wantnm(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) int bn_table(DESCR_t *args, int nargs, DESCR_t *out, int op);
__attribute__((visibility("hidden"))) int bn_convert(DESCR_t *args, int nargs, DESCR_t *out, int op);
#endif
