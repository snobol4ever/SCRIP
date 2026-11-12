#ifndef RUNTIME_LIB_BN_22_RK_H
#define RUNTIME_LIB_BN_22_RK_H
#include "runtime/lib/bn_head.h"
void rk_rsprintf(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
void rk_rsprintf_legacy(const char *fmt, DESCR_t *args, int nargs, int from, char **outp, size_t *outlen);
#endif
