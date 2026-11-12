#ifndef RUNTIME_LIB_BN_23_RK_H
#define RUNTIME_LIB_BN_23_RK_H
#include "runtime/lib/bn_head.h"
__attribute__((visibility("hidden"))) DESCR_t rk_io_mk(const char *path);
__attribute__((visibility("hidden"))) void rk_io_init(void);
__attribute__((visibility("hidden"))) int rk_io_fh_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out);
__attribute__((visibility("hidden"))) int rk_io_path_method(const char *m, DESCR_t *args, int nargs, DESCR_t *out);
#endif
