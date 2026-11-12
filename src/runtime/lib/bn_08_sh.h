#ifndef RUNTIME_LIB_BN_08_SH_H
#define RUNTIME_LIB_BN_08_SH_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) const char *rk_typeobj_name(DESCR_t v);
__attribute__((visibility("hidden"))) const char *to_cstring(DESCR_t v, char *scratch, size_t scap);
__attribute__((visibility("hidden"))) double rk_atanh(double x);
__attribute__((visibility("hidden"))) unsigned rk_utf8_cp(const unsigned char *p, size_t n, size_t *len);
__attribute__((visibility("hidden"))) size_t rk_utf8_put(unsigned cp, char *o);
__attribute__((visibility("hidden"))) int rk_trans_items(DESCR_t side, const char **o, int cap);
__attribute__((visibility("hidden"))) DESCR_t rk_txt_elem(const char *t, size_t L);
__attribute__((visibility("hidden"))) char *rk_av_joined(rk_av_t a, const char *sep, const char *open, const char *close);
__attribute__((visibility("hidden"))) const char *rk_uni_str(const unsigned *c, int n);
__attribute__((visibility("hidden"))) int rk_seq_stop(DESCR_t v, DESCR_t ev, int kind, int dir, int gen_code);
__attribute__((visibility("hidden"))) int rk_arr_hole(DESCR_t e);
#endif
