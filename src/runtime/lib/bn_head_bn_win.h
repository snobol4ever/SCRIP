#ifndef BN_HEAD_BN_WIN_H
#define BN_HEAD_BN_WIN_H
#include <glob.h>
#include "runtime/core/core_arithmetic_api.h"
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { const char *s; long n; int pos; uint64_t *env; } bn_win_t;
#ifdef __cplusplus
}
#endif
#endif
