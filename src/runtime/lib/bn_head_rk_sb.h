#ifndef BN_HEAD_RK_SB_H
#define BN_HEAD_RK_SB_H
#include <glob.h>
#include "runtime/rt/rt.h"
#include "parsers/prolog/pl_cell.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { char *b; size_t n, cap; } rk_sb_t;
#ifdef __cplusplus
}
#endif
#endif
