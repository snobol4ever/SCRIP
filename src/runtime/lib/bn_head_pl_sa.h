#ifndef BN_HEAD_PL_SA_H
#define BN_HEAD_PL_SA_H
#include <glob.h>
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *s; size_t blen; long n; int mode; long k0, k1; const char *sub; size_t sublen; long subn; long count; } pl_sa_t;
#ifdef __cplusplus
}
#endif
#endif
